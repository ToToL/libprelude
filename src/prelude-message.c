/*****
*
* Copyright (C) 2001 Yoann Vandoorselaere <yoann@mandrakesoft.com>
* All Rights Reserved
*
* This file is part of the Prelude program.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/uio.h>
#include <pthread.h>
#include <inttypes.h>
#include <netinet/in.h>

#include "common.h"
#include "prelude-io.h"
#include "prelude-list.h"
#include "prelude-message.h"


#define PRELUDE_MSG_VERSION 0
#define PRELUDE_MSG_HDR_SIZE 7



typedef struct {
        uint8_t version;
        uint8_t tag;
        uint8_t priority;
        uint32_t datalen;
} prelude_msg_hdr_t;



struct prelude_msg {
        PRELUDE_LINKED_OBJECT;
        
        uint32_t msg_index;
        prelude_msg_hdr_t hdr;
        unsigned char *payload;

        void (*destroy)(prelude_msg_t *msg);
};



static void msg_read_destroy(prelude_msg_t *msg) 
{
        if ( msg->payload )
                free(msg->payload);
        free(msg);
}




static void msg_write_destroy(prelude_msg_t *msg)
{
        free(msg);
}




static int read_message_header(prelude_msg_t *msg, prelude_io_t *src) 
{
        int ret;
        unsigned char buf[PRELUDE_MSG_HDR_SIZE];
        
        /*
         * Read the whole header.
         */
        ret = prelude_io_read(src, buf, sizeof(buf));
        if ( ret <= 0 )
                return -1;

        msg->hdr.version  = buf[0];
        msg->hdr.tag      = buf[1];
        msg->hdr.priority = buf[2];
        msg->hdr.datalen  = *((uint32_t *) &buf[3]);
        
        /*
         * Check protocol version.
         */
        if ( msg->hdr.version != PRELUDE_MSG_VERSION ) {
                log(LOG_ERR, "protocol used isn't the same : (use %d, recv %d).\n",
                    PRELUDE_MSG_VERSION, msg->hdr.version);
                return -1;
        }

        msg->hdr.datalen = ntohl(msg->hdr.datalen);
        
        return 0;
}




/**
 * prelude_msg_read_header:
 * @src: Pointer on a #prelude_io_t object representing the FD to get data from.
 *
 * prelude_msg_read_header() read the next availlable message header
 * on the file descriptor represented by @src. It will wait until enough
 * data are available then return.
 *
 * This function should only be used in combination with
 * prelude_msg_read_content().
 *
 * Returns: a #prelude_msg_t object representating the current message
 * or NULL if an error occured.
 */
prelude_msg_t *prelude_msg_read_header(prelude_io_t *src) 
{
        int ret;
        prelude_msg_t *msg;

        msg = malloc(sizeof(prelude_msg_t));
        if (! msg )
                return NULL;
        
        ret = read_message_header(msg, src);
        if ( ret < 0 ) {
                free(msg);
                return NULL;
        }
        
        msg->payload = NULL;
        msg->destroy = msg_read_destroy;
        
        return msg;
}



/**
 * prelude_msg_read_content:
 * @msg: Pointer on a #prelude_msg_t object to store the content into.
 * @src: Pointer on a #prelude_io_t object representating the FD to get data from.
 *
 * prelude_msg_read_content() read the next availlable message following
 * a header that should have been gathered using the prelude_msg_read_header()
 * function.
 *
 * The @msg object used as argument is the one returned by prelude_msg_read_header().
 *
 * Returns: 0 on success, -1 if an error occured.
 */
int prelude_msg_read_content(prelude_msg_t *msg, prelude_io_t *src) 
{
        int ret;

        assert(msg->payload == NULL);
        
        msg->payload = malloc(msg->hdr.datalen);
        if ( ! msg->payload ) 
                return -1;
        
        ret = prelude_io_read(src, msg->payload, msg->hdr.datalen);
        if ( ret <= 0 ) {
                free(msg->payload);
                return -1;
        }

        msg->msg_index = 0;

        return 0;
}




/**
 * prelude_msg_read:
 * @src: Pointer on a #prelude_io_t object representating the FD to read data from.
 *
 * prelude_msg_read() read the next available message (including the message
 * header), and store it into the returned #prelude_msg_t object.
 *
 * Returns: a #prelude_msg_t object, or NULL if an error occured.
 */
prelude_msg_t *prelude_msg_read(prelude_io_t *src) 
{
        int ret;
        prelude_msg_t *msg;

        msg = prelude_msg_read_header(src);
        if ( ! msg )
                return NULL;
        
        ret = prelude_msg_read_content(msg, src);
        if ( ret < 0 ) {
                free(msg);
                return NULL;
        }
        
        return msg;
}




/**
 * prelude_msg_get:
 * @msg: Pointer on a #prelude_msg_t object representing the message to get data from.
 * @tag: Pointer on a 8 bits unsigned integer to store the message tag.
 * @len: Pointer on a 32 bits unsigned integer to store the message len to.
 * @buf: Address of a pointer to store the buffer starting address.
 *
 * prelude_msg_get() read the next data chunk contained in the message.
 * @tag is updated to contain the kind of data the chunk contain.
 * @len is updated to contain the len of the data chunk.
 * @buf is updated to point on the data chunk.
 *
 * Returns: 1 on success, 0 if there is no more data chunk to read, or -1 if
 * an error occured.
 */
int prelude_msg_get(prelude_msg_t *msg, uint8_t *tag, uint32_t *len, void **buf) 
{
        if ( msg->msg_index == msg->hdr.datalen ) 
                return 0; /* no more sub - messages */
        
        if ( (msg->msg_index + PRELUDE_MSG_HDR_SIZE) > msg->hdr.datalen ) 
                return -1;
        
        *tag = msg->payload[msg->msg_index++];
        *len = *(uint32_t *)&msg->payload[msg->msg_index];
        *len = ntohl(*len);
        
        msg->msg_index += 4;
        
        if ( (msg->msg_index + *len + 1) > msg->hdr.datalen )
                return -1;
                
        *buf = &msg->payload[msg->msg_index];
        msg->msg_index += *len;

        /*
         * Skip end of message.
         */
        msg->msg_index++;
        
        return 1;
}



/**
 * prelude_msg_forward:
 * @msg: Pointer on a #prelude_msg_t object containing a message header.
 * @dst: Pointer on a #prelude_io_t object to send message to.
 * @src: Pointer on a #prelude_io_t object to read message from.
 *
 * prelude_msg_forward() read the message corresponding to the @msg object
 * containing the message header previously gathered using prelude_msg_read_header()
 * from the @src object, and transfer it to @dst. The header is also transfered.
 *
 * Returns: 0 on success, -1 if an error occured.
 */
int prelude_msg_forward(prelude_msg_t *msg, prelude_io_t *dst, prelude_io_t *src) 
{
        int ret;
        uint32_t dlen = htonl(msg->hdr.datalen);
        unsigned char buf[PRELUDE_MSG_HDR_SIZE];

        buf[0] = msg->hdr.version;
        buf[1] = msg->hdr.tag;
        buf[2] = msg->hdr.priority;
        buf[3] = dlen;
        buf[4] = dlen >> 8;
        buf[5] = dlen >> 16;
        buf[6] = dlen >> 24;
              
        ret = prelude_io_write(dst, buf, sizeof(buf));
        if ( ret < 0 )
                return -1;
        
        ret = prelude_io_forward(dst, src, msg->hdr.datalen);
        if ( ret < 0 )
                return -1;
        
        return 0;
}




/**
 * prelude_msg_write:
 * @msg: Pointer on a #prelude_msg_t object containing the message.
 * @dst: Pointer on a #prelude_io_t object to send the message to.
 *
 * prelude_msg_write() write the message corresponding to the @msg
 * object to @dst. The message should have been created using the
 * prelude_msg_new() and prelude_msg_set() functions.
 *
 * Returns: The number of bytes written, or -1 if an error occured.
 */
int prelude_msg_write(prelude_msg_t *msg, prelude_io_t *dst) 
{
        uint32_t dlen = htonl(msg->msg_index - PRELUDE_MSG_HDR_SIZE);
                
        msg->payload[3] = dlen;
        msg->payload[4] = dlen >> 8;
        msg->payload[5] = dlen >> 16;
        msg->payload[6] = dlen >> 24;

        return prelude_io_write(dst, msg->payload, msg->msg_index); 
}




/**
 * prelude_msg_set:
 * @msg: Pointer on a #prelude_msg_t object to store the data to.
 * @tag: 8 bits unsigned integer describing the kind of data.
 * @len: len of the data chunk.
 * @data: Pointer to the starting address of the data.
 *
 * prelude_msg_set() append @len bytes of data from the @data buffer
 * to the @msg object representing a message. The data is tagged with @tag.
 */
void prelude_msg_set(prelude_msg_t *msg, uint8_t tag, uint32_t len, const void *data) 
{        
        uint32_t l;
        
        assert( (len + 6) < (msg->hdr.datalen - (msg->msg_index - 1)) );

        l = htonl(len);
             
        msg->payload[msg->msg_index++] = tag;
        msg->payload[msg->msg_index++] = l;
        msg->payload[msg->msg_index++] = l >> 8;
        msg->payload[msg->msg_index++] = l >> 16;
        msg->payload[msg->msg_index++] = l >> 24;

        memcpy(&msg->payload[msg->msg_index], data, len);
        msg->msg_index += len;

        /*
         * 0xff mean end of message.
         */
        msg->payload[msg->msg_index++] = 0xff;
}





/**
 * prelude_msg_new:
 * @msgcount: Number of chunk of data the created object can accept.
 * @msglen: Maximum number of bytes the object should handle for all the chunks.
 * @tag: A tag identifying the kind of message.
 * @priority: The priority of this message.
 *
 * Allocate a new #prelude_msg_t object. prelude_msg_set() can then be used to
 * add chunk of data to the message, and prelude_msg_write() to send it.
 *
 * Returns: A pointer on a #prelude_msg_t object or NULL if an error occured.
 */
prelude_msg_t *prelude_msg_new(size_t msgcount, size_t msglen, uint8_t tag, uint8_t priority) 
{
        size_t len;
        prelude_msg_t *msg;
        
        len = msglen;

        /*
         * 6 bytes of header by chunks :
         * - 1 byte:  tag
         * - 4 bytes: len
         * - 1 byte:  end of message
         */ 
        len += msgcount * 6;

        /*
         * For alert header.
         */
        len += PRELUDE_MSG_HDR_SIZE;
                
        msg = malloc(sizeof(prelude_msg_t) + len);
        if ( ! msg ) 
                return NULL;

        msg->destroy = msg_write_destroy;
        
        msg->payload = (unsigned char *) msg + sizeof(prelude_msg_t);

        msg->hdr.version  = msg->payload[0] = PRELUDE_MSG_VERSION;
        msg->hdr.tag      = msg->payload[1] = tag;
        msg->hdr.priority = msg->payload[2] = priority;
        msg->hdr.datalen = len;
        
        msg->msg_index = PRELUDE_MSG_HDR_SIZE;
        
        return msg;
}



/**
 * prelude_msg_get_tag:
 * @msg: Pointer on a #prelude_msg_t object.
 *
 * prelude_msg_get_tag() return the tag contained in the @msg header.
 *
 * Returns: A tag.
 */
uint8_t prelude_msg_get_tag(prelude_msg_t *msg)
{
        return msg->hdr.tag;
}



/**
 * prelude_msg_get_priority:
 * @msg: Pointer on a #prelude_msg_t object.
 *
 * prelude_msg_get_priority() return the priority contained in the @msg header.
 *
 * Returns: A priority.
 */
uint8_t prelude_msg_get_priority(prelude_msg_t *msg) 
{
        return msg->hdr.priority;
}




/**
 * prelude_msg_get_datalen:
 * @msg: Pointer on a #prelude_msg_t object.
 *
 * prelude_msg_get_datalen() return the len of the whole message
 * contained in the @msg header.
 *
 * Returns: Len of the message.
 */
uint32_t prelude_msg_get_datalen(prelude_msg_t *msg) 
{
        return msg->hdr.datalen;
}



/**
 * prelude_msg_destroy:
 * @msg: Pointer on a #prelude_msg_t object.
 *
 * prelude_msg_destroy() destroy the #prelude_msg_t object pointed
 * to by @msg. All the ressources for this message are freed.
 */
void prelude_msg_destroy(prelude_msg_t *msg) 
{
        msg->destroy(msg);
}









/*****
*
* Copyright (C) 2004 Yoann Vandoorselaere <yoann@prelude-ids.org>
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <gcrypt.h>
#include <gnutls/gnutls.h>
#include <gnutls/extra.h>

#include "libmissing.h"
#include "prelude-client.h"
#include "prelude-inet.h"
#include "prelude-error.h"
#include "server.h"
#include "tls-register.h"

#include "config.h"

static char *one_shot_passwd;


static gnutls_session new_tls_session(int sock, gnutls_srp_server_credentials cred)
{
        int ret;
        gnutls_session session;
        const int kx_priority[] = { GNUTLS_KX_SRP, GNUTLS_KX_SRP_DSS, GNUTLS_KX_SRP_RSA, 0 };
        
        gnutls_init(&session, GNUTLS_SERVER);
        
        gnutls_set_default_priority(session);
        gnutls_kx_set_priority(session, kx_priority);
        gnutls_credentials_set(session, GNUTLS_CRD_SRP, cred);
        
        gnutls_certificate_server_set_request(session, GNUTLS_CERT_IGNORE);

        gnutls_transport_set_ptr(session, (gnutls_transport_ptr) sock);
        
        ret = gnutls_handshake(session);        
        if ( ret < 0 ) {
                fprintf(stderr, "  - GnuTLS handshake failed: %s.\n", gnutls_strerror(ret));
                return NULL;
        }
        
        return session;
}



static int handle_client_connection(prelude_client_profile_t *cp, prelude_io_t *fd,
                                    gnutls_srp_server_credentials cred, gnutls_x509_privkey key,
                                    gnutls_x509_crt cacrt, gnutls_x509_crt crt, const char *peer)
{
        gnutls_session session;
        
        session = new_tls_session(prelude_io_get_fd(fd), cred);
        if ( ! session )
                return -1;
        
        prelude_io_set_tls_io(fd, session);
        
        return tls_handle_certificate_request(cp, fd, key, cacrt, crt);
}



static int wait_connection(prelude_client_profile_t *cp, int sock, int keepalive,
                           gnutls_srp_server_credentials cred, gnutls_x509_privkey key,
                           gnutls_x509_crt cacrt, gnutls_x509_crt crt)
{
        char buf[512];
        int csock, ret;
        socklen_t len;
        prelude_io_t *fd;
        struct sockaddr *sa;
        unsigned short *port;
#ifndef HAVE_IPV6
        struct sockaddr_in addr;
#else
        struct sockaddr_in6 addr;
#endif

        ret = prelude_io_new(&fd);
        if ( ret < 0 ) {
                fprintf(stderr, "%s: error creating a new IO object: %s.\n",
                        prelude_strsource(ret), prelude_strerror(ret));
                return -1;
        }
        
        do {
                fprintf(stderr, "\n  - Waiting for install request from peer...\n");
                sa = (struct sockaddr *) &addr;
                len = sizeof(addr);

#ifdef HAVE_IPV6
                port = &addr.sin6_port;
#else
                port = &addr.sin_port;
#endif
                csock = accept(sock, sa, &len);
                if ( csock < 0 ) {
                        fprintf(stderr, "accept returned an error: %s.\n", strerror(errno));
                        return -1;
                }
                
                prelude_inet_ntop(sa->sa_family, prelude_inet_sockaddr_get_inaddr(sa), buf, sizeof(buf));
                fprintf(stderr, "  - Connection from %s:%u.\n", buf, *port);
                prelude_io_set_sys_io(fd, csock);
                
                ret = handle_client_connection(cp, fd, cred, key, cacrt, crt, "");
                if ( ret == 0 )
                        fprintf(stderr, "  - %s:%u successfully registered.\n", buf, *port);
                
                prelude_io_close(fd);
                
        } while ( keepalive || ret < 0 );

        prelude_io_destroy(fd);
        
        return ret;
}



static int setup_server(void)
{
        int sock, ret, on = 1;
        struct sockaddr_in sa_server;
        
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket");
		return -1;
	}

	memset(&sa_server, '\0', sizeof(sa_server));
	sa_server.sin_family = AF_INET;
	sa_server.sin_addr.s_addr = INADDR_ANY;
	sa_server.sin_port = htons(5553);

        ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
        if ( ret < 0 ) {
                perror("setsockopt");
                return -1;
        }
        
	ret = bind(sock, (struct sockaddr *) &sa_server, sizeof(sa_server));
	if ( ret < 0 ) {
		perror("bind");
		return -1;
	}

	ret = listen(sock, 5);
	if ( ret < 0 ) {
		perror("listen");
		return -1;
	}

        return sock;
}



static int ask_one_shot_password(char **buf)
{
        int ret;
	char *pass1, *pass2;
		
	pass1 = getpass("  - enter registration one-shot password: ");
	if ( ! pass1 )
		return -1;
	
	pass1 = strdup(pass1);
	if ( ! pass1 )
		return -1;
	
	pass2 = getpass("  - confirm registration one-shot password: ");
	if ( ! pass2 )
		return -1;

	ret = strcmp(pass1, pass2);
	memset(pass2, 0, strlen(pass2));
	
	if ( ret == 0 ) {
		*buf = pass1;
		return 0;
	}

	memset(pass1, 0, strlen(pass1));
	free(pass1);
	
	return ask_one_shot_password(buf);

}




static int generate_one_shot_password(char **buf) 
{
        int i;
        char c, *mybuf;
        struct timeval tv;
        const int passlen = 8;
	const char letters[] = "01234567890abcdefghijklmnopqrstuvwxyz";

        gettimeofday(&tv, NULL);
        
        srand((unsigned int) getpid() * tv.tv_usec);
        
	mybuf = malloc(passlen + 1);
	if ( ! mybuf )
		return -1;
	
        for ( i = 0; i < passlen; i++ ) {
		c = letters[rand() % (sizeof(letters) - 1)];
                mybuf[i] = c;
        }

        mybuf[passlen] = '\0';

	*buf = mybuf;

        fprintf(stderr, "  - generated one-shot password is \"%s\".\n\n"
                "    This password will be requested by \"prelude-adduser\" in order to connect.\n"
                "    Please remove the first and last quote from this password before using it.\n", mybuf);
        
        return 0;
}




static int copy_datum(gnutls_datum *dst, const gnutls_datum *src)
{
        dst->size = src->size;
        
        dst->data = gnutls_malloc(dst->size);
        if ( ! dst->data ) {
                fprintf(stderr, "memory exhausted.\n");
                return -1;
        }
        
        memcpy(dst->data, src->data, dst->size);

        return 0;
}




static int srp_callback(gnutls_session session, const char *username, gnutls_datum *salt,
                        gnutls_datum *verifier, gnutls_datum *generator, gnutls_datum *prime)
{
        int ret;
        
        if ( strcmp(username, "prelude-adduser") != 0 ) 
                return -1;
        
        salt->size = 4;

        salt->data = gnutls_malloc(4);
        if ( ! salt->data ) {
                fprintf(stderr, "memory exhausted.\n");
                return -1;
        }
        
        gcry_randomize(salt->data, salt->size, GCRY_WEAK_RANDOM);
        
        ret = copy_datum(generator, &gnutls_srp_1024_group_generator);
        if ( ret < 0 ) 
                return -1;
        
        ret = copy_datum(prime, &gnutls_srp_1024_group_prime);
        if ( ret < 0 ) 
                return -1;
        
        return gnutls_srp_verifier(username, one_shot_passwd, salt, generator, prime, verifier);
}



int server_create(prelude_client_profile_t *cp, int keepalive, int prompt,
                  gnutls_x509_privkey key, gnutls_x509_crt cacrt, gnutls_x509_crt crt) 
{
        int sock, ret;
        gnutls_srp_server_credentials cred;

        if ( ! prompt )
    		ret = generate_one_shot_password(&one_shot_passwd);
        else {
                fprintf(stderr,
                        "\n  Please enter registration one-shot password.\n"
                        "  This password will be requested by \"prelude-adduser\" in order to connect.\n\n");
                
		ret = ask_one_shot_password(&one_shot_passwd);
	}

        if ( ret < 0 )
                return -1;
        
        sock = setup_server();
        if ( sock < 0 )
                return -1;

        ret = gnutls_srp_allocate_server_credentials(&cred);
        if ( ret < 0 ) {
                fprintf(stderr, "error creating SRP credentials: %s.\n", gnutls_strerror(ret));
                close(sock);
                return -1;
        }

        gnutls_srp_set_server_credentials_function(cred, srp_callback);
        wait_connection(cp, sock, keepalive, cred, key, cacrt, crt);
        gnutls_srp_free_server_credentials(cred);
        
        return 0;
}

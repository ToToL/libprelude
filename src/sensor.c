/*****
*
* Copyright (C) 2001, 2002 Yoann Vandoorselaere <yoann@mandrakesoft.com>
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
#include <string.h>
#include <sys/time.h>
#include <grp.h>
#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"

#include "prelude-path.h"
#include "prelude-list.h"
#include "config-engine.h"
#include "plugin-common.h"
#include "common.h"

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#include "ssl.h"
#endif

#include "prelude-io.h"
#include "prelude-message.h"
#include "prelude-client-mgr.h"
#include "prelude-getopt.h"
#include "prelude-async.h"
#include "prelude-auth.h"
#include "sensor.h"
#include "client-ident.h"


/*
 * Path to the defalt sensor configuration file.
 */
#define DEFAULT_SENSOR_CONFIG SENSORS_CONFIG_DIR"/sensors-default.conf"


int is_caller_a_sensor = 0;
static prelude_client_mgr_t *manager_list = NULL;



static int create_account(const char *arg) 
{
        int ret;
        char buf[16];
        char filename[256];
        
        fprintf(stderr, "\n\nAuthentication method (cipher/plaintext) [cipher] : ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';
        
        ret = strcmp(buf, "plaintext");
        if ( ret == 0 ) {
                prelude_get_auth_filename(filename, sizeof(filename));
                ret = prelude_auth_create_account(filename, 0);
        } else {
#ifdef HAVE_SSL
                ret = ssl_add_certificate();
#else
                ret = -1;
                fprintf(stderr, "SSL support is not compiled in.\n");
#endif
        }

        exit(ret);
}




static int setup_manager_addr(const char *arg) 
{               
        manager_list = prelude_client_mgr_new(arg);
        if ( ! manager_list ) 
                return prelude_option_error;
        
        return prelude_option_success;
}



static int parse_argument(const char *filename, int argc, char **argv) 
{
        int ret;
        int old_flags;
        
        
        /*
         * Declare library options.
         */
        prelude_option_add(NULL, CLI_HOOK|CFG_HOOK, 'c', "create-account",
                           "Create an account to be used by this sensor", no_argument,
                           create_account, NULL);
        
        prelude_option_add(NULL, CLI_HOOK|CFG_HOOK|WIDE_HOOK, 'm', "manager-addr",
                           "Address where manager is listening (addr:port)",
                           required_argument, setup_manager_addr, NULL);

        /*
         * When parsing our own option, we don't want libprelude to whine
         * about unknow argument on command line (theses can be the own application
         * arguments).
         */
        prelude_option_set_warnings(~(OPT_INVAL|OPT_INVAL_ARG), &old_flags);

        /*
         * Parse configuration and command line arguments.
         */        
        ret = prelude_option_parse_arguments(NULL, filename, argc, argv);
        if ( ret == prelude_option_error ) {
                log(LOG_INFO, "error processing sensor options.\n", filename);
                goto out;
        }
        else if ( ret == prelude_option_end )
                goto out;
        
        /*
         * The sensors configuration file we just parsed doesn't contain
         * entry to specify the Manager address we should connect to.
         *
         * Here we try using the default sensors configuration file.
         */
        if ( ! manager_list ) {
           
                ret = prelude_option_parse_arguments(NULL, DEFAULT_SENSOR_CONFIG, 0, NULL);
                if ( ret < 0 ) {
                        log(LOG_INFO, "error processing generic sensors configuration file.\n");
                        goto out;
                }
                
                if ( manager_list )
                        goto out;
                
                log(LOG_INFO,
                    "No Manager were configured. You need to setup a Manager for this Sensor\n"
                    "to report events. Please use the \"manager-addr\" entry in the Sensor\n"
                    "config file or the -a and eventually -p command line options.\n");
        }

 out:
        /*
         * Destroy option list and restore old option flags.
         */
        prelude_option_set_warnings(old_flags, NULL);

        return ret;
}







/**
 * libprelude_sensor_init:
 * @sname: Name of the sensor.
 * @filename: Configuration file of the calling sensor.
 * @argc: Argument count provided to the calling sensor.
 * @argv: Argument array provided to the calling sensor.
 *
 * Init the sensor library,
 * connect to the manager.
 *
 * Returns: 0 on success, -1 on error.
 */
int prelude_sensor_init(const char *sname, const char *filename, int argc, char **argv)
{
        int ret;

        is_caller_a_sensor = 1;
        
        if ( ! sname ) {
                errno = EINVAL;
                return -1;
        }

        prelude_set_program_name(sname);
        
        ret = prelude_client_ident_init(sname);
        if ( ret < 0 )
                return -1;
        
        ret = parse_argument(filename, argc, argv);
        if ( ret == prelude_option_end || ret == prelude_option_error )
                return ret;
        
        ret = prelude_async_init();
        if ( ret < 0 ) {
                log(LOG_ERR, "couldn't initialize asynchronous subsystem.\n");
                return -1;
        }
        
        return ret;
}



/** 
 * prelude_sensor_send_alert:
 * @msg: Pointer on a message to send.
 *
 * Send @msg to all Manager server we're connected to.
 */
void prelude_sensor_send_alert(prelude_msg_t *msg) 
{
        prelude_client_mgr_broadcast_async(manager_list, msg);
}

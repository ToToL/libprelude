
/* Auto-generated by the GenerateIDMEFMessageSendC package (Sat Dec 13 17:03:59 2003) */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "list.h"
#include "prelude-log.h"
#include "prelude-io.h"
#include "prelude-message.h"
#include "prelude-message-buffered.h"
#include "prelude-message-id.h"
#include "idmef-message-id.h"
#include "idmef.h"
#include "idmef-tree-wrap.h"
#include "idmef-message-send.h"
#include "sensor.h"
#include "prelude-client.h"
#include "common.h"

/*
 * If you wonder why we do this, and why life is complicated,
 * then wonder why the hell the guys that wrote IDMEF choose to use XML.
 * XML is dog slow. And XML'll never achieve performance needed for real time IDS.
 *
 * Here we are trying to communicate using a home made, binary version of IDMEF.
 */


static inline void idmef_send_string(prelude_msgbuf_t *msg, uint8_t tag, idmef_string_t *string)
{
        if ( ! string || ! idmef_string_get_string(string) )
                return;

        prelude_msgbuf_set(msg, tag, idmef_string_get_len(string), idmef_string_get_string(string));
}



static inline void idmef_send_uint64(prelude_msgbuf_t *msg, uint8_t tag, uint64_t data) 
{
        uint64_t dst;
        
        if ( data == 0 )
                return;

        dst = prelude_hton64(data);
        
        prelude_msgbuf_set(msg, tag, sizeof(dst), &dst);
}



static inline void idmef_send_uint32(prelude_msgbuf_t *msg, uint8_t tag, uint32_t data) 
{        
        if ( data == 0 )
                return;
        
        data = htonl(data);
        prelude_msgbuf_set(msg, tag, sizeof(data), &data);
}



static inline void idmef_send_uint16(prelude_msgbuf_t *msg, uint8_t tag, uint16_t data) 
{
        if ( data == 0 )
                return;
        
        data = htons(data);
        prelude_msgbuf_set(msg, tag, sizeof(data), &data);
}



static inline void idmef_send_float(prelude_msgbuf_t *msg, uint8_t tag, float data)
{
	if ( data == 0.0 )
		return;

	prelude_msgbuf_set(msg, tag, sizeof (data), &data);
}



inline void idmef_send_time(prelude_msgbuf_t *msg, uint8_t tag, idmef_time_t *data) 
{
	idmef_time_t dst;

	if ( ! data)
		return;

	idmef_time_set_sec(&dst, htonl(idmef_time_get_sec(data)));
	idmef_time_set_usec(&dst, htonl(idmef_time_get_usec(data)));

	prelude_msgbuf_set(msg, tag, sizeof (dst), &dst);
}



static inline void idmef_send_data(prelude_msgbuf_t *msg, uint8_t tag, idmef_data_t *data)
{
	if ( ! data )
		return;

	prelude_msgbuf_set(msg, tag, idmef_data_get_len(data), idmef_data_get_data(data));
}

void idmef_send_additional_data(prelude_msgbuf_t *msg, idmef_additional_data_t *additional_data)
{
        if ( ! additional_data )
                return;

        prelude_msgbuf_set(msg, MSG_ADDITIONAL_DATA_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_ADDITIONAL_DATA_TYPE, idmef_additional_data_get_type(additional_data));
        idmef_send_string(msg, MSG_ADDITIONAL_DATA_MEANING, idmef_additional_data_get_meaning(additional_data));
        idmef_send_data(msg, MSG_ADDITIONAL_DATA_DATA, idmef_additional_data_get_data(additional_data));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_classification(prelude_msgbuf_t *msg, idmef_classification_t *classification)
{
        if ( ! classification )
                return;

        prelude_msgbuf_set(msg, MSG_CLASSIFICATION_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_CLASSIFICATION_ORIGIN, idmef_classification_get_origin(classification));
        idmef_send_string(msg, MSG_CLASSIFICATION_NAME, idmef_classification_get_name(classification));
        idmef_send_string(msg, MSG_CLASSIFICATION_URL, idmef_classification_get_url(classification));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_userid(prelude_msgbuf_t *msg, idmef_userid_t *userid)
{
        if ( ! userid )
                return;

        prelude_msgbuf_set(msg, MSG_USERID_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_USERID_IDENT, idmef_userid_get_ident(userid));
        idmef_send_uint32(msg, MSG_USERID_TYPE, idmef_userid_get_type(userid));
        idmef_send_string(msg, MSG_USERID_NAME, idmef_userid_get_name(userid));
        idmef_send_uint32(msg, MSG_USERID_NUMBER, idmef_userid_get_number(userid));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_user(prelude_msgbuf_t *msg, idmef_user_t *user)
{
        if ( ! user )
                return;

        prelude_msgbuf_set(msg, MSG_USER_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_USER_IDENT, idmef_user_get_ident(user));
        idmef_send_uint32(msg, MSG_USER_CATEGORY, idmef_user_get_category(user));

        {
                idmef_userid_t *userid = NULL;

                while ( (userid = idmef_user_get_next_userid(user, userid)) ) {
                        idmef_send_userid(msg, userid);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_address(prelude_msgbuf_t *msg, idmef_address_t *address)
{
        if ( ! address )
                return;

        prelude_msgbuf_set(msg, MSG_ADDRESS_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_ADDRESS_IDENT, idmef_address_get_ident(address));
        idmef_send_uint32(msg, MSG_ADDRESS_CATEGORY, idmef_address_get_category(address));
        idmef_send_string(msg, MSG_ADDRESS_VLAN_NAME, idmef_address_get_vlan_name(address));
        idmef_send_uint32(msg, MSG_ADDRESS_VLAN_NUM, idmef_address_get_vlan_num(address));
        idmef_send_string(msg, MSG_ADDRESS_ADDRESS, idmef_address_get_address(address));
        idmef_send_string(msg, MSG_ADDRESS_NETMASK, idmef_address_get_netmask(address));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_process(prelude_msgbuf_t *msg, idmef_process_t *process)
{
        if ( ! process )
                return;

        prelude_msgbuf_set(msg, MSG_PROCESS_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_PROCESS_IDENT, idmef_process_get_ident(process));
        idmef_send_string(msg, MSG_PROCESS_NAME, idmef_process_get_name(process));
        idmef_send_uint32(msg, MSG_PROCESS_PID, idmef_process_get_pid(process));
        idmef_send_string(msg, MSG_PROCESS_PATH, idmef_process_get_path(process));

        {
                idmef_string_t *arg = NULL;

                while ( (arg = idmef_process_get_next_arg(process, arg)) ) {
                        idmef_send_string(msg, MSG_PROCESS_ARG, arg);
                }
        }


        {
                idmef_string_t *env = NULL;

                while ( (env = idmef_process_get_next_env(process, env)) ) {
                        idmef_send_string(msg, MSG_PROCESS_ENV, env);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_webservice(prelude_msgbuf_t *msg, idmef_webservice_t *webservice)
{
        if ( ! webservice )
                return;

        prelude_msgbuf_set(msg, MSG_WEBSERVICE_TAG, 0, NULL);

        idmef_send_string(msg, MSG_WEBSERVICE_URL, idmef_webservice_get_url(webservice));
        idmef_send_string(msg, MSG_WEBSERVICE_CGI, idmef_webservice_get_cgi(webservice));
        idmef_send_string(msg, MSG_WEBSERVICE_HTTP_METHOD, idmef_webservice_get_http_method(webservice));

        {
                idmef_string_t *arg = NULL;

                while ( (arg = idmef_webservice_get_next_arg(webservice, arg)) ) {
                        idmef_send_string(msg, MSG_WEBSERVICE_ARG, arg);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_snmpservice(prelude_msgbuf_t *msg, idmef_snmpservice_t *snmpservice)
{
        if ( ! snmpservice )
                return;

        prelude_msgbuf_set(msg, MSG_SNMPSERVICE_TAG, 0, NULL);

        idmef_send_string(msg, MSG_SNMPSERVICE_OID, idmef_snmpservice_get_oid(snmpservice));
        idmef_send_string(msg, MSG_SNMPSERVICE_COMMUNITY, idmef_snmpservice_get_community(snmpservice));
        idmef_send_string(msg, MSG_SNMPSERVICE_COMMAND, idmef_snmpservice_get_command(snmpservice));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_service(prelude_msgbuf_t *msg, idmef_service_t *service)
{
        if ( ! service )
                return;

        prelude_msgbuf_set(msg, MSG_SERVICE_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_SERVICE_IDENT, idmef_service_get_ident(service));
        idmef_send_string(msg, MSG_SERVICE_NAME, idmef_service_get_name(service));
        idmef_send_uint16(msg, MSG_SERVICE_PORT, idmef_service_get_port(service));
        idmef_send_string(msg, MSG_SERVICE_PORTLIST, idmef_service_get_portlist(service));
        idmef_send_string(msg, MSG_SERVICE_PROTOCOL, idmef_service_get_protocol(service));

        switch ( idmef_service_get_type(service) ) {

                case web_service:
                        idmef_send_webservice(msg, idmef_service_get_web(service));
                        break;

                case snmp_service:
                        idmef_send_snmpservice(msg, idmef_service_get_snmp(service));
                        break;

                default:
                        /* nop */;

        }

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_node(prelude_msgbuf_t *msg, idmef_node_t *node)
{
        if ( ! node )
                return;

        prelude_msgbuf_set(msg, MSG_NODE_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_NODE_IDENT, idmef_node_get_ident(node));
        idmef_send_uint32(msg, MSG_NODE_CATEGORY, idmef_node_get_category(node));
        idmef_send_string(msg, MSG_NODE_LOCATION, idmef_node_get_location(node));
        idmef_send_string(msg, MSG_NODE_NAME, idmef_node_get_name(node));

        {
                idmef_address_t *address = NULL;

                while ( (address = idmef_node_get_next_address(node, address)) ) {
                        idmef_send_address(msg, address);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_source(prelude_msgbuf_t *msg, idmef_source_t *source)
{
        if ( ! source )
                return;

        prelude_msgbuf_set(msg, MSG_SOURCE_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_SOURCE_IDENT, idmef_source_get_ident(source));
        idmef_send_uint32(msg, MSG_SOURCE_SPOOFED, idmef_source_get_spoofed(source));
        idmef_send_string(msg, MSG_SOURCE_INTERFACE, idmef_source_get_interface(source));
        idmef_send_node(msg, idmef_source_get_node(source));
        idmef_send_user(msg, idmef_source_get_user(source));
        idmef_send_process(msg, idmef_source_get_process(source));
        idmef_send_service(msg, idmef_source_get_service(source));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_file_access(prelude_msgbuf_t *msg, idmef_file_access_t *file_access)
{
        if ( ! file_access )
                return;

        prelude_msgbuf_set(msg, MSG_FILE_ACCESS_TAG, 0, NULL);

        idmef_send_userid(msg, idmef_file_access_get_userid(file_access));

        {
                idmef_string_t *permission = NULL;

                while ( (permission = idmef_file_access_get_next_permission(file_access, permission)) ) {
                        idmef_send_string(msg, MSG_FILE_ACCESS_PERMISSION, permission);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_inode(prelude_msgbuf_t *msg, idmef_inode_t *inode)
{
        if ( ! inode )
                return;

        prelude_msgbuf_set(msg, MSG_INODE_TAG, 0, NULL);

        idmef_send_time(msg, MSG_INODE_CHANGE_TIME, idmef_inode_get_change_time(inode));
        idmef_send_uint32(msg, MSG_INODE_NUMBER, idmef_inode_get_number(inode));
        idmef_send_uint32(msg, MSG_INODE_MAJOR_DEVICE, idmef_inode_get_major_device(inode));
        idmef_send_uint32(msg, MSG_INODE_MINOR_DEVICE, idmef_inode_get_minor_device(inode));
        idmef_send_uint32(msg, MSG_INODE_C_MAJOR_DEVICE, idmef_inode_get_c_major_device(inode));
        idmef_send_uint32(msg, MSG_INODE_C_MINOR_DEVICE, idmef_inode_get_c_minor_device(inode));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_linkage(prelude_msgbuf_t *, idmef_linkage_t *);

void idmef_send_file(prelude_msgbuf_t *msg, idmef_file_t *file)
{
        if ( ! file )
                return;

        prelude_msgbuf_set(msg, MSG_FILE_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_FILE_IDENT, idmef_file_get_ident(file));
        idmef_send_uint32(msg, MSG_FILE_CATEGORY, idmef_file_get_category(file));
        idmef_send_string(msg, MSG_FILE_FSTYPE, idmef_file_get_fstype(file));
        idmef_send_string(msg, MSG_FILE_NAME, idmef_file_get_name(file));
        idmef_send_string(msg, MSG_FILE_PATH, idmef_file_get_path(file));
        idmef_send_time(msg, MSG_FILE_CREATE_TIME, idmef_file_get_create_time(file));
        idmef_send_time(msg, MSG_FILE_MODIFY_TIME, idmef_file_get_modify_time(file));
        idmef_send_time(msg, MSG_FILE_ACCESS_TIME, idmef_file_get_access_time(file));
        idmef_send_uint32(msg, MSG_FILE_DATA_SIZE, idmef_file_get_data_size(file));
        idmef_send_uint32(msg, MSG_FILE_DISK_SIZE, idmef_file_get_disk_size(file));

        {
                idmef_file_access_t *file_access = NULL;

                while ( (file_access = idmef_file_get_next_file_access(file, file_access)) ) {
                        idmef_send_file_access(msg, file_access);
                }
        }


        {
                idmef_linkage_t *file_linkage = NULL;

                while ( (file_linkage = idmef_file_get_next_file_linkage(file, file_linkage)) ) {
                        idmef_send_linkage(msg, file_linkage);
                }
        }

        idmef_send_inode(msg, idmef_file_get_inode(file));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_linkage(prelude_msgbuf_t *msg, idmef_linkage_t *linkage)
{
        if ( ! linkage )
                return;

        prelude_msgbuf_set(msg, MSG_LINKAGE_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_LINKAGE_CATEGORY, idmef_linkage_get_category(linkage));
        idmef_send_string(msg, MSG_LINKAGE_NAME, idmef_linkage_get_name(linkage));
        idmef_send_string(msg, MSG_LINKAGE_PATH, idmef_linkage_get_path(linkage));
        idmef_send_file(msg, idmef_linkage_get_file(linkage));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_target(prelude_msgbuf_t *msg, idmef_target_t *target)
{
        if ( ! target )
                return;

        prelude_msgbuf_set(msg, MSG_TARGET_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_TARGET_IDENT, idmef_target_get_ident(target));
        idmef_send_uint32(msg, MSG_TARGET_DECOY, idmef_target_get_decoy(target));
        idmef_send_string(msg, MSG_TARGET_INTERFACE, idmef_target_get_interface(target));
        idmef_send_node(msg, idmef_target_get_node(target));
        idmef_send_user(msg, idmef_target_get_user(target));
        idmef_send_process(msg, idmef_target_get_process(target));
        idmef_send_service(msg, idmef_target_get_service(target));

        {
                idmef_file_t *file = NULL;

                while ( (file = idmef_target_get_next_file(target, file)) ) {
                        idmef_send_file(msg, file);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_analyzer(prelude_msgbuf_t *msg, idmef_analyzer_t *analyzer)
{
        if ( ! analyzer )
                return;

        prelude_msgbuf_set(msg, MSG_ANALYZER_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_ANALYZER_ANALYZERID, idmef_analyzer_get_analyzerid(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_MANUFACTURER, idmef_analyzer_get_manufacturer(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_MODEL, idmef_analyzer_get_model(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_VERSION, idmef_analyzer_get_version(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_CLASS, idmef_analyzer_get_class(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_OSTYPE, idmef_analyzer_get_ostype(analyzer));
        idmef_send_string(msg, MSG_ANALYZER_OSVERSION, idmef_analyzer_get_osversion(analyzer));
        idmef_send_node(msg, idmef_analyzer_get_node(analyzer));
        idmef_send_process(msg, idmef_analyzer_get_process(analyzer));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_alertident(prelude_msgbuf_t *msg, idmef_alertident_t *alertident)
{
        if ( ! alertident )
                return;

        prelude_msgbuf_set(msg, MSG_ALERTIDENT_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_ALERTIDENT_ALERTIDENT, idmef_alertident_get_alertident(alertident));
        idmef_send_uint64(msg, MSG_ALERTIDENT_ANALYZERID, idmef_alertident_get_analyzerid(alertident));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_impact(prelude_msgbuf_t *msg, idmef_impact_t *impact)
{
        if ( ! impact )
                return;

        prelude_msgbuf_set(msg, MSG_IMPACT_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_IMPACT_SEVERITY, idmef_impact_get_severity(impact));
        idmef_send_uint32(msg, MSG_IMPACT_COMPLETION, idmef_impact_get_completion(impact));
        idmef_send_uint32(msg, MSG_IMPACT_TYPE, idmef_impact_get_type(impact));
        idmef_send_string(msg, MSG_IMPACT_DESCRIPTION, idmef_impact_get_description(impact));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_action(prelude_msgbuf_t *msg, idmef_action_t *action)
{
        if ( ! action )
                return;

        prelude_msgbuf_set(msg, MSG_ACTION_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_ACTION_CATEGORY, idmef_action_get_category(action));
        idmef_send_string(msg, MSG_ACTION_DESCRIPTION, idmef_action_get_description(action));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_confidence(prelude_msgbuf_t *msg, idmef_confidence_t *confidence)
{
        if ( ! confidence )
                return;

        prelude_msgbuf_set(msg, MSG_CONFIDENCE_TAG, 0, NULL);

        idmef_send_uint32(msg, MSG_CONFIDENCE_RATING, idmef_confidence_get_rating(confidence));
        idmef_send_float(msg, MSG_CONFIDENCE_CONFIDENCE, idmef_confidence_get_confidence(confidence));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_assessment(prelude_msgbuf_t *msg, idmef_assessment_t *assessment)
{
        if ( ! assessment )
                return;

        prelude_msgbuf_set(msg, MSG_ASSESSMENT_TAG, 0, NULL);

        idmef_send_impact(msg, idmef_assessment_get_impact(assessment));

        {
                idmef_action_t *action = NULL;

                while ( (action = idmef_assessment_get_next_action(assessment, action)) ) {
                        idmef_send_action(msg, action);
                }
        }

        idmef_send_confidence(msg, idmef_assessment_get_confidence(assessment));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_tool_alert(prelude_msgbuf_t *msg, idmef_tool_alert_t *tool_alert)
{
        if ( ! tool_alert )
                return;

        prelude_msgbuf_set(msg, MSG_TOOL_ALERT_TAG, 0, NULL);

        idmef_send_string(msg, MSG_TOOL_ALERT_NAME, idmef_tool_alert_get_name(tool_alert));
        idmef_send_string(msg, MSG_TOOL_ALERT_COMMAND, idmef_tool_alert_get_command(tool_alert));

        {
                idmef_alertident_t *alertident = NULL;

                while ( (alertident = idmef_tool_alert_get_next_alertident(tool_alert, alertident)) ) {
                        idmef_send_alertident(msg, alertident);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_correlation_alert(prelude_msgbuf_t *msg, idmef_correlation_alert_t *correlation_alert)
{
        if ( ! correlation_alert )
                return;

        prelude_msgbuf_set(msg, MSG_CORRELATION_ALERT_TAG, 0, NULL);

        idmef_send_string(msg, MSG_CORRELATION_ALERT_NAME, idmef_correlation_alert_get_name(correlation_alert));

        {
                idmef_alertident_t *alertident = NULL;

                while ( (alertident = idmef_correlation_alert_get_next_alertident(correlation_alert, alertident)) ) {
                        idmef_send_alertident(msg, alertident);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_overflow_alert(prelude_msgbuf_t *msg, idmef_overflow_alert_t *overflow_alert)
{
        if ( ! overflow_alert )
                return;

        prelude_msgbuf_set(msg, MSG_OVERFLOW_ALERT_TAG, 0, NULL);

        idmef_send_string(msg, MSG_OVERFLOW_ALERT_PROGRAM, idmef_overflow_alert_get_program(overflow_alert));
        idmef_send_uint32(msg, MSG_OVERFLOW_ALERT_SIZE, idmef_overflow_alert_get_size(overflow_alert));
        idmef_send_data(msg, MSG_OVERFLOW_ALERT_BUFFER, idmef_overflow_alert_get_buffer(overflow_alert));

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_alert(prelude_msgbuf_t *msg, idmef_alert_t *alert)
{
        if ( ! alert )
                return;

        prelude_msgbuf_set(msg, MSG_ALERT_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_ALERT_IDENT, idmef_alert_get_ident(alert));
        idmef_send_assessment(msg, idmef_alert_get_assessment(alert));
        idmef_send_analyzer(msg, idmef_alert_get_analyzer(alert));
        idmef_send_time(msg, MSG_ALERT_CREATE_TIME, idmef_alert_get_create_time(alert));
        idmef_send_time(msg, MSG_ALERT_DETECT_TIME, idmef_alert_get_detect_time(alert));
        idmef_send_time(msg, MSG_ALERT_ANALYZER_TIME, idmef_alert_get_analyzer_time(alert));

        {
                idmef_source_t *source = NULL;

                while ( (source = idmef_alert_get_next_source(alert, source)) ) {
                        idmef_send_source(msg, source);
                }
        }


        {
                idmef_target_t *target = NULL;

                while ( (target = idmef_alert_get_next_target(alert, target)) ) {
                        idmef_send_target(msg, target);
                }
        }


        {
                idmef_classification_t *classification = NULL;

                while ( (classification = idmef_alert_get_next_classification(alert, classification)) ) {
                        idmef_send_classification(msg, classification);
                }
        }


        {
                idmef_additional_data_t *additional_data = NULL;

                while ( (additional_data = idmef_alert_get_next_additional_data(alert, additional_data)) ) {
                        idmef_send_additional_data(msg, additional_data);
                }
        }


        switch ( idmef_alert_get_type(alert) ) {

                case idmef_tool_alert:
                        idmef_send_tool_alert(msg, idmef_alert_get_tool_alert(alert));
                        break;

                case idmef_correlation_alert:
                        idmef_send_correlation_alert(msg, idmef_alert_get_correlation_alert(alert));
                        break;

                case idmef_overflow_alert:
                        idmef_send_overflow_alert(msg, idmef_alert_get_overflow_alert(alert));
                        break;

                default:
                        /* nop */;

        }

        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_heartbeat(prelude_msgbuf_t *msg, idmef_heartbeat_t *heartbeat)
{
        if ( ! heartbeat )
                return;

        prelude_msgbuf_set(msg, MSG_HEARTBEAT_TAG, 0, NULL);

        idmef_send_uint64(msg, MSG_HEARTBEAT_IDENT, idmef_heartbeat_get_ident(heartbeat));
        idmef_send_analyzer(msg, idmef_heartbeat_get_analyzer(heartbeat));
        idmef_send_time(msg, MSG_HEARTBEAT_CREATE_TIME, idmef_heartbeat_get_create_time(heartbeat));
        idmef_send_time(msg, MSG_HEARTBEAT_ANALYZER_TIME, idmef_heartbeat_get_analyzer_time(heartbeat));

        {
                idmef_additional_data_t *additional_data = NULL;

                while ( (additional_data = idmef_heartbeat_get_next_additional_data(heartbeat, additional_data)) ) {
                        idmef_send_additional_data(msg, additional_data);
                }
        }


        prelude_msgbuf_set(msg, MSG_END_OF_TAG, 0, NULL);
}


void idmef_send_message(prelude_msgbuf_t *msg, idmef_message_t *message)
{
        if ( ! message )
                return;

        idmef_send_string(msg, MSG_MESSAGE_VERSION, idmef_message_get_version(message));

        switch ( idmef_message_get_type(message) ) {

                case idmef_alert_message:
                        idmef_send_alert(msg, idmef_message_get_alert(message));
                        break;

                case idmef_heartbeat_message:
                        idmef_send_heartbeat(msg, idmef_message_get_heartbeat(message));
                        break;

                default:
                        /* nop */;

        }
        prelude_msgbuf_mark_end(msg);
}



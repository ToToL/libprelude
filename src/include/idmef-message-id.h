
/* Auto-generated by the GenerateIDMEFMessageIdH package (Mon Jul  5 10:55:14 2004) */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_ID_H
#define _LIBPRELUDE_IDMEF_MESSAGE_ID_H

/*
 * Misc value
 */

#define MSG_OWN_FORMAT                                 253
#define MSG_END_OF_TAG                                 254

/*
 * Tag value
 */

#define MSG_ADDITIONAL_DATA_TAG 0
#define MSG_CLASSIFICATION_TAG 1
#define MSG_USERID_TAG 2
#define MSG_USER_TAG 3
#define MSG_ADDRESS_TAG 4
#define MSG_PROCESS_TAG 5
#define MSG_WEBSERVICE_TAG 6
#define MSG_SNMPSERVICE_TAG 7
#define MSG_SERVICE_TAG 8
#define MSG_NODE_TAG 9
#define MSG_SOURCE_TAG 10
#define MSG_FILE_ACCESS_TAG 11
#define MSG_INODE_TAG 12
#define MSG_FILE_TAG 13
#define MSG_LINKAGE_TAG 14
#define MSG_TARGET_TAG 15
#define MSG_ANALYZER_TAG 16
#define MSG_ALERTIDENT_TAG 17
#define MSG_IMPACT_TAG 18
#define MSG_ACTION_TAG 19
#define MSG_CONFIDENCE_TAG 20
#define MSG_ASSESSMENT_TAG 21
#define MSG_TOOL_ALERT_TAG 22
#define MSG_CORRELATION_ALERT_TAG 23
#define MSG_OVERFLOW_ALERT_TAG 24
#define MSG_ALERT_TAG 25
#define MSG_HEARTBEAT_TAG 26


/*
 * IDMEF Additional_data
 */
#define MSG_ADDITIONAL_DATA_TYPE 27
#define MSG_ADDITIONAL_DATA_MEANING 28
#define MSG_ADDITIONAL_DATA_DATA 29


/*
 * IDMEF Classification
 */
#define MSG_CLASSIFICATION_ORIGIN 27
#define MSG_CLASSIFICATION_NAME 28
#define MSG_CLASSIFICATION_URL 29


/*
 * IDMEF Userid
 */
#define MSG_USERID_IDENT 27
#define MSG_USERID_TYPE 28
#define MSG_USERID_NAME 29
#define MSG_USERID_NUMBER 30


/*
 * IDMEF User
 */
#define MSG_USER_IDENT 27
#define MSG_USER_CATEGORY 28


/*
 * IDMEF Address
 */
#define MSG_ADDRESS_IDENT 27
#define MSG_ADDRESS_CATEGORY 28
#define MSG_ADDRESS_VLAN_NAME 29
#define MSG_ADDRESS_VLAN_NUM 30
#define MSG_ADDRESS_ADDRESS 31
#define MSG_ADDRESS_NETMASK 32


/*
 * IDMEF Process
 */
#define MSG_PROCESS_IDENT 27
#define MSG_PROCESS_NAME 28
#define MSG_PROCESS_PID 29
#define MSG_PROCESS_PATH 30
#define MSG_PROCESS_ARG 31
#define MSG_PROCESS_ENV 32


/*
 * IDMEF Webservice
 */
#define MSG_WEBSERVICE_URL 27
#define MSG_WEBSERVICE_CGI 28
#define MSG_WEBSERVICE_HTTP_METHOD 29
#define MSG_WEBSERVICE_ARG 30


/*
 * IDMEF Snmpservice
 */
#define MSG_SNMPSERVICE_OID 27
#define MSG_SNMPSERVICE_COMMUNITY 28
#define MSG_SNMPSERVICE_COMMAND 29


/*
 * IDMEF Service
 */
#define MSG_SERVICE_IDENT 27
#define MSG_SERVICE_NAME 28
#define MSG_SERVICE_PORT 29
#define MSG_SERVICE_PORTLIST 30
#define MSG_SERVICE_PROTOCOL 31


/*
 * IDMEF Node
 */
#define MSG_NODE_IDENT 27
#define MSG_NODE_CATEGORY 28
#define MSG_NODE_LOCATION 29
#define MSG_NODE_NAME 30


/*
 * IDMEF Source
 */
#define MSG_SOURCE_IDENT 27
#define MSG_SOURCE_SPOOFED 28
#define MSG_SOURCE_INTERFACE 29


/*
 * IDMEF File_access
 */
#define MSG_FILE_ACCESS_PERMISSION 27


/*
 * IDMEF Inode
 */
#define MSG_INODE_CHANGE_TIME 27
#define MSG_INODE_NUMBER 28
#define MSG_INODE_MAJOR_DEVICE 29
#define MSG_INODE_MINOR_DEVICE 30
#define MSG_INODE_C_MAJOR_DEVICE 31
#define MSG_INODE_C_MINOR_DEVICE 32


/*
 * IDMEF File
 */
#define MSG_FILE_IDENT 27
#define MSG_FILE_CATEGORY 28
#define MSG_FILE_FSTYPE 29
#define MSG_FILE_NAME 30
#define MSG_FILE_PATH 31
#define MSG_FILE_CREATE_TIME 32
#define MSG_FILE_MODIFY_TIME 33
#define MSG_FILE_ACCESS_TIME 34
#define MSG_FILE_DATA_SIZE 35
#define MSG_FILE_DISK_SIZE 36


/*
 * IDMEF Linkage
 */
#define MSG_LINKAGE_CATEGORY 27
#define MSG_LINKAGE_NAME 28
#define MSG_LINKAGE_PATH 29


/*
 * IDMEF Target
 */
#define MSG_TARGET_IDENT 27
#define MSG_TARGET_DECOY 28
#define MSG_TARGET_INTERFACE 29


/*
 * IDMEF Analyzer
 */
#define MSG_ANALYZER_ANALYZERID 27
#define MSG_ANALYZER_MANUFACTURER 28
#define MSG_ANALYZER_MODEL 29
#define MSG_ANALYZER_VERSION 30
#define MSG_ANALYZER_CLASS 31
#define MSG_ANALYZER_OSTYPE 32
#define MSG_ANALYZER_OSVERSION 33


/*
 * IDMEF Alertident
 */
#define MSG_ALERTIDENT_ALERTIDENT 27
#define MSG_ALERTIDENT_ANALYZERID 28


/*
 * IDMEF Impact
 */
#define MSG_IMPACT_SEVERITY 27
#define MSG_IMPACT_COMPLETION 28
#define MSG_IMPACT_TYPE 29
#define MSG_IMPACT_DESCRIPTION 30


/*
 * IDMEF Action
 */
#define MSG_ACTION_CATEGORY 27
#define MSG_ACTION_DESCRIPTION 28


/*
 * IDMEF Confidence
 */
#define MSG_CONFIDENCE_RATING 27
#define MSG_CONFIDENCE_CONFIDENCE 28


/*
 * IDMEF Assessment
 */


/*
 * IDMEF Tool_alert
 */
#define MSG_TOOL_ALERT_NAME 27
#define MSG_TOOL_ALERT_COMMAND 28


/*
 * IDMEF Correlation_alert
 */
#define MSG_CORRELATION_ALERT_NAME 27


/*
 * IDMEF Overflow_alert
 */
#define MSG_OVERFLOW_ALERT_PROGRAM 27
#define MSG_OVERFLOW_ALERT_SIZE 28
#define MSG_OVERFLOW_ALERT_BUFFER 29


/*
 * IDMEF Alert
 */
#define MSG_ALERT_IDENT 27
#define MSG_ALERT_CREATE_TIME 28
#define MSG_ALERT_DETECT_TIME 29
#define MSG_ALERT_ANALYZER_TIME 30


/*
 * IDMEF Heartbeat
 */
#define MSG_HEARTBEAT_IDENT 27
#define MSG_HEARTBEAT_CREATE_TIME 28
#define MSG_HEARTBEAT_ANALYZER_TIME 29


/*
 * IDMEF Message
 */
#define MSG_MESSAGE_VERSION 27


#endif /* _LIBPRELUDE_IDMEF_MESSAGE_ID_H */

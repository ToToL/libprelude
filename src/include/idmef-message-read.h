
/* Auto-generated by the GenerateIDMEFMessageReadH package (Mon Jul  5 10:55:14 2004) */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_READ_H
#define _LIBPRELUDE_IDMEF_MESSAGE_READ_H


idmef_additional_data_t *idmef_read_additional_data(prelude_msg_t *msg, idmef_additional_data_t *additional_data);

idmef_classification_t *idmef_read_classification(prelude_msg_t *msg, idmef_classification_t *classification);

idmef_userid_t *idmef_read_userid(prelude_msg_t *msg, idmef_userid_t *userid);

idmef_user_t *idmef_read_user(prelude_msg_t *msg, idmef_user_t *user);

idmef_address_t *idmef_read_address(prelude_msg_t *msg, idmef_address_t *address);

idmef_process_t *idmef_read_process(prelude_msg_t *msg, idmef_process_t *process);

idmef_webservice_t *idmef_read_webservice(prelude_msg_t *msg, idmef_webservice_t *webservice);

idmef_snmpservice_t *idmef_read_snmpservice(prelude_msg_t *msg, idmef_snmpservice_t *snmpservice);

idmef_service_t *idmef_read_service(prelude_msg_t *msg, idmef_service_t *service);

idmef_node_t *idmef_read_node(prelude_msg_t *msg, idmef_node_t *node);

idmef_source_t *idmef_read_source(prelude_msg_t *msg, idmef_source_t *source);

idmef_file_access_t *idmef_read_file_access(prelude_msg_t *msg, idmef_file_access_t *file_access);

idmef_inode_t *idmef_read_inode(prelude_msg_t *msg, idmef_inode_t *inode);

idmef_file_t *idmef_read_file(prelude_msg_t *msg, idmef_file_t *file);

idmef_linkage_t *idmef_read_linkage(prelude_msg_t *msg, idmef_linkage_t *linkage);

idmef_target_t *idmef_read_target(prelude_msg_t *msg, idmef_target_t *target);

idmef_analyzer_t *idmef_read_analyzer(prelude_msg_t *msg, idmef_analyzer_t *analyzer);

idmef_alertident_t *idmef_read_alertident(prelude_msg_t *msg, idmef_alertident_t *alertident);

idmef_impact_t *idmef_read_impact(prelude_msg_t *msg, idmef_impact_t *impact);

idmef_action_t *idmef_read_action(prelude_msg_t *msg, idmef_action_t *action);

idmef_confidence_t *idmef_read_confidence(prelude_msg_t *msg, idmef_confidence_t *confidence);

idmef_assessment_t *idmef_read_assessment(prelude_msg_t *msg, idmef_assessment_t *assessment);

idmef_tool_alert_t *idmef_read_tool_alert(prelude_msg_t *msg, idmef_tool_alert_t *tool_alert);

idmef_correlation_alert_t *idmef_read_correlation_alert(prelude_msg_t *msg, idmef_correlation_alert_t *correlation_alert);

idmef_overflow_alert_t *idmef_read_overflow_alert(prelude_msg_t *msg, idmef_overflow_alert_t *overflow_alert);

idmef_alert_t *idmef_read_alert(prelude_msg_t *msg, idmef_alert_t *alert);

idmef_heartbeat_t *idmef_read_heartbeat(prelude_msg_t *msg, idmef_heartbeat_t *heartbeat);

#endif /* _LIBPRELUDE_IDMEF_MESSAGE_READ_H */

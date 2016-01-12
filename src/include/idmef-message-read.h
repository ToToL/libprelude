
/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
*
* This file is part of the Prelude library.
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
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

/* Auto-generated by the GenerateIDMEFMessageReadH package */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_READ_H
#define _LIBPRELUDE_IDMEF_MESSAGE_READ_H

#include "prelude-inttypes.h"
#include "prelude-msgbuf.h"

#ifdef __cplusplus
 extern "C" {
#endif


int idmef_additional_data_read(idmef_additional_data_t *additional_data, prelude_msg_t *msg);

int idmef_reference_read(idmef_reference_t *reference, prelude_msg_t *msg);

int idmef_classification_read(idmef_classification_t *classification, prelude_msg_t *msg);

int idmef_user_id_read(idmef_user_id_t *user_id, prelude_msg_t *msg);

int idmef_user_read(idmef_user_t *user, prelude_msg_t *msg);

int idmef_address_read(idmef_address_t *address, prelude_msg_t *msg);

int idmef_process_read(idmef_process_t *process, prelude_msg_t *msg);

int idmef_web_service_read(idmef_web_service_t *web_service, prelude_msg_t *msg);

int idmef_snmp_service_read(idmef_snmp_service_t *snmp_service, prelude_msg_t *msg);

int idmef_service_read(idmef_service_t *service, prelude_msg_t *msg);

int idmef_node_read(idmef_node_t *node, prelude_msg_t *msg);

int idmef_source_read(idmef_source_t *source, prelude_msg_t *msg);

int idmef_file_access_read(idmef_file_access_t *file_access, prelude_msg_t *msg);

int idmef_inode_read(idmef_inode_t *inode, prelude_msg_t *msg);

int idmef_checksum_read(idmef_checksum_t *checksum, prelude_msg_t *msg);

int idmef_file_read(idmef_file_t *file, prelude_msg_t *msg);

int idmef_linkage_read(idmef_linkage_t *linkage, prelude_msg_t *msg);

int idmef_target_read(idmef_target_t *target, prelude_msg_t *msg);

int idmef_analyzer_read(idmef_analyzer_t *analyzer, prelude_msg_t *msg);

int idmef_alertident_read(idmef_alertident_t *alertident, prelude_msg_t *msg);

int idmef_impact_read(idmef_impact_t *impact, prelude_msg_t *msg);

int idmef_action_read(idmef_action_t *action, prelude_msg_t *msg);

int idmef_confidence_read(idmef_confidence_t *confidence, prelude_msg_t *msg);

int idmef_assessment_read(idmef_assessment_t *assessment, prelude_msg_t *msg);

int idmef_tool_alert_read(idmef_tool_alert_t *tool_alert, prelude_msg_t *msg);

int idmef_correlation_alert_read(idmef_correlation_alert_t *correlation_alert, prelude_msg_t *msg);

int idmef_overflow_alert_read(idmef_overflow_alert_t *overflow_alert, prelude_msg_t *msg);

int idmef_alert_read(idmef_alert_t *alert, prelude_msg_t *msg);

int idmef_heartbeat_read(idmef_heartbeat_t *heartbeat, prelude_msg_t *msg);

int idmef_message_read(idmef_message_t *message, prelude_msg_t *msg);


#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IDMEF_MESSAGE_READ_H */


/*****
*
* Copyright (C) 2003, 2004, 2005 PreludeIDS Technologies. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
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

/* Auto-generated by the GenerateIDMEFTreePrintH package */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_PRINT_H
#define _LIBPRELUDE_IDMEF_MESSAGE_PRINT_H


void idmef_additional_data_print(idmef_additional_data_t *ptr, prelude_io_t *fd);
void idmef_reference_print(idmef_reference_t *ptr, prelude_io_t *fd);
void idmef_classification_print(idmef_classification_t *ptr, prelude_io_t *fd);
void idmef_user_id_print(idmef_user_id_t *ptr, prelude_io_t *fd);
void idmef_user_print(idmef_user_t *ptr, prelude_io_t *fd);
void idmef_address_print(idmef_address_t *ptr, prelude_io_t *fd);
void idmef_process_print(idmef_process_t *ptr, prelude_io_t *fd);
void idmef_web_service_print(idmef_web_service_t *ptr, prelude_io_t *fd);
void idmef_snmp_service_print(idmef_snmp_service_t *ptr, prelude_io_t *fd);
void idmef_service_print(idmef_service_t *ptr, prelude_io_t *fd);
void idmef_node_print(idmef_node_t *ptr, prelude_io_t *fd);
void idmef_source_print(idmef_source_t *ptr, prelude_io_t *fd);
void idmef_file_access_print(idmef_file_access_t *ptr, prelude_io_t *fd);
void idmef_inode_print(idmef_inode_t *ptr, prelude_io_t *fd);
void idmef_checksum_print(idmef_checksum_t *ptr, prelude_io_t *fd);
void idmef_file_print(idmef_file_t *ptr, prelude_io_t *fd);
void idmef_linkage_print(idmef_linkage_t *ptr, prelude_io_t *fd);
void idmef_target_print(idmef_target_t *ptr, prelude_io_t *fd);
void idmef_analyzer_print(idmef_analyzer_t *ptr, prelude_io_t *fd);
void idmef_alertident_print(idmef_alertident_t *ptr, prelude_io_t *fd);
void idmef_impact_print(idmef_impact_t *ptr, prelude_io_t *fd);
void idmef_action_print(idmef_action_t *ptr, prelude_io_t *fd);
void idmef_confidence_print(idmef_confidence_t *ptr, prelude_io_t *fd);
void idmef_assessment_print(idmef_assessment_t *ptr, prelude_io_t *fd);
void idmef_tool_alert_print(idmef_tool_alert_t *ptr, prelude_io_t *fd);
void idmef_correlation_alert_print(idmef_correlation_alert_t *ptr, prelude_io_t *fd);
void idmef_overflow_alert_print(idmef_overflow_alert_t *ptr, prelude_io_t *fd);
void idmef_alert_print(idmef_alert_t *ptr, prelude_io_t *fd);
void idmef_heartbeat_print(idmef_heartbeat_t *ptr, prelude_io_t *fd);
void idmef_message_print(idmef_message_t *ptr, prelude_io_t *fd);

#endif /* _LIBPRELUDE_IDMEF_MESSAGE_PRINT_H */

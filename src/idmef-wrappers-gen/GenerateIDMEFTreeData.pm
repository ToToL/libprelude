# Copyright (C) 2003, 2004, 2005 PreludeIDS Technologies. All Rights Reserved.
# Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
#
# This file is part of the Prelude library.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

package GenerateIDMEFTreeData;

use Generate;
@ISA = qw/Generate/;

use strict;
use IDMEFTree; 

sub	header
{
    my	$self = shift;

    $self->output("
/* Auto-generated by the GenerateIDMEFTreeData package */

typedef struct \{
	const char *name;
	prelude_bool_t list;
	idmef_value_type_id_t type;
	idmef_class_id_t class;
\} children_list_t;

");
}

sub	struct
{
    my	$self = shift;
    my	$tree = shift;
    my	$struct = shift;
    my	$name;
    my	$list;
    my	$object;
    my	$object_type;

    $self->output("const children_list_t idmef_$struct->{short_typename}_children_list[] = \{\n");

    foreach my $field ( @{ $struct->{field_list} } ) {

	if ( $field->{metatype} == &METATYPE_UNION ) {

	    foreach my $member ( @{ $field->{member_list} } ) {

		$self->output("        \{ \"$member->{name}\", 0, IDMEF_VALUE_TYPE_CLASS, IDMEF_CLASS_ID_" . uc("$member->{short_typename}") . " \},\n");
	    }

	} else {

	    $list = 0;
	    $name = $field->{name};

	    if ( $field->{metatype} & &METATYPE_NORMAL) {

		if ( $field->{metatype} & &METATYPE_PRIMITIVE ) {
		    $object = "IDMEF_VALUE_TYPE_" . uc("$field->{value_type}");
		    $object_type = 0;

		} elsif ( $field->{metatype} & &METATYPE_STRUCT ) {
		    $object = "IDMEF_VALUE_TYPE_CLASS";
		    $object_type = "IDMEF_CLASS_ID_" . uc("$field->{short_typename}");

		} elsif ( $field->{metatype} & &METATYPE_ENUM ) {
		    $object = "IDMEF_VALUE_TYPE_ENUM";
		    $object_type = "IDMEF_CLASS_ID_" . uc("$field->{short_typename}");
		}

	    } elsif ( $field->{metatype} & &METATYPE_LIST ) {
		$list = 1;
		$name = $field->{short_name};

		if ( $field->{metatype} & &METATYPE_PRIMITIVE ) {
		    $object = "IDMEF_VALUE_TYPE_" . uc("$field->{value_type}");
		    $object_type = 0;

		} else {
		    $object = "IDMEF_VALUE_TYPE_CLASS";
		    $object_type = "IDMEF_CLASS_ID_" . uc("$field->{short_typename}");
		}
	    }

	    $self->output("        \{ \"$name\", $list, $object, $object_type \},\n");
	}
    }

    $self->output("        { NULL, 0, 0, 0 }\n");
    $self->output("\};\n\n");
}

sub	footer
{
    my	$self = shift;
    my	$tree = shift;
    my	$last_id = -1;

    $self->output("
typedef struct \{
	const char *name;
	const children_list_t *children_list;
	int (*get_child)(void *ptr, idmef_class_child_id_t child, void **ret);
	int (*new_child)(void *ptr, idmef_class_child_id_t child, int n, void **ret);
	int (*to_numeric)(const char *name);
	const char *(*to_string)(int val);
        int (*copy)(const void *src, void *dst);
        int (*clone)(const void *src, void **dst);
\} object_data_t;


const object_data_t object_data[] = \{
");
    
    foreach my $obj ( sort { $a->{id} <=> $b->{id} } map { ($_->{obj_type} != &OBJ_PRE_DECLARED ? $_ : () ) } @{ $tree->{obj_list} } ) {

	for ( my $i = $last_id + 1; $i < $obj->{id}; $i++ ) {
	    $self->output("        \{ \"(unassigned)\", NULL, NULL, NULL \}, /* ID: $i */\n");
	}

	$last_id = $obj->{id};

	$self->output("        \{ ",
		      "\"$obj->{short_typename}\", ",
		      "idmef_$obj->{short_typename}_children_list, ",
		      "idmef_$obj->{short_typename}_get_child, ",
		      "idmef_$obj->{short_typename}_new_child, ",
		      "NULL, ",
		      "NULL, ",
		      "idmef_$obj->{short_typename}_copy, ",
		      "idmef_$obj->{short_typename}_clone \}, ",
		      "/* ID: $obj->{id} */\n") if ( $obj->{obj_type} == &OBJ_STRUCT );

	$self->output("        \{ ",
		      "\"$obj->{short_typename}\", ",
		      "NULL, ", 
		      "NULL, ", 
		      "NULL, ",
		      "idmef_$obj->{short_typename}_to_numeric, ",
		      "idmef_$obj->{short_typename}_to_string, ",
		      "NULL, ",
		      "NULL, ",
		      "\}, ",
		      "/* ID: $obj->{id} */\n") if ( $obj->{obj_type} == &OBJ_ENUM );
    }

    $self->output("        \{ NULL, NULL, NULL, NULL \}\n");
    $self->output("};\n");
}

1;

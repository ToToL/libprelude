/*****
*
* Copyright (C) 2003 Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* Copyright (C) 2003 Nicolas Delon <delon.nicolas@wanadoo.fr>
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
     
%{

#include "libmissing.h"
        
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <ctype.h>
#include <pthread.h>

#include "prelude-log.h"
#include "prelude-inttypes.h"

#include "idmef.h"
#include "idmef-criteria.h"
#include "idmef-criteria-string.h"

idmef_criteria_t *criteria;

#define operator_or 1
#define operator_and 2
 
extern int yylex(void);
extern void yylex_init(void);
extern void yylex_destroy(void);
static void yyerror(char *s);
extern void *yy_scan_string(const char *);
extern void yy_delete_buffer(void *);

%}

%union {
	char *str;
        int operator;
	idmef_criterion_t *criterion;
	idmef_criteria_t *criteria;
	idmef_value_relation_t relation;
}     

/* BISON Declarations */

%token <str> TOK_STRING

%token TOK_RELATION_SUBSTRING
%token TOK_RELATION_REGEXP
%token TOK_RELATION_GREATER
%token TOK_RELATION_GREATER_OR_EQUAL
%token TOK_RELATION_LESS
%token TOK_RELATION_LESS_OR_EQUAL
%token TOK_RELATION_EQUAL
%token TOK_RELATION_NOT_EQUAL
%token TOK_RELATION_IS_NULL

%token TOK_OPERATOR_AND
%token TOK_OPERATOR_OR

%type <criteria> criteria
%type <criteria> criteria_base
%type <criterion> criterion
%type <relation> relation
%type <operator> operator
     
/* Grammar follows */
%%

input: criteria					{
							criteria = $1;
						}
;

criteria:	criteria_base			{
							$$ = $1;
						}
	| criteria operator criteria_base	{                                                        
                                                        if ( $2 == operator_or ) {
                                                                idmef_criteria_or_criteria($1, $3);
                                                        } else {
                                                                idmef_criteria_and_criteria($1, $3);
                                                        }
                                                        
                                                        $$ = $1;
						}
;

criteria_base:	criterion			{
							idmef_criteria_t *criteria;

							criteria = idmef_criteria_new();
							if ( ! criteria )
								YYABORT;
							criteria->criterion = $1;
							$$ = criteria;
                                                        
						}
	| '(' criteria ')'			{
							$$ = $2;
						}
;

criterion: TOK_STRING relation TOK_STRING 	{
							idmef_object_t *object = NULL;
							idmef_criterion_value_t *value = NULL;
							idmef_value_relation_t relation;
							idmef_criterion_t *criterion;

							object = idmef_object_new_fast($1);
							if ( ! object ) {
								log(LOG_ERR, "cannot build object '%s'\n", $1);
								free($1);
								free($3);
								YYABORT;
							}

							value = idmef_criterion_value_new_generic(object, $3);
							if ( ! value ) {
								log(LOG_ERR, "cannot build value '%s' for object '%s'\n",
								    $3, $1);
								free($1);
								free($3);
								idmef_object_destroy(object);
								YYABORT;
							}

							relation = $2;

							criterion = idmef_criterion_new(object, value, relation);
							if ( ! criterion ) {
								log(LOG_ERR, "cannot build criterion for "
								    "object: %s and value: %s\n", $1, $3);
								free($1);
								free($3);
								idmef_object_destroy(object);
								idmef_criterion_value_destroy(value);
								YYABORT;
							}

							free($1);
							free($3);
                                                        
							$$ = criterion;
						}
	| TOK_STRING				{
							idmef_object_t *object;
							idmef_criterion_t *criterion;
                                                        
							object = idmef_object_new_fast($1);
							if ( ! object ) {
								log(LOG_ERR, "cannot build object '%s'\n", $1);
								free($1);
								YYABORT;
							}

							criterion = idmef_criterion_new(object, NULL, IDMEF_VALUE_RELATION_IS_NOT_NULL);
							if ( ! criterion ) {
								log(LOG_ERR,
								    "cannot build criterion for object: '%s' and value: NULL\n",
								    $1);
								free($1);
								idmef_object_destroy(object);
								YYABORT;
							}

							free($1);

							$$ = criterion;
						}
	| TOK_RELATION_IS_NULL TOK_STRING	{
							idmef_object_t *object;
							idmef_criterion_t *criterion;

							object = idmef_object_new_fast($2);
							if ( ! object ) {
								log(LOG_ERR, "cannot build object '%s'\n", $2);
								free($2);
								YYABORT;
							}

							criterion = idmef_criterion_new(object, NULL, IDMEF_VALUE_RELATION_IS_NULL);
							if ( ! criterion ) {
								log(LOG_ERR,
								    "cannot build criterion for object: '%s' and value: NULL\n",
								    $2);
								free($2);
								idmef_object_destroy(object);
								YYABORT;
							}

							free($2);

							$$ = criterion;
						}
;

relation:	TOK_RELATION_SUBSTRING		{ $$ = IDMEF_VALUE_RELATION_SUBSTR; }
	|	TOK_RELATION_REGEXP		{ $$ = IDMEF_VALUE_RELATION_REGEX; }
	|	TOK_RELATION_GREATER		{ $$ = IDMEF_VALUE_RELATION_GREATER; }
	|	TOK_RELATION_GREATER_OR_EQUAL	{ $$ = IDMEF_VALUE_RELATION_GREATER|IDMEF_VALUE_RELATION_EQUAL; }
	|	TOK_RELATION_LESS		{ $$ = IDMEF_VALUE_RELATION_LESSER; }
	|	TOK_RELATION_LESS_OR_EQUAL	{ $$ = IDMEF_VALUE_RELATION_LESSER|IDMEF_VALUE_RELATION_EQUAL; }
	|	TOK_RELATION_EQUAL		{ $$ = IDMEF_VALUE_RELATION_EQUAL; }
	|	TOK_RELATION_NOT_EQUAL		{ $$ = IDMEF_VALUE_RELATION_NOT_EQUAL; }
	|	TOK_RELATION_IS_NULL		{ $$ = IDMEF_VALUE_RELATION_IS_NULL; }
;

operator:	TOK_OPERATOR_AND		{ $$ = operator_and; }
       |	TOK_OPERATOR_OR		        { $$ = operator_or; }
;

%%


static void yyerror(char *s)  /* Called by yyparse on error */
{
	/* nop */
}



idmef_criteria_t *idmef_criteria_new_string(const char *str)
{
        int retval;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	void *state;

	pthread_mutex_lock(&mutex);

	criteria = NULL;
	state = yy_scan_string(str);
	retval = yyparse();
	yy_delete_buffer(state);
	if ( retval != 0 && criteria ) {
		idmef_criteria_destroy(criteria);
		criteria = NULL;
	}

	pthread_mutex_unlock(&mutex);
        
	return criteria;
}

# errno_h.m4 serial 1
dnl Copyright (C) 2004, 2006, 2008 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_HEADER_ERRNO_H],
[
  dnl Use AC_REQUIRE here, so that the default behavior below is expanded
  dnl once only, before all statements that occur in other macros.
  AC_REQUIRE([gl_HEADER_ERRNO_H_BODY])
])

AC_DEFUN([gl_HEADER_ERRNO_H_BODY],
[
  AC_REQUIRE([AC_PROG_CC])
  AC_CACHE_CHECK([for complete errno.h], gl_cv_header_errno_h_complete, [
    AC_EGREP_CPP(booboo,[
#include <errno.h>
#if !defined EOVERFLOW || !defined ECANCELED
booboo
#endif
      ],
      [gl_cv_header_errno_h_complete=no],
      [gl_cv_header_errno_h_complete=yes])
  ])
  if test $gl_cv_header_errno_h_complete = yes; then
    ERRNO_H=''
  else
    gl_CHECK_NEXT_HEADERS([errno.h])
    ERRNO_H='errno.h'
  fi
  AC_SUBST([ERRNO_H])
  gl_REPLACE_ERRNO_VALUE([EMULTIHOP])
  gl_REPLACE_ERRNO_VALUE([ENOLINK])
  gl_REPLACE_ERRNO_VALUE([EOVERFLOW])
])

# Assuming $1 = EOVERFLOW.
# The EOVERFLOW errno value ought to be defined in <errno.h>, according to
# POSIX.  But some systems (like OpenBSD 4.0 or AIX 3) don't define it, and
# some systems (like OSF/1) define it when _XOPEN_SOURCE_EXTENDED is defined.
# Check for the value of EOVERFLOW.
# Set the variables EOVERFLOW_HIDDEN and EOVERFLOW_VALUE.
AC_DEFUN([gl_REPLACE_ERRNO_VALUE],
[
  if test -n "$ERRNO_H"; then
    AC_CACHE_CHECK([for ]$1[ value], [gl_cv_header_errno_h_]$1, [
      AC_EGREP_CPP(yes,[
#include <errno.h>
#ifdef ]$1[
yes
#endif
      ],
      [gl_cv_header_errno_h_]$1[=yes],
      [gl_cv_header_errno_h_]$1[=no])
      if test $gl_cv_header_errno_h_]$1[ = no; then
        AC_EGREP_CPP(yes,[
#define _XOPEN_SOURCE_EXTENDED 1
#include <errno.h>
#ifdef ]$1[
yes
#endif
          ], [gl_cv_header_errno_h_]$1[=hidden])
        if test $gl_cv_header_errno_h_]$1[ = hidden; then
          dnl The macro exists but is hidden.
          dnl Define it to the same value.
          AC_COMPUTE_INT([gl_cv_header_errno_h_]$1, $1, [
#define _XOPEN_SOURCE_EXTENDED 1
#include <errno.h>
/* The following two lines are a workaround against an autoconf-2.52 bug.  */
#include <stdio.h>
#include <stdlib.h>
])
        fi
      fi
    ])
    case $gl_cv_header_errno_h_]$1[ in
      yes | no)
        ]$1[_HIDDEN=0; ]$1[_VALUE=
        ;;
      *)
        ]$1[_HIDDEN=1; ]$1[_VALUE="$gl_cv_header_errno_h_]$1["
        ;;
    esac
    AC_SUBST($1[_HIDDEN])
    AC_SUBST($1[_VALUE])
  fi
])

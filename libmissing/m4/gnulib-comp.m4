# Copyright (C) 2004-2006 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects using CVS, this file can be treated like other built files.


# This macro should be invoked from ./configure.in, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([gl_EARLY],
[
  AC_REQUIRE([AC_PROG_RANLIB])
  AC_REQUIRE([AC_GNU_SOURCE])
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
])

# This macro should be invoked from ./configure.in, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl_INIT],
[
  AM_CONDITIONAL([GL_COND_LIBTOOL], [true])
changequote(,)dnl
LTALLOCA=`echo "$ALLOCA" | sed 's/\.[^.]* /.lo /g;s/\.[^.]*$/.lo/'`
changequote([, ])dnl
AC_SUBST([LTALLOCA])
  gl_FUNC_ALLOCA
  dnl gl_USE_SYSTEM_EXTENSIONS must be added quite early to configure.ac.
  gl_FUNC_FTW
  gl_GETADDRINFO
  gl_INET_NTOP
  AC_FUNC_MALLOC
  gl_MBCHAR
  gl_MBITER
  gl_FUNC_MEMCHR
  gl_MINMAX
  gl_FUNC_MKTIME
  gl_PATHMAX
  gl_REGEX
  gl_C_RESTRICT
  gl_SIZE_MAX
  gl_FUNC_SNPRINTF
  gl_TYPE_SOCKLEN_T
  gt_TYPE_SSIZE_T
  AM_STDBOOL_H
  gl_STRCASE
  gl_FUNC_STRCASESTR
  gl_FUNC_STRDUP
  gl_FUNC_STRNDUP
  gl_FUNC_STRNLEN
  gl_FUNC_STRPBRK
  gl_FUNC_STRSEP
  gl_HEADER_SYS_SOCKET
  gl_TIME_R
  gl_FUNC_TIMEGM
  gl_FUNC_VASNPRINTF
  gl_FUNC_VSNPRINTF
  gl_FUNC_WCWIDTH
  gl_XSIZE
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl_FILE_LIST], [
  lib/alloca.c
  lib/alloca_.h
  lib/asnprintf.c
  lib/ftw.c
  lib/ftw_.h
  lib/gai_strerror.c
  lib/getaddrinfo.c
  lib/getaddrinfo.h
  lib/gettext.h
  lib/inet_ntop.c
  lib/inet_ntop.h
  lib/malloc.c
  lib/mbchar.c
  lib/mbchar.h
  lib/mbuiter.h
  lib/memchr.c
  lib/minmax.h
  lib/mktime.c
  lib/pathmax.h
  lib/printf-args.c
  lib/printf-args.h
  lib/printf-parse.c
  lib/printf-parse.h
  lib/regcomp.c
  lib/regex.c
  lib/regex.h
  lib/regex_internal.c
  lib/regex_internal.h
  lib/regexec.c
  lib/size_max.h
  lib/snprintf.c
  lib/snprintf.h
  lib/socket_.h
  lib/stdbool_.h
  lib/strcase.h
  lib/strcasecmp.c
  lib/strcasestr.c
  lib/strcasestr.h
  lib/strdup.c
  lib/strdup.h
  lib/strncasecmp.c
  lib/strndup.c
  lib/strndup.h
  lib/strnlen.c
  lib/strnlen.h
  lib/strnlen1.c
  lib/strnlen1.h
  lib/strpbrk.c
  lib/strpbrk.h
  lib/strsep.c
  lib/strsep.h
  lib/time_r.c
  lib/time_r.h
  lib/timegm.c
  lib/timegm.h
  lib/vasnprintf.c
  lib/vasnprintf.h
  lib/vsnprintf.c
  lib/vsnprintf.h
  lib/wcwidth.h
  lib/xsize.h
  m4/alloca.m4
  m4/codeset.m4
  m4/eoverflow.m4
  m4/extensions.m4
  m4/ftw.m4
  m4/getaddrinfo.m4
  m4/inet_ntop.m4
  m4/intmax_t.m4
  m4/inttypes_h.m4
  m4/longdouble.m4
  m4/longlong.m4
  m4/mbchar.m4
  m4/mbiter.m4
  m4/mbrtowc.m4
  m4/memchr.m4
  m4/minmax.m4
  m4/mktime.m4
  m4/pathmax.m4
  m4/regex.m4
  m4/restrict.m4
  m4/signed.m4
  m4/size_max.m4
  m4/snprintf.m4
  m4/socklen.m4
  m4/sockpfaf.m4
  m4/ssize_t.m4
  m4/stdbool.m4
  m4/stdint_h.m4
  m4/strcase.m4
  m4/strcasestr.m4
  m4/strdup.m4
  m4/strndup.m4
  m4/strnlen.m4
  m4/strpbrk.m4
  m4/strsep.m4
  m4/sys_socket_h.m4
  m4/time_r.m4
  m4/timegm.m4
  m4/vasnprintf.m4
  m4/vsnprintf.m4
  m4/wchar_t.m4
  m4/wcwidth.m4
  m4/wint_t.m4
  m4/xsize.m4
])

# -*- autoconf -*-

dnl Checks for WINDOWS.
AC_DEFUN([ESNACC_CHECK_WIN32],
  [AC_CHECK_HEADER([windows.h],
                   [WIN32=yes],
                   [WIN32=no])
   AM_CONDITIONAL([WIN32], [test "$WIN32" = yes])
   if test "$WIN32" = yes; then
      AC_DEFINE([WIN32], [1], [Define to 1 when building for WINDOWS])
   fi])

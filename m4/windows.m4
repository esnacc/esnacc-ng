# -*- autoconf -*-

dnl Checks for WINDOWS.
AC_DEFUN([ESNACC_CHECK_WIN32],
  [AC_CHECK_HEADER([windows.h],
                   [WIN32=yes],
                   [WIN32=no])
   AM_CONDITIONAL([WIN32], [test "$WIN32" = yes])
   if test "$WIN32" = yes; then
         AC_ARG_WITH([pthread],
         [AS_HELP_STRING([--with-pthread=DIR],
            [root of the pthread-win32 directory])],
         [
            case "$withval" in
            "" | y | ye | yes | n | no)
            AC_MSG_ERROR([Invalid --with-pthread value])
              ;;
            *)
            PTHREAD_WIN32_DIR=$withval/lib/x86
            PTHREAD_WIN32_DIR_DLL=/$(echo ${withval} | ${SED} -e 's/://')/dll/x86
            PTHREAD_WIN32_DIR_DLL_WIN_FORM=$withval/dll/x86
            PTHREAD_INCLUDES=-I$withval/include
            PTHREAD_LDFLAGS=-L$PTHREAD_WIN32_DIR
            PTHREAD_LIBS="-lpthreadVC2"
            AC_SUBST([PTHREAD_WIN32_DIR_DLL_WIN_FORM])
            AC_SUBST([PTHREAD_WIN32_DIR_DLL])
            AC_SUBST([PTHREAD_INCLUDES])
            AC_SUBST([PTHREAD_LDFLAGS])
            AC_SUBST([PTHREAD_LIBS])
              ;;
            esac
         ], [
            AC_MSG_ERROR([pthread directory not specified])
         ]
      )
      AC_DEFINE([WIN32], [1], [Define to 1 when building for WINDOWS])
   fi])

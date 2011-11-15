dnl
dnl Local defined autoconf macros, used to tweak the configure script
dnl
dnl Default auto configure adds a -g to CFLAGS, this macro does not. This macro 
dnl should be called after AC_PROG_CC. Debug builds should not be the default, 
dnl but the GNU configure folks don't want to change it. 
dnl AC_PROG_CC_NO_G
dnl -----------------
dnl CFLAGS by default contain the -g -O2 options. We only need -O2 
dnl defaulting configure to debug is dumb
AC_DEFUN(AC_PROG_CC_NO_G,
[AC_BEFORE([$0], [AC_PROG_CPP])dnl
AC_CHECK_PROG(CC, gcc, gcc)
if test -z "$CC"; then
  AC_CHECK_PROG(CC, cc, cc, , , /usr/ucb/cc)
  test -z "$CC" && AC_MSG_ERROR([no acceptable cc found in \$PATH])
fi
 GCC=yes
 ac_test_CFLAGS="${CFLAGS+set}"
 ac_save_CFLAGS="$CFLAGS"
dnl  CFLAGS=
dnl   C Begin
dnl AC_PROG_CC_G
dnl   C End
  if test "$mydebug" = 1; then
	USEDEBUGLIB="yes"
    	CFLAGS="-g -D_DEBUG -Wall $CFLAGS"
  elif test "$ac_test_CFLAGS" = 1; then
	USEDEBUGLIB="no"
	CFLAGS="-O2 -Wall $CFLAGS"
dnl   C Begin
dnl  elif test $ac_cv_prog_cc_g = yes; then
dnl    CFLAGS="-g -O2"
dnl   C End
  else
	USEDEBUGLIB="no"
    	CFLAGS="-O2 -Wall $CFLAGS"
  fi

AC_SUBST(USEDEBUGLIB)
AC_SUBST(CFLAGS)
])

dnl AC_PROG_CXX_NO_G
dnl -----------------
dnl CXXFLAGS by default contain the -g -O2 options. We only need -O2 
dnl defaulting configure to debug is dumb
AC_DEFUN(AC_PROG_CXX_NO_G,
[AC_BEFORE([$0], [AC_PROG_CXX])dnl
AC_CHECK_PROGS(ac_cxxnog, g++ c++)
if test -z "$ac_cxxnog"; then
  AC_MSG_ERROR([no acceptable c++ found in \$PATH])
  fi
  GCC=yes
 ac_test_CXXFLAGS="${CXXFLAGS+set}"
 ac_save_CXXFLAGS="$CXXFLAGS"
dnl  CFLAGS=
dnl   C Begin
dnl AC_PROG_CC_G
dnl   C End
  if test "$mydebug" = 1; then
	USEDEBUGLIB="yes"
    	CXXFLAGS="-g -D_DEBUG -Wall -DSTDC_HEADERS $CXXFLAGS"
  elif test "$ac_test_CFLAGS" = 1; then
	USEDEBUGLIB="no"
	CXXFLAGS="-O2 -Wall -DSTDC_HEADERS $CXXFLAGS "
dnl   C Begin
dnl  elif test $ac_cv_prog_cc_g = yes; then
dnl    CFLAGS="-g -O2 $CFLAGS"
dnl   C End
  else
	USEDEBUGLIB="no"
    	CXXFLAGS="-O2 -Wall -DSTDC_HEADERS $CXXFLAGS "
  fi
AC_SUBST(USEDEBUGLIB)
AC_SUBST(CXXFLAGS)
])

dnl
dnl
dnl Check gcc version
dnl
dnl AC_CHECK_GCC_VERSION
 AC_DEFUN(AC_CHECK_GCC_VERSION,
[lgcc_version=`gcc -v 2>&1|egrep 'version'|sed 's/gcc version //'|awk -F. '{if ($$"1" >= 3) ;if ($$"2" >= 1) print "yes";else print "no"}'`
 AC_MSG_CHECKING("Checking GNU compiler version")
 if test $lgcc_version = no; then
        AC_MSG_WARN([***** WARNING ***** Incorrect version of GNU - Should be version 3.1])
 else
        AC_MSG_RESULT([Correct version])
 fi
 ])
C_GET_LD_FLAG()
dnl ===============
AC_DEFUN(AC_GET_LD_FLAG,
[dnl
dnl Set default
LFLAGS="-shared"
echo OSTYPE is $OSTYPE
if test -z "$GOSTYPE"; then
OSTYPE=`uname -s`
else
OSTYPE="$GOSTYPE"
fi
echo OSTYPE is $OSTYPE
   SHEXT="so"
 if test "$OSTYPE" = "Linux"; then
   AC_MSG_RESULT([SETTING Load flag to -shared])
   LFLAGS="-shared"
 elif test "$OSTYPE" = "HP-UX"; then
   LFLAGS="-shared -fPIC"
   OSTYPE="HPUX"
   SHEXT="sl"
 elif test "$OSTYPE" = "HPUX32"; then
   LFLAGS="-shared -fPIC"
   SHEXT="sl"
   LIBS=-lpthread
 elif test "$OSTYPE" = "SunOS"; then
   AC_MSG_RESULT([SETTING Load flag to -G])
   LFLAGS="-G -O"
 elif test "$OSTYPE" = "SCO_SV"; then
   AC_MSG_RESULT([SETTING Load flag to -G])
   LFLAGS="-G"
 fi
AC_SUBST(OSTYPE)
AC_SUBST(SHEXT)
AC_SUBST(LFLAGS)
])dnl


# AC_CHECK_XSLTPROC
# NOTE: The win32 check MUST have run before this.

AC_DEFUN([AC_CHECK_XSLTPROC],
[
    AC_ARG_WITH(xsltproc, AS_HELP_STRING([--with-xsltproc[[[[[=PATH]]]]]],
                                         [Use the xsltproc binary at PATH.]),
                                         [ ac_with_xsltproc=$withval; ],
                                         [ ac_with_xsltproc=find; ])
    if test "$ac_with_xsltproc" = "yes" -o "$ac_with_xsltproc" = "find"; then
        AC_PATH_PROGS(XSLTPROC,xsltproc)
    else
        if test "$ac_with_xsltproc" != "no"; then
            if test -x "$ac_with_xsltproc"; then
               XSLTPROC="$ac_with_xsltproc";
            else
               AC_MSG_ERROR([Specified path $ac_with_xsltproc is not executable])
            fi
        else
            if test "$WIN32" = "yes"; then
                AC_MSG_NOTICE([Disabling xsltproc check on windows.])
            else
                AC_MSG_ERROR([Cannot disable the XSLTPROC check.])
            fi
        fi
    fi
])

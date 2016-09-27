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
    if test "$XSLTPROC" != ""; then
       DOCBOOK_XSL_LOCATION=http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl
       AC_MSG_CHECKING(for manpage docbook)
       for i in /usr/share/sgml/docbook/xsl-stylesheets/manpages/docbook.xsl /usr/share/xml/docbook/stylesheet/current/manpages/docbook.xsl /usr/share/xml/docbook/stylesheet/nwalsh/current/manpages/docbook.xsl /usr/share/xml/docbook/stylesheet/nwalsh5/current/manpages/docbook.xsl /usr/share/xml/docbook/stylesheet/docbook-xsl/manpages/docbook.xsl /usr/share/xml/docbook/xsl-stylesheets-*/manpages/docbook.xsl; do
           if test -r $i; then
              DOCBOOK_XSL_LOCATION=$i
           fi
       done
       AC_MSG_RESULT($DOCBOOK_XSL_LOCATION)
       AC_SUBST(DOCBOOK_XSL_LOCATION, $DOCBOOK_XSL_LOCATION)
   fi
])

# file: tkuti.tcl
# miscellaneous Tk utilities.
#
# $Header: /baseline/SNACC/tcl-lib/tkuti.tcl,v 1.2 2003/12/17 19:05:04 gronej Exp $
# $Log: tkuti.tcl,v $
# Revision 1.2  2003/12/17 19:05:04  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:58:56  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:35:46  leonberp
# First CVS Version of SNACC.
#
# Revision 1.1  1997/01/01 23:12:03  rj
# first check-in
#

proc getpos {w xn yn} \
{
  upvar $xn x $yn y
  set geom [wm geometry $w]
  scan $geom {%dx%d+%d+%d} w h x y
}

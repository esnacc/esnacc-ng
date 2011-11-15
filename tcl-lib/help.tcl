# file: help.tcl
# toplevel widget to display a help text (modal)
#
# $Header: /baseline/SNACC/tcl-lib/help.tcl,v 1.2 2003/12/17 19:05:04 gronej Exp $
# $Log: help.tcl,v $
# Revision 1.2  2003/12/17 19:05:04  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:58:56  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:35:46  leonberp
# First CVS Version of SNACC.
#
# Revision 1.1  1997/01/01 23:11:54  rj
# first check-in
#

#\[sep]-----------------------------------------------------------------------------------------------------------------------------
proc help {w helptext} \
{
  set help .help
  set text $help.text
  set sb $help.sb
  set dismiss $help.dismiss

  getpos $w x y
  incr x -100
  toplevel $help -class Dialog
  wm title $help {Help}
  wm transient $help .
  wm geometry $help +$x+$y
  wm minsize $help 0 0

  text $text -borderwidth 2 -relief sunken -yscrollcommand [list $sb set] -width 32 -height 8
  scrollbar $sb -relief sunken -command [list $text yview] -width 10 -cursor arrow
  button $dismiss -text Dismiss -command [list destroy $help]

  pack $dismiss -side bottom -pady 2
  pack $sb -side right -fill y
  pack $text -expand true -fill both

  bind $text <Any-Key> [list destroy $help]

  $text insert end $helptext

  set oldfocus [focus]
  focus $text
  tkwait window $help
  focus $oldfocus
}

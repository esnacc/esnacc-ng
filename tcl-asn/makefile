#
# Makefile for the asnwish
#
# $Header: /baseline/SNACC/tcl-asn/makefile,v 1.2 2003/12/17 19:05:04 gronej Exp $
# $Log: makefile,v $
# Revision 1.2  2003/12/17 19:05:04  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:58:56  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:35:47  leonberp
# First CVS Version of SNACC.
#
# Revision 1.3  1997/10/10 13:43:18  wan
# Corrected bug in generic table decoder wrt. indefinite length elements
# Corrected compiler access to freed memory (bug reported by Markku Savela)
# Broke asnwish.c into two pieces so that one can build ones on wish
# Added beredit tool (based on asnwish, allowes to edit BER messages)
#
# Revision 1.2  1997/09/03 12:13:13  wan
# Reworked asnwish to neither strip nor encapsulate; instead,
# buffer reads during decoding incrementally Tcl_Read the channel.
#
# Revision 1.1  1997/08/27 15:55:51  wan
# Added generic table decoding, debug routines, berdecode, and asnwish.
#
#

include ../makehead

TOP		= ..

SNACC		= $(TOP)/compiler/snacc

ASN1_SRC_DIR	= $(TOP)/asn1specs
C_LIB		= $(TOP)/c-lib
ASN1INC		= $(C_LIB)/inc
ASN1LIB		= $(C_LIB)/libasn1ctbl.a

CPPFLAGS	+= -I$(TOP) -I$(ASN1INC) $(CPPFLAGS.TCL) $(CPPFLAGS.X11)

OFILES	= 	tclasn.o asnwish.o

DISTFILES = 	makefile tclasn.c asnwish.c README beredit

LDFLAGS		+= $(LDFLAGS.X11)
LDLIBS		+= $(ASN1LIB) $(TCLLIBS) $(LDLIBS.X11pre) -lX11 $(LDLIBS.X11extra) $(LIBS)

WISH		= asnwish

#-------------------------------------------------------------------------------

all::		$(WISH)

$(WISH):	$(OFILES) $(ASN1LIB)
		$(REASON)
		$(CC) $(LDFLAGS) -o $(WISH) $(OFILES) $(LDLIBS)

$(ASN1LIB)::
		cd $(@D) && $(MAKE) tbl-lib

# if $(ASN1INC)/asn_useful.h doesn't exist, someone probably called `make clean` in $(C++_LIB).
$(ASN1INC)/asn-useful.h:
		$(MAKE) $(ASN1LIB)

$(SNACC)::
		cd $(@D) && $(MAKE) $(@F)

clean::
		$(RM) $(ASN1HFILES) $(ASN1CFILES)
		$(RM) $(OFILES)

clobber::
		$(RM) $(WISH)

install-sh:
		ln $(TOP)/install-sh $@

$(bindir):
		$(TOP)/mkinstalldirs $@

install::       $(WISH) install-sh $(bindir)

init-depend::
		test -f dependencies || touch dependencies

install::
		$(INSTALL_PROGRAM) $(WISH) $(bindir)/
		$(INSTALL_PROGRAM) beredit $(bindir)/
		$(INSTALL_PROGRAM) tclasn.o $(libdir)/

include ../maketail

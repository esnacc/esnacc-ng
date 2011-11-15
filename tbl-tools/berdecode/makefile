# file: .../tbl-tools/ptbl/makefile
#
# $Header: /baseline/SNACC/tbl-tools/berdecode/makefile,v 1.2 2003/12/17 19:05:04 gronej Exp $
# $Log: makefile,v $
# Revision 1.2  2003/12/17 19:05:04  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:58:56  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:35:48  leonberp
# First CVS Version of SNACC.
#
# Revision 1.1  1997/08/27 15:55:49  wan
# Added generic table decoding, debug routines, berdecode, and asnwish.
#
# Revision 1.1  1997/02/15 19:26:20  rj
# first check-in
#

include ../../makehead

TOP		= ../..

ASN1_C_LIB_DIR	= $(TOP)/c-lib
ASN1_C_INC_DIR	= $(ASN1_C_LIB_DIR)/inc
TTBL_ASN1_LIB	= $(ASN1_C_LIB_DIR)/libasn1ctbl.a

HFILES = \

CFILES = \
	berdecode.c

OFILES = \
	berdecode.o

DISTFILES = \
	README		\
	makefile	\
	$(HFILES)	\
	$(CFILES)

CPPFLAGS	+= -I$(TOP) -I$(ASN1_C_INC_DIR)

#-------------------------------------------------------------------------------

all::		berdecode

berdecode:	$(OFILES) $(TTBL_ASN1_LIB)
		$(REASON)
		$(CC) $(LDFLAGS) -o $@ $(OFILES) $(TTBL_ASN1_LIB) $(LIBS)

install-sh:
		ln $(TOP)/install-sh $@

$(bindir):
		$(TOP)/mkinstalldirs $@

install::	berdecode install-sh $(bindir)

install::
		$(INSTALL_PROGRAM) berdecode $(bindir)/

clean::
		$(RM) *.o *~ core .emacs*

clobber::
		$(RM) berdecode

include ../../maketail

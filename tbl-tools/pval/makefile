# file: .../tbl-tools/pval/makefile
#
# $Header: /baseline/SNACC/tbl-tools/pval/makefile,v 1.2 2003/12/17 19:05:04 gronej Exp $
# $Log: makefile,v $
# Revision 1.2  2003/12/17 19:05:04  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:59:00  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:35:47  leonberp
# First CVS Version of SNACC.
#
# Revision 1.1  1997/02/15 19:15:08  rj
# first check-in
#

include ../../makehead

TOP		= ../..

ASN1_C_LIB_DIR	= $(TOP)/c-lib
ASN1_C_INC_DIR	= $(ASN1_C_LIB_DIR)/inc
TTBL_ASN1_LIB	= $(ASN1_C_LIB_DIR)/libasn1ctbl.a

CFILES = \
	pval.c

OFILES = \
	pval.o

DISTFILES = \
	README		\
	makefile	\
	$(CFILES)

BUFFLAG		= -DUSE_GEN_BUF

CPPFLAGS	+= -I$(TOP) -I$(ASN1_C_INC_DIR) $(BUFFLAG)

#-------------------------------------------------------------------------------

all::		pval

pval:		$(OFILES) $(TTBL_ASN1_LIB)
		$(REASON)
		$(CC) $(LDFLAGS) -o $@ $(OFILES) $(TTBL_ASN1_LIB) $(LIBS)

install-sh:
		ln $(TOP)/install-sh $@

$(bindir):
		$(TOP)/mkinstalldirs $@

install::	pval install-sh $(bindir)

install::
		$(INSTALL_PROGRAM) pval $(bindir)/

clean::
		$(RM) *.o *~ core .emacs*

clobber::
		$(RM) pval

include ../../maketail

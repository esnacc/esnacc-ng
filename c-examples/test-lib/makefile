# c-examples/test-lib/makefile
#
# $Header: /baseline/SNACC/c-examples/test-lib/makefile,v 1.2 2003/12/17 19:05:03 gronej Exp $
# $Log: makefile,v $
# Revision 1.2  2003/12/17 19:05:03  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.1.2.1  2003/11/05 14:58:59  gronej
# working PER code merged with esnacc_1_6
#
# Revision 1.1.1.1  2000/08/21 20:36:06  leonberp
# First CVS Version of SNACC.
#
# Revision 1.4  1995/07/24  20:50:11  rj
# changed `_' to `-' in file names.
#
# Revision 1.3  1995/02/13  15:05:59  rj
# augment CPPFLAGS, not overwrite.
#
# Revision 1.2  1994/08/31  21:43:20  rj
# rebuild the executables when the c-lib is newer.
#
# Revision 1.1  1994/08/31  08:46:37  rj
# first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
#

include ../../makehead

TOP		= ../..

ASN1_C_LIB_DIR	= $(TOP)/c-lib
ASN1_C_INC_DIR	= $(ASN1_C_LIB_DIR)/inc
ASN1_C_LIB	= $(ASN1_C_LIB_DIR)/libasn1csbuf.a

CPPFLAGS	+= -I$(TOP) -I$(ASN1_C_INC_DIR) -DUSE_SBUF

LDLIBS		= $(ASN1_C_LIB) $(LIBS)

CFILES = \
	test-lib.c

DISTFILES = \
	README		\
	makefile	\
	$(CFILES)

#-------------------------------------------------------------------------------

default::	check

check::		test-lib
		./test-lib

test-lib:	$(ASN1_C_LIB)

clean::
		$(RM) *.o *~ .emacs* test-lib core

include ../../maketail

TESTS = c-examples/simple/genber

check_PROGRAMS += c-examples/simple/genber 
#c-examples/simple/minbuf-ex \
#	c-examples/simple/expbuf-ex \
#	c-examples/simple/sbuf-ex 

c_examples_simple_genber_SOURCES = \
	asn1specs/p-rec.asn1 \
	c-examples/simple/genber.c 

c_examples_simple_genber_CFLAGS = \
	-I$(top_srcdir)/asn1specs \
	-I$(builddir)/asn1specs \
	-I$(top_srcdir)/c-lib/inc

c_examples_simple_genber_LDADD = \
	c-lib/libcasn1.la

CLEANFILES += asn1specs/p-rec.h asn1specs/p-rec.c pr.ber

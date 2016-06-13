TESTS = c-examples/simple/genber \
	c-examples/simple/minbuf \
	c-examples/simple/expbuf \
	c-examples/simple/sbuf

check_PROGRAMS += c-examples/simple/genber \
	c-examples/simple/minbuf \
	c-examples/simple/expbuf \
	c-examples/simple/sbuf

c_examples_simple_CFLAGS = \
	-I$(top_srcdir)/asn1specs \
	-I$(builddir)/asn1specs \
	-I$(top_srcdir)/c-lib/inc

c_examples_simple_genber_SOURCES = \
	asn1specs/p-rec.asn1 \
	c-examples/simple/genber.c 

c_examples_simple_genber_CFLAGS = \
	$(c_examples_simple_CFLAGS)

c_examples_simple_genber_LDADD = \
	c-lib/libcasn1.la

c_examples_simple_minbuf_SOURCES = \
	asn1specs/p-rec.asn1 \
	c-examples/simple/minbuf-ex.c 

c_examples_simple_minbuf_CFLAGS = \
	$(c_examples_simple_CFLAGS)

c_examples_simple_minbuf_LDADD = \
	c-lib/libcasn1.la

c_examples_simple_expbuf_SOURCES = \
	asn1specs/p-rec.asn1 \
	c-examples/simple/expbuf-ex.c

c_examples_simple_expbuf_CFLAGS = \
	$(c_examples_simple_CFLAGS)

c_examples_simple_expbuf_LDADD = \
	c-lib/libcasn1.la

c_examples_simple_sbuf_SOURCES = \
	asn1specs/p-rec.asn1 \
	c-examples/simple/sbuf-ex.c

c_examples_simple_sbuf_CFLAGS = \
	$(c_examples_simple_CFLAGS)

c_examples_simple_sbuf_LDADD = \
	c-lib/libcasn1.la

CLEANFILES += asn1specs/p-rec.h asn1specs/p-rec.c pr.ber

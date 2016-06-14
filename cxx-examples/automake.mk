TESTS += cxx-examples/main

check_PROGRAMS += cxx-examples/main

cxx_examples_main_SOURCES = \
	cxx-examples/src/asnutil.cpp \
	cxx-examples/src/asnutil.h \
	cxx-examples/src/bitstest.cpp \
	cxx-examples/src/gfsi.cpp \
	cxx-examples/src/inttest.cpp \
	cxx-examples/src/main.cpp \
	cxx-examples/src/per_tests.cpp \
	cxx-examples/src/rfc1155-smi.cpp \
	cxx-examples/src/rfc1157-snmp.cpp \
	cxx-examples/src/snmp.cpp \
	cxx-examples/src/testbuf.cpp
#	cxx-examples/src/testsetsorting.cpp 
#	cxx-examples/src/vdatest_asn.asn1 \
#	cxx-examples/src/vdatest_asn2.asn1 \
#	cxx-examples/src/vdatest.cpp \
#	cxx-examples/src/vda_threads.cpp \
#	cxx-examples/src/vda_threads.h

cxx_examples_main_CXXFLAGS = \
	-I$(top_srcdir)/cxx-lib/inc \
	-I$(builddir)/cxx-examples/src \
	$(PTHREAD_CFLAGS)

cxx_examples_main_LDADD = \
	cxx-lib/libcxxasn1.la \
	$(PTHREAD_LIBS)

cxx_examples_main_LDFLAGS = \
	$(PTHREAD_CFLAGS)

$(top_builddir)/cxx-examples/src/rfc1155-smi.cpp: \
 $(top_srcdir)/cxx-examples/src/rfc1155-smi.asn1 \
 $(top_builddir)/compiler/esnacc
	$(top_builddir)/compiler/esnacc -C -mo `dirname $@` $<

$(top_builddir)/cxx-examples/src/rfc1157-snmp.cpp: \
 $(top_srcdir)/cxx-examples/src/rfc1157-snmp.asn1 \
 $(top_builddir)/compiler/esnacc
	$(top_builddir)/compiler/esnacc -C -mo `dirname $@` \
	$< -I $(top_srcdir)/cxx-examples/src

EXTRA_DIST+= \
	cxx-examples/src/rfc1155-smi.asn1 \
	cxx-examples/src/rfc1157-snmp.asn1

CLEANFILES+= \
	test.txt \
	cxx-examples/src/rfc1157-snmp.cpp \
	cxx-examples/src/rfc1157-snmp.h \
	cxx-examples/src/rfc1155-smi.cpp \
	cxx-examples/src/rfc1155-smi.h

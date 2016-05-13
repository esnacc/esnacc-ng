lib_LTLIBRARIES += c-lib/libcasn1.la

c_lib_libcasn1_la_SOURCES = \
	c-lib/src/asn1init.c \
	c-lib/src/asn-octs.c \
	c-lib/src/mem.c \
	c-lib/src/min-buf.c \
	c-lib/src/asn-any.c \
	c-lib/src/asn-oid.c \
	c-lib/src/nibble-alloc.c \
	c-lib/src/asn-bits.c \
	c-lib/src/asn-PrintableStr.c \
	c-lib/src/print.c \
	c-lib/src/asn-BMPString.c \
	c-lib/src/asn-real.c \
	c-lib/src/sbuf.c \
	c-lib/src/asn-bool.c \
	c-lib/src/asn-relative-oid.c \
	c-lib/src/str-stk.c \
	c-lib/src/asn-der.c \
	c-lib/src/asn-tag.c \
	c-lib/src/tbl-dbg.c \
	c-lib/src/asn-enum.c \
	c-lib/src/asn-TeletexString.c \
	c-lib/src/tbl-dec.c \
	c-lib/src/asn-IA5String.c \
	c-lib/src/asn-UniversalString.c \
	c-lib/src/tbl-enc.c \
	c-lib/src/asn-int.c \
	c-lib/src/tbl-free.c \
	c-lib/src/asn-len.c \
	c-lib/src/asn-UTF8String.c \
	c-lib/src/tbl-gen.c \
	c-lib/src/asn-list.c \
	c-lib/src/asn-VisibleString.c \
	c-lib/src/tbl-print.c \
	c-lib/src/asn-null.c \
	c-lib/src/exp-buf.c \
	c-lib/src/tbl-util.c \
	c-lib/src/asn-NumericString.c \
	c-lib/src/hash.c 

nobase_include_HEADERS += c-lib/inc/asn-any.h \
	c-lib/inc/asn-bits.h \
	c-lib/inc/asn-BMPString.h \
	c-lib/inc/asn-bool.h \
	c-lib/inc/asn-config.h \
	c-lib/inc/asn-der.h \
	c-lib/inc/asn-enum.h \
	c-lib/inc/asn-IA5String.h \
	c-lib/inc/asn-incl.h \
	c-lib/inc/asn-int.h \
	c-lib/inc/asn-len.h \
	c-lib/inc/asn-list.h \
	c-lib/inc/asn-null.h \
	c-lib/inc/asn-NumericString.h \
	c-lib/inc/asn-octs.h \
	c-lib/inc/asn-oid.h \
	c-lib/inc/asn-PrintableStr.h \
	c-lib/inc/asn-real.h \
	c-lib/inc/asn-relative-oid.h \
	c-lib/inc/asn-tag.h \
	c-lib/inc/asn-TeletexString.h \
	c-lib/inc/asn-UniversalString.h \
	c-lib/inc/asn-UTF8String.h \
	c-lib/inc/asn-VisibleString.h \
	c-lib/inc/exp-buf.h \
	c-lib/inc/gen-buf.h \
	c-lib/inc/hash.h \
	c-lib/inc/mem.h \
	c-lib/inc/min-buf.h \
	c-lib/inc/nibble-alloc.h \
	c-lib/inc/print.h \
	c-lib/inc/sbuf.h \
	c-lib/inc/snaccCder.h \
	c-lib/inc/str-stk.h \
	c-lib/inc/tbl-dbg.h \
	c-lib/inc/tbl-dec.h \
	c-lib/inc/tbl-enc.h \
	c-lib/inc/tbl-free.h \
	c-lib/inc/tbl-gen-c-hdr.h \
	c-lib/inc/tbl-gen.h \
	c-lib/inc/tbl-incl.h \
	c-lib/inc/tbl-print.h \
	c-lib/inc/tbl-util.h

c_lib_libcasn1_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/c-lib \
	-I$(top_srcdir)/c-lib/src \
	-I$(top_srcdir)/c-lib/inc

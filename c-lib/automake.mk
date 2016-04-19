lib_LTLIBRARIES += c-lib/libcasn1.la

c_lib_libcasn1_la_SOURCES = \
	c-lib/src/asn1init.c \
	c-lib/src/asn-octs.c \
	c-lib/src/mem.c \
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

c_lib_libcasn1_la_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/c-lib \
	-I$(top_srcdir)/c-lib/src \
	-I$(top_srcdir)/c-lib/inc

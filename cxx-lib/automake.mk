lib_LTLIBRARIES += cxx-lib/libcxxasn1.la

nobase_include_HEADERS += cxx-lib/inc/asn-buf.h \
	cxx-lib/inc/asn-chartraits.h \
	cxx-lib/inc/asn-config.h \
	cxx-lib/inc/asn-incl.h \
	cxx-lib/inc/asn-list.h \
	cxx-lib/inc/asn-listset.h \
	cxx-lib/inc/asn-usefultypes.h \
	cxx-lib/inc/init.h \
	cxx-lib/inc/meta.h \
	cxx-lib/inc/snaccdll.h \
	cxx-lib/inc/snaccexcept.h \
	cxx-lib/inc/tcl-if.h

cxx_lib_libcxxasn1_la_SOURCES = \
	cxx-lib/src/asn-null.cpp \
	cxx-lib/src/asn-oid.cpp \
	cxx-lib/src/asn-RelativeOid.cpp \
	cxx-lib/src/asn-PERGeneral.cpp \
	cxx-lib/src/snaccdll.cpp \
	cxx-lib/src/snaccexcept.cpp \
	cxx-lib/src/tkAppInit.c \
	cxx-lib/src/asn-enum.cpp \
	cxx-lib/src/asn-extension.cpp \
	cxx-lib/src/asn-any.cpp \
	cxx-lib/src/asn-type.cpp \
	cxx-lib/src/asn-buf.cpp \
	cxx-lib/src/asn-int.cpp \
	cxx-lib/src/asn-tag.cpp \
	cxx-lib/src/asn-stringtype.cpp \
	cxx-lib/src/asn-bufbits.cpp \
	cxx-lib/src/asn-real.cpp \
	cxx-lib/src/asn-usefultypes.cpp \
	cxx-lib/src/asn-bool.cpp \
	cxx-lib/src/hash.cpp \
	cxx-lib/src/meta.cpp \
	cxx-lib/src/asn-octs.cpp \
	cxx-lib/src/vda_threads2.cpp \
	cxx-lib/src/print.cpp \
	cxx-lib/src/tcl-if.cpp \
	cxx-lib/src/asn-bits.cpp \
	cxx-lib/src/asn-len.cpp \
	cxx-lib/src/asn-fileseg.cpp \
	cxx-lib/src/asn-rvsbuf.cpp 

cxx_lib_libcxxasn1_la_WIN32_FLAGS=
cxx_lib_libcxxasn1_la_WIN32_LIBADD=
cxx_lib_libcxxasn1_la_WIN32_LDFLAGS=
if WIN32
cxx_lib_libcxxasn1_la_WIN32_FLAGS += -DSNACCDLL_EXPORTS=1 \
	${PTHREAD_INCLUDES}
cxx_lib_libcxxasn1_la_WIN32_LDFLAGS += ${PTHREAD_LDFLAGS}
cxx_lib_libcxxasn1_la_WIN32_LIBADD += ${PTHREAD_LIBS}
endif

cxx_lib_libcxxasn1_la_CXXFLAGS = \
	$(cxx_lib_libcxxasn1_la_WIN32_FLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/cxx-lib \
	-I$(top_srcdir)/cxx-lib/src \
	-I$(top_srcdir)/cxx-lib/inc

cxx_lib_libcxxasn1_la_CFLAGS = \
	$(cxx_lib_libcxxasn1_la_WIN32_FLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/cxx-lib \
	-I$(top_srcdir)/cxx-lib/src \
	-I$(top_srcdir)/cxx-lib/inc

cxx_lib_libcxxasn1_la_LIBADD = \
	$(cxx_lib_libcxxasn1_la_WIN32_LIBADD)

cxx_lib_libcxxasn1_la_LDFLAGS = \
	$(cxx_lib_libcxxasn1_la_WIN32_LDFLAGS)

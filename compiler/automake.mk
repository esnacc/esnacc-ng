bin_PROGRAMS += compiler/esnacc

compiler_esnacc_SOURCES = \
	compiler/core/asn1module.h \
	compiler/core/y.tab.y \
	compiler/core/define.c \
	compiler/core/define.h \
	compiler/core/dependency.c \
	compiler/core/do-macros.c \
	compiler/core/do-macros.h \
	compiler/core/enc-rules.c \
	compiler/core/enc-rules.h \
	compiler/core/err-chk.c \
	compiler/core/exports.c \
	compiler/core/exports.h \
	compiler/core/gen-tbls.c \
	compiler/core/lex-asn1.l \
	compiler/core/lex-stuff.h \
	compiler/core/lib-types.c \
	compiler/core/lib-types.h \
	compiler/core/link-types.c \
	compiler/core/link-values.c \
	compiler/core/meta.c \
	compiler/core/meta.h \
	compiler/core/normalize.c \
	compiler/core/oid.c \
	compiler/core/print.c \
	compiler/core/print.h \
	compiler/core/recursive.c \
	compiler/core/snacc.c \
	compiler/core/snacc-util.c \
	compiler/core/snacc-util.h \
	compiler/core/tbl.c \
	compiler/core/tbl.h \
	compiler/core/val-parser.c \
	compiler/core/gfsi.c \
	compiler/back-ends/cond.c \
	compiler/back-ends/str-util.c \
	compiler/back-ends/str-util.h \
	compiler/back-ends/tag-util.c \
	compiler/back-ends/tag-util.h \
	compiler/back-ends/c-gen/gen-any.c \
	compiler/back-ends/c-gen/gen-type.c \
	compiler/back-ends/c-gen/gen-code.c \
	compiler/back-ends/c-gen/gen-vals.c \
	compiler/back-ends/c-gen/gen-dec.c \
	compiler/back-ends/c-gen/kwd.c \
	compiler/back-ends/c-gen/gen-enc.c \
	compiler/back-ends/c-gen/rules.c \
	compiler/back-ends/c-gen/rules.h \
        compiler/back-ends/c-gen/gen-free.c \
	compiler/back-ends/c-gen/type-info.c \
	compiler/back-ends/c-gen/type-info.h \
	compiler/back-ends/c-gen/gen-print.c \
	compiler/back-ends/c-gen/util.c \
	compiler/back-ends/c-gen/util.h \
	compiler/back-ends/c++-gen/cxxconstraints.c \
	compiler/back-ends/c++-gen/cxxconstraints.h \
	compiler/back-ends/c++-gen/cxxmultipleconstraints.c \
	compiler/back-ends/c++-gen/cxxmultipleconstraints.h \
	compiler/back-ends/c++-gen/gen-any.c \
	compiler/back-ends/c++-gen/gen-code.c \
	compiler/back-ends/c++-gen/gen-vals.c \
	compiler/back-ends/c++-gen/kwd.c \
	compiler/back-ends/c++-gen/rules.c \
	compiler/back-ends/c++-gen/rules.h \
	compiler/back-ends/c++-gen/types.c \
	compiler/back-ends/idl-gen/gen-any.c \
	compiler/back-ends/idl-gen/rules.c \
	compiler/back-ends/idl-gen/rules.h \
	compiler/back-ends/idl-gen/gen-code.c \
	compiler/back-ends/idl-gen/types.c \
	compiler/back-ends/idl-gen/gen-vals.c \
	compiler/back-ends/py-gen/gen-any.c \
	compiler/back-ends/py-gen/gen-code.c \
	compiler/back-ends/py-gen/rules.c \
	compiler/back-ends/py-gen/rules.h \
	compiler/back-ends/py-gen/types.c \
	version.h

compiler/core/y.tab.c compiler/core/y.tab.h: compiler/core/y.tab.y
	$(YACC) -v -t -d -r all -o compiler/core/y.tab.c $(srcdir)/compiler/core/y.tab.y

compiler/core/lex-asn1.c: compiler/core/lex-asn1.l compiler/core/y.tab.c compiler/core/y.tab.h
	$(LEX) -t $< | $(SED) -e 's@#include <unistd.h>@@g' > $@

EXTRA_DIST += compiler/esnacc.xml.in

if !WIN32
man_MANS += compiler/esnacc.1
DISTCLEANFILES += compiler/esnacc.1
endif

compiler/esnacc.1: compiler/esnacc.xml
	$(XSLTPROC) --novalid -o $@ $(DOCBOOK_LOCATION) $<

CLEANFILES += compiler/core/lex-asn1.c \
	compiler/core/y.output \
	compiler/core/y.tab.c \
	compiler/core/y.tab.h \
	compiler/esnacc.xml

compiler_esnacc_CFLAGS = \
	-I$(top_srcdir)/compiler/core \
	-I$(top_srcdir)/c-lib/inc \
	-I$(top_srcdir)/compiler/back-ends \
	-I$(top_srcdir) \
	-DCOMPILER

compiler_esnacc_LDADD = \
	c-lib/libcasn1.la


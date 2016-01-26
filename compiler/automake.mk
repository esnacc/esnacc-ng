bin_PROGRAMS += compiler/esnacc

compiler_esnacc_SOURCES = \
	compiler/core/y.tab.c \
	compiler/core/define.c \
	compiler/core/dependency.c \
	compiler/core/do-macros.c \
	compiler/core/enc-rules.c \
	compiler/core/err-chk.c \
	compiler/core/exports.c \
	compiler/core/gen-tbls.c \
	compiler/core/lex-asn1.c \
	compiler/core/lib-types.c \
	compiler/core/link-types.c \
	compiler/core/link-values.c \
	compiler/core/mem.c \
	compiler/core/meta.c \
	compiler/core/normalize.c \
	compiler/core/oid.c \
	compiler/core/print.c \
	compiler/core/recursive.c \
	compiler/core/snacc.c \
	compiler/core/snacc-util.c \
	compiler/core/tbl.c \
	compiler/core/val-parser.c \
	compiler/core/gfsi.c \
	compiler/back-ends/cond.c \
	compiler/back-ends/str-util.c \
	compiler/back-ends/tag-util.c \
	compiler/back-ends/c-gen/gen-any.c \
	compiler/back-ends/c-gen/gen-type.c \
	compiler/back-ends/c-gen/gen-code.c \
	compiler/back-ends/c-gen/gen-vals.c \
	compiler/back-ends/c-gen/gen-dec.c \
	compiler/back-ends/c-gen/kwd.c \
	compiler/back-ends/c-gen/gen-enc.c \
	compiler/back-ends/c-gen/rules.c \
        compiler/back-ends/c-gen/gen-free.c \
	compiler/back-ends/c-gen/type-info.c \
	compiler/back-ends/c-gen/gen-print.c \
	compiler/back-ends/c-gen/util.c \
	compiler/back-ends/c++-gen/cxxconstraints.c \
	compiler/back-ends/c++-gen/cxxmultipleconstraints.c \
	compiler/back-ends/c++-gen/gen-any.c \
	compiler/back-ends/c++-gen/gen-code.c \
	compiler/back-ends/c++-gen/gen-vals.c \
	compiler/back-ends/c++-gen/kwd.c \
	compiler/back-ends/c++-gen/rules.c \
	compiler/back-ends/c++-gen/types.c \
	compiler/back-ends/idl-gen/gen-any.c \
	compiler/back-ends/idl-gen/rules.c \
	compiler/back-ends/idl-gen/gen-code.c \
	compiler/back-ends/idl-gen/types.c \
	compiler/back-ends/idl-gen/gen-vals.c

compiler/core/y.tab.c: compiler/core/y.tab.y
	$(YACC) -v -t -d -r all -o compiler/core/y.tab.c compiler/core/y.tab.y

compiler/core/lex-asn1.c: compiler/core/lex-asn1.l compiler/core/y.tab.c
	$(LEX) -t $< | $(SED) -e 's@#include <unistd.h>@@g' > $@

CLEANFILES += compiler/core/y.tab.c compiler/core/y.tab.h \
	compiler/core/lex-asn1.c

compiler_esnacc_CFLAGS = \
	-I$(top_srcdir)/compiler/core \
	-I$(top_srcdir)/c-lib/inc \
	-I$(top_srcdir)/compiler/back-ends \
	-I$(top_srcdir) \
	-DCOMPILER

compiler_esnacc_LDADD = \
	c-lib/libcasn1.la

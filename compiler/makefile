
include ../makehead

HDRS         = $(wildcard core/*.h ./back-ends/c-gen/*.h ./back-end/c++-gen/*.h)
SRC          = core/define.c core/dependency.c core/do-macros.c core/enc-rules.c \
					core/err-chk.c core/exports.c core/gen-tbls.c core/lex-asn1.c \
					core/lib-types.c core/link-types.c core/link-values.c core/mem.c core/meta.c \
					core/normalize.c core/oid.c core/parse-asn1.c core/print.c core/recursive.c \
					core/snacc.c core/snacc-util.c core/tbl.c core/val-parser.c core/gfsi.c \
               $(wildcard ./back-ends/*.c ./back-ends/c-gen/*.c ./back-ends/c++-gen/*.c \
               ./back-ends/idl-gen/*.c)

OBJS         = $(SRC:.c=.o)

INCLUDES     = -I./c-gen \
               -I./c++-gen \
               -I./core \
               -I../c-lib/inc \
				   -I./back-ends \
               -I./back-ends/idl-gen \
               -I../

DEFS	  	    = -DCOMPILER 
CPPFLAGS	   += $(DEFS) $(INCLUDES)

TARGET       = esnacc

all: $(TARGET)

##
## BISON/FLEX dependencies
##
./core/lex-asn1.c: ./core/lex-asn1.l ./core/parse-asn1.h
./core/parse-asn1.c:./core/parse-asn1.y 

compilerFLAGS	= $(DEFS) -I../compiler/$(CORE_DIR)

$(OBJS): $(HDRS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) ../c-lib/libcasn1.a $(LEXLIB)

clean:
		rm -f $(OBJS)

install:
	(/bin/sh ../mkinstalldirs $(prefix)/bin;\
	/bin/sh ../install-sh -c $(TARGET) $(prefix)/bin)

include ../maketail

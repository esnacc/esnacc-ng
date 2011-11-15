include ../makehead

DEPENDENCIES	=

TOP		= ..

CPPFLAGS	+= -I$(TOP) -Iinc $(bufFLAGS) $(compilerFLAGS)

HFILES = $(wildcard inc/*.h)
CFILES = $(wildcard src/*.c)
#CFILES1 = $(wildcard src/*.c)
#CFILES = $(CFILES1:src/sbuf.c= )
OFILES = $(CFILES:.c=.o)

TARGET = libcasn1.a

MAKEFLAGS.ebuf	= $(CFLAGS) bufFLAGS=-DUSE_EXP_BUF b=ebuf ebuf
MAKEFLAGS.Cebuf	= $(CFLAGS) bufFLAGS=-DUSE_EXP_BUF b=Cebuf Cebuf

#-------------------------------------------------------------------------------
all:: $(OFILES) $(TARGET)

$(TARGET): $(OFILES)
	@echo *** Creating archive library ***
	$(AR) rv $@ $(OFILES) >/dev/null

$(OFILES): $(HFILES)

include ../maketail

clean:
	rm -f $(OFILES) >/dev/null 2>&1

install:
	(/bin/sh ../mkinstalldirs $(prefix)/include/esnacc/c; \
	for files in $(HFILES) ; do \
	 echo Installing $$files; \
	 /bin/sh ../install-sh -c $$files $(prefix)/include/esnacc/c; \
	done; \
	/bin/sh ../mkinstalldirs $(prefix)/lib; \
	/bin/sh ../install-sh -c $(TARGET) $(prefix)/lib )


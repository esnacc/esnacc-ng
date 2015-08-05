include ../makehead

DEPENDENCIES	=

TOP		= ..

CPPFLAGS	+= -I$(TOP) -Iinc $(bufFLAGS) $(compilerFLAGS) -DSNACCDLL_EXPORTS

ifeq ($(USETHREADS), no)
CPPFLAGS += -DNO_THREADS
endif

HFILES = $(wildcard inc/*.h)
CFILES1 = $(wildcard src/*.cpp)
#
# TEMPORARY REMOVE sm_buffer.cpp and sm_decodebuf.cpp
# from build.  These files are to be moved else where.
#
OFILES = $(CFILES1:.cpp=.o)

TARGET  = libc++asn1.a
TARGET2 = libc++asn1.$(SHEXT)

ifeq ($(USETHREADS), yes)
SNACCLIBS = -lpthread
else
SNACCLIBS =
endif

#-------------------------------------------------------------------------------
DEPEND = $(wildcard ./inc/*.h)

all: $(TARGET) $(TARGET2)

$(TARGET): $(OFILES)
	@echo *** Creating archive library ***
	$(AR) rv $@ $(OFILES) >/dev/null

$(TARGET2): $(OFILES)
	@echo *** Creating shared object library ***
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$@ -o $@ $(OFILES) $(SNACCLIBS)

$(OFILES): $(DEPEND)

clean:
	rm -f $(OFILES) >/dev/null 2>&1

install:
	(/bin/sh ../mkinstalldirs $(prefix)/include/esnacc/c++; \
	for files in inc/*.h; do \
	 echo Installing $$files; \
	 /bin/sh ../install-sh -c $$files $(prefix)/include/esnacc/c++;  \
	done; \
	/bin/sh ../mkinstalldirs $(prefix)/lib; \
	/bin/sh ../install-sh -c $(TARGET2) $(prefix)/lib )
#	/bin/sh ../install-sh -c $(TARGET) $(prefix)/lib;\

include ../maketail

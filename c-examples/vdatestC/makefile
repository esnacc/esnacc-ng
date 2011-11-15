
include ../../makehead

SMPDIST = ../../../SMPDist/
SNACC   =../../compiler/esnacc -c -E DER -I ./
TARGET  = ../snacctestc
TARGET2  = ../snacctestcso
ASNMODS = $(wildcard *.asn1)
ASNOBJS = $(ASNMODS:.asn1=.o)
ASN_H   = $(ASNMODS:.asn1=.h)
ASN_CPP = $(ASNMODS:.asn1=.c)
SRC     = $(wildcard *.c)
OBJS    = $(SRC:.c=.o)

.SUFFIXES : .asn1

INCLUDE = -I../../c-lib/inc -I../.. -I./ 
  
ifeq ($(USETHREADS), no)
CPPFLAGS = $(INCLUDE)
else
CPPFLAGS = $(INCLUDE) -DNO_THREADS
endif

all: 
	@echo "...building ASN.1 modules..."
	make $(ASN_CPP)
	make targets

targets: $(TARGET) $(TARGET2)

$(TARGET): $(ASNOBJS) $(OBJS)
ifeq ($(USETHREADS), no)
	$(CC) -o $@ $(OBJS) ../../c-lib/libcasn1.a -lm 
else
	$(CC) -o $@ $(OBJS) ../../c-lib/libcasn1.a -lm -lpthread
endif

$(TARGET2): $(ASNOBJS) $(OBJS)
ifeq ($(USETHREADS), no)
	$(CC) -o $@ $(OBJS) -L../../c-lib -lcasn1 -lm
else
	$(CC) -o $@ $(OBJS) -L../../c-lib -lcasn1 -lm -lpthread
endif
	@echo "**************************************************"
	@echo "**** Perform a 'make install' and make sure   ****"
	@echo "**** libcasn1.so is on your LD_LIBRARY_PATH ****"
	@echo "**** prior to executing 'snacctestcso'         ****"
	@echo "**************************************************"

##
## DEPENDENCIES FOR ASN.1 MODULES
##

.asn1.c: 
	$(SNACC) $< 

clean:
	rm -f $(OBJS) $(ASH_H) $(ASN_CPP) $(TARGET) $(TARGET2)

include ../../maketail

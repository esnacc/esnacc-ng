
include ../../makehead

SMPDIST = ../../../SMPDist/
SNACC   = ../../compiler/esnacc
TARGET  = ../constraints_teste
TARGET2  = ../constraints_testso
ASNMODS = $(wildcard *.asn1)
ASNOBJS = $(ASNMODS:.asn1=.o)
ASN_H   = $(ASNMODS:.asn1=.h)
ASN_CPP = $(ASNMODS:.asn1=.cpp)
SRC     = $(wildcard *.cpp)
OBJS    = $(SRC:.cpp=.o)

INCLUDE = -I../../c++-lib/inc -I../.. -I./ -I../src
  
ifeq ($(USETHREADS), no)
CPPFLAGS = $(INCLUDE) -DNO_THREADS
else
CPPFLAGS = $(INCLUDE)
endif

all: 
	@echo "...building ASN.1 modules..."
	make $(ASN_CPP)
	make targets

targets: $(TARGET) $(TARGET2)

$(TARGET): $(ASNOBJS) $(OBJS)
ifeq ($(USETHREADS), yes)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) ../../c++-lib/libc++asn1.a -lpthread
else
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) ../../c++-lib/libc++asn1.a $(LIBS)
endif

$(TARGET2): $(ASNOBJS) $(OBJS)
ifeq ($(USETHREADS), yes)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) -L../../c++-lib -lc++asn1 -lpthread
else
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) -L../../c++-lib -lc++asn1 $(LIBS)
endif
	@echo "**************************************************"
	@echo "**** Perform a 'make install' and make sure   ****"
	@echo "**** libc++asn1.so is on your LD_LIBRARY_PATH ****"
	@echo "**** prior to executing 'snacctestso'         ****"
	@echo "**************************************************"

##
## DEPENDENCIES FOR ASN.1 MODULES
##

.SUFFIXES : .asn1 .cpp .asn

$(ASN_CPP): $(SNACC)

.asn1.cpp: 
	$(SNACC) -C -I . $< 

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET2)

clean-asn:
	rm -f $(ASN_H) $(ASN_CPP)

include ../../maketail

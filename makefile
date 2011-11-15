#! /bin/sh

include ./makehead

#-------------------------------------------------------------------------------
SUBDIRS = c-lib c++-lib compiler c-examples c++-examples

all: all-redirect 
	@for i in $(SUBDIRS) ; do \
		echo "Entering Directory $$I"; \
		(cd $(top_builddir)/$$i; $(MAKE) $(MFLAGS)); \
	done

clean:
	@for i in $(SUBDIRS) ; do \
		echo "Entering Directory $$I"; \
		(cd $(top_builddir)/$$i; $(MAKE) clean); \
	done

.SUFFIXES:

install: .PHONY
	@echo "Making install in `pwd`"
	  @for i in $(SUBDIRS) ; do  \
		echo "Entering Directory $$i"; \
		(cd $(top_builddir)/$$i; $(MAKE) $(MFLAGS) install ); \
	  done 

distclean:
	@rm -f makehead config.h config.cache config.log config.status stamp-h >/dev/null 2>&1


.PHONY: all-redirect

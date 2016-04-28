# Copyright (C) 2016 Aaron Conole
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without warranty of any kind.

RPMBUILD_TOP ?= $(abs_top_builddir)/rpm/rpmbuild

EXTRA_DIST += redhat/esnacc.spec.in \
	redhat/esnacc.spec

$(srcdir)/redhat/esnacc.spec: redhat/esnacc.spec.in

rpm: dist redhat/esnacc.spec
	${MKDIR_P} ${RPMBUILD_TOP}/SOURCES
	cp ${DIST_ARCHIVES} ${RPMBUILD_TOP}/SOURCES
	rpmbuild ${RPMBUILD_OPT} \
                 -D "_topdir ${RPMBUILD_TOP}" \
                 -bb $(srcdir)/redhat/esnacc.spec

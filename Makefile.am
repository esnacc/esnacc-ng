# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the GNU General Public License v2 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
#
# Please note, this may NOT be relicensed under any version of the
# GNU General Public License, other than the one specified.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -Wstrict-prototypes
AM_CFLAGS += $(WARNING_FLAGS)

# if NDEBUG
# AM_CPPFLAGS += -DNDEBUG
# AM_CFLAGS += -fomit-frame-pointer
# endif

# if WIN32
# psep=";"
# else
# psep=":"
# endif

ALL_LOCAL =
BUILT_SOURCES =
CLEANFILES =
CLEAN_LOCAL =
DISTCLEANFILES =

docs = \
	README.md \
	licenseq.txt \
	install.txt \
	NEWS \
	copying

EXTRA_DIST = \
	$(docs) \
	.travis.yml \
	scan_script.sh \
	doc/PERCompilerRequirementsMatrix.doc \
	doc/eSNACCManuals.zip

bin_PROGRAMS =
sbin_PROGRAMS =
bin_SCRIPTS =
DIST_HOOKS =
dist_man_MANS =
dist_pkgdata_DATA =
dist_pkgdata_SCRIPTS =
dist_sbin_SCRIPTS =
dist_scripts_SCRIPTS =
dist_scripts_DATA =
INSTALL_DATA_LOCAL =
UNINSTALL_LOCAL =
man_MANS =
MAN_FRAGMENTS =
MAN_ROOTS =
noinst_DATA =
noinst_HEADERS =
lib_LTLIBRARIES =
noinst_man_MANS =
noinst_PROGRAMS =
noinst_SCRIPTS =
pkgdata_DATA =
sbin_SCRIPTS =
scripts_SCRIPTS =
completion_SCRIPTS =
scripts_DATA =
SUFFIXES =
check_DATA =
check_SCRIPTS =
pkgconfig_DATA =
FLAKE8_PYFILES =
check_PROGRAMS =

scriptsdir = $(pkgdatadir)/scripts
completiondir = $(sysconfdir)/bash_completion.d
pkgconfigdir = $(libdir)/pkgconfig

# This ensures that files added to EXTRA_DIST are always distributed,
# even if they are inside an Automake if...endif conditional block that is
# disabled by some particular "configure" run.  For more information, see:
# http://article.gmane.org/gmane.comp.sysutils.automake.general/10891
noinst_HEADERS += $(EXTRA_DIST)

.asn1.c:
	compiler/esnacc -c $<
	mv -f $(notdir $(basename $<)).{h,c} $(dir $@)

.asn1.cpp:
	compiler/esnacc -D $(@D) -C $(notdir $<)

include c-lib/automake.mk
include cxx-lib/automake.mk
include compiler/automake.mk
include c-examples/automake.mk

# Copyright (c) 2016, Aaron Conole
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

m4_define([esnacc_major], [1])
m4_define([esnacc_minor], [8])
m4_define([esnacc_micro], [99])
m4_define([esnacc_interface_age], [0])
m4_define([esnacc_binary_age], [m4_eval(100 * esnacc_minor + esnacc_micro)])
m4_define([esnacc_binary_compat], [1.7.4])

m4_define([esnacc_version], [esnacc_major.esnacc_minor.esnacc_micro])

AC_INIT([esnacc], [esnacc_version])
AC_PREREQ([2.60])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_INSTALL
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AC_PROG_CXX
AC_USE_SYSTEM_EXTENSIONS
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LN_S
AC_PROG_LEX
if test "x$LEX" != xflex; then
  LEX="$SHELL $missing_dir/missing flex"
  AC_SUBST([LEX_OUTPUT_ROOT], [lex.yy])
  AC_SUBST([LEXLIB], [''])
fi
AC_PROG_SED
AC_PROG_YACC
AC_PROG_INSTALL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_STDC
LT_INIT
AC_DEFINE([__STDC_LIMIT_MACROS],[1],[C99 requires this define])
AC_CHECK_LIB([m], [pow])
AC_CHECK_FUNCS([strchr getchar memset memcmp memcpy])
AX_PTHREAD
ESNACC_CHECK_WIN32
ESNACC_CHECK_WARNINGS
AX_CODE_COVERAGE
AS_IF([ test "$enable_code_coverage" = "yes"], [
    CFLAGS=$CODE_COVERAGE_CFLAGS
    CXXFLAGS=$CODE_COVERAGE_CXXFLAGS
    LDFLAGS=$CODE_COVERAGE_LDFLAGS
])
AC_CHECK_XSLTPROC

#libtool version interface
m4_define([lt_current],
          [m4_eval(100 * esnacc_minor + esnacc_micro - esnacc_interface_age)])
m4_define([lt_revision], [esnacc_interface_age])
m4_define([lt_age], [m4_eval(esnacc_binary_age - esnacc_interface_age)])
LT_VERSION_INTF="lt_current:lt_revision:lt_age"
AC_SUBST(LT_VERSION_INTF)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT



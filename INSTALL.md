How to install Enhanced Sample Neufeld ASN.1 C Compiler
=======================================================

This document describes the process for building and installing the esnacc
package, generically, on any system which has autotools support, and the
appropriate C and C++ compilers.


Build Requirements
------------------

To compile the software for the Enhanced Sample Neufeld ASN.1 C Compiler,
you will need:

 - GNU Make

 - Supported C and C++ compilers

   * Clang

   * GCC 4.x and GCC 5.x

   * MSVC 2013

   While esnacc may build with other C and C++ compilers, the ones listed above
   are tested more thoroughly.  It is therefore recommended to use one of the
   compilers listed above.

 - flex, or lex

 - bison, or yacc

If you are working from a Git tree or snapshot (instead of from a
distribution tarball), or if you modify the esnacc build system
you will also need the following software:

  - Autoconf version 2.63 or later.

  - Automake version 1.10 or later.

  - libtool version 2.4 or later.  (Older versions might work too.)


Building and Installing Enhanced Sample Neufeld ASN.1 C Compiler
================================================================

Once you have installed all the prerequisites listed above, you
may follow the procedure below to build the configuration and build
systems.

Bootstrapping the sources
-------------------------

This step is not needed if you have downloaded a released tarball.
If you pulled the sources directly from the esnacc Git tree or
got a Git tree snapshot, then run boot.sh in the top source directory
to build the "configure" script.

      `% ./boot.sh`

Configuring the Sources
-----------------------

Configure the package by running the configure script.  You can
usually invoke configure without any arguments.  For example:

      `% ./configure`

By default all files are installed under /usr/local.  esnacc also
expects to place header files under /usr/local/include/c-lib/inc
and /usr/local/include/cxx-lib/inc by default.  If you want to install
files under /usr instead (for example), then add options as shown
below:

      `% ./configure --prefix=/usr`

Note that distributions which ship esnacc will be using the various
options to configure the various binary directories.

By default, static libraries are built and linked against.  If you
want to use shared libraries instead:

      `% ./configure --enable-shared`

To use a specific C compiler for compiling the esnacc software, also
specify it on the configure command line, like so:

      `% ./configure CC=specific-c-compiler`

To use a specific C++ compiler for compiling the esnacc software,
specify it on the configure command line, like so:

      `% ./configure CXX=specific-cxx-compiler`

These options may be combined, for example:

      `% ./configure CC=specific-c-compiler CXX=specific-cxx-compiler`

To supply special flags to the C compiler, specify them as CFLAGS on
the configure command line.  If you want the default CFLAGS, which
include "-g" to build debug symbols and "-O2" to enable optimizations,
you must include them yourself.  For example, to build with the
default CFLAGS plus "-mssse3", you might run configure as follows:

      `% ./configure CFLAGS="-g -O2 -mssse3"`

Similarly, to configure the C++ flags, customize the CXXFLAGS variable
as follows:

      `% ./configure CXXFLAGS="-g -O2 -mssse3"`

These variables may be combined, as with the CC and CXX variables.

The configure script accepts a number of other options and honors
additional environment variables.  For a full list, invoke
configure with the --help option.

You can also run configure from a separate build directory.  This
is helpful if you want to build esnacc in more than one way
from a single source directory, e.g. to try out both GCC and Clang
compilers. Here is an example:

      `% mkdir _gcc && (cd _gcc && ../configure CC=gcc)`
      `% mkdir _clang && (cd _clang && ../configure CC=clang)`


Building the Sources
--------------------

1. Run GNU make in the build directory, e.g.:

      `% make`

   or if GNU make is installed as "gmake":

      `% gmake`

   If you used a separate build directory, run make or gmake from that
   directory, e.g.:

      `% make -C _gcc`
      `% make -C _clang`

   Some versions of Clang and ccache are not completely compatible.
   If you see unusual warnings when you use both together, consider
   disabling ccache for use with Clang.

2. Consider running the testsuite.  Refer to "Running the Testsuite"
   below, for instructions.

3. Become root by running "su" or another program.

4. Run "make install" to install the executables and manpages into the
   running system, by default under /usr/local.


Packages
========

esnacc includes a Debian compatible rules set, which can be used to generate
appropriate .deb files for installation on any Debian or Debian-derived system
(such as Ubuntu).  In order to build these packages, it is required that all
prerequisites listed are installed.  This can be done using the `apt-get`
and `dpkg` utilities.  For all builds, you must install at least the "cdbs,"
"build-essential," "dh-autoreconf," and "fakeroot" packages.  This can be done,
e.g. with:

   `apt-get install build-essential cdbs dh-autoreconf fakeroot`

It is then necessary to install all of the packages listed in the Build-Depends
line in the debian/control file.  You can install these similarly e.g. with
`apt-get install`.

Check that you have installed all prerequisites by running
`dpkg-checkbuilddeps` from the top-level of the esnacc directory.  If
everything is installed correctly, dpkg-checkbuilddeps will exit without
printing an error message. If you are missing any dependencies, they will
be listed.

To build the packages, run `fakeroot debian/rules binary` from the top-level
esnacc directory.  This will do a build, including executing the targets of
`make check`, and produce a number of .deb files in the parent directory.

To install these built packages, use `dpkg -i`.


Testsuites
==========

This section describe esnacc's built-in support for various test suites.
You must bootstrap, configure and build esnacc (steps are in
"Building and Installing Enhanced Sample Neufeld ASN.1 C Compiler"
above) before you run the tests described here.  You do not need to
install esnacc in order to run these test suites.  You do not need supervisor
privilege to run these test suites.

Self-Tests
----------

esnacc includes a suite of self-tests.  Before you submit patches
upstream, we advise that you run the tests and ensure that they pass.
If you add new features to esnacc, then adding tests for those
features will ensure your features don't break as developers modify
other areas.

Refer to "Testsuites" above for prerequisites.

To run all the unit tests in esnacc, one at a time:
      `make check`

Continuous Integration with Travis-CI and AppVeyor
--------------------------------------------------

A .travis.yml file is provided to automatically build esnacc with
various build configurations and run the testsuite using travis-ci.
Builds will be performed with gcc, and clang for linux, as well as
Mac OS X.  Additionally, an appveyor.yml file is provided to ensure
builds under the Microsoft Windows operating system.

The CI build is triggered via git push (regardless of the specific
branch) or pull request against any esnacc GitHub repository that
is linked to travis-ci.

Instructions to setup travis-ci for your GitHub repository:

1. Go to http://travis-ci.org/ and sign in using your GitHub ID.
2. Go to the "Repositories" tab and enable the ovs repository. You
   may disable builds for pushes or pull requests.
3. In order to avoid forks sending build failures to the upstream
   mailing list, the notification email recipient is encrypted. If you
   want to receive email notification for build failures, replace the
   the encrypted string:
   3.1) Install the travis-ci CLI (Requires ruby >=2.0):
           gem install travis
   3.2) In your esnacc-ng repository:
           travis encrypt mylist@mydomain.org
   3.3) Add/replace the notifications section in .travis.yml and fill
        in the secure string as returned by travis encrypt:

         notifications:
           email:
             recipients:
               - secure: "....."

   (You may remove/omit the notifications section to fall back to
    default notification behaviour which is to send an email directly
    to the author and committer of the failing commit. Note that the
    email is only sent if the author/committer have commit rights for
    the particular GitHub repository).

4. Pushing a commit to the repository which breaks the build or the
   testsuite will now trigger a email sent to mylist@mydomain.org


Bugs
====

Please report bugs to
[the esnacc github repository](https://github.com/esnacc/esnacc-ng).

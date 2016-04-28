eSNACC Project Ideas
====================

This file lists a number of items and ideas for enhancing eSNACC.

Programming Project Ideas
=========================

Each of these projects should result in one or more patches submitted to
to the esnacc project (using the GitHub interface, for now).

Fedora Spec File
----------------

One existed for snacc - just need to get it and update it for esnacc.

eSNACC m4 automake rules
------------------------

It would be really good if there was an esnacc.m4 that would populate
makefile entries with the appropriate rules and targets to build .asn1
files using esnacc.

Hook up the Examples
--------------------

This is an ongoing effort to fix up and re-enable the various c and c++
language examples.  They are all there under c-examples and c++-examples.

Steps for this task:

 1. Ensure that all of the c-examples are compiling
 2. Rename c++-examples to cxx-examples.
 3. Hook up the c++ examples
 4. (Stretch) Have the code generator generate "fuzz tests" or test
    code that can be exercised in a unit test to validate encoding,
    saving and decoding.

Re-enable tcl support
---------------------

During the conversion to esnacc, support for TCLSH interfaces was dropped,
mostly due to testing compatibility reasons.  This should be re-enabled so
that esnacc can get feature parity again.

Python, C#, golang, and JS backend
----------------------------------

eSNACC should really have backends for these (and other) popular languages.
Minimally, BER support should go along with those languages, and if possible
adding addtional encoding rule support should be done.  This requires mostly
copying existing boilerplate and modifying it... which really means

Template Generation Engine
--------------------------

It would be better to just refactor the back-end system to use template
files for generation.  That way, there's no need to even bother with additional
specific backend code - just write new templates.  This requires modifying the
code emitting system to support a new template language and extending that
language to be future capable, but it really is a better design overall.

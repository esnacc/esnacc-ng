# Copyright (C) 2017, Aaron Conole <aconole@bytheb.org>

esnaccpylib_PYTHON =
esnaccpylibdir = $(pkgpythondir)

if HAVE_PYTHON
esnaccpylib_PYTHON += \
	py-lib/esnacc/__init__.py \
	py-lib/esnacc/asn_base.py \
	py-lib/esnacc/asn_bool.py \
	py-lib/esnacc/asn_buffer.py \
	py-lib/esnacc/asn_ints.py \
	py-lib/esnacc/asn_list.py \
	py-lib/esnacc/asn_octs.py \
	py-lib/esnacc/asn_useful.py
endif


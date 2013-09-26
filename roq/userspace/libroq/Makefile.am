
lib_LTLIBRARIES = src/libroq.la

AM_CFLAGS = -g -Wall -D_GNU_SOURCE
AM_CFLAGS += -I$(top_srcdir)/../../microcode/include
ACLOCAL_AMFLAGS = -I m4


if HAVE_LD_VERSION_SCRIPT
	roq_version_script = -Wl,--version-script=$(srcdir)/src/roq.map
else
	roq_version_script =
endif

src_libroq_la_SOURCES = src/roq_verbs.c src/roq.c src/roq_uverbs.c

src_libroq_la_LDFLAGS = -avoid-version -release @IBV_DEVICE_LIBRARY_EXTENSION@ \
    $(roq_version_script)

roqconfdir = $(sysconfdir)/libibverbs.d
roqconf_DATA = roq.driver

RPM_DIR			:= $(CURDIR)/../../..
rpm:
	rpmbuild -bb \
		--define "_topdir $(RPM_DIR)"  \
		--define "ROQLIB_DIR $(CURDIR)" \
		./roq-library.spec

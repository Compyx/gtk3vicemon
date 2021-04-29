# vim: set noet ts=8 sw=8 sts=8:
#
# root Makefile.am for gtk3vicemon

SUBDIRS = mon ui


bin_PROGRAMS = gtk3vicemon

AM_CPPFLAGS = \
	@MON_CPPFLAGS@ \
	-I$(top_srcdir)/src/ui \
	-I$(top_srcdir)/src/mon
AM_CFLAGS = @MON_CFLAGS@
AM_LDFLAGS = @MON_LDFLAGS@

LDADD = $(top_builddir)/src/ui/libui.a \
	$(top_builddir)/src/mon/libmon.a

gtk3vicemon_SOURCES = \
	main.c \
	app-resources.c \
	app-resources.h \
	hexdump.c \
	log.c \
	settings.c


EXTRA_DIST = \
	debug.h \
	hexdump.h \
	log.h \
	settings.h \
	vicemonapi.h

RESOURCE_FILES = \
	$(top_srcdir)/data/ui/app-menu.xml


app-resources.c: $(top_srcdir)/data/resources.xml $(RESOURCE_FILES)
	glib-compile-resources \
		--sourcedir=$(srcdir) \
		--sourcedir=$(top_builddir)/data \
		--target=$@ \
		--generate-source \
		--c-name app \
		--manual-register \
		$(top_srcdir)/data/resources.xml

app-resources.h: $(top_srcdir)/data/resources.xml $(RESOURCE_FILES)
	glib-compile-resources \
		--sourcedir=$(srcdir) \
		--sourcedir=$(top_builddir)/data \
		--target=$@ \
		--generate-header \
		--c-name app \
		--manual-register \
		$(top_srcdir)/data/resources.xml

BUILT_SOURCES = \
	app-resources.c \
	app-resources.h


clean-local:
	rm -f $(BUILT_SOURCES)


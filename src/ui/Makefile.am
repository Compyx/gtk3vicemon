# vim: set noet ts=8 sw=8 sts=8:
#
# Makefile.am for /src/ui

AM_CPPFLAGS = \
	@MON_CPPFLAGS@ \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/mon \
	-I$(top_srcdir)/src/ui

AM_CFLAGS = @MON_CFLAGS@
AM_LDFLAGS = @MON_LDFLAGS@


noinst_LIBRARIES = libui.a


#LDADD = $(top_builddir)/src/mon/libmon.a

libui_a_SOURCES = \
	appwindow.c \
	statusbar.c \
	connection-widget.c

EXTRA_DIST = \
	appwindow.h \
	statusbar.h \
	connection-widget.h

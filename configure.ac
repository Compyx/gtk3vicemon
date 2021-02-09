dnl vim: set et ts=4 sw=4 sts=4:
dnl This file is part of gtk3vicemon, the standalone VICE monitor.
dnl See README for copyright notice.


dnl Init autoconf
AC_INIT([gtk3vicemon], [0.0.0])
AC_CONFIG_SRCDIR([src/main.c])
dnl Init automake
AM_INIT_AUTOMAKE([-Wno-portability foreign])

dnl Make sure autotools doesn't add '-g -O2' by default when the user's CFLAGS
dnl or CXXFLAGS are empty
dnl This has to happen before any other AC_* calls, so leave it here please.
: ${CFLAGS=""}
: ${CXXFLAGS=""}
: ${OBJCFLAGS=""}

dnl Set gtk3vicemon compiler flags (can be overridden by the user)
MON_CPPFLAGS=""
MON_CFLAGS="-g -O3 -Wall -Wextra"
MON_LDFLAGS=""

dnl Check for toolchain and extra programs

dnl Compiler check
AC_LANG(C)
AC_PROG_CC([gcc clang cc])
AC_PROG_CC_C99

dnl Check for pkg-config
PKG_PROG_PKG_CONFIG

dnl Check for xmllint (to lint the resource XML files via glib-compile-resources)
AC_CHECK_PROG([HAVE_XMLLINT], [xmllint], [yes], [no])

dnl Check for glib-compile-resources
AC_CHECK_PROG([HAVE_GLIB_COMPILE_RESOURCES], [glib-compile-resources], [yes], [no])

AC_PROG_RANLIB

dnl
dnl Check for libraries
dnl

dnl Gtk3 >= 3.24
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.24])
MON_CFLAGS="$MON_CFLAGS $GTK_CFLAGS"
MON_LDFLAGS="$MON_LDFLAGS $GTK_LIBS"

AC_SUBST(MON_CPPFLAGS)
AC_SUBST(MON_CFLAGS)
AC_SUBST(MON_LDFLAGS)

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
    Makefile
    data/Makefile
    data/ui/Makefile
    src/Makefile
    src/ui/Makefile
])
AC_OUTPUT


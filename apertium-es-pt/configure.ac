#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.52)

AC_INIT([apertium-es-pt], [1.0.3], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-es-pt], [1.0.3], no-define)
AC_PROG_LN_S
AC_PROG_AWK
AC_CONFIG_HEADER([config])

PKG_CHECK_MODULES(APERTIUM, apertium-3.0 >= 3.0.0)

APERTIUM_VER=30

AC_SUBST(APERTIUM_VER)

AC_OUTPUT([Makefile])


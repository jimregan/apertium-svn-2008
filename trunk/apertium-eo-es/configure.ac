#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.52)

m4_define([required_apertium_version], [3.0.0])

AC_INIT([apertium-eo-es], [0.8], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-eo-es], [0.8], no-define)
AC_PROG_LN_S
AC_PROG_AWK
AC_CONFIG_HEADER([config])


PKG_CHECK_MODULES(apertium, [dnl
  apertium-3.0 >= required_apertium_version])

AC_OUTPUT([Makefile])

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.52)

m4_define([required_apertium_version], [2.0.0])

AC_INIT([apertium-es-ro], [0.7.0], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-es-ro], [0.7.0], no-define)
AC_PROG_LN_S
AC_PROG_AWK
AC_CONFIG_HEADER([config])

PKG_CHECK_MODULES(apertium, [dnl
  apertium-3.0 >= required_apertium_version])
  
AC_OUTPUT([Makefile])

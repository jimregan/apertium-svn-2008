#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)

m4_define([required_apertium_version], [1.9.0])

AC_INIT([apertium-oc-es], [1.0.2], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-oc-es], [1.0.2], no-define)
AC_PROG_LN_S
AC_PROG_AWK
AC_CONFIG_HEADER([config])

AC_DEFUN(PKG_CHECK_APERTIUM, [
  APERTIUM_VER=0
  PKG_CHECK_MODULES(APERTIUM, apertium-1.0 >= 1.0.0,
  [
      APERTIUM_VER=10
  ],
  [
      PKG_CHECK_MODULES(APERTIUM, apertium-2.0 >= 2.0.0,
      [
          APERTIUM_VER=20
      ],
      [
          PKG_CHECK_MODULES(APERTIUM, apertium-3.0 >= 3.0.0)
          APERTIUM_VER=30
      ])
  ])
  AC_SUBST(APERTIUM_VER)
])

PKG_CHECK_APERTIUM

if test $APERTIUM_VER = 10;
then MODES="";
else MODES="modes";
fi

AC_SUBST(MODES)

if test -d .deps
then echo "";
else mkdir .deps
fi

AC_OUTPUT([Makefile])

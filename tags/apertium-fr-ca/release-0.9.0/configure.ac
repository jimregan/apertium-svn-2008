#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.52)

AC_INIT([apertium-fr-ca], [0.9], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-fr-ca], [0.9], no-define)
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
      PKG_CHECK_MODULES(APERTIUM, apertium-1.9 >= 1.9.0,
      [
          APERTIUM_VER=19
      ],
      [
          PKG_CHECK_MODULES(APERTIUM, apertium-2.0 >= 2.0.0)
          APERTIUM_VER=20
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

AC_OUTPUT([Makefile])

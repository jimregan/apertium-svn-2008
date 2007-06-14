#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.52)

AC_INIT([apertium-en-ca], [0.8.1], [sortiz@users.sourceforge.net])
AM_INIT_AUTOMAKE([apertium-en-ca], [0.8.1], no-define)
AC_PROG_LN_S
AC_PROG_AWK
AC_CONFIG_HEADER([config])

AC_DEFUN(PKG_CHECK_APERTIUM, [
  APERTIUM_VER=0
  PKG_CHECK_MODULES(APERTIUM, apertium-1.9 >= 1.9.0,
  [
      APERTIUM_VER=19
  ],
  [
      PKG_CHECK_MODULES(APERTIUM, apertium-2.0 >= 2.0.0)
      APERTIUM_VER=20
  ])
  AC_SUBST(APERTIUM_VER)
])
                          
PKG_CHECK_APERTIUM

AC_PATH_PROG(XSLTPROC, xsltproc, no)
if test x$ac_cv_path_XSLTPROC = x
then
  AC_MSG_ERROR([You don't have xsltproc installed.])
fi
if test x$ac_cv_path_XSLTPROC = xno
then
  AC_MSG_ERROR([You don't have xsltproc installed.])
fi
                     
  
AC_OUTPUT([Makefile])

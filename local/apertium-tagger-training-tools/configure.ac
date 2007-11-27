#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)

m4_define([required_lttoolbox_version], [2.0])
m4_define([required_apertium_version], [2.0])
m4_define([required_libxml_version], [2.6.17])

AC_INIT([apertium-tagger-training-tools], [1.0.0], [fsanchez@dlsi.ua.es])

AC_CONFIG_HEADERS([src/configure.H])

# Release versioning
GENERIC_MAJOR_VERSION=1
GENERIC_MINOR_VERSION=0
GENERIC_MICRO_VERSION=0

GENERIC_API_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_API_VERSION)

PACKAGE=apertium-tagger-training-tools

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)

# Checks for programs.

AC_MSG_CHECKING([Compilation architecture: PPC, i686, x86_64, Other])
if test x$(which arch) = x
then ARCH=$($(which uname) -m)
else ARCH=$($(which arch))
fi

if test x$ARCH = xppc
then
  AC_MSG_RESULT([PowerPC])
  CFLAGS="-Wall -ansi -mpowerpc -maltivec -fno-pic -fomit-frame-pointer $CFLAGS"
  CXXFLAGS="-Wall -ansi -mpowerpc -maltivec -fno-pic -fomit-frame-pointer $CXXFLAGS"
else
  if test x$ARCH = xi686
  then
    AC_MSG_RESULT([i686])
    CFLAGS="-Wall -ansi -march=i686 -O3 -fomit-frame-pointer -funroll-loops $CFLAGS"
    CXXFLAGS="-Wall -ansi -march=i686 -O3 \
              -fomit-frame-pointer -funroll-loops $CXXFLAGS"
  
  else
    if test x$ARCH = xx86_64
    then
      AC_MSG_RESULT([x86_64])
      CFLAGS="-Wall -ansi -O3 -mtune=nocona -fomit-frame-pointer -funroll-loops $CFLAGS"
      CXXFLAGS="-Wall -ansi -O3 -mtune=nocona \
              -fomit-frame-pointer -funroll-loops $CXXFLAGS"
    else
      AC_MSG_RESULT([Other])
      CFLAGS="-Wall -ansi -O3 $CFLAGS"
      CXXFLAGS="-Wall -ansi -O3 $CXXFLAGS"
    fi
  fi
fi

AC_PROG_CXX
AC_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

AC_ARG_ENABLE(debug,
              [  --enable-debug    Enable "-g -Wall" compiler options], 
              [CXXFLAGS="-g -Wall"; CFLAGS="-g -Wall"])

AC_ARG_ENABLE(profile,
              [  --enable-profile  Enable "-pg -g -Wall" compiler options],
              [CXXFLAGS="-pg -g -Wall"; CFLAGS="-pg -g -Wall"; LDFLAGS="-pg"])

if test x$ARCH = xppc
then
  AC_PATH_PROG(XSLTPROC, sabcmd, no)
  if test x$ac_cv_path_XSLTPROC = x
  then
    AC_MSG_ERROR([You don't have sablotron installed.])
  fi
  if test x$ac_cv_path_XSLTPROC = xno
  then
    AC_MSG_ERROR([You don't have sablotron installed.])
  fi
else 
  AC_PATH_PROG(XSLTPROC, xsltproc, no)
  if test x$ac_cv_path_XSLTPROC = x
  then
    AC_MSG_ERROR([You don't have xsltproc installed.])
  fi
  if test x$ac_cv_path_XSLTPROC = xno
  then
    AC_MSG_ERROR([You don't have xsltproc installed.])
  fi
fi

AC_PATH_PROG(BASH, bash, no)
if test x$ac_cv_path_BASH = x
then
  AC_MSG_ERROR([You don't have bash installed.])
fi
if test x$ac_cv_path_BASH = xno
then
  AC_MSG_ERROR([You don't have bash installed.])
fi

AC_PATH_PROG(PERL, perl, no)
if test x$ac_cv_path_PERL = x
then
  AC_MSG_ERROR([You don't have perl installed.])
fi
if test x$ac_cv_path_PERL = xno
then
  AC_MSG_ERROR([You don't have perl installed.])
fi
    
AC_PATH_PROG(APERTIUM_DESTXT, apertium-destxt, no)
AC_PATH_PROG(LTPROC, lt-proc, no)

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test x$ac_cv_path_PKG_CONFIG = x
then
  AC_MSG_ERROR([You don't have pkg-config installed.])
fi
if test x$ac_cv_path_PKG_CONFIG = xno
then
  AC_MSG_ERROR([You don't have pkg-config installed.])
fi

PKG_CHECK_MODULES(LIBXML2, [dnl
  libxml-2.0 >= required_libxml_version])

PKG_CHECK_MODULES(LTTOOLBOX, [dnl
  lttoolbox-2.0 >= required_lttoolbox_version])
  
PKG_CHECK_MODULES(APERTIUM, [dnl
  apertium-2.0 >= required_apertium_version])

#Check for libraries.
AC_CHECK_LIB(xml2, xmlReaderForFile)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([regex.h sys/types.h sys/times.h sys/wait.h sys/resource.h locale.h getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC

#AC_CHECK_DECLS([fread_unlocked, fwrite_unlocked, fgetc_unlocked, fputc_unlocked, fputs_unlocked, getopt_long])
AC_CHECK_DECLS([getopt_long])

#AC_CHECK_FUNCS([setlocale strdup])
AC_CHECK_FUNCS([setlocale])

AC_OUTPUT([Makefile src/Makefile apertium-tagger-training-tools-1.0.pc])

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.52)

m4_define([required_libxml_version], [2.6.17])

AC_INIT([lttoolbox/lttoolbox.h], [3.0.2], [sortiz@users.sourceforge.net])
AC_CONFIG_HEADER([lttoolbox/lttools_config.h])

GENERIC_LIBRARY_NAME=lttoolbox

# Release versioning
GENERIC_MAJOR_VERSION=3
GENERIC_MINOR_VERSION=0
GENERIC_MICRO_VERSION=2

# API version (often = GENERIC_MAJOR_VERSION.GENERIC_MINOR_VERSION)
GENERIC_API_VERSION=$GENERIC_MAJOR_VERSION
AC_SUBST(GENERIC_API_VERSION)

# Shared library versioning
GENERIC_LIBRARY_VERSION=0:0:0
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed

AC_SUBST(GENERIC_LIBRARY_VERSION)
PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

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
AM_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

AC_ARG_ENABLE(debug,
              [  --enable-debug  Enable "-g -Wall" compiler options],
              [CXXFLAGS="-g -Wall";CFLAGS="-g -Wall"])

AC_ARG_ENABLE(profile,
              [  --enable-profile  Enable "-pg -g -Wall" compiler options],
              [CXXFLAGS="-pg -g -Wall"; CFLAGS="-pg -g -Wall"; LDFLAGS="-pg"])
                            

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test x$ac_cv_path_PKG_CONFIG = x
then
  AC_MSG_ERROR([You don't have pkg-config installed])
fi
if test x$ac_cv_path_PKG_CONFIG = xno
then
  AC_MSG_ERROR([You don't have pkg-config installed])
fi

PKG_CHECK_MODULES(LTTOOLBOX, [dnl
  libxml-2.0 >= required_libxml_version])

# Checks for libraries.
AC_CHECK_LIB(xml2, xmlReaderForFile)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h stddef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CHECK_DECLS([fread_unlocked, fwrite_unlocked, fgetc_unlocked, \ 
                fputc_unlocked, fputs_unlocked, \
                fgetwc_unlocked, fputwc_unlocked, fputws_unlocked])
                
AC_CHECK_FUNCS([setlocale strdup getopt_long])

AC_OUTPUT([Makefile lttoolbox-3.0.pc lttoolbox/Makefile])

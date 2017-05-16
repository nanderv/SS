AC_INIT([MACS2],[0.1],[j.j.g.meijer@utwente.nl])
AC_CONFIG_SRCDIR([src/macs2.c])
AM_INIT_AUTOMAKE([1.14 -Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC_C99
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

PKG_PROG_PKG_CONFIG
m4_pattern_forbid([^PKG_[A-Z_]+$], [missing some pkgconf macros (pkgconf package)])

AS_IF([test x"$PKG_CONFIG" = x], [AC_MSG_FAILURE([Please install pkgconf])])

PKG_CHECK_MODULES([SYLVAN], [sylvan >= 1.2],,
    [AC_MSG_FAILURE([Sylvan >= 1.2 is not installed.])])

AC_PROG_LEX
AX_PROG_BISON([BISON="$YACC"],[AC_MSG_FAILURE([Please install Bison])])
AX_PROG_BISON_VERSION([3.0.2],,[AC_MSG_FAILURE([Please install Bison >= 3.0.2])])

#AC_CHECK_LIB([popt],[poptGetContext],,[AC_MSG_FAILURE([popt not found.])])
#AC_CHECK_HEADER([popt.h],[], [AC_MSG_FAILURE([popt.h not found.])])

PKG_CHECK_MODULES([XML], [libxml-2.0],,[
    AC_MSG_FAILURE(libxml2 is not installed. Please install libxml2.)
]) 

AC_MSG_NOTICE([All is okay :)])

AC_OUTPUT


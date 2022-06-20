dnl Process this file with autoconf to produce a configure script.
AC_INIT(GUI.cpp)

dnl Set various version strings - taken gratefully from the GTk sources

# Making releases:
#   MICRO_VERSION += 1;
#   INTERFACE_AGE += 1;
#   BINARY_AGE += 1;
# if any functions have been added, set INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set BINARY_AGE and INTERFACE_AGE to 0.

MAJOR_VERSION=2
MINOR_VERSION=0
MICRO_VERSION=0
INTERFACE_AGE=0
BINARY_AGE=0
VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)
AC_SUBST(INTERFACE_AGE)
AC_SUBST(BINARY_AGE)
AC_SUBST(VERSION)

# libtool versioning
LT_RELEASE=$MAJOR_VERSION.$MINOR_VERSION
LT_CURRENT=`expr $MICRO_VERSION - $INTERFACE_AGE`
LT_REVISION=$INTERFACE_AGE
LT_AGE=`expr $BINARY_AGE - $INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

dnl Detect the canonical build and host environments
AC_CANONICAL_HOST

dnl Setup for automake
AM_INIT_AUTOMAKE(GUIlib, $VERSION)

dnl Check for tools

AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_INSTALL
LT_INIT([win32-dll])

dnl Check for SDL
SDL_VERSION=2.0.0
AM_PATH_SDL2($SDL_VERSION,
            :,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_MSG_CHECKING(for GCC -Wall option)
save_CFLAGS="$CFLAGS"
CFLAGS="$save_CFLAGS -Wall"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([int x = 0;])], [have_gcc_Wall=yes],[have_gcc_Wall=no])
AC_MSG_RESULT($have_gcc_Wall)
CFLAGS="$save_CFLAGS"
if test x$have_gcc_Wall = xyes; then
    CFLAGS="$CFLAGS -Wall"

dnl Haiku headers use multicharacter constants all over the place. Ignore these warnings when using -Wall.
    AC_MSG_CHECKING(for necessary GCC -Wno-multichar option)
    need_gcc_Wno_multichar=no
    case "$host" in
        *-*-haiku*)
            need_gcc_Wno_multichar=yes
            ;;
    esac
    AC_MSG_RESULT($need_gcc_Wno_multichar)
    if test x$need_gcc_Wno_multichar = xyes; then
        CFLAGS="$CFLAGS -Wno-multichar"
    fi
fi

AC_MSG_CHECKING(for linker option --no-undefined)
have_no_undefined=no
case "${host_os}" in
dnl Skip this on platforms where it is just simply busted.
    openbsd*) ;;
    darwin*) have_no_undefined="-Wl,-undefined,error"
       LDFLAGS="$LDFLAGS -Wl,-undefined,error" ;;
    *) save_LDFLAGS="$LDFLAGS"
       LDFLAGS="$LDFLAGS -Wl,--no-undefined"
       AC_LINK_IFELSE([AC_LANG_PROGRAM],
          [have_no_undefined=yes],[LDFLAGS="$save_LDFLAGS"])
       ;;
esac
AC_MSG_RESULT($have_no_undefined)

dnl C++ flags are the same as C flags
CXXFLAGS="$CFLAGS"

# Finally create all the generated files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

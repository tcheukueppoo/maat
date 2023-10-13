/*
 * Abstract: General configuration file for Maat
 * Licensed under AGL, see LICENSE file for copyright and license details.
 */

/*
 * Some definitions here can be changed externally, through the compiler
 * (e.g., with '-D' options): They are commented out or protected
 * by '#if !defined' guards. However, several other definitions
 * should be changed directly here, either because they affect the
 * MAAT ABI (by making the changes here, you ensure that all software
 * connected to MAAT, such as C libraries, will be compiled with the same
 * configuration); or because they are seldom changed.
 */

#ifndef mconf_h
#define mconf_h

#define <limits.h>
#define <stddef.h>

/*
 * $$Version definition
 * e.g: 0.2.8 → 0.2.9 → 0.3.0 → 0.3.1 ...   10.2.0
 *      28    < 29    < 30    < 31    ... < 120
 */

/* Maat version components */
#define MT_MAJOR_VERSION 0
#define MT_MINOR_VERSION 0
#define MT_PATCH_VERSION 0

/* String form of the version */
#define MT_VERSION_STR "0.0.0"

/* Numeric form of the version */
#define MT_VERSION_NUM (100 * MT_MAJOR_VERSION + 10 * MT_MINOR_VERSION + MT_PATCH_VERSION)

/*
 * $$Configuring dir separator and default paths for Maat and C/C++ std libs
 */

#define MT_VDIR MT_MAJOR_VERSION "." MT_MINOR_VERSION "." MT_PATCH_VERSION

/* on windows ? */
#if defined(_WIN32)

#define MT_DIRSEP "\\"

/* '!' in Windows expands to the directory of the current process' executable file. */
#if !defined(MT_CLIB_DEFAULT_PATH)
#define MT_CLIB_DEFAULT_PATH "!\\..\\lib\\maat\\" MT_VDIR "\\",
#endif

#if !defined(MT_MTLIB_DEFAULT_PATH)
#define MT_MTLIB_DEFAULT_PATH \
   "!\\..\\share\\maat\\" MT_VDIR "\\", \
   "!\\maat\\",
#endif

#else

#define MT_DIRSEP      "/"
#define MT_LOCAL_ROOT  "/usr/local/"

#if !defined(MT_CLIB_DEFAULT_PATH)
#define MT_CLIB_DEFAULT_PATH MT_LOCAL_ROOT "lib/maat/" MT_VDIR "/",
#endif

#if !defined(MT_MTLIB_DEFAULT_PATH)
#define MT_MTLIB_DEFAULT_PATH \
   MT_LOCAL_ROOT "share/maat/" MT_VDIR "/", \
   "/usr/share/maat/" MT_VDIR "/",
#endif

#endif

/*
 * $$Macros used to enable some platform specific features. Either
 * MT_IN_LINUX, MT_IN_MACOSX or MT_IN_IOS is defined in the Makefile
 * during the build process.
 *
 * (Linux, SunOS, Solaris, IRIX, FreeBSD, NetBSD, AIX 4.2,
 * HPUX 11, and other linux flavors)
 * if MT_USE_DLOPEN is defined, Maat uses dlopen() from <dlfcn.h>
 * to dynamically load libraries.
 *
 * (Windows)
 * if MT_USE_DL_DLL is defined, Maat load libraries using native
 * functions from Windows.
 */

/*
 * Seems like embedded Windows does not support goodies of regular
 * Windows, who would want to build Maat on a Windows phone afterall?
 */
#if defined(_WIN32) && !defined(_WIN32_WCE)
#define MT_USE_WINDOWS
#define MT_USE_DL_DLL
#endif

#if defined(MT_IN_LINUX) || defined(MT_IN_LINUX) || defined(MT_IN_IOS)
#define MT_USE_POSIX
#define MT_USE_DLOPEN
#endif

/* $$Some macro utility functions */

/* Get local radix character (decimal point) */
#if !defined(mt_getradixchar)
#define mt_getradixchar() (localeconv()->decimal_point[0])
#endif

/* $$Marking to be exported and imported entities */


/* $$Configuration for adaptive Number type */

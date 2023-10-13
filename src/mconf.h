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

/* $$Version and authors */

/* Maat version components */
#define MT_MAJOR_VERSION  1
#define MT_MINOR_VERSION  0
#define MT_PATCH_VERSION  0

/* String form of the version */
#define MT_VERSION  MT_MAJOR_VERSION "." MT_MINOR_VERSION
#define MT_PATCH    MT_VERSION "." MT_PATCH_VERSION

/* Some patches after releasing Maat version $MT_VERSION$ */
#define MT_PATCH_NUM  ((10 * MT_MAJOR_VERSION + MT_MINOR_VERSION) * 100 + MT_PATCH_VERSION)

#define MT_COPYRIGHT  "Maat " MT_PATCH  " Copyright (C) 2023 Maat.cm, PanLab.africa"
#define MT_AUTHORS    "Kueppo Tcheukam J. W."

#define MAAT  maat MT_VERSION

/*
 * $$Configuring dir separator and default paths for Maat and external libs
 */

/* on windows ? */
#if defined(_WIN32)

/*
 * '!' in Windows expands to the directory of the current process' executable file.
 * In Makefile, if $(DESTDIR) expands to 'C:\\Program Files' && $(PREFIX) to 'Maat'
 * then '!' would probably expand to 'C:\\Program Files\Maat\'. We make sure multiple
 * versions of Maat can co-exist within expanded '!'.
 */
#if !defined(MT_CLIB_DEFAULT_PATH)
#define MT_CLIB_DEFAULT_PATH  "!\\" MT_VERSION "\\lib\\",
#endif

#if !defined(MT_MTLIB_DEFAULT_PATH)
#define MT_MTLIB_DEFAULT_PATH \
   "!\\" MT_VERSION "\\share\\", \
   "!\\" MT_VERSION "\\maat\\",
#endif

/* For use elsewhere */
#define MT_DIRSEP "\\"

#else

#define MT_LOCAL_ROOT  "/usr/local/"

#if !defined(MT_CLIB_DEFAULT_PATH)
#define MT_CLIB_DEFAULT_PATH MT_LOCAL_ROOT "lib/maat/" MT_VERSION "/",
#endif

#if !defined(MT_MTLIB_DEFAULT_PATH)
#define MT_MTLIB_DEFAULT_PATH \
   MT_LOCAL_ROOT "share/maat/" MT_VERSION "/", \
   "/usr/share/maat/" MT_VERSION "/",
#endif

#define MT_DIRSEP "/"

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

/* $$Some macros which acts as utility functions */

/* Get local radix character (decimal point) */
#if !defined(mt_getradixchar)
#define mt_getradixchar() (localeconv()->decimal_point[0])
#endif

/* $$Marking to be exported and imported entities */


/* $$Configuration for adaptive Number type */

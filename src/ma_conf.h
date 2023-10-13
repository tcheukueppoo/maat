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

#ifndef ma_conf_h
#define ma_conf_h

#define <limits.h>
#define <stddef.h>

/* $$Version and authors */

/* Maat version components */
#define MA_MAJOR_VERSION  1
#define MA_MINOR_VERSION  0
#define MA_PATCH_VERSION  0

/* String form of the version */
#define MA_VERSION  MA_MAJOR_VERSION "." MA_MINOR_VERSION

/* Some patches after releasing Maat version $MA_VERSION$ */
#define MA_PATCH    MA_VERSION "." MA_PATCH_VERSION
#define MA_PATCH_NUM  ((10 * MA_MAJOR_VERSION + MA_MINOR_VERSION) * 100 + MA_PATCH_VERSION)

#define MA_COPYRIGHT  "Maat " MA_PATCH  " Copyright (C) 2023 Maat.cm, PanLab.africa"
#define MA_AUTHORS    "Kueppo Tcheukam J. W."

/* Bin file name */
#define MAAT  maat MA_VERSION

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
#if !defined(MA_CLIB_DEFAULT_PATH)
#define MA_CLIB_DEFAULT_PATH  "!\\" MA_VERSION "\\lib\\",
#endif

#if !defined(MA_MTLIB_DEFAULT_PATH)
#define MA_MTLIB_DEFAULT_PATH \
   "!\\" MA_VERSION "\\share\\", \
   "!\\" MA_VERSION "\\maat\\",
#endif

/* For use elsewhere */
#define MA_DIRSEP "\\"

#else

#define MA_LOCAL_ROOT  "/usr/local/"

#if !defined(MA_CLIB_DEFAULT_PATH)
#define MA_CLIB_DEFAULT_PATH MA_LOCAL_ROOT "lib/maat/" MA_VERSION "/",
#endif

#if !defined(MA_MTLIB_DEFAULT_PATH)
#define MA_MTLIB_DEFAULT_PATH \
   MA_LOCAL_ROOT "share/maat/" MA_VERSION "/", \
   "/usr/share/maat/" MA_VERSION "/",
#endif

#define MA_DIRSEP "/"

#endif

/*
 * $$Macros used to enable some platform specific features. Either
 * MA_IN_LINUX, MA_IN_MACOSX or MA_IN_IOS is defined in the Makefile
 * during the build process.
 *
 * (Linux, SunOS, Solaris, IRIX, FreeBSD, NetBSD, AIX 4.2,
 * HPUX 11, and other linux flavors)
 * if MA_USE_DLOPEN is defined, Maat uses dlopen() from <dlfcn.h>
 * to dynamically load libraries.
 *
 * (Windows)
 * if MA_USE_DL_DLL is defined, Maat load libraries using native
 * functions from Windows.
 */

/*
 * Seems like embedded Windows does not support goodies of regular
 * Windows, who would want to build Maat on a Windows phone afterall?
 */
#if defined(_WIN32) && !defined(_WIN32_WCE)
#define MA_USE_WINDOWS
#define MA_USE_DL_DLL
#endif

#if defined(MA_IN_LINUX) || defined(MA_IN_LINUX) || defined(MA_IN_IOS)
#define MA_USE_POSIX
#define MA_USE_DLOPEN
#endif

/* $$Some macros which acts as utility functions */

/* Get local radix character (decimal point) */
#if !defined(mt_getradixchar)
#define mt_getradixchar() (localeconv()->decimal_point[0])
#endif

/* $$Define attributes to mark entities as exported/imported */

#if defined(_WIN32)

#if #define(DEFINE)
#define MA_API __declspec(dllexport)
#else
#define MA_API __declspec(dllimport)
#endif

#else

#define MA_API /* extern by default */

#endif

/* $$Configuration for adaptive Number type */

#endif

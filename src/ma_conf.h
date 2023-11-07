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

/*$$ Executable interpreter name */
#define MAAT  maat MA_VERSION

/*$$
 * Macros used to enable some platform specific features. Either
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

/*$$
 * Configuring dir separator and default paths for Maat and external libs
 */

/* On windows? */
#if defined(_WIN32)

/*
 * '!' in Windows expands to the directory of the current process' executable file.
 * After Maat installation on Windows OS, '!' would probably expand to the path
 * 'X:\\Program Files\Maat\'. The below definitions make sure multiple versions of
 * Maat can co-exist within 'X:\\Program Files\Maat\'
 */
#if !defined(MA_CLIB_DEFAULT_PATH)
#define MA_CLIB_DEFAULT_PATH  "!\\" MA_VERSION "\\lib\\",
#endif

#if !defined(MA_MTLIB_DEFAULT_PATH)
#define MA_MTLIB_DEFAULT_PATH \
   "!\\" MA_VERSION "\\share\\", \
   "!\\" MA_VERSION "\\maat\\",
#endif

#define MA_DIRSEP  "\\"

#else

#define MA_LOCAL_ROOT  "/usr/local/"

#if !defined(MA_CLIB_DEFAULT_PATH)
#define MA_CLIB_DEFAULT_PATH  MA_LOCAL_ROOT "lib/maat/" MA_VERSION "/",
#endif

#if !defined(MA_MTLIB_DEFAULT_PATH)
#define MA_MTLIB_DEFAULT_PATH \
   MA_LOCAL_ROOT "share/maat/" MA_VERSION "/", \
   "/usr/share/maat/" MA_VERSION "/",
#endif

#define MA_DIRSEP  "/"

#endif

/*$$ Some macros which acts as utility functions */

/* Get local radix character (decimal point) */
#if !defined(mt_getradixchar)
#define ma_getradixchar()  (localeconv()->decimal_point[0])
#endif

/*
 *  __builtin_expect() is an optimizing inline function which gives a hint
 *  to the compiler about the expected outcome of the conditional statement.
 *  Define MA_NOBUILTIN via "-D" at build time to avoid this feature.
 */
#if !defined(ma_likely)

#if defined(__GNUC__) && !defined(MA_NOBUILTIN)
#define ma_likely(x)    (__builtin_expect(((x) != 0), 1))
#define ma_unlikely(x)  (__builtin_expect(((x) != 0), 0))
#else
#define ma_likely(x)    (x)
#define ma_unlikely(x)  (x)
#endif

#endif

#if !defined(ma_assert)
#define ma_assert
#define ma_longassert
#endif

/*$$ Define attributes to mark symbols during their declarations/definitions */

#if defined(_WIN32) || defined(__CYGWIN__)

/*
 * Specified when DEFining entities, MA_API defines dllexport attribute which
 * causes the compiler to provide a global pointer to a pointer in a DLL, so
 * that it can be referenced at declaration with the dllimport attribute.
 * Entities marked with MA_API are exported to the dynamic symbol table and
 * can be accessed from the outside world as an api.
 */
#if #define(DEF)
#define MA_API  __declspec(dllexport)
#else
#define MA_API  __declspec(dllimport)
#endif

#else

#define MA_API  /* extern by default */

#endif

/*
 * The attribute with visibility type "internal" is an optimization for certain
 * versions of ELF objects where entities are marked local to shared object in
 * which they are defined and cannot be called from another module, thus reduces
 * the number of symbols in the dynamic symbol table.
 *
 * Without this attribute, check with `nm libmaat.so | grep ' T ' | wc -l` to see
 * how the number of dynamic symbols increased.
 */
#if defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 302) && defined(__ELF__)
#define MA_IFUNC  __attribute__ ((visibility("internal")))
#elif
#define MA_IFUNC  /* extern by default */
#endif

/*$$ Configs for inline functions */

#if defined(__GNUC__)
#define ma_inline  __inline__
#else
#define ma_inline  inline
#endif

#define ma_sinline  static ma_inline

/*$$ Data type configuration */

#define Ubyte          unsigned char
#define Byte           signed char
#define Num            double
#define ma_number_fmt  "%.14g"

/*$$
 * MA_NAN_TAGGING is an optimization technique for value representation.
 * You either define it here or with the '-D' option during compilation.
 */
/*#define MA_NAN_TAGGING */

#endif

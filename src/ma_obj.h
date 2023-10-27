/*
 * Abstract: Type definitions for Maat objects
 * Lisenced under AGL, see LICENSE file for copyright and license details.
 */

#ifndef ma_obj_h
#define ma_obj_h

#define unsigned char u_byte

typedef enum {
   Bool, Num, Str,
   Range, Array, Map,
   Set, MSet, Bag,
   MBag, Fun, GFun,
   Regex, Ma, Chan,
   Pipe, File, Dir,
   Date, Sys, Socket,
   Proc, Term,
} type;

#define isBool
#define isNum
#define isStr
#define isRange
#define isArray
#define isMap
#define isSet
#define isMSet
#define isBag
#define isMBag
#define isFun
#define isGFun
#define isRegex
#define isMa
#define isChan
#define isPipe
#define isFile
#define isDir
#define isDate
#define isSys
#define isSocket
#define isProc
#define isTerm



/*
 * Header of all objects
 * $type: type of the object
 * $obj_class: class to which the object belongs
 * $next: next obj to keep track of all objects
 * $class: the object's class
 * $marked: object marked for collection?
 */
typedef struct {
   u_byte type;
   struct Class *class;
   struct Header *next;
} Header;

/*
 * $obj: header
 * $hash: hash value of $str
 * $length: length of $str
 * $str: the string itself
 */
typedef struct {
   Header obj;
   unsigned int hash;
   size_t length;
   char str[1];
} Str;

/*
 * $obj: header
 * $from: from (inclusive)
 * $to: to (inclusive)
 */
typedef struct {
   Header obj;
   double from;
   double to;
} Range;

typedef struct {
   Header obj;
   
} Map;

/*
 * $obj: header
 * $sup: class' superclasses (C3 mro)
 * $name: class name
 * $nfields: number of fields
 * $attributes: attributes
 * $methods: a map for methods
 */
typedef struct {
   Header obj;
   struct Class **sup;
   Str *name;
   int nfields;
   Map **methods;
   Value *attributes;
} Class;




#endif


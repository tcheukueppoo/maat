/*
 * Abstract: Type definitions for Maat objects
 * Lisenced under AGL, see LICENSE file for copyright and license details.
 */

#ifndef ma_obj_h
#define ma_obj_h

#include "ma_conf.h"

/*
 * The struct `Class` is also an object which
 * is collectable.
 */
#define Class
#define Bool
#define Num
#define Str
#define Range
#define Array
#define Map
#define Set
#define MSet
#define Bag
#define MBag
#define Fun
#define GFun
#define Ma
#define Work
#define Chan
#define Regex
#define Socket
#define Pipe
#define File
#define Dir
#define Proc
#define Sys
#define Date
#define Term

/*
 * Representing a value
 */
#if defined(MA_NAN_TAGGING)

#else 

/* Different possible types of values */
#define VAL_NIL   0
#define VAL_BOOL  1
#define VAL_NUM   2
#define VAL_CFUNC 3

/*
 * $val: the value itself.
 *
 * $type: determines val's type, this variable already gather all what is
 * necessary to represent booleans and nils values. It is segmented into
 * three parts. Bits distribution:
 *
 * - bits 0-4
 *   to represent the different types of values except as objects. 'val'
 *   is an object when bits 5-6 are set to 1, if 'val' is an object then
 *   bits 0-4 determine its object's type. This gives us a maximum of 32
 *   objects which is sufficient enough.
 *
 * - bits 5-6
 *   to represent variants of certain VAL_* types. You can have at most
 *   2 variants since bits 5-6 all set to one tell 'val' is an object.
 *
 * - bit 7
 *   Whether 'val' is collectable(1) or not(0).
 */
typedef struct {
   Ubyte type;
   union {
      struct Header *obj;
      Num n;
   } val;
} Value;


#define OBJ_BITS  (0b11 << 5)

/* Vary type 't' with the variant bits 'v' */
#define vary(t,v)  ((t) | (v << 5))

#define withvariant(t)     (t.type)
#define withoutvariant(t)  (t.type & (~OBJ_BITS))

#define isObj(v)    ((withvariant(v) & OBJ_BITS) == OBJ_BITS)
#define isNum(v)    (withoutvariant(v) == VAL_NUM)
#define isNil(v)    (withoutvariant(v) == VAL_NIL)
#define isBool(v)   (withoutvariant(v) == VAL_BOOL)
#define isCFunc(v)  (withoutvariant(v) == VAL_CFUNC)

/* Define the 'nil' value with its variants */
#define NIL     vary(VAL_NIL, 0)
#define KEYNIL  vary(VAL_NIL, 1)
#define VALNIL  vary(VAL_NIL, 2)

#define isStrictNil(v)  ()


/* Variants of a boolean value */
#define BOOL     vary(VAL_BOOL, 0) /* This will be our bool object */
#define TRUE     vary(VAL_BOOL, 1)
#define FALSE    vary(VAL_BOOL, 2)

#define isFalse
#define isTrue

#define IS_COLLECTABLE(v)

#endif

/*
 * Header to all objects
 * $type: type of the object
 * $Class: the object's class
 * $next: next obj, to keep track of all objects
 */
typedef struct Header {
   Ubyte type;
   struct Class *class;
   struct Header *next;
} Header;

/*
 * $obj: header
 * $hash: hash value of $str
 * $length: length of $str
 * $str: the string itself
 */
typedef struct Str {
   Header obj;
   unsigned int hash;
   long double length;
   char str[1];
} Str;

/*
 * $obj: header
 * $from: from (inclusive)
 * $to: to (inclusive)
 */
typedef struct Range {
   Header obj;
   double from;
   double to;
} Range;

typedef struct Map {
   Header obj;
   Value *
} Map;

/*
 * $obj: header
 * $sup: class' superclasses (C3 mro)
 * $name: class name
 * $nfields: number of fields
 * $attributes: attributes
 * $methods: a map for methods
 */
typedef struct Class {
   Header obj;
   struct Class **sup;
   Str *name;
   int nfields;
   Map **methods;
   Value *attributes;
} Class;



#endif


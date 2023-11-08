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
#define TYPE_NIL   0
#define TYPE_BOOL  1
#define TYPE_NUM   2
#define TYPE_CFUNC 3

/*
 * $val: the value itself.
 *    $n: Maat's representation of a value(configured at 'ma_conf.h').
 *    $obj: pointer to an object, value is an object.
 *
 *    The below attributes are used to extend Maat code with C/C++:
 *
 *    $p: pointer to C-class attributes('struct's).
 *    $f: pointer to a C functions, either represent a method to a
 *    C-class or just a standalone function.
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
 *   to represent variants of certain TYPE_* types. You can have at most
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
      void *p;
      CFunc f;
   } val;
} Value;


#define OBJ_BITS  (0b11 << 5)

/* Vary type 't' with the variant bits 'v' */
#define vary(t,v)  ((t) | (v << 5))

#define with_variant(v)     (v.type)
#define without_variant(v)  (v.type & (~OBJ_BITS))
#define check_type(v,t)     (without_variant(v) == t)
#define check_vartype(v,t)  (with_variant(v) == t)

#define is_obj(v)    ((with_variant(v) & OBJ_BITS) == OBJ_BITS)
#define is_num(v)    check_type(v,TYPE_NUM)
#define is_nil(v)    check_type(v,TYPE_NIL)
#define is_bool(v)   check_type(v,TYPE_BOOL)
#define is_cfunc(v)  check_type(v,TYPE_CFUNC)

/* Define the 'nil' value/type with its variants */
#define NIL     vary(TYPE_NIL, 0)
#define KEYNIL  vary(TYPE_NIL, 1)
#define VALNIL  vary(TYPE_NIL, 2)

#define is_strict_nil(v)  check_vartype(v,NIL)


/* Define the boolean value/type with its Variants */
#define BOOL   vary(TYPE_BOOL, 0)
#define TRUE   vary(TYPE_BOOL, 1)
#define FALSE  vary(TYPE_BOOL, 2)

#define is_bool(v)   check_type(v,TYPE_BOOL)
#define is_false(v)  check_vartype(to_bool(v),FALSE)
#define is_true(v)   check_vartype(to_bool(v),TRUE)

#define to_bool  (is_bool(v) ? v : coerce_to_bool(v))

#define as_bool(v)   (is_true(v))
#define as_num(v)    (v.val.n)
#define as_obj(v)    (v.val.obj)
#define as_cfunc(v)  (v.val.f)

#define COLLECTABLE_BIT    (0b1 << 7)
#define is_collectable(v)  (with_variant(v) & COLLECTABLE_BIT)

#endif

/*
 * struct Header inherited by all objects
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


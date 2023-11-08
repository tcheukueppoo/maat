/*
 * Abstract: Type definitions for Maat objects
 * Lisenced under AGL, see LICENSE file for copyright and license details.
 */

#ifndef ma_obj_h
#define ma_obj_h

#include "ma_conf.h"

/* Vary type 't' with the variant bits 'v' */
#define vary(t,v)  ((t) | (v << 5))

#define with_variant(v)     (v.type)
#define without_variant(v)  (v.type & (~OBJ_BITS))
#define check_type(v,t)     (without_variant(v) == t)
#define check_vartype(v,t)  (with_variant(v) == t)

#if defined(MA_NAN_TAGGING)

/*
 * TODO
 */
typedef uint64_t Value;


#else 

/*
 * $val: the value itself.
 *   $n: Maat's representation of a value(configured at 'ma_conf.h').
 *   $obj: pointer to an object, value is an object.
 *
 *   The below attributes are used to extend Maat code with C/C++:
 *
 *   $cdata: pointer to C-class attributes('struct's).
 *   $f: pointer to a C functions, either represent a cdata's method
 *   or just a standalone function.
 *
 * $type: determines val's type, this variable already gather all what is
 * necessary to represent booleans and nils values. It is segmented into
 * three parts. Bits distribution:
 *
 * - bits 0-4:
 *   to represent the different types of values except for object values
 *   which is determined by the MSB. if 'val' is an object then bits 0-4
 *   determine its object's type. This gives us a maximum of 32 objects
 *   which is sufficient enough.
 *
 * - bits 5-6:
 *   to represent variants of certain TYPE_* types. You can have at most
 *   3 variants which is sufficient enough.
 *
 * - bit 7: Whether 'val' is(1) an object or not(0)
 */
typedef struct {
   Ubyte type;
   union {
      struct Header *obj;
      Num n;
      CFunc f;
      void *cdata;
   } val;
} Value;


/* All posible standard types of values (TYPE_OBJ marked by the MSB) */
#define TYPE_NIL    0
#define TYPE_BOOL   1
#define TYPE_NUM    2
#define TYPE_CFUNC  3
#define TYPE_CDATA  4

#define OBJ_BIT  (0b1 << 7)

#define is_obj(v)    (with_variant(v) & OBJ_BIT)
#define is_num(v)    check_type(v,TYPE_NUM)
#define is_nil(v)    check_type(v,TYPE_NIL)
#define is_bool(v)   check_type(v,TYPE_BOOL)
#define is_cfunc(v)  check_type(v,TYPE_CFUNC)
#define is_cdata(v)  check_type(v,TYPE_CDATA)

#define is_collectable(v)  is_obj(v)

#define as_bool(v)   (is_true(v))
#define as_num(v)    (v.val.n)
#define as_obj(v)    (v.val.obj)
#define as_cfunc(v)  (v.val.f)
#define as_cdata(v)  (v.val.cdata)

#define type(v)  with_variant(v)

/*
 * real type, real in the sense that we don't need to know whether or
 * not internally it is an object.
 */
#define rtype(v)  (ma_likely(is_obj(v)) ? as_obj(v).type : type(v))

/*
 * Define the 'nil' type, its variants, and singleton values.
 *
 * TYPEV_FREE: this nil variant differentiates a key whose corresponding
 * value is a user-perceived 'nil' from a free position in a hash table.
 *
 * TYPEV_ABSKEY: when indexing a hash table, return this value when key
 * isn't found.
 */
#define TYPEV_FREE    vary(TYPE_NIL, 1)
#define TYPEV_ABSKEY  vary(TYPE_NIL, 2)

#define FREE    (Value){ TYPEV_FREE,   { 0 } }
#define ABSKEY  (Value){ TYPEV_ABSKEY, { 0 } }

#define is_strict_nil(v)  check_vartype(v,NIL)
#define is_free(v)        check_vartype(v,TYPEV_NILKEY)
#define is_abskey(v)      check_vartype(v,TYPEV_ABSKEY)


/* Define boolean type variants with its singleton values */
#define TYPEV_TRUE   vary(TYPE_BOOL, 1)
#define TYPEV_FALSE  vary(TYPE_BOOL, 2)

#define BOOL   (Value){ TYPE_BOOL,   { 0 } }
#define TRUE   (Value){ TYPEV_TRUE,  { 0 } }
#define FALSE  (Value){ TYPEV_FALSE, { 0 } }

#define is_bool(v)   check_type(v,TYPE_BOOL)
#define is_false(v)  check_vartype(to_bool(v),TYPEV_FALSE)
#define is_true(v)   check_vartype(to_bool(v),TYPEV_TRUE)

#define to_bool  (ma_likely(is_bool(v)) ? v : coerce_to_bool(v))

#endif

/*
 *
 */
#define Class  5
#define Bool   6
#define Num    7
#define Str    8
#define Range  9
#define Array  10
#define Map    11
#define Set    12 /* variant: MSet */
#define Bag    12 /* variant: MBag */
#define Fun    11 /* variant: GFun */
#define Ma     12 /* variant: Work */
#define Rbq    13 /* variant: Chan, Schedq */
#define Regex  15
#define Socket 16
#define Pipe   17
#define File   18
#define Dir    19
#define Proc   20
#define Sys    21
#define Date   22
#define Term   23

/*
 * struct Header inherited by all objects
 * $type: type of the object
 * $marked: flag to mark the object during gc
 * $class: the object's class
 * $next: next obj, to keep track of all objects
 */
typedef struct Header {
   Ubyte type;
   Ubyte mark;
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


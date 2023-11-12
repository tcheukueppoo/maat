/*
 * Abstract: Defining objects and values
 * License: AGL, see LICENSE file for copyright and license details.
 */

#ifndef ma_obj_h
#define ma_obj_h

#include "ma_conf.h"

/*
 * Macros defined below are both used to manipulate objects and
 * non NAN-TAGGING represented values, reason why they aren't
 * specific to `#if` body of the MA_NAN_TAGGING check.
 */

/* Vary type 't' with the variant bits 'v', see Value */
#define vary(t, v)  ((t) | (v << 5))

#define with_variant(v)      ((v)->type & 0x7F)
#define without_variant(v)   ((v)->type & 0x1F)
#define check_type(v, t)     (without_variant(v) == t)
#define check_vartype(v, t)  (with_variant(v) == t)

/* Get top level value type of 'v' */
#define vtype(v)  with_variant(v)

/* value 'o' is an object and get its object type */
#define otype(o)  with_variant(as_obj(v))

/* Get profound type of 'v': dig into 'v' if it's an object */
#define type(v)  (ma_likely(is_obj(v)) ? otype(v) : vtype(v))

#if !defined(MA_NAN_TAGGING)

/*
 * $val: the value itself.
 *   $n: representation of a number (see 'ma_conf.h').
 *   $obj: pointer to an object, here, $val is an object.
 *
 *   The below attributes are used to extend Maat code with C/C++:
 *
 *   $cdata: pointer to C-class attributes('struct's).
 *   $f: pointer to a C function, either represent a cdata's
 *       method or just a standalone function.
 *
 * $type: determines $val's type, its bits are segmented into three
 * parts. This variable already gathers all what is necessary to
 * represent booleans and nils values.
 *
 * Bits distribution:
 *
 * - bits 0-4:
 *   to represent the different types of values except for object
 *   values which is determined by the MSB. If $val is an object
 *   then bits 0-4 determine its object's type. This gives us a
 *   maximum of 32 objects which suffices.
 *
 * - bits 5-6:
 *   to represent variants of certain (O_)?TYPE_.* types. You can
 *   have at most 3 variants for each base type which suffices.
 *
 * - bit 7: Mark bit: if 1 then $val is an object, 0 otherwise.
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

#define set_type(v, t)      ((v)->type = t)
#define set_val(v, val, t)  set_type(v, t); ((v)->val = val);
#define set_valo(v, o)      set_val(v, o, (o)->type)

/*
 * For objects, copy by reference v2 into v1 while
 * for non objects (numbers, etc), copy by value.
 */
#define copy(v1, v2)  set_val(v1, v2->val, v2->type)

/*
 * All the possible standard types of a value, look! V_TYPE_OBJ
 * is also one. A value 'v' is an object if the MSB of its $type
 * is 1.
 */
#define V_TYPE_NIL    0
#define V_TYPE_BOOL   1
#define V_TYPE_NUM    2
#define V_TYPE_CFUNC  3
#define V_TYPE_CDATA  4

#define OBJ_BIT  (0b1 << 7)

#define is_obj(v)    (type(v) & OBJ_BIT)
#define is_num(v)    check_type(v, V_TYPE_NUM)
#define is_nil(v)    check_type(v, V_TYPE_NIL)
#define is_bool(v)   check_type(v, V_TYPE_BOOL)
#define is_cfunc(v)  check_type(v, V_TYPE_CFUNC)
#define is_cdata(v)  check_type(v, V_TYPE_CDATA)

/* Check if a value is collectable */
#define is_ctb(v)  is_obj(v)

#define as_bool(v)   (is_true(v))
#define as_num(v)    ((v)->val.n)
#define as_obj(v)    ((v)->val.obj)
#define as_cfunc(v)  ((v)->val.f)
#define as_cdata(v)  ((v)->val.cdata)

/*
 * Define variants of the 'nil' type and its singleton values.
 *
 * V_TYPEV_FREE: this nil variant differentiates a key whose
 * corresponding value is a user-perceived 'nil' from a free
 * position in a hash table.
 *
 * V_TYPEV_ABSKEY: when indexing a hash table and key isn't
 * found, return a dummy value of type V_TYPEV_ABSKEY.
 */
#define V_TYPEV_FREE    vary(V_TYPE_NIL, 1)
#define V_TYPEV_ABSKEY  vary(V_TYPE_NIL, 2)

#define FREE    (Value){ V_TYPEV_FREE,   { 0 } }
#define ABSKEY  (Value){ V_TYPEV_ABSKEY, { 0 } }

#define is_strict_nil(v)  check_vartype(v, V_TYPE_NIL)
#define is_free(v)        check_vartype(v, V_TYPEV_NILKEY)
#define is_abskey(v)      check_vartype(v, V_TYPEV_ABSKEY)

/* Define boolean type variants with its singleton values */
#define V_TYPEV_TRUE   vary(V_TYPE_BOOL,1)
#define V_TYPEV_FALSE  vary(V_TYPE_BOOL,2)

#define BOOL   (Value){ V_TYPE_BOOL,   { 0 } }
#define TRUE   (Value){ V_TYPEV_TRUE,  { 0 } }
#define FALSE  (Value){ V_TYPEV_FALSE, { 0 } }

#define is_false(v)  check_vartype(to_bool(v), V_TYPEV_FALSE)
#define is_true(v)   check_vartype(to_bool(v), V_TYPEV_TRUE)

#define to_bool(v)  (ma_likely(is_bool(v)) ? v : coerce_to_bool(v))

#else

/*
 * TODO
 */
typedef uint64_t Value;

#endif

/*
 * What? Header inherited by all the below objects
 * $type: type of the object
 * $mark: flag to mark the object during collection
 * $class: the object's class
 * $next: next obj,to keep track of all objects
 */
typedef struct Header {
   Ubyte type;
   Ubyte mark;
   struct Class *class;
   struct Header *next;
} Header;

/* Test if the value 'v' is an object of type 't' */
#define o_check_type(v, t)  is_obj(v) && check_type(v, t)

/* Defining of all base objects */
#define O_TYPE_CLASS   5    
#define O_TYPE_STR     6    
#define O_TYPE_RANGE   7    
#define O_TYPE_ARRAY   8   /* variants: O_TYPEV_SET, O_TYPEV_MSET, O_TYPEV_LIST */
#define O_TYPE_MAP     9   /* variants: O_TYPEV_BAG, O_TYPEV_MBAG */
#define O_TYPE_FUN     10  /* variants: O_TYPEV_CLOSURE */
#define O_TYPE_CO     12  /* variants: O_TYPEV_MA, O_TYPEV_WORK */
#define O_TYPE_RBQ     13  /* variants: O_TYPEV_CHAN, O_TYPEV_SCHEDQ */
#define O_TYPE_REGEX   14
#define O_TYPE_SOCKET  15
#define O_TYPE_PIPE    16
#define O_TYPE_FILE    17
#define O_TYPE_DIR     18
#define O_TYPE_PROC    19
#define O_TYPE_SYS     20
#define O_TYPE_DATE    21
#define O_TYPE_TERM    22
#define O_DEADKEY      31  /* */

/* A Check macro on 'v' for each object type */
#define is_class(v)   o_check_type(v, O_TYPE_CLASS)
#define is_str(v)     o_check_type(v, O_TYPE_STR)
#define is_range(v)   o_check_type(v, O_TYPE_RANGE)
#define is_array(v)   o_check_type(v, O_TYPE_ARRAY)
#define is_map(v)     o_check_type(v, O_TYPE_MAP)
#define is_fun(v)     o_check_type(v, O_TYPE_FUN)
#define is_ma(v)      o_check_type(v, O_TYPE_MA)
#define is_rbq(v)     o_check_type(v, O_TYPE_RBQ)
#define is_regex(v)   o_check_type(v, O_TYPE_REGEX)
#define is_socket(v)  o_check_type(v, O_TYPE_SOCKET)
#define is_pipe(v)    o_check_type(v, O_TYPE_PIPE)
#define is_file(v)    o_check_type(v, O_TYPE_FILE)
#define is_dir(v)     o_check_type(v, O_TYPE_DIR)
#define is_proc(v)    o_check_type(v, O_TYPE_PROC)
#define is_date(v)    o_check_type(v, O_TYPE_DATE)
#define is_term(v)    o_check_type(v, O_TYPE_TERM) /* See term.h */

/* Test if the value 'v' is an object variant of variant type 't' */
#define o_check_vartype(v,t)  is_obj(v) && check_vartype(v,t)

/* Here are variants of some base objects */

/* Immutable and mutable bags */
#define O_TYPEV_BAG   vary(O_TYPE_MAP, 1)
#define O_TYPEV_MBAG  vary(O_TYPE_MAP, 2)

#define is_bag(v)     o_check_vartype(v, O_TYPEV_BAG)
#define is_mbag(v)    o_check_vartype(v, O_TYPEV_MBAG)

/* Comma-separated list of values, immutable and mutable sets */
#define O_TYPEV_LIST  vary(O_TYPE_ARRAY, 1)
#define O_TYPEV_SET   vary(O_TYPE_ARRAY, 2)
#define O_TYPEV_MSET  vary(O_TYPE_ARRAY, 3)

#define is_list(v)  o_check_type(v, O_TYPE_LIST)
#define is_set(v)   o_check_vartype(v, O_TYPEV_SET)
#define is_mset(v)  o_check_vartype(v, O_TYPEV_MSET)

/* A closure, it encapsulates upvalues */
#define O_TYPEV_CLOSURE vary(O_TYPE_FUN, 1)

#define is_closure(v)  o_check_vartype(v, O_TYPEV_CLOSURE)

#define O_TYPEV_MA    vary(O_TYPE_CMA, 1)
#define O_TYPEV_GFUN  vary(O_TYPE_CMA, 1)
#define O_TYPEV_MA    vary(O_TYPE_CMA, 1)

/* A threadsafe Channel and Scheduler queue */
#define O_TYPEV_CHAN    vary(O_TYPE_RBQ, 1)
#define O_TYPEV_SCHEDQ  vary(O_TYPE_RBQ, 2)

#define is_chan(v)    o_check_type(v, O_TYPEV_CHAN)
#define is_schedq(v)  o_check_type(v, O_TYPEV_SCHEDQ)

/*
 * What? utf8 string object
 * $str: utf8 encoded string itself
 * $hash: hash value of $str
 * $rlen: real length of $str
 * $len: user-percieved length of $str
 */
typedef struct {
   Header obj;
   Uint hash;
   size_t rlen;
   size_t len;
   Byte str[1];
} Str;

/*
 * What? Range object x..y
 * $x: the start (inclusive)
 * $y: the end (inclusive)
 */
typedef struct {
   Header obj;
   Num x, y;
} Range;

typedef struct {
   Header obj;
} Map;

/*
 * $super: class' superclasses (C3 mro)
 * $name: class name
 * $nfields: number of fields
 * $methods: a Map for methods
 */
typedef struct Class {
   Header obj;
   struct Class *super[1];
   Str *name;
   Ubyte nfields;
   Map *methods[1];
} Class;

/*
 */
typedef struct {
   Header obj;
   //...
} Fun;

/*
 */
typedef struct Upvalue {
   Header obj;
   Value *to;
   union {
      struct Upvalue *next;
      Value val;
   } state;
} Upvalue;

#endif


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

/* Vary type 't' with variant bits 'v', see Value */
#define vary(t, v)  ((t) | (v << 5))

#define with_variant(v)      ((v)->type & 0x7F)
#define without_variant(v)   ((v)->type & 0x1F)
#define check_type(v, t)     (without_variant(v) == t)
#define check_vartype(v, t)  (with_variant(v) == t)

/* Get top level value type of 'v' */
#define vtype(v)  with_variant(v)

/* Value 'o' is an object and get its object type */
#define otype(o)  with_variant(as_obj(v))

/* Get profound type of 'v': dig into 'v' if it's an object */
#define type(v)  (ma_likely(is_obj(v)) ? otype(v) : vtype(v))

#if !defined(MA_NAN_TAGGING)

/*
 * $val: The value itself.
 *   $n: Representation of a number (see 'ma_conf.h').
 *   $obj: Pointer to an object, here, $val is an object.
 *
 *   The below attributes are used to extend Maat code with C/C++:
 *
 *   $cdata: Pointer to some memory representing data of a C class
 *   $f: Pointer to a C function, either represent a cdata's
 *       method or just a standalone function.
 *
 * $type: Determines $val's type, its bits are segmented into three
 * parts. This variable already gathers all what is necessary to
 * represent booleans and nils values.
 *
 * Bits distribution:
 *
 * - Bits 0-4:
 *   To represent the different types of values except for object
 *   values which is detected when the value's MSB is 1. If $val
 *   is an object then bits 0-4 determine its object's type. This
 *   gives us a maximum of 32 objects which suffices.
 *
 * - Bits 5-6:
 *   To represent variants of certain (O_)?TYPE_.* types. You can
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
      void *p;
   } val;
} Value;

#define set_type(v, t)      ((v)->type = t)
#define set_val(v, val, t)  set_type(v, t); ((v)->val = val);
#define set_valo(v, o)      set_val(v, o, (o)->type)

/*
 * For objects, copy by reference 'v2' into 'v1' while for other type
 * of values like numbers, booleans, and function/void pointers, do a
 * copy by value.
 */
#define copy(v1, v2)  set_val(v1, v2->val, v2->type)

/*
 * Defines all the possible standard types of a value, V_TYPE_OBJ is
 * also a value. A value 'v' is an object if the MSB of its $type is 1.
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

/* Check if value 'v' is collectable */
#define is_ctb(v)  is_obj(v)

#define as_bool(v)   (is_true(v))
#define as_num(v)    ((v)->val.n)
#define as_obj(v)    ((v)->val.obj)
#define as_cfunc(v)  ((v)->val.f)
#define as_cdata(v)  ((v)->val.cdata)

/*
 * Define variants of the 'nil' type and its singleton values.
 *
 * V_TYPEV_FREE: This nil variant differentiates a key whose
 * corresponding value is a user-perceived 'nil' from a free
 * position in a map.
 *
 * V_TYPEV_ABSKEY: When indexing a table and key isn't found,
 * return a dummy value of type V_TYPEV_ABSKEY.
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
 * $$Header inherited by all the below objects
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
#define O_TYPE_CLASS   5   /* variant: O_TYPEV_CCLASS */
#define O_TYPE_STR     6    
#define O_TYPE_RANGE   7    
#define O_TYPE_ARRAY   8   /* variants: O_TYPEV_SET, O_TYPEV_MSET, O_TYPEV_LIST */
#define O_TYPE_MAP     9   /* variants: O_TYPEV_BAG, O_TYPEV_MBAG */
#define O_TYPE_FUN     10  /* variant:  O_TYPEV_CLOSURE */
#define O_TYPE_MA      11  /* variants: O_TYPEV_CO, O_TYPEV_WORK */
#define O_TYPE_RBQ     12  /* variants: O_TYPEV_CHAN, O_TYPEV_SCHEDQ */
#define O_TYPE_REGEX   13
#define O_TYPE_SOCKET  14
#define O_TYPE_PIPE    15
#define O_TYPE_FILE    16
#define O_TYPE_DIR     17
#define O_TYPE_PROC    18
#define O_TYPE_SYS     19
#define O_TYPE_DATE    20
#define O_TYPE_PKG     21
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

/* Here are variants of some standard objects */

/*
 * Cclass--a special way of extending Maat code with C/C++, here
 * we store references to C/C++ functions as methods and structs
 * as attributes.
 */
#define O_TYPEV_CCLASS  vary(O_TYPE_CLASS, 1)

#define is_cclass(v)  o_check_vartype(v, O_TYPEV_CCLASS)

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

/* Two other concurrency models: Coroutine, Work */
#define O_TYPEV_CO    vary(O_TYPE_CMA, 1)
#define O_TYPEV_WORK  vary(O_TYPE_CMA, 2)

/* A threadsafe Channel and Scheduler queue */
#define O_TYPEV_CHAN    vary(O_TYPE_RBQ, 1)
#define O_TYPEV_SCHEDQ  vary(O_TYPE_RBQ, 2)

#define is_chan(v)    o_check_type(v, O_TYPEV_CHAN)
#define is_schedq(v)  o_check_type(v, O_TYPEV_SCHEDQ)

/*
 * $$string object
 * $str: utf-8 encoded string itself
 * $hash: hash value of $str
 * $rlen: real length of $str
 * $len: user-percieved length of $str
 */

typedef struct {
   Header obj;
   unsigned int hash;
   size_t rlen;
   size_t len;
   Byte str[1];
} Str;

/*
 * $$Range object [x..y] (inclusive)
 * $x: the start
 * $y: and the end
 */

typedef struct {
   Header obj;
   Num x;
   Num y;
} Range;

/*
 */

typedef struct {
   Header obj;
} Map;

/*
 * $$Representation of a class, every object has a class, even the class
 * itself. Class of objects which aren't first class values are useless.
 *
 * $name: The class' name.
 * $methods: A map for the class' methods.
 *
 * A C class is a class whose implemented is done in foreign languages
 * like C or C++, it is similar to full userdata in Lua lang.
 *
 * $as: A union which contains data specific to each type of class.
 * The inherited field $type tells us which struct to use.
 *
 * $cdata: Pointer to the raw memory.
 * $size: Size of the memory pointed by cdata.
 *
 * $c3: List of classes obtained after c3 method resolution was applied
 * $fields: Hash map for the class' fields, each field has the default
 * value 'nil' unless explicitly stated.
 */

typedef struct Class {
   Header obj;
   Str *name;
   union {
      struct {
         Map *fields;
         struct Class *roles, *supers, *c3;
      } ma;
      struct {
         Value cdata;
         size_t size;
      } c;
   } as;
   Map *methods;
} Class;


/*
 * $$Instance of a class.
 *
 * $fields: A map, the instance's fields, during object instanciation we
 * copy the instance's class $fields map which contains all the defaults.
 * Is this a good idea?
 */
typedef struct {
   Header obj;
   Map *fields;
} Instance;

/*
 * $$Represents the spec of a namespace: the namespace value and
 * its variables which have to be fully qualified when accessed
 * from outside.
 *
 * $ours: A map for the namespace's "our" variables.
 * $val: Either a class, role or package.
 */
typedef struct Namespace_Spec {
   Header obj;
   Map *ours;
   Value val;
} Namespace_Spec;

/*
 * $ns_spec: A buffer of namespaces, since namespaces are unique
 * and aren't restrained by the concept of scopes wether or not
 * a namespace is defined in another, w
 */
typedef struct NameSpace {
   Header obj;
   NameSpace_Spec ns_spec;
} NameSpace;

typedef struct Ma {
} Ma;

typedef struct {
   Ma ma;
} Work;

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


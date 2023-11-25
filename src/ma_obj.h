/*
 * Abstract: Defining objects and values
 * License: AGL, see LICENSE file for copyright and license details.
 */

#ifndef ma_obj_h
#define ma_obj_h

#include "ma_conf.h"

/*
 * Macros defined below are both used to manipulate objects and non
 * NAN-TAGGING represented values, reason why they aren't specific
 * to `#if` body of the "MA_NAN_TAGGING" conditional test.
 */

/* Vary type 't' with variant bits 'v', see Value */
#define vary(t, v)  ((t) | (v << 5))

#define with_variant(v)      ((v)->type & 0x7F)
#define without_variant(v)   ((v)->type & 0x1F)
#define check_type(v, t)     (without_variant(v) == t)
#define check_vartype(v, t)  (with_variant(v) == t)

#if !defined(MA_NAN_TAGGING)

/* Collectable object has the same type as the orignal value */
#define type(v)  without_variant(v)

/*
 * #val: The value itself.
 *   #n: Representation of a number (see 'ma_conf.h').
 *   #obj: Pointer to an object, here, $val is an object.
 *
 *   The below attributes are used to extend Maat code with C/C++:
 *
 *   #cdata: Pointer to some memory representing data of a C class
 *   #f: Pointer to a C function, either represent a cdata's
 *       method or just a standalone function.
 *
 * #type: Determines $val's type, its bits are segmented into three
 * parts. This variable already gathers all what is necessary to
 * represent booleans and nils values.
 *
 * Bits distribution:
 *
 * - Bits 0-4:
 *   To represent the different types of values except for object
 *   values which is detected when the value's MSB is 1. If #val
 *   is an object then bits 0-4 determine its object's type. This
 *   gives us a maximum of 32 objects which suffices.
 *
 * - Bits 5-6:
 *   To represent variants of certain (O_)?TYPE_.* types. You can
 *   have at most 3 variants for each base type which suffices.
 *
 * - Bit 7: Mark bit: if 1 then #val is an object, 0 otherwise.
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

#define set_type(v, t)       ((v)->type = t)
#define set_val(v, _val, t)  set_type(v, t); ((v)->val = _val);
#define set_valo(v, o)       set_val(v, o, (o)->type)

/*
 * For objects, copy by reference 'v2' into 'v1' while for other type
 * of values like numbers, booleans, and function/void pointers, do a
 * copy by value.
 */
#define copy(v1, v2)  set_val(v1, v2->val, v2->type)

/*
 * Defines all the possible standard types of a value, V_TYPE_OBJ is
 * also a value. A value 'v' is an object if the MSB of its #type is 1.
 */
#define V_TYPE_NIL    0
#define V_TYPE_BOOL   1
#define V_TYPE_NUM    2
#define V_TYPE_CFUNC  3
#define V_TYPE_CDATA  4

#define OBJ_BIT  (0b1 << 7)

#define is_obj(v)    ((v) & OBJ_BIT)
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
 * ##Header inherited by all the below objects
 * #type: type of the object
 * #mark: flag to mark the object during collection
 * #class: the object's class
 * #next: next obj,to keep track of all objects
 */
typedef struct Header {
   Ubyte type;
   Ubyte mark;
   struct Class *class;
   struct Header *next;
} Header;

/* Test if the value 'v' is an object of type 't' */
#define o_check_type(v, t)  is_obj(v) && check_type(v, t)

/* Defining all base objects, each object here may or not have variants */

#define O_TYPE_CLASS     5   /* variant: O_TYPEV_CCLASS */
#define O_TYPE_INSTANCE  6
#define O_TYPE_STR       7   /* variants: O_TYPEV_SHTSTR, O_TYPEV_LNGSTR */
#define O_TYPE_RANGE     8    
#define O_TYPE_ARRAY     9   /* variants: O_TYPEV_SET, O_TYPEV_MSET, O_TYPEV_LIST */
#define O_TYPE_MAP       10  /* variants: O_TYPEV_BAG, O_TYPEV_MBAG */
#define O_TYPE_FUN       11  /* variant:  O_TYPEV_CLOSURE */
#define O_TYPE_UPVAL     12
#define O_TYPE_MA        13  /* variants: O_TYPEV_CO, O_TYPEV_WORK */
#define O_TYPE_RBQ       14  /* variants: O_TYPEV_CHAN, O_TYPEV_SCHEDQ */
#define O_TYPE_REGEX     15
#define O_TYPE_SOCKET    16
#define O_TYPE_PIPE      17
#define O_TYPE_FILE      18
#define O_TYPE_DIR       19
#define O_TYPE_PROC      20
#define O_TYPE_SYS       21
#define O_TYPE_DATE      22
#define O_TYPE_NS        23
#define O_TYPE_TERM      24

/*
 * A special kind of type to mark objects which act as keys to maps as
 * "dead". Dead objects will be resurrected by the program if they are
 * needed before the next GC cycle.
 */
#define O_TYPE_DEADKEY      31

/* A Check macro on 'v' for each object type */
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


/* Two other concurrency models: Coroutine, Work */
#define O_TYPEV_CO    vary(O_TYPE_CMA, 1)
#define O_TYPEV_WORK  vary(O_TYPE_CMA, 2)

/* A threadsafe Channel and Scheduler queue */
#define O_TYPEV_CHAN    vary(O_TYPE_RBQ, 1)
#define O_TYPEV_SCHEDQ  vary(O_TYPE_RBQ, 2)

#define is_chan(v)    o_check_type(v, O_TYPEV_CHAN)
#define is_schedq(v)  o_check_type(v, O_TYPEV_SCHEDQ)

/*
 * ##String object, represents both short and long strings
 *
 * #str: utf-8 encoded string itself
 * #hash: hash value of $str
 * #rlen: real length of $str
 * #len: user-percieved length of $str
 */

#define O_TYPEV_SHTSTR  vary(O_TYPE_STR, 1)
#define O_TYPEV_LNGSTR  vary(O_TYPE_STR, 2)

#define is_str(v)     o_check_type(v, O_TYPE_STR)
#define is_shtstr(v)  o_check_type(v, O_TYPEV_SHTSTR)
#define is_lngstr(v)  o_check_type(v, O_TYPEV_LNGSTR)

#define as_str(v)     ((Str *)as_obj(v))
#define as_shtstr(v)  as_str(v)
#define as_lngstr(v)  as_str(v)

typedef struct Str {
   Header obj;
   unsigned int hash;
   size_t rlen;
   size_t len;
   Byte *str;
} Str;

/*
 * ##Range object [x..y] (inclusive).
 *
 * #x: The start and.
 * #y: The end.
 */
#define is_range(v)  o_check_type(v, O_TYPE_RANGE)
#define as_range(v)  ((Range *)as_obj(v))

typedef struct {
   Header obj;
   Num x;
   Num y;
} Range;

/*
 * ##Representation of a Map(a.k.a Hash table), it has
 *
 * #lsize:
 * #array:
 * #asize:
 * #node:
 *
 * ##Representation of a node
 *
 * #val: node's value
 * #k:
 *   #key: node's key
 *   #next: next node in case of collision
 */

/* Variants of a Map: immutable and mutable bags */
#define O_TYPEV_BAG   vary(O_TYPE_MAP, 1)
#define O_TYPEV_MBAG  vary(O_TYPE_MAP, 2)

#define is_map(v)   o_check_type(v, O_TYPE_MAP)
#define iss_map(v)  o_check_vartype(v, O_TYPE_MAP)
#define is_bag(v)   o_check_vartype(v, O_TYPEV_BAG)
#define is_mbag(v)  o_check_vartype(v, O_TYPEV_MBAG)

#define as_map(v)   ((Map *)as_obj(v))
#define as_bag(v)   as_map(v)
#define as_mbag(v)  as_map(v)

typedef struct Node {
   struct {
      Value key;
      Int next;
   } k;
   Value val;
} Node;

typedef struct Map {
   Header obj;
   Value *array;
   Uint asize;
   Ubyte lsize;
   Node *node;
} Map;

/* ##Representation of an Array object
 *
 * #array: The array itself.
 * #size: Its size.
 * #capacity: The array's capacity.
 */

/* Array Variants: comma-separated list of values, immutable and mutable sets */
#define O_TYPEV_LIST  vary(O_TYPE_ARRAY, 1)
#define O_TYPEV_SET   vary(O_TYPE_ARRAY, 2)
#define O_TYPEV_MSET  vary(O_TYPE_ARRAY, 3)

#define is_array(v)   o_check_type(v, O_TYPE_ARRAY)
#define iss_array(v)  o_check_vartype(v, O_TYPE_ARRAY)
#define is_list(v)    o_check_vartype(v, O_TYPEV_LIST)
#define is_set(v)     o_check_vartype(v, O_TYPEV_SET)
#define is_mset(v)    o_check_vartype(v, O_TYPEV_MSET)

#define as_array(v)  ((Array *)as_obj(v))
#define as_list(v)   as_array(v)
#define as_set(v)    as_array(v)
#define as_mset(v)   as_array(v)

typedef struct Array {
   Header obj;
   size_t size;
   size_t capacity;
   Value *array;
} Array;

/*
 * ##Representation of a class, every collectable object has a class, even
 * the class itself. The Class of objects which aren't first class values
 * are probably useless, e.g Upval.
 *
 * #name: The class' name.
 * #methods: Points to map for the class' methods. A value to a key here is a
 * pointer to a Closure but can later on change to an Array so as to cache
 * super methods, the cache can never be invalidate since we do not have the
 * luxery of changing inheritance relationships at runtime.
 *
 * A C class is a class whose implemention is done in foreign languages
 * like C or C++, it is similar to full userdata in Lua lang.
 *
 * #in: A union which defines data depending on the object variant, here the
 * object Class defines 2 variants which are Roles and Cclasses.
 *
 *  For a Cclass we have in #c:
 *
 *   #cdata: Pointer to the raw memory.
 *   #size: Size of the memory pointed by #cdata.
 *
 *  For a Ma class or Role we have in #ma:
 *
 *   #c3: List of classes obtained after c3 linearization was applied.
 *   #fields: Pointer to an array of values, indexes correspond to fields
 *   with values holding their defaults.
 *   #roles: Keeps the list of roles the class ":does".
 *   #supers: Keeps the list of directly inherited superclasses.
 *
 * #supers exist mainly for class/object introspection since #c3 handles
 * super calls.
 */

/*
 * Cclass-A special way of extending Maat code with C/C++, here we store
 * references to C/C++ functions as methods and structs as attributes.
 */
#define O_TYPEV_ROLE    vary(O_TYPE_CLASS, 1)
#define O_TYPEV_CCLASS  vary(O_TYPE_CLASS, 2)

#define is_class(v)   o_check_type(v, O_TYPE_CLASS)
#define iss_class(v)  o_check_vartype(v, O_TYPE_CLASS)
#define is_role(v)    o_check_vartype(v, O_TYPEV_ROLE)
#define is_cclass(v)  o_check_vartype(v, O_TYPEV_CCLASS)

#define as_class(v)   ((Class *)as_obj(v))
#define as_role(v)    as_class(v)
#define as_cclass(v)  as_class(v)

typedef struct Class {
   Header obj;
   Str *name;
   Map *methods;
   union {
      struct {
         Value *fields;
         struct Class *roles;
         struct Class *supers;
         struct Class *c3;
      } ma;
      struct {
         Value cdata;
         size_t size;
      } c;
   } in;
} Class;

/*
 * ##Instance of a class.
 *
 * #fields: A pointer to a to be allocated array of type "Value", each
 * field has a unique id which corresponds to an index in #fields. This
 * also holds inherited fields.
 * #call_level: Keeps track of the level of super calls for each method called
 * on the instance, it is 'NULL' if none of the methods of the instance's class
 * do super calls i.e self.SUPER::<method_name>(...). Where SUPER is a
 * a pseudo-class that resolves to the next class in the instance's class c3
 * list.
 */
#define is_instance(v)  o_check_type(v, O_TYPE_INSTANCE)
#define as_instance(v)  ((Instance *)as_obj(v))

typedef struct Instance {
   Header obj;
   Value *fields;
   Map *call_level;
} Instance;

/*
 * ##Represents a namespace: 
 * variables which have to be fully qualified when accessed from outside
 * itself.
 *
 * #name: Namespace's name.
 * #ns_val: The namespace value, either a class, role or package.
 *
 * #ours: A map for the namespace's "our" variables. For the main::
 * namespace, #ours takes care of the following type I & II special
 * variables:
 *      ENV, ARGC, ARGV, INC, PATH, SIG, DATA, $v, $o, $,, $/, $\
 *      $|, $", $$, $(, $), $<, $>, $f, and $0.
 *
 * The "main::" namespace define with package{} is "use"d by all the other
 * namespaces and as a result these special variables can be accessed
 * without using their FQNs.
 *
 * #exports: Values for keys of to be exported "our" variables.
 */
#define is_ns(v)  o_check_type(v, O_TYPE_NS)
#define as_ns(v)  ((Namespace *)as_obj(v))

typedef struct Namespace {
   Header obj;
   Str *name; 
   Map *ours;
   Value *exports;
   Value ns_val;
} Namespace;

/*
 * ##Struct of a Maat function.
 *
 * #arity: The number of arguments the function takes.
 * #up_count: Number of upvals the function has.
 * #code: Its bytecode.
 * #constants: The function's contant values.
 * #ns: The function's namespace, it's mainly used for instrospection.
 */
#define is_fun(v)  o_check_type(v, O_TYPE_FUN)
#define as_fun(v)  ((Fun *)as_obj(v))

typedef struct Fun {
   Header obj;
   int up_count;
   int arity;
   CodeBuf code;
   ValueBuf constants;
   Namespace *ns;
} Fun;

/*
 * ##A function that keeps track of its upvalues is called a closure, upvalues
 * initially are lexically scoped variables living in vm's registers, they
 * were supposed to stop existing when the program goes out of their scopes
 * of definition but since some functions need them, these lexicals are kept
 * as upvalues.
 *
 * #p: Pointer to upvalue, when an upvalue is opened, it points to a value
 * in a vm register but when closed, it points to #state.
 *
 * #state: A pointer to the #next open upvalue when this one is still open
 * otherwise it'll contain the register vm value #p pointed to.
 */

/* #define close_upval(u, v)  u->state.val = v; copy_val(u->state.val, v) */
/* #define next_upval(u)      (u->state.next) */

typedef struct Upval {
   Header obj;
   Value *p;
   union {
      struct Upval *next;
      Value val;
   } state;
} Upval;

/* ##A closure is a variant of a function which keep tracks of its upvalues.
 * That is why it is called a closure.
 *
 * #fun: The closure's function.
 * #upvals: The List of upvalues the function has.
 */
#define O_TYPEV_CLOSURE  vary(O_TYPE_FUN, 1)

#define is_closure(v)  o_check_vartype(v, O_TYPEV_CLOSURE)
#define as_closure(v)  ((Closure *)as_obj(v))

typedef struct Closure {
   Header obj;
   Fun *fun;
   Upvalue *upvals;
} Closure;

/*
 * #ip:
 *
 * #temps: For temporarization(Maat's "temp" keyword) of package variables. It
 * is a map with keys corresponding to scope level counts since multiple scopes
 * can temporarize a same package variable, we are using a map because the
 * Array type has an initial capacity which is wastful in this scenario while the
 * Map uses its array part when necessary then optimizes into a real map when
 * scope indexes are sparsed. $temps takes care of the following special
 * variables:
 *   $<digit>(e.g $1, $2, $3, etc), $., $`, $&, and $'
 *
 * It is internally implemented c codes that have to appropiately set these
 * variables in $temps for usage e.g the Regex's method ".gmatch".
 */

typedef struct CallFrame {
   uint8_t *ip;
   Map *temps;
} CallFrame;

#endif

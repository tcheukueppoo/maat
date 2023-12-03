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
      struct Object *obj;
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
 * ##Object-struct inherited by all the below objects.
 *
 * #type: Type of the object.
 * #mark: Flag to mark the object during collection.
 * #class: The object's class.
 * #next: Next obj, to keep track of all objects.
 */
typedef struct Object {
   Ubyte type;
   Ubyte mark;
   struct Class *class;
   struct Object *next;
} Object;

/* Test if the value 'v' is an object of type 't' */
#define o_check_type(v, t)  is_obj(v) && check_type(v, t)

/* Here are base objects, some do have variants */

/* O_TYPEV_CCLASS, O_TYPEV_ROLE */
#define O_TYPE_CLASS     5

#define O_TYPE_INSTANCE  6

/* O_TYPEV_SHTSTR, O_TYPEV_LNGSTR */
#define O_TYPE_STR       7

#define O_TYPE_RANGE     8    

/* O_TYPEV_SET, O_TYPEV_MSET, O_TYPEV_LIST */
#define O_TYPE_ARRAY     9

/* O_TYPEV_BAG, O_TYPEV_MBAG */
#define O_TYPE_MAP       10

/* O_TYPEV_CLOSURE */
#define O_TYPE_FUN       11

#define O_TYPE_UPVAL     12

/* O_TYPEV_CO, O_TYPEV_GFUN, O_TYPEV_MA, O_TYPEV_WORK */
#define O_TYPE_MSTATE    13

/* O_TYPEV_CHAN, O_TYPEV_SCHEDQ */
#define O_TYPE_RBQ       14

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
 * ##String object.
 * #str: The UTF-8 encoded string itself.
 * #hash: Hash value of $str.
 * #rlen: Real length of $str.
 * #len: User-percieved length of $str.
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
   Object obj;
   unsigned int hash;
   size_t rlen;
   size_t len;
   Byte *str;
} Str;

/*
 * ##Range object [x..y] (inclusive).
 * #x: The start and.
 * #y: The end.
 */
#define is_range(v)  o_check_type(v, O_TYPE_RANGE)
#define as_range(v)  ((Range *)as_obj(v))

typedef struct {
   Object obj;
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
   Object obj;
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

/* Variants: comma-separated list of values, immutable and mutable sets */
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
   Object obj;
   size_t size;
   size_t capacity;
   Value *array;
} Array;

/*
 * ##Representation of a class, every collectable object has a
 * class, even the class itself. The Class of objects which
 * aren't first class values are probably useless, e.g Upval.
 *
 * #name: The class' name.
 * #methods: Pointer to a map for the class' methods. A value to
 * a key here is a pointer to a Closure but can later on change
 * to an Array of closures so as to cache super methods, the
 * cache can never be invalidate since c3 linearization is done
 * at compile time.
 *
 * A C class is a class whose implemention is done in foreign
 * languages like C or C++, it is similar to full userdata in Lua.
 *
 * variant, here the object Class defines 2 variants which are
 * Roles and Cclasses.
 *
 * For a Maat class and a Role we have the following fields:
 *  #c3: List of classes got after c3 linearization was applied.
 *  #fields: Pointer to an array of values, indexes correspond
 *  to fields with values holding their defaults.
 *  #roles: Keeps the list of roles that this lass ":does".
 *  #sups: Keeps the list of directly inherited superclasses.
 *  #sups exist mainly for introspection since #c3 handles super
 *  calls.
 *
 * And for a Cclass we have:
 *  #cdata: A pointer to the raw memory.
 *  #size: The size of the memory pointed by #cdata.
 */
#define O_TYPEV_ROLE  vary(O_TYPE_CLASS, 1)

#define iss_class(v)  o_check_vartype(v, O_TYPE_CLASS)
#define is_class(v)   o_check_type(v, O_TYPE_CLASS)
#define is_role(v)    o_check_vartype(v, O_TYPEV_ROLE)

#define as_class(v)  ((Class *)as_obj(v))
#define as_role(v)   as_class(v)

typedef struct Class {
   Object obj;
   Str *name;
   Value *fields;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Map *methods;
} Class;

#define O_TYPEV_CCLASS  vary(O_TYPE_CLASS, 2)

#define is_cclass(v)  o_check_vartype(v, O_TYPEV_CCLASS)
#define as_cclass(v)  ((Cclass *)as_obj(v))

typedef struct Cclass {
   Object obj;
   Str *name;
   Value cdata;
   size_t size;
   Map *methods;
} Cclass;

/*
 * ##Instance of a class.
 *
 * #fields: A pointer to a to-be-allocated array of values, each
 * field has a unique id which corresponds to an index in #fields.
 * This also holds inherited fields too.
 *
 * #s_level: Keeps track of the level of super calls for each
 * method called on the instance, in otherwords it is a list 
 * of indexes to super methods cached at the level of this
 * instance's class, it is set 'NULL' if none of the methods
 * of this instance's class does a super call.
 *
 * "self.SUPER::<method_name>(...)" Where SUPER is a pseudo-class
 * that resolves to the next class in the instance's class c3
 * list.
 */
#define is_instance(v)  o_check_type(v, O_TYPE_INSTANCE)
#define as_instance(v)  ((Instance *)as_obj(v))

typedef struct Instance {
   Object obj;
   Value *fields;
   Map *s_level;
} Instance;

/*
 * ##Represents a namespace e.g FOO::BAR. A namespace can either
 * be represented as a package, role or (c)class.
 * "FOO::BAR::x()" is a call to the function "x" in "FOO::BAR"
 * if ever there is.
 *
 * #name: Namespace's name.
 * #ours_const: Constant globals of the #ns_val (thread-safe)
 * #ours: Stores writeable globals (not thread-safe, use mutex?)
 *
 * #ours of the package "main::" takes care of the following
 * type I & II special variables:
 *
 *   ENV, ARGC, ARGV, INC, PATH, SIG, DATA, $v, $o, $,, $/, $\
 *   $|, $", $$, $(, $), $<, $>, $f, and $0.
 *
 * Access to one these variables from another package most not
 * necessarily be done using its fully qualified form unless a
 * variables of the same name declared in a scope shields it.
 *
 * #exports: Names of to-be-exported globals when used by another
 * package.
 *
 * NB:
 * - The argument to the "use" statement never translates to a
 *   namespace.
 * - Not all classes or roles are namespaces, some can be
 *   lexically scoped and thus can never be fully qualified.
 * - For consistency, you cannot nest namespaces as they don't
 *   really have an identity.
 * 
 * #ns_val: The namespace value, a package/class/role.
 */
#define is_ns(v)  o_check_type(v, O_TYPE_NS)
#define as_ns(v)  ((Namespace *)as_obj(v))

typedef struct Namespace {
   Object obj;
   Str *name; 
   Map *ours;
   Map *const_ours;
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
 * #ns: The function's namespace.
 */
#define is_fun(v)  o_check_type(v, O_TYPE_FUN)
#define as_fun(v)  ((Fun *)as_obj(v))

typedef struct Fun {
   Object obj;
   int up_count;
   int arity;
   CodeBuf code;
   ValueBuf constants;
   Namespace *ns;
} Fun;

/*
 * ##A function that keeps track of its upvalues is called a
 * closure, upvalues initially are lexically scoped variables
 * living in vm's registers, they were supposed to stop existing
 * when the program goes out of their scopes of definition but
 * since some functions need them, these lexicals are kept as
 * upvalues.
 *
 * #p: Pointer to upvalue, when an upvalue is opened, it points
 * to a value in a vm register but when closed, it points to
 * #state.
 *
 * #state: A pointer to the #next open upvalue when this one is
 * still open otherwise it'll contain the register vm value #p
 * pointed to.
 */
typedef struct Upval {
   Object obj;
   Value *p;
   union {
      struct Upval *next;
      Value val;
   } state;
} Upval;

/* ##A closure is a variant of a function which keep tracks of
 * its upvalues.
 *
 * #fun: The closure's function.
 * #upvals: The List of upvalues the function has.
 */
#define O_TYPEV_CLOSURE  vary(O_TYPE_FUN, 1)

#define is_closure(v)  o_check_vartype(v, O_TYPEV_CLOSURE)
#define as_closure(v)  ((Closure *)as_obj(v))

typedef struct Closure {
   Object obj;
   Fun *fun;
   Upvalue *upvals;
} Closure;

/* ##Union of all collectable objects used for conversion. */
union Ounion {
   Object obj;
   struct Str str;
   struct Array ar;
   struct Map map;
   struct Fun fun;
   struct Closure clo;
   struct Upval uv;
   struct Class cls;
   struct Cclass ccls;
   struct Instance ins;
   struct Co co;
   struct Ma ma;
   struct Work wk;
   struct Rbq rbq;
   struct Namespace ns;
   struct MVM mvm;
} Ounion;

#define ounion_of(o)  ((Ounion *)o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */
#define o2str(o)   (ma_assert(check_type(v, O_TYPE_STR)), &(ounion_of(o)->str))
#define o2ar(o)    (ma_assert(check_vartype(v, O_TYPE_ARRAY)), &(ounion_of(o)->ar))
#define o2map(o)   (ma_assert(check_vartype(v, O_TYPE_MAP)), &(ounion_of(o)->map))
#define o2fun(o)   (ma_assert(check_vartype(v, O_TYPE_FUN)), &(ounion_of(o)->fun))
#define o2clo(o)   (ma_assert(check_vartype(v, O_TYPEV_CLOSURE)), &(ounion_of(o)->clo))
#define o2uv(o)    (ma_assert(check_vartype(v, O_TYPE_UV)), &(ounion_of(o)->uv))
#define o2cls(o)   (ma_assert(check_type(v, O_TYPE_CLASS)), &(ounion_of(o)->cls))
#define o2ccls(o)  (ma_assert(check_vartype(v, O_TYPEV_CLASS)), &(ounion_of(o)->ccls))
#define o2ins(o)   (ma_assert(check_type(v, O_TYPE_INSTANCE)), &(ounion_of(o)->ins))
#define o2co(o)    (ma_assert(check_vartype(v, O_TYPE_CO)), &(ounion_of(o)->co))
#define o2ma(o)    (ma_assert(check_vartype(v, O_TYPE_MA)), &(ounion_of(o)->ma))
#define o2wk(o)    (ma_assert(check_vartype(v, O_TYPE_WORK)), &(ounion_of(o)->wk))
#define o2rbq(o)   (ma_assert(check_vartype(v, O_TYPE_RBQ)), &(ounion_of(o)->rbq))
#define o2ns(o)    (ma_assert(check_vartype(v, O_TYPE_NS)), &(ounion_of(o)->ns))
#define o2mvm(o)   (ma_assert(check_type(v, O_TYPE_MVM)), &(ounion_of(o)->mvm))

/* The other way around */
#define xo2o(v)  (ma_assert(is_obj(v)), &(ounion_of(o)->obj))

#endif

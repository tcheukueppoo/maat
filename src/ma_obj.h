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
 *   To represent variants of certain (O_)?.* types. You can
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
#define set_obj(v, o)        set_val(v, o, (o)->type)

/*
 * For objects, copy by reference 'v2' into 'v1' while for non-objects
 * like numbers, booleans, and pointers, do a copy by value.
 */
#define copy(v1, v2)  set_val(v1, v2->val, v2->type)

/*
 * Defines all the possible standard types of a value, V_OBJ is
 * also one of them. A value 'v' is an object if the MSB of its
 * #type is 1.
 */
#define V_NIL    0
#define V_BOOL   1
#define V_NUM    2
#define V_CFUNC  3
#define V_CDATA  4

#define OBJ_BIT  (0b1 << 7)

#define is_obj(v)    ((v) & OBJ_BIT)
#define is_num(v)    check_type(v, V_NUM)
#define is_nil(v)    check_type(v, V_NIL)
#define is_bool(v)   check_type(v, V_BOOL)
#define is_cfunc(v)  check_type(v, V_CFUNC)
#define is_cdata(v)  check_type(v, V_CDATA)

/* Checks if value 'v' is collectable */
#define is_ctb(v)  is_obj(v)

#define as_bool(v)   (ma_assert(is_bool(v)), (is_true(v)))
#define as_num(v)    (ma_assert(is_num(v)), ((v)->val.n))
#define as_obj(v)    (ma_assert(is_obj(v)), ((v)->val.obj))
#define as_cfunc(v)  (ma_assert(is_cfunc(v)), ((v)->val.f))
#define as_cdata(v)  (ma_assert(is_cdata(v)), ((v)->val.cdata))

/*
 * Define variants of the 'nil' type and its singleton values.
 *
 * V_VFREE: This nil variant differentiates a key whose
 * corresponding value is a user-perceived 'nil' from a free
 * position in a map.
 *
 * V_VABSKEY: When indexing a table and key isn't found,
 * return a dummy value of type V_VABSKEY.
 */
#define V_VFREE    vary(V_NIL, 1)
#define V_VABSKEY  vary(V_NIL, 2)

#define FREE    (Value){ V_VFREE,   { 0 } }
#define ABSKEY  (Value){ V_VABSKEY, { 0 } }

#define is_strict_nil(v)  check_vartype(v, V_NIL)
#define is_free(v)        check_vartype(v, V_VNILKEY)
#define is_abskey(v)      check_vartype(v, V_VABSKEY)

/* Define boolean type variants with its singleton values */
#define V_VTRUE   vary(V_BOOL,1)
#define V_VFALSE  vary(V_BOOL,2)

#define BOOL   (Value){ V_BOOL,   { 0 } }
#define TRUE   (Value){ V_VTRUE,  { 0 } }
#define FALSE  (Value){ V_VFALSE, { 0 } }

#define is_false(v)  check_vartype(to_bool(v), V_VFALSE)
#define is_true(v)   check_vartype(to_bool(v), V_VTRUE)

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

/* O_VCCLASS, O_VROLE */
#define O_CLASS     5

#define O_INSTANCE  6

/* O_VSHTSTR, O_VLNGSTR */
#define O_STR       7

#define O_RANGE     8    

/* O_VSET, O_VMSET, O_VLIST */
#define O_ARRAY     9

/* O_VBAG, O_VMBAG */
#define O_MAP       10

/* O_VCHAN, O_VSCHEDQ */
#define O_RBQ       11

/* O_VCLOSURE */
#define O_FUN       12

#define O_UPVAL     13

/* O_VGFUN, O_VMCO */
#define O_CO        14

#define O_MA        15
#define O_WORK      16

#define O_REGEX     17
#define O_SOCKET    18
#define O_PIPE      19
#define O_FILE      20
#define O_DIR       21
#define O_PROC      22
#define O_SYS       23
#define O_DATE      24
#define O_NS        25
#define O_TERM      26

/* A Check macro on 'v' for each object type */
#define is_ma(v)      o_check_type(v, O_MA)
#define is_rbq(v)     o_check_type(v, O_RBQ)
#define is_regex(v)   o_check_type(v, O_REGEX)
#define is_socket(v)  o_check_type(v, O_SOCKET)
#define is_pipe(v)    o_check_type(v, O_PIPE)
#define is_file(v)    o_check_type(v, O_FILE)
#define is_dir(v)     o_check_type(v, O_DIR)
#define is_proc(v)    o_check_type(v, O_PROC)
#define is_date(v)    o_check_type(v, O_DATE)
#define is_term(v)    o_check_type(v, O_TERM) /* See term.h */

/* Test if the value 'v' is an object variant of variant type 't' */
#define o_check_vartype(v,t)  is_obj(v) && check_vartype(v,t)


/* A threadsafe Channel and Scheduler queue */
#define O_VCHAN    vary(O_RBQ, 1)
#define O_VSCHEDQ  vary(O_RBQ, 2)

#define is_chan(v)    o_check_type(v, O_VCHAN)
#define is_schedq(v)  o_check_type(v, O_VSCHEDQ)

/*
 * ##String object.
 * #str: The UTF-8 encoded string itself.
 * #hash: Hash value of $str.
 * #rlen: Real length of $str.
 * #len: User-percieved length of $str.
 */
#define O_VSHTSTR  vary(O_STR, 1)
#define O_VLNGSTR  vary(O_STR, 2)

#define iss_str(v)    o_check_vartype(v, O_STR)
#define is_str(v)     o_check_type(v, O_STR)
#define is_shtstr(v)  o_check_vartype(v, O_VSHTSTR)
#define is_lngstr(v)  o_check_vartype(v, O_VLNGSTR)

#define as_str(v)     (ma_assert(is_str(v)), cast(Str *, as_obj(v)))
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
#define is_range(v)  o_check_type(v, O_RANGE)
#define as_range(v)  (ma_assert(is_range(v)), cast(Range *, as_obj(v)))

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
#define O_VBAG   vary(O_MAP, 1)
#define O_VMBAG  vary(O_MAP, 2)

#define iss_map(v)  o_check_vartype(v, O_MAP)
#define is_map(v)   o_check_type(v, O_MAP)
#define is_bag(v)   o_check_vartype(v, O_VBAG)
#define is_mbag(v)  o_check_vartype(v, O_VMBAG)

#define as_map(v)   (ma_assert(is_map(v)), cast(Map *, as_obj(v)))
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
 * #cap: Its capacity.
 */

/* Variants: comma-separated list of values, immutable and mutable sets */
#define O_VLIST  vary(O_ARRAY, 1)
#define O_VSET   vary(O_ARRAY, 2)
#define O_VMSET  vary(O_ARRAY, 3)

#define iss_array(v)  o_check_vartype(v, O_ARRAY)
#define is_array(v)   o_check_type(v, O_ARRAY)
#define is_list(v)    o_check_vartype(v, O_VLIST)
#define is_set(v)     o_check_vartype(v, O_VSET)
#define is_mset(v)    o_check_vartype(v, O_VMSET)

#define as_array(v)  (ma_assert(is_array(v)), cast(Array *, as_obj(v)))
#define as_list(v)   as_array(v)
#define as_set(v)    as_array(v)
#define as_mset(v)   as_array(v)

typedef struct Array {
   Object obj;
   size_t size;
   size_t cap;
   Value *array;
} Array;

/*
 * ##Representation of a class, every collectable object has a
 * class, even the class itself. The Class of objects which
 * aren't first class values are probably useless, e.g Upval.
 *
 * #name: The class' name.
 * #meths: Pointer to a map for the class' methods. A value to
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
#define O_VROLE  vary(O_CLASS, 1)

#define iss_class(v)  o_check_vartype(v, O_CLASS)
#define is_class(v)   o_check_type(v, O_CLASS)
#define is_role(v)    o_check_vartype(v, O_VROLE)

#define as_class(v)  (ma_assert(is_class(v)), cast(Class *, as_obj(v)))
#define as_role(v)   as_class(v)

typedef struct Class {
   Object obj;
   Str *name;
   Value *fields;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Map *meths;
} Class;

#define O_VCCLASS  vary(O_CLASS, 2)

#define is_cclass(v)  o_check_vartype(v, O_VCCLASS)
#define as_cclass(v)  (ma_assert(is_class(v)), cast(Cclass *, as_obj(v)))

typedef struct Cclass {
   Object obj;
   Str *name;
   Value cdata;
   size_t size;
   Map *meths;
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
#define is_instance(v)  o_check_type(v, O_INSTANCE)
#define as_instance(v)  (ma_assert(is_instance(v)), cast(Instance *, as_obj(v)))

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
 * #mux: mutex to protect write access to #ours.
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
#define is_ns(v)  o_check_type(v, O_NS)
#define as_ns(v)  (ma_assert(is_ns(v)), cast(Namespace *, as_obj(v)))

typedef struct Namespace {
   Object obj;
   Str *name; 
   Mutex *mux;
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
#define is_fun(v)  o_check_type(v, O_FUN)
#define as_fun(v)  (ma_assert(is_fun(v)), cast(Fun *, as_obj(v)))

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
#define O_VCLOSURE  vary(O_FUN, 1)

#define is_closure(v)  o_check_vartype(v, O_VCLOSURE)
#define as_closure(v)  (ma_assert(is_closure(v)), cast(Closure *, as_obj(v)))

typedef struct Closure {
   Object obj;
   Fun *fun;
   Upvalue *upvals;
} Closure;

/* ##Union of all collectable objects used for conversion. */

/* Cast 'o' to a pointer to an Ounion struct */
#define ounion_of(o)  cast(Ounion *, o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */
#define o2str(o)   (ma_assert(check_type(v, O_STR)), &(ounion_of(o)->str))
#define o2ar(o)    (ma_assert(check_vartype(v, O_ARRAY)), &(ounion_of(o)->ar))
#define o2map(o)   (ma_assert(check_vartype(v, O_MAP)), &(ounion_of(o)->map))
#define o2fun(o)   (ma_assert(check_vartype(v, O_FUN)), &(ounion_of(o)->fun))
#define o2clo(o)   (ma_assert(check_vartype(v, O_VCLOSURE)), &(ounion_of(o)->clo))
#define o2uv(o)    (ma_assert(check_vartype(v, O_UV)), &(ounion_of(o)->uv))
#define o2cls(o)   (ma_assert(check_type(v, O_CLASS)), &(ounion_of(o)->cls))
#define o2ccls(o)  (ma_assert(check_vartype(v, O_VCLASS)), &(ounion_of(o)->ccls))
#define o2ins(o)   (ma_assert(check_type(v, O_INSTANCE)), &(ounion_of(o)->ins))
#define o2co(o)    (ma_assert(check_vartype(v, O_CO)), &(ounion_of(o)->co))
#define o2ma(o)    (ma_assert(check_vartype(v, O_MA)), &(ounion_of(o)->ma))
#define o2wk(o)    (ma_assert(check_vartype(v, O_WORK)), &(ounion_of(o)->wk))
#define o2rbq(o)   (ma_assert(check_vartype(v, O_RBQ)), &(ounion_of(o)->rbq))
#define o2ns(o)    (ma_assert(check_vartype(v, O_NS)), &(ounion_of(o)->ns))
#define o2mvm(o)   (ma_assert(check_type(v, O_MVM)), &(ounion_of(o)->mvm))

/* The other way around */
#define xo2o(v)  (ma_assert(is_obj(v)), &(ounion_of(o)->obj))

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

#endif

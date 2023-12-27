/*
 * ###Definition Maat Values and Objects
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_val_h
#define ma_val_h

#include "ma_conf.h"
#include "ma_limits.h"

/*
 * Macros defined below are both used to manipulate objects and non
 * NAN-TAGGING represented values, reason why they aren't specific
 * to `#if` body of the "MA_NAN_TAGGING" conditional test.
 */

/* Vary type 't' with variant bits 'v', see Value */
#define vary(t, v)  ((t) | (v << 5))

#define type(v)              raw_type(v)
#define raw_type(v)          ((v)->type)
#define with_variant(v)      (raw_type(v) & 0x7F)
#define without_variant(v)   (raw_type(v) & 0x1F)
#define check_type(v, t)     (without_variant(v) == t)
#define check_vartype(v, t)  (with_variant(v) == t)
#define check_rtype(v, t)    (raw_type(v) == t)

#if !defined(MA_NAN_TAGGING)

/* ##Representation of a Maat value.
 *
 * #val: The value itself.
 *   #n: Representation of a number (see 'ma_conf.h').
 *   #obj: Pointer to a collectable object which make val
 *         collectable.
 *   #p: Pointer to a memory location representing the data of a
 *       Cclass.
 *   #f: Pointer to a C function, it either represents a cdata's
 *       method or just a standalone function.
 *
 * #type: Determines $val's type, its bits are segmented into
 * three parts. This variable already gots all what is
 * necessary to represent values of the boolean and nil types.
 *
 * Bits distribution:
 * - Bits 0-4:
 *   Represents the different types of types regardless of
 *   whether they are collectable or not, here we have maximum
 *   32 objects which suffices.
 *
 * - Bits 5-6:
 *   Represents variants of certain (O_)?.* types. You can have
 *   at most 3 variants for each base type which also suffices.
 *
 * - Bit 7: is 1 if #val stores a collectable object and 0
 *   otherwise.
 */
typedef struct Value {
   Ubyte type;
   union {
      Num n;
      CFunc f;
      void *p;
      struct Object *obj;
   } val;
} Value;

#define val(v)          ((v)->val)
#define set_type(v, t)  (type(v) = t)
#define set_cval(v, o)  { set_type(v, ctb(o->type)); \
                          (val(v).obj = x2o(o)); }

/*
 * A value stores an object. Defines all non-collectable objects
 * (or types?). A value 'v' is collectable if the MSB of its
 * #type is 1 which means it stores a collectable object.
 */
#define V_NIL    0
#define V_BOOL   1
#define V_NUM    2
#define V_CFUNC  3
#define V_CDATA  4

#define is_num(v)    check_type(v, V_NUM)
#define is_nil(v)    check_type(v, V_NIL)
#define is_bool(v)   check_type(v, V_BOOL)
#define is_cfunc(v)  check_type(v, V_CFUNC)
#define is_cdata(v)  check_type(v, V_CDATA)

#define IS_COLLECTABLE_BIT  (0b1 << 7)

/* Check if value 'v' is collectable, set collectable bit */
#define is_ctb(v)  (type(v) & IS_COLLECTABLE_BIT)
#define ctb(v)     (type(v) | IS_COLLECTABLE_BIT)

#define as_bool(v)   (ma_assert(is_bool(v)), (is_true(v)))
#define as_num(v)    (ma_assert(is_num(v)), (val(v).n))
#define as_cfunc(v)  (ma_assert(is_cfunc(v)), (val(v).f))
#define as_cdata(v)  (ma_assert(is_cdata(v)), (val(v).p))

/* as_obj() for collectable objects defined below */
#define as_obj(v)    (ma_assert(is_obj(v)), (val(v).obj))

/*
 * Define variants of the 'nil' type and its singleton values.
 *
 * V_VFREE: This nil variant differentiates a key whose
 * corresponding value is a user-perceived 'nil' from a free
 * position in a map.
 *
 * V_VABSKEY: When indexing a hash map with a key that isn't
 * found, return a dummy value of type V_VABSKEY.
 */
#define V_VNIL     vary(V_NIL, 0)
#define V_VFREE    vary(V_NIL, 1)
#define V_VABSKEY  vary(V_NIL, 2)

#define FREE    (Value){ V_VFREE,   { 0 } }
#define ABSKEY  (Value){ V_VABSKEY, { 0 } }

#define is_nil(v)     check_type(v, V_NIL)
#define iss_nil(v)    check_rtype(v, V_VNIL)
#define is_free(v)    check_rtype(v, V_VNILKEY)
#define is_abskey(v)  check_rtype(v, V_VABSKEY)

/* Define the boolean type variants with its singleton values. */
#define V_VFALSE  vary(V_BOOL, 0)
#define V_VTRUE   vary(V_BOOL, 1)

#define FALSE  (Value){ V_VFALSE, { 0 } }
#define TRUE   (Value){ V_VTRUE,  { 0 } }

#define is_false(v)  check_rtype(to_bool(v), V_VFALSE)
#define is_true(v)   check_rtype(to_bool(v), V_VTRUE)

#define to_bool(v)  (ma_likely(is_bool(v)) ? v : coerce_to_bool(v))

#else

/*
 * TODO
 */
typedef uint64_t Value;

#endif

/* Header common to all Maat objects */
#define Header  Ubyte type; \
                Ubyte mark; \
                struct Class *next; \
                struct Object *next

/*
 * ##Object struct inherited by all the below objects.
 *
 * #type: Type of the object.
 * #mark: Flag to mark the object during collection.
 * #class: The object's class.
 * #next: Next obj, to keep track of all objects.
 */
typedef struct Object {
   Header;
} Object;

/* Test if the value 'v' is an object of type 't' */
#define o_check_type(v, t)  is_ctb(v) && check_type(v, t)

/* Here are base colletable objects, some do have variants */

/* O_VCCLASS, O_VROLE */
#define O_CLASS     5

#define O_INSTANCE  6

/* O_VSHTSTR, O_VLNGSTR */
#define O_STR       7

/* O_VULNGSTR, O_VUSHTSTR */
#define O_U8STR     8

#define O_RANGE     9    

/* O_VLIST */
#define O_ARRAY     10

#define O_MAP       11

/* O_VCHAN, O_VSCHEDQ */
#define O_RBQ       13

/* O_VCLOSURE */
#define O_FUN       14

#define O_UPVAL     15

/* O_VGFUN, O_VCO */
#define O_STATE     16

#define O_MA        17
#define O_WORK      18

#define O_NS        19
#define O_TERM      20

/* Test if the value 'v' is an object of variant type 't' */
#define o_check_rtype(v, t)  is_obj(v) && check_rtype(v, t)

/* A threadsafe Channel and Scheduler queue */
#define O_VCHAN    vary(O_RBQ, 1)
#define O_VSCHEDQ  vary(O_RBQ, 2)

#define is_chan(v)    o_check_type(v, O_VCHAN)
#define is_schedq(v)  o_check_type(v, O_VSCHEDQ)

/* ##Representation of a string object. */
#define Str_Header  Object *next_nsobj;                         \
                    Ubyte sl;                                   \
                    union { size_t len; struct Str *snext; } u; \
                    Uint hash;                                  \
                    Byte *str

#define O_VLNGSTR   vary(O_STR, 0)
#define O_VSHTSTR   vary(O_STR, 1)
#define O_VUSHTSTR  vary(O_U8STR, 0)
#define O_VULNGSTR  vary(O_U8STR, 1)

#define is_str(v)   (is_astr(v) || is_ustr(v))
#define is_astr(v)  o_check_type(v, O_STR)
#define is_ustr(v)  o_check_type(v, O_U8STR)

#define is_shtstr(v)  (o_check_rtype(v, ctb(O_VSHTSTR)) || \
                       o_check_rtype(v, ctb(O_VUSHTSTR)))
#define is_lngstr(v)  (o_check_rtype(v, ctb(O_VLNGSTR)) || \
                       o_check_rtype(v, ctb(O_VULNGSTR)))

#define as_astr(v)   (ma_assert(is_str(v)), cast(Str *, as_obj(v)))
#define as_u8str(v)  (ma_assert(is_u8str(v)), cast(U8Str *, as_obj(v)))

/*
 * ##ASCII string struct.
 *
 * #Str_Header:
 *   #str: The string itself.
 *   #sl: Length of #str for short strings with its MSB
 *   determining whether #str is reserved. For long strings, #sl
 *   serves as a boolean value to check if #str already has its
 *   #hash.
 *   #u: For short strings, #u is #snext which is a pointer to
 *   #str's next string in our hash map of short strings
 *   (##SMap). For long strings, #u is the #len of #str.
 *   #hash: Hash value of #str.
 */
typedef struct Str {
   Header;
   Str_Header;
} Str;

/*
 * ##UTF8-8 encoded string struct.
 *
 * #Str_Header: The string header explained above.
 * #ngraph: Total size of the subparts of the grapheme clusters
 * of #str.
 */
typedef struct U8Str {
   Header;
   Str_Header;
   size_t ngraph;
} U8Str;

/* ##Range object(inclusive), #x the start and #y the end. */
#define is_range(v)  o_check_rtype(v, ctb(O_RANGE))
#define as_range(v)  (ma_assert(is_range(v)), cast(Range *, as_obj(v)))

typedef struct Range {
   Header;
   Num x;
   Num y;
} Range;

/* ##Representation of a Map object. */

/* The Node struct.
 * #val: Node's value.
 * #key:
 *   #k: The key itself.
 *   #next: Next node in case of collision.
 *   #m_key: When necessary, Maat provides locks for concurrent
 *   access to variables(not values), in a condition where a
 *   Map is the #ours of a namespace, a mutex is allocated for
 *   each of these variables.
 */
typedef struct Node {
   struct {
      Value k;
      Mutex *m_key;
      Int next;
   } key;
   Value val;
} Node;

/*
 * The map itself.
 *
 * #lsize:
 * #array:
 * #asize:
 * #node:
 */
#define is_map(v)   o_check_rtype(v, ctb(O_MAP))
#define as_map(v)   (ma_assert(is_map(v)), cast(Map *, as_obj(v)))

typedef struct Map {
   Header;
   Value *array;
   Uint asize;
   Ubyte lsize;
   Node *node;
   Object *next_nsobj;
} Map;

/* ##Representation of an Array object.
 *
 * #array: The array itself.
 * #size: The size of the array.
 * #cap: The capacity of the array.
 */

/* Variant: list object: comma-separated list of values */
#define O_VLIST  vary(O_ARRAY, 1)

#define iss_array(v)  o_check_rtype(v, ctb(O_ARRAY))
#define is_array(v)   o_check_type(v, O_ARRAY)
#define is_list(v)    o_check_rtype(v, ctb(O_VLIST))

#define as_array(v)  (ma_assert(is_array(v)), cast(Array *, as_obj(v)))
#define as_list(v)   as_array(v)

typedef struct Array {
   Header;
   size_t size;
   size_t cap;
   Value *array;
   Object *next_nsobj;
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
 *  #roles: Keeps the list of roles that this class ":does".
 *  #sups: Keeps the list of directly inherited superclasses.
 *  #sups exist mainly for introspection since #c3 handles super
 *  calls.
 *
 * And for a Cclass we have:
 *  #cdata: A pointer to the raw memory.
 *  #size: The size of the memory pointed by #cdata.
 */
#define O_VROLE  vary(O_CLASS, 1)

#define iss_class(v)  o_check_rtype(v, ctb(O_CLASS))
#define is_class(v)   o_check_type(v, O_CLASS)
#define is_role(v)    o_check_rtype(v, ctb(O_VROLE))

#define as_class(v)  (ma_assert(is_class(v)), cast(Class *, as_obj(v)))
#define as_role(v)   as_class(v)

typedef struct Class {
   Header;
   Str *name;
   Value *fields;
   Map *meths;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Object *next_nsobj;
} Class;

#define O_VCCLASS  vary(O_CLASS, 2)

#define is_cclass(v)  o_check_rtype(v, ctb(O_VCCLASS))
#define as_cclass(v)  (ma_assert(is_cclass(v)), cast(Cclass *, as_obj(v)))

typedef struct Cclass {
   Header;
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
 * #sup_level: Keeps track of the level of super calls for each
 * method called on the instance, in otherwords it is a list 
 * of indexes to super methods cached at the level of this
 * instance's class, it is set 'NULL' if none of the methods
 * of this instance's class does a super call.
 *
 * "self.SUPER::<method_name>(...)" Where SUPER is a pseudo-class
 * that resolves to the next class in the instance's class c3
 * list.
 */
#define is_instance(v)  o_check_rtype(v, ctb(O_INSTANCE))
#define as_instance(v)  (ma_assert(is_instance(v)), cast(Instance *, as_obj(v)))

typedef struct Instance {
   Header;
   Value *fields;
   Map *sup_level;
   Object *next_nsobj;
} Instance;

/*
 * ##Represents a namespace e.g FOO::BAR. A namespace can either
 * be represented as a package, role or (c)class.
 * "FOO::BAR::x()" is a call to the function "x" in "FOO::BAR"
 * if ever there is.
 *
 * #name: Namespace's name.
 * #ours: Stores it globals (uses mutex when required)
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
 * #exports: Names of to-be-exported globals when this namespace
 * is used by another.
 *
 * NB:
 * - The argument to the "use" statement never translates to a
 *   namespace.
 * - Not all classes or roles are namespaces, some can be
 *   lexically scoped and thus can never be fully qualified.
 * - For consistency, you cannot nest namespaces as they don't
 *   really have an identity.
 * 
 * #ns_val: The namespace value, a package, class, or role.
 */
#define is_ns(v)  o_check_rtype(v, ctb(O_NS))
#define as_ns(v)  (ma_assert(is_ns(v)), cast(Namespace *, as_obj(v)))

typedef struct Namespace {
   Header;
   Str *name; 
   Map *ours;
   ExportBuf export;
   Value ns_val;
} Namespace;

/*
 * ##Struct of a Maat function.
 *
 * #arity: The number of arguments the function takes.
 * #code: Its bytecode.
 * #constants: The function's constant values.
 * #ns: Index of the #NSBuf to access the namespace of this
 * function.
 */
#define is_fun(v)  o_check_rtype(v, ctb(O_FUN))
#define as_fun(v)  (ma_assert(is_fun(v)), cast(Fun *, as_obj(v)))

typedef struct Fun {
   Header;
   Ubyte arity;
   size_t ns;
   CodeBuf code;
   ValueBuf constants;
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
 * #state: A pointer to the #next open upvalue when this one is
 * still opened otherwise it'll contain the vm register value
 * #p pointed to.
 * #m_uv: Whenever an upval is shared by closures of States
 * distributed accross maatines, a mutex must be used to manage
 * it concurrent access. This field is optional as this
 * condition can only be determined at runtime.
 */
typedef struct Upval {
   Header;
   Value *p;
   union {
      Value val;
      struct Upval *next;
   } state;
   Mutex *m_uv;
} Upval;

/* ##A closure is a variant of a function which keep tracks of
 * its upvalues.
 *
 * #fun: A pointer to the closure's function.
 * #nupvals: The number of upvalues.
 * #upvals: The List of upvalues the function has.
 */
#define O_VCLOSURE  vary(O_FUN, 1)

#define is_closure(v)  o_check_rtype(v, ctb(O_VCLOSURE))
#define as_closure(v)  (ma_assert(is_closure(v)), cast(Closure *, as_obj(v)))

typedef struct Closure {
   Header;
   Object *next_nsobj;
   Fun *fun;
   Ubyte nupvals;
   Upval *upvals;
} Closure;

/* ##The definition of these objects are in 'ma_state.h'  */

/* ##Type operation macros for the State object. */

#define O_VCO  vary(O_STATE, 1)

#define iss_state(v)  o_check_rtype(v, ctb(O_STATE))
#define is_state(v)   o_check_type(v, O_STATE)
#define is_co(c)      o_check_rtype(v, ctb(O_VCO))

#define as_state(v)  (ma_assert(is_state(v)), cast(State *, as_obj(v)))
#define as_co(v)     as_state(v)

/* ##Type operation macros for the Work object. */
#define is_work(v)  o_check_rtype(v, ctb(O_WORK))
#define as_work(v)  (ma_assert(is_state(v)), cast(Work *, as_obj(v)))

/* ##Type operation macros for a Maatine object. */
#define is_ma(v)  o_check_rtype(v, ctb(O_MA))
#define as_ma(v)  (ma_assert(is_ma(v)), cast(Ma *, as_obj(v)))

/* ##Union of all collectable objects used for conversion. */

/* Cast 'o' to a pointer to an Ounion struct */
#define ounion_of(o)  cast(Ounion *, o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */

#define co_type(o, t)  is_ctb(o) && (type(o) == t)

#define o2str(o)   (ma_assert(co_type(o, O_STR) || co_type(v, O_U8STR)), &(ounion_of(o)->str))
#define o2ar(o)    (ma_assert(co_type(o, O_ARRAY)), &(ounion_of(o)->ar))
#define o2map(o)   (ma_assert(co_type(o, O_MAP)), &(ounion_of(o)->map))
#define o2fun(o)   (ma_assert(co_type(o, O_FUN)), &(ounion_of(o)->fun))
#define o2clo(o)   (ma_assert(co_type(o, O_VCLOSURE)), &(ounion_of(o)->clo))
#define o2uv(o)    (ma_assert(co_type(o, O_UV)), &(ounion_of(o)->uv))
#define o2cls(o)   (ma_assert(co_type(o, O_CLASS)), &(ounion_of(o)->cls))
#define o2ccls(o)  (ma_assert(co_type(o, O_VCCLASS)), &(ounion_of(o)->ccls))
#define o2ins(o)   (ma_assert(co_type(o, O_INSTANCE)), &(ounion_of(o)->ins))
#define o2stt(o)   (ma_assert(co_type(o, O_STATE)), &(ounion_of(o)->stt))
#define o2ma(o)    (ma_assert(co_type(o, O_MA)), &(ounion_of(o)->ma))
#define o2wk(o)    (ma_assert(co_type(o, O_WORK)), &(ounion_of(o)->wk))
#define o2rbq(o)   (ma_assert(co_type(o, O_RBQ)), &(ounion_of(o)->rbq))
#define o2ns(o)    (ma_assert(co_type(o, O_NS)), &(ounion_of(o)->ns))
#define o2mvm(o)   (ma_assert(co_type(o, O_MVM)), &(ounion_of(o)->mvm))

/* The other way around */
#define x2o(v)  (ma_assert(is_obj(v)), &(ounion_of(o)->obj))

union Ounion {
   Object obj;
   Str str;
   Array ar;
   Map map;
   Fun fun;
   Closure clo;
   Upval uv;
   Class cls;
   Cclass ccls;
   Instance ins;
   State stt;
   Ma ma;
   Work wk;
   Rbq rbq;
   Namespace ns;
   MVM mvm;
} Ounion;

/* It is on its own as this is needed at 'ma_str.c' */
#define sunion_of(s)  cast(Sunion *, s)
#define a2u8(s)       (ma_assert(co_type(s, O_STR)), &(sunion_of(s)->as))
#define u82a(s)       (ma_assert(co_type(s, O_U6STR)), &(sunion_of(s)->u8s))

union Sunion {
   Str as;
   U8Str u8s;
} Sunion;

#endif

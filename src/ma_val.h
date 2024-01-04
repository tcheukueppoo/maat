/*
 * $$$Definition Maat Values and Objects
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_val_h
#define ma_val_h

#include "ma_conf.h"
#include "ma_limits.h"

/* Vary type 't' with variant bits 'v'. */
#define vary(t, vb)  ((t) | (vb << 5))

/* 'x' can either be a 'Value' or a collectable 'Object'. */
#define type(x)             raw_type(x)
#define raw_type(x)         ((x)->type)
#define without_variant(x)  (raw_type(x) & 0x1F)
#define check_type(x, t)    (without_variant(x) == t)
#define check_rtype(x, t)   (raw_type(x) == t)

#define with_variant(v)      (raw_type(v) & 0x7F)
#define check_vartype(v, t)  (with_variant(v) == t)

/* $$Representation of a Maat value.
 *
 * $val: The value itself, which is a union of
 *  $n: Maat's representation of a number (see 'ma_conf.h').
 *  $gc_obj: Pointer to a collectable object.
 *  $p: Pointer to a memory location representing the data of a
 *  Cclass.
 *  $f: Pointer to a C function, it either represents a cdata's
 *  method or just a standalone function.
 *
 * $type: Determines $val's type, its bits are segmented into
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
 * - Bit 7: is 1 if $val stores a collectable object and 0
 *   otherwise.
 */
typedef union _Value {
   Num n;
   CFunc f;
   void *p;
   struct Object *gc_obj;
} _Value;

#define Valuefields  Ubyte type; _Value val

typedef struct Value {
   Valuefields;
} Value;

#define val(v)          ((v)->val)
#define set_type(v, t)  (type(v) = t)
#define gco2val(v, o)   set_type(v, ctb(o->type)); \
                        (val(v).obj = x2gco(o))

/*
 * Defines types of all non-collectable objects. A value 'v' is
 * collectable if the MSB of its $type is 1 meaning it stores
 * a collectable object.
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

#define IS_CTB_BIT  (0b1 << 7)

/* Check if value 'v' is collectable, set collectable bit. */
#define is_ctb(v)  (type(v) & IS_CTB_BIT)
#define ctb(v)     (type(v) | IS_CTB_BIT)

#define as_bool(v)   (ma_assert(is_bool(v)), (is_true(v)))
#define as_num(v)    (ma_assert(is_num(v)), (val(v).n))
#define as_cfunc(v)  (ma_assert(is_cfunc(v)), (val(v).f))
#define as_cdata(v)  (ma_assert(is_cdata(v)), (val(v).p))

/*
 * 'as_gcobj()' for collectable objects defined below. Things
 * dealing with collectable objects are preceeded with '[oO]_'.
 */
#define as_gcobj(v)    (ma_assert(is_ctb(v)), (val(v).gc_obj))

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

/* Header common to all collectable Maat objects */
#define Header  Ubyte type; \
                Ubyte mark; \
                struct Class *next; \
                struct Object *next

/*
 * $$Object struct inherited by all the below objects.
 *
 * $type: Type of the object.
 * $mark: Flag to mark the object during collection.
 * $class: The object's class.
 * $next: Next obj, to keep track of all objects.
 */
typedef struct Object {
   Header;
} Object;

/* Here are base colletable objects, some do have variants */

/* O_VCLASS, O_VCCLASS, O_VROLE */
#define O_CLASS  5

/* O_VINST O_VCINST */
#define O_INST   6

/* O_VSHTSTR, O_VLNGSTR */
#define O_STR    7

/* O_VULNGSTR, O_VUSHTSTR */
#define O_U8STR  8

/* O_VRANGE, O_VSRANGE? */
#define O_RANGE  9    

/* O_VARRAY, O_VLIST */
#define O_ARRAY  10

/* O_VMAP, O_VNSMAP */
#define O_MAP    11

/* O_VCHAN, O_VSCHEDQ */
#define O_RBQ    13

/* O_VFUN, O_VCLOSURE */
#define O_FUN    14

/* O_VUPVAL */
#define O_UPVAL  15

/* O_VSTATE, O_VCO */
#define O_STATE  16

/* O_VMA */
#define O_MA     17

/* O_VWORK */
#define O_WORK   18

/* O_VNS */
#define O_NS     19

/* A thread-safe channel and scheduler queue */
#define O_VCHAN    vary(O_RBQ, 1)
#define O_VSCHEDQ  vary(O_RBQ, 2)

#define is_chan(v)    check_type(v, O_VCHAN)
#define is_schedq(v)  check_type(v, O_VSCHEDQ)

/* $$Representation of all string objects. */
#define O_VLNGSTR   vary(O_STR, 0)
#define O_VSHTSTR   vary(O_STR, 1)
#define O_VUSHTSTR  vary(O_U8STR, 0)
#define O_VULNGSTR  vary(O_U8STR, 1)

#define is_str(v)   (is_astr(v) || is_ustr(v))
#define is_astr(v)  check_type(v, O_STR)
#define is_ustr(v)  check_type(v, O_U8STR)

#define is_shtstr(v)  (check_rtype(v, ctb(O_VSHTSTR)) || \
                       check_rtype(v, ctb(O_VUSHTSTR)))
#define is_lngstr(v)  (check_rtype(v, ctb(O_VLNGSTR)) || \
                       check_rtype(v, ctb(O_VULNGSTR)))

#define as_str(v)    (ma_assert(is_str(v)), cast(Str *, as_gcobj(v)))
#define as_u8str(v)  (ma_assert(is_u8str(v)), cast(U8Str *, as_gcobj(v)))

#define Str_Header  Object *next_sobj; \
                    Ubyte sl; \
                    Uint hash; \
                    Byte *str; \
                    union { size_t len; struct Str *snext; } u;

/*
 * Ascii version of a Maat string, $str is the ascii string
 * itself and has hash value $hash.
 *
 * $sl: For short strings, last 3 bits of $sl is for the length
 * of $str whereas its first bit determines whether $str is
 * reserved. For long strings, $sl serves as a boolean value to
 * check if $str already has its $hash.
 * $u: For short strings, $u is $snext which is a pointer to
 * $str's next string in our hash map of short strings ($$SMap)
 * meanwhile for long strings, $u is the $len of $str.
 */
typedef struct Str {
   Header;
   Str_Header;
} Str;

/*
 * UTF-8 encoded string object, has $Str_Header plus $ngraph.
 * $ngraph is the total size of the subparts of the grapheme
 * clusters of $str.
 */
typedef struct U8Str {
   Header;
   Str_Header;
   size_t ngraph;
} U8Str;

/*
 * $$Range object with each bound inclusive.
 *
 * $a the start.
 * $b the end.
 * $c the step.
 */
#define O_VRANGE  vary(O_RANGE, 0)

#define is_range(v)  check_rtype(v, ctb(O_VRANGE))
#define as_range(v)  (ma_assert(is_range(v)), cast(Range *, as_gcobj(v)))

#define range2v(v, r)  gco2val(v, r)

typedef struct Range {
   Header;
   Num a;
   Num b;
   Int c;
} Range;

/* $$Representation of Map objects. */
#define O_VMAP    vary(O_MAP, 0)
#define O_VNSMAP  vary(O_MAP, 1)

#define is_map(v)    check_rtype(v, ctb(O_VMAP))
#define is_nsmap(v)  check_rtype(v, ctb(O_VNSMAP))

#define as_map(v)    (ma_assert(is_map(v)), cast(Map *, as_gcobj(v)))
#define as_nsmap(v)  as_map(v)

/* */
#define Nodefields  Ubyte key_t; \
                    Int next; \
                    _Value key_v

/*
 * $$Node of an `O_VMAP' map object.
 *
 * $val:
 *   Direct access to the Node's value, $val corresponds to
 *   $Valuefields since Node is a union.
 * $k: Node's key.
 *   $key_t: key's type.
 *   $key_v: Key's value.
 *   $next: Next node in case of collisions.
 */
typedef union Node {
   struct {
      Valuefields;
      Nodefields;
   } k;
   Value val;
} Node;

/* 
 * $$SNode of an `O_VNSMAP' map object, same as $$Node but has
 * mutex $m to manage concurrent access to entries. `O_VNSMAP'
 * maps are used by Namespaces to store their global symbols.
 */
typedef union SNode {
   struct {
      Valuefields;
      Nodefields;
      Mutex m;
   } k;
   Value val;
} SNode;

/*
 * The Map object. Our map has two parts: an array and a hash
 * part, its array part is used in an optimized way to store
 * nodes with non-negative integer keys. This array is used in
 * such a way that more than half of its slots are in use.
 * Its hash part stores the rest of the nodes obtained after
 * we've got our optimized $array.
 *
 * $array: Array part of the Map.
 * $asize: Size of the array part of the Map.
 * $rasize:
 *  Boolean value to check if $array's size $asize is actually
 *  its real size.
 * $node: Hash part of this Map, points to a Node/SNode.
 * $lnode: The last free node in $node.
 * $lg2size: log2 of the size of #node.
 */
typedef struct Map {
   Header;
   Ubyte rasize;
   Ubyte lg2size;
   Uint asize;
   Value *array;
   void *node;
   void *lnode;
   Object *next_sobj;
} Map;

/*
 * $$Representation of an Array object. Though we got a Map
 * optimized to be kind-of an array for fast access, Maat still
 * got a true Array object.
 *
 * $array: The array itself.
 * $size: The size of $array.
 * $cap: The capacity of $array.
 */

/* Variant: list object: comma-separated list of values */
#define O_VARRAY  vary(O_ARRAY, 0)
#define O_VLIST   vary(O_ARRAY, 1)

#define is_array(v)   check_type(v, O_ARRAY)
#define iss_array(v)  check_rtype(v, ctb(O_VARRAY))
#define is_list(v)    check_rtype(v, ctb(O_VLIST))

#define as_array(v)  (ma_assert(is_array(v)), cast(Array *, as_gcobj(v)))
#define as_list(v)   as_array(v)

typedef struct Array {
   Header;
   size_t size;
   size_t cap;
   Value *array;
   Object *next_sobj;
} Array;

/*
 * $$Representation of a class, every collectable object has a
 * class, even the class object itself. The Class of objects
 * which aren't first class values are probably useless, we
 * have for example Upvalues.
 *
 * $name: The class' name.
 * $meths:
 *   Pointer to a map for the class' methods. A value to a key
 *   here is a pointer to a Closure but can later on change to
 *   an Array of closures so as to cache super methods, the
 *   cache can never be invalidate since c3 linearization is
 *   done at compile time.
 *
 * A C class is a class whose implemention is done in foreign
 * languages like C or C++.
 *
 * variants, here the object Class defines 3 variants which are
 * Class, Role and Cclasse.
 *
 * For a Maat class and a Role we have the following fields:
 *  $c3: List of classes got after c3 linearization was applied.
 *  $fields:
 *    Pointer to an array of values, indexes correspond to
 *    fields with values holding their defaults.
 *  $roles: Keeps the list of roles that this class ":does".
 *  $sups:
 *    Keeps the list of directly inherited superclasses. $sups
 *    exist mainly for introspection since $c3 handles super
 *    calls.
 */
#define O_VCLASS  vary(O_CLASS, 0)
#define O_VROLE   vary(O_CLASS, 1)

#define is_class(v)   check_type(v, O_CLASS)
#define iss_class(v)  check_rtype(v, ctb(O_VCLASS))
#define is_role(v)    check_rtype(v, ctb(O_VROLE))

#define as_class(v)  (ma_assert(is_class(v)), cast(Class *, as_gcobj(v)))
#define as_role(v)   as_class(v)

typedef struct Class {
   Header;
   Str *name;
   Value *fields;
   Map *meths;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Object *next_sobj;
} Class;

/* $$A C class.
 *
 * $cdata: A pointer to a raw memory.
 * $size: The size of the memory pointed by $cdata.
 */
#define O_VCCLASS  vary(O_CLASS, 2)

#define is_cclass(v)  check_rtype(v, ctb(O_VCCLASS))
#define as_cclass(v)  (ma_assert(is_cclass(v)), cast(Cclass *, as_gcobj(v)))

typedef struct Cclass {
   Header;
   Str *name;
   size_t size;
   Map *meths;
} Cclass;

/* $$Instance of a c?class. */
#define is_inst(v)   check_type(v, O_INST)

#define O_VMINST vary(O_INST, 0)

#define is_minst(v)  check_rtype(v, ctb(O_VMINST))
#define as_minst(v)  (ma_assert(is_minst(v)), cast(MInst *, as_gcobj(v)))

/*
 * $$Instance of a Maat class.
 *
 * $fields:
 *   A pointer to a to-be-allocated array of values, each field
 *   has a unique id which corresponds to an index in $fields.
 *   This also holds inherited fields too.
 * $sup_level:
 *   Keeps track of the level of super calls for each method
 *   called on the instance, in otherwords it is a list of
 *   indexes to super methods cached at the level of this
 *   instance's class, it is set 'NULL' if none of the methods
 *   of this instance's class does a super call.
 *
 * "self.SUPER::<method_name>(...)" Where SUPER is a pseudo
 * class that resolves to the next class in the instance's class
 * c3 list.
 */
typedef struct MInst {
   Header;
   Value *fields;
   Map *sup_level;
   Object *next_sobj;
} MInstance;

/*
 * $$Instance of a Cclass.
 *
 * $cdata: Cclass' data.
 */
#define O_VCINST vary(O_INST, 1)

#define is_cinst(v)  check_rtype(v, ctb(O_VMINST))
#define as_cinst(v)  (ma_assert(is_cinst(v)), cast(CInst *, as_gcobj(v)))

typedef struct CInst {
   Header;
   void *cdata;
} CInstance;

/*
 * $$Represents a namespace e.g FOO::BAR. A namespace can either
 * be represented as a package, role or (c)class.
 * "FOO::BAR::x()" is a call to the function "x" in "FOO::BAR"
 * if ever there is.
 *
 * $name: Namespace's name.
 * $ours: Stores it globals (uses mutex when required)
 *
 * $ours of the package "main::" takes care of the following
 * type I & II special variables:
 *
 *   ENV, ARGC, ARGV, INC, PATH, SIG, DATA, $v, $o, $,, $/, $\
 *   $|, $", $$, $(, $), $<, $>, $f, and $0.
 *
 * Access to one these variables from another package most not
 * necessarily be done using its fully qualified form unless a
 * variables of the same name declared in a scope shields it.
 *
 * $exports: Names of to-be-exported globals when this namespace
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
 * $ns_val: The namespace value, a package, class, or role.
 */
#define O_VNS  vary(O_NS, 0)

#define is_ns(v)  check_rtype(v, ctb(O_VNS))
#define as_ns(v)  (ma_assert(is_ns(v)), cast(Namespace *, as_gcobj(v)))

typedef struct Namespace {
   Header;
   Str *name; 
   Map *ours;
   ExportBuf export;
   Value ns_val;
} Namespace;

/*
 * $$Struct of a Maat function.
 *
 * $arity: The number of arguments the function takes.
 * $code: Its bytecode.
 * $constants: The function's constant values.
 * $ns:
 *   Index of the $NSBuf to access the namespace of this
 *   function.
 */
#define O_VFUN  vary(O_FUN, 0)

#define is_fun(v)  check_rtype(v, ctb(O_VFUN))
#define as_fun(v)  (ma_assert(is_fun(v)), cast(Fun *, as_gcobj(v)))

typedef struct Fun {
   Header;
   Ubyte arity;
   size_t ns;
   CodeBuf code;
   ValueBuf constants;
} Fun;

/*
 * $$A function that keeps track of its upvalues is called a
 * closure, upvalues initially are lexically scoped variables
 * living in vm's registers, they were supposed to stop existing
 * when the program goes out of their scopes of definition but
 * since some functions need them, these lexicals are kept as
 * upvalues.
 *
 * $p:
 *   Pointer to upvalue, when an upvalue is opened, it points
 *   to a value in a vm register but when closed, it points to
 *   $state.
 * $state:
 *   A pointer to the $next open upvalue when this one is still
 *   opened otherwise it'll contain the vm register value $p
 *   pointed to.
 * $m_uv:
 *   Whenever an upvalue is shared by closures of States
 *   distributed across maatines, a mutex must be used to manage
 *   it concurrent access. This field is optional as this
 *   condition can only be determined at runtime.
 */
#define O_VUPVAL  vary(O_UPVAL, 0)

/* Upvals aren't first class values. */
typedef struct Upval {
   Header;
   Value *p;
   union {
      Value val;
      struct Upval *next;
   } state;
   Mutex m_uv;
} Upval;

/* $$A closure is a variant of a function which keep tracks of
 * its upvalues.
 *
 * $fun: A pointer to the closure's function.
 * $nupvals: The number of upvalues.
 * $upvals: The List of upvalues the function has.
 */
#define O_VCLOSURE  vary(O_FUN, 1)

#define is_closure(v)  check_rtype(v, ctb(O_VCLOSURE))
#define as_closure(v)  (ma_assert(is_closure(v)), cast(Closure *, as_gcobj(v)))

typedef struct Closure {
   Header;
   Object *next_sobj;
   Fun *fun;
   Ubyte nupvals;
   Upval **upvals;
} Closure;

/* $$The definition of these objects are in 'ma_state.h'  */

/* $$Macros of the State object. */
#define O_VSTATE  vary(O_STATE, 0)
#define O_VCO     vary(O_STATE, 1)

#define is_state(v)   check_type(v, O_STATE)
#define iss_state(v)  check_rtype(v, ctb(O_VSTATE))
#define is_co(c)      check_rtype(v, ctb(O_VCO))

#define as_state(v)  (ma_assert(is_state(v)), cast(State *, as_gcobj(v)))
#define as_co(v)     as_state(v)

/* $$Macros of the Work object. */
#define O_VWORK  vary(O_WORK, 0)

#define is_work(v)  check_rtype(v, ctb(O_VWORK))
#define as_work(v)  (ma_assert(is_state(v)), cast(Work *, as_gcobj(v)))

/* $$Macros of the Maatine object. */
#define O_VMA  vary(O_MA, 0)

#define is_ma(v)  check_rtype(v, ctb(O_VMA))
#define as_ma(v)  (ma_assert(is_ma(v)), cast(Ma *, as_gcobj(v)))

/* $$Union of all collectable objects used for conversion. */

/* Cast 'o' to a pointer to an Ounion struct. */
#define ounion_of(o)  cast(Ounion *, o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */
#define gco2str(o)   (ma_assert(check_type(o, O_STR) || check_type(v, O_U8STR)), &(ounion_of(o)->str))
#define gco2ar(o)    (ma_assert(check_type(o, O_ARRAY)), &(ounion_of(o)->ar))
#define gco2map(o)   (ma_assert(check_type(o, O_MAP)), &(ounion_of(o)->map))
#define gco2rng(o)   (ma_assert(check_type(o, O_RANGE)), &(ounion_of(o)->rng))
#define gco2fun(o)   (ma_assert(check_rtype(o, O_VFUN)), &(ounion_of(o)->fun))
#define gco2clo(o)   (ma_assert(check_rtype(o, O_VCLOSURE)), &(ounion_of(o)->clo))
#define gco2uv(o)    (ma_assert(check_type(o, O_UPVAL)), &(ounion_of(o)->uv))
#define gco2cls(o)   (ma_assert(check_rtype(o, O_VCLASS) || check_rtype(o, O_VROLE)), &(ounion_of(o)->cls))
#define gco2ccls(o)  (ma_assert(check_rtype(o, O_VCCLASS)), &(ounion_of(o)->ccls))
#define gco2ins(o)   (ma_assert(check_type(o, O_VINSTANCE)), &(ounion_of(o)->ins))
#define gco2stt(o)   (ma_assert(check_type(o, O_STATE)), &(ounion_of(o)->stt))
#define gco2ma(o)    (ma_assert(check_type(o, O_MA)), &(ounion_of(o)->ma))
#define gco2wk(o)    (ma_assert(check_rtype(o, O_VWORK)), &(ounion_of(o)->wk))
#define gco2rbq(o)   (ma_assert(check_rtype(o, O_VRBQ)), &(ounion_of(o)->rbq))
#define gco2ns(o)    (ma_assert(check_rtype(o, O_VNS)), &(ounion_of(o)->ns))

/* The other way around. */
#define x2gco(v)  (ma_assert(is_obj(v)), &(ounion_of(o)->gc_obj))

/* Union of all collectable objects */
union Ounion {
   Object gc_obj;
   Str str;
   Array ar;
   Map map;
   Range rng;
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
} Ounion;

/* It is on its own as this is needed at 'ma_str.c' */
#define sunion_of(s)  cast(Sunion *, s)
#define a2u8(s)       (ma_assert(check_type(s, O_STR)), &(sunion_of(s)->as))
#define u82a(s)       (ma_assert(check_type(s, O_U8STR)), &(sunion_of(s)->u8s))

union Sunion {
   Str as;
   U8Str u8s;
} Sunion;

#endif

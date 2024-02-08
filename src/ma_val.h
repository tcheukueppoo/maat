/*
 * $$$Definition Maat Values and Objects
 * License: MAAT, see LICENSE file for details.
 */

#ifndef ma_val_h
#define ma_val_h

#include "ma_conf.h"
#include "ma_limits.h"

/* Vary type 't' with variant bits 'vb'. */
#define vary(t, vb)  ((t) | (vb << 5))

/* 'x' can either be a 'Value' or a collectable 'Object'. */
#define type(x)             raw_type(x)
#define raw_type(x)         ((x)->type)
#define without_variant(x)  (raw_type(x) & 0x1F)
#define check_type(x, t)    (without_variant(x) == t)
#define check_rtype(x, t)   (raw_type(x) == t)

#define with_variant(v)      (raw_type(v) & 0x7F)
#define check_vartype(v, t)  (with_variant(v) == t)

/* $$Repr of a Maat value.
 *
 * $val: The value itself, which is a union of
 *  $n: Maat's representation of a number (see 'ma_conf.h').
 *  $gc_obj: Pointer to a collectable object.
 *  $p: Pointer to a memory location representing the data of a
 *  foreign class.
 *  $f: Pointer to a C function, it either represents a cvalue's
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

#define Valuefields  _Value val; Ubyte type

typedef struct Value {
   Valuefields;
} Value;

#define val(v)          ((v)->val)
#define set_type(v, t)  (type(v) = t)
#define gco2val(o, v)   set_type(v, ctb(o->type)); \
                        (val(v).gc_obj = x2gco(o))

/*
 * Defines types of all non-collectable objects. A value 'v' is
 * collectable if the MSB of its $type is 1 meaning it stores
 * a collectable object.
 */
#define V_NIL     0
#define V_BOOL    1
#define V_NUM     2
#define V_FFUNC   3
#define V_FVALUE  4

#define is_num(v)     check_type(v, V_NUM)
#define is_nil(v)     check_type(v, V_NIL)
#define is_bool(v)    check_type(v, V_BOOL)
#define is_ffunc(v)   check_type(v, V_FFUNC)
#define is_fvalue(v)  check_type(v, V_FVALUE)

#define IS_CTB_BIT  (0b1 << 7)

/* Check if value 'v' is collectable, set collectable bit. */
#define is_ctb(v)  (type(v) & IS_CTB_BIT)
#define ctb(v)     (type(v) | IS_CTB_BIT)

#define as_bool(v)    (ma_assert(is_bool(v)), (is_true(v)))
#define as_num(v)     (ma_assert(is_num(v)), (val(v).n))
#define as_ffunc(v)   (ma_assert(is_ffunc(v)), (val(v).f))
#define as_fvalue(v)  (ma_assert(is_fvalue(v)), (val(v).p))

/*
 * 'as_gcobj()' for collectable objects defined below. Stuffs
 * dealing with collectable objects are preceeded with '[oO]_'.
 */
#define as_gcobj(v)   (val(v).gc_obj)

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

/* Header common to all collectable Maat objects. */
#define Header  struct Class *class; \
                struct Object *next; \
                UInt mid; \
                Ubyte type; \
                Ubyte mark

/*
 * $$Object struct inherited by all the below objects.
 *
 * $type: Type of the object.
 * $mark: To mark the object during collection.
 * $class: The object's class.
 * $next: Next obj, to keep track of all objects.
 * $mid: The id of the maatine that created the object.
 */
typedef struct Object {
   Header;
} Object;

/* Here are base colletable objects, some do have variants */

/* O_VCLASS, O_VCCLASS, O_VROLE */
#define O_CLASS  5

/* O_VINS O_VCINS */
#define O_INS    6

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

/* O_VUPVAL, O_VSUPVAL */
#define O_UPVAL  15

/* O_VSTATE, O_VCO */
#define O_STATE  16

/* O_VMA */
#define O_MA     17

/* O_VWORK */
#define O_WORK   18

/* O_VNS */
#define O_NS     19

/* A thread-safe channel and scheduler ring buffer queue. */
#define O_VCHAN    vary(O_RBQ, 1)
#define O_VSCHEDQ  vary(O_RBQ, 2)

#define is_chan(v)    check_type(v, O_VCHAN)
#define is_schedq(v)  check_type(v, O_VSCHEDQ)

/* $$Repr of all string objects. */
#define O_VLNGSTR   vary(O_STR, 0)
#define O_VSHTSTR   vary(O_STR, 1)
#define O_VUSHTSTR  vary(O_U8STR, 0)
#define O_VULNGSTR  vary(O_U8STR, 1)

#define str2v(s, v)  gco2val(s, v)
#define v2str(v)     (ma_assert(is_str(v)), gco2str((v).gc_obj))

#define is_str(v)   (is_astr(v) || is_ustr(v))
#define is_astr(v)  check_type(v, O_STR)
#define is_ustr(v)  check_type(v, O_U8STR)

#define is_shtstr(v)  (check_rtype(v, ctb(O_VSHTSTR)) || \
                       check_rtype(v, ctb(O_VUSHTSTR)))
#define is_lngstr(v)  (check_rtype(v, ctb(O_VLNGSTR)) || \
                       check_rtype(v, ctb(O_VULNGSTR)))

#define as_str(v)    (ma_assert(is_str(v)), cast(Str *, as_gcobj(v)))
#define as_u8str(v)  (ma_assert(is_u8str(v)), cast(U8Str *, as_gcobj(v)))

#define Str_Header  UByte sl; \
                    Object *next_sobj; \
                    Byte *str; \
                    union { size_t len; struct Str *snext; } u; \
                    UInt hash

/*
 * Ascii version of a Maat string, $str is the ascii string
 * itself and has hash value $hash.
 *
 * $sl:
 *   For short strings, last 3 bits of $sl are for $str' length
 *   whereas its first bit determines whether $str is reserved.
 *   For long strings, $sl serves as a boolean value to check if
 *   $str already has its $hash.
 * $u:
 *   For short strings, $u is $snext which is a pointer to
 *   $str's next string in our hash map of short strings
 *   ($$SMap) meanwhile for long strings, $u is the $len of
 *   $str.
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

#define rng2v(r, v)  gco2val(r, v)
#define v2rng(v)     (ma_assert(is_rng(v)), gco2rng((v).gc_obj))

#define is_rng(v)  check_rtype(v, ctb(O_VRANGE))
#define as_rng(v)  (ma_assert(is_rng(v)), cast(Range *, as_gcobj(v)))

typedef struct Range {
   Header;
   Num a;
   Num b;
   Int c;
   Object *next_sobj;
} Range;

/* $$Repr of Map objects. */
#define O_VMAP   vary(O_MAP, 0)
#define O_VCMAP  vary(O_MAP, 1)

#define map2v(m, v)   gco2val(m, v)
#define v2map2(v, m)  (ma_assert(is_map(v), g))

#define is_map(v)   check_rtype(v, ctb(O_VMAP))
#define is_cmap(v)  check_rtype(v, ctb(O_VCMAP))

#define as_map(v)   (ma_assert(is_map(v)), cast(Map *, as_gcobj(v)))
#define as_cmap(v)  (ma_assert(is_map(v)), cast(CMap *, as_gcobj(v)))

/*
 * $$Node of a map object.
 *
 * $val:
 *   Direct access to the Node's value, $val is $Valuefields
 *   since Node is a union.
 * $k: Node's key.
 *   $key_t: key's type.
 *   $key_v: Key's value.
 *   $next: Offset to next node in case of collisions.
 *   $prev: Offset to the previous node, used for tracking back
 *          to the first entry.
 */
typedef union Node {
   struct {
      Valuefields;
      _Value key_v;
      Int next;
      Int prev;
      UByte key_t;
   } k;
   Value val;
} Node;

/*
 * The Map object. Our map has two parts: an array and a hash
 * part, its array part is used in an optimized way to store
 * nodes with non-negative integer keys. This array is allocated
 * such that more than half of its slots are in use. Its hash
 * part stores the rest of the nodes obtained after we've got
 * our optimized $array.
 *
 * $array: Array part of the Map.
 * $asize: Size of the array part of the Map.
 * $rasize:
 *  Boolean value to check if $array's size $asize is actually
 *  its real size.
 * $node:
 *   Hash part of the Map, can later on be casted to an $$SNode
 *   in case the Map is an `O_VNSMAP' map.
 * $last: The last free node in $node.
 * $lg2size: log2 of the size of $node.
 */
#define MapPointerFields Value *array; \
                         Node *node; \
                         Node *last; \
                         Object *next_sobj

#define MapUbyteFields  UByte rasize; \
                        UByte lg2size
typedef struct Map {
   Header;
   MapPointerFields;
   UInt asize;
   MapUbyteFields;
} Map;

/*
 * CMap is a thread-safe version of the Map object.
 *
 * TODO: doc fields.
 */
typedef struct CMap {
   Header;
   MapPointerFields;
   /* TODO: librs structures */
   UInt asize;
   /* TODO: librs structures? */
   MapUbyteFields;
} CMap;

/*
 * $$Repr of an Array object. Though we got a Map
 * optimized to be kind-of an array for fast access, Maat still
 * got a true Array object.
 *
 * Its variant is the list object, a comma-separated list of
 * values
 */
#define O_VARRAY   vary(O_ARRAY, 0)
#define O_VLIST    vary(O_ARRAY, 1)
#define O_VCARRAY  vary(O_ARRAY, 2)

#define arr2v(a, v)  gco2val(a, v)
#define v2arr(v)     (ma_assert(is_arr(v)), gco2arr((v).gc_obj))

#define is_arr(v)   check_type(v, O_ARRAY)
#define is_carr(v)  check_rtype(v, ctb(O_VCARRAY))
#define iss_arr(v)  check_rtype(v, ctb(O_VARRAY))
#define is_list(v)  check_rtype(v, ctb(O_VLIST))

#define as_carr(v)  (ma_assert(is_carr(v)), cast(CArray *, as_gcobj(v)))
#define as_arr(v)   (ma_assert(is_arr(v)), cast(Array *, as_gcobj(v)))
#define as_list(v)  as_arr(v)

#define ArrayFields  Object *next_sobj; \
                     Value *array; \
                     size_t size; \
                     size_t cap
/*
 * $size: The size of $array.
 * $cap: The capacity of $array.
 * $array: The array itself.
 */
typedef struct Array {
   Header;
   ArrayFields;
} Array;

/*
 * CArray is a thread-safe lock-free version of the Array
 * object.
 *
 * TODO: doc fields.
 */
typedef struct CArray {
   Header;
   ArrayFields;
   /* TODO: librs fields */
} CArray;

/*
 * $$Repr of a different class objects, every collectable object
 * has a class, even the class object itself. Class header of
 * objects which aren't first class values are probably useless,
 * e.g Upvalues.
 */

/*
 * Repr of a class' attribute.
 *
 * $name: The attribute's name.
 * $type: '1' if it's a reference attribute; '0' otherwise.
 * $val:
 *    Get attribute's value. $p, a pointer to the referenced
 *    value if the attribute is a reference attribute; $v
 *    otherwise.
 */
#define is_attref(a)      ((a)->type == 1)
#define attval(a)         ((a)->val.v)
#define attvalc(a)        (ma_likely(is_attref(a)) ? *((a)->val.p) : attval(a))
#define set_attref(a, v)  ((a)->val.p = &(v)); \
                          ((a)->type = 1)

typedef struct Attr {
   Str *name;
   union {
      Value *p;
      Value v;
   } val;
   UByte type;
} Attr;

/*
 * Repr of a Maat class with the variant type 'Role'.
 *
 * $name: The class' name.
 * $attrs:
 *    A buffer of attributes, holds attributes' default values,
 *    includes all inherited and roles attributes. Conflicts are
 *    resolved at class creation. This field is copied into the
 *    instance object at instanciation.
 * $c3: List of classes, c3 linearization result.
 * $csize: Size of the c3 list.
 * $roles:
 *    Keeps the list of roles the class ':does' regardless of
 *    which class variant it is.
 * $rsize: Size of the role list.
 * $sups:
 *    Keeps the list of directly inherited superclasses. $sups
 *    exists mainly for class introspection since $c3 handles
 *    super calls.
 * $ssize: Size of the super list.
 * $meths:
 *    A map to store the class' methods. A value to a key here
 *    is a pointer to a Closure but can later on change to
 *    an Array of closures so as to cache super methods, the
 *    cache can never be invalidate since c3 linearization is
 *    done at compile time.
 */
#define O_VCLASS  vary(O_CLASS, 0)
#define O_VROLE   vary(O_CLASS, 1)

#define cls2v(a, v)  gco2val(a, v)
#define v2cls(v)     (ma_assert(iss_cls(v)), gco2cls((v).gc_obj))

#define is_cls(v)   check_type(v, O_CLASS)
#define iss_cls(v)  check_rtype(v, ctb(O_VCLASS))
#define is_role(v)  check_rtype(v, ctb(O_VROLE))

#define as_cls(v)   (ma_assert(is_cls(v)), cast(Class *, as_gcobj(v)))
#define as_role(v)  as_cls(v)

typedef struct Class {
   Header;
   Str *name;
   AttrBuf *attrs;
   Map *meths;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Object *next_sobj;
   UByte rsize;
   UByte ssize;
   UByte csize;
} Class;

/*
 * Repr of a foreign class, an Fclass is a class whose
 * implemention is done in foreign languages like C or C++.
 *
 * $name: The foreign class' name
 * $meths: The foreign class' methods
 * $size: The size of the memory pointed by $fdata.
 */
#define O_VFCLASS  vary(O_CLASS, 2)

#define fcls2v(fc, v)  gco2val(fc, v)
#define v2fcls(v)      (ma_assert(is_fcls(v)), gco2fcls((v).gc_obj))

#define is_fcls(v)  check_rtype(v, ctb(O_VFCLASS))
#define as_fcls(v)  (ma_assert(is_fcls(v)), cast(FClass *, as_gcobj(v)))

typedef struct FClass {
   Header;
   Str *name;
   size_t size;
   Map *meths;
} FClass;

/*
 * $$Repr of classes' instances, for both Maat and other foreign
 * languages like C/C++.
 */

/* Instance of a Maat Class. */

#define inst2v(i, v)  gco2val(i, v)
#define is_inst(v)    check_type(v, O_INST)

/*
 * $$Instance of a Maat class. If this object ends up shared,
 * fields themselves are marked shared since they are the only
 * mutable part of this object.
 *
 * $fields:
 *   A pointer to a to-be-allocated array of values, each field
 *   has a unique id which corresponds to an index in $fields.
 *   This too also holds inherited fields.
 * $sup_level:
 *   Keeps track of super calls levels for each method called on
 *   the instance, in otherwords it is a list of indexes to
 *   super methods cached at the level of the instance's class.
 *   It is `NULL' if none of the methods implemented by the
 *   instance's class (checked from the class itself and then
 *   its roles) do a super call.
 *
 * "self.SUPER::<method_name>(...)" Where SUPER is a pseudo
 * class that resolves to the next class in the instance's class
 * c3 list.
 */
#define O_VMINS  vary(O_INS, 0)

#define is_mins(v)  check_rtype(v, ctb(O_VMINS))
#define as_mins(v)  (ma_assert(is_mins(v)), cast(MIns *, as_gcobj(v)))

#define v2mins(v)  (ma_assert(is_ins(v)), gco2mins((v).gc_obj))

typedef struct MIns {
   Header;
   Object *next_sobj;
   AttrBuf *attrs;
   Map *sup_level;
} MIns;

/*
 * $$Instance of a Cclass, this object doesn't have a thread-s-
 * afe lock-free variant, it's of the responsibility of the
 * foreign code to provide synchronization in case cdata has to
 * be accessed concurrently via its foreign methods.
 *
 * $cvalue: The foreign class' value.
 */
#define O_VCINS  vary(O_INS, 1)

#define is_cins(v)  check_rtype(v, ctb(O_VCINS))
#define as_cins(v)  (ma_assert(is_cins(v)), cast(CIns *, as_gcobj(v)))

#define v2cins(v)  (ma_assert(is_ins(v)), gco2cins((v).gc_obj))

typedef struct CIns {
   Header;
   Object *next_sobj;
   Value fvalue;
} CIns;

/*
 * $$Repr of a namespace e.g FOO::BAR. A namespace can either
 * be represented as a package, role or (c)class.
 * 'FOO::BAR::x()' is a call to the function 'x' in 'FOO::BAR'
 * if ever there is.
 *
 * $ours: Stores it globals.
 *
 * $ours of the package 'main::' takes care of the following
 * type I & II special variables:
 *
 *   ENV, ARGC, ARGV, INC, PATH, SIG, DATA, $v, $o, $,, $/, $\
 *   $|, $", $$, $(, $), $<, $>, $f, and $0.
 *
 * Accessing any of these variables from the main package most
 * not necessarily be done using a fully qualified form unless
 * a variable of the same name declared in a scope takes
 * precedence when the compiler resolves it.
 *
 * $exports:
 *   Names of to-be-exported globals when this namespace
 *   is used by another.
 *
 * NB:
 * - The argument to the 'use' statement never translates to a
 *   namespace.
 * - Classes and roles don't always have to give an identity to
 *   namespaces, some can exist as lexically scoped variables or
 *   global symbols of a namespace having an identity of a
 *   package.
 * - For consistency, you can't nest namespaces, so 'FOO::BAR'
 *   having identity of a package doesn't imply that 'BAR' is
 *   another package or a class in 'FOO'.
 * 
 * $val: The namespace value, either a package, class, or role.
 *
 * A namespace is not a first class value as it can bring
 * confusion between its global symbols and other namespaces.
 */
#define O_VNS  vary(O_NS, 0)

typedef struct Namespace {
   Header;
   Map *ours;
   ExportBuf export;
   Value val;
} Namespace;

/*
 * $$Repr of a Maat function object.
 *
 * $arity: The number of arguments the function takes.
 * $code: Its bytecode.
 * $cons: The function's constant values.
 * $ns: Access index to namespace of the function in $NSBuf.
 */
#define O_VFN  vary(O_FN, 0)

#define is_fn(v)  check_rtype(v, O_VFN)
#define as_fn(v)  (ma_assert(is_fn(v)), cast(Fn *, as_gcobj(v)))

typedef struct Fn {
   Header;
   Ubyte arity;
   size_t ns;
   CodeBuf code;
   ValueBuf cons;
} Fn;

/*
 * $$A function that has at least an upvalue is a closure but we
 * still have closures with no upvalues. An upvalue isn't a
 * first class value, it simply points to a value(of a lexical)
 * in the stack of a callframe so that this function can still
 * keep a reference to it when the executing function associated
 * to that callframe finishes its execution.
 *
 * $ncl:
 *   Boolean value, is '0' if only one closure closes over it;
 *   '1' otherwise.
 * $p:
 *   Pointer to the value. When an upvalue is opened, it points
 *   to a living value in a vm register but when closed, it
 *   points to $state.
 * $state:
 *   A pointer to the $next open upvalue when this one is still
 *   opened otherwise it'll contain the vm register value $p
 *   pointed to.
 * $m_uv:
 *   Field for an O_VSUPVAL, whenever an upvalue is shared by
 *   closures of States distributed across maatines, a mutex
 *   must be used to sync access operations to it. This field is
 *   optional as this condition can only be determined at
 *   runtime.
 */
#define UVfields  Ubyte ncl; \
                  Value *p; \
                  union { Value val; struct Upval *next; } state

#define O_VUPVAL  vary(O_UPVAL, 0)

typedef struct Upval {
   Header;
   UVfields;
} Upval;

#define O_VSUPVAL  vary(O_UPVAL, 1)

/*
 * Upvalues aren't first class values! these are utilities used
 * when synchronizing access to upvalues shared across maatines.
 */
#define is_vsupval(o)  check_rtype(o, O_VSUPVAL)
#define as_vsupval(o)  cast(SUpval *, o)

typedef struct SUpval {
   Header;
   UVfields;
   /* TODO: librs fields */
} SUpval;

/* $$A closure is a variant of a function which keep tracks of
 * its upvalues.
 *
 * $fn: A pointer to the closure's function.
 * $nuv: The number of upvalues.
 * $upvals: The List of upvalues the function has.
 * $state: Buffer of values to keep values of static variables.
 */
#define O_VCLOSURE  vary(O_FUN, 1)

#define is_closure(v)  check_rtype(v, ctb(O_VCLOSURE))
#define as_closure(v)  (ma_assert(is_closure(v)), cast(Closure *, as_gcobj(v)))

typedef struct Closure {
   Header;
   Object *next_sobj;
   Fn *fn;
   Ubyte nuv;
   Upval **upvals;
   ValueBuf state;
} Closure;

/* $$See definition of these objects in their respective header files. */

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

#define ounion_of(o)  cast(Ounion *, o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */
#define gco2mins(o)  (ma_assert(check_type(o, O_VMINS)), &(ounion_of(o)->mins))
#define gco2cins(o)  (ma_assert(check_type(o, O_VCINS)), &(ounion_of(o)->cins))
#define gco2str(o)   (ma_assert(check_type(o, O_STR) || check_type(v, O_U8STR)), &(ounion_of(o)->str))
#define gco2arr(o)   (ma_assert(check_type(o, O_ARRAY)), &(ounion_of(o)->ar))
#define gco2map(o)   (ma_assert(check_type(o, O_MAP)), &(ounion_of(o)->map))
#define gco2rg(o)    (ma_assert(check_type(o, O_RANGE)), &(ounion_of(o)->rng))
#define gco2fun(o)   (ma_assert(check_rtype(o, O_VFUN)), &(ounion_of(o)->fun))
#define gco2clo(o)   (ma_assert(check_rtype(o, O_VCLOSURE)), &(ounion_of(o)->clo))
#define gco2uv(o)    (ma_assert(check_type(o, O_UPVAL)), &(ounion_of(o)->uv))
#define gco2cls(o)   (ma_assert(check_rtype(o, O_VCLASS) || check_rtype(o, O_VROLE)), &(ounion_of(o)->cls))
#define gco2fcls(o)  (ma_assert(check_rtype(o, O_VFCLASS)), &(ounion_of(o)->fcls))
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
   Array arr;
   Map map;
   Range rng;
   Fn fn;
   Closure clo;
   Upval uv;
   Class cls;
   FClass fcls;
   MIns mins;
   CIns cins;
   State stt;
   Ma ma;
   Work wk;
   Rbq rbq;
   Namespace ns;
} Ounion;

/* It's on its own as it's needed at 'ma_str.c' and elsewhere. */
#define sunion_of(s)  cast(Sunion *, s)
#define a2u8(s)       (ma_assert(check_type(s, O_STR)), &(sunion_of(s)->as))
#define u82a(s)       (ma_assert(check_type(s, O_U8STR)), &(sunion_of(s)->u8s))

union Sunion {
   Str as;
   U8Str u8s;
} Sunion;

#endif

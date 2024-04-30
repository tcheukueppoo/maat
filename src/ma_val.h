/*
 * License: MAATL, see LICENSE file for copyright and license details.
 * @@@@Definition of Maat Values.
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

/* @@Value: Repr of a Maat value.
 *
 * - @val: The value itself, which is a union of
 *   - @n: Maat's repr of a number, see 'ma_conf.h'.
 *   - @gc_obj: Pointer to a collectable object.
 *   - @p: Pointer to a memory location representing the data of
 *     a foreign class.
 *   - @f: Pointer to a C function, it either represents a
 *     cvalue's method or just a standalone function.
 *
 * - @type: Determines @val's type, its bits are segmented into
 *   three parts. This variable already gots all what is
 *   necessary to represent values of the boolean and nil types.
 *
 * Bits distribution:
 * - Bits 0-4: Represents the different value types regardless
 *   of whether they are collectable or not, here we have maximum
 *   32 objects which suffices.
 *
 * - Bits 5-6: Represents variants of certain (O_)?.* types. You
 *   can have at most 3 variants for each base type which also
 *   suffices.
 *
 * - Bit 7: It is '1' if @val stores a collectable object; '0'
 *   otherwise.
 */
#define Valuefields  _Value val; UByte type

typedef union _Value {
   Num n;
   void *p;
   Ffn f;
   struct Object *gc_obj;
} _Value;

typedef struct Value {
   Valuefields;
} Value;

#define val(v)           ((v)->val)
#define cmp_val(v1, v2)  (type(v2) == type(v2))
#define set_type(v, t)   (type(v) = t)
#define gco2val(o, v)    set_type(v, ctb(o->type)); \
                         (val(v).gc_obj = x2gco(o))

/*
 * Define types of all non-collectable objects.
 *
 * A value 'v' is collectable if the MSB of its @type is '1' in
 * otherwords, it stores a collectable object.
 */
#define V_NIL     0
#define V_BOOL    1
#define V_NUM     2
#define V_FFN     3
#define V_FVALUE  4

#define is_num(v)     check_type(v, V_NUM)
#define is_nil(v)     check_type(v, V_NIL)
#define is_bool(v)    check_type(v, V_BOOL)
#define is_ffn(v)     check_type(v, V_FFN)
#define is_fvalue(v)  check_type(v, V_FVALUE)

#define IS_CTB_BIT  (0b1 << 7)

/* Check if value 'v' is collectable, set collectable bit. */
#define is_ctb(v)  (type(v) & IS_CTB_BIT)
#define ctb(v)     (type(v) | IS_CTB_BIT)

#define as_bool(v)    (ma_assert(is_bool(v)), (is_true(v)))
#define as_num(v)     (ma_assert(is_num(v)), (val(v).n))
#define as_ffn(v)     (ma_assert(is_ffn(v)), (val(v).f))
#define as_fvalue(v)  (ma_assert(is_fvalue(v)), (val(v).p))

/*
 * 'as_gcobj()' for collectable objects defined below. Stuffs
 * dealing with collectable objects are preceeded with '[oO]_'.
 */
#define as_gcobj(v)  (val(v).gc_obj)

/*
 * Define variants of the 'nil' type and its singleton values.
 *
 * - V_VFREE: This nil variant differentiates a key whose
 *   corresponding value is a user-perceived 'nil' from a free
 *   position in a map.
 *
 * - V_VABSKEY: When indexing a hash map with a key that isn't
 *   found, return a dummy value of type V_VABSKEY.
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
#define Header  struct Class *class;  \
                struct Object *next;   \
                UInt mid;               \
                UByte type;              \
                UByte mark

/*
 * @@Object struct inherited by all collectable objects.
 *
 * - @type: Type of the object.
 * - @mark: To mark the object during collection.
 * - @class: The object's class.
 * - @next: Next obj, to keep track of all objects.
 * - @mid: The id of the maatine that created the object.
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

/* O_VARRAY, O_VCARRAY, O_VLIST */
#define O_ARRAY  10

/* O_VMAP, O_VCMAP */
#define O_MAP    11

/* O_VCHAN, O_VSCHEDQ */
#define O_RBQ    13

/* O_VFN, O_VCLOSURE, O_VMETH */
#define O_FN     14

/* O_VUPVAL, O_VSUPVAL */
#define O_UPVAL  15

/* O_VSTATE, O_VCO, O_VGFUN */
#define O_STATE  16

/* O_VMA */
#define O_MA     17

/* O_VWORK */
#define O_WORK   18

/* O_VNS */
#define O_NS     19

/* @@@Repr of all type of string objects. */
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

#define Stringfields  UByte sl;            \
                      Object *next_sobj;    \
                      Byte *str;             \
                      union {                 \
                         struct Str *snext;    \
                         size_t len;            \
                      } u;                       \
                      UInt hash

/*
 * @@Str: Repr of an ASCII Maat string.
 *
 * - @sl: For short strings, last 3 bits of @sl are for @str'
 *   length whereas its first bit determines whether @str is
 *   reserved. For long strings, @sl serves as a boolean value
 *   to check if @str already has its @hash.
 * - @u: For short strings @u is @snext, a pointer to @str's
 *   next string in our hash map of short strings (@@SMap); for
 *   long strings, @u is the @len of @str.
 */
typedef struct Str {
   Header;
   Stringfields;
} Str;

/*
 * @@U8Str: Object repr of an UTF-8 encoded string.
 *
 * - @ngraph: The total size of the subparts of the grapheme
 *   clusters of @str.
 */
typedef struct U8Str {
   Header;
   Stringfields;
   size_t ngraph;
} U8Str;

/*
 * @@Range object with each bound inclusive.
 *
 * - @a: The start.
 * - @b: The end.
 * - @c: The step.
 */
#define O_VRANGE  vary(O_RANGE, 0)

#define rng2v(r, v)  gco2val(r, v)
#define v2rng(v)     (ma_assert(is_rng(v)), gco2rng((v).gc_obj))
#define v2rng_rw(v)  gco2rng((v).gc_obj)

#define is_rng(v)  check_rtype(v, ctb(O_VRANGE))
#define as_rng(v)  (ma_assert(is_rng(v)), cast(Range *, as_gcobj(v)))

typedef struct Range {
   Header;
   Object *next_sobj;
   Num a;
   Num b;
   Int c;
} Range;

/* @@@Repr of Map objects. */
#define O_VMAP   vary(O_MAP, 0)
#define O_VCMAP  vary(O_MAP, 1)

#define map2v(m, v)      gco2val(m, v)
#define v2map2(v, m)     (ma_assert(is_map(v)), gco2map((v).gc_obj))
#define v2map2_rw(v, m)  gco2map((v).gc_obj)

#define is_map(v)   check_rtype(v, ctb(O_VMAP))
#define is_cmap(v)  check_rtype(v, ctb(O_VCMAP))

#define as_map(v)   (ma_assert(is_map(v)), cast(Map *, as_gcobj(v)))
#define as_cmap(v)  (ma_assert(is_map(v)), cast(CMap *, as_gcobj(v)))

/*
 * @@Node: Node of a map object.
 *
 * - @val: Direct access to the node value from @k.
 * - @k: The node key with other searching parameters.
 *   - @key_t: The node key type.
 *   - @key_v: The node key value.
 *   - @next: Offset to next node in case of collision.
 *   - @prev: Offset to the previous node, used for tracking back
 *     to the first entry, it's indispensable for synced access
 *     operations if ever the map eventually ends up shared.
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
 * @@Map: Repr of a Map object.
 *
 * Map has two parts: an array and a hash part, its array part
 * is used in an optimized way to store nodes with non-negative
 * integer keys. This array is allocated such that more than
 * half of its slots are in use. Its hash part stores the rest
 * of the nodes obtained after @array's size was optimized.
 *
 * - @array: Array part of the Map.
 * - @asize: Size of the array part of the Map.
 *
 * - @rasize: Boolean value to check if @asize is actually its
 *   real size.
 *
 * - @node: Hash part of the Map.
 * - @last: The last free node in @node.
 * - @lg2size: log2 of the size of @node.
 */
#define MapPointerFields Value *array;     \
                         Node *node;        \
                         Node *last;         \
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
 * @@CMap: A thread-safe version of the Map object.
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
 * @@Repr of an Array object. Though we got a Map optimized to
 * be kind-of an array for fast access, Maat still got a true
 * Array object.
 *
 * Variants: its concurrent lock-free version and a list.
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

#define Arrayfields  Object *next_sobj; \
                     Value *array;       \
                     size_t cap;          \
                     size_t size
/*
 * @@Array: Repr of an Array object.
 *
 * - @size: The size of @array.
 * - @cap: The capacity of @array.
 * - @array: The array itself.
 */
typedef struct Array {
   Header;
   Arrayfields;
} Array;

/*
 * @@CArray: A thread-safe lock-free version of @@Array.
 *
 * TODO: doc fields.
 */
typedef struct CArray {
   Header;
   Arrayfields;
   /* TODO: librs fields */
} CArray;

/*
 * @@Fn: Repr of a Maat function object.
 *
 * - @arity: The number of arguments the function takes.
 * - @code: Its bytecode.
 * - @cons: The function's constant values.
 * - @ns: Access index to namespace of the function in @NSBuf.
 */
#define O_VFN  vary(O_FN, 0)

#define fn2v(f, v)  gco2val(f, v)
#define v2fn_rw(v)  gco2fn((v).gc_obj)
#define v2fn(v)     (ma_assert(is_fn(v)), gco2fn((v).gc_obj))

#define is_fn(v)  check_rtype(v, O_VFN)
#define as_fn(v)  (ma_assert(is_fn(v)), cast(Fn *, as_gcobj(v)))

typedef struct Fn {
   Header;
   Ubyte arity;
   size_t ns;
   CodeBuf code;
   ValueBuf cons;
} Fn;

/* @@@Repr of upvalues. */

/*
 * @@Upval: An upvalue isn't a first class value, it simply
 * points to a value (of a lexical) in the stack of a callframe
 * so that this function can still keep a reference to it when
 * the executing function associated to that callframe finishes
 * its execution.
 *
 * - @ncl: Boolean value, is '0' if only one closure closes over
 *   it; '1' otherwise.
 * - @p: Pointer to the value. When an upvalue is opened, it
 *   points to a living value in a vm register but when closed,
 *   it points to @state.
 * - @state: A pointer to the @next open upvalue when this one
 *   is still opened otherwise it'll contain the vm register
 *   value @p points to.
 */
#define O_VUPVAL  vary(O_UPVAL, 0)

#define Upvalfields  Ubyte ncl;           \
                     Value *p;             \
                     union {                \
                        struct Upval *next;  \
                        Value val;            \
                     } state

typedef struct Upval {
   Header;
   Upvalfields;
} Upval;

/* @@SUpval: Repr of shared upvalues */
#define O_VSUPVAL  vary(O_UPVAL, 1)

/* Upvalues aren't first class values! here are utils for sync ops. */
#define is_vsupval(o)  check_rtype(o, O_VSUPVAL)
#define as_vsupval(o)  cast(SUpval *, o)

typedef struct SUpval {
   Header;
   Upvalfields;
   /* TODO: librs fields */
} SUpval;

/*
 * @@Closure: A closure is a variant of a function which keep
 * tracks of its upvalues, functions are never used directly.
 *
 * - @fn: A pointer to the closure's function.
 * - @state: Value buffer to keep values of static lexicals.
 * - @upvals: The List of upvalues the function has.
 * - @nuv: The number of upvalues.
 */
#define O_VCLOSURE  vary(O_FN, 1)

#define clo2v(c, v)  gco2val(c, v)
#define v2clo_rw(v)  gco2clo((v).gc_obj)
#define v2clo(v)     (ma_assert(is_clo(v)), gco2clo((v).gc_obj))

#define is_clo(v)  check_rtype(v, ctb(O_VCLOSURE))
#define as_clo(v)  (ma_assert(is_clo(v)), cast(Closure *, as_gcobj(v)))

#define Closurefields  UByte nuv;        \
                       Fn *fn;            \
                       Object *next_sobj;  \
                       ValueBuf state;      \
                       Upval *upvals[flex]


typedef struct Closure {
   Header;
   Closurefields;
} Closure;

/*
 *
 */
#define O_VRCLOSURE vary(O_FN, 2)

#define rclo2v(c, v)  gco2val(c, v)
#define v2rclo(v)     (ma_assert(is_rclo(v)), gco2rclo((v).gc_obj))
#define v2rclo_rw(v)  gco2rclo((v).gc_obj)

#define is_rclo(v)  check_rtype(v, ctb(O_VRCLOSURE))
#define as_rclo(v)  (ma_assert(is_rclo(v)), cast(RClosure *, as_gcobj(v)))

typedef struct RClosure {
   Header;
   Closurefields;
   UByte offset;
} Closure;

/*
 * @@@Repr of various classes, every collectable object has a
 * class, even the class object itself since a class is also
 * collectable. Class header of objects that aren't first class
 * values are probably not useful, e.g Upvalues.
 *
 * @@Attr: Repr of a class attribute.
 *
 * - @name: The field's name.
 * - @val: The field's value.
 */
#define fieldval(a)  ((a)->val)

typedef struct Field {
   Value val;
   Str *name;
} Field;

/*
 * @@Class: Repr of a Maat class with the variant type 'Role'.
 *
 * - @name: The class' name.
 * - @attrs: A buffer of attributes, holds attributes' default
 *   values, includes all inherited and roles attributes.
 *   Conflicts are resolved at class creation. This field is
 *   copied into the instance object at instanciation.
 * - @asize: Size of the attribute buffer.
 *
 * - @c3: Buffer of classes, the class' c3 linearization.
 * - @offsets: The attribute buffer offset of each class in the
 *   c3 list. A method found in a class of the c3 list at an
 *   index `i' uses the offset at index `i' in @offsets to
 *   properly access its attributes from the instance attribute
 *   buffer.
 * - @csize: Size of the c3 list and the offset list since they
 *   both must have the same size.
 *
 * - @roles: Keeps the list of roles the class ':does' regardless
 *   of which class variant the class is.
 * - @rsize: Size of the role list.
 *
 * - @sups: Keeps the list of directly inherited superclasses.
 *   @sups exists mainly for the runtime build of the @c3 list
 *   as it's @c3 that handles super calls.
 * - @ssize: Size of the super list.
 * - @meths: A map to store the class' methods (not the inherited
 *   ones). A value to a key here is a pointer to a Closure.
 *
 * - @mro_cache:
 */
#define O_VCLASS  vary(O_CLASS, 0)
#define O_VROLE   vary(O_CLASS, 1)

#define cls2v(c, v)   gco2val(c, v)
#define role2v(c, v)  gco2val(c, v)
#define v2cls(v)      (ma_assert(iss_cls(v)), gco2cls((v).gc_obj))
#define v2role(v)     (ma_assert(is_role(v)), gco2cls((v).gc_obj))
#define v2cls_rw(v)   gco2cls((v).gc_obj)
#define v2role_rw(v)  v2cls_rw(v)

#define is_cls(v)   check_type(v, O_CLASS)
#define iss_cls(v)  check_rtype(v, ctb(O_VCLASS))
#define is_role(v)  check_rtype(v, ctb(O_VROLE))

#define as_cls(v)   (ma_assert(is_cls(v)), cast(Class *, as_gcobj(v)))
#define as_role(v)  as_cls(v)

typedef struct Class {
   Header;
   Str *name;
   Field *fbuf;
   Map *meths;
   Map *mro_cache;
   struct Class *roles;
   struct Class *sups;
   struct Class *c3;
   Object *next_sobj;
   UByte rsize;
   UByte ssize;
   UByte csize;
   UByte asize;
} Class;

/*
 * @@FClass: Repr of a foreign class, an FClass is a class whose
 * implemention is done in foreign languages like C and C++.
 *
 * - @name: Foreign class name.
 * - @size: Length of the allocated bytes referenced by @fdata.
 * - @meths: Foreign class methods.
 */
#define O_VFCLASS  vary(O_CLASS, 2)

#define fcls2v(f, v)  gco2val(f, v)
#define v2fcls_rw(v)  gco2fcls((v).gc_obj)
#define v2fcls(v)     (ma_assert(is_fcls(v)), gco2fcls((v).gc_obj))

#define is_fcls(v)  check_rtype(v, ctb(O_VFCLASS))
#define as_fcls(v)  (ma_assert(is_fcls(v)), cast(FClass *, as_gcobj(v)))

typedef struct FClass {
   Header;
   Str *name;
   Map *meths;
   size_t len;
} FClass;

/*
 * @@@Repr of classes instance, both for Maat and other foreign
 * languages like C/C++.
 */
#define is_ins(v)  check_type(v, O_INS)

/*
 * @@MIns: Instance of a Maat class. If this object somehow ends
 * up shared then attributes themselves are marked shared since
 * they are the only mutable part of this object.
 *
 * - @attrs: A copy of the instance's class @attrs.
 * - @sup_level:
 *
 * "self.SUPER::<method_name>(...)" Where SUPER is a pseudo
 * class that resolves to the next class in the instance's class
 * c3 list.
 */
#define O_VMINS  vary(O_INS, 0)

#define mins2v(i, v)  gco2val(i, v)
#define v2mins(v)     (ma_assert(is_mins(v)), gco2mins((v).gc_obj))

#define is_mins(v)  check_rtype(v, ctb(O_VMINS))
#define as_mins(v)  (ma_assert(is_mins(v)), cast(MIns *, as_gcobj(v)))

typedef struct MIns {
   Header;
   Object *next_sobj;
   AttrBuf *attrs;
   Map *sup_level;
} MIns;

/*
 * @@FIns: Instance of an FClass, this object doesn't have a
 * thread-safe lock-free variant, it's of the responsibility of
 * the foreign code to provide synchronization in case @fvalue
 * has to be concurrently accessed via its foreign methods.
 *
 * - @fvalue: The foreign class' value.
 */
#define O_VFINS  vary(O_INS, 1)

#define fins2v(i, v)  gco2val(i, v)
#define v2fins(v)     (ma_assert(is_fins(v)), gco2fins((v).gc_obj))
#define v2fins_rw(v)  gco2fins((v).gc_obj)

#define is_fins(v)  check_rtype(v, ctb(O_VFINS))
#define as_fins(v)  (ma_assert(is_fins(v)), cast(FIns *, as_gcobj(v)))

typedef struct FIns {
   Header;
   Object *next_sobj;
   Value fvalue;
} FIns;

/*
 * @@Namespace: Repr of a namespace e.g 'FOO::BAR'. A namespace
 * can either be represented as a package, role or (c)class.
 * 'FOO::BAR::x()' is a call to the function 'x' in 'FOO::BAR'
 * if ever there is.
 *
 * - @ours: Stores the namespace' global symbols, 'x' is a
 *   global symbol in package namespace 'FOO::BAR' and it can
 *   be fully qualified as in the above call.
 *
 * @ours of the package 'main::' takes care of the following
 * type I & II special variables:
 *
 *   - ENV, ARGC, ARGV, INC, PATH, SIG, DATA
 *   - #v, #o, #,, #/, #\, #|, #", ##, #(, #), #<, #>, #f, #0
 *
 * Accessing any of these variables from the main package most
 * not necessarily be done using a fully qualified form unless
 * a variable of the same name declared in a scope takes
 * precedence when the compiler resolves it.
 *
 * - @exports: Names of to-be-exported globals when this
 *   namespace is used by another.
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
 * - @val: The namespace value, either a package/class/role.
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

/* @@@Definition of below objects are in their respective '.h' files. */

/* A thread-safe channel and scheduler ring buffer queue. */
#define O_VCHAN    vary(O_RBQ, 1)
#define O_VSCHEDQ  vary(O_RBQ, 2)

#define is_chan(v)    check_type(v, O_VCHAN)
#define is_schedq(v)  check_type(v, O_VSCHEDQ)

/* @@State object, see 'ma_state.h'. */
#define O_VSTATE  vary(O_STATE, 0)
#define O_VCO     vary(O_STATE, 1)

#define is_state(v)   check_type(v, O_STATE)
#define iss_state(v)  check_rtype(v, ctb(O_VSTATE))
#define is_co(c)      check_rtype(v, ctb(O_VCO))

#define as_state(v)  (ma_assert(is_state(v)), cast(State *, as_gcobj(v)))
#define as_co(v)     as_state(v)

/* @@Work object, see 'ma_work.h'. */
#define O_VWORK  vary(O_WORK, 0)

#define is_work(v)  check_rtype(v, ctb(O_VWORK))
#define as_work(v)  (ma_assert(is_state(v)), cast(Work *, as_gcobj(v)))

/* @@Maatine object, see 'ma_maa.h'. */
#define O_VMAA  vary(O_MAA, 0)

#define is_maa(v)  check_rtype(v, ctb(O_VMAA))
#define as_maa(v)  (ma_assert(is_maa(v)), cast(Maa *, as_gcobj(v)))

/* @@@Union of all collectable objects used for conversion. */

#define ounion(o)  cast(OUnion *, o)

/*
 * ISO C99 says that a pointer to a union object, suitably
 * converted, points to each of its members, and vice versa.
 */
#define gco2mins(o)  (ma_assert(check_type(o, O_VMINS)), &(ounion(o)->mins))
#define gco2fins(o)  (ma_assert(check_type(o, O_VFINS)), &(ounion(o)->fins))
#define gco2str(o)   (ma_assert(check_type(o, O_STR) || check_type(v, O_U8STR)), &(ounion(o)->str))
#define gco2arr(o)   (ma_assert(check_type(o, O_ARRAY)), &(ounion(o)->ar))
#define gco2map(o)   (ma_assert(check_type(o, O_MAP)), &(ounion(o)->map))
#define gco2rg(o)    (ma_assert(check_type(o, O_RANGE)), &(ounion(o)->rng))
#define gco2fun(o)   (ma_assert(check_rtype(o, O_VFUN)), &(ounion(o)->fun))
#define gco2clo(o)   (ma_assert(check_rtype(o, O_VCLOSURE)), &(ounion(o)->clo))
#define gco2rclo(o)  (ma_assert(check_rtype(o, O_VRCLOSURE)), &(ounion(o)->rclo))
#define gco2meth(o)  (ma_assert(check_rtype(o, O_VMETH)), &(ounion(o)->meth))
#define gco2uv(o)    (ma_assert(check_type(o, O_UPVAL)), &(ounion(o)->uv))
#define gco2cls(o)   (ma_assert(check_rtype(o, O_VCLASS) || check_rtype(o, O_VROLE)), &(ounion(o)->cls))
#define gco2fcls(o)  (ma_assert(check_rtype(o, O_VFCLASS)), &(ounion(o)->fcls))
#define gco2stt(o)   (ma_assert(check_type(o, O_STATE)), &(ounion(o)->stt))
#define gco2ma(o)    (ma_assert(check_type(o, O_MA)), &(ounion(o)->ma))
#define gco2wk(o)    (ma_assert(check_rtype(o, O_VWORK)), &(ounion(o)->wk))
#define gco2rbq(o)   (ma_assert(check_rtype(o, O_VRBQ)), &(ounion(o)->rbq))
#define gco2ns(o)    (ma_assert(check_rtype(o, O_VNS)), &(ounion(o)->ns))

/* The other way around. */
#define x2gco(x)  (ma_assert(is_ctb(x)), &(ounion(x)->gc_obj))

/* Union of all collectable objects */
union OUnion {
   Object gc_obj;
   Str str;
   Array arr;
   Map map;
   Range rng;
   Fn fn;
   Closure clo;
   RClosure rclo;
   Upval uv;
   Class cls;
   FClass fcls;
   MIns mins;
   FIns fins;
   Rbq rbq;
   State stt;
   Ma ma;
   Work wk;
   Namespace ns;
} OUnion;

/* It's on its own as it's needed at 'ma_str.c' and elsewhere. */
#define sunion_of(s)  cast(Sunion *, s)
#define a2u8(s)       (ma_assert(check_type(s, O_STR)), &(sunion_of(s)->as))
#define u82a(s)       (ma_assert(check_type(s, O_U8STR)), &(sunion_of(s)->u8s))

union Sunion {
   Str as;
   U8Str u8s;
} Sunion;

#endif

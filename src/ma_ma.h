#ifndef ma_ma_h
#define ma_ma_h

#include "ma_val.h"
#include "ma_state.h"

/* $$Map of short strings $map, it has size $size, capacity $cap. */
typedef struct SMap {
   Int size;
   Int cap;
   Str **map;
} SMap;

/*
 * $$Data common to all Maatines, mutex must be used for some
 * of these variables.
 */
typedef struct GMa {
   /* $seed: Random seed for Maps. */
   UInt seed;

   /*
    * $scache: Caching strings to reuse later, as it is shared
    * across maatines.
    */
   MStr scache[M_SCACHE][N_SCACHE];
   /* TODO: librs fields */

   /*
    * $smap: Map of short strings, we reuse short strings and
    * never avoid duplicates, this is not to be confused with
    * $scache.
    */
   SMap smap;
   /* TODO: librs fields */

   /*
    * $ns_names: Map of namespaces, each ns name corresponds to
    * an index of $ns_buf. It is used for checks at compile time
    * so that each namespace has a unique index.
    * $ns_buf: A buffer of namespaces, since ns resolution is done
    * at compile time, each namespace is accessible here via a
    * unique index and hence zero collision at runtime.
    */
   Map *ns_names;
   NamespaceBuf nsbuf;

   /* Linked-list of shared objects. */
   Object *lso;

} GMa;

/*
 * $$The Maatine object, it represents a VM-level thread with its
 * execution scheduled by maat's runtime scheduler.
 */
typedef struct Ma {
   Header;

   /* $status: Status of this Maatine. */
   UByte status;

   /* $id: The id of this Maatine. */
   Uint id;

   /*
    * $state: Initial state of this Maatine as we know Maatines
    * can switch to that of a coroutine or Gfun.
    */
   State *state;

   /* $co: Pointer to the first coroutine of this Maatine. */
   State *co;

   /* $mvm: An instance of a VM this Maatine is attached to. */
   struct MVM *mvm;

   Mem debt;
   UMem mem;
   UMem estimate;

   /*
    * $gc_fgcc:
    *   Controls whether the maatine has done its first GC cycle
    *   for the first GC round. 1 if yes; 0 otherwise.
    */
   Ubyte gc_fgcc;
   UByte gc_phase;
   UByte gc_state;
   UByte gc_type;
   UByte gc_pause;
   UByte gc_emerg;
   UByte gc_ssize;
   UByte gc_smul;
   UByte gc_minor;
   UByte gc_major;

   Int ntmps;
   Object *tmproots;
   Object *mobj;
   Object *nursery2;
   Object *old;
   Object *old2;
   Object *gray;
   Object *gray2;
   Object *fin_nur2;
   Object *fin_old;
   Object *fin_old2;

   /* $mma: Points to the main Maatine. */
   Ma *mma;

   /* $states_uvs: list of states with open upvals */
   State *states_uvs

   /* $gma: Points to the data common to all Maatines. */
   GMa *gma;
} Ma;

#endif

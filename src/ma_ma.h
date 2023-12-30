#ifndef ma_ma_h
#define ma_ma_h

#include "ma_val.h"
#include "ma_state.h"

/*
 * $$Data common to all Maatines, mutex must be used for some
 * of these variables.
 */
typedef struct GMa {
   /* $seed: Random seed for Maps. */
   UInt seed;

   /*
    * $use_mtx: Controls the use of mutexs as some aren't useful in
    * *single* threaded (both at the VM and OS level) running Maat
    * programs.
    */
   Ubyte use_mtx;

   Mutex m_maat;

   /*
    * $scache: Caching strings to reuse later, as it is shared
    * across maatines, it has a mutex $m_scache.
    */
   Mutex m_scache;
   Str scache[M_SCACHE][N_SCACHE];

   /*
    * $smap: Map of short strings, we reuse short strings and
    * never avoid duplicates, this is not to be confused with
    * $scache. This too has a mutex $m_smap.
    */
   Mutex m_smap;
   SMap smap;

   /*
    * $ns_names: Map of namespaces, each ns name corresponding to
    * an index of $ns_buf. It is used for checks at compile time
    * so that each namespace has a unique index.
    * $ns_buf: A buffer of namespaces, since ns resolution is done
    * at compile time, each namespace is accessible here via a
    * unique index and hence zero collision at runtime.
    * $m_ns: Occasional mutex to be used at runtime when maat is
    * concurrently compiling multiple sources.
    */
   Mutex m_ns;
   Map *ns_names;
   NamespaceBuf ns_buf;

  /*
   * $sobj: List of all objects in the SOS (Shared Object Space).
   * $m_sobj: 
   */
   Object *sobj;
   Mutex m_sobj;
} GMa;

/*
 * $$The Maatine object, it represents a VM-level thread with
 * its execution scheduled by maat's runtime scheduler.
 */
typedef struct Ma {
   Header;

   /* $status: Status of this Maatine. */
   UByte status;

   /* $id: The id of this Maatine. */
   Uint id;

   /*
    * $cstt: The state of the currently running program.
    * $state: Initial state of this Maatine as we know Maatines
    * can switch to coroutine states.
    */
   State *cstt;
   State *state;

   /* $co: Pointer to the first coroutine of this Maatine. */
   State *co;

   /* $mvm: An instance of a Maat VM this Maatine is attached to. */
   struct MVM *mvm;

   Mem debt;
   UMem mem;
   UMem estimate;

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

   /* $mma: Points to the main Maatine. `NULL' if this is the main. */
   Ma *mma;

   /* $gma: Points to the data common to all Maatines. */
   GMa *gma;
} Ma;

#endif

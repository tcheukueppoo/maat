#ifndef ma_state_h
#define ma_state_h

#include "ma_conf.h"
#include "ma_val.h"

/*
 * ##Representation of a Callframe.
 *
 * #ip: Instruction pointer which points to the next to be
 * executed instruction.
 * #closure: The closure associated to that callframe.
 * #stack: The start of the frame, contains the function's first
 * arg or local.
 * #temps: For temporarization(Maat's "temp" keyword) of package
 * variables. It is a map with keys corresponding to scope level
 * counts since multiple scopes can temporarize a same package
 * variable, we are using a map because it uses its array part
 * when necessary then optimizes into a real map when scope
 * indexes are far apart from each other. $temps lets some
 * methods declare the following lexically scoped special
 * variables:
 *
 *   $<digit>($1, $2, $3, etc), $., $`, $&, and $'
 *
 * It is internally implemented c codes that have to
 * appropiately set these variables in $temps e.g ".match"
 * Regex method.
 */

typedef struct CallFrame {
   uint8_t *ip;
   Map *temps;
   Value *start;
   Closure *closure;
} CallFrame;

/*
 * ##A Maat state which can either be a blocking vm-level thread
 * called a coroutine, a generator function or simple the state
 * of a maatine.
 *
 * #cw: #cw is #wk if this State is that of maatine, #wk as an
 * optional field is a pointer to the Work object that
 * sollicated a maatine of this State to run its code(#wk_code).
 * If this State is that of a coroutine then #cw is a pointer
 * to its caller coroutine or maatine state #ca.
 * Coroutines as well as generator functions are blocking kind
 * of threads running by changing the context(state) of the
 * maatine it belongs to. That being said, it is quite
 * impossible for a coroutine to yield control back to another
 * coroutine of a different maatine, but that is not a problem
 * as we are implementing asymmetric coroutines.
 * #open_uv: Link list of open upvalues of this state.
 */
typedef struct State {
   Object obj;
   UByte state;
   CallFrame *callstack;
   size_t cs_cap;
   size_t cs_size;
   Value *top;
   Value *stack;
   Upval *open_uv;
   struct Ma *ma;
   union {
      struct State *ca;
      struct Work *wk;
   } cw;
} State;

/*
 * ##Data common to all Maatines, mutex must be used for some
 * of these variables.
 *
 * #stab: String table, has mutex #m_stab.
 * #scache: Caching short strings, has mutex #m_scache.
 * #seed: Random seed for Maps.
 * #ns: Namespaces thingy.
 *   #names: Map of namespaces to check and keep track of
 *   unique indexes corresponding to each namespace.
 *   #buf: A buffer of namespaces since resolution is done at
 *   compile time, each ns is accessible in #buf via a unique
 *   index, this frees use from collisions at execution time.
 *   #m: Occasional mutex to be allocated for used when
 *   concurrently compiling sources at execution time using the
 *   `do' statement.
 */
typedef struct GMa {
   UInt seed;
   Str scache[M_SCACHE][N_SCACHE];
   Mutex m_scache;
   STab stab;
   Mutex m_stab;
   struct {
      Map *names;
      NamespaceBuf buf;
      Mutex *m;
   } ns;
   Object *ns_gco;
} GMa;

/*
 * ##A maatine is a VM-level thread with its execution
 * scheduled by maat's runtime scheduler.
 *
 * #instance: An instance of a Maat VM.
 * #co: List of coroutines of this Maatine.
 * #mm: Pointer to the main Maatine, #mm is NULL if this Maatine
 * is the main one.
 */
typedef struct Ma {
   Object obj;
   UByte state;
   State *state;
   State *co;
   struct MVM *instance;
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
   Object *gco;
   Object *nursery2;
   Object *old;
   Object *old2;
   Object *gray;
   Object *gray2;
   Object *fin_nur2;
   Object *fin_old;
   Object *fin_old2;

   Ma *m;
   GMa *G;
} Ma;

typedef struct Work {
   Object obj;
   UByte state;
   size_t then_size;
   Closure *wk_code;
   Closure *then;
   Closure *catch[1];
} Work;

#endif

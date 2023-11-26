#ifndef ma_state_h
#define ma_state_h

#include "ma_conf.h"
#include "ma_val.h"

/*
 * ##Representation of a Callframe.
 *
 * #ip: instruction pointer which points to the next-to-be-executed instruction.
 * #closure: The closure associated to that callframe.
 * #stack: The start of the frame, contains the function's first arg or local.
 *
 * #temps: For temporarization(Maat's "temp" keyword) of package variables. It
 * is a map with keys corresponding to scope level counts since multiple scopes
 * can temporarize a same package variable, we are using a map because it uses
 * its array part when necessary then optimizes into a real map when scope
 * indexes are sparsed. $temps lets some methods declare the following lexically
 * scoped special variables:
 *
 *   $<digit>($1, $2, $3, etc), $., $`, $&, and $'
 *
 * It is internally implemented c codes that have to appropiately set these
 * variables in $temps e.g ".gmatch" Regex method.
 */

typedef struct CallFrame {
   uint8_t *ip;
   Map *temps;
   Value *start_stack;
   Closure *closure;
} CallFrame;

/*
 * ##An Mstate simple represent a thread of execution,
 * Maatines(VM-level threads), Coroutines, and generator functions all inherit
 * an MState.
 *
 * #
 */
typedef struct MState {
   Header obj;
   CallFrame *callstack;
   size_t cs_cap;
   size_t cs_size;
   Upval *upvals;
   Value *stack;
   Value *stack_top;
} MState;

/*
 * ##VM-level thread, kind-of a context executed by an OS-level thread
 * #state: Current state of the Maatine
 * #allobj: List of all allocated object in the current Maatine
 * #is_work: Is maatine running as a Work?
 * #num_temps: Number of temporary roots.
 * #
 */
typedef struct Ma {
   Object obj;
   MState *mstate;
   UByte state;
   struct Work *work;
   struct MVM *instance;
   Object *gray;
   Object *gcount;
   Object *allobj;
   Object *temproots;
   Int num_temps;
} Ma;

/*
 * ##A coroutine, it is a blocking VM-level thread that runs on top of a Maatine
 */
typedef struct Co {
   Object obj;
   UByte state;
   MState *mstate;
   Ma *ma;
   struct Co *caller;
} Co;

typedef struct Work {
   Object obj;
   UByte state;
   Closure *then;
   Closure *catch;
} Work;

#endif

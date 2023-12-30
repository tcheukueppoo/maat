/*
 * $$$ 
 * License: AGL, see LICENSE file for details.
 */
#ifndef ma_state_h
#define ma_state_h

#include "ma_conf.h"
#include "ma_val.h"

/* * $$Struct of a Callframe.
 *
 * $ip: Instruction pointer which points to the next-to-be
 * executed vm instruction.
 * $closure: The closure associated to that callframe.
 * $stack: The start of the frame, points the function's first
 * argument or local.
 * $temps: For temporarization(the "temp" declarator) of package
 * variables. It is a map with keys corresponding to scope level
 * counts since multiple scopes can temporarize same package
 * variables. We are using a map because it uses its array part
 * when necessary then optimizes into a real map when scope
 * indexes are far apart from each other. $temps lets some
 * in-house methods define the following lexically scoped
 * special variables:
 *
 *   $<digit>($1, $2, $3, etc), $., $`, $&, and $'
 *
 * It is internally implemented C codes that have to
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
 * $$A Maat state which can either be a blocking vm-level thread
 * called a coroutine, a generator function or simple the state
 * of a maatine.
 */
typedef struct State {
   Header;

   /* $state: State of this state/coroutine. */
   UByte state;

   /*
    * $callstack: Stack of callframes.
    * $cs_cap: Capacity of $callstack.
    * $cs_size: Number of used callframes.
    */
   CallFrame *callstack;
   size_t cs_cap;
   size_t cs_size;
   Value *top;
   Value *stack;

   /* $open_uv: Linked-list of open upvals of this state. */
   Upval *open_uv;

   /* $ma: The Maatine owning this state/coroutine. */
   struct Ma *ma;

   /*
    * $cw: $cw is $wk if this State is that of maatine, $wk as an
    * optional field is a pointer to the Work object that solicited
    * a maatine of this State to run its code($wk_code).
    * If this State is that of a coroutine then $cw is a pointer to
    * its caller coroutine or maatine state $ca.
    * Coroutines as well as generator functions are blocking kinds
    * of threads running by changing the context(state) of the
    * maatine it belongs to. That being said, it is quite
    * impossible for a coroutine to yield control back to another
    * coroutine of a different maatine, but that is not a problem
    * as we are implementing asymmetric coroutines.
    */
   union {
      struct State *ca;
      struct Work *wk;
   } cw;
} State;

/*
 * $$Map of short strings whether ascii or utf-8 encoded short
 * strings.
 *
 * $size: Size of our hash map.
 * $cap: Its Capacity.
 * $map: And then the map itself.
 */
typedef struct SMap {
   Int size;
   Int cap;
   Str **map;
} SMap;

#endif

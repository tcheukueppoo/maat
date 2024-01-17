/*
 * $$$ 
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_state_h
#define ma_state_h

#include "ma_conf.h"
#include "ma_val.h"

/*
 * $$SV is a queue element of $svq queue for special variables.
 *
 * $id: Id of the scope these special variables belongs to.
 * $sv: Special variables of this scope, keys could be:
 *
 *  $<digit>($1, $2, $3, etc), $., $`, $&, or $'
 *
 * It is internally implemented C codes that have to set
 * these variables in $temps. e.g the ".match" RE method.
 */
typedef struct SV {
   Ubyte id;
   Map *sv;
} SV;

/* $$Struct of a Callframe */
typedef struct CallFrame {

   /*
    * $pc: Program counter which points to the next-to-be executed
    * vm instruction.
    */
   uint8_t *pc;

   /*
    * $stack: The start of the frame, points the function's first
    * argument or local.
    */
   Value *base;

   /* The id of the current scope in this callframe. */
   Ubyte id;

   /*
    * $temps: For temporization of package variables, i.e a change on
    * the contents of package variables made local to the scope from
    * where the change was made. See doc of 'temp' var specifier.
    *
    * Resolution is done at compile time just like with lexicals.
    */
   size_t ntemps;
   ValueBuf *temps;

   /*
    * $svq: Queue of $$SV elements.
    * $qsize: Queue size.
    * $front: Index of the front element to-be dequeued when we go
    * out-of-scope the SV element belongs to. The front always get
    * the variable we want to runtime resolve, so it's pretty fast.
    */
   SV *sv_queue;
   Ubyte front;
   Ubyte qsize;


   /* $closure: The closure associated to this callframe. */
   Closure *closure;

} CallFrame;

/*
 * $$A state can either be a blocking vm-level thread i.e coroutine
 * or generator function or simply the state of a Maatine.
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

   /*
    * $stack:
    * $top:
    */
   Value *stack;
   Value *top;

   /* $open_uv: Linked-list of open upvals of this state. */
   Upval *open_uv;

   /* $ma: The Maatine owning this state/coroutine/Gfun. */
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

#endif

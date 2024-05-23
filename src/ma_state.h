/*
 * @@@ 
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_state_h
#define ma_state_h

#include "ma_conf.h"
#include "ma_val.h"

/*
 * @@SV: SV is a queue element of @svq queue for special variables.
 *
 * - @id: Id of the scope these special variables belongs to.
 * - @sv: Special variables of this scope, keys could be:
 *
 *  @<digit>(@1, @2, @3, etc), @., @`, @&, or @'
 *
 * It is in-house C codes that have to set these special variables
 * in below @tmps. e.g the ".match" regex method.
 */
typedef struct SV {
   Map *sv;
   UByte sc_id;
} SV;

/* @@CallFrame: Just a callframe. */
typedef struct CallFrame {

   /*
    * @pc: Program counter which points to the next-to-be executed
    * vm instruction.
    */
   uint8_t *pc;

   /*
    * @base: The start of the frame, points the function's first
    * argument or local or the method's reciever.
    */
   Value *base;

   /*
    * @tmps: For temporization of package variables, i.e a change
    * on the contents of package variables made local to the scope
    * from where the change was made. See doc of 'temp' variable
    * specifier. Resolution is done at compile time just like with
    * lexicals, this is because a fully qualified name is treated
    * as a variable name which shadows the corresponding package
    * symbol.
    */
   ValueBuf tmps;

   /* @closure: The closure associated to the callframe. */
   Closure *clo;

   /*
    * - @svq: Queue of @@SV elements.
    * - @qsize: Queue size.
    * - @front: Index of the front element to-be dequeued when we
    *   go out-of-scope the SV element belongs to. The front always
    *   get the variable we want to runtime resolve, so it's fast.
    */
   SV *svq;
   UByte front;
   UByte qsize;

   /*
    * @id: The id of the current scope in this callframe. Sadly,
    * we will have to check at each scope pop if the front to
    * dequeue its @@SV element.
    */
   UByte sc_id;

   /*
    * - @a_offset: Class' role or inherited methods need specific
    *   offsets to properly access their attributes in the instance
    *   attribute buffer.
    * - @cc3: 
    */
   UByte f_offset;
   UByte *cc3;

} CallFrame;

/*
 * @@State: A state can either be a blocking vm-level thread i.e
 * a coroutine or a generator function or simply the initial state
 * of a Maatine.
 */
typedef struct State {
   Header;

   /* @state: State of this state/coroutine. */
   UByte state;

   /*
    * - @cstk: A stack of callframes.
    * - @cs_cap: Capacity of the @callstack.
    * - @cs_size: Number of used frames.
    */
   CallFrame *cstk;
   size_t cs_cap;
   size_t cs_size;

   /*
    * @stack:
    * @top:
    */
   Value *stack;
   Value *top;

   /* @ouv: Linked-list of open upvals of this state. */
   Upval *ouv;

   /* @ma: The Maatine owning this State/Coroutine/Gfun. */
   struct Ma *ma;

   /*
    * @cw: @cw is @wk if this State is that of maatine, @wk as an
    * optional field is a pointer to the Work object that solicited
    * a maatine of this State to run its code(@wk_code).
    * If this State is that of a coroutine then @cw is a pointer to
    * its caller coroutine or maatine state @ca.
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

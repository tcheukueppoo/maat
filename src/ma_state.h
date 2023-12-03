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
 * ##A coroutine
 * #callstack: Coroutine's callstack
 * #cs_cap: Coroutine's callstack capacity.
 * #cs_size: Coroutine's callstack size.
 * #ma: The maatine to who this coroutine belongs.
 * #stack: Coroutine's stack.
 * #stop: The stack's top.
 * #upvals: Coroutine's list of upvals.
 * #cc: The Caller coroutine.
 */
typedef struct Co {
   Object obj;
   UByte state;
   CallFrame *callstack;
   size_t cs_cap;
   size_t cs_size;
   Upval *upvals;
   Value *stack;
   Value *top;
   Ma *ma;
   struct Co *cc;
} Co;

/*
 * ##A maatine is a VM-level thread, 
 * #state: Current state of the Maatine
 * #ntemps: Number of temporary roots.
 * #co: The main Coroutine bounded to this Maatine(VM thread), it does not yield
 * control back to any other coroutine and
 * #gco: list of all collectable object, it is called the nursery in gen mode.
 * #mm: Pointer to the main maatine
 */
typedef struct Ma {
   Object obj;
   Co co[1];
   UByte state;
   struct Work *work;
   struct MVM *instance;
   Int ntemps;
   Object *temproots;
   Object *gco;
   Object *nursery2;
   Object *old;
   Object *old2;
   Object *gray;
   Object *gray2;
   Object *fin_nur2;
   Object *fin_old;
   Object *fin_old2;

   Ma *mm;
} Ma;

typedef struct Work {
   Object obj;
   UByte state;
   Closure *thens;
   Closure *catch;
} Work;

#endif

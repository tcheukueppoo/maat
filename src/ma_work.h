#ifndef work_h
#define work_h

#include "ma_val.h"
#include "ma_state.h"

/* ##The Work object.
 *
 * #state: State of the work.
 * #then_size: Size of the list of .then({}) of this Work.
 * #then: List of thens.
 * #catch: Exception handler if an exception is thrown;
 */
typedef struct Work {
   Header;
   UByte state;
   UByte then_size;
   Closure *wk_code;
   Closure *then;
   Closure *catch[1];
} Work;

#endif

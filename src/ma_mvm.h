#ifndef ma_mvm_h
#define ma_mvm_h

#include "ma_conf"
#include "ma_mstate"

/* 
 * The Maat VM, running maat program may have multiple instances
 * of this VM, each OS-thread is attributed an instance to run
 * maat code, the runtime Maat scheduler schedules the execution
 * of maatines over each Maat vm instance.
 */
typedef struct MVM {


   /* To sync traversal on shared objects */
   AO_t pass_smark;
} MVM;


#endif

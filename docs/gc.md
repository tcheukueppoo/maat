# Maat GC

Maat implements generational and incremental garbage collection.
This section focuses more on implementation details, check the below
links to get to know what generational and incremental garbage
collection are.

We are in a concurrent system where maatines(VM-level non-blocking threads)
are scheduled by the Maat runtime scheduler to run over operating system
threads (using libuv). A maatine is basically a lightweight thread that has
atleast one state with each state owning a stack to executing some code, a 
maatine can switch between any of its states, each maatine performs its
collection either incrementally or generationally and independently of the
other ones. That said, a lot must be considered to assure coherency and
synchronization.

Why have we choosen this design?

* Most objects are maatine-local.
* Only a few number of maatines (a.k.a threads) might need a GC run at the same time.
* Concurrent collection avoids us from stoping the world, each maatine performs it collection(in gen or inc mode) without annoying other maatines unless necessary.

With this design, major problems faced during collection are the following:

## Collection of open upvalues scattered across states of a Maatine

A Maatine has atleast one state which with the use of its own stack, simply has to
run a function which is literally just a closure that possibly has open upvalues
pointing to values living in the stack of another state (if any) of this same
maatine, the latter state may have not been marked and ends up collected but before
this has to happen, its open upvalues that have been marked during the marking
phase of the maatine GC run must be closed and painted black so that they cannot
longer be collected during the sweeping phase.

Solving this problem enforces us to do in each maatine's GC cycle the following:

After all gray objects have been processed, all reachable states are now marked
black, so go through this maatine's list of states with still opened upvalues
and for each unmarked state, identify all its non-white open upvalues to mark
the stack values they are pointing to to black thus making them reachable. Unless
they'll get reached again before the sweeping phase, unmarked states we found do
not need to be in the list-of-states-with-open-upvalues again as they are now
considered garbage.

## Objects shared accross maatines

The fact that each maatine runs its garbage collector independently of the other
ones poses a real issue on objects shared across Maat's maatines.

### Possibly ways of sharing

1. Upvalues

The state of a Maatine may have a closure with open upvalues pointing to values
in the stack of the state of another Maatine. We not only have to synchronized
access operations on these upvalues during runtime in case maat runs on in
multi-threaded environment but also have to make sure the maatine detaining those
stack values does not collect them as long as some other maatines use them via
closures or some other stuffs.

This problem is also encountered in closed upvalues as a maatine might want to
free unreachable closed upvalues reachable to other maatines.

2. Cached Strings

For memory efficiency, we have a global cache of strings and a global Map of short
strings for re-utilization. Concurrent access to these globals structures must be
synchronized, that said, it is sure a running program will get to a point where a
string will be created by the state of one maatine and later on get reused by the
state of another maatine and thus making that string shared.

3. Objects of namespaces' global symbols

The way we have designed our garbage collector makes objects of namespaces' global
symbols pretty hard to manage, In most programming languages, these globals form
part of the root set but it is a completely different story in Maat. A maatine can
perform a collection at anytime and thus colors can be concurrently set to a
collectable object leading to data race and lost of references.

Let's consider a case where a maatine in the mark phase of its collection marks
all objects of global symbols black and another maatine which is in the sweep
phase of its collection changes the colors of all reachable objects back to white
for its next gc cycle, at this point even objects marked black by the former
maatine which hasn't even entered its sweep phase are now white which raises
a big problem as they're now treated as garbage.

4. Arguments to Maatine calls

A newly started maatine simply invoke a function in a new state with arguments
(potentially collectable values) gotten from the maatine that started this one.
Collectable values passed to a function call are passed by reference and thus
these collectable values become shared.

Channels which are themselves shared objects serve as a means to thread-safely
share objects across maatines. A channels can be shared via global symbols, upvals
or function calls.

### Shared Object List (SOL)

To safely collect objects shared across maatines, maat introduces a linked-list of
shared objects kept in the global state of each maatine (`GMa` struct) and two
object colors which are the blue and red.

An object that is shared accross maatines can only

After analysis, we have come up with the following properties

* A shared object can only is considered garbage after a GC tour.
* 


# Gen algorithm

Two parameters determine whether the next collection to be done is a
minor or major one. Let X and Y be paramaters which represent the ceil
of the percentage increase of the estimated non-garbage memory in use
for the minor and major collection respectively.

Why not setting estimate in minor collections

# inc

Paint to special white ready for the next GC cycle to avoid multiple
write barriers.

# issue with upvals and coroutines



- Impossible to know the color of all closures pointing to an upvals
- A coroutine might point to a white newly created object(barrier?)


#### some useful terms

gc step size: How much to allocate before the next step (log2)
gc debt: The bytes allocated but not yet compensated by the garbage collector
gc estimate: it is an estimate of non-garbage memory in use, it is update each time garbage is freed from the program
gc mul: factor which determines how much work is done in a garbage collection cycle
gc pause size: it sets the garbage collection debt, it determines how long the program has to wait
before starting a new cycle

### TODO

- PROBLEMS
- SOS, MAATINE COLLABORATION
- GEN MODE, ALGORITHM
- INC MODE, ALGORITHM
- INC MODE <-> GEN MODE
- GC PARAMATERS
- FINALIZATION

- WEAK REF

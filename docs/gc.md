# Maat GC

Maat implements generational and incremental garbage collection.
This section focuses more on implementation details, check the below
links to get to know what generational and incremental garbage
collection are.

We are in a concurrent system where maatines(VM-level non-blocking
threads) are scheduled by the Maat runtime scheduler to run over
operating system threads (using libuv). A maatine is basically a
lightweight thread that has atleast one state owning a stack, each
maatine performs its collection either incrementally or
generationally and independent of the other. That said, a lot must
be considered to assure coherency and synchronization.

Major problems faced during collection are the following:

## Collection of open upvalues scattered across the states of a Maatine

A Maatine has atleast one state which with the use of its own stack,
simply has to run a function which is literally just a closure that
possibly has open upvalues pointing to values living in the stack of
another state of this same maatine, the latter state may have not
been marked and ends up collected but before this has to happen, its
open upvalues that have been marked during the marking phase must be
closed and painted black so that they cannot longer be collected
during the sweeping phase.

Solving this problem enforces us to do the following in each
maatine's GC cycle:

After all gray objects have been processed, all reachable states are
now marked black, so go through this maatine's list of states with
still opened upvalues and for each unmarked state, identify all
its non-white open upvalues to mark the stack values they are pointing
black, making them now reachable. Unless they'll get reached again
before the sweeping phase, these unmarked states need to be out of
the list of states with open upvalues as they are now considered as
garbage.

## Objects shared accross maatines.

The fact that each maatine runs its garbage collector independently
poses a real issue on objects shared across Maat's maatines.

Possibly shared objects are:

### Upvalues

The state of a Maatine may have a closure with upvalues pointing to
values in the stack of the state of another Maatine. Not only we have
to sync access to this upvalues, we also have to make sure a maatine
does not collect these values meanwhile some other maatines actually
used them via closures and stuffs like that.

### Cached Strings

For memory efficiency, we have a global cache of strings and map
of short strings with access to each of them synchronised. That
said, a string might have been created by the state from a maatine
and is later on reused by the state of another maatine.

### Objects assigned to namespaces' global symbols.

Content of global variables are obviously shared accross maatines as
all states within every maatine can modify globals.

To solve these problems, 











# Gen algorithm

Two parameters determine whether the next collection to be done is
a minor or major one. Let X and Y be paramaters which represent the
ceil of the percentage increase of the estimated non-garbage memory
in use for the minor and major collection respectively.

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

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

## Open upvalues scattered accross states own by different maatines.

A Maatine has atleast one state which with the use of its own stack,
simply has to run a function which is literally just a closure that
possibly has open upvalues pointing to values living in the stack of
the state of another maatine, this former maatine can at anytime
during runtime be destroyed or reused after it has finished what
it was build for but before this has to happen, its open upvalues
marked by living objects most be closed and painted black to avoid
being collected but then it can be too late to do so.

Though we said a maatine GC run is independent to GC runs from other
maatines, solving this problem enforces us to do the following in
each maatine's GC cycle:

1.

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

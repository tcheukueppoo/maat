# Maat GC

Maat implements generational and incremental garbage collection. This
section focuses more on implementation details, check the below links
to know what generational and incremental garbage collection are.

We are in a concurrent system where maatines
(VM-level non-blocking threads)
are scheduled by the Maat runtime scheduler to run over operating
system threads (using libuv). A maatine is basically a lightweight
thread that has at least one state with each state owning a stack to
executing some code, a maatine can switch between any of its
states, each maatine performs its collection either incrementally or
generationally and independently of the other ones. That said, a lot
must be considered to assure coherency and synchronization.

Why have we choosen this design?

* Most objects are maatine-local.
* Only a few number of maatines (a.k.a lightweight threads) might need
  a GC run at the same time.
* Concurrent collection avoids us from stoping the world, each maatine
  performs it collection(in gen or inc mode) without annoying other
  maatines unless necessary.

With this design, major problems faced during collection are the
following:

## Collection of open upvalues scattered across states of a Maatine

A Maatine has at least one state which with the use of its own
stack, simply has to run a function which is literally just a closure
that possibly has open upvalues pointing to values living in the stack
of another state (if any) of this same maatine, the latter state may
have not been marked and ends up collected but before this has to
happen, its open upvalues that have been marked during the marking
phase of the maatine GC run must be closed and painted black so that
they cannot longer be collected during the sweeping phase.

Solving this problem enforces us in each maatine's GC cycle to do
the following:

After all gray objects have been processed, all reachable states are
now marked black, so go through this maatine's list of states with
still opened upvalues and for each unmarked state, identify all its
non-white open upvalues to mark the stack values they are pointing to
to black thus making them reachable. Unless they'll get reached again
before the sweeping phase, unmarked states we found do not need to be
in the list-of-states-with-open-upvalues again as they are now
considered garbage.

## Objects shared accross maatines

The fact that each maatine runs its garbage collector independently of
the other ones poses a real issue on objects shared across Maat's
maatines.

### List of sharing points

1. Upvalues

The state of a Maatine may have a closure with open upvalues pointing
to values in the stack of the state of another Maatine. We not only have
to synchronized access operations on these upvalues during runtime in
case maat runs on in multi-threaded environment but also have to make
sure the maatine detaining those stack values does not collect them as
long as some other maatines use them via closures or by other means.
This problem is also encountered in closed upvalues as a maatine might
want to free unreachable closed upvalues reachable to other maatines.

2. Cached Strings

For memory efficiency, we have a global cache of strings and a global
Map of short strings for re-utilization. Concurrent access to these
globals structures must be synchronized, that said, it is sure a running
program will get to a point where a string will be created by the state
of one maatine and later on get reused by the state of another maatine
and thus making that string shared.

3. Objects of namespaces' global symbols

The way we have designed our garbage collector makes objects of
namespaces' global symbols pretty hard to manage, in most programming
languages, these globals form part of the root set but it is rather a 
completely different thing here. A maatine can perform a collection at
anytime and thus colors can be concurrently set to a collectable object
leading to data race and lost of references. Let's consider a case where
a maatine in the mark phase of its collection marks all objects of global
symbols black and another maatine which is in the sweep phase of its
collection changes the colors of all reachable objects back to white for
its next gc cycle, at this point even objects marked black by the former
maatine which hasn't even entered its sweep phase are now white which
raises a big problem since they're now treated as garbage.

4. Arguments to Maatine calls

A newly started maatine simply invoke a function in a new state with
arguments (potentially collectable values) gotten from the maatine that
started this one. Collectable values passed to a function call are passed
by reference and thus these collectable values become shared.

Channels which are themselves shared objects serve as a means to
thread-safely share objects across maatines. A channel can be shared via
global symbols, upvalues or function calls.

### Linked-List of Shared Objects (LSO)

To safely collect objects shared across maatines, maat introduces a
linked-list of shared objects kept in the global state
(i.e in the `GMa` struct). Each GC object pottentially considered
shared will be added to this linked-list.

A concurrently running program is said to have done a complete GC round
when all its maatines have at least done a single GC cycle whether in
incremental or generational mode. All unreachable shared objects from our
program will be freed by performing a **LSO sweep** after every complete
GC round.

A running maat program in a multi-threaded environment identifies all the
GC object sharing points to efficiently mark each root object as **shared**.
A root shared object may have direct/indirect links to other GC objects
which implies they are shared too but we won't go down the tree to mark them,
it'll terribly slow down our running program, marking the roots suffices,
however there is an exception where upvalues of shared closures are also
marked shared for the sake of implementing
[built-in auto-synchronization](./concurrency.md).

Some useful properties:

* The liveness of a shared object can only be determined if that object
  has gone through a complete GC round.
* The first maatine of a GC round is the one who in a set of `n` running
  maatines detected at the time it started its first GC run, is the first
  to have performed a complete GC cycle.
* The last maatine of a GC round is the one in that set to have last
  performed a complete GC cycle.
* Maatines eligible for a GC round are the ones who were running when the
  first maatine of that round started its GC run.
* In a running program of `n` maatines, while performing the current GC
  round, `(n - 1)` maatines can perform GC cycles of the round following
  this one and we thus have maximum 2 rounds at a time.
* A maatine can go through multiple GC cycles before the current round is done.
* New shared objects detected when a maatine performs multiple GC cycles
  before the end of the current GC round aren't part of this round but the
  next one. These new shared objects are neither directly added to the LSO nor
  marked **shared** but added in a special pending list, this is to avoid the
  need to synchronize when propagating marks.

For each maatine part of the current GC round in its GC cycle, encountered
green/white root shared objects in the mark phase are turned black but this
black color isn't propagated over incident node objects of these root objects.
After the sweep phase, the LSO is traversed turning all white root shared
objects green. In the first GC cycle of the last maatine, after the sweep
phase, green objects are turned red as they are not reference by any other
maatine provided that the conversative nature of this GC round is preserved.

It will be a disaster if mark propagation requires synchronization,
fortunately we only have to sync when attempting to mark shared objects.
Checking wether or not an object is shared does not even require sync, it is
so thanks to our pending list and the concept of GC rounds.
An object marked **shared** will remain shared till its memory is reclaimed,
even if it ends up referenced by a single Maatine. This is because turning
any shared GC object unshared requires expensives synchronization and it
isn't efficient as it may become shared again. It isn't that inefficient
to unnecessarily sync operations on a GC object as we will be using an
appropiate spinlock from `librs`.

At the end of each GC round, the LSO will either have black or red objects
from its head to a certain limit (this limit is discussed below). The red
color of these objects is propagated all over their incident node objects
and memory of these red objects are then reclaimed, this is known as an
**LSO sweep**.

Given that multiple maatines can concurrently collect, it appears that while
traversing the LSO, a root shared object to be or already marked green by a
maatine may be reachable to other maatines and thus synchronization must be
done to make sure that in such cases the root shared object in question ends
up black. While the green mark is propagated on incident node objects, black
objects could be encountered which is due to the fact that some reachable
objects local to a maatine have links to this shared space, in such cases it
is necessary to avoid them, leave them unchanged. Why? after every complete
GC round, the goal is to traverse green objects and mark the ones unreachable
to all maatines red and later on free all red objects, so we won't want
to free a green object reference by a reachable object local to a maatine.

Controlling the number of maatines in a running maat program to choose the
starting and ending point of a complete GC round is extremely hard, here
are some problems faced and possible solutions.

1. GC objects held by namespace variables

If someone tells you that GC objects held by namespace variables are
shared, it is true. This is because any maatine who has `use`d or was
created by a maatine that `use`d some namespaces can at any time get
or set the content of global symbols of these namespaces. These shared
GC objects won't be treated in the same way as we treat the others
arising from other sharing points during a GC round, only one maatine
can mark the global space, so a maatine in its GC cycle, before
attempting to mark the global space, should check if the first maatine
of the current round has already done it, oooh shit! there is another
problem. We are in a concurrent system and a maatine can perform multiple
collections before an LSO sweep. So if we mark global objects black then
they will eventually get white in the sweep phase of the first maatine.
This is not diserable as these objects may be part of other maatines,
hence some will potentially get freed. To solve this we will have
to specially mark global objects yellow, hmm... another color? yes!
it fixes this, we just have to maintain global objects yellow through
out the GC round so that they do not get collected, at this point, new
GC object assigned to global symbols will be marked yellow and an object
dereferenced from the global symbol will be marked back to white or black
if it was reachable from within one or more maatines. YES! this implies
that in an object a yellow color can co-exist with any other color as
yellow is just a mark which tells that the object belongs to the global
space. Why? again because a GC object belonging to a global space doesn't
necessary means it isn't reference by a reachable object within a
maatine.

2. Setting boundries in the processing of shared objects.

The bad thing with concurrency collection as stated above is that when
it comes to processing shared objects, we don't know where to start and
end. We are at the point of starting a GC round, we set an atomic variable
to notify the start of the round, maatines being part of the round go
through GC cycles but without even done with this round, a maatine part of
the round spawns off another maatine, well, this will potentially create
new shared objects or be linked to already existing ones. How does the
current GC round deal with this? below are things to consider in order to
assure coherency and avoid lost of references, bear in mind that most
collectors are conservative and these techniques aren't far from that.

* All root shared GC objects shared between maatines part of the current
  GC round and the ones not part of it must be marked black. A root
  shared object that'll definitely ends up red after the end of the
  current round should logically never be linked to the object space of
  maatines that aren't part of the current round. But because gc execution
  interleaves with the mutator, an object of a new maatine can be linked
  to a shared object that later on gets unreachable before the
  **LSO sweep**, if all the maatines part of the current round has lost
  references to this shared object, then new maatines may lost
  references to it.

* 


#### The Concurrency Invariant








# Gen algorithm

Two parameters determine whether the next collection to be done is a minor
or major one. Let X and Y be paramaters which represent the ceil of the
percentage increase of the estimated non-garbage memory in use for the
minor and major collection respectively.

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

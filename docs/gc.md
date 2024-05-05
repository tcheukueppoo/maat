# Maat GC

Maat implements generational and incremental garbage collection. This section
focuses more on implementation details, check the below links to know what
generational and incremental garbage collection are.

We are in a concurrent system where maatines a.k.a VM-level non-blocking threads
are scheduled by the Maat runtime scheduler to run over operating system
threads. A maatine is basically a lightweight thread that has at least one state
with each state owning a stack to execute some bytecodes, a maatine can switch
between any of its states, each maatine performs its collection either
incrementally or generationally and independently of the other ones. That said,
a lot must be considered to assure coherency and efficient synchronization.

Why have we choosen this design?

* Most objects are maatine-local.
* Only a few number of maatines (a.k.a lightweight threads) might need a GC run
  at the same time.
* Concurrent collection avoids us from stoping the world, each maatine performs
  it collection in gen or inc mode without annoying other maatines unless
  necessary.

With this design, major problems faced during collection are the following:

## Collection of open upvalues scattered across states of a Maatine

A Maatine has at least one state which with the use of its own stack, simply has
to run a function which is literally just a closure that possibly has open
upvalues pointing to values living in the stack of another state (if any) of
this same maatine, the latter state may have not been marked and ends up
collected but before this has to happen, its open upvalues that have been marked
during the marking phase of the maatine GC run must be closed and painted black
so that they cannot longer be collected during the sweeping phase.

Solving this problem enforces us to do the following in each maatine's GC cycle:

After all gray objects have been processed, all reachable states are now marked
black, so go through this maatine's list of states with still opened upvalues
and for each unmarked state, identify all its non-white open upvalues to mark
the stack values they are pointing to to black thus making them reachable.
Unless they'll get reached again before the sweeping phase(maybe in the atomic
phase), remove the unmarked states we found in the
list-of-states-with-open-upvalues from that list as they are now considered
garbage.

## Objects shared accross maatines

The fact that each maatine runs its garbage collector independently of the other
ones poses a real issue on objects shared across Maat's maatines.

### List of sharing points

1. Upvalues

The state of a Maatine may have a closure with open upvalues pointing to values
in the stack of the state of another Maatine. We not only have to synchronized
access operations on these upvalues during runtime in case maat runs on in
multi-threaded environment but we also have to make sure the unreachability of
the maatine detaining those stack values does not cause these values to be
collected as long as some other reachable maatines use them via closures or by
other means. This problem is also encountered in closed upvalues as a the gc may
want to free unreachable closed upvalues reachable to other maatines.

2. Cached Strings

For memory efficiency, we have a global cache of strings and a global Map of
short strings for internalization. Concurrent access to these globals structures
must be synchronized, that said, it is sure a running program will get to a
point where a string will be created by the state of one maatine and later on
get reused by the state of another maatine and thus making that string shared.

3. Objects of namespaces' global symbols

The way we designed our garbage collector makes objects of namespaces' global
symbols pretty hard to manage, in most programming languages, these globals form
part of the root set but it is rather a completely different thing here. A
maatine can perform a collection at anytime and thus colors can be concurrently
set to a collectable object leading to data race and lost of references. Let's
consider a case where a maatine in the mark phase of its collection marks all
objects of global symbols black and another maatine which is in the sweep phase
of its collection changes the colors of all reachable objects back to white for
its next gc cycle, at this point even objects marked black by the former maatine
which hasn't even entered its sweep phase are now white which raises a big
problem since they're now treated as garbage.

4. Arguments to Maatine calls

A newly started maatine simply invokes a function in a new state with arguments
(potentially collectable values) gotten from the maatine that started this one.
Collectable values passed to a function call are passed by reference and thus
these collectable values become shared.

5. Channels

Shared Channels serve as means to safely share objects across maatines, a
channel can be shared via global symbols, upvalues or function calls. Though
channels themselves are shared, mutable objects shared across them won't be
marked shared as it voilates the concept of shared by communication, when a GC
mutable object or shared immutable object is send from one maatine to another,
that object in question is supposed to be used only in the destination maatine
and thus maat will report an arror and exit if this condition is voilated unless
the object is immutable.

Unfortunately, the only feasible way to implement this for now is by having each
GC object track the id of the maatine that allocated him and thus channeling a
GC object from one maatine to another also implies transfer ownership by
changing the object's id. This technique is not space efficient as it will
require an additional field in the header common to all objects.

### Linked-List of Shared Objects (LSO)

To safely collect objects shared across maatines, maat introduces a linked-list
of shared objects each hold by every maatine.

A concurrently running program is said to have done a complete GC round when all
its maatines have at least done a single GC cycle whether in incremental or
generational mode.

A running maat program in a multi-threaded environment identifies all the GC
object sharing points to efficiently mark each root object **shared**. A root
shared object may have direct/indirect links to other GC objects which implies
they are shared too but we won't go down the tree to mark them, it'll terribly
slow down our running program, marking the roots suffices, however there is an
exception where upvalues of shared closures are also marked shared for the sake
of implementing [built-in auto-synchronization](./concurrency.md).

Maatines should only free objects they have allocated, no other maatine should
do it! That said, we should find a way to know if a shared object unreachable to
the maatine that created it is not referenced by a living object in another
maatine and we can only be sure of this by making sure that at the sweeping
phase of a gc of maatine, unreachable shared objects instead of being freed,
they are rather it added to that maatine's LSO. The collection of objects in the
LSO of each maatine will be done when all maatines have performed a collection
since this assures us on the reachable of all shared objects.

When an OS thread starts the garbage collector of a maatine, it'll have to
process that maatine' worklist of objects which at the start logically should
contain rooted objects, these objects belong to that maatine. In normal
circumstances the collector should only traverse objects of that maatine to
propagate marks but due to the fact that objects can be shared accross maatines,
the collector might encounter a shared object, an object that may not belong
to this maatine. As we said from the start, memory of objects should only be
reclaimed by the collector that ran the collection for the maatine owning these
objects which is why any encountered shared objects from the worklist will
either be marked gray or send to the worklist of the maatine that owns this
shared object. We are not stoping the world when performing a collection which
is why processing shared objects by marking them gray will likely happen unless
there's an emergency in which all the maatines much make a gc run. Every
encountered shared object we don't own might have links to objects we own and so
if we cannot send shared objects to the worklist of their owners and rely on
them to forward back our objects to our worklist, we'll be obliged to process
them in our worklist with the aim of marking other shared objects we don't own
gray and shared and blacken the ones we own.

**Why are we marking shared objects gray?**

* A shared object marked gray will eventually be in the LSO of its owner, i.e
  the maatine the owns the shared object.
* Only shared unreachable objects are added to an LSO which means that at the
  end of the sweep phase of a maatine, its LSO will either have white or gray
  objects which gives us a hint on which shared object to free at the time
  we'll be sweeping the LSO.
* Although crossing over a shared object while leaving it gray breaks the
  invariant, the object will enventually ends up black in case it's reachable
  in it's maatine and therefore it'll not be part of the maatine's LSO.

**Why are you marking any subsequent object directly or indirectly linked to the
shared object we don't know shared**

Remember that we only marked shared the top-level objects at sharing points, the
objects directly/indirectly linked to the shared object which logically should be marked shared where
not marked shared and this was a wise move because it actually did avoid pauses.
We are in the middle of a gc run which means that we'll eventually traverse
object which is an opportunity to mark supposed to be shared object shared. An this is
done in the following scenarios.

* Propagating the black mark over shared reachable objects we own is a great opportunity
* The shared mark does not only help us in avoiding the lost of references but also
  in the auto-synchronization of access to shared resources. Which is
  why whenever operations on complex shared object such as arrays, maps return other
  objects, these objects will be automatically be marked shared because they may be
  subjected to other concurrent access operations.
* When processing the share worklist, the share work list of a maatine is processed after 
  its worklist has been process with the aim of marking all object shared and gray
  and this is done before the sweeping phase because we need to make sure all shared
  objects end up in the maatine' LSO.


**What happens when we encounter a shared black object we don't own?**

A shared black object we don't own is already reachable to the maatine that owns
the object and will not appear in the object's LSO which is why we just have to
simply ignore it, if the object is linked to any object we own, the gc run from
that maatine will make sure to send it to our share work list.


**Why sending a shared object we don't own to the worklist of the maatine that
owns it?**


**Why sending a shared gray object to the share worklist of the maatine that
owns it?**

A gc run for a maatine processes its worklist before processing its share
worklist. There is no garantee that the shared gray object is reachable to the
maatine that owns it, if it is reachable, it'll eventually be blackened before
the share worklist is even processed which is why every black object in the
shared worklist will be ignored.

#### The Mutator



#### 



Some useful properties:

* The liveness of a shared object can only be determined if that object has
  survived a complete GC round, that means the shared object is used by at least
  one maatine.
* The first maatine of a GC round is the one who in a set of `n` running
  maatines detected at the time the first GC run was started, is the first to
  have performed a complete GC cycle.
* The last maatine of a GC round is the one in that set to have last performed a
  complete GC cycle.
* Maatines eligible for a GC round are the ones who were running when the ever
  first GC run of that round was started.
* In a running program of `n` maatines, while performing the current GC round,
  `(n - 1)` maatines can perform GC cycles of the round following this one and
  we thus have maximum 2 rounds at a time.
* A maatine can go through multiple GC cycles before the current round is done.
* New shared objects detected when a maatine performs multiple GC cycles before
  the end of the current GC round aren't part of this round but the next one.



For each maatine part of the current GC round, in its own GC cycle, encountered
green/white root shared objects in the mark phase are turned black but this
black color isn't propagated over incident node objects. After the sweep phase,
the LSO is traversed turning all white root shared objects green. In the first
GC cycle of the last maatine, after the sweep phase, green objects are turned
red as they are not referenced by any other maatine provided that the
conversative nature of this GC round is preserved.

It will be a disaster if mark propagation requires synchronization, fortunately,
we only have to sync when attempting to mark shared objects. Checking wether or
not an object is shared does not even require synchronization, it is so thanks
to our pending list and the concept of GC rounds. An object marked **shared**
will remain shared till its memory is reclaimed, even if it ends up referenced
by a single Maatine, this implies that it is safe to check without the need to
synchronized if an object is shared when propagating marks. This is also because
turning any shared GC object unshared requires expensives synchronization and
it isn't efficient as it may become shared again. It isn't that inefficient to
unnecessarily sync operations on a GC object as we will be using spinlocks from
`librs` and not syscalls and in such cases, there is zero contention.

At the end of each GC round, the LSO will either have black or red objects.
The red mark of these objects is propagated all over their incident node
objects and memory of these red objects are then reclaimed, this is known
as an **LSO sweep**.

Given that multiple maatines can concurrently collect, it appears that
while traversing the LSO, a root shared object to be or already marked
green by a maatine may be reachable to other maatines and thus
synchronization must be done to make sure that in such cases the root
shared object in question ends up black. While the green mark is propagated
on incident node objects, black objects could be encountered which is due to
the fact that some reachable objects local to a maatine have links to the
shared space, in such cases it is necessary to avoid them, leave them
unchanged. Why? after every complete GC round, the goal is to traverse green
objects and mark the ones unreachable to all maatines red and later on free
all red objects, so we won't want to free a green object referenced by a
reachable object local to a maatine.

Controlling the number of maatines in a running maat program to choose the
starting and ending point of a complete GC round is extremely hard, here
are some problems faced and possible solutions.

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
  references to it. Yep! this is kind-of a conversative behavoir here.

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

# The Maat Garbage Collector

Maat does generational and incremental garbage collection **without** stoping
the world. Technically, it either simply do an incremental collection or a
somehow incremental generational collection, it makes sure all maatines
collectively performing a garbage collection in small steps since doing so can
significantly reduce latency. This section assumes you know what incremental and
generational collection are and focuses more on implementation, optimization,
and synchronization details. If you don't know yet, check the below links to
know what generational and incremental garbage collection are.

We are in a concurrent system where maatines a.k.a VM-level non-blocking threads
are scheduled by the Maat runtime scheduler to run over operating system
threads. A maatine is basically a lightweight or virtual thread that has at
least one state a.k.a context with each state owning a stack to execute some
bytecode. A maatine can switch between any of its states or states from other
maatines gotten via sharing points. Each maatine performs its collection in
incremental or genenrational mode and makes sure an object unreachable to the
maatine that owns it, referenced by a live objects from other maatines, is not
reclaimed by the GC. This is achieved by establishing a synchronization point at
the end of each atomic phase where all collectors have to gather before
proceeding to the sweep phase. That said, a lot must be considered to assure
coherency and efficient synchronization. Before you start arguing the sense
behind doing a generational garbage collection incrementally, note that an
incremental collection in generational mode is only performed in a major
collection since short living objects are small enough for a minor collection to
be done in multiple garbage collection steps.

Why have we choosen this approach:

Only a few number of maatines (lightweight threads) might need a collective GC
but we must synchronize this way to avoid loss of references of shared objects.
Stoping the world and using multiple threads to perform a collection can
introduce very long pauses in a multi-task environment since many threads in the
threadpool will be stuck on long running C code and therefore having each maatine
perform a collection in steps can be very beneficial as it does not only
reduces these pauses but prevent other virtual threads from starving.

Since we are in a multi-threaded environment, they're so many subtleties we'll
have do deal with but for now I'm going to take your attention on some
implementation details regarding generational and incremental collection.

## Generational Garbage Collection

### The Minor Collection

Collectible objects survive two GC cycles before getting old. As you can see, we
most of the time insist on the fact that Maat can run in a multi-threaded
environment, this is because effectively handling concurrent execution of Maat
code is one of the design goals of Maat, we have threads that perform GC in
steps and that's good, but a given collection pace is not always the best for
certain cases and as a consequence our program can consume too much
memory which is why having too many potentially unreachable objects in the old
generation which are later on collected in GC **steps** can unwantedly reduce
the proportion of collection to memory usage as the mutator also has the right
to allocate more objects when interleaved with the garbage collector. Also,
migrating to the old generation only after surviving 2 GC cycles can reduce the
number of short-lived objects that only survive because we happened to GC during
their short lifetime.

The below diagram is a snapshot of the states of objects of a maatine in each of
their generations just right after the GC has completed the atomic phase.
Double line arrows represent the migration of survivals objects from one
generation to the next and single line arrows represent write barriers between
objects of different generations. Of course, objects within a generation are
linked and form a complex graph but the represention of these links does not
address the issues we want to discuss on.

* `(#<number>)` represents a given backward write barrier.
* `(M)` represents the migration of objects from one generation to the other.
* `[b]` represents a black object.
* `[w]` represents a white object.
* `t`:  *touched* mark, for nursery-2 objects pointing to nursery-1 objects.
* `t1`: *touched1* mark, for old objects pointing to nursery-1 objects.
* `t2`: *touched2* mark, for old objects pointing to nursery-2 objects.

```
                     (#1)
                 .--------------------------------------.
                 |                                      |
                 |             (#0)                     |
                 |     .-----------------.              |
      Nursery-1  |     |                 |  Nursery-2   |
 .---------------|-----|-----.       .---|--------------|-------.
 |               v     v     |       |   |       [b]    |       |
 | [b]  [w]     [b]   [w]    |       | .---.          .---. [b] |
 |                           |======>| |w,t| [w] [w]  |w,t|     |=====*
 |    [b]   [w]      [w] [b] | (M)   | '---'          '---' [w] |     #
 |              [w]          |======>|         [b]              |===* #
 |     [w] [b]       [b]     |       | .---.   .---. [w]    [b] |   # #
 | [b]      ^   [w]      [w] |       | |b,t|   |b,t|    [b]     |   # #
 |  ^  [b]  |        [b]     |       | '---'   '---'     ^  [w] |   # #
 |  |   ^   |    [w]  ^      |       |    |     | ^  [w] |      |   # #
 `--|---|---|---------|------' (#2)  `----|-----|-|------|------'   # #
    |   |   |         `-------------------'     | |      |          # #
    |   |   `-----------------------------------' |      |          # #
    |   |   (#3)               .------------------'      |          # # (M)
    |   |                      |                         |          # #
    |   |                      |(#5)                     |(#4)      # #
    |   |                      |                         |          # #
    |   |                  .---|-------------------------|------.   # #
    |   |                  |   |      [b]    [b]         |      |   # #
    |   | (#6)             | .----.              .----.  |      |   # #
    |   `--------------------|b,t1| [b]  [b]     |b,t2|--'  [b] |<==# #
    |                      | '----'          [b] '----'         |     #
    |                      |           [b]                      |<====#
    |   (#7)               |  .----.         [b] [b]   [b]  [b] |
    `-------------------------|b,t1| [b] [b]                    |
                           |  '----'            [b]    [b]  [b] |
                           `------------------------------------'

                                     Old
```

We have three generations--nursery-1, nursery-2 and old. Objects become old when
they've survived two GC cycles, i.e the first in nursery-1 and the second in
nursery-2. After sweeping the first generation (nursery-1) in the first cycle,
survivors are migrated to the second generation (nursery-2) and only survivors
of the second generation in the second GC cycle are migrated to the old
generation. That's not just all, we may get into a situation where old objects
point to objects in either nursery-1 or 2 or nursery-2 objects point to
nursery-1 objects. In other generational GC implementations where new objects
only have to survive one GC cycle before being migrated to the old generation,
we would simple have to implement a backward write barrier to track all old
objects (touched) referencing the new ones so we can later on traverse them in
the atomic phase and blacken every encountered object from there since we're not
really sure of the liveness of old objects. At the end of the cycle, all the
blackened objects become old and the generational invariant is not broken until
the mutator runs before the next GC cycle and thus the list of touched objects
will simple be dropped at the end of every sweeps. In this implementation,
unreachable objects can be swept both in nursery-1 and 2 which can break the
generational invariant just right after migrations.

No matter how complicated this will look, the main aim is to make sure that when
the generational invariant is broken by making an old object point to any
object from either nursery-1 and/or 2, that old object is grayed and added to a
special list of touched object to be traversed in the atomic phase just before
sweeping objects from each generation and migrating them to their respective
next generations. As said before, it's because we don't really know anything
about the liveness of old objects until we do a major collection, so we'd rather
be conservative to avoid potential loss of references. Let's study each of
these cases:

1. Case `#0`

When the mutator detects a nursery-2 object pointing to a nursery-1 object, the
detected object is marked `touched` as we are not sure of its liveness until
nursery2 is swept. Case `#2` and `#3` dicusses how useful is this mark in case
the detected object is migrated to the old generation. In this our case, the
detected object ends up dead after the atomic phase and it'll simply be
reclaimed. An object reclaimed in nursery-2 simply proofs that the life time of
short-living of the object exceeds a single GC cycle.

2. Case `#1`

This is a special case of the previous one where the nursery-1 object pointed to
is black. This case is handled the same way as the previous one since the
nursery-2 object pointing to the nursery-1 object is not reachable from the
nursery-1 object because if it were then the nursery-2 object would've been
blackened before the sweeping phase.

3. Case `#2` and `#3`

An object in nursery-2 can still be reclaimed by the garbage collector but if it's
not reclaimed when nursery-2 is swept, it means it has been blackened in the
atomic phase and thus, will be migrated to the old generation, but wait..., what
happens if the mutator makes this object point to an object in nursery-1 before
the atomic phase? You may have not figured it out yet, this black object
migrated to the old generation could be the only object pointing to some objects
in nursery-1 and even if it's not the case, an old object is considered live
until we do a major collection because after the migration, at anytime, this old
object can become dead. Therefore, whenever an object in nursery-2 linked to
any object in nursery-1 is migrated to the old generation, its touched value is
changed to "touched2" and it's then pushed onto the list of touched objects so
that objects directly/indirectly linked to this old object are conserved until
they are migrated to the old generation awaiting a major collection. When this
object is migrated to the old generation, objects it was linked to in nursery-1
are migration to nursery-2 and in the next GC cycle they're finally migrated to
the old generation.

In simple terms, the nursery-2 object migrated to the old generation is added to
the list of touched objects for the next GC cycle because it still references
nursery-1 objects which may be collected when migrated to nursery-2 and you
don't want this to happen.

Rough pseudo-code demonstrating how objects in nursery-2 are swept:

```
let k = objects_in_nursery2(maa);
let (iter, prev) = k;

while iter != nil {
   let o = iter.obj;

   ...;
   if o.is_touched && o.is_black {
      // 'o' is now 'touched2' and not 'touched'
      o.touched2(1);

      // for the next GC cycle
      touched_list(maa).push(o);
   }
   elsif o.is_white {
      k = iter.next if iter == k;
      reclamation_of(o);
      prev.next = iter.next if prev;
   }

   prev = iter;
   iter = iter.next;
   ...
}

// 'k' now points to the list of survivors, migrate them to the old gen.
migrate_to_old_gen(k);
```

A nursery-2 object marked `touched` by the mutator will not be instantly added
to the touched list because we are not sure of its liveness until it migrates to
the old generation. We would still do the same thing if we were sure of its
liveness because the to-become old object should only be traversed in the next
GC cycle as nursery-1 objects it references are later on migrated to nursery-2.

4. Case `#4`

As you can see, we are in a case where an old object points to a nursery-2
object which means the old object in question needs to be grayed, marked
`touched2`, and pushed onto the touched list to be traversed in the atomic
phase. Again, this is a conservative measure, as we're not sure of the liveness
of old objects until a major collection is done.

After the sweep phase:

* All objects referenced by this old object will be migrated to the old
  generation which means there is no way they can break the invariant unless
  they've got references to objects in nursery-1 (case `#2` and `#3` explained
  how this is tackled).

* All `touched2` objects are dropped from the touched list since the nursery-2
  objects they points to are now officially old.

5. Case `#5`

This is a special case of the previous one where the nursery-2 object being
pointed to, points to a nursery-1 object. Though the nursery-2 object is
`touched`, it does not necessarily guarantee that it won't be reclaimed. So when
a write barrier detects an old object pointing to a `touched` nursery-2 object,
the old object is grayed, marked `touched2`, and pushed onto the touched list
just like we did in case `#4`. What is done in case `#4` after the sweep phase
is also done here.

Rough pseudo-code implementing a write backward barrier for nursery-2 objects:

```
// old -> nursery2
fn o_p_n2_write_barrier(o) {

   assert(o.is_gray()), return if o.is_touched1;

   o.mark_gray;

   // make 'o' 'touched2'
   o.is_touched2(1);

   touched_list(maa).push(o);
}
```

The `touched1` mark takes precedence over the `touched2` mark, this is because
the old object needs to stay in the touched list for some more time, see case
`#6` to get to know why it's so.

6. Case `#6`

Until now we've been using backward write barriers to make sure our garbage
collector remains conservative enough to avoid potential lost of references, we
will stick to this type of barrier til to the end. Other programming languages
like Lua for example, implement generational collectors that rely on forward
write barriers and hence too many new objects become old which can be really
inconvenient here. Why? Because when a nursery-1 object is referenced by an old
object, there's a chance that the old object lost references to that nursery-1
object when this one is migrated to nursery-2 and if when in nursery-2,
ends up being unreachable after the atomic phase, it'll be reclaimed with all
other unreachable objects it points to.

Here is a Maat code (not a pseudo-code!) that will serve us as an example to
demonstrate how marking nursery-1 objects old too fast, may unnecessarily full
up the old generation.

```maat
GC.mode('gen');

let a = <>;

GC.collect;
GC.collect;
// Collectible object in 'a' is officially old

// This calls a write barrier
a[1] = <Darfur El-Fasher>;

GC.collect;

// ...

a[1] = nil;
// <Darfur El-Fasher> is in nursery-2 and not eligible for collection

// ...

GC.collect;
```

If the write barrier makes the collectible object is in `a[1]` old, the object
will only be a candidate for collection in the next major collection which is
too far and not efficient because the reference by the old object is lost. When
the reference is lost, nothing proofs wrong that the new object made old could
potentially be unreachable if it was instead migrated to nursery-2. Another
drawback of using a forward write barrier in a generational collection where new
objects have to survive 2 GC cycles in order to become old is that it enforces
us to have two many kinds of old generation because a nursery-1 object marked
old still hold pointers to non-old objects and directly moving it to the old
generation will result to so many write barriers which is why programming
languages like Lua revisits these marked old nursery-1 objects in each GC cycle
until the non-old objects they point to become old, this is a bit similar to
what we are going to do here but much simpler.

As said before, a nursery-1 object pointed by an old object could still be
collected in nursery-2 and even if it's not, it's still got pointers to other
objects of its generation which is why to instantly mark it old is not a option.
To make sure it gets migrated to the old generation together with survivor
objects of its generation that it may or not have references to, we push the old
object that points to the nursery-1 object onto the touched list and make sure
it stays there until all the survivor objects of the generation of that
nursery-1 object including the nursery-1 object itself become old, i.e they'll
be migrated to nursery-2 and then to the old generation while the touched mark
of the old (touched) object is change from `touched1` to `touched2`. This
approach provides a benefit: in case the old object loses reference to the
already migrated to nursery-2 the nursery-1 object, the migrated object will be
reclaimed when nursery-2 will be swept unless it's still reachable from the root
set or other old (touched) objects from the touched list.


Rough pseudo-code implementing a write backward barrier for nursery-1 objects:

```
// old -> nursery-1
fn o_p_n1_write_barrier(o) {

   // mark 'touched1', 'o' already in the touched list
   o.touched1(1), assert(o.is_gray), return if o.is_touched2;

   o.mark_gray;

   // make 'o' 'touched1'
   o.touched1(1);

   touched_list(maa).push(o)
}
```

Rough pseudo-code that demonstrates what happens to the touched list after all
the migrations in a GC cycle have been performed.

```
let tl = touched_list(maa);
let (iter, prev) = (tl, tl);

while iter != nil {
   let o = iter.obj;

   if o.is_touched2 {
      tl = iter.next if iter = tl;
      prev.next = iter.next;
   }
   elsif o.is_touched1 {
      o.touched2(1);
   }
   else {
      fail "Error: Found a touched object that's neither 'touched1' nor 'touched2'.";
   }

   prev = iter;
   iter = iter.next
}
```

Some key points:

* The list of touched objects of a maatine should only contain `touched1` and
  `touched2` objects.
* All `touched1` objects are changed to `touched2` after every GC cycle and
  remain in the touched list until all the nursery-1 objects in the generation
  of the object the touched objects points to are migrated to the old
  generation.
* All `touched2` objects are dropped off the list after every GC cycle.
* A nursery-1 object referenced by an old object might still be collected when
  migrated to nursery-2.

### Major Collection (Incrementally)

A minor collections is done in a single garbage collection step because the
number of short-lived objects are small and thus the marking and sweeping
operations are relatively inexpensive in terms of time. In contrast, the cost of
a major collection is about the same as for a stop-the-world basic
mark-and-sweep garbage collector, so proceeding with an incremental major
collection in non-emergency situations is very beneficial. More is explained
in the section discussing the `MajorSize` GC parameter.

### Collection Parameters

We have multiple parameters that control the behavoir of the Maat garbage
collector. Here, we are going to discuss the ones common to the two type of
collectors Maat implements and dive deep into the ones specific to the
generational collector.

1. Garbage Collection Debt (**GCDebt**)

The garbage collection debt determines how much memory your program owns.
Initially, the debt is set to a negative integer say `-K` which means maat has
to allocate at least `K` bytes before initiating a GC. In other words if the
garbage collection debt is greater than zero then it means maat needs to perform
a collection so as to pay its memory debt.

```
fn gc_step (Maa) {
   if gc_debt(Maa) > 0 {
      // an inc or gen step
      do_a_gc_step(Maa)
   }
}
```

When a maat program starts running, it allocates memory for its structures which
at that time is the total number of bytes allocated in the system. Further
allocations of objects augment the debt up until it gets to a value greater than
zero which triggers a collection. Therefore the debt is an essential parameter
used in calculating the total number of allocated bytes in the system.

Setting the garbage collection debt requires us to substract the debt from the
total number of bytes such that the debt added to the total number of bytes
equals the actual number of bytes allocated in the system.

```
fn set_gc_debt (Maa, debt) {
    let max_mem = get_max_maat_mem(Maa);

    // Actual allocated mem in the system, it's based on the current debt
    let totalbytes = get_totalbytes(Maa) + get_debt(Maa);

    // Make sure (totalbytes - debt) does not overflow.
    debt = totalbytes - max_mem if debt < totalbytes - max_mem;

    set_debt(Maa, debt);

    // (totalbytes + debt) remains unchanged
    set_totalbytes(Maa, totalbytes - debt);
}
```

```
                                  Debt
(Still need to allocate memory)    0   (Owes more than the debt, time to collect)
---------------------------------------------------------------------------------
                                   ^
```

At any time `(get_totalbytes(Maa) + get_debt(Maa))` is always the correct number
of bytes of memory allocated in the system. As the debt grows to a positive
value, it's got more bytes to be added to `totalbytes` and when it reduces to
a negative value, it's got more bytes to be reduced from `totalbytes` which
happens after the collector has freed unreachable objects.

2. Garbage Collection Estimate (**GCEstimate**)

This is an estimate of non-garbage memory in use. The garbage collection
estimate changes after every complete garbage collection cycle. At the end of a
garbage collection cycle, the garbage collection estimate value equals the total
number of bytes allocated in the system.

3. Garbage Collection Pause Size (**GCPause**)

GCPause defines the wait time between successive garbage collection cycles. It's
from this parameter that we determine the garbage collection debt, it's also a
factor that determines how much of the initial estimate of the memory of your maat
program has to grow before we start a new collection.

This algorithms calculates based on the pause size and sets the garbage
collection debt:

```
fn cal_and_set_gc_debt (Maa) {
    let estimate = get_gc_estimate(Maa);
    let pause_size = get_gc_pausesize(Maa);
    let totalbytes = get_totalbytes(Maa);

    // 
    assert(totalbytes == estimate);

    // The maximum amount of memory maat can allocate
    let max_mem = get_max_maat_mem(Maa);

    threshold = (pause < (max_mem / estimate)) ? estimate * pause_size : max_mem;
    new_debt  = totalbytes - threshold;

    set_gc_debt(Maa, new_debt);
}
```

Below is an abstract representation of the Maat memory:

```

 +-----------------------------------+
 |+++++++++++++++++++++++++++++++++++|
 +-----------------------------------+
 <--   estimate   --> <-- garbage -->

With pause_size = 2
                                     <-- New debt given 'pause_size'  -->
 +------------------------------------------------------------------è---+
 |+++++++++++++++++++++++++++++++++++???????????????????????????????????|
 +----------------------------------------------------------------------+

Allocations fills in the '?' uptil it triggers a collection
```

4. Minor collection size (**MinorSize**)

GCMinorSIze controls the minor collection pace, it starts a minor collection
when memory grows `MinorSize%` larger than it was since the last collection.

```
fn set_minordebt (Maa) {
    let minor_size = get_minor_size(Maa);
    let totalbytes = get_totalbytes(Maa) + get_debt(Maa);
    let debt = ( totalbytes / 100 ) * minor_size;

    set_gc_debt(Maa, -debt);
}
```

In a generational step, the collector determines if it's in a condition of
performing a major collection, if not, it performs a minor collection and sets
the minor debt for the next generational step.

3. Major collection size (**MajorSize**)

A major collection is performed when memory grows `MajorSize%` larger than the
garbage collection estimate obtained in the last major collection. In a minor
collection, the rest of the non-garbage objects in the nursery are migrated to
the old generation. This happens for every minor collection and thus objects are
accumulated in the old generation up until the condition to perform a major
collection is met.

```
fn generational_step (Maa) {

    // Get the GCEstimate of the last major collection.
    let major_base = get_mejor_base(Maa);

    let major_size = get_major_size(Maa);
    let totalbytes = get_totalbytes(Maa) + get_debt(Maa);

    let major_new = (major_base / 100) * major_size;

    // Major collection condition met?
    if totalbytes >= major_base + major_new { 
        num_objs = major_collection(Maa);

        // Now test if major collection was good, meaning that it has
        // freed at least half of the memory that grown.
        if totalbytes < major_base + (major_new / 2) {
            // To start doing minor collection
            assert(num_objs == 0);
            set_minordebt(Maa);
        }

        // A bad collection, would switch to inc mode until further
        // collections become good.
        else {
            cal_and_set_gc_debt(Maa);

            // Signal to the next GC cycle that this one was bad
            set_num_objs_traversed(Maa, num_objs);
        }
    }
    
    // Do a minor collection and set the minor debt
    else {
        minor_collection(Maa);
        set_minordebt(Maa);

        // Restore it if minor_collection() ever changed it
        set_major_base(Maa, major_base);
    }
}
```

In the Lua programming language, it's stated that a major collection is said to
be a good collection if it has freed at least half of the memory that has grown
since the last major collection; otherwise it's bad. It's also stated that a bad
major collection arises when a running program is building an immensely big
data structure, the build allocats lots of memory but yields very little garbage
and at this time minor collections are a waste as they will be called
repeatedly to free very little or no memory. These minor collections are
unavoidable until they trigger a major collection which in most of the cases is
potentially a bad collection. When this bad collection is detected, the
collector switches to incremental mode in order to improve performance and later
on returns to generational mode when any further collection is good. We
determine whether or not a major collection is good after its atomic phase by
comparing the complete number of objects traversed with the one of the previous
major collection. If the number of object traversed is considerably lower than
the one of the previous major collection, the collection is said to be good and
collector switches back to generational mode before finishing its cycle and set
the minor debt for the next cycle. As in normal incremental mode, a major
collection goes full in emergency situations.

### Weak Maps

### Ephemeron Maps

### Finalizers

## Incremental Garbage Collection

The Maat programming language also got an incremental garbage collector in which
the execution of the main program is interleaved with the collector. From the
garbage collector's point of view, the program is just some nuisance changing
the data it is trying to collect and hence it's called the mutator. In order to
implement an incremental collector, we need to know how work is percieved here
and the miminum work that needs to be done in an incremental step.

In an incremental step, a work is done when an object is processed. An object
could be processed to change its mark, reclame its memory or to perform a
special task. The execution of the main program is interleaved with the
collector at the rate at which the main program allocated memory and therefore
we need to find the equivalence between a unit of work (processing an object)
and memory. Letting a unit of work be an abitrary number of bytes affects the
collection pace but we merely can't estimate how much number of bytes which is
why the step multiplier exists as a factor to speed up the collection.

1. Garbage Collection Step Size (**GCSS**)



2. Garbage Collection Step Multiplier (**GCSM**)



### Collection Paramaters

### Finalizers

## Other major problems faced during collection

### Collection of open upvalues scattered across different states of a Maatine

A Maatine has at least one state which with the use of its own stack runs
a function which is literally just a closure that possibly has open
upvalues pointing to values living in the stack of another state (if any) of
this same maatine, the latter state may have not been marked and ends up
collected but before this happens, its open upvalues that have been marked
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

### Objects shared accross maatines

The fact that each maatine runs its garbage collector independently of the other
ones poses a real issue on objects shared across Maat's maatines.

#### List of sharing points

1. Upvalues

The state of a Maatine may have a closure with open upvalues pointing to values
in the stack of the state of another Maatine. We not only have to synchronized
access operations on these upvalues during runtime in case maat runs on in
multi-threaded environment but we also have to make sure the unreachability of
the maatine detaining those stack values does not cause these values to be
collected as long as some other reachable maatines use them via closures or by
other means. This problem is also encountered in closed upvalues as a the gc may
want to free unreachable closed upvalues reachable to other maatines.

2. Internalized Strings

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
set to a collectible object leading to data race and lost of references. Let's
consider a case where a maatine in the mark phase of its collection marks all
objects of global symbols black and another maatine which is in the sweep phase
of its collection changes the colors of all reachable objects back to white for
its next gc cycle, at this point even objects marked black by the former maatine
which hasn't even entered its sweep phase are now white which raises a big
problem since they're now treated as garbage.

4. Arguments to Maatine calls

A newly started maatine simply invokes a function in a new state with arguments
(potentially collectible values) gotten from the maatine that started this one.
Collectable values passed to a function call are passed by reference and thus
these collectible values become shared.

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
GC object from one maatine to another also implies transfering ownership by
changing the object's id. This technique is not space efficient as it will
require an additional field in the header common to all objects.

#### Managing Concurrent Collection across OS Threads.

To safely collect objects shared across maatines, maat introduces a linked-list
of shared objects. Each maatine hold an LSO.

A running maat program in a multi-threaded environment identifies all the GC
object sharing points to efficiently mark each root object **shared**. A root
shared object may have direct/indirect links to other GC objects which implies
they are shared too but we won't go down the tree to mark them, it'll terribly
slow down our running program, marking the roots suffices, however there is an
exception where upvalues of shared closures are also marked shared for the sake
of implementing [built-in auto-synchronization](./concurrency.md).

Maatines should only free objects they have allocated. That said, we should find
a way to know if a shared object unreachable to the maatine that allocated it
is not referenced by a living object in another maatine. We can only be sure of
this by making sure the first two sweeps in the gc of a maatine (`sweep_gcs` and
`sweep_fin`) put unreachable shared objects in the gc's LSO. An LSO sweep can
only be done when all maatines have initiated a gc run that has passed the
atomic phase as it'll assure all reachable shared objects are marked and avoid
loss of references. This means that the garbage collector of a maatine will wait
until all the other garbage collectors of the other maatines have passed the
atomic phase(`atomic`) before sweeping its LSO, this is not really bad because
most objects are maatine-local and have already been freed in the previous
sweeps.

When an OS thread starts the garbage collector for a maatine, it'll have to
process the maatine' worklist of objects which at the start should logically
contain rooted objects the maatine owns. In normal circumstances the collector
should only traverse objects of that maatine to propagate marks but due to the
fact that objects can be shared accross maatines, the collector might encounter
a shared object that does not belong to this maatine. As we said from the start,
memory of objects should only be reclaimed by the collector of the maatine
owning those objects which is why any encountered shared object we don't own
from the worklist will first be marked **gray** and be either processed or send
to the share worklist of its owner. Whether to process or send the object to the
share worklist of its owner depends on the garbage collection state of the
owner. Every encountered shared object we don't own might have links to objects
we own and so if we cannot send shared objects to the share worklist of their
owners and rely on them to forward back our objects to our share worklist, we'll
be obliged to process them in our worklist with the aim of blackening objects
we own and marking **shared** and **gray** shared objects we don't own.

```maat
// Possible objects' colors
let gcolor = Enum.new(<white white2 gray black>);

// Garbage collection states
let gstate = Enum.new(<
                        propagate
                        atomic
                        end_atomic
                        propagate_lso
                        sweep_gcs
                        sweep_fin
                        call_tobefin
                        pause
                     >);

// Abstract representation of a garbage collectible object
let obj = {
   // ...
   color  => gcolor<white>,
   shared => 0,
   gray2  => 0,
   // ...
};
```

**What is a worklist?**

A worklist is simply a list of reachable objects a maatine has to traverse with
the aim of blackening them. When a running program in a multi-threaded
environment have more than one maatine, a gc of a maatine can encountered
objects it does not own.

**What is a share worklist?**

Each maatine has its own share worklist which contains shared objects that it
owns and were found by other maatines when they were going through their
worklists. Don't misunderstand the meaning of "share" in share worklist as a
list containing any shared object, these shared object should only be owned by
the maatine that owns the list. Processing a share worklist is all about marking
every objects in the list gray and shared before the sweep phase.

**Why and when to send a shared object we don't own to the share worklist of the
maatine that owns it**

Any shared object we don't own encountered in our worklist **may** have links to
objects we own, this truly is not just an assumption and there is no garantee
that the objects we own linked to this shared object is reachable from our root
set. It's very hard to speculate on the number of objects we own that are
forward-linked to shared objects we don't own and in the worst case these shared
objects could be connected to a very dense graphs of objects reachable to
maatines that owns them which would imply multiple useless graph retraversals if
we decide to process these shared objects, this is why it's performant to let
the gc of maatines process shared objects the own. However there's an exception,

After the atomic phase, all shared white object we own detected by other
maatines are not reachable from our root set because if they were, they would've
been traversed and blackened after the atomic phase. This is why after the
atomic phase, it wouldn't be efficient to sent any shared object we own from
other maatines to our share worklist because its reachability solely depends on
other maatines and so we should let them process it as it is less likely we'll
be concerned later.

A gc run for a maatine processes its worklist before its share worklist. There
is no garantee that the shared object we don't own encountered in our worklist
is reachable to the maatine that owns it as it depends on the gc state of that
maatine, if it is reachable, it'll eventually be blackened before the share
worklist of the owner of that maatine is even processed which is why all black
objects in the share worklist are not processed. With all this said, we can
conclude that the best time to send a shared object we don't own to the share
worklist of the maatine that owns it is when the gc state of that maatine has
not passed the atomic phase.

```
(shared gray object) --(x)--> (shared black object)
```

```
fn send_share_object_to_share_list(ma, obj) {
   spin_lock();
   if (gc_state(m1) == NONE || gc_state(m2) > SWEEP)
      return false;
   spin_unlock();
   return un
}
```

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
shared object we don't own shared?**

Remember that we only marked "shared" top-level shared objects at sharing
points, objects directly/indirectly linked to these top-level objects which
logically should also be marked "shared" where not marked and this was made so
to avoid pauses. We are in the middle of a gc run which means that we'll
eventually traverse object which is an opportunity to mark supposed to be marked
"shared" object "shared" and this is done in the following scenarios:

* When propagating the black mark over shared reachable objects we own, this is
  a great opportunity we have here.

* The `propagate_lso` phase. At sharing points, top-level shared objects are
  added to the LSO of their owners so that at this phase the LSO is processed to
  propagate the "shared" mark. This phase is right before the sweeping phase to
  avoid freeing not yet marked "shared" some shared objects that ended up being
  reachable only to maatines they don't belong to. Black shared objects are
  skipped from the LSO since the "shared" mark is also propagated when
  propagating the black mark.

```maat
let iter = get_lso_list(maa);

while iter != nil {
    let obj = iter.obj;

    if ! obj.is_black {
        let objs = objects_directly_linked_to(obj);

        objs.each: .mark_shared;
        lso_list(maa).append(objs);
    }

    prev = iter;
    iter = iter.next;
}
```
  
* The mark "shared" does not only help us in avoiding lost of references when
  garbage collecting but also helps us in the auto-synchronization of concurrent
  access to shared resources. Which is why whenever operations on complex shared
  object such as arrays, maps return other objects, these objects will be
  automatically marked "shared" because they may be subjected to other concurrent
  access operations. Marking the to-be-returned object "shared" does not require
  sync since access to the shared object it is linked is sync.

* When processing a share worklist. The share worklist of a maatine is processed
  after its worklist has been processed, the aim of processing the share
  worklist is to mark all objects shared and gray and this is done before the
  sweeping phase because we need to make sure all shared objects end up in the
  maatine' LSO.

**What happens when we encounter a shared black object we don't own?**

Whether in the worklist or share worklist, a shared black object we don't own is
already reachable to the maatine that owns it and will not appear in that
maatine's LSO which is why we just have to simply ignore it, if the shared black
object is linked to any object we own, the gc for that maatine will make sure
it sends it to our share worklist.

**What happens when we encounter a shared gray object we don't own?**

Such shared objects have already been encountered in the gc run of a maatine
that does not own it and all the things we would've done has already or is
currently been done and so we just have to simply ignore it.

#### Sweeping the LSO of each Maatine

Sweeping the LSO of a maatine is the very last sweep in the gc for that maatine
and depends on that gc state of other maatines, when is to 

The gc of a maatine after its sweep phase where the memory of all unreachable
non-shared objects are reclame, it needs to wait until all the other maatine
have done the same before sweeping the LSO. 



#### The Mutator


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

# Maat concurrency

## Safe Points

* Creation of shared upvalues
* Marking a collectable object shared
* 


## Synchronization Points

(if more than one thread to execute Maat code)

* Check environment type(single or multi-threaded?)
* Shared short strings
* Cached strings
* Int -> Str cache
* Propagating over shared collectable objects.
* Insertion of shared objects into LSOs
* Shared memory operations (READ, WRITE, READ-MODIFY-WRITE(RMW))
* 

One of our major challenge is to avoid ruining performance by combining
bottleneck critical section, lots of processors, and inefficient spin-lock.
Memory shared accross maatines are via global symbols and upvalues, if
maatines run in a multi-threaded environment, Maat automatically syncs
concurrent access to shared memory using fast sync techniques.

We should detect all possible concurrent access to shared memory operations
and from there determine all synchronization points and apply them
effectively.

### Shared Objects

Immutable objects like strings, closures, ranges,... are thread-safe and
thus do not need any synchronization operation in their implementations
meanwhile mutable objects like Arrays, Maps, Insts need to in their
implementation detect if their environment is multi-threaded and if yes
then apply all the necessary synchronizations. This isn't easy at all
and can significantly affect the performance of our system.

The topic of how to efficiently implement thread-safe Arrays and Maps will
not be much discussed here though it counts a lot, we will rather focus
more on how to automatically detect the need to synchronize and perform
it without hindering the performance of our system.

Given the way we designed our [garbage collector](./gc.md), we can leverage
the colors set to collection objects during sharing points as a way to
determine in an object implementation if it's shared and if so then sync
its ops but this sync ever happens if we are running in a mutli-threaded
environment (that's at least two threads in the thread pool to schedule
execution of maatines).



### Variables



1. Write operation

```
x = 20
```

WRITE(X, 20)

2. Read operation

```
say x + 100
```

READ(X)

3. RMW (Read-Modify-Write) operation

```
let x = 20

ma for ^10 { x = x + 1 }
```
                -----+
READ(x)              |
CALL +, ARGS: 1      +-------> No other write/read on 'x' as long 
WRITE(x, REG)        |         as this still runs. Sync on 'x' has 
                -----+         nothing todo with sync on the value
                               stored in 'x' if ever that value is
                               shared & mutable.

In practice, the computation done between the READ and WRITE operation is
extremely lengthy so much that busy spin waiting strategies performed by
other threads trying to access the same critical section can lead to CPU
overloads.


```
x.push(3)
```

READ(X)
CALL push, ARGS: 3


## Communicate By Sharing: Channels

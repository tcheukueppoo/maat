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

Memory shared accross maatines are via global symbols and upvalues, Maat attemps
to automatically synchronize concurrent access to shared memory either through
mutexes or atomic operations.

Suppose we are in a multi-threaded environment and that variable `x` is accessible
to all maatines, having more than one threads to execution maat code implies more
then one maatine, let us determine all the synchronization points in each of the
operations below.

1. Write operation

```
x = 20
```

2. Read operation

```
say x + 100
```

3. RMW (Read-Modify-Write) operation

```
x = x + 1
```

```
x = [1, 2]

x.push(3)
```


## Communicate By Sharing: Channels

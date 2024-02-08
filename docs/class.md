# Classes

## Overview

## Class Artichecture

## Inheritances

Maat uses C3 mro as resolution order for resolving method calls, method
resolution is done during compilation which makes Maat pretty fast at
execution.

```

                (f(D))        (f(Z))
                 D ──────────► Z
                 ▲
                 │
                 │
                 │

                (f(B))
        ┌──────►  B ────────► E (f(E))
        │                    ▲
 (f(A)) A                    │
        │                    │
        │       (f(C))       │
        └──────► C ──────────┘
                 │
                 │
               │       (f(M))     (f(K))
                 └──────► M ───────► K
```

The above diagram shows the inheritance relationships between the classes
`A`, `B`, `C`, `D`, `E`, `M`, `K`, and `Z`. For example we have class `A`
which directly inherits from class `B` and `C` and indirectly inherits from
class `K`.

### Problem

We wish to resolve a method `f` when calling it from an object which is an
instance of `A`.

### Solution

Let `X` be a class belonging to the set of classes
`{A, B, C, D, E, M, K, Z}`, `X` **may** have its own implementation of `f`
denoted `f(X)`. By applying the C3 method resolution algorithm at
compilation, we use the divide and conquer approach to perform a breadth-first
traversal on the inheritance tree of `X`, this yields an array which contains
contains in C3 order pointers to all the implementations of `f` we found.
This array is recorded as the value stored at the entry of `f` in the method
hash table of X, the first element of this arrays is the top level method that
is going to be called during an `f` call by an object instance of `X`. The
rest of the elements are used to handle `__SUPER__` calls, especially when
they are chained across multiple `f` calls.

Let function R(X:f) denotes the `f` method resolution of class `X` that
returns the array we were talking of.

We have the following:

```
```

For an object instance of A, the following is true

```
...
```

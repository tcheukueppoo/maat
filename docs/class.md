# Classes

## Class Artichecture

(To Draw)

## Attributes

Having an `O(1)` access to an instance' field is critical for system's
performance since shared instances might require synchronization when maatines
in a multi-threaded environment concurrently access their fields which is
probably via a ticket-based spinlock from `librs`. This is why critical section
needs to be as small as possible. Class' fields are handled the same way we do
with lexical variables, i.e by allocating on a stack, but in this case it's each
class that holds it own fixed size non-growable stack. For this reason fields
can only be accessed internally/externally using auto-generated accessors. This
will imply that a field resolution is done at compile time, each field of an
instance has a unique index to a slot of a stack part of that instance. Setting
and getting the value of an field within an instance is respectively about
reading and writing to a particular slot.

The idea of compiling fields into a buffer to speed up access is quite
efficient but has drawbacks when dealing with inheritance and roles and not to
count the (significant?) slowdown at compilation.

### Resolving Role Attributes

The rule with roles in maat is that at composition we should not encounter two
fields have the same name and so if ever there is an field conflict when
composing a role with other role or class, the program reports an error.
Resolving attributes when a class does a role is done by simple joining their
two field buffers with zero conflicts, we keep the name of each fields in each
slot giving us the possibility to check conflicts and report them. The
composition is done at class creation which is at runtime.

Pseudo-code:

```
fn compose(A, R) {
   // Methods declared in R use this offset to
   // properly access their fields.
   offsets(A).append(len(A));
   for buf(R) -> r {
      for buf(A) -> a {
         if r.name == a.name {
            error "conflict";
         }
      }
      buf(A).append(r);
   }
}
```

```
for roles(A) -> R {
   next
     if already composed R in A or a role composed in A;
   compose(A, R);
}
```
When a role does another role, the same process described above happens, this is
why we performed the `if` conditional check to avoid rejoins which may cause
unwanted conflicts. Wow, this is three for-loops! does it impact performance?
Not at a huge scale!! The first loop iterates over the superclasses/roles which
in practice are quite small in number and at the end we have O(1) access to
fields which is very critical for concurrent code.

### Resolving Inherited Attributes

Instead of raising conflicts like we do in roles, fields of the derived class
take predecence over those from the based classes. If fields of the same name
that do not exist in the derived class coexist among the base classes, the most
recently encountered one takes precedence over the others. To avoid patching
bytecode of methods of these based classes to refer to fields that took prece
dence over their respective ones, we'll simply modify the buffer to refer to the
field that took precedence so that whenever the derived class calls methods
from the based classes, auto-generated accessors access the correct slots.

```
// Do this after you've already composed all the roles in A.
fn inherit(A, B) {
   
}
```

## Roles


## Inheritances

Maat implements c3 linearization, it c3 linearizes at compile time to speed
runtime execution of super method calls.

## Static Attributes and Method

You can declared lexically scoped variables in a class using `let`, this look
similar to static variables and methods in a class acesses these variables by
simply closing over them. There will be no over-head with static methods when
doing roles or inheriting class, only inherited methods of a base class can
access lexicals declared in superclasses.

```
class A {
   let x = 20;
   meth a {
      ++x;
      say x
   }
}

class B :is A {
   meth b { say x } // <-- error, 'x' is not declared.
}

B.new.b;
```
I believe it's rare to find yourself in a situation where you'd want `B.new.b`
to output `21` knowing that in principle static fields are bounded to the class
in which they were declared and it'll be awkward to also have to resolve
conflicts between static vars from superclasses, Well this is my logic of how OO
should be implement because other programming languages do it the reverse.
Declaring static variables with `let` in roles plays nicely with roles
themselves, especially when we don't want a role's field to take part in a
class-role or role-role composition.

```
role R {
   field c :rw = 2;
   meth say(s \\= "wooy!") { say join(' ', str(s) x c) }
}

class A :does R {}

A.new.say;
A.new(x => 3).say; // output: wooy! wooy! wooy!
```

```
role R {
   const let c = 2;
   meth say(s \\= "wooy!") { /* same code. */ }
}

class A :does R {}

A.new.say;         // output: wooy! wooy!
A.new(x => 3).say; // error, 'x' is not declared.
```

Variables in classes declared with `state` work exactly the same way with those
declared with `let`. That said, one could've leverage `state` to properly
implement static fields and method.

Lexical variables declared with `let` in a class are only accessible within it,
you'll have to define their accessors in order to access them from outside their
classes.


### What to know.

* A class can inherit (`:is`) classes and at the same time do (`:does`) roles.
* A class can do (`:does`) multiple roles and inherit (`:is`) multiple classes.
* A class can't inherit (`:is`) roles.
* A role can do (`:does`) other roles but never it does inherit (`:is`) a role
  or a class.


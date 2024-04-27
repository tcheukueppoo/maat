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

The idea of compiling fields into a buffer to speed up access is quite efficient
but has drawbacks when dealing with inheritance and roles.

## Roles

A Role

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
fn _compose(A, r) {
   let offset = len A;

   // Go through attributes of A to detect conflicts.
   for buf(r) -> f {
      if buf(A).first: .name == f.name {
         error "({r.name}, {A.name}): conflict with {f.name}";
         exit 0;
      }
      buf(A).append(clone f);
   }

   // Bind role methods and set offset for accessors
   // to correctly read/write to buf(A).
   for meths(r) -> m {
      let cm = clone m;
      cm.offset = offset;
      bind_to(A, m);
   }
}

fn compose(A, r) {
   _compose(A, r) if ! already_composed(A, r);
   for roles(r) -> r1 {
      _compose(A, r1) if ! already_composed(A, r1);
   }
}

for roles(A) -> r { compose(A, r) }
```

To reduce complexity, nothing actually happens when a role does other roles and
all the work is done when compositing roles in classes.

Meaning that

```
role R1 {}
role R :does R1 {}
class A :does R {}
```

is basically just doing

```
role R1 {}
role R :does R1 {}
class A :does R :does R1 {}
```

because `R1` is not really composed in `R` but rather registered in `R` so that
`R1` is later on composed in `A`.

Wow, this is a lot of for-loops! does it impact performance? Not really, not at
a huge scale!! The first loop from the entry point of the resolution process
iterates over the roles of `A` which in practice are quite small in number and
at the end we have O(1) access to fields which is very critical for concurrent
code.

## Inheritances

Maat implements c3 linearization, it c3 linearizes at compile time to speed
runtime execution of super method calls.

* Classes never inherit (`:is`) roles.
* A class can inherit (`:is`) classes and at the same time do (`:does`) roles.
* A class can do (`:does`) multiple roles and inherit (`:is`) multiple classes.
* A role can do (`:does`) other roles but never it does inherit (`:is`) a role
  or a class.

Instead of raising conflicts like we do in roles, resolution of an inherited
field is done by traversing the c3 linearization list of the derived class. The
first matching field is registered in the the derived class' field buffer and
accessors to this field is auto-generated and cached into the derived class to
optimize subsequent retrievals. Given that fields are resolved at compiled time,
its advisable to use roles or redefine fields instead of relying on inherited
fields as it may hinder runtime performance even worser than a first call to a
super method. All these slowdowns are not noticeable and probably insignificant
but it does not exclude the fact that we are constandly striving for an ideal
performance especially in a concurrent environment which is even why we decided
to compile time resolve access to fields.

Pseudo-code:

```
fn search_field(A, name) {
   return accessor(A, name) if exists_meth(A, name);

   for C3(A) -> i {
      for buf(i) -> i {
         next unless i.name == name;

         // Set default value
         buf(A).append(clone i);
         let accessor = { |v|
            state l = len(A);
            buf(A)[l] = v if v;
            return buf(A)[l]
         };
         cache_accessor(A, name, accessor);
         return accessor;
      }
   }

   return nil;
}
```

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

Lexically scoped variables declared with `let` in a class are only accessible
within it, you'll have to define their accessors in order to access them from
outside their classes.



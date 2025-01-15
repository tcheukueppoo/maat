# Classes

## Class Artichecture

(To Draw)

## Instance' Field

Having an `O(1)` access to an instance' field is critical for system's
performance since concurrent access to a field of a shared instance by maatines
in a multi-threaded environment requires synchronization probably via a
ticket-based spinlock from `librs`. This is why critical section needs to be as
small as possible. Class' fields are handled the same way as we handle lexical
variables, i.e by allocating on a stack, but in this case it's each class that
holds its own stack called a field buffer. For this reason fields can only be
accessed internal/external to its instance using accessors auto-generated at
compile-time. This will imply that a field resolution is done at compile-time,
each field of an instance has a unique index to a slot of a field buffer part of
that instance. Setting and getting the value of a field within an instance is
respectively about reading and writing to a particular slot.

The idea of compiling fields into a buffer to speed up access is quite efficient
but has drawbacks when dealing with inheritance and roles.

## Roles

Just like a class, a role constitutes of fields and methods, the main purpose
of roles is to describe a certain behavoir of objects. Roles favor code reuse
because objects of different classes can exhibit similar behaviors. Roles are
used by composing them with classes or even roles. The composition of a role
in a class is simply done by copying the fields and methods of that role in
that class.

### Composing Roles in a Class

A set of roles is said to be composable in a class if the following conditions
are met otherwise the program reports an error:

1. No duplicate fields should exist among participating roles, in other
   words none of the fields out of their buffers have identical names.

2. No common methods should exist among participating roles, the commonality
   isn't just measured based on methods' names, this means that multi-methods
   can be defined anywhere in the participating roles and each of them will be
   considered unique.

Composing a role in a class is simple done by properly binding the role's
methods to that class and joining their respectives field buffers with
zero conflicts. In a field's structure there is its name which aids us on
checking conflicts. Composition is done at runtime which is after each of these
entities have been allocated and initialized.

```maat
role R1         {}
role R :does R1 {}
```

To reduce complexity, we actually do not compose roles with roles, meaning that

```maat
class A :does R {}
```

is basically the same as doing

```maat
class A :does R :does R1 {}
```

because `R1` is not really composed in `R` but rather recorded in `R` so that
`R1` is later on composed in `A`. This implies that the relationship between
roles is represented as a directed graph and determining all the roles to be
composed in a class requires a graph traversal.

```
 A------+
        |
        +-----+-----+
        |     |     |
        v     v     v
        R1    R2--->R3------>R7
        |     |     |        |
        |     v     +-->R8<--+
        +---->R4    |
              |     |
              v     v
              R5--->R6

```

The above is a class `A` which does roles which are connected to a network of
other roles, I believe doing a graph traversal to determine all the roles that
need to be composed in `A` is a necessity

The above translates to the following Maat code:

```maat
role R6                            {}
role R8                            {}
role R7 :does R8                   {}
role R5 :does R6                   {}
role R4 :does R5                   {}
role R3 :does R6 :does R8 :does R7 {}
role R2 :does R4 :does R3          {}
role R1 :does R4                   {}
class A :does R1 :does R2 :does R3 {}
```

A naive traversal can seriouly hinder runtime performance because one of the
most important features of roles is it reusability, I might be trying to
prematurely optimize role composition but I think this risk is worth taking.

When a role does other roles, we could do more than just referencing, we could
compute the graph traversal of that role and record it in its internal struture
so that any other role/class that does it can use the result to compose it in a
class or as a subsolution to compute other traversals, this is more of a
dynamice programming approach which turns out to be viable and efficient. This
also looks similar to the c3-linearization algorithm.

For the above relationship diagram, we have:

```
Traverse(R6) =                                                   [R6]
Traverse(R8) =                                                   [R8]
Traverse(R7) = R7 + Traverse(R8)                               = [R7, R8]
Traverse(R3) = R3 + Traverse(R6) + Traverse(R8) + Traverse(R7) = [R3, R6, R8, R7]
Traverse(R5) = R5 + Traverse(R6)                               = [R5, R6]
Traverse(R4) = R4 + Traverse(R5)                               = [R4, R5, R6]
Traverse(R2) = R2 + Traverse(R4) + Traverse(R3)                = [R2, R4, R3, R5, R6, R8, R7]
Traverse(R1) = R1 + Traverse(R4)                               = [R1, R4, R5, R6]
Traverse(A)  =      Traverse(R1) + Traverse(R2) + Traverse(R3) = [R1, R2, R3, R4, R6, R5, R8, R7]
```

This looks good because there's no need of using an external data structure as
dynamic table to keep track of the sub-solutions.

Pseudo-code:

```maat
fn linearize_roles_of(R) {
   let role_list = roles(R);

   // If R does only one role
   return linearized_roles(role_list[0]) if role_list.len == 1;

   let result  = [];
   let max_len = role_list.map(:.len).max;

   loop (let i = 0; i < max_len; i++) {
      let cands = role_list.map(:linearized_roles(_)).grep(:.len <= i).map: _[i];
      result.push(_) unless result.has(_) for cands;
   }

   return result;
}
```

No worries on the performace of `.map` as in practice `role_list.len` is not
even greater than `5` but Wooooy! We still have these two loops! does it impact
performance? Not really, not at a huge scale!! The merging process is done in a
way that the cost of linearization is propagated over the nodes of the graph
which reduces pauses at runtime because having to traverse the whole graph for
each class can potentially introduces pauses since the computation has nothing
to do with the program's main goal.

The real work is not yet done, we need to compose! By taking into consideration
the composition rules described above, each role found in the lineariezation
list has its method bound to the targeting class and its buffer joined with the
class' buffer.

We can leverage stubs to define abstract interfaces in roles and expect
classes doing these roles or other roles to be composed in those classes to
provide their implementation but in all cases, it is methods defined in a class
that takes precedence over those defined in the roles to be composed in that
class even if they are just stubs.

```
fn bind_to(A, m, offset) {
   let cm = offset ? clone m : m;

   cm.offset = offset;
   bind(A, cm);
}

fn compose(A) {
   let (stub, debug) = (0, []);

   for linearize_roles_of(A) -> r {
      let offset = len buf(A);

      // Bind methods of 'r' to 'A'
      for meths(r) -> m {
         let Am = fetch_meth(A, m.name);

         if defined Am {
            die "{A.name}: conflict with method {m.name} in {r.name}"
              if is_role_meth(Am) && !is_stub(Am);
            if !is_stub(m) && is_role_meth(Am) && is_stub(Am) {
               stub--;
               bind_to(A, m, offset);
            }
         }
         else {
            if is_stub(m) {
               stub++;
               debug.push([r.name, m.name]);
               bind_to(A, m);
            }
            else {
               bind_to(A, m, offset);
            }
         }
      }

      // Join field buffers of 'r' with that of 'A'
      for buf(r) -> f {
         die "{A.name}: conflict with field {f.name} in {r.name}"
           if buf(A).first: f.name = .name;
         buf(A).append(f);
      }
   }

   if stub {
      die <<~EOE;
       {A.name}: The following methods may have not been implemented:
          {debug.dump}
      EOE
   }
}
```

Having to iterate over `buf(r)` to check conflicts reveals how computationally
expensive this solution is but since identifiers are mostly short strings,
they'll probably get internalized and thus testing for string equality fallbacks
to pointer equality.

## Inheritances

Maat implements c3 linearization, it c3 linearizes at runtime time by using the
dynamic programming approach.

* A Class cannot inherit (`:is`) roles.
* A class can inherit (`:is`) classes and at the same time do (`:does`) roles.
* A class can do (`:does`) multiple roles and inherit (`:is`) multiple classes.
* A role can do (`:does`) roles but never it does inherit (`:is`) a role or a
  class.

Instead of raising conflicts like we do in roles, resolution of an inherited
field is done by traversing the c3 linearization list of the derived class. The
first matching field is registered in the derived class' field buffer and
accessors to this field is auto-generated and cached into the derived class to
optimize subsequent recalls. Given that access to fields of a class' instance is
resolved at compile-time, it is advisable to use roles or redefine fields in
the derived class instead of relying on inherited fields as it may hinder
runtime performance even worser than a first call to a super method. All these
slowdowns are not noticeable and probably insignificant but it does not exclude
the fact that we are constantly striving for an ideal performance especially in
a concurrent environment which is even why we decided to compile-time resolve
access to fields.

Pseudo-code:

```maat
fn search_field (A, name) {
   return accessor(A, name) if exists_meth(A, name);

   for C3(A) -> i {
      for buf(i) -> i {
         next unless i.name == name && i.priv == false;

         // Set default value
         buf(A).append(clone i);
         let l = len(A) - 1;
         let accessor = { |v|
            return __NARGS__ == 1 ? buf(A)[l] = v : buf(A)[l];
         };
         bind_accessor(A, name, accessor);
         return accessor;
      }
   }

   return nil;
}
```

Supercalls are also resolved by walking down the c3-mro list to find their
entries and if found, we cache the results in the mro cache of the object's
class so as to optimize subsequent recalls by the same object or any other
object instance of that class. The `^.augment` meta-method call on a class can
invalidates that class' mro cache and all of that of the affected classes, i.e
the ones it's directly or indirectly linked to and therefore it's preferable you
monkey-patch a class before creating any instance of it.

## Static Attributes and Methods

You can declared lexically scoped variables in a class using `let`, this looks
similar to static variables, methods in a class have access to these variables
by simply closing over them just like normal closures. There will be no
over-head with static methods when composing with roles or inheriting classes,
only inherited methods of a base class can access lexicals declared in
superclasses.

```maat
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
to output `21` while knowing that in principle a static field is bounded to the
class in which is was declared and it'll be awkward to also have to resolve
conflicts between static fields from superclasses. I'm saying so based on maat's
own view of OO programming as implementations differs across programming
languages. Declaring static variables with `let` in roles plays nicely with
roles themselves, especially when we don't want a role's field to take part in a
class-role composition.

```maat
role R {
   field c :rw = 2;
   meth say(s \\= "wooy!") { say join(' ', str(s) x c) }
}

class A :does R {}

A.new.say;
A.new(x => 3).say; // output: wooy! wooy! wooy!
```

```maat
role R {
   const let c = 2;
   meth say(s \\= "wooy!") { /* same code. */ }
}

class A :does R {}

A.new.say;         // output: wooy! wooy!
A.new(x => 3).say; // error, 'x' is not declared.
```

Variables in classes declared with `state` work exactly the same way as those
declared with `let`. That said, one could've leverage `state` to properly
implement static fields and methods.

Lexically scoped variables declared with `let` in a class are only accessible
within it, you'll have to define their accessors in order to access them from
outside their classes which is less common in practice as `field` is there!

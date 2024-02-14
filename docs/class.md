# Classes

## Class Artichecture

(To Draw)

## Attributes

Having an `O(1)` access to an instance' attribute is critical for system's
performance since shared instances might require synchronization when
maatines in a multi-threaded environment concurrently access their
attributes probably via a ticket-based spinlock from `librs`. This is why
critical section needs to be as small as possible. Class' attributes are
private and can only be externally accessed using accessors auto-generated
with the help of traits, it is so because attribute resolution is done at
compile time where each attribute of an instance has a unique index to a
slot of an attribute buffer part of that instance. Setting and getting the
value of an attribute within an instance is respectively about reading and
writing to a particular slot.

The idea of compiling attributes into a buffer to speed up access is quite
efficient but has drawbacks when dealing with inheritance and roles.

### Resolving Role Attributes

Roles clearly state that if ever there is an attribute conflict when
composing roles with other roles or classes, then the program reports an
error. A class doing a role simple implies joining their two attribute
buffers with zero conflicts, we keep the name of each attributes in each
slot so that this gives us the possibility to check conflicts and report
them. The composition is done at class creation which is at runtime.

Some pseudo-codes:

```
A: A class
R1, R2, R3: Roles 'A' does
R2: Roles 'R1' does

At creation of A:
   # Applying roles' attributes
   # Go through the list of roles 'A' has to do
   Al: list of already applied roles
   for R1, R2, R3 -> r {
      if Al.has(r) { next } # (p1)
      join_buffer_of_A_with_that_of_r(buf(A), buf(r))
      Al.push(r)
      Al.push(roles_r_does)
   }
```

Since each role and 

```
# join two attribute buffer
fn join_buffer_of_A_with_that_of_r(buf_A, buf_r) {
   for buf_r -> i {

      # buf_A.len is the offset of 
      # 

      for buf_A -> j {
         if j.attribute_name == i.attribute_name {

            # check if the attribute wasn't defined in class 'A' and if
            # so then it means we just found the role that did define it.
            if j.type == empty_reference {
               j.type = reference

               # there's no backward referencing with roles
               # 'v' is a union of 'value_ref' and 'value'
               i.v.delta = j.type == reference ? j.v.value_ref : &j.v.value
            }
            else {
               error "conflict"
            }
         }
         else {
            buf_A.push(j)
         }
      }
   }
}
```

When a role does other roles, the same process described above happens
where `A` is a role, this is why we perform the `if` conditional check
(`p1`) to avoid rejoins which may cause unwanted conflicts.

### Resolving Inherited Attributes

Resolving inherited attributes is a little bit more different, if any based
class has an attribute already declared in the derived class, that derived
class' attribute takes precedence over the one from the based class, meaning
that its initial value will be used at instanciation. In cases the derived
class hasn't already defined any of the based class attributes, only the
attribute encountered earlier is used and the rest redeclared from other
based classes refere to the former one.





```
A: The derived class
B, C, D: Base classes A inherits

At creation of A:
    // After you've resolved roles' attributes
    //
```



## Roles


## Inheritances

Maat implements c3 linearization, it c3 linearizes at compile time to
speed runtime execution of super method calls.


### What to know.

* A class can inherit (`:is`) classes and at the same time do
  (`:does`) roles.
* A class can do (`:does`) multiple roles and inherit (`:is`)
  multiple classes.
* A class can't inherit (`:is`) roles.
* A role can do (`:does`) other roles but never it does inherit
  (`:is`) a role or a class.



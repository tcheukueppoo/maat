```
package A

our x = 2
class B { ... }

package A::B

our y = 2
class B :p :is(...) { ... }

package main
# x

let a = A::B.new     # ref 'B' in package 'A'
let a_b = A:::B.new  # ref 'B' in package 'A::B' because of the ':p' trait
```

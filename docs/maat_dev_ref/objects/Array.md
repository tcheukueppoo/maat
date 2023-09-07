# Array

`Maat` has a very tiny type system with the `Array` type inheriting directly from the `Any` type.

## Declaration

```
-- Yep, we have multiple ways of declaring an Array object
var a = a<liza madjou monthe>
var b = ["kueppo", "cyrus", "christ"]
var c = Array.new(2, 3, "four", a<5 6>)

.say for a.lmap(.split) -- a{l i z a m a d j o u m o n t h e}
.say for a.map(.split) -- a{l i z a}, a{m a d j o u}, a{m o n t h e}
```

## Methods

(**Note that parenthesis are Always optional in method calls**)

Let `a` be a variable containing an object of type `Array`, `a` has the following
methods:

- `a.len() -> Num`: Return the number of elements in `a`.
- `a.pop([ Any n ]) -> Any | Array`: If `n` is omitted, pop last element from `a` and return it else pop `n` elements from it and return them in an Array. Fail if array is emty.
- `a.push(Any x [, Any y, ... ]) -> Array`: Push into `a` elements passed as arguments to `push` into `a`. return the modified array.
- `a.append(Any x [, Any y, ... ]) -> Array`: Same as `push` except that it flattens arrays to append them invidually instead of appending as a single unit. Return the modified array.
- `a.map()`:
- `a.lmap()`:
- `a.grep()`:

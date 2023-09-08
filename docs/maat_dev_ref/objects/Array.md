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

### Key notes

- Parenthesis in method and function calls are always optional
- Maat is dynamically and at the same time statically type and thus type errors aren't raised unless necessary
- `->` Specifies the return type
- `|` Stands for alternation, so for example `-> Num | Array` means to return either a `Num` or an `Array`
- `[]` Simply means the rest of the arguments are optional
- `EXP`: represents an expression, see maat's BNF for what is an expression

### Note

Let `a` be a variable containing an object of type `Array`, `a` has the following
methods:

- `a.len -> Num`: Return the number of elements in `a`
- `a.of()`: Tell us `of` what type are the elements of `a`
- `a.clone()`: Clone object in `a` into a new fresh one and return it
- `a.min -> Any`: Return the minimum element in `a` based on numeric comparison
- `a.minstr -> Any`: Return the minimum element in `a` based on stringwise comparison
- `a.max -> Any`: Return the maximum element in `a` based on numeric comparison
- `a.maxstr -> Any`: Return the maximum element in `a` based on stringwise comparison
- `a.cmp(Array b) -> Num`: 
- `a.end -> Any`: Return the index of the last element in `a`
- `a.minmax -> Array`: Return two element in the list, first the the maxinum and second in the minimum both in `a` (Completely different from infix `minmax` operator)
- `a.minmaxstr -> Array`: The stringwise version of `a.minmax`
- `a.keys -> Array`: Return an Array of indexes of `a`
- `a.kv -> Map`: Return a created Map from elements of `a`, abort if `a.len` is odd.
- `a.ikv -> Map`: Return the Map version of `a` with indexes as keys
- `a.rev -> Array`: Return the list in reverse order
- `a.join([ Str sep ]) -> Str`: Stringyfy elements in `a` and interleave them with the separator `sep` or empty string if not specified
- `a.pop([ Num | Str n ]) -> Num | Array`: If `n` is omitted, pop last element from `a` and return it else pop `n` elements from it. Fail if array is emty.
- `a.push(Any x [, Any y, ... ]) -> Array`: Push into `a` elements passed as arguments to `push` into `a`. return the modified array.
- `a.append(Any x [, Any y, ... ]) -> Array`: Same as `push` except that it flattens arrays to append them invidually. Return the modified array.
- `a.shift([ Num | Str n ])`: 
- `a.unshift([ Num | Str n ])`: 
- `a.prepend() -> Array`: 
- `a.del(Num | Str n)`:
- `a.splice() -> Array`: 
- `a.flat() -> Array`: flattens 
- `a.map(Fun f | Exp) -> Array`: run function F or evaluate expression for each elements in `a` and collect the return values into an Array
- `a.lmap(Fun f | Exp) -> Array`: Same as `a.map` but `.flat` the result return it
- `a.grep(Fun f | Exp) -> Array:
- `a.last_by(Fun f | Exp) -> Any`:
- `a.first_by(Fun f | Exp) -> Any`:
- ``

# Array

`Maat` has a very tiny type system with the `Array` type inheriting directly from the `Any` type.

## Declaration

```
-- Yep, we have multiple ways of declaring an Array object
var a = a<liza madjou monthe>
var b = ["kueppo", "cyrus", "christ"]
var c = Array.new(2, 3, "four", a<5 6>)

.say for a.lmap(.split) -- a{l i z a m a d j o u m o n t h e}
.say for a.map(.split)  -- a{l i z a}, a{m a d j o u}, a{m o n t h e}
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
- `a.of -> ` Tell us `of` what type are the elements of `a`
- `a.clone`: Clone object in `a` into a new fresh one and return it
- `a.min -> Any`: Return the minimum element in `a` based on numeric comparison
- `a.minstr -> Any`: Return the minimum element in `a` based on stringwise comparison
- `a.max -> Any`: Return the maximum element in `a` based on numeric comparison
- `a.maxstr -> Any`: Return the maximum element in `a` based on stringwise comparison
- `a.end -> Any`: Return the index of the last element in `a`
- `a.minmax -> Array`: Return an `Array` of two elements, first and second is the max and min in `a`
- `a.minmax_op(Array | Set | MSet | Bag | MBag b) -> Array`: Return an `Array` of two elements, first and second are the max and min in `a` and `b` resepctively
- `a.minmax_op_str(Array | Set | MSet | Bag | MBag b) -> Array`: Stringwise version of `a.minmax_op`
- `a.minmaxstr -> Array`: The stringwise version of `a.minmax`
- `a.keys -> Array`: Return an Array of indexes of `a`
- `a.kv -> Map`: Return a new `Map` object elements of `a`, abort if `a.len` is odd
- `a.ikv -> Map`: Return in a new object the `Map` version of `a` with indexes as keys
- `a.rev -> Array`: Return a new array which contains reversed elements of `a`
- `a.join([ Str | Num sep ]) -> Str`: Stringyfy elements in `a` and interleave them with the separator `sep` or empty string if not specified
- `a.pop([ Num | Str n ]) -> Num | Array`: `n` defaults do `1` if omitted. At the end, pop and return `n` elements from `a`, as a `Num` if n is 1 else as an `Array`
- `a.push(Any x [, Any y, ... ]) -> Array`: At the end, push into `a` elements passed as arguments to `push`
- `a.append(Any x [, Any y, ... ]) -> Array`: Same as `push` except that it flattens arrays to append individual elements
- `a.shift([ Num | Str n ])`: Same as `a.pop` but does it at the front
- `a.unshift([ Num | Str n ])`: Same as `a.push` but does it at the front
- `a.prepend() -> Array`: Same as `a.append` but does it at the front
- `a.del(Num | Str n)`: Delete element at index `n`, fail if index does not exist in that array
- `a.splice([ Num | Str start ] [, Num | Str end ] [, Array | Bag | MBag | Set | MSet b ]) -> Array`: 
- `a.flat() -> Array`: It flattens the Array object so that all the elements found in nested array end up direct elements of `a`
- `a.map(Fun f | Exp) -> Array`: Run function `f` or evaluate expression `Exp` for each elements in `a` and collect their return values into an array
- `a.lmap(Fun f | Exp) -> Array`: Same as `a.map` but `.flat` the result return it
- `a.grep(Fun f | Exp) -> Array`: Returns elements in `a` for which it evaluation by `f` or `Exp` return a true value
- `a.last_by(Fun f | Exp) -> Any`:
- `a.first_by(Fun f | Exp) -> Any`:
- `a.cmp(Array b) -> Num`: 
- `a.print -> Bool`:
- `a.say`:
- `a.dump`:

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
- `[]` Surrounding an argument simply means the argument is optional
- `Expr`: represents an expression, see maat's BNF to know what is an expression

Let `a` be a variable containing an object of type `Array`, `a` supports the following methods:

- `a.len -> Num`: Return the number of elements in `a`
- `a.of -> Str`: Tell us `of` what type are the elements of `a`
- `a.clone -> Array`: Clone object in `a` into a new fresh one and return it
- `a.min -> Any`: Return the minimum element in `a` based on numeric comparison
- `a.minstr -> Any`: Return the minimum element in `a` based on stringwise comparison
- `a.max -> Any`: Return the maximum element in `a` based on numeric comparison
- `a.maxstr -> Any`: Return the maximum element in `a` based on stringwise comparison
- `a.minmax -> Array`: Return an `Array` of two elements which are the max and min in `a` respectively.
- `a.minmaxstr -> Array`: The stringwise version of `a.minmax`
- `a.minmax_op(Array | Set | MSet | Bag | MBag b) -> Array`: Return an `Array` of two elements, first and second are the max and min in `a` and `b` resepctively
- `a.minmax_op_str(Array | Set | MSet | Bag | MBag b) -> Array`: Stringwise version of `a.minmax_op`
- `a.compact -> Array`: Remove all `nil` values in `a`
- `a.uniq([ Fun f | Expr ]) -> Array`: Return a new Array object without duplicate elements
- `a.first(Fun f | Expr)`: Return the first element for which its evalution be `f` or `Expr` returns a true value
- `a.head(Str | Num n = 1) -> Any | Array`: Return the first `n` elements of `a` if `n >= 1` or the first `n - 1` elements if `n <= 0`. Fail is `a.is_empty`
- `a.tail(Str | Num n = 1) -> Any | Array`: Same as `a.head` except that it  performs on the last elements of `a`
- `a.shuf(str | Num n = 1) -> Array`: Shuffle elements in `a` `n` times and return `a`
- `a.end -> Any`: Return the index of the last element in `a`
- `a.keys -> Array`: Return an Array of indexes of `a`
- `a.hash -> Map`: Return a new `Map` object with elements from `a`, abort if `a.len` is odd
- `a.ihash -> Map`: Return in a new object the `Map` version of `a` with indexes as keys
- `a.rev -> Array`: Return a new array which contains reversed elements of `a`
- `a.join([ Str | Num sep = '' ]) -> Str`: Stringyfy elements in `a` and interleave them with the separator `sep`
- `a.pop([ Num | Str n = 1 ]) -> Num | Array`: At the end, pop and return `n` elements from `a`, as a `Num` if n is 1 else as an `Array`
- `a.push(Any x [, Any y, ... ]) -> Array`: At the end, push into `a` elements passed as arguments to `push`
- `a.append(Any x [, Any y, ... ]) -> Array`: Same as `push` except that it flattens arrays to append individual elements
- `a.shift([ Num | Str n ])`: Same as what `a.pop` does but does it at the front
- `a.unshift([ Num | Str n = 1 ])`: Same as what `a.push` does but does it at the front
- `a.prepend() -> Array`: Same as what `a.append` does but does it at the front
- `a.del([ Num | Str n ])`: Delete element at index `n` or deleta, fail if index does not exist in `a`
- `a.splice([ Num | Str s = 0 ] [, Num | Str l =  ] [, Array | Bag | MBag | Set | MSet b ]) -> Array`: Remove and return `l` elements from `a`, start at `s`, replace them by `b` if any
- `a.flat -> Array`: It flattens the Array object so that all the elements found in nested array end up direct elements of `a`
- `a.map(Fun f | Expr) -> Array`: Run function `f` or evaluate expression `Expr` for each elements in `a` and collect their return values into an array
- `a.lmap(Fun f | Expr) -> Array`: Same as `a.map` but `.flat` the result before returning it
- `a.grep(Fun f | Expr) -> Array`: Returns elements in `a` for which their evaluations by `f` or `Expr` return a true value
- `a.each(Fun f | Expr) -> Array`:
- `a.each_kv(Fun f | Expr) -> Array`:
- `a.each_ikv(Fun f | Expr) -> Array`:
- `a.cmp(Array b) -> Num`: 
- `a.print -> Bool`:
- `a.say -> Bool`:
- `a.dump -> Bool`:


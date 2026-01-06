## Num

The `Num` type for an immutable literal value represents any binary, octal, hexadecimal and double-precision floating point number, it inherits directly from
the `double` C programming language data type or any of its variant depending on how the Maat project was compiled.

`∞` or `Inf` is a object of type `Num`. Even though in mathematics `∞` is not considered a number, every object in the Maat type system should have a type.

Here, we explore all the properties and operations involving infinity across different mathematical contexts:

```
let a;

∞ + ∞ == ∞

a = -5;
∞  + a == ∞;     // true, where `a' is finite
-∞ + a == -∞;    // true

-∞ - ∞ == -∞;    // true
∞  - ∞ === NaN;  // true
∞  - ∞ == NaN;   // false

∞ * ∞ == ∞;      // true
∞ * a == ∞;      // true
∞ * 0 == NaN;    // true
a = 5;
∞ * (-a) == -∞   // true, where a > 0

a / ∞ == 0       // true
∞ / a == ∞       // true
∞ / ∞ === NaN    // true
0 / 0 === NaN    // true

 ∞ > a // true
-∞ < a // true

∞.Bool // --> true
```

Let `i` be a variable whose value is an object of type `Num`, `i` supports the following public methods:



## `.times`





## Num

The `Num` type for an immutable literal value represents any binary, octal, hexadecimal and double-precision floating point number, it inherits directly from
the `double` C programming language data type or any of its variant depending on how the Maat project was compiled.

`∞`/`Inf` and `NaN` are objects of type `Num`. In mathematics, even though `∞` is not considered a number, every object in the Maat type system is expected to
have type.

Here, we explore all the properties and operations involving infinity across different mathematical contexts:

```
let a;

∞ + ∞ == ∞

a = -5;
∞  + a == ∞;     // true, where `a' is finite
-∞ + a == -∞;    // true

-∞ - ∞ == -∞;    // true

∞ * ∞   == ∞;    // true
-∞ * -∞ == ∞;    // true
∞ * -∞  == -∞;   // true
∞ * a   == ∞;    // true
a = 5;
∞ * (-a) == -∞   // true, where a > 0

a / ∞ == 0;      // true
∞ / a == ∞;      // true
a = -10;
∞ / a == -∞;     // true, where a < 0
a / 0 == -∞;     // true, where a < 0
a = 12;
a / 0 == ∞;      // true, where a > 0

0 ** ∞ == ∞;     // true

 ∞ > a // true
-∞ < a // true

// Indeterminate forms

∞ - ∞ == NaN;    // false
∞ - ∞  === NaN;  // true
∞ / ∞  === NaN;  // true
0 / 0  === NaN;  // true
∞ * 0  === NaN;  // true
∞ ** 0 === NaN;  // true
1 ** ∞ === NaN;  // true
0 ** 0 === NaN;  // true

// T

// Hyperbolic functions
∞.sinh;  //  ∞
-∞.sinh; // -∞
∞.cosh;  //  ∞
-∞.cosh; //  ∞
∞.tanh;  //  1
-∞.tanh; // -1

// Exponentials and Logarithms

a = 10;
a ** ∞  == ∞ // true, for a > 1
a ** -∞ == 0 // true, for a > 1

a 


∞.Bool // true
```

Let `i` be a variable whose value is an object of type `Num`, `i` supports the following public methods:



## `.times`





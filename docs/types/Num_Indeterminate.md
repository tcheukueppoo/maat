# Proof The Basic Indeterminate Forms

To show that an operation involving infinity is indeterminate (`NaN`), we need to show how different limits of the variants of that operation that maintains
the same form, diverges to different results. For the simplest indeterminate forms, we are going to use examples that show variety in results.

> **NB**: The below is not maat code!

## The 7 Indeterminate forms

1. `∞ - ∞`, e.g as `n -> ∞`, `n - n -> 0` while `n - (n - 1) -> 1`
2. `0 * ∞`, e.g as `n -> ∞`, `(1 / n) * n -> 1` while `(2 / n) * n -> 2`
3. `∞ / ∞`, e.g as `n -> ∞`, `n / n -> 1` while `(n ** 4) / n -> ∞`
4. `1 ** ∞`, e.g as `n -> ∞`, `(1 + (1 / n)) ** n -> e` while `(1 + (2 / n)) ** n -> e ** 2`
5. `0 / 0`, e.g as `n -> 0`, `n / (n ** 2) -> ∞` while `(n ** 2) / n -> 0`
6. `∞ ** 0`

`∞ ** 0` is an example expression of the form `f(n) ** g(n)` where `f(n)` grows to `∞` and `g(n)` shrinks to zero as `n -> ∞`.
Proving that `∞ ** 0` is `NaN`, it suffices to prove that `f(n) ** g(n)` can take any non-negative real depending on the precise
rate at which the base and the exponent approach their limits.

Let's write `f(n) ** g(n)` as an exponential of a logarithm:

`f(n) ** g(n) = e ** (g(n) * ln(f(n)))`

As `n -> ∞`, we know that `g(n) -> 0`. `ln(f(n)) -> ∞` and thus `g(n) * ln(f(n))` is indeterminate and as a result `f(n) ** g(n)` is indeterminate.

7. `0 ** 0`

---
title: The Maat Programming Language Specification
author: Kueppo Tcheukam J. W.
email: tcheukueppo@yandex.com
---

# Maat 

`Maat` is a multi-paradigm general purpose programming language that empowers
programmers to easily build fast and scalable applications.

Key features of the Maat programming language:

* Functional Programing
* Object Oriented Programming
* Multiple Dispatching
* Closures
* Concurrency with [Works](./types/Work.md) and [Maatines](./types/Ma.md)
* Traits
* Lazy Evaluation
* Regular Expressions using PCRE2
* Full Unicode support
* Memoization

This is the specification of the Maat programming language, it is written as a
guidance for its implementation.

# Operators

This is the list of all operators supported by the Maat programming language.

## Lonely operator

* `…`, `...`: to specify unimplemented code

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

* `++`: **(p,b)** Incrementation operator.
* `--`: **(p,b)** Decrementation operator.
* `-`: **(b)** Negation operator.
* `+`: **(b)** Positive operator, result of the evaluation is equal to the operand.
* `~`: **(b)** Binary complement.
* `*`: **(b)** Array destructor/accumulator operator.
* `^`: **(p)** Range operator, `^5` returns a `Range` object, i.e `Range.new(0, 5)`.
* `√`: **(p)** Sqaure root operator.
* `⁰`, `¹`, `²`, `³`, `⁴`, `⁵`, `⁶`, `⁷`, `⁸`, `⁹`: **(b)** Power operators.
* `Σ`: **(p)** Summation operator.
* `Π`: **(p)** Product operator.

## Named unary operators

* `defined`: **(b)** Check if a variable is `nil`.
* `sleep`: **(b)** Sleep the current Maatine for a given amount of time.
* `return`: **(b)** Return from a function.
* `assert`: **(b)** Test an assertion.
* `assert_eq`: **(b)** Equality test assertion.
* `exit`: **(b)** Exit program with given exit code.

## Named list operators

* `say`: **(b)** Print to the standard output with a trailing new line.
* `print`: **(b)** Print without a new line.
* `printf`: **(b)** Print formatted string.
* `sprintf`: **(b)** Sprintf, return formatted string.
* `join`: **(b)** Function version of `.join` method of the `Array` type.
* `map`: **(b)** Function version of `.map` method of the `Array` type.
* `lmap`: **(b)** Function version of `.lmap` method of the `Array` type.
* `grep`: **(b)** Function version of `.grep` method of the `Array` type.
* `run`: **(b)** Run the passed as argument commands with their parameters on a new shell.
* `die`: **(b)** Raise an exception and exit program if there is no handler.
* `warn`: **(b)** Warn by sending a message to the stderr.

## Binary operators for maat objects

* `=`: **(i)** Assignment operator.
* `.`: **(i)** Method call operator on objects and classes.
* `.^`: **(i)** Method call operator for meta-programming.
* `,`: **(i)** Comma separator operator.
* `=>`: **(i)** Key-value separator operator.
* `!`: **(p)** Negation operator ex: `!true == false`.
* `//`: **(i)** A variant of the `||` operator that tests for definedness, `a // b`, returns `a` if it isn't `nil` otherwise `b`.
* `==` / `⩵`, `!=` / `≠`, `>`, `>=` / `≥`, `<`, `<=` / `≤`: **(i)** Basic infix operators.
* `+`, `-`, `/` / `÷`, `*`,  `%`, `..` / ``: **(i)** Add, sub, div, mul, remainder and range operator.
* `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `%=`: **(i)** `X op= Y` same as `X = X op Y`.
* `<<`, `>>`, `^`, `&`, `&=`, `|=`: **(i)** Bitwise shift on left and right, logical `or` and `and`.
* `&&`, `||`, `&&=`, `||=`: **(i)** Logical "and" and "or" operator.
* `≅` / `=~`, `~~`: **(i)** Regex operator and smartmatch operator.
* `∉`,  `∈`, `∊`, `∍`, `∋`, `∌`, `⊂`, `⊄`, `⊆`, `⊈`, `⊃`, `⊅`, `⊇`, `⊉`, `≡`, `≢`, `⊖`, `∩`, `⊍`, `∪`, `⊖`, `⊎`, `∖`: **(i)** Set operators.
* `<=>` : **(i)** `X <=> Y` says yield `-1` if `X < Y`, `0` if `X == Y`, or `-1` if `X > Y`.
* `∘`: **(i)** Function composition infix operator.
* `?:`: **(?)** Tenary operator.

## List of all operators from highest precedence to lowest

* terms and list operators (leftward): left
* grouping operator `( )`: right
* method call operator `.`: left
* `++`, `--`, `√` and unary prefix `…` / `...`: nonassoc
* `**`, `⁰`, `¹`, `²`, `³`, `⁴`, `⁵`, `⁶`, `⁷`, `⁸`, `⁹`: right
* `!`, `~`, `\` and unary `+` and `-`: right
* `=~`, `!~`: left
* `*`, `/`, `%`: left
* `+`, `-`, `.`, `∘`: left
* `∩`, `⊍`: left
* `∪`,  `⊖`, `⊎`, `∖`: left
* `∈`, `∊`, `∉`, `∋`, `∍`, `∌`, `≡`, `≢`, `⊂`, `⊄`, `⊃`, `⊅`, `⊆`, `⊈`, `⊇`, `⊉`, `≼`, `≽`: chained
* `<<`, `>>`: left
* named unary operators: nonassoc
* `isa`: nonassoc
* `<`, `>`, `<=` / `≤`, `>=` / `≥`: chained
* `==`, `!=`, `<=>`, `~~`: chain/na
* `&`: left
* `|`, `^`: left
* `&&`: left
* `||`, `//`: left
* `..`, lonely operator `…` / `...`: nonassoc
* `?:`: right
* `=`, `:=`, `&=`, `|=`, `&&=`, `||=`, `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `.=`, `%=`, `last`, `break`, `redo`, and `dump`: right
* `,`, `=>`: list

# Comments

Use `#` for single line comments and `---` for multi-line comments.

```
# single line comment
---
a
multi
line
comment
---
```

# Delimiters

## Pair delimiters

The following pair delimiters can be used as delimiters for regexes values
(`@r`) and operators and other quoted values like strings (`@q`, `@Q`, and
`@x`), arrays (`@a`) and maps (`@m`).

```
() [] {} <> «» ‹›
```

### Examples

```
# Could still for example use @a{one two three}
let x = ['one', 'two', 'three']

# [ "Three", "Two", "One" ]
let b = x.map(:.cap).rev

# [ "0ne", "tw0", "three" ]
x =~ s<o>«0»
```

## Single character delimiters

We also have a restricted set of single character delimiters for quoted values
and regex operators.

```
/ `` | % " ' , $ !
```

### Examples

```
# same as ['ONE', 'TWO', 'THREE']
let a = @a|ONE TWO THREE|;
a.each: .lc.say;

say @q/interpolation won't work/;

say "interpolation works, array: #{a}";

# [ "0ne", "Tw0" ]
a.grep({|x| x =~ m|o|i }).map(:s|o|0|r).map(:.ucfirst).say;
```

The `|`s in `|x|` has nothing to do with single character delimiters, it
part of the anonymous function syntax.

# Variables

`Maat` has four types of variables: package, lexical, temporary and static
variables.

A module file has a `.mm` extension, a package is a namespace, a variable that
can be accessed from outside the package from which it was declared is called a
package variable and this can only be done by using the variable's fully
qualified name. There is no one-to-one mapping between a package and module
file, therefore multiple packages can be defined in a single module file but
this is usually not a good practice.

Package variables can be accessed from other packages using their fully
qualified name and lexically scoped variables cannot be accessed from outside
the package in which they were declared.

A temporary variable is a variable that is lexically scoped and refers to a
package variable declared from another package if the name of the temporary
variable at declaration is fully qualified, otherwise it refers to a package
variable of the same name declared in its own package. Any modification made to
temporary variables remain **local** to their scopes of declaration and thus out
of these scopes, package variables they refer to remain untouched. You cannot
temporarize lexically scoped variables. Every package variable regardless of its
package can to temporarize.

We declare package variables with the keyword `our`, lexically scoped variables
with `let` and temporary variables with `temp`.

```
package One::Two {
   our x = ['one', 'two', 'three'];

   let a = {one => 1};
   {
      # a: {one => 1, two => 2}
      let a += {two => 2};

      # Could still use "One::Two::x" at declaration
      temp x = {};
      # empty hash
      say One::Two::x;
   }

   # Output: {one => 1}
   a.say;
   # Output: ['one', 'two', 'three']
   x.say;
}

package One::Two::Three {
   # Refers to the package variable "x" declared in the "One::Two" package
   say One::Two::x;

   # Compiler fails and tells that "a" isn't declared in namespace "One::Two::Three"
   say One::Two::Three::a;
}
```

Static variables are lexically scoped variables that when defined in a function
or block, retains their values between recalls and jumps. We declare static
variables with the `state` keyword.

```
fn incr(n) {
   state k = n;

   return k if ++k == 9;
   __FN__(nil);
}

# Output: 9
say incr(5);
```

```
# Output: 3, 11, 4, 12
X: {
   state x = 2;
   ++x;
   say x;
   {
      state x = 10;
      ++x;
      say x;
   }
   redo X if x == 3;
}
```

A constant variable when declared is lexically scoped by default unless you
explicitly indicate that it is a package variables.

```
# lexically scoped declaration of a constant
const z = 4;

# constant package variables
const our (x, y) = (2, 10);
```

## Special variables

Some special variables are package variables (`p`) while others are lexically
scoped (`l`). Also, some of these variables are writable (`w`) while others are
read-only (`r`).

### Type I special variables

We expand the content of a type I special variable using the sigil `$`. It is
prohibited to declare a variable with a sigil, regardless of whether or not it
is special.

```
say "Running $0 on $OS"
```
* `v`: **(r,p)** Maat version.
* `o`: **(r,p)** OS on which `Maat` was build.
* `,`: **(w,p)** Cutput field separator.
* `/`: **(w,p)** Input record separator, new line by default.
* `|`: **(w,p)** If true, do not wait when buffer is full, flush to the stream after every read/write operation.
* `"`: **(w,p)** Separator char for arrays when interpolated in double-quoted strings and regexs.
* `$`: **(r,p)** Pid of the current running process.
* `0`: **(r,p)** Name of the executing program.
* `(`, `)`: **(r,w,p)** Real and effective gid of the current process, only works on OS supporting membership.
* `<`, `>`: **(r,w,p)** Real and effective uid of the current process.
* `f`: **(w,p)** An array object containing the result of `.split` when using maat's `-a`, `-n`, and `-F` CLI options.
* `*`: **(r,l)** Id of the current maatine, unique to each maatine.
* `.`: **(r,l)** Line number for the last accessed file handle, .
* `!`: **(r)** Retrieve errors from syscalls.

### Type II special variables

* `_`: **(w,l)** Topic variable, lexically scoped.

A topic variable `_` is declared as a lexically scoped variable and assigned a
value when topicalizing with `with`, `given`, `for` or anonymous functions
having undeclared parameters. `_` is lexically scoped as Maat supports language
threads called Maatines.

```
for ^5 {
    let _
}
```

The above declaration of `_` will fail during compilation since it was already
auto-declared and used by the loop.

* `ENV`: **(w,p)** A `Map` object containing your environment variables.
* `PATH`: **(w,p)** An `Array` object containing the absolute path to directories where maat searches for modules.
* `INC`: **(w,p)** A `Map`, which maps each imported module to their path location on disk.
* `SIG`: **(w,p)** For traping signals, map a signal name to a `Fn` object to be called when a triggered signal is trapped.
* `ARGV`: **(w,p)** An `Array` object containing command line arguments.
* `ARGC`: **(w,p)** A `Num` object representing the number of command line arguments.
* `DATA`: **(w,p)** Represents a file handle to manipulate data under the `___DATA___` special token in your maat code.
* `__FILE__`: **(r,p)** A `Str` object, it is the name of the source file in which it occurs.
* `__FN__`: **(r,l)** It returns the current function object if enclosed by it, otherwise, `nil`.

As any other non-constant package variable, all the above package
variables can be temporarized.

### Special token

* `___DATA___`

`___DATA___` is not a variable. Hmm... it is a special token you fit into your
maat code right at the end so that whatever data following it can be obtained
via a file handle in the special variable `DATA`.

```
temp $/;

# Outputs what follows ___DATA___
DATA.readline.say

___DATA___
This is not code but rather data accessible via "DATA".
This feature is taken from Perl.
```

## Constants

* `π`: Pi
* `ℇ`: Euler constant
* `ℎ`: Planck constant
* `ℏ`: Planck constant over 2 pi

## Assignments

What you first need to understand is that a list of expressions grouped with
`()` is also an expression which evaluates all its inner expressions and leave
on the stack returned the value of the the last evaluated one. A exception
arises when we start grouping variables on the left where is per

You can use the accumulator and destructor operator in assignments, here are
some examples which I believe should be self-documentary.

```
let (a, b, c, d, e, f);
let array = [1, 2, 3, 4, 5];

# Just map and discard the rests
(a, b, c) = (2, 10, -1, 5); # a: 2, b: 10, c: -1

#
(a, b, c) = array; # a: [1, 2, 3, 4, 5], b: nil, c: nil

# Destruct the array and map the result
(a, b, c) = ([2, 4, 5]*); # a: 2, b: 4, c: 5 

# Same but 'c' accumulates the rest in an array
(a, b, *c) = (array*, 10); # a: 1, b: 2, c: [3, 4, 5, 10]

# Map but discard the 2nd element in the list obtained after destruction
(a,, b) = (array*); # a: 1, b: 3

# 'b' is greedy as accumulates the rest and 'c' is 'nil'
(a, *b, c) = (2, 4, 5); # a: 1, b: [4, 5], c: nil

# Mutiple trailing ',' on the right side have no special effects
(a, b, c) = (4,, 10, 12); # a: 4, b: 10, c: 12

# '(a, b, c) = 2' then '10' then '-1' 
(a, b, c) = 2, 10, -1, 5; # a: 2, b: nil, c: nil

(a, b, c) = array*; # a: 1, b: nil, c: nil

(a, b) = array*, 10; # a: 1, b: nil

(a, *c) = array*; # a: 1, c: nil

let a = (10, 3, -2, 5); # a: 5

let a = 11, 4, -391; # a: 11

let (a) = (10, 4, -3); # a: 10. just map and discard the rest

# Just executing 'let a' then 'b' then 'c = (5, 9, -1)'
let a, b, c = (5, 9, -1); # a: nil, b: nil, c: -1.

let a, b = 5, 10; # a: nil, b: 5

let a, b, c = 10, 39; # a: nil, 

# Fails at compilation, use '*' once!
(a, *b, *c) = 2, 4, 2, 4;

# Fails at compilation, use '*' when grouping
a, *b, c = 2, 40;

(e, f) = (10, -1); # e: 10, f: -1
(e, f) = (f, e);  # e: -1, f: 10
```


# Types

Maat has ... builtin types, types are objects and objects are types.

* [`Bool`](./types/Bool.md)
* [`Num`](./types/Num.md)
* [`Str`](./types/Str.md)
* [`Range`](./types/Range.md)
* [`Array`](./types/Array.md)
* [`Map`](./types/Map.md)
* [`Set`](./types/Set.md)
* [`Bag`](./types/Bag.md)
* [`Lazy`](./types/Lazy.md)
* [`Fun`](./types/Fun.md)
* [`GFun`](./types/GFun.md)
* [`Regex`](./types/Regex.md)
* [`Ma`](./types/Ma.md)
* [`Co`](./types/Co.md)
* [`Work`](./types/Work.md)
* [`Chan`](./types/Chan.md)
* [`Socket`](./types/Socket.md)
* [`Socket::Work`](./types/Socket::Work.md)
* [`Proc`](./types/Proc.md)
* [`Proc::Work`](./types/Proc::Work.md)
* [`Pipe`](./types/Pipe.md)
* [`File`](./types/File.md)
* [`Dir`](./types/Dir.md)
* [`Date`](./types/Date.md)
* [`Sys`](./types/Sys.md)
* [`Term`](./types/Term.md)

## Mutability Trait

The mutability of an object can be set at the time of its allocation by using
the immutable trait `:im`, this way of making an object immutable is very
unusual but can be very useful in certain scenarios. Consider the following
code:

```
our c :i = {
    Burkina_Faso => '',
    Niger        => '',
    Mali         => '',
};
```

This code declares a package variable `c` and assigns to it a map which is set
immutable using the `:i` trait, please note that there's really nothing that
relates the mutability of `c` to `:im` although `c` . If you intend to not
modify a complex struture through out the execution of your program then think
of using `:im` as it enhances the performance of your program by avoiding
unnecessary needed to synchronize operations on that structure when your program
is executing in a multi-threaded environment.

So `:im` simply means that any object assigned to `c` must be marked immutable
is

only possible if and only if the object 

# Blocks and Flow Controls

We separate statements with a generic newline or a semicolon in case we have
more than one statement on a single line.

1. Blocks

```
block ::= [ LABEL: ] '{' CODE '}'
```

```
say 1;
say 2; say 3;
{ say 1 }; { say 4 }

{
    say "one";
    { say "two" }

    # recall the current block
    _BLOCK_;
}

block: {
    state x = 2;
    say x++;
    redo block if x < 20;
}
```

2. The `do` statement

```
do '{' CODE '}'
do 'PATH/TO/A/MAAT/FILE'
```

`do` runs the code from the passed block or file and returns the result of the
last evaluated expression.

```
let v = do { 2 };

# output: 2
say v;

# output: 3
(do { 3 }).say;

do { false } || die "failed";

# x: 18
let x = do { 2 ** 4 } + 2;
```

## Topic variables

Maat does topicalization with flow control statements and anonymous functions a
way similar to that in Perl and Raku. It is important to understand this now to
avoid confusion when reading the below specification. Here, we'll focus on
topicalization regarding flow controls and the one about functions will be
explained later.

Topicalization is a technique used to apply a set of operations on one or a
group of specific targets, it involves making the topic of operations more
accessible in code. If not explicitly defined, a topic variable is represented
by the type II special variable `_` which holds the current default or implied
argument for certain operations. Topicalization allows you to use this special
variable implicitly or explicitly within your code to refer to the current topic
without having to explicitly declare it as parameter to each operation. An
example is using the default topic variable in an anynomous function which
implicitly means the anonymous function take a single parameter and the default
topic variable refers to it. Another example is using the default topic variable
within the code of the `with` construct to implicitly refer to the return value
of its conditional expression.

2. The `if` flow control statement

```
if EXPR [ -> TOPIC_VAR ]      { CODE }
[ elsif EXPR [ -> TOPIC_VAR ] { CODE } ]*
[ else                        { CODE } ]

EXPR if EXPR
```

Conditional construct `if`, note that paranthesis are optional.

You must explicitly defined a topic variable if the return value of the
conditional expression of `if` or any `elsif` is of interest to you as this flow
construct neither changes nor defines the default topic variable `_`.

```
if true { say "it is true" }

if 0 {
    say "you are a failure";
}
elsif false {
    say "still a failure, go away!!";
}
else {
    say "welcome my man!";
}


let x = Num.rand(120);
if x % 2 -> r {
    say "remainder is #{r}";
}
```

`if` has a statement modifer form where EXPR does not get executed in a new
scope.

```
say "one" if true;

let k = 2 if 1;
# output: 2
k.say;
```

3. The `with` flow control

```
with EXPR [ -> TOPIC_VAR ]     { CODE }
[ orwith EXPR [ -> TOPIC_VAR ] { CODE } ]*
[ else                         { CODE } ]

EXPR with EXPR
```

Conditional `with` construct, parathensis are optional as always.

`with` tests for definedness (that's `!nil`) whereas `if` tests for truth.
Unlike the `if` construct, the `with` and `orwith` sets the default topic
variable `_` to the value returned by the their conditional expressions.

```
let (u, y) = 5, nil;

with u { say "defined, see: #{_}" }

# output: 5
with   y          { say "never here" }
orwith u / 2 -> m { say m, u }
else              { say "and never here too" }
```

The `with` statement avoid you from doing the following

```
let x = (y + 1) / 2;
with x { .say }
```

But simple do

```
with (y + 1) / 2 { .say }
```

Its statement modifer form.

> 
>
>

```
# output: 2 4 4 8
for 4, 8 {
    let k = _ with _ / 2;
    say k / _
}
```

4. The `for` loop control

```
for <LIST | ARRAY | MAP | RANGE | LAZY | GFun> [ -> TOPIC_VAR [ , TOPIC_VAR ]* ] { CODE }

EXPR for <LIST | ARRAY | RANGE | MAP | LAZY | GFun>
```

`for` either iterate over an iterable object or a comma separated list of values
which internal is represented as an object. Iterable objects here are object of
the following types: `Array`, `Map` `Range`, `Lazy` and `GFun`.

Here are examples of iterations over a comma seperated list of values

```
# three iterations
for "a", r/regex/, [2, 4] { .say }

let ar = [ qw<one two three four five> ];

# trailing comma to indicate it is a list and thus only one iteration
for ar, { .say }
# or
.say for (ar,);
# and not this otherwise you get something weird if what follows "," can be
# evaluated by the for loop or compiler bails out if it does not make sense
.say for ar,;

---
We have a.len + 1 iterations, the array destruction operator is used to break
'ar' into a list. At topic let declaration, set a custom default value to handle
situations where we are out of elements
---
for ar*, 2 -> m, n = 'default' { (n + '-' + m).say }
```

Iterating over an Array objects

```
# output: 3 3 5 4 4
for ar -> i { say i.len }

# 'ar' is now @a(3 3 5 4 4) as 'j' binds the corresponding indexed element
for ar -> j {
    j = j.len
}

# output: (3, 3) (5, 4) (4, none)
for ar -> i, j = "none" {
    print "(#i, #j) "
}

# output: 3 3 5 4 4
.say for ar;

# output: @a(3 3 5 4 4)
.say for ar,;

# Output: 2
for (((2, (2, 10)), 2)) { .say }

# syntax error
for ((2, (2, 10)), 2),  { .say }

let points = { kueppo => @a(10 30 9), monthe => [100, 39, 10] };

# Output: ['10', '30', '9']
for points<kueppo>,   { .say }
for (points<kueppo>,) { .say }

# Output: 100, 39, 10
for points<month> { .say }

# Output: 10, 2
# it's so as parenthese are optionals, this means that the first
# encountered parenthese is part of the `for' syntax.
for ((2, (2, 10)), 2) { .say }
```

For lazy evaluation, we iterate over a lazy object. A lazy object implements a
subset of Array methods that process array elements to produce other elements.
Each method call on a lazy object registers that method with its arguments so
that it gets called for every lazy iteration.

```
let a = @a(nairobi niamey yaounde)

# "grep" and "map" got registered and they later on get called for every element in "a"
for a.lazy
     .grep(:/^n/)
     .map(:[.ucfirst, .len])
                            -> x { x.dump }
```

5. The `when` flow control

```
when COND [ | EXPR ]* { CODE }

EXPR when COND
```

the when flow control is similar the `if` construct but differs from  how the
condition is tested if COND is just an expression, then CODE gets executed if
the value of the topic variable smartmatch the result of EXPR


6. The `given` topicalizer

```
given EXPR { CODE }

EXPR given EXPR
```



```
# output: Num, 42
given 34 {
    when Num { say "Num"; proceed }
    when 42  { say "42" }
    default  { say "Default" }
}
```

For topicalization, you can also use `given` as a standalone statement

```
let x = [2, 5];
given x {
    say "variable x has two elements" if .len == 2
}

print .map {|rx| rx ** 2 } given x
```

7. looping with the `loop` construct

```
loop [ [ INIT ] ; [ COND ] ; [ STEP ] ] { CODE }
```

Just like the C-for loop

```
loop let k = 0; k ≤ 20; k² { k.say }

# you can skip some parts
loop let k = 0;;k++ {
    k.say;
    break if k == 10
}

loop { say "looping forever" }
```

8. `while` and `until`

The basic `while` and `until` loop.

```
let k = 6;

while k > 1 {
   k.say;
   k--;
}

until k == 0 {
   say "not entering here";
}
```

9. `do`-`while`/`until`

```
do { CODE } while COND

do { CODE } until COND
```

```
let k = Set.new(2, 4, 5);
let b = [2, 7, 3];

do k.push(b.pop) while set(2, 7) ∉ k;

do {
   say "forever"
} until false;
```

10. loop control statments `next`, `break`, and `redo`

```
break [ LABEL ]
next  [ LABEL ]
redo  [ LABEL ]
```
use loop control statements to control the behavoir of loop and non-flow control
blocks

* `next`: just like `C`'s `continue` loop control statement
* `break`: just like `C`'s `break` loop control statement
* `redo`: to rerun current loop block without evaluating the conditional 

if the LABEL is omitted, the control statement refers to the innermost enclosing
loop or non-flow control block.

11. `labels`

labels permits you to label blocks and flow controls so as to be able to perform
jumps using control statements like `redo`, `break` and `next`.

```
# an infinite loop with prints "one"
ONE: {
    say "one";
    redo ONE
}

# print "two" to the stdout and repeatly print "three"
TWO: {
    say "two";
    THREE: {
        say "three";
        redo THREE;
    }
    # dead code
    say "never gonna be executed";
}
```

```
fn do_sleep(n) {
    LOOP:
    for ^10 {
        break LOOP when n;
        .sleep
    }
}
```

12. Execute once in a loop/function with `once`

```
once STATEMENT
```

`once` gives you the possibility to execute a statement within a loop only once
regardless of the number of iterations. One great advantage it offers is freeing
us from using a conditional construct to avoid the execution of a statement.

```
let amap = @m(one 1 two 2 three 3);

amap.each_kv {|k,v|
    once say 'only once!';
    printf "%s => %d\n", k, v;
}
```

12. Handling exceptions with `try`-`catch`-`finally`

```
try { CODE } catch ( EXCEPTION_VAR ) { CODE } [ catch () { CODE } ]* [ finally { CODE } ]
```

`try`-`catch` for handling exceptions.


13. Concurrency with the `ma` statement prefix

```
ma METHOD_CALL
ma FOR_LOOP
```

`ma` is a statement prefix for function calls and the `for` loop, it is
responsible of running code concurrently using maatines. When prefixed to
function calls, it runs the function in a new lightweight thread known as a
Maatine.

```
fn just_sleep(n) { n.sleep }

for 10..20 -> time {
    ma just_sleep(time)
}
```

When `ma` is used along side with a `for` loop, each block runs run on its own
Maatine. In Maat, you'll never have to care about concurrent access to shared
memory because synchronization is done by Maat itself.

```
ma for ^10 { .sleep }
```

# Functions

```
[ mul ] fn NAME [ ( [  [ PARAM [ = DEFAULT_VALUE ] ] [, PARAM [ = DEFAULT_VALUE ] ]* ] ) ] { CODE }

{ [ | PARAM [ = DEFAULT_VALUE ] [, PARAM [ = DEFAULT_VALUE ] ]* | ] CODE }

: EXPR
```

Functions are first class values and hence they can be assigned to variables.
The use of parantheses during a function call is optional while its usage in a
function definition is mandatory if and only if that function takes arguments.

```
fn hello_world {
    say "Hello, World!"
}

hello_world()
```

Here we are declaring an anonymous function which takes a single parameter and
then call it.

```
let sleep = {|x| x.sleep; say "slept for #x seconds" };
sleep.call(5)
```

You can use the other syntax if your anonymous function is just a
single expression. `__FUNC__` does not work here!

```
let sleep = :5.sleep;
sleep.call
```

## The default Topic Variable `_`

The type II special variable `_` is called a topic variable, this variable can
be implicitly refered on anonymous functions and some flow control blocks. The
usage of this default topic variable within the code of an anonymous function
implies that this anonymous function takes a single argument whose parameter
wasn't explicitly declared with `| ... |` and hence defaults to `_`.

It is possible to omit `_` when calling a method on the content of a topic
variable.

```
let anony = { say _ * 2 };

# output: tanzaniatanzania
anony.call("tanzania");

# Err: takes only one arg as the topic let is used in anony
anony.call("a", "b");

# .ucfirst is the same as _.ucfirst
let ar = @a(tcheukam madjou monthe);
say ar.map: .ucfirst;
```

The usage of the default topic variable in an anonymous function having declared
parameter or expecting no parameters refers to the one from outer scopes.

The method `.times` of the `Num` type expects no argument when called and thus
the default topic variable refers to the one from the outer scope.

```
# output: 88888888 666666 666666
ar.map(:.len).each: .times: .print;
```

Maat has support for multiple dispatching and named arguments. Mixing named
arguments with unnamed ones brings a lot of confusion in your code and hence
either you name all your arguments or you don't name anything at all.

```
fn call(c, n) { c.call(_) for ^n }
call({ .say }, 5);

mul fn who(name, age) {
    say "Hello, my name is #name, I'm #{age}yo"
}

mul fn who(name) {
    say "Hello, my name is #name!"
}

who("kueppo", 20);
who("madjou");
who(age => 5, name => liza);

# no candidates for this and thus fails at compile time
who(age => 10);

fn mul(s, k) { s * k }
fn mul(s)    { s * 2 }

mul("one").say;    # output: oneone
mul("two", 5).say; # output: twotwotwotwo
```

Maat has what we call an accumulator operator, this accumulator operator in a
method/function definition is a prefix operator used to collect the rest of
extra indefinite number of arguments as an array into the last declared
parameter of that function/method during calls. This is done by prepending `…`
to the last declared parameter at function/method definition.

```
# using the array accumulator operator for variadic arguments
fn count(name, *counts) {
    printf "You have %d %ss\n", counts.len > 0 ? counts.sum : 0, name
}

count("pineaple", 2, 4, 10); # output: You have 16 pineaples
count("orange");             # output: You have 0 oranges

fn sum(*ar) { ar.sum }

# they don't make sense at all! fail at compilation
fn bad_func1(a, *b, c)  { ... }
fn bad_func2(*a, b, c)  { ... }
fn bad_func3(*a, b, *c) { ... }
fn bad_func4(a, *b, *c) { ... }
```

The reason why `fn` optionally precedes `{}` when defining anonymous functions
is mainly for syntatic sugar and so to avoid confusion with blocks, it is
prohibited to declare an anonymous function the following ways as it may add
much work to the compiler.

```
# fails at compile time
{ .say }.call(20);
```

But the following are permitted:

```
Fn.new({ .say }).call(20);
(:.say).call(20);
fn { .say }.call(20);
```

## Traits

Functions as well as methods support the `:save` trait, the `:save` trait caches
the return value of a function call to avoid recomputation of the same function
call when doing recursive calls. This trait can help you do dynamic programming
with less overhead.

```
fn fib(n) :s {
    n < 2 ? n : __FN__(n - 1) + __FN__(n - 2)
}
```

## Generator function

A Generator function for gathering values with `take`, a generator function is
an iterable object where each iteration resumes from where the generator
function was paused by a `take` call.

```
fn factors(n) :g {
    let k = 1;

    while k ** 2 < n {
        take k, n.div(k) if n % k;
        k++;
    }
    take k if k ** 2 == n;
}

.say for factors(36)
```


# Classes

```
class CLASS_NAME {
    <
        <
            <
                < has | state > ATTR_NAME :TRAIT [ = DEFAULT_VALUE ]
            >
            |
            <
                [ state ] meth METHOD_NAME
                [
                    ( [ PARAM [ = DEFAULT_VALUE ] [, PARAM [ = DEFAULT_VALUE ] ]* ] )
                ]
                { CODE }
            >
        >+
    |
        ...
    >
}
```

```
# unimplemented classes
class B { ... }
class C { ... }

# "is" for inheritance and "does" for roles
class A :is(B, C) :does(D, E) {
    # read-only attribute, ro: say A.x; not possible: A.x = "some value"
    has x :ro;

    # read-write attribute with default value '0', write: A.y = 2; read: say A.y
    has y :rw = 0;

    has z;

    # static variable which is accessible to all objects via class 'A': A.count
    state count = 0;

    # static method (A.m()), self isn't valid here as with static variables
    state meth m() {
        ...
    }

    meth xyz() {
        # self.x, self.y, etc.
        ...
    }

    mul meth amethod() { ... }

    # takes a parameter x
    mul meth amethod(x) { ... }

    # defining a method 'priv' as private, oi means only-in
    meth priv() :oi { ... }
}
```

List of traits supported by attributes and methods

* `rw`: Make attribute read-write
* `ro`: Make attribute read-only
* `built`: Make attribute private but can be set at instanciation
* `oi`: Make method private to the class

To every object is associated a metaobject which permits object introspection,
given a variable `obj` containing an abitrary object, this object can be
introspected via its metaobject by using the `.^` method call operator.

```
obj.^who;      # returns the class from which obj was instanciated
obj.^name;     # name of the class from which the object was instantiated
obj.^methods;  # 
```

# Roles

```
role ROLE_NAME {
    <
        <
            [ state ] meth METHOD_NAME
            [
                ( [ PARAM [ = DEFAULT_VALUE ] [, PARAM [ = DEFAULT_VALUE ] ]* ] )
            ]
            { CODE }
        >+
    |
        ...
    >
}
```

A class does 

```
role D { ... } # unimplemented role

role E {
    meth eat(x) {
        say "#{self.name} eats #x"
    }
}
```

# Regular Expressions

Maat uses Perl compatible regular expressions(PCRE2).

```
```

See section on the [Regex](./types/Regex.md) type for more information.

# Async Programming with Works

Maat has two concurrent programming model which are the asynchronous and
thread-like model. The thread-like model is what we call
[Maatines](./types/Ma.md) and the async model is what we call
[Works](./types/Work.md). Async functionality is implemented with the help of
Maatines, so yeah… it is just an abstract layer over it. A Work encapsulate a
computation called a Work which runs internally as a new or updated Maatine
object.

A Work has three possible state which are the `Do`, `Done` and `Failed` state.

1. In the `Do` state, Work is either not started or on progress.
2. In the `Done` state, Work is `Done` and has its result saved for latter retrieval.
3. In the `Failed` state, an exception was thrown when the Work was doing its work and hence `Fail`ed.

The below code creates a new Work which is initially in the `Do` state,
completes its work with the `done` method and from there checks its updated
status and result with the `status` and `done` methods respectively.

```
let w = Work.new;   # new Work object
say w.status;       # output: Do
w.done("I'm done");
say w.status;       # output: Done
say w.result;       # output: I'm done
```

You can return a Work that is already done with the static `done` method and you
can also return a work that has already failed with the `failed` method.

```
Work.done.status.say;   # output: Done
Work.failed.status.say; # output: Failed
```

We can chain Works just like you do with Promises in Javascript

> The chaining mechanism of Works in Maat is completely
> different from that of Promises in javascript.

Listen to the story: you first start by creating a work with `Work.does` which
returns a work object on which you can but not only do either of the following:

1. Call the `.catch` method to handle any exception on the created Work, it
   returns back the same object
2. Call the `.then` method to create and return a new Work to be scheduled for
   execution if Work object invoker is `Done`

If the exception of the work created by `.then` is not handled, it inherits its
exception handler from the highiest top level Work.

```
let w = Work.does({ sleep 4; 10 })
            .catch({|e| say "Catched: #{e}" })
            .then({ say "My handler is the one above"; _ + 2 })
            .then({(|r| say "Mine is below"; r - 2 })
            .catch({|e| warn "Couldn't sub 2 from 12? ans: #{e}" });

say abide w; # output: 10
```

To abide a work say `w` with function `abide` is to call the method `.result` on
it. `abide` function can take multiple Works as argument and return their
results in an Array. If a work with a handled exception encounters an
exception, calling `.result` on it returns `nil`.

You can setup a work to do work by sleeping for a given amount of time using the
static `.for` method, yes! sleeping is a kind of a work, even in real life :).

```
Work.for(5)
    .then({ say "Previous work was to sleep 5 seconds" });
```

The above is kinda similar to this one and both of them can be used to build
timers.

```
Work.does(:5.sleep)
    .then(:…);
```

You can also set a work to do work by waiting til a specified time using the
`.til` static method

```
Work.til(Date.now + 10)
    .then(:say "Previous work is done at ", _);
```

You can use the `Work.allof` method to return a new Work object that will be
`Done` when all the work passed as arguments are either `Done` or `Failed`. The
value return by `.result` method call on the returned work object is always
`true` and practically useless.

```
let k = ^5.map {|i| Work.does(:sleep i) };

Work.allof(k)
    .then: k.map(:.result).sum.say;
```

See section on the [Work](./types/Work.md) type for more information.

# Maatines

Maatines are lightweight threads managed by the Maat runtime, they are extremely
easy and less costly to create and destroy. Internally, Maat has a high
performant scheduler which schedules the executions of Maatines across OS
threads.

See section on the [Ma](./types/Ma.md) type for more information.

# Packages

```
```

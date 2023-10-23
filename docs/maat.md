---
title: Maat Programming Language Specification
author: Kueppo Tcheukam J. W.
email: tcheukueppo@yandex.com
---

# Maat 

`Maat` is a multi-paradigm general purpose programming language that
empowers programmers to easily build fast and scalable applications.

Key features of the Maat programming language:

- Functional Programing
- Object Oriented Programming
- Multiple Dispatching
- Closures
- Concurrency with [Works](./types/Work.md) and [Maatines](./types/Ma.md)
- Traits
- Lazy Evaluation
- Regular Expressions using PCRE2
- Full Unicode support
- Phasers

This is the specification of the Maat programming language, it is
written as a guidance for its implementation.

> **NB**: Take note of the following conventions about syntax definition

> - `[ X ]` implies X is optional
> - `|` implies alternation, for example `A | B` signifies either `A` or `B`
> - `...` implies the previous entity be it optional or not, can occur multiple times
> - Words written in capital letter are self-documentary, for example `CODE`

# Operators

## Lonely operator

- `…`, `...`: to specify unimplemented code

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

- `++`: ****(p,b)**** incrementation operator
- `--`: **(p,b)** decrementation operator
- `-`: **(b)** negation operator
- `+`: **(b)** positive operator, result of the evaluation is equal to the operand
- `~`: **(b)** binary complement
- `…` or `...`: **(b)** Destructor/Accumulator operator
- `^`: **(p)** Range operator, `^5` return a `Range` object, i.e `Range.new(0, 5)`
- `√`: **(p)** sqaure root operator
- `⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`: **(b)** power operators
- `Σ`: **(p)** summation operator
- `Π`: **(p)** product operator

## Named unary operators

- `defined`: **(b)** check if a variable is `nil`
- `sleep`: **(b)** sleep the current Maatine for a given amount of time
- `return`: **(b)** return from a function
- `assert`: **(b)** test an assertion
- `assert_eq`: **(b)** test an assertion
- `exit`: **(b)** exit program with given exit code

## Named list operators

- `say`: **(b)** print to the standard output with a trailing new line
- `print`: **(b)** print without a new line
- `printf`: **(b)** print formatted string
- `sprintf`: **(b)** sprintf, return formatted string
- `join`: **(b)** Function version of `.join` method of the Array type
- `map`: **(b)** Function version of `.map` method of the Array type
- `lmap`: **(b)** Function version of `.lmap` method of the Array type
- `grep`: **(b)** Function version of `.grep` method of the Array type
- `run`: **(b)** run the passed command on a new shell
- `die`: **(b)** raise an exception with the given message on the stderr and exit program if there is no handler
- `warn`: **(b)** warn by sending a message to the stderr

## Named binary operators

- `isa`: **(i)** checks if the left object `isa` or kind of the class with the right object
- `minmax`: **(i)** return in an array the min and the max of the right and left operand respectively

## Binary operators for maat objects

- `.`: **(i)** method call operator on objects and classes
- `.^`: **(i)** method call operator for meta-programming
- `,`, `=>`: **(i,i)** comma operator, and key-value separator infix operator
- `!`: **(p)** negation operator ex: `!true == false`
- `=`, `:=`: **(i)** assignment and binding operator
- `//`: **(i)** a variant of the `||` operator that tests for definedness, `a // b`, return `a` it is not `nil` otherwise `b`
- `==` / `⩵`, `!=` / `≠`, `>`, `>=` / `≥`, `<`, `<=` / `≤`: **(i)** basic infix operators
- `+`, `-`, `/` / `÷`, `*`,  `%`, `..` / ``: **(i)** add, sub, div, div, mul, remainder and range operator
- `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `%=`: **(i)** `X op= Y` same as `X = X op Y`
- `<<`, `>>`, `^`, `&`, `&=`, `|=`: **(i)** bitwise shift on left and right, logical `or` and `and`
- `&&`, `||`, `&&=`, `||=`: **(i)** logical "and" and "or" operator
- `≅` / `=~`, `~~`: **(i)** regex operator and smartmatch operator
- `∉`,  `∈`, `∊`, `∍`, `∋`, `∌`, `⊂`, `⊄`, `⊆`, `⊈`, `⊃`, `⊅`, `⊇`, `⊉`, `≡`, `≢`, `⊖`, `∩`, `⊍`, `∪`, `⊖`, `⊎`, `∖`: **(i)** set operators
- `<=>` : **(i)** `X <=> Y` says if `X < Y` yield `-1` or if `X == Y` yield `0` elseif `X > Y` yield `1`
- `∘`: **(i)** function composition infix operator
- `?:`: **(?)** tenary operator

## List of all operators from highest precedence to lowest

- left        terms and list operators (leftward)
- right       grouping operator `( )`
- left        method call operator `.`
- nonassoc    `++`, `--`, `√` and unary prefix `…` / `...`
- right       `**`, `⁰`, `¹`, `²`, `³`, `⁴`, `⁵`, `⁶`, `⁷`, `⁸`, `⁹`
- right       `!`, `~`, `\` and unary `+` and `-`
- left        `=~`, `!~`
- left        `*`, `/`, `%`
- left        `+`, `-`, `.`, `∘`
- left        `∩`, `⊍`
- left        `∪`,  `⊖`, `⊎`, `∖`
- chained     `∈`, `∊`, `∉`, `∋`, `∍`, `∌`, `≡`, `≢`, `⊂`, `⊄`, `⊃`, `⊅`, `⊆`, `⊈`, `⊇`, `⊉`, `≼`, `≽`
- left        `<<`, `>>`
- nonassoc    named unary operators
- nonassoc    `isa`
- chained     `<`, `>`, `<=` / `≤`, `>=` / `≥`
- chain/na    `==`, `!=`, `<=>`, `~~`
- left        `&`
- left        `|`, `^`
- left        `&&`
- left        `||`, `//`
- nonassoc    `..`, lonely operator `…` / `...`
- right       `?:`
- right       `=`, `:=`, `&=`, `|=`, `&&=`, `||=`, `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `.=`, `%=`, `last`, `break`, `redo`, and `dump`
- list        `,`, `=>`

# Comments

Single line comments are done with `#` and multi-line comments with `---`

```
# single line comment
---
multi
line
comment
---
```

# Delimiter

## Pair delimiters


The following pair delimiters can be used as delimiters for quoted values
and regular expression operators.

```
( )       [ ]       { }       < >
⦗ ⦘       ⧼ ⧽      〈 〉      ❨ ❩
❲ ❳       ❴ ❵       ⟅ ⟆       ⟦ ⟧
« »       » «       ‹ ›       › ‹
〈 〉    〈 〉     《 》     「 」
„ ”       “ ”       ‘ ’       ‚ ’
『 』     【 】     〔 〕    〖 〗
〘 〙     〚 〛     ⌈ ⌉       ⌊ ⌋
❪ ❫       ❬ ❭       ❮ ❯       ❰ ❱
⟨ ⟩       ⟪ ⟫       ⟬ ⟭       ⟮ ⟯
⦃ ⦄       ⦅ ⦆       ⦋ ⦌       ⦍ ⦎
⦏ ⦐       ⦑ ⦒
```

### Examples

```
var x = [ qw(one two three) ]

# [ "Three", "Two", "One" ]
var b = x.map(:.cap).rev

# [ "0ne", "tw0", "three" ]
x =~ s<o>«0»
```

## Single character delimiter

We also have a restricted set of delimiters for quoted values and regular
expression operators.

```
/ | % "  '
```

### Examples

```raku
var a = [ qw|ONE TWO THREE| ]
a.each: .lc.say

say q"interpolation won't work"

say qq<interpolation works, array: #a>

# [ "0ne", "Tw0" ]
a.grep({|x| x =~ m|o|i }).map(:s|o|0|r).map(:.ucfirst).say
```

# Variables

`Maat` has four types of variables: package, lexical, temporary and persistent variables.

A module file has a `.mm` extension, a package is a namespace, variables who are fully
qualified are known as package variables, there is no one-to-one mapping between a
namespace and module file, therefore multiple namespaces can be define in a single
module file but this is not a good practice.

Package variables can be accessed from other packages using their fully qualified
name and lexically scoped variables cannot be accessed from outside the package in
which they were declared.

A temporary variable is a variable that is lexically scoped and refers to a package
variable declared from another package if the name of the temporary variable at
declaration is fully qualified otherwise, it refers to a package variable declared
in its own package. Any modification made to temporary variables remains **local** to
their scopes of declaration, thus, out of these scopes, package variables they refer
to remain untouched. You cannot temporarize lexically scoped variables.

We declare package variables with the keyword `pkg`, lexically scoped variables
with `var` and temporary variables with `temp`.

```raku
package One::Two {
    pkg x = [ qw<one two three> ]

    var a = {one => 1}
    {
        # a: {one => 1, two => 2}
        var a += {two => 2}

        # could still use "One::Two::x" at declaration
        temp x = {}
        # empty hash
        say One::Two::x
    }

    # output: {one => 1}
    a.say
    # output: [ qw<one two three> ]
    x.say
}

package One::Two::Three {
    # refers to the package variable "x" declared in the namespace "One::Two"
    say One::Two::x

    # compiler fails and tells that "a" isn't declared in namespace "One::Two::Three"
    say One::Two::Three::a
}
```

Static variables are lexically scoped variables that when defined in a function
or block, retains their values between recalls and jumps. We declare static
variables with the `state` keyword.

```
fun incr(n) {
   state k = n

   return k if ++k == 9
   _FUN_(nil)
}

# output: 9
say incr(5)
```
constant variables are lexically scoped by default unless you precise they're
global with the global keyword.

```js
# lexically scoped declaration of a constant
const z = 4

# a constant package variable
const pkg (x, y) = (2, 10)
```

## special package variables

Special variables are package variables, some are writetable and can change the
behavoir of your programs while others are readonly and contain useful information
to make important decisions.

### Type I special variables

We expand the content of special variables using the sigil `$`.
Some of these variables are writable(`w`) while others are read-only(`r`).

```js
say "Running #{$0} on #$OS"
```
- `V`: (r) Maat version
- `O`: (r) OS version on which `pity` was build
- `.`: (w) Current line in a file
- `,`: (w) Output field separator
- `/`: (w) Input record separator
- `"`: (w) Separator character during interpolation
- `$`: (r) Pid of the current running process
- `0`: (r) Name of the executing program
- `(`, `)`, `<`, `>`: (w) real

- `F`: (w)

- `!`: (r) retrieve errors from syscalls

### Type II special variable

We donot expand type 2 special variables with `$`, they are just
like simple variable we use in our Maat programs

- `_` : Topic variable, assinged a value when topicalizing with `with`, `given`, `for`, `if`, ... etc
- `ENV`: a `Map` object which contains your current environment variables
- `PATH`: an `Array` object which contains the absolute path to directories where maat searches for modules
- `INC`: a `Map`, which map each imported module to their path location in the filesystem
- `SIG`: for traping signals, map a signal name to a `Fun` object to be called when given signal is trapped
- `ARGV`: An array containing command line arguments
- `ARGC`: represents the argument count, it is an object of type `Num`
- `DATA`: represents a file handle to manipulate data under `_DATA_`, just like in perl

### Special tokens

- `_FUN_`: for recursion, call the current function
- `_BLOCK_`: for recursion, call the current block
- `_FILE_`: a string object, represents the name of current script in execution

## Constants

- `π`: Pi
- `ℇ`: Euler constant
- `ℎ`: Planck constant
- `ℏ`: Planck constant over 2 pi

## Assignments

You can use the accumulator and destructor operator in assignments, here are
some examples which are self understandable.

```js
var (a, b, c, d, e, f)
var array = [1, 2, 3, 4, 5]

(a, b, c)    = 2, 10, -1, 5 # a: 2, b: 10, c: -1
(a, b, c)    = array        # a: [1, 2, 3, 4, 5], b: nil, c: nil
(a, b, c)    = [2, 4, 5]... # a: 2, b: 4, c: 5
(a, b, ...c) = array..., 10 # a: 1, b: 2, c: [3, 4, 5, 10]
(a,,b)       = array...     # a: 1, b: 3

# fails during compilation, only use '...' at the end
(a, ...b, c) = 2, 4, 5

(e, f) = (10, -1) # e: 10, f: -1
(e, f) = (f, e)   # e: -1, f: 10
```

# Types

Maat has ... builtin objects, types are objects and objects are types, check
details on each types here.

- [`Any`](./types/Any.md)
- [`Bool`](./types/Bool.md)
- [`Num`](./types/Num.md)
- [`Str`](./types/Str.md)
- [`Range`](./types/Range.md)
- [`Array`](./types/Array.md)
- [`Map`](./types/Map.md)
- [`Set`](./types/Set.md)
- [`MSet`](./types/MSet.md)
- [`Bag`](./types/Bag.md)
- [`MBag`](./types/MBag.md)
- [`Lazy`](./types/Lazy.md)
- [`Fun`](./types/Fun.md)
- [`GFun`](./types/GFun.md)
- [`Regex`](./types/Regex.md)
- [`Ma`](./types/Ma.md)
- [`Work`](./types/Work.md)
- [`Chan`](./types/Chan.md)
- [`Socket`](./types/Socket.md)
- [`Socket::Work`](./types/Socket::Work.md)
- [`Proc`](./types/Proc.md)
- [`Proc::Work`](./types/Proc::Work.md)
- [`Pipe`](./types/Pipe.md)
- [`File`](./types/File.md)
- [`Dir`](./types/Dir.md)
- [`Date`](./types/Date.md)
- [`Sys`](./types/Sys.md)
- [`Term`](./types/Term.md)

# Blocks and Flow Controls

We separate statements with a generic newline or a semicolon in case we have
more than one statement on a single line.

1. Blocks

`[ LABEL: ] { CODE }`

```
say 1
say 2; say 3
{ say 1 }; { say 4 }

{
    say "one"
    { say "two" }

    # recall the current block
    _BLOCK_
}

block: {
    state x = 2
    say x++
    redo block if x < 20
}
```

2. `do` block

`do { CODE }`

```
var v = do { 2 }

# output: 2
say v

# output: 3
(do { 3 }).say

do { false } || die "failed"
```

## Topic variable `_`

Maat does topicalization with flow control statements and anonymous functions a way
similar to that in Perl and Raku. It is important to understand this more to avoid
confusion when reading the below specification. Here, we'll focus on topicalization
regarding flow controls and the one about functions will be exlpained later.


2. `if` conditional construct

```
if EXPR [ -> VAR ] { CODE } [ elsif EXPR [ -> VAR ] { CODE } ]... [ else { CODE } ]
EXPR if EXPR
```

Conditional construct `if`, note that paranthesis are optional.

You must explicitly defined a topic variable as the `if` / `elseif`
construct does not change the value of the default topic variable `_`.

```js
if true { say "it is true" }

if 0 {
    say "you are a failure"
}
elsif false {
    say "still a failure, go away!!"
}
else {
    say "welcome my man!"
}


var x = Num.rand(120)
if x % 2 -> r {
    say "remainder is #{r}"
}
```

if has a statement modifer form where EXPR does not get executed
in a new scope.

```
say "one" if true

var k = 2 if 1
# output: 2
k.say
```

3. `with` conditional construct

```
with EXPR [ -> VAR ] { CODE } [ orwith EXPR [ -> VAR ] { CODE } ]... [ else { CODE } ]
EXPR with EXPR
```

Conditional `with` construct, parathensis are optional as always.

`with` tests for definedness (that's `!nil`) whereas `if` tests for
`truth`. Unlike the `if` construct, the `with` and `orwith` sets the
default topic variable `_` to the value returned by the their
conditional expressions.

```raku
var (u, y) = 5, nil

with u { say "defined, see: #{_}" }

# output: 5
with   y          { say "never here" }
orwith u / 2 -> m { say m, u }
else              { say "and never here too" }
```

The `with` statement avoid you from doing the following

```raku
var x = (y + 1) / 2
with x { .say }
```

But simple do

```raku
with (y + 1) / 2  { .say }
```

Its statement modifer form.

>
>
>

```raku
# output: 2 4 4 8
for 4, 8 {
    var k = _ with _ / 2
    say k / _
}
```

4. `for` loop control

```
for LIST | ARRAY | MAP | RANGE [ -> VAR [ , ... ] ] { CODE }
EXPR for LIST | ARRAY | RANGE | MAP
```

`for` either iterate over a comma separated list of values or an iterable objects.

`for` can iterate over objects of type Array, Map, Range and Lazy.


Here are examples of iterations over a comma seperated list of values

```
# Three iterations
for "a", r/regex/, [2, 4] { .say }

var ar = qa<one two three four five>

# trailing comma to indicate it is a list and thus only one iteration
for ar, { .say }
# or
.say for (ar,)
# and not this otherwise you get something weird if what follows "," can be
# evaluated by the for loop or compiler bails out if it does not make sense
.say for ar,

---
We have a.len + 1 iterations, using the array destruction operator which
breaks 'ar' into a list, at topic var declaration, set a custom default
value when we are out of elements
---
for ar…, 2 -> m, n = 'default' { (n + '-' + m).say }
```

Iterating over an Array objects

```
# output: 3 3 5 4 4
for ar -> i { say i.len }

# 'ar' is now a[3 3 5 4 4] as 'j' binds the corresponding indexed element
for ar -> j {
    j = j.len
}

# output: (3, 3) (5, 4) (4, none)
for ar -> i, j = "none" {
    print "(#i, #j) "
}

.say for ar
```

For lazy evaluation, we iterate over a lazy object

```
var a = qa(nairobi niamey yaounde)
for a.lazy
     .map(:[.ucfirst, .len])
                            -> x { x.dump }
```

5. `given`-`when`

```
EXPR given EXPR
given EXPR { CODE }
given EXPR { when COND [ | COND ] ... { CODE } [ when COND [ | COND ] ... { CODE } ] ... [ default { CODE } ] }
```

```
# output: Num, 42
given 34 {
    when Num { say "Num"; proceed }
    when 42  { say "42" }
    default  { say "Default" }
}

# use '|' for alternation
var name = "kueppo"
given name {
    when /^k/ | /o$/ { say "matches" }
    when /^m/        { say "starts with 'm'" }
    default          { say "nothing worked" }
}
```

You can also use `given` as a standalone statement to specify the variable of concern
in the execution of a block.

```
var x = [2, 5]
given x {
    say "variable x has two elements" if .len == 2
}

print .map {|rx| rx ** 2 } given x
```

6. looping with the `loop` construct

```
loop [ [ INIT ] ; [ COND ] ; [ STEP ] ] { CODE }
```

Just like the C-for loop

general form: `loop initializer; condition; step { ... }`

```
loop var k = 0; k ≤ 20; k² { k.say }

# you can skip some parts
loop var k = 0;;k++ {
    k.say
    break if k == 10
}

loop { say "looping forever" }
```

7. `while` and `until`

The basic `while` and `until` loop.

```
var k = 6

while k > 1 {
    k.say
    k--
}

until k == 0 {
    say "not entering here"
}
```

8. `do`-`while`/`until`

```
var k = Set.new(2, 4, 5)
var b = [2, 7, 3]

do {
    k.push(b.pop)
} while [2, 7] ∉ k

do {
    say "forever"
} until false;
```

9. loop control statments: `next`, `break`, and `redo`

general form: `next [LABEL|LEVEL]`, if you donot specify the label then it
performs the action for the current block.

- `next`: just like `C`'s `continue` loop control statement
- `break`: just like `C`'s `break` loop control statement
- `redo`: rerun the block without testing the condition

10. `labels`

labels permits you to jump between labeled blocks using a loop control statement

```raku
# an infinite loop with prints "one"
ONE: {
    say "one"
    redo ONE
}

# print "two" to the stdout and repeatly print "three"
TWO: {
    say "two"
    THREE: {
        say "three"
        _BLOCK_
    }
    # dead code
    say "never gonna be executed"
}
```

It is not just blocks, you can labeled the `while`, `until`, `for` and `loop` construct

```raku
```

11. Execute once in a loop/function with `once`

`once` gives you the possibility to execute a statement within a loop
only once regardless of the number of iterations. One great advantage
it offers is avoid the burdens of using a conditional construct to avoid
the execution of a statement.

```
var amap = qm{one 1 two 2 three 3}

amap.each_kv {|k,v|
    once say 'only once!' if v == 1
    "%s => %d".printfln(k, v)
}
```

11. Handling exceptions with `try`-`catch`-`finally`

```
try { CODE } catch ( EXCEPTION_VAR ) { CODE } [ catch () { CODE } ]... [ finally { CODE } ]
```

`try`-`catch` for handling exceptions.


12. Concurrency with the `ma` statement prefix

```
ma METHOD_CALL
ma FOR_LOOP
```

`ma` is a function call and `for` statement prefix, it is responsible
for running code concurrently using maatines. When prefixed to function
calls, it runs the function in a new lightweight thread known as a
Maatine.

```
fun just_sleep(n) { n.sleep }

for 10..20 -> time {
    ma just_sleep(time)
}
```

When `ma` is used on a `for` loop, each block runs run on its own
Maatine. In Maat, you'll never have to care about concurrent access
to shared memory because synchronization is done my Maat itself.

```raku
ma for ^10 { .sleep }
```

# Functions

```
fun NAME [ ( [ARG1] [, ARG2] ...) ] { CODE  }
{ [ | VARNAME [ = VALUE ] [, VARNAME [ = VALUE ] ] ... | ] CODE }
: EXPR
```

Functions are first class values and thus can be assigned to a variable.

Paranthese during function calls arguments while paranthese during function
definition are optional if and only if the function takes no argument.

```
fun hello_world {
    say "Hello, World!"
}

hello_world()
```

Declaring an anonymous function that takes a single parameter and call it

```ruby
var sleep = {|x| x.sleep; say "slept for #x seconds" }
sleep.call(5)
```

You can use a different syntax if your anonymous function is just a single expression

```
var sleep = :5.sleep
sleep
```

## Topic variable `_`

The type II variable `_` is called a topic variable, this variable operates on
anonymous functions and flow control blocks. The usage of a topic variable when
calling an anonymous function implies the anonymous function takes a single
argument whose parameter wasn't explicitly declared with `|...|` and hence
defaults to `_`.

It is possible to omit `_` when calling a method on the content of the topic
variable.

```raku
var anony = { say _.Str * 2 }

# output: tanzaniatanzania
anony.call("tanzania")

# Err: takes only one arg as the topic var is used in anony
anony.call("a", "b")

# .ucfirst is the same as _.ucfirst
var ar = qm{tcheukam madjou monthe}
say ar.map({.ucfirst})
```

The topic variable in an anonymous function with declared or expecting no
parameters refers to if exists from outer scopes.

The method `.times` of the `Num` type expects no argument when called and thus
refers to the topic variable from the lowest level outer scope.

```raku
# output: 88888888 666666 666666
ar.map(:.len).each { .times { .print } }
```

Maat has support for multiple dispatching and named arguments. Mixing named
arguments with unnamed ones brings a lot of confusion in your code and hence
either you name all your arguments or you don't name anything at all.

```ruby
fun call(c, n) { c.call(_) for ^n }
call({ .say }, 5)

mul fun who(name, age) {
    say "Hello, my name is #name, I'm #{age}yo"
}

mul fun who(name) {
    say "Hello, my name is #name!"
}

who("kueppo", 20)
who("madjou")
who(age => 5, name => liza)

# no candidates for this and thus fails at compile time
who(age => 10)

fun mul(s, k) { s * k }
fun mul(s)    { s * 2 }

mul("one").say    # output: oneone
mul("two", 5).say # output: twotwotwotwo
```

Maat has what we call an accumulator, let us cover this up. The accumulator
operator is a postfix operator used to collect the rest of extra indefinite
number of arguments as an array into the last declared parameter of that called
function/method. This is done by appending `…` to the last declared parameter
at function definition.

```
# using the array accumulator operator for variadic arguments
fun count(name, …counts) {
    printf "You have %d %ss\n", count.len > 0 ? counts.sum : 0, name
}

count("pineaple", 2, 4, 10) # output: You have 16 pineaples
count("orange")             # output: You have 0 oranges

fun sum(...ar) { ar.sum }

# does not make sense, all fail at compilation
fun bad_func1(a, ...b, c)    { ... }
fun bad_func2(...a, b, c)    { ... }
fun bad_func3(...a, b, ...c) { ... }
fun bad_func4(a, ...b, ...c) { ... }
```

```
{ .say }.call(20)
Fun.new({ .say }).call(20)
(:.say).call(20)
```

## Traits

Functions as well as methods support the `:save` trait, the `:save` trait
caches the return value of a function call to avoid recomputation of the
same function call in recursive calls. This trait can help you do dynamic
programming with less overhead.

```
fun fib(n) :save {
    n < 2 ? n : _FUN_(n - 1) + _FUN_(n - 2)
}
```

## Generator function

A Generator function for gathering values with `take`

The `take` keyword pauses generator function execution and the 

```
fun factors(n) {
    var k = 1

    while k ** 2 < n {
        take k, n.div(k) if n % k 
        k++
    }
    take k if k ** 2 == n
}

.say for factors(36)
```


# Classes and Roles

```
role D { ... }

role E {
}

class B { ... }
class C { ... }

# "is" for inheritance and "does" for roles
class A :is(B, C) :does(D, E) {
    # read-only attribute, ro: say A.x; not possible: A.x = "some value"
    has x :ro   

    # read-write attribute with default value '0', write: A.y = 2; read: say A.y
    has y :rw = 0

    has z

    # static variable which is accessible to all objects via class 'A': A.count
    state count = 0

    # static method (A.m()), self isn't valid here
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

List of traits supported by class attributes

- `rw`: Make attribute read-write
- `ro`: Make attribute read-only
- `built`: Make attribute private but can be set when instanciating
- `oi`: Make method private to the class

To every object is associated a metaobject which permits object
introspection, given an object `obj`, you can introspect this method
via its metaobject by using the `.^` method call operator.

```
# consider 'obj' a variable containing an object, that object supports the following metamethods
obj.^who      # 
obj.^name     # name of the class from which the object was instantiated
obj.^methods  #
```

# Regular Expressions

Maat uses Perl compatible regular expressions(PCRE2).

```
```

See section on the [Regex](./types/Regex.md) type for more information.

# Work

Maat has two concurrent programming model which are the asynchronous and
thread-like model. The thread-like model is what we call **Maatines** and
the async model is what we call **Works**. Async functionality is
implemented with the help of **Maatines**, so yeah… it is just an abstract
layer over it. A Work encapsulate a computation called a Work which runs
internally, is represented as new or resurrected Maatine.

A Work has three possible state which are the `Do`, `Done` and `Failed` state.

1. In the `Do` state, Work is either not started or on progress.
2. In the `Done` state, Work is `Done` and has its result saved for latter retrieval.
3. In the `Failed` state, an exception was thrown when the Work was doing its work and hence `Fail`ed.

The below code creates a new Work which is initially in the `Do`
state, completes its work with the `done` method and from there checks
its updated status and result with the `status` and `done` methods
respectively.

```raku
var w = Work.new   # new Work object
say w.status       # output: Do
w.done("I'm done")
say w.status       # output: Done
say w.result       # output: I'm done
```

You can return a Work that is already done with the static `done`
method and you can also return a work that has already failed with
the `failed` method.

```raku
Work.done.status.say   # output: Done
Work.failed.status.say # output: Failed
```

We can chain Works just like you do with Promises in Javascript

ATTENTION! The chaining mechanism of Works in Maat is completely
different from that of Promises in javascript.

Listen to the story: you first start by creating a work with say
`Work.does`, it returns a work object on which you can but not only
do either of the following:

1. Call the `.catch` method to handle any exception on the created Work, it returns back the same object
2. Call the `.then` method to create and return a new Work to be scheduled for execution if Work object invoker is `Done`

If the exception of the work created by `.then` is not handled, it inherits its exception
handler from the top level Work.

```raku
var w = Work.does({ sleep 4; 10 })
            .catch({(e) say "Catched: #{e}" })
            .then({ say "My handler is the one above"; _ + 2 })
            .then({(r) say "Mine is below"; r - 2 })
            .catch({(e) warn "Couldn't sub 2 from 12? ans: #{e}" })

say abide w # output: 10
```

To abide a work say `w` with function `abide` is to call the method `.result`
on it. `abide` function can take multiple Works as argument and return their
results in an Array. If a work with a handled exception encounters an 
exception, calling `.result` on it return `nil`.

You can set a work to do work by sleeping for a given amount of time using
the static `.for` method, yes! sleeping is a kind of a work, even in real
life :).

```raku
Work.for(5)
    .then({ say "Previous work was to sleep 5 seconds" })
```

The above is kinda similar to this one and both of them can be used to
build timers.

```raku
Work.does(:5.sleep)
    .then(:…)
```

You can also set a work to do work by waiting til a specified time
using the `.til` static method

```raku
Work.til(Date.now + 10)
    .then(:say "Previous work is done at ", _ )
```

You can use the `Work.allof` method to return a new Work object that
will be `Done` when all the work passed as arguments are either `Done`
or `Failed`. The value return by `.result` method called of this work
is always `true` and is practically useless.

```raku
var k = ^5.map {|i| Work.does(:sleep i) }

Work.allof(k)
    .then: k.map(:.result).sum.say
```

See section on the [Work](./types/Work.md) type for more information.

# Maatines

Maatines are lightweight threads managed by the Maat runtime, they are
extremely easy and less costly to create and destroy. Internally, Maat
has a high performant scheduler which schedules the executions of
Maatines accross Operating systems threads.

See section on the [Ma](./types/Ma.md) type for more information.

# Packages

```
```

# Phasers

```
```

# Conclusion


# Maat 

`Maat` is a multi-paradigm programming(functional and object oriented)language
inspired from the lovely `Perl`, `Raku` and `Lua` programming languages.

- Functional Programing
- Object Oriented Programming
- Work and Maroutines
- Traits
- Multiple Dispatching
- Type Checks
- Closures
- Lazy Evaluation
- Pattern Matching using PCRE
- Unicode support

# Operators

## Lonely operator

- `…`, `...`: to specify unimplemented code

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

- `++`: (p,b) inc
- `--`: (p,b) dec
- `-`: (b) negative
- `+`: (b) absolute
- `~`: (b) binary complement
- `…` or `...`: (b) untight operator
- `^`: (p) `^5` return an array of element i.e `0` to `5`
- `√`: (p) sqaure root operator
- `⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`: (b) super-script power operators

## Named unary operators

- `defined`: (b) check if a varible is `nil` and return true otherwise
- `chomp`: (b)
- `chop`: (b) strip 
- `sleep`: (b) call sleep() syscall
- `return`: (b) return from a function
- `exit`: exit program with given exit code

## Named list operators

- `say`: (b) print to the standard output with a trailing new line
- `print`: (b) print without a new line
- `printf`: (b) print formatted string
- `printfln`: (b) formatted string + a trailing new line and return to a file descriptor
- `sprintf`: (b) sprintf, return formatted string
- `sprintfln`: (b) sprintf + a trailing new line
- `min`: (b) yield min from a list
- `max`: (b) yield max from a list
- `minmax`: (b) yield min and max, return them in a list
- `die`: (b) program dies with a message on `STDERR`
- `warn`: (b) warn with a message on `STDERR`

## Named binary operators

- `isa`: (i) checks if the left object `isa`(of the same class or kind of inherited) the right object

## Binary operators for maat objects

- `.`, `.^`: method/attribute call operators for objects and metaobjects/metaclasses respectively

- `,`, `=>`: (i, b) comma operator, and key-value separator infix operator

- `!`: (p) negation operator ex: `!true == false`

- `=`, `:=`: (i) assignment and binding operator

- `//`: (i) `a // b`, return `a` it is set otherwise `b`

- `==` / `⩵  `, `!=` / ` ≠`, `>`, `>=` / `≥`, `<`, `<=` / `≤`: (i) basic operators between objects

- `+`, `-`, `/` / `÷`, `*`,  `%`, `..` / ``: (i) add, sub, div, div, mul, remainder and range operator

- `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `%=`: (i) `left operand(a variable)` `=` `left operand value` `op` `right operand`

- `<<`, `>>`, `^`, `&`, `&=`, `|=`: (i) bitwise shift on left and right, logical `or` and `and`

- `&&`, `||`, `&&=`, `||=`: (i) logical "and" and "or" operator

- `≅  ` /  `=~`, `~~`: (i) regex operator and smart match operator

- `∉ `,  `∈  `, `∊`,  `∍ `, `∋ `,  `∌ `, `⊂  `, ` ⊄ ` , `⊆ `, `⊈  `, ` ⊃  `, `⊅ `
  ` ⊇  `, ` ⊉  `, `≡ `, `≢  `, `⊖`, `∩ `, `⊍ `, `∪ `,  `⊖`, `⊎ `, `∖`: (i) set operators.

- `<=>` : `op1 <=> op2` says if `op1 < op2` yield `-1`, `op1 == op2` yield `0`, `op1 > op2` yield `1`

- `∘`: mathematic function composition, take two subroutines as operand

- `?:`: tenary operator

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
- left        `∩ `, `⊍ `
- left        `∪ `,  `⊖`, `⊎ `, `∖`
- chained     `∈ `, `∊`, `∉ `, `∋ `, `∍ `, `∌ `, `≡ `, `≢ `, `⊂ `, `⊄ `, `⊃ `, `⊅ `, `⊆ `, `⊈ `, `⊇ `, `⊉ `, `≼ `, `≽ `
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

# Delimiter

## Pair delimiters

The pair delimiters below are used in declararing enums, arrays, hashes and on the regex operators (substitution, transliteration and pattern matching).

```
( )       [ ]       { }       < >
« »       » «       ‹ ›       › ‹
„ ”       “ ”       ‘ ’       ‚ ’
〈 〉    〈 〉     《 》     「 」
『 』     【 】     〔 〕    〖 〗
〘 〙     〚 〛     ⌈ ⌉       ⌊ ⌋
❪ ❫       ❬ ❭       ❮ ❯       ❰ ❱
❲ ❳       ❴ ❵       ⟅ ⟆       ⟦ ⟧
⟨ ⟩       ⟪ ⟫       ⟬ ⟭       ⟮ ⟯
⦃ ⦄       ⦅ ⦆       ⦋ ⦌       ⦍ ⦎
⦗ ⦘       ⧼ ⧽      〈 〉      ❨ ❩
⦏ ⦐       ⦑ ⦒
```

### Examples

```
var a = w|one two three|

-- [ "Three", "Two", "One" ]
var b = a.map(.cap).reverse

-- [ "0ne", "tw0", "three" ]
a =~ s<o>«0»
```

## Single character delimiter

We also have a restricted set of delimiter characters for double quoted strings(`q`), single
quoted strings(`Q`) and regex operators, Just like in Perl.

`/`, `|`, `%`, `"`, and `'`

### Examples

```
var a = w|ONE TWO THREE|
a.each{.lc.say}

say q"interpolation won't work"

say Q<interpolation works, array: #a>

-- [ "0ne", "Tw0" ]
b = a.grep({(x) x =~ m|o| }).map(s|o|0|r).map(.ucfirst)
b.say
```

# Variables

`Maat` has four types of variables: package, lexical, temporal and persistent variables.

Package variable can be accessed from other packages using their full qualified name and lexically
scoped variables cannot be accessed from outside the package in which it was declared.

Temporal variables are declared within a scope and refers to previously declared package
variables from the current package if its name at declaration isn't fully qualified otherwise
refers to the variable in the specified package. Any changes made to temporal variables remains
local to the scope from where it was declare and thus the referenced variables remains untouched.
You cannot localize lexically scoped variables.

Declare package variables with the keyword `global`, lexically scoped variables with
`var` and temporal variable with `tmp`.

```
package One::Two {
    global x = w<one two three>

    var a = { one => 1 }
    {
        -- a: { one => 1, two => 2 }
        var a += { two => 2 }

        -- could still use "One::Two::x" at declaration
        tmp x = {}
        -- empty hash
        say One::Two::x
    }

    -- a: {one => 1}
    a.say
    -- x: ["one", "two", "three"], unchanged!
    x.say
}

package One::Two::Three {
    -- refers to the package variable "x" declared in the namespace "One::Two"
    say One::Two::Three::x

    -- compiler tells there is no such package variable in namespace "One::Two::Three"
    say One::Two::Three::a
}
```

In regard to functions, static variables are lexically scoped variables which retains their values between
function and block(during recursion or jumps with a loop control) calls. We declare static lexically
scoped variables with the `state` keyword.

```
fun increment(n) {
    state k = n
    _FUN_(nil), return k if ++k != 9
}

-- 9
increment(0, 9).say
```
constant variables are lexically scoped by default unless you precise they're global with the global keyword.

```
-- lexically scoped constant
const z = 4

-- a constant global
const global (x, y) = (2, 10)
```

## Special package variables

Special variables are package variables, some are writetable and can change the behavoir of your programs
while others are readonly and contain useful information to make important decisions.

### Type I special variables

We expand the content of special variables using the sigil `$`. some of these variables are writable(`w`)
while others are read-only(`r`).

#### Example

```
say "Running #$0 on #$OS"
```
- `Maat`: (r) Maat version
- `OS`: (r) OS version on which `pity` was build
- `.`: (r) current line in a file
- `,`: (w) output field separator
- `/`: (w) input record separator
- `"`: (w) Separator character during interpolation
- `$`: (r) Pid of the current maat program
- `0`: (r) Program name
- `!`: (r) retrieve errors from syscalls

### Type II special variable

We donot expand type 2 special variables with `$`,  they are just like simple variable we use in our Maat programs

- `_` : (w) Topic variable, used mostly in blocks
- `__` : (w) Topic variable, used mostly in blocks
- `ENV`: a `Map` which contains your current environment variables
- `PATH`: an `Array` which contains the absolute path to directories where maat searches for modules
- `INC`: a `Map`, each key correspond to an imported module and have a value which correspond its location in the filesystem
- `SIG`: for traping signals, map a signal name to a `Fun` object to be called when given signal is trapped
- `ARGV`: array containing command line arguments
- `ARGC`: represents the argument count, it is an object of type `Int`
- `DATA`: represents a file handle to manipulate data under `_DATA_`, just like in perl
- `_FUN_`: for recursion, call the current function
- `_BLOCK_`: for recursion, call the current block
- `_FILE_`: a string object, represents the name of current script in execution

## Constants

- `π`: Pi, 3.14....
- `e`: Euler's number

# Objects

 Maat has 16 builtin objects, types are objects and objects are types, check details on
each types here.

- Maat 
- Any
- Bool
- Num
- Rat
- Str
- Array
- Map
- Set
- Bag
- Fun
- File
- Dir
- Pipe
- Socket
- Regex
- Range
- Date
- Sys
- Supply
- Work
- Lazy

# Flow control

Here is an overview of the `maat` syntax.

We separate statements with a generic newline or a semicolon in case we have more
than one statement on a single line.

1. Blocks

```
say 1
say 2; say 3
{ say 1 }; { say 4 }

{
    say "one"
    { say "two" }

    -- recall the current block
    _BLOCK_
}
```

2. `do` Block

```
var v = do { 2 }
-- "2"
say v

-- "3"
(do { 3 }).say

do { false } or die "failed"
```

3. `awork` Blocks

run a block asyncronously

```
awork {
    4.sleep
    say "done"
}

-- declare a function and assign it to "a"
var a = fun { sleep 4; say "done" }

-- run function in "a" asyncronously and return a promise
var w = awork a.call()

say "do stuffs"

-- await work 'w'
abide p
```

4. `if`

Conditional `if` statement, note that paranthesis are optional.

`if ... { ... } [elsif ... { ... } ...] [else { ... }]`

```
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

say 1 if true
```

5. `with`

Conditional `with` statement, parathensis are always optional.

`with` tests for definedness (that's `!nil`) whereas `if` tests for `truth` in
the returned value of the expression.

```
var (u, y) = (5, nil)

-- 5, 5
with u { say _, u }

-- 5
with   y      { say "never here" }
orwith u -> m { say m }
else          { say "never here" }
```

6. `for`

```
for "a", r/regex/, [2, 4] { .say }

-- output: 3 3 5 4 4
for ar -> i { i.len.say }

ar = <one two three four five>
-- "ar" is now [3, 3, 5, 4, 4]
for ar -> j {
    j = j.len
}

-- set a custom value when we are running out of elements
-- (3, 3) (5, 4) (4, none)
for ar -> i, j = "none" {
    say "(#i, #j)"
}

.say for ar
```

7. `gather`-`take`

`gather` is statement/block prefix which returns a sequence of values comming from
calls to `take` in the dynamic scope of block/function passed as argument to `gather`.

```
fun factors(n) {
  var k = 1

  gather {
    while k ** 2 < n {
        if n % k {
            take k
            take n.div(k)
        }
        k++
    }
    take k if k ** 2 == n
  }
}

factors(36).say
```

8. `keys`

`keys` is a looping construct which iterates over hash keys to perform certain operations if any of them smart-matches
any of the cases.

```
var h = { banana => 2, orange => 1, melon => 2 }

var s = keys h {
    match r/^b/ | r/ge$/ { _ = .chop; __ += 2 }
    default { __ *= 2 }
}

-- { banan => 5, orang => 3, melon => 4 }
s.say
```

8. `given`-`match`

We implement the switch-case using `given`-`match` construct, When an object is specified this construct tests the topic
variable initialized to the argument passed to `given` against the following cases using the smartmatch operator(`~~`).
We execute the block of the first matching case and instantly exit the `given` block. We can continue on to the
next case by using the `proceed` instruction within the block of a case.

```
-- output: Num, 42
given 34 {
  match Num { say "Num"; proceed }
  match 42  { say "42" }
  default   { say "Default" }
}

-- use '|' for alternation
var name = "kueppo"
given name {
    match /^k/ | /o$/ { say "matches" }
    match /^m/        { say "starts with 'm'" }
    default           { say "default" }
}
```

Note that smartmatch operator is the default operator used when 

You can also use `given` as a standalone statement to specify the variable of concern in
the execution of a block.

```
var x = [2, 5]
given x {
    say "variable x has two elements" if x.len == 2
}

print .map {(rx) rx ** 2 } given x
```

10. `loop`

Just like the C-for loop

general form: `loop initializer; condition; step { ... }`

```
loop var k = 0; k ≤ 20; k² { k.say }

-- you can skip some parts
loop var k = 0;;k++ {
    k.say
    break if k == 10
}

loop { say "looping forever" }
```

12. `while` and `until`

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

13. `do`-`while`/`until`

```
var k = Set.new(2, 4, 5)
var b = [2, 7, 3]

do {
    k.add(b.pop)
} while [2, 7] ∉ k

do {
    say "forever"
} until false;
```

14. loop control statments: `next`, `break`, and `redo`

general form: `next [LABEL|LEVEL]`, if you donot specify the label then it
performs the action for the current block.

- `next`: just like `C`'s `continue` loop control statement
- `break`: just like `C`'s `break` loop control statement
- `redo`: rerun the block without testing the condition

15. `labels`

labels permits you to jump between labeled blocks using a loop control statement

```
-- an infinite loop with prints "one"
ONE: {
    say "one"
    redo ONE
}

-- print "two" to the stdout and repeatly print "three"
TWO: {
    say "two"
    THREE: {
        say "three"
        _BLOCK_
    }
    # dead code to be wiped by the compiler
    say "never gonna be executed"
}
```

16. `once`

`once` gives you the possibility to execute a statement within a loop only once regardless
of the number of iterations. One great advantage it offers is avoid the burdens of using a
conditional construct to avoid the execution of a statement.

```
var h = h{one 1 two 2 three 3}

for h.each_kv { say "#_ => #__" }

for h.each_kv {
    (k,v)
    once say 'only once!' if v == 1
    printfln "%s => %d", k, v
}

-- for syntatic suger
for h.each_kv -> k, v {
    once say 'only once!' if v == 1
    printfln "%s => %d", k, v
}
```

17. `try`-`catch`-`finally`

`try`-`catch` for handling exceptions.

18. topic variables `_` and `__`

We've been using topic variables since the begining of this section without known what they are, a topic
variable is just an argument passed to an executing block, you can declare a topic variable to avoid the
default one(`_`), you can declare more than one topic variable to fetch the desired number of elements
for calls.

NOTE: topic variables should only be named at the begining of the block of concern.

```
var a = [2, 5, 34]

-- declaring a topic variable x
print a.map {(x)
    once x++
    next if x == 3
    √x
}

-- 2,2 4,none
[2, 2, 4].each {
    (x, y = "none")
    say "#x,#y"
}
```

# Functions

 Maat has support for multiple dispatching, type checks and named arguments. Mixing named arguments
with unnamed ones brings a lot of confusion in your code and hence either you name all your arguments
or you don't name anything at all.

```
fun callme(c, n) { c.call(_) for ^n }
callme({ .say }, 5)

mul fun intro(name, age) {
    say "Hello, my name is #name, I'm #{age}yo"
}

mul fun intro(name) {
    say "Hello, my name is #name!"
}

intro("kueppo", "20")
intro("sarah")
intro(age = 5, name = liza)

-- no candidates for this and thus fails at compile time
intro(age = 10)

-- You can also specify the return type
fun mul(Str str, Int k) -> Str { str * k }

fun mul(Str str) { str * 2 }

mul("one").say; mul("two", 5).say
```

Function as well as methods do have support for the `save` trait, note that return type has
to appears after trait.

```
fun fib(n) :save -> Num {
    n < 2 ? n : _FUN_(n - 1) + _FUN_(n - 2)
}
```

# Classes & Roles

```
role D { ... }
role E { ... }

class B { ... }
class C { ... }

-- "isa" for inheritance and "does" for roles
class A :is(B, C) :does(D, E) {
    has x :ro     -- read-only attribute, ro: say A.x; not possible: A.x = "some value"
    has y :rw = 0 -- read-write attribute with default value '0', write: A.y = 2; read: say A.y
    has Num z     -- attribute of type 'Num', maat has a type check system

    state count = 0 -- static variable which is accessible to all objects via class 'A': A.count

    meth xyz() {
        -- self.x, self.y, etc.
        ...
    }

    mul meth amethod() {}

    -- a method which returns an object of type 'Num'
    mul meth amethod() -> Num {}

    -- defining a method 'priv' as private, oi means only-in
    meth priv() :oi {}
}
```

List of traits supported by class attributes

- `rw`: Attribute is read-write
- `ro`: Attribute is read-only
- `built`: Make attribute private but can only also be written from outside the object only via object instanciation

We also have the `oi` trait which makes a method private

To every object is associated a metaobject which permits object introspection, given an object `obj`, you can
introspect this method via its metaobject by using the `.^` method call operator.

```
-- consider 'obj' an object, we have the following
obj.^who  -- 
obj.^name -- name of the class from which the object was instantiated
obj.^
```


# Regular expressions

Maat uses perl compatible regular expressions(PCRE), see Regex for more details.

```raku
var 
```

# Work and Concurrency

```raku
```

# Supply/React

```raku
```

# Packages

```raku
```

# Phasers

# Conclusion

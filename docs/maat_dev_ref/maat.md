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

- `++`: (p,b) incrementation operator
- `--`: (p,b) decrementation operator
- `-`: (b) negate the operand
- `+`: (b) 
- `~`: (b) binary complement
- `…` or `...`: (b) Array destruction operator in the context of list assingment and and 
- `^`: (p) `^5` return an array of element i.e from `0` to `5`
- `√`: (p) sqaure root operator
- `⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`: (b) super-script power operators

## Named unary operators

- `defined`: (b) check if a varible is `nil` and return true otherwise
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
- `join`: (b) 
- `die`: (b) program dies with a message on `STDERR`
- `warn`: (b) warn with a message on `STDERR`

## Named binary operators

- `isa`: (i) checks if the left object `isa`(of the same class or kind of inherited) the right object
- `minmax`: (i) return in an Array the min and the max of the right and left operand respectively

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
- `∉ `,  `∈  `, `∊`,  `∍ `, `∋ `,  `∌ `, `⊂  `, ` ⊄ ` , `⊆ `, `⊈  `, ` ⊃  `, `⊅ `, ` ⊇  `, ` ⊉  `, `≡ `, `≢  `, `⊖`, `∩ `, `⊍ `, `∪ `,  `⊖`, `⊎ `, `∖`: (i) set operators.
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

Pair delimiters below are used to declare enums, arrays, hashes and regexs

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
var x = qa|one two three|

-- [ "Three", "Two", "One" ]
var b = x.map(:.cap).rev

-- [ "0ne", "tw0", "three" ]
x =~ s<o>«0»
```

## Single character delimiter

We also have a restricted set of delimiter characters for double quoted strings(`q`), single quoted strings(`Q`)
and regex operators.

```
/ | % "  '
```

### Examples

```
var a = qa|ONE TWO THREE|
a.each: .lc.say

say q"interpolation won't work"

say Q<interpolation works, array: #a>

-- [ "0ne", "Tw0" ]
a.grep({(x) x =~ m|o| }).map(:s|o|0|r).map(:.ucfirst).say
```

# Variables

`Maat` has four types of variables: package, lexical, temporal and persistent variables.

Package variable can be accessed from other packages using their full qualified name and lexically scoped variables
cannot be accessed from outside the package in which it was declared.

Temporal variables are declared within a scope and refers to previously declared package variables from the current
package if its name at declaration isn't fully qualified otherwise refers to the variable in the specified package.
Any changes made to temporal variables remains local to the scope from where it was declare and thus the referenced
variables remains untouched. You cannot localize lexically scoped variables.

Declare package variables with the keyword `global`, lexically scoped variables with
`var` and temporal variable with `tmp`.

```
package One::Two {
    global x = a<one two three>

    var a = { one => 1 }
    {
        -- a: { one => 1, two => 2 }
        var a += { two => 2 }

        -- could still use "One::Two::x" at declaration
        tmp x = {}
        -- empty hash
        say One::Two::x
    }

    -- output: h{one 1}
    a.say
    -- output: a<one two three>
    x.say
}

package One::Two::Three {
    -- refers to the package variable "x" declared in the namespace "One::Two"
    say One::Two::Three::x

    -- compiler tells there is no such package variable in namespace "One::Two::Three"
    say One::Two::Three::a
}
```

In regards to functions, static variables are lexically scoped variables which retains their values between
function and block(during recursion or jumps with a loop control) calls. We declare static lexically scoped
variables with the `state` keyword.

```
func increment(n) {
    state k = n
    _FUNC_(nil), return k if ++k != 9
}

-- 9
increment(0, 9).say
```
constant variables are lexically scoped by default unless you precise they're global with the global keyword.

```
-- lexically scoped declaration of a constant
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
- `_FN_`: for recursion, call the current function
- `_BLOCK_`: for recursion, call the current block
- `_FILE_`: a string object, represents the name of current script in execution

## Constants

- `π`: Pi, 3.14....
- `e`: Euler's number

# Objects

Maat has 25 builtin objects, types are objects and objects are types, check details on
each types here.

- Maat 
- Any
- Bool
- Num
- Rat
- Str
- Range
- Array
- Map
- Set
- MSet
- Bag
- MBag
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
- Chan
- Ma
- Work
- Lazy
- Term

# Blocks and Flow Controls

We separate statements with a generic newline or a semicolon in case we have more than one statement
on a single line.

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

2. `do` block

`do { CODE }`

```
var v = do { 2 }

-- output: "2"
say v

-- "3"
(do { 3 }).say

do { false } or die "failed"
```

3. `work` block

`work { CODE }`

Run a block asyncronously

```
work {
    4.sleep
    say "done"
}

var i = work { INF.sleep }

say "do stuffs"

-- abide work 'w' for maximum 4 seconds
abide w

-- abide work 'i' forever
abide i
```

4. `if`

`if EXPR [ -> VAR ] { CODE } [ elsif EXPR -> [ VAR ] { CODE } ] [ else { CODE } ]`

Conditional `if` construct, note that paranthesis are optional.

You must explicitly defined a topic variable as the `if` construct does not change the value of
the default topic variable `_`.

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

var x = Num.rand(120)
if x % 2 -> r {
    say "remainder is #{r}"
}
```

5. `with`

`with EXPR [ -> VAR ] { CODE } [ orwith EXPR [ -> VAR ] { CODE } ] [ else { CODE } ]`

Conditional `with` statement, parathensis are optional as always.

`with` tests for definedness (that's `!nil`) whereas `if` tests for `truth` in the returned
value of the expression.

Just like the `if` construct, the `with` does not set the default topic variable `_` but you can explicitly
define a topic variable if the return value of the evaluated expression EXPR is of interest to you.

```
var (u, y) = 5, nil

with u { say "defined" }

-- output: 5
with   y          { say "never here" }
orwith u / 2 -> m { say m, u }
else              { say "and never here too" }
```

Explicit topicalization avoids you from doing the following

```
var x = (y + 1) / 2
with x { ... }
```

But simple do

```
with (y + 1) / 2 -> x { ... }
```

6. `for`

`for LIST          [ -> VAR [ , ... ] ] { CODE }`
`for ARRAY         [ -> VAR [ , ... ] ] { CODE }`
`for LAZY_ITERATOR [ -> VAR [ , ... ] ] { CODE }`

`for` iterator over the following iterable objects

- List: Comma separated list of values
- Array: Array objects are iterable
- Lazy itero: 


Here is an example of iterations over a list of values

```
-- list: three iterations
for "a", r/regex/, [2, 4] { .say }

var ar = a<one two three four five>

-- trailing comma to indicate it is a list and thus only one iteration
for ar, { .say }

-- we have a.len + 1 iterations
-- list: using the array destruction operator which breaks 'ar' into a list
-- set a custom default value when we are out of elements
for ar…, 2 -> m, n = 'default' { (n + '-' + m).say }
```

Iterating over Array objects

```
-- output: 3 3 5 4 4
for ar -> i { say i.len }

-- 'ar' is now a[3 3 5 4 4] as 'j' binds the corresponding indexed element
for ar -> j {
    j = j.len
}

-- output: (3, 3) (5, 4) (4, none)
for ar -> i, j = "none" {
    print "(#i, #j) "
}

.say for ar

```

```
```

7. `gather`-`take`

`gather` is statement/block prefix which returns a sequence of values comming from calls to `take` in
the dynamic scope of block/function passed as argument to `gather`.

```
func factors(n) {
  var k = 1

  lazy gather {
    while k ** 2 < n {
        take k, n.div(k) if n % k 
        k++
    }
    take k if k ** 2 == n
  }
}

factors(36).each { .say }
```

8. `keys`

`keys` is a looping construct which iterates over hash keys to perform certain operations if any of them
smart-matches any of the cases.

```
var fruits = h[banana 2 orange 1 melon 2]

keys fruits -> k, v {
    match /^b/ | /ge$/ { _ = .rev; v += 2 }
    default { v *= 2 }
}

-- output: { banan => 5, orang => 3, melon => 4 }
fruits.say
```

8. `given`-`match`

We implement the switch-case using `given`-`match` construct, When an object is specified this construct tests
the topic variable initialized to the argument passed to `given` against the following cases using the smartmatch
operator(`~~`). We execute the block of the first matching case and instantly exit the `given` block. We can
continue on to the next case by using the `proceed` instruction within the block of a case.

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

You can also use `given` as a standalone statement to specify the variable of concern in the execution
of a block.

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
    k.push(b.pop)
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
var h = qm{one 1 two 2 three 3}

h.each_kv {(k,v)
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

-- output: 2,2 4,nan
[2, 2, 4].each {(x, y = "nan") say "#x,#y" }
```

# Functions

 Maat has support for multiple dispatching, type checks and named arguments. Mixing named arguments
with unnamed ones brings a lot of confusion in your code and hence either you name all your arguments
or you don't name anything at all.

```
func callme(c, n) { c.call(_) for ^n }
callme({ .say }, 5)

mul func intro(name, age) {
    say "Hello, my name is #name, I'm #{age}yo"
}

mul func intro(name) {
    say "Hello, my name is #name!"
}

intro("kueppo", "20")
intro("sarah")
intro(age = 5, name = liza)

-- no candidates for this and thus fails at compile time
intro(age = 10)

-- You can also specify the return type
func mul(s, k) { s * k }

func mul(s) { s * 2 }

mul("one").say
mul("two", 5).say

-- using the array accumulator operator for variadic arguments
func tellme(Str name, Array counts…) {
    printfln "You have %d %s", counts.sum, name
}

tellme("pineaples", 2, 4, 10)
```

Function as well as methods do have support for the `save` trait, note that return type has
to appear after any trait

```
func fib(n) :save -> Num {
    n < 2 ? n : _FUNC_(n - 1) + _FUNC_(n - 2)
}
```

# Classes & Roles

```
role D { ... }
role E { ... }

class B { ... }
class C { ... }

-- "is" for inheritance and "does" for roles
class A :is(B, C) :does(D, E) {
    has x :ro     -- read-only attribute, ro: say A.x; not possible: A.x = "some value"
    has y :rw = 0 -- read-write attribute with default value '0', write: A.y = 2; read: say A.y
    has z         -- 

    state count = 0 -- static variable which is accessible to all objects via class 'A': A.count

    -- static method (A.m()), self isn't valid here
    state meth m() {
        ...
    }

    meth xyz() {
        -- self.x, self.y, etc.
        ...
    }

    mul meth amethod() {}

    -- takes a parameter x
    mul meth amethod(x) {}

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
-- consider 'obj' a variable containing an object, We have the following metamethods
obj.^who  -- 
obj.^name -- name of the class from which the object was instantiated
obj.^methods
```

# Regular Expressions

Maat uses Perl compatible regular expressions(PCRE2).

See the Regex object for more details.

```
```

# Work

Maat has two concurrent programming model which are the asynchronous and thread-like model. The thread-like
model is what we call **Maatines** and the async model is what we call **Works**. Async functionality is
implemented with the help of **Maatines**, so yeah… it is just an abstract layer over it. A Work encapsulate
a computation called a Work which runs internally, is represented as new or resurrected Maatine.

A Work has three possible state which are the `Do`, `Done` and `Fail` state.

1. In the `Do` state, Work is either not started or on progress.
2. In the `Done` state, Work is `Done` and has its result saved for latter retrieval.
3. In the `Fail` state, an exception was thrown when the Work was doing its work and hence `Fail`ed.

The below code creates a new Work which is initially in the `Do` state, completes its work with the
`done` method and from there checks its updated status and result with the `status` and `done` methods
respectively.

```
var w = Work.new   -- new Work object
say w.status       -- Output: Do
w.done("I'm done")
say w.status       -- Output: Done
say w.result       -- Output: I'm done
```

You can return a Work that is already done with the static `done` method and you can also
return a work that has already failed with the `failed` method.

```
Work.done.status.say   -- Output: Done
Work.failed.status.say -- Output: Failed
```

We can chain Works just like you do with Promises in Javascript

ATTENTION! The chaining mechanism of Works in Maat is completely different from that of Promises in javascript.

So this is the story: you first start by creating a work with say `Work.does({ ... })`, this returns a work
object on which you can do either but on only the following:

1. Call the `.catch` method to handle any exception on the created Work, it returns back the same object for further chaining
2. Call the `.then` method returns a new Work to be scheduled for execution if the invoker is `Done` with no expections.

If the exception of the newly Work created by `.then` is not handled, it inherits it the exception handler of the top level Work.

```
var w = Work.does({ sleep 4; 10 })
            .catch({(e) say "Catched: #{e}" })
            .then({ say "My handler is the one above"; _ + 2 })
            .then({(r) say "Mine is below"; r - 2 })
            .catch({(e) warn "Couldn't sub 2 from 12? ans: #{e}" })

say abide w -- Output: 10
```

To abide a work say `w` with function `abide` is to call the method `.result` on it.

You can set a work to do work by sleeping for a given amount of time using the static `.for` method, yes! sleeping is a
kind of work, even in real life :).

```
Work.for(5)
    .then({ say "Previous work was to sleep 5 seconds" })
```

The above is kinda similar to this one and both of them can be used to build timers.

```
Work.does(:5.sleep)
    .then(:…)
```

You can also set a work to do work by waiting til a specified period of time using the `.til` static method

```
Work.til(Date.now + 10)
    .then(:say "Previous work is done at #{_}")
```

You can also use the block version of work


Check the documentation on the object `Work` for more information regarding async programming in Maat.

# Maatines

Maatines are lightweight threads managed by the Maat runtime, they are extremely easy and less costly to create
and destroy. Internally, Maat has a high performant scheduler which schedules the executions of Maatines accross
Operating systems threads.


# Supply/React

```
```

# Packages

```
```

# Phasers

```
```

# Conclusion


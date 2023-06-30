# Pity

`Pity` is a multi-paradigm programming(functional and object oriented)language
inspired from the lovely `Perl`, `Raku` and `Lua` programming languages.

- Functional Programing
- Object Oriented Programming
- Promises and Concurrency
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
- `^`: (p) `^5` return a list of element i.e `0` to `5`
- `√`: (p) sqaure root operator
- `⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`: (b) super-script power operators

## Named unary operators

- `defined`: (b) check if a varible is `nil` and return true otherwise
- `chomp`: (b)
- `chop`: (b)
- `sleep`: (b)
- ``:

## Named list operators

- `say`: (b) print to the standard output with a trailing new line
- `die`: (b) program dies instantly
- `warn`: (b) warn on stderr
- `print`: (b) print without a new line
- `printf`: (b) print formatted string
- `printfln`: (b) formatted string + a trailing new line and return to stdout/stderr
- `sprintf`: (b) sprintf, return formatted string
- `sprintfln`: (b) sprintf + a trailing new line
- `min`: (b) yield min from a list
- `max`: (b) yield max from a list
- `minmax`: (b) yield min and max, return it in a list

## Named binary operators

- `isa`: (i) checks if the left object `isa`(of the same class or kind of inherited) the right object

## Binary operators for pity objects

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

The pair delimiters below are used in declararing enums, arrays, hashes and on the regex
operators (substitution, transliteration and pattern matching).

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

```raku
a = |one two three|

# [ "Three", "TWo", "One" ], Lennon Stella :-)
b = a.map{.capitalize}.reverse

# [ "0ne", "tw0", "three" ]
a =~ s<o>«0»
```

## Single character delimiter

We also have a restricted set of delimiter characters for double quoted strings(`qq`), single
quoted strings(`q`) and regex operators, Just like in Perl.

`/`, `|`, `%`

### Examples

```raku
var a = qw|ONE TWO THREE|
a.each{.lc}

say q%interpolation won't work%

say qq/interpolation works, array: #a/

# [ "0ne", "Tw0" ]
b = a.grep({(x) x =~ m|o| }).map{ s|o|0| }.map{ .ucfirst }
b.say
```

# Variables

`Pity` has four types of variables: package, lexical, temporal and persistent variables.

Package variable can be accessed from other packages using their full qualified name and lexically
scoped variables cannot be accessed from outside the package in which it was declared.

Temporal variables are declared within a scope and refers to previously declared package
variables from the current package if its name at declaration isn't fully qualified otherwise
refers to the variable in the specified package. Any changes made to temporal variables remains
local to the scope from where it was declare and thus the referenced variables remains untouched.
You cannot temporarize/localize lexically scoped variables.

Declare package variables with the keyword `global`, lexically scoped variables with
`let` and temporal variable with `temp`.

```raku
package One::Two {
    global x = <one two three>

    var a = {one => 1}
    {
        # a: {one => 1, two => 2}
        var a += {two => 2}

        # could still use "One::Two::x" at declaration
        temp x = {}
        # empty hash
        say One::Two::x
    }

    # a: {one => 1}
    a.say
    # x: ["one", "two", "three"], unchanged!
    x.say
}

package One::Two::Three {
    # refers to the package variable "x" declared in the namespace "One::Two"
    say One::Two::Three::x
    # returns nil, "a" in "One::Two" is lexically scoped
    say One::Two::Three::a
}
```

Persistent variables are lexically scoped variables which retains their values between
function and block(during recursion or jumps with a loop control) calls.
We declare persistent lexically scoped variables with `state`, just like in Perl.

```raku
fun increment(n) {
    state k = n
    __FUNC__(nil), return k if ++k != 9
}

# 9
increment(0, 9).say
```
constant variables are lexically scoped by default unless you precise they're global with the global
keyword.

```raku
# lexically scoped constant
const z = 4

# a constant global
const global (x, y) = (2, 10)
```

## Special package variables

Special variables are package variables, some are writetable and can change the
behavoir of your programs while others are readonly and contain useful information
to make decisions important decisions.

### Type I special variables

We expand the content of special variables using the sigil `$`.

#### Example

```raku
say "Running #$0 on #$OS"
```
- `OS`: OS version on which `pity` was build
- `.`: current line in a file
- `,`: output field separator
- `/`: input record separator
- `Pity`: Pity version
- `"`: Separator character during interpolation
- `$`: Pid of the current pity program
- `0`: Program name
- `!`: retrieve errors from syscalls
- `_` : Topic variable, used mostly in blocks

### Type II special variable

We donot expand type 2 special variables with `$`

- `ENV`: a `Hash` which contains your current environment variables
- `PATH`: an `Array` which contains the absolute path to directories where pity searches for modules
- `INC`: a `Hash`, each key correspond to an imported module and have a value which correspond its location in the filesystem
- `SIG`: for traping signals, each signal is represented as a key and maps to a `Fun` which is to be executed whenever the signal is trapped
- `ARGV`: array containing command line arguments
- `ARGC`: represents the argument count, it is an object of type `Int`
- `DATA`: represents a file handle to manipulate data under `__DATA__`, just like in perl
- `__FUNC__`: for recursion, call the current function
- `__BLOCK__`: for recursion, call the current block
- `__FILE__`: a string object, represents the name of current script in execution

## Constants

- `π`: Pi, 3.14....
- `e`: Euler's number

# Objects

Pity has 16 builtin objects, types are objects and objects are types, check details on
each types here.

- Bool
- Pity
- Any
- Num
- Rat
- Str
- Array
- Hash
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
- Promise
- Laziness

# Flow control

Here is an overview of the `pity` syntax.

We separate statements with a generic newline or a semicolon in case we have more
than one statement on a single line.

1. Blocks

```raku
say 1
say 2; say 3
{ say 1 }; { say 4 }

{
    say "one"
    { say "two" }

    # recall the current block
    __BLOCK__
}
```

2. `do` Block

```raku
var v = do { 2 }
# "2"
say v

# "3"
do { 3 }.say

do { false } or die "failed"
```

3. `bg` Blocks

run a block asyncronously

```javascript
bg {
    4.sleep
    say "done"
}

# declare a function and assign it to "a"
var a = fun { sleep 4; say "done" }

# run function in "a" asyncronously and return a promise
var p = bg a.call

say "do stuffs"

# await promise p
fg p
```

4. `if`

Conditional `if` statement, note that paranthesis are optional.

`if ... { ... } [elsif ... { ... } ...] [else { ... }]`

```raku
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

```raku
var (u, y) = (5, nil)

# 5, 5
with u { say _, u }

# 5
with   y { say "never here" }
orwith u {:m say m }
else     { say "never here" }
```

6. `for`

```ruby
foreach "a", qr/regex/, [2, 4] { .say }

# output: 3 3 5 4 4
foreach i in ar { i.len.say }

ar = <one two three four five>
# "ar" is now [3, 3, 5, 4, 4]
for ar -> j {
    j = j.len
}

# (3, 3) (5, 4) (4, nil)
for ar {(i,j)
    say "(#i, #j)"
}

# set a custom value when we are running out of elements
# (3, 3) (5, 4) (4, none)
for ar -> i, j = "none" {
    say "(#i, #j)"
}

.say for ar
```

7. `gather`-`take`

`gather` is statement/block prefix which returns a sequence of values comming from
calls to `take` in the dynamic scope of block/function passed as argument to `gather`.

```raku
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

8. `given`-`when`

We implement the switch-case using `given`-`when`, this construct tests the topic variable
initialized to the argument passed to `given` against the following cases using the smartmatch
operator(`~~`). We execute the block of the first matching case and instantly exit the `given` block.
We can continue on to the next case by using the `proceed` instruction within the block of a case.

```raku
# output: Num, 42
given 34 {
  when Num { say "Num"; proceed }
  when 42  { say "42" }
  default  { say "Default" }
}
```

You can also use `given` as a standalone statement to specify the variable of concern in
the execution of a block.

```raku
x = [2, 5]
given x {
    say "variable x has two elements" if x.len == 2
}

print .map {(rx) rx ** 2 } given x
```

10. `loop`

Just like the C-for loop

general form: `loop initializer; condition; step { ... }`

```raku
loop var k = 0; k ≤ 20; k² { k.say }

# you can skip some parts
loop var k = 0;;k++ {
    k.say
    break if k == 10
}

loop { say "looping forever" }
```

12. `while` and `until`

The basic `while` and `until` loop.

```raku
k = 6
while k > 1 {
    k.say
    k--
}

until k == 0 {
    say "not entering here"
}
```

13. `repeat`-`while`/`until`

```raku
k = Set.new(2, 4, 5)
b = [2, 7, 3]

repeat {
    k.add(b.pop)
} while [2, 7] ∉ k

repeat {
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
        __BLOCK__
    }
    # dead code to be wiped by the compiler
    say "never gonna be executed"
}
```

16. `once`

`once` gives you the possibility to execute a statement within a loop only once regardless
of the number of iterations. One great advantage it offers is avoid the burdens of using a
conditional construct to avoid the execution of a statement.

```raku
var h = { one => 1, two => 1, three => 3 }
for h.each_kv {(k,v)
    once say 'only once!' if v == 1
    printfln "%s => %d", k, v
}

# for syntatic suger
for h.each_kv -> k, v {
    once say 'only once!' if v == 1
    printfln "%s => %d", k, v
}

```

17. `try`-`catch`-`finally`

`try`-`catch` for handling exceptions.

18. topic variable `_`

We've been using topic variables since the begining of this section without known what they are, a topic
variable is just an argument passed to an executing block, you can declare a topic variable to avoid the
default one(`_`), you can declare more than one topic variable to fetch the desired number of elements
for calls.

NOTE: topic variables should only be named at the begining of the block of concern.

```raku
var a = [2, 5, 34]

# declaring a topic variable x
print a.map {(x)
    once x++
    next if x == 3
    √x
}

# 2,2 4,none
[2, 2, 4].each {
    (x, y = "none")
    say "#x,#y"
}
```

# Functions

Pity has support for multiple dispatching, type checks and named arguments. Mixing named arguments
with unnamed ones brings a lot of confusion in your code and hence either you name all your arguments
or you don't name anything at all.

```raku
fun callme(code, n) { code.call(_) for ^n }
callme({ .say }, 5)

multi fun intro(name, age) {
    say "Hello, my name is #name, I'm #{age}yo"
}

multi fun intro(name) {
    say "Hello, my name is #name!"
}

intro("kueppo", "20")
intro("sarah")
intro(age => 5, name => liza)

# no candidates for this and thus fails at compile time
intro(age)

fun mul(Str str, Int k) { say str * k }
fun mul(Str str) { say str * 2 }

mul("one")
mul("two")
```

# Classes

```raku
class {
    method get() {
        
    }
}
```

# Regular expressions



```raku
```

# Promises and Concurrency

```raku
```

# Supply/React

```raku
```

# Packages

```raku
```

# Conclusion

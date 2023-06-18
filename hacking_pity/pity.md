# Pity

`Pity` is a multi-paradigm programming(functional and object oriented)language
inspired from the lovely `Perl`, `Raku` and `Lua` programming languages.

- Functional programing
- Object Oriented programming
- Promises and Concurrency
- Traits
- Multiple dispatching
- Type checks
- Closures
- Lazy Evaluation
- Pattern Matching using PCRE

# Operators

## Lonely operator

`…`, `...`: to specify unimplemented code

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

- `++`: (p,b) inc
- `--`: (p,b) dec
- `-`: (b) negative
- `+`: (b) absolute
- `~`: (b) binary complement
- `…` or `...`: (b) untight operator
- `^`: (p) `^5` return a list of element i.e `0` to `5`
- `⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`: (p) super-script power operators

## Named unary operators

- `defined`: (b) check if a varible is `nil` and return true otherwise
- `chop`:
- `chomp`:
- `chop`:
- `sleep`:
- ``:

## Named list operators

- `say`: (b) print to the standard output with a trailing new line
- `die`: program dies instantly
- `warn`: warn on stderr
- `print`: (b) print without a new line
- `printf`: (b) print formatted string without adding a trailing new line
- `printfln`:
- `sprintf`:
- `sprintfln`:
- `min`: (p) yield min from a list
- `max`: (p) yield max from a list
- `minmax`: (p) yield min and max, return it in a list

## Named binary operators

- `isa`: (i) checks if the left object `isa`(of the same class or kind of inherited) the right object

## Binary operators for pity objects

- `,`, `=>`: (i, b) comma operator, and key-value separator infix operator

- `!`: (p) negation operator ex: `!true == false`

- `=`, `:=`: (i) assignment and binding operator

- `//`: (i) `a // b`, return `a` it is set otherwise `b`

- `==` / `⩵  `, `!=` / ` ≠`, `>`, `>=` / `≥`, `<`, `<=` / `≤`: (i) basic operators between objects

- `+`, `-`, `/` / `÷`, `*`,  `%`, `..`, ``: (i) add, sub, div, div, mul, remainder and range operator.

- `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `%=`: (i) `left operand(a variable)` `=` `left operand value` `op` `right operand`

- `<<`, `>>`, `^`, `&`, `&=`, `|=`: (i) bitwise shift on left and right, logical `or` and `and`

- `&&`, `||`, `&&=`, `||=`: (i) logical "and" and "or" operator

- `≅  ` /  `=~`, `~~`: (i) regex operator and smart match operator

- `∉ `,  `∈  `, `∊`,  `∍ `, `∋ `,  `∌ `, `⊂  `, ` ⊄ ` , `⊆ `, `⊈  `, ` ⊃  `, `⊅ ` , ` ⊇  `, ` ⊉  `, `≡ `, `≢  `, `⊖`, `∖`, ``, ``, ``, ``: (i) set operators.

- `<=>` : `op1 <=> op2` says if `op1 < op2` yield `-1`, `op1 == op2` yield `0`, `op1 > op2` yield `1`

- `∘`: mathematic function composition, take two subroutines as operand

- `?:`: tenary operator

## List of all operators from highest precedence to lowest

- left        terms and list operators (leftward)
- left        method call operator (`.`)
- nonassoc    `++`, `--` and unary prefix `…` / `...`
- right       `** ⁰ ¹ ² ³ ⁴ ⁵ ⁶ ⁷ ⁸ ⁹`
- right       `!`, `~` `\` and unary `+` and `-`
- left        `=~`, `!~`
- left        `*`, `/`, `%`
- left        `+`, `-`, `.`, `∘`
- left        `∩ `
- left        `∪ `,  `⊖`, `⊎ `, `∖`
- chained     `∈ `, `∊`, `∉ `, `∋ `, `∍ `, `∌ `, `≡ `, `≢ `, `⊂ `, `⊄ `, `⊃ `, `⊅ `, `⊆ `, `⊈ `, `⊇ `, `⊉ `, `≼ `, `≽ `
- left        `<<`, `>>`
- nonassoc    named unary operators
- nonassoc    `isa`
- chained     `<`, `>`, `<=`, `>=`
- chain/na    `==`, `!=`, `<=>`, `~~`
- left        `&`
- left        `|`, `^`
- left        `&&`
- left        `||`, `//`
- nonassoc    `..`, lonely operator `…` / `...`
- right       `?:`
- right       `=`, `:=`, `&=`, `|=`, `&&=`, `||=`, `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `.=`, `%=`, `last`, `break`, `redo`, `dump`
- left        `,`, `=>`
- nonassoc    list operators (rightward)
- 

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
a = <one two three>

# [ "Three", "TWo", "One" ], Lennon Stella album :)
b = [a].map{ .capitalize }.reverse

# [ "0ne", "tw0", "three" ]
a =~ s<o>«0»
```

## Single character delimiter

We also have a restricted set of delimiter characters for quoted words(`qw`), double
quoted strings(`qq`), single quoted strings(`q`) and regex operators, Just like in Perl.

`/`, `|`, `%`

### Examples

```raku
let a = qw|ONE TWO THREE|
a.each{ .lc }

say q%interpolation won't work%

say qq/interpolation works, array: #a/

# [ "0ne", "Tw0" ]
b = a.grep{:x x =~ m|o| }.map{ s|o|0| }.map{ .ucfirst }
b.say
```

# Special variables

## Type I special variables

Expand the content of special variables with `$`

### Example
```raku
say "Running #$0 on #$OS"
```

- `OS`: OS version on which `pity` was build
- `_` : Topic variable, mostly in blocks
- `Pity`: Pity version
- `"`: Separator character during interpolation
- `$`: Pid of the current pity program
- `0`: Program name

## Type II special variable

We donot expand type 2 special variables with `$`

- `ARGV`: array of cmd line arguments, it is an object of type `Array`
- `ARGC`: represents the argument count, it is an object of type `Int`

# Constants

- `π`: pi
- `` :

# Variables

# Types

# Flow control

# Functions

# Classes

# Packages

# Regular expressions

# Promises

# Supply/React

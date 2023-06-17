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

### Lonely operator

`…`, `...`: to specify unimplemented code

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

- `++`: (p,b) inc
- `--`: (p,b) dec
- `-`: (b) negative
- `+`: (b) absolute
- `~`: (b) binary complement
- `…` or `...`: (b) untight operator

## Function like unary operator

- `say`: (b) print to the standard output with a trailing new line
- `print`: (b) print without a new line
- `printf`: (b) print formatted string without adding a trailing new line
- `defined`: (b) check if a varible is `nil` and return true otherwise
- `isa`: (i) check if the left object `isa`(of the same class or kind of inherited) the right object

## Binary operators for pity objects

- `,`, `=>`: comma operator, and key-value separator infix operator

- `!`: negation operator ex: `!true == false`

- `=`: (i) Assignment Operator

- `//`: (i) `a // b`, return `a` if set otherwise `b` (Perl feature)

- `==` / `⩵  `, `!=` or ` ≠`, `>`, `>=`, `<`, `<=`: (i) Object ==, !=, >, >=, <, <= respectively.

- `+`, `-`, `\`, `÷`, `*`,  `%`, `..`, ``: (i) add, sub, div, div, mul, remainder and range operator.

- `+=`, `/=` / `÷=`, `-=`, `//=`, `*=`, `.=` `%=`: (i) `left operand(a variable)` `=` `left operand value` `op` `right operand`

- `<<`, `>>`, `^`, `&`: (i)bitwise shift on left and right, logical `or` and `and`

- `=<<`, `=>>`, `=^`, `=&`: counterparts

- `&&`, `||`: logical "and" and "or" operator

- `&&=`, `||=`: counterparts

- `.`, `x`: string concat, string multiplication("o" x 2 give "oo")

- `...`, ``: destruction operator

- `≅  ` /  `=~`: 

- `∉ `,  `∈  `, `∋ `,  `∌ `, `⊂  `, ` ⊄ ` , `⊆ `, `⊈  `, ` ⊃  `, `⊅ ` , ` ⊇  `, ` ⊉  `, `≡ `, `≢  `: (i) set operators.

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

e.g `say "Running #$0 on #$OS"`

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

# Variables

# Types

# Flow control

# Functions

# Classes

# Packages

# Regular expressions

# Promises

# Supply/React

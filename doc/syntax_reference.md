# Types

- `Nil`: not a value
- `Bool`: Boolean value: `true` and `false`.
- `Int`: integer: `1`, `2`, `0`, `-1`, `∞ ` etc
- `String`: unicode strings `'literal'`, `"interpolation works here"`
- `Array`: Array of any types `["I", "Love", "Pity"]`, can also use paire delimiters to
avoid quotes when dealing with ascii encoded word

User can also use these delimiters to delimitate regular expressions.

```
( )       [ ]       { }       < >
« »       » «       ‹ ›       › ‹
„ ”       “ ”       ‘ ’       ‚ ’
〈 〉    〈 〉     《 》     「 」
『 』     【 】     〔 〕    〖 
〘 〙     〚 〛     ⌈ ⌉       ⌊ ⌋
❪ ❫       ❬ ❭       ❮ ❯       ❰ ❱
❲ ❳       ❴ ❵       ⟅ ⟆       ⟦ ⟧
⟨ ⟩       ⟪ ⟫       ⟬ ⟭       ⟮ ⟯
⦃ ⦄       ⦅ ⦆
⦋ ⦌       ⦍ ⦎       ⦏ ⦐       ⦑ ⦒
⦗ ⦘        ⧼ ⧽    〈 〉     ❨ ❩
```

- `Map`: key-value args `{key1 -> "Perl", "key2" -> 2}`, quote keys that are not ascii encoded.
- `Complex`: Complex numbers `1 + i`, `2 - 3i`, etc
- `Bytes`: series of bytes(8bits each)
- `Regex`: PCRE, `/[0-9]/`, more delimiters: `m{[0-9]}`, `m|[0-9]|`, `m<[0-9]>`, etc see pairs above.

# Unary operators

**p**: postfix, **i**: infix, **b**: prefix

## Basic unary operators

`++`: (p,b) inc
`--`: (p,b) dec
`-`: (b) negative
`+`: (b) absolute
`~`: (b) complement

## Extended unary operator

- `say`: (b) print to the standard output with a new line at the end
- `print`: (b) print without a new line
- `@`: (b) length of (array, hash values, string)
- `%`: (b) return keys of a hash in an array
- `defined`: (b) check if a varible is `nil` and return true otherwise.

# Binary operators

- `,`, `->`: comma operator, use `->` for readability in the hash datastructure.
- `!`: negation operator ex: `! true == false`
- `=`: (i) Assignment Operator
- `//`: (i) `a // b`, return `a` if set otherwise `b` (Perl feature)
- `==`, `!=`, `>`, `>=`, `<`, `<=`: (i) Numeric ==, !=, >, >=, <, <= respectively.
- `eq`, `ne`, `gt`, `ge`, `lt`, `le`: (i) String ==, !=, >, >=, <, <= respectively.
- `+`, `-`, `\`, `÷`, `*`,  `%`, `start..end[..step]`: (i) add, sub, div, div, mul, remainder and range operator.
- `+=`, `/=`, `-=`, `//=`, `÷=`, `*=`, `.=` `%=`: (i) `left operand(a variable)` `=` `left operand value` `op` `right operand`
- `<<`, `>>`, `^`, `&`: (i)bitwise shift on left and right, logical `or` and `and`
- `=<<`, `=>>`, `=^`, `=&`: counterparts
- `&&`, `||`: logical "and" and "or" operator
- `&&=`, `||=`: counterparts
- `.`, `x`: string concat, string multiplication("o" x 2 give "oo")

# Special variables 

## The ones with `$` prepended

- `OS`: OS version on which `pity` was build
- `_` : Topic variable, mostly in blocks
- `Pity`: Pity version
- `"`: array 
- `$`: pid of the current pity program
- `0`: program name

# Pity Objects 

All types are objects and support a set of method specific/common to them.

Paranthesis are optional in method and function call

## Number

## String

Let `s` be a string, `s` of type `String` supports the following methods

- `s.len`: return string length
- `s.gt(s1)`: return true if s gt s1 else false, see opts above
- `s.ge(s1)`: return true if s ge s1 else false
- `s.lt(s1)`: return true if s lt s1 else false
- `s.le(s1)`: return true if s le s1 else false
- `s.eq(s1)`: return true if s eq s1 else false
- `s.ne(s1)`: return true if s ne s1 else false
- `s.div(num)`: divide string `s` by `num`

> Well, this is new let me explain with an example
> `"ILovePity".div(3)` yields `["ILo", "veP", "ity"]`
> in case `num` is greater than `s.len` return
> `s` as a single element in the an `Array`.
> `"Pity".div(3)` yields `["Pit", "y"]`.

- `s.match(/regex/flags)`: return a matched object, see `Match` object below
- `s.gmatch(/regex/flags)`: global match, return an array of `Match` objects
- `s.sub()`:
- `s.gsub()`:
- `s.esub()`:
- `s.tr()`: transliteration s

- `s.uc`: uppercase s
- `s.ucfirst`: uppercase the first character of s
- `s.lc`: lowercase s
- `s.lcfirst`: lowercase the first character of s
- `s.fc`: foldcase s
- `s.split(delim)`: split s based on delimiter (`delim`: string or regex) which is a regex
- `s.cmp(s1)`: cmp `s` with `s1`: s eq s1 yield 0, s lt s1 yields -1, s gt s1 yield 1
- `s.mul(num)`: multiply `s` by `num`, just  like the `x` operator
- `s.append(s1)`, `s.concat(s1)`: append s1 to s
- `s.prepend(s1)`: prepend s1 to s
- `s.first(num)`: return the first `num` characters of s
- `s.last(num)`: return the last `num` characters of s
- `s.char(pos)`: return character at position `pos`, NB: zero indexed.
- `s.wordcase`: word case `s`, example `"i love afRica".wordcase` yield `"I Love Africa"`
- `s.capitalize(X)`: capitalize `s`
- `s.chop()`: (From Perl) remove the last character of s and return it
- `s.pop()`: remove the last character from s and return the rest
- `s.chomp()`: (From Perl) remove trailing new lines
- `s.bin()`: coerce s into a number and return it in base 2
- `s.oct()`: coerce s into a number and return it in base 8
- `s.hex()`: ..... s into ... and in base 16
- `s.crypt()`: crypt s, crypt from the C standard library
- `s.decode_base64()`: base64 decode `s`
- `s.encode_base65()`: base64 encode `s`
- `s.md5()`: 
- `s.sha()`:
- `s.clear()`: set s to an empty string
- `s.substr(offset, len)`: NB: inclusive
- `s.join([s1, s2, ...])`: join strings in argument to s
- `s.slice(start, end)`: negative numbers mean relative to the end of s, since s.char(-1) is s.char(s.len - 1); NB: inclusive
- `s.insert(string, pos, [len])`: insert at pos if len is 0 else replace `len` chars with `s1` starting from `pos`
- `s.index`:
- `s.rindex`:
- `s.ord`:
- `s.reverse`:
- `s.print`: print without new line
- `s.printf`: printf ...
- `s.sprintf`: sprintf ...
- `s.say`: print with new line
- `s.printlnf`: printf with new line
- `s.sprintflnf`: sprintf with new line

- `s.is_empty`: return bool, check if string is empty
- s.is_lowercase: ...
- s.is_uppercase: ...
- s.is_ascii: ...
- s.is_space: ...
- s.is_word: ...
- s.is_punct: ...
- s.is_alpha: ...
- s.is_alphanum: ...
- s.is_digit: ...
- s.is_xdigit: ...
- s.is_control: ...
- s.is_printable: ...
- s.is.graph: ...

## Array

## Hash

## 

## Pity is OO by default

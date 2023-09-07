# String

Let `s` be a variable whose content is a object of type `String`, `s` of type
`String` supports the following public methods:

- `s.len`: return string length
- `s.gt(s1)`: return true if `s` gt `s1` else false, see opts above
- `s.ge(s1)`: return true if `s` ge `s1` else false
- `s.lt(s1)`: return true if `s` lt `s1` else false
- `s.le(s1)`: return true if `s` le `s1` else false
- `s.eq(s1)`: return true if `s` eq `s1` else false
- `s.ne(s1)`: return true if `s` ne `s1` else false
- `s.div(num)`: divide string `s` by `num`

> Well, this is new let so me give an example
> `"ILovePity".div(3)` yields `["ILo", "veP", "ity"]`.
> In case `num` is greater than `s.len` then return
> an array containing `s` as the single element.
> also, `"Pity".div(3)` yields `["Pit", "y"]`.

- `s.match(/regex/flags)`: return a matched object, see `Match` object below
- `s.gmatch(/regex/flags)`: global match, return an array of `Match` objects
- `s.sub()`:
- `s.gsub()`:
- `s.esub()`:
- `s.tr()`: transliteration `s`

- `s.uc`: uppercase `s`
- `s.ucfirst`: uppercase the first character of `s`
- `s.lc`: lowercase `s`
- `s.lcfirst`: lowercase the first character of `s`
- `s.fc`: foldcase `s`
- `s.split(delim)`: split `s` based on delimiter (`delim`: string or regex) which is a regex
- `s.cmp(s1)`: cmp `s` with `s1`: `s` eq `s1` yield 0, `s` lt `s1` yields -1, `s` gt `s1` yield 1
- `s.mul(num)`: multiply `s` by `num`, just  like the `x` operator
- `s.append(s1)`, `s.concat(s1)`: append `s1` to `s`
- `s.prepend(s1)`: prepend `s1` to `s`
- `s.first(num)`: return the first `num` characters of `s`
- `s.last(num)`: return the last `num` characters of `s`
- `s.char(pos)`: return character at position `pos`, NB: zero indexed.
- `s.wordcase`: word case `s`, example `"i love afRica".wordcase` yield `"I Love Africa"`
- `s.capitalize(X)`: capitalize `s`
- `s.chop()`: (From Perl) remove the last character of `s` and return it
- `s.pop()`: remove the last character from `s` and return the rest
- `s.chomp()`: (From Perl) remove trailing new lines
- `s.bin()`: coerce `s` into a number and return it in base 2
- `s.oct()`: coerce `s` into a number and return it in base 8
- `s.hex()`: ..... `s` into ... and in base 16
- `s.crypt()`: crypt s, crypt from the C standard library
- `s.decode_base64()`: base64 decode `s`
- `s.encode_base65()`: base64 encode `s`
- `s.md5()`: 
- `s.sha()`:
- `s.clear()`: set `s` to an empty string
- `s.substr(offset, len)`: NB: inclusive
- `s.join([s1, s2, ...])`: join strings in argument to `s`
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
- `s.is_lowercase`: ...
- `s.is_uppercase`: ...
- `s.is_ascii`: ...
- `s.is_space`: ...
- `s.is_word`: ...
- `s.is_punct`: ...
- `s.is_alpha`: ...
- `s.is_alphanum`: ...
- `s.is_digit`: ...
- `s.is_xdigit`: ...
- `s.is_control`: ...
- `s.is_printable`: ...
- `s.is.graph`: ...

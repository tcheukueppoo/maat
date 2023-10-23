# String

Let `s` be a variable whose content is an object of type `Str`, `s` supports following public methods:

- `s.len`: Return string length
- `s.gt(s1)`: Return `True` if `s` gt `s1` else `False`, see opts below
- `s.ge(s1)`: Return `True` if `s` ge `s1` else `False`
- `s.lt(s1)`: Return `True` if `s` lt `s1` else `False`
- `s.le(s1)`: Return `True` if `s` le `s1` else `False`
- `s.eq(s1)`: Return `True` if `s` eq `s1` else `False`
- `s.ne(s1)`: Return `True` if `s` ne `s1` else `False`
- `s.div(Num n)`: Divide string `s` by `n`

Well, this is new so let me give an example:

`"ILoveMaat".div(3) -- returns a(ILo veM aat)`

In case `n` is greater than `s.len` then return an array containing `s` as the single element.
also, `"Maat".div(3)` returns `["Maa", "t"]`.

- `s.match(/regex/flags)`: Return a matched object, see `Match` object below
- `s.gmatch(/regex/flags)`: Global match, return an array of `Match` objects
- `s.sub`:
- `s.gsub`:
- `s.esub`:
- `s.tr`: Transliteration `s`

- `s.clone`: Return a new object which is a clone of `s`
- `s.uc`: Uppercase `s`
- `s.ucfirst`: Uppercase the first character of `s`
- `s.lc`: Lowercase `s`
- `s.lcfirst`: Lowercase the first character of `s`
- `s.fc`: Foldcase `s`
- `s.split(delim)`: Split `s` based on delimiter (`delim`: string or regex) which is a regex
- `s.cmp(s1)`: Cmp `s` with `s1`: `s` eq `s1` return 0, `s` lt `s1` returns -1, `s` gt `s1` yield 1
- `s.mul(Num n)`: Multiply `s` by `n`, just  like the `x` operator
- `s.append(s1)`, `s.concat(s1)`: Append `s1` to `s`
- `s.prepend(s1)`: Prepend `s1` to `s` and return the result in a new object
- `s.first(Num n)`: Return the first `n` characters of `s`
- `s.last(Num n)`: Return the last `n` characters of `s`
- `s.char(pos)`: Return character at position `pos`, NB: zero indexed.
- `s.wordcase`: Word case `s`, example `"i love BurkinA faSo".wordcase` return `"I Love Burkina Faso"`
- `s.capitalize(X)`: Capitalize `s`
- `s.chop -> Str`: Remove the last character of `s` and return it
- `s.pop`: Remove the last character from `s` and return the rest
- `s.chomp`: Remove trailing new lines
- `s.bin`: Coerce `s` into a number and return it in base 2
- `s.oct`: Coerce `s` into a number and return it in base 8
- `s.hex`: ..... `s` into ... and in base 16
- `s.crypt`: Crypt s, crypt from the C standard library
- `s.decode_base64`: Return base64 decoded `s`
- `s.encode_base65`: Return base64 encoded `s`
- `s.md5`: Return a md5 hashed `s`
- `s.sha1`: Return a sha1 hashed `s`
- `s.sha256`: Return a sha256 hashed `s`
- `s.sha512`: Return a sha512 hashed `s`
- `s.clear`: Set `s` to an empty string
- `s.substr(offset, len)`: NB: inclusive
- `s.join(Any s0 [, Any s1, Any s2, ...]) -> Str`: Join strings in argument to `s` and return the results
- `s.slice(Num start, Num end) -> Str`: Negative numbers mean relative to the end of s, since s.char(-1) is s.char(s.len - 1); NB: inclusive
- `s.insert(Str s1, Num pos [, Num len ]) -> Str`: Insert at pos if len is 0 else replace `len` chars with `s1` starting from `pos`
- `s.index`:
- `s.rindex`:
- `s.ord`:
- `s.rev`: Return reversed `s`
- `s.say`: Print `s` to the standard output with a trailing new line
- `s.print`: Print `s` to the standard output without a trailing new line
- `s.dump`: Same as above

- `s.print`: Print without new line
- `s.printf`: Printf ...
- `s.sprintf`: Sprintf ...
- `s.printlnf`: Printf with new line
- `s.sprintflnf`: Sprintf with new line

- `s.is_empty -> Bool`: Return bool, check if string is empty
- `s.is_lc -> Bool`: ...
- `s.is_uc -> Bool`: ...
- `s.is_ascii -> Bool`: ...
- `s.is_space -> Bool`: ...
- `s.is_word -> Bool`: ...
- `s.is_punct -> Bool`: ...
- `s.is_alpha -> Bool`: ...
- `s.is_alphanum -> Bool`: ...
- `s.is_digit -> Bool`: ...
- `s.is_xdigit -> Bool`: ...
- `s.is_control -> Bool`: ...
- `s.is_printable -> Bool`: ...
- `s.is.graph -> Bool`: ...

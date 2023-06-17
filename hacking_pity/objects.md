# Objects

- `Nil`: not a value
- `Bool`: Boolean value: `true` and `false`.
- `Int`: integer: `1`, `2`, `0`, `-1`, `∞ ` etc
- `String`: unicode strings `'literal'`, `"interpolation works here"`
- `Array`: Array of any types `["I", "Love", "Pity"]`, can also use paire delimiters to avoid quotes when dealing with ascii encoded word
- `Hash`: key-value args `{key1 -> "Perl", "key2" -> 2}`, quote keys that are not ascii encoded.
- `Complex`: Complex numbers `1 + i`, `2 - 3i`, etc
- `Bytes`: series of bytes(8bits each)
- `Regex`: PCRE, `/[0-9]/`, more delimiters: `m{[0-9]}`, `m|[0-9]|`, `m<[0-9]>`, etc see pairs above.
- `Promise`: working with promises
- `Supply`: raku's supply for data stream

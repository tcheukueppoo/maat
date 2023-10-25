# MAAT's CUSTOM BNF

## Terminologies

- 
-
-

```
# Terminals
Term_var_name: 
Term_comment: qr/ #.* \z /x
Term_comment: qr/ --- .*? --- /x
Term_whitespace: \R | \h

separators           -> Term_comment | Term_whitespace 
statements           -> separators [ separators ... ] statement [ separators [ separators ... ] statement ]
statement            -> declaration | expression | flow_constructs
delcaration          -> variable_declaration | block_declaration | class_declaration
block_declaration    -> named_block_declaration | function_declaration
class_declaration    -> class Term_class_name
variable_declaration -> var_modifier variables '=' expression
variables            -> Term_varname | '(' Term_varname [ ',' Term_varname ',' ... ] ')'
expresion            -> builtin_object_declaration | do_block | literal | tenary_op
flow_construct       ->
```
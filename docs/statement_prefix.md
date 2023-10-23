# Implementing Statement Prefixes

`Maat` supports the following statement prefixes:

1. `do`
2. `ma`

Statement prefixes are keywords preceding statements that changes
their default behavoir. Internally, this changes the way the Maat
compiler compiles Maat code and thus generates different type of
opcodes for the intention of the statement prefix to take effect
during bytecode execution.

## do

```
do FILENAME
do { CODE }
do if ...
do for ...
do with ...
```

The `do` statement prefix treats statements as expressions and returns
the value of the last evaluation in the statement of concern.

Given the following maat code

```raku
var x = do { 2 ** 4 } + 14                # output: 
var y = 12 + do if True { 2 } else { 3 }  # output:
```

Here the Maat compiler compiles the `do` statment and at execution, it
puts the result of the last evaluated expression of that statement in
the required register to serve as operand for the arithmetic addition
operation.

```
var x = 10
var m = do for ^5 -> w { x += 1 }

-- output: a{11 12 13 14 15}
m.say
```

### ma

```
ma for ...
```



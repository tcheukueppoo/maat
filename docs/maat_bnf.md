# Syntax Of The Maat Programming Language

* `#` is for comments
* `{X}` stands for 0 or more of `X`
* `[X]` implies `X` is optional
* `|` stands for alternation

```
<block> ::= '{' {<stat>} '}'

<stat> ::=    ';'
            | <expr_list>                         # An expression is a special case of statement returning a value
            | <label_name> ':'
            | <loop_control> [<label_name> | <number>]
            | <func_def>
            | <class_def>
            | <role_def>
            | <package_def>
            | <vars_def>
            | <stat_modifier>
            | <vars_assign>
            | <loop_stat>
            | <flow_control>
            | try <block> {catch <> <block>} [finally <block>]
              defer <expr_or_block>

<expr_list> ::= <expr> {, <expr>}

<label_name> ::= <name>

<loop_control> ::= break | next | redo | goto

<expr_or_block> ::= <expr> | <block>

<flow_control> ::=   if <expr> [<topic_var>] <block> {elsif <expr> [<topic_var>] <block>} [else <block>]
                   | with <expr> [<topic_var>] <block> {orwith <expr> [<topic_var>] <block>} [else <block>]
                   | given <expr> [<topic_var>] <block>

<topic_var> ::= '->' <var>

<var> ::= <name>

<loop> ::=   loop <expr> <block> 
           | for <expr> [<topic_vars>] <block>
           | while <expr> <block>
           | until <expr> <block>

<topic_vars> ::= <topic_var> {, <var>}

<stat_modifier> ::= <expr> <stat_kind> <expr>

<stat_kind> ::= if | unless | given | when | until | when

<func_def> ::= fn <name> ['(' ')'] [':' <func_trait>] <block>

<func_trait> ::= save | gen

<class_def> ::= class <name> <class_link> <class_body>

<role_def> ::= role <name> <role_link> <class_body>

<class_body> ::=

<expr> ::=   <nil>
           | <boolean>
           | <number>
           | <string_literal>
           | <regex_literal>
           | <anonymous_func>
           | <map>
           | <array>
           | <expr> <infix_op> <expr>
           | <expr> <postfix_op>
           | <prefix_op> <expr>
           | <stat_expr>
           | <expr_group>

<stat_expr> ::= <stat_prefix> <>

```



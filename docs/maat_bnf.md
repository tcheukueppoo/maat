# Syntax Of The Maat Programming Language

* `{X}` stands for 0 or more of `X`
* `[X]` implies `X` is optional
* `|` stands for alternation
* `(X)` for grouping, `X` is considered a single unit

```
<block> ::= '{' {<stat>} '}'

<stat> ::=    <sep>
            | <exp_list>
            | <label_name> ':'
            | (break | next | redo | goto) [<label_name> | <loop_number>]
            | <package_def>
            | <var_def>
            | <package_def>
            | <func_def>
            | <class_def>
            | <role_def>
            | <stat_modifier>
            | <vars_assign>
            | <loops>
            | <flows>
            | use <ns_name> ['(' [<symbol>] {',' <symbol>} ')']
            | load <string_literal>
            | try <block> {catch <> <block>} [finally <block>]
            | defer (<exp_list> | <block>)

<sep> ::= ';' | <new_line> | <generic_new_line>

<exp_list> ::= <exp> {',' {','} <exp> {','}}

<label_name> ::= <name>

<package_def> :: package <ns_name> [<block>]

<var_def> ::= <scope> <var_assign>
<scope>   ::= let | our
<var_tmp> ::= tmp <var_assign>

<var_assign> ::= ('(' (<var> {','} | <star_var> {<__var>} | {<var__>} <star_var> | <var__> <star_var> <__var>) ')' | <var>) '=' <exp_list>
<star_var>   ::= '*' <var>
<__var>      ::= ',' {','} <var>
<var__>      ::= <var> ',' {','}

<flows> ::=   if <exp_list> [<topic_var>] <block> {elseif <exp_list> [<topic_var>] <block>} [else <block>]
            | with <exp_list> [<topic_var>] <block> {orwith <exp_list> [<topic_var>] <block>} [else <block>]
            | given <exp_list> [<topic_var>] <block>

<topic_var> ::= '->' <var>

<var> ::= <name>

<stat_modifier> ::= <exp_list> (if | unless | when | given | for | until | while) <exp_list>

<loops> ::=   loop [[<exp_list>] ';' [<exp_list>] ';' [<exp_list>]] <block> 
            | for <exp_list> [<topic_vars>] <block>
            | while <exp_list> <block>
            | until <exp_list> <block>

<topic_vars> ::= '->' <var> {'=' <exp>} {',' <var> {'=' <exp>}}

<func_def> ::= fn <name> ['(' {<func_args>} ')'] [':' (save | gen)] <func_body>

<func_args> ::= <func_arg> {',' <func_arg>} ['*' <var>]
<func_arg>  ::= <var> [('=' | '=//') <exp>]
<func_body> ::= <block>

<package_def> ::= package <ns_name> [<block>]

<class_def> ::= [<scope>] class <name> ([<rel>] [<identity>] | [<identity>] [<rel>]) <class_body>

<identity> ::= ':' i
<rel>      ::= ':' (is | does) <rel_args>

<rel_args>      ::= '(' [<name__ns_name> {',' <name__ns_name>}] ')'
<name__ns_name> ::= <name> | <ns_name>

<class_body> ::= '{' {<method_def> | <stat>} '}'

<role_def>  ::= [<scope>] role <name> [':' does <rel_args>] [<identity>] <role_body>
<role_body> ::= <class_body>

<exp> ::=    <nil>
           | <boolean>
           | <number>
           | <string_literal>
           | <regex_literal>
           | <anonymous_func>
           | <map>
           | <array>
           | <exp> <infix_op> <exp>
           | <exp> <postfix_op>
           | <prefix_op> <exp>
           | <stat_exp>
           | <

<nil> ::=

<boolean> ::=

<number> ::=

<regex_literal> ::=

<anonymous_func> ::= [fn] '{' ['|' <func_args> '|'] {<stat>} '}' | ':' <exp_list>

<string_literal> ::=

<map> ::= 

<array> ::= '[' {<exp>} ']' | '@a' <> <>

<stat_exp> ::= <stat_prefix> <>
<exp_group> ::=

```

An expession is a special case of statement returning a value

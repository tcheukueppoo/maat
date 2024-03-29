# Syntax Of The Maat Programming Language

* `{X}` stands for 0 or more of `X`
* `[X]` implies `X` is optional
* `|` stands for alternation
* `(X)` for grouping, `X` is considered a single unit

```
<block> ::= '{' {<stat>} '}'

<stat> ::=   <sep>
           | <exp_list>
           | <label>
           | (break | next | redo | goto) [<name> | <loop_id>]
           | <package_def>
           | <var_def>
           | <package_def>
           | <func_def>
           | <class_def>
           | <role_def>
           | <stat_modifier>
           | <any_var_assign>
           | <loops>
           | <flows>
           | use <ns_name> ['(' [<symbol> {',' <symbol>}] ')']
           | load <string_literal>
           | try <block> {catch <> <block>} [finally <block>]
           | defer (<exp_list> | <block>)
           | <unimplemented>

<unimplemented> ::= '…' | '...'
<sep>           ::= ';' | <new_line> | <generic_new_line>

<exp_list> ::= <exp> {<commas> <exp> {','}}

<commas> ::= ',' {','}

<label> ::= <name> ':'

<package_def> ::= package <ns_name> [<block>]

<var_def>         ::= <lex_var_def> | <package_var_def> | <state_var_def> | <tmp_var>
<lex_var_def>     ::= let   <var_assign>
<package_var_def> ::= our   <var_assign>
<state_var_def>   ::= state <var_assign>
<tmp_var>         ::= temp  <any_var_assign>

<var_assign>     ::= ('(' <var_list>     {','} ')' | <var>    ) '=' <exp>
<any_var_assign> ::= ('(' <any_var_list> {','} ')' | <any_var>) '=' <exp>

<any_var>            ::= <fq_package_var> | <lex_or_package_var> | <special_var>
<lex_or_package_var> ::= <name>
<special_var>        ::= '$' <name>

<var_list> ::=   <var__> <star_var> <__var>
               | <var> {<__var>}
               | <star_var> {<__var>}
               | {<var__>} <star_var>

<any_var_list> ::=   <any_var__> <star_any_var> <__any_var>
                   | <any_var> {<__any_var>}
                   | <star_any_var> {<__any_var>}
                   | {<any_var__>} <star_any_var>

<star_var> ::= '*' <var>      [<trait_immut>]
<__var>    ::= <commas> <var> [<trait_immut>]
<var__>    ::= <var>          [<trait_immut>] <commas> 

<trait_immut> ::= ':' im

<star_any_var> ::= '*' <any_var>      [<trait_immut>]
<__any_var>    ::= <commas> <any_var> [<trait_immut>]
<any_var__>    ::= <any_var>          [<trait_immut>] <commas>

<flows> ::=   given <exp_list> [<topic_var>] <block>
            | if    <exp_list> [<topic_var>] <block> {elseif <exp_list> [<topic_var>] <block>} [else <block>]
            | with  <exp_list> [<topic_var>] <block> {orwith <exp_list> [<topic_var>] <block>} [else <block>]

<topic_var> ::= '->' <var>
<var>       ::= <name>

<stat_modifier> ::= <exp_list> (if | unless | when | given | for | until | while) <exp_list>

<loops> ::=   for   <exp_list> [<topic_vars>]                        <block>
            | while <exp_list>                                       <block>
            | until <exp_list>                                       <block>
            | loop  [[<exp_list>] ';' [<exp_list>] ';' [<exp_list>]] <block>

<topic_vars> ::= '->' <var> ['=' <exp>] {',' <var> ['=' <exp>]}

<func_def> ::= [mul] fn <name> ['(' [<func_args>] ')'] [':' (save | gen)] <func_body>

<func_args> ::=   <norm_arg> {',' <norm_arg>} [',' <star_arg>] [',' <2star_arg>]
                | <star_arg> ',' <2star_arg>
                | <star_arg>
                | <2star_arg>

<norm_arg>  ::= <var> [('=' | '=//') <exp>]
<star_arg>  ::= <star_var>
<2star_arg> ::= '*' <star_arg>
<func_body> ::= <block>

<class_def>           ::= [<scope>] class <name> [<class_rel_and_trait>] <class_body>
<class_rel_and_trait> ::=   <class_rel> <trait_main>
                          | <trait_main> <class_rel>
                          | <class_rel>
                          | <trait_main>

<class_rel>     ::= ':' (is | does) '(' [<rel_args>] ')'
<rel_args>      ::= <name__ns_name> {',' <name__ns_name>}
<name__ns_name> ::= <name> | <ns_name>

<trait_main> ::= ':' m

<scope> ::= let | our

<class_body> ::= '{' ({(<class_attr_def> | <state_var_def> | <lex_var_def>)} {<method_def>} | <unimplemented>) '}'

<class_attr_def> ::= has <name> [':' (rw | ro | built)] ['=' <expr>]

<method_def>  ::= [mul] meth ['(' [<func_args>] ')'] [':' io] <method_body>
<method_body> ::= <func_body>

<role_def>           ::= [<scope>] role <name> [<role_rel_and_trait>]  <role_body>
<role_rel_and_trait> ::=   <role_rel> <trait_main>
                         | <trait_main> <role_rel>
                         | <trait_main>
                         | <role_rel>

<role_rel> ::= ':' does '(' [<rel_args>] ')'

<role_body> ::= <class_body>

<exp> ::=   <nil>
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

<nil> ::= nil

<boolean> ::= true | false

<number> ::=

<regex_literal> ::= '@r'  | '/' <> '/'

<anonymous_func> ::= [fn] '{' ['|' <func_args> '|'] {<stat>} '}' | ':' <exp>

<string_literal> ::= <single_quoted> | <double_quoted> | <back_quoted>

<map> ::= '{' '}' | '@m'

<array> ::= '[' {<exp>} ']' | '@a' <> <>

<stat_exp> ::= <stat_prefix> <>

<exp_group> ::=

```

An expession is a special case of statement returning a value

# "Syntax" Of "The" Maat "Programming" Language

* `{X}` "stands" for "0" or "more" of `X`
* `["X"]` "implies" `X` "is" optional
* `|` "stands" for alternation
* `(X)` "for" grouping, `X` "is" considered "a" single unit

```
Block = "{" { Stat } "}" .

Stat =   separator 
       | unimpl
       | ExpList
       | Label
       | StatMod
       | Loop
       | LoopCtrl
       | Flow
       | Defer 
       | Try
       | Use
       | Load .

ExpList = Exp { Commas Exp } { "," } .

Commas = "," { "," } .

Label = name ":" .

Defer = "defer" ( Exp | Block ) .

Try = "try" Block { "catch" Exp Block } [ "finally" Block ] .

Use = "use" namespace [ "(" [ name { "," name } ] ")" ] .

Load = "load" StringLiteral .

Def =   Class
      | Role
      | Func
      | VarDef
      | Package .

Package = "package" namespace [ Block ] .

VarDef = ( [ "const" ] ( [ scope ] | "state" ) LexNames | "const" LexNames | "temp" AnyNames ) [ EqExp ] .

EqExp = "=" Exp .

LexNames = "(" LexNameList { "," } ")" | name .

LexNameList =   Name__ StarName __Name
              | name { __Name }
              | StarName { __Name }
              | { Name__ } StarName .

StarName = "*" name    [ immut ] .
__Name   = Commas name [ immut ] .
Name__   = name        [ immut ] Commas .

AnyNames = "(" AnyNameList { "," } ")" | any_name .

AnyNameList =   any_name StarAnyName __AnyName
              | any_name { __AnyName }
              | StarAnyName { __AnyName }
              | { AnyName__ } StarAnyName .

StarAnyName = "*" any_name    [ immut ] .
__AnyName   = Commas any_name [ immut ] .
AnyName     = any_name        [ immut ] Commas .

Flow =   "given" ExpList [ TopicVar ] Block
       | "if"    ExpList [ TopicVar ] Block { elseif ExpList [ TopicVar ] Block } [ else Block ]
       | "with"  ExpList [ TopicVar ] Block { orwith ExpList [ TopicVar ] Block } [ else Block ] .

TopicVar = "->" name .

StatMod = ExpList ( conditional_keyword | loop_keyword ) ExpList .

Loop =   "for"   ExpList [ TopicVars ]                           Block
       | "while" ExpList                                         Block
       | "until" ExpList                                         Block
       | "loop"  [ [ ExpList ] ";" [ ExpList ] ";" [ ExpList ] ] Block .

LoopCtrl = ( "break" | "next" | "redo" | "goto" ) [ name | loop_id ] .

TopicVars = "->" name [ EqExp ] { "," name [ EqExp ] } .

Func = [ "const" ] [ scope ] [ "mul" ] "fn" name [ "(" [ FuncArgs ] ")" ] [ ":save" | ":gen" ] FuncBody .

FuncArgs =   NormArg { "," NormArg } [ "," StarArg ] [ "," SStarArg ]
           | StarArg "," SStarArg
           | StarArg
           | SStarArg .

NormArg  = name [ ( "=" | "=//" ) Exp ] .
StarArg  = StarName .
SStarArg = "*" StarArg .
FuncBody = Block .

Class            = [ "const" ] [ scope ] "class" name [ ClassRelAndTrait ] ClassBody
ClassRelAndTrait =   ClassRel main
                   | main ClassRel
                   | ClassRel
                   | main .

ClassRel     = (  ":is(" | ":does("  ) [ RelArgs ] ")"
RelArgs      = NameOrFqName { "," NameOrFqName } .
NameOrFqName = name | fq_name .


ClassBody = "{" ( { ( ClassAttr | ValidInClassStat ) } { ( Method | ValidInClassStat ) } | uimpl ) "}" .

ClassAttr = "has" name [ ":rw" | ":ro" | ":built" ] [ EqExp ] .

ValidInClassStat =   Def
                   | StatMod
                   | Loop
                   | Flow
                   | Defer
                   | Try
                   | ExpList .

Method     = [ "mul" ] "meth" [ "(" [ FuncArgs ] ")" ] [ ":io" ] MethodBody .
MethodBody = FuncBody .

Role            = [ "const" ] [ scope ] "role" name [ RoleRelAndTrait ] RoleBody .
RoleRelAndTrait =   RoleRel main
                  | main RoleRel
                  | main
                  | RoleRel .

RoleRel = ":does(" [ RelArgs ] ")" .

RoleBody = ClassBody .

Exp =   nil
      | boolean
      | number
      | string_literal
      | regex_literal
      | AnonymousFunc
      | Array
      | Map
      | VarDef
      | Exp infix_op Exp
      | Exp postfix_op
      | prefix_op Exp
      | StatExp
      |

AnonymousFunc = [ "fn" ] "{" [ "|" FuncArgs "|" ] { Stat } "}" | ":" Exp .

Map = DefaultArray | QuotedMap .

Array = DefaultArray | QuotedArray .

StatExp = StatPrefix <>

ExpGroup =

/* Lexical tokens */

separator = ";" .

unimpl = "…" | "..." .

immut = ":im" .
main  = ":m" .

nil     = "nil" .
boolean = "true" | "false" .

number =

scope               = "let" | "our" .
loop_keyword        = "for" | "until"  | "while" .
conditional_keyword = "if"  | "unless" | "when" | "given" .

name        = 
_namespace  = name { "::" name } .
namespace   = "::" name | name "::" _namespace .
fq_name     = namespace | _namespace [ ":::" name ] .
any_name    = fq_name | name | special_var .
special_var = "$" name .

string_literal           = single_quoted | double_quoted | backquoted | string_quoted_with_pairs
single_quoted            = 
double_quoted            =
backquoted               =
string_quoted_with_pairs = 

regex_literal           = default_regex_literal | regex_quoted_with_pairs
default_regex_literal   =
regex_quoted_with_pairs = 
```

An expession is a special case of statement returning a value

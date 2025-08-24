**Program:**

![Program](diagram/Program.png)

```
Program  ::= TopLevelDecl*
```

**TopLevelDecl:**

![TopLevelDecl](diagram/TopLevelDecl.png)

```
TopLevelDecl
         ::= UseDecl
           | FuncDef
           | ExternDecl
```

referenced by:

* Program

**UseDecl:**

![UseDecl](diagram/UseDecl.png)

```
UseDecl  ::= 'use' PkgId ( 'as' Ident )? ';'
```

referenced by:

* TopLevelDecl

**PkgId:**

![PkgId](diagram/PkgId.png)

```
PkgId    ::= Ident ( '.' Ident )*
```

referenced by:

* UseDecl

**Type:**

![Type](diagram/Type.png)

```
Type     ::= 'bool'
           | 'int8'
           | 'int16'
           | 'int32'
           | 'int64'
           | 'uint8'
           | 'uint16'
           | 'uint32'
           | 'uint64'
           | 'fp32'
           | 'fp64'
```

referenced by:

* DeclInit
* DeclStmt

**DeclStmt:**

![DeclStmt](diagram/DeclStmt.png)

```
DeclStmt ::= 'mut'? ( Type | 'var' ) Ident ( '=' Expr )? ';'
```

referenced by:

* Stmt

**Stmt:**

![Stmt](diagram/Stmt.png)

```
Stmt     ::= Block
           | DeclStmt
           | ( AssignStmt | ExprStmt | ReturnStmt ) ';'
           | IfStmt
           | WhileStmt
           | ForStmt
           | TernaryStmt
```

referenced by:

* Block
* ForStmt
* IfStmt
* TernaryStmt
* WhileStmt

**Block:**

![Block](diagram/Block.png)

```
Block    ::= '{' Stmt* '}'
```

referenced by:

* FuncDef
* Stmt

**AssignStmt:**

![AssignStmt](diagram/AssignStmt.png)

```
AssignStmt
         ::= Place AssignOp Expr
```

referenced by:

* ForInit
* ForStep
* Stmt

**AssignOp:**

![AssignOp](diagram/AssignOp.png)

```
AssignOp ::= '='
           | '+='
           | '-='
           | '*='
           | '/='
           | '%='
           | '<<='
           | '>>='
           | '&='
           | '^='
           | '|='
```

referenced by:

* AssignStmt

**Place:**

![Place](diagram/Place.png)

```
Place    ::= Ident
```

referenced by:

* AssignStmt
* IncDecPrefix
* Postfix

**ExprStmt:**

![ExprStmt](diagram/ExprStmt.png)

```
ExprStmt ::= Expr
```

referenced by:

* Stmt

**TernaryStmt:**

![TernaryStmt](diagram/TernaryStmt.png)

```
TernaryStmt
         ::= Expr '?' Stmt ':' Stmt
```

referenced by:

* Stmt

**IfStmt:**

![IfStmt](diagram/IfStmt.png)

```
IfStmt   ::= 'if' '(' Expr ')' Stmt ( 'else' Stmt )?
```

referenced by:

* Stmt

**WhileStmt:**

![WhileStmt](diagram/WhileStmt.png)

```
WhileStmt
         ::= 'while' '(' Expr ')' Stmt
```

referenced by:

* Stmt

**ForStmt:**

![ForStmt](diagram/ForStmt.png)

```
ForStmt  ::= 'for' '(' ForInit ';' Expr? ';' ForStep ')' Stmt
```

referenced by:

* Stmt

**ForInit:**

![ForInit](diagram/ForInit.png)

```
ForInit  ::= ( DeclInit | AssignStmt )?
```

referenced by:

* ForStmt

**DeclInit:**

![DeclInit](diagram/DeclInit.png)

```
DeclInit ::= 'mut'? ( Type | 'var' ) Ident '=' Expr
```

referenced by:

* ForInit

**ForStep:**

![ForStep](diagram/ForStep.png)

```
ForStep  ::= AssignStmt?
```

referenced by:

* ForStmt

**ReturnStmt:**

![ReturnStmt](diagram/ReturnStmt.png)

```
ReturnStmt
         ::= 'return' Expr?
```

referenced by:

* Stmt

**Expr:**

![Expr](diagram/Expr.png)

```
Expr     ::= LogicOr
```

referenced by:

* AssignStmt
* DeclInit
* DeclStmt
* ExprStmt
* ForStmt
* IfStmt
* Primary
* ReturnStmt
* TernaryStmt
* WhileStmt

**LogicOr:**

![LogicOr](diagram/LogicOr.png)

```
LogicOr  ::= LogicAnd ( '||' LogicAnd )*
```

referenced by:

* Expr

**LogicAnd:**

![LogicAnd](diagram/LogicAnd.png)

```
LogicAnd ::= BitOr ( '&&' BitOr )*
```

referenced by:

* LogicOr

**BitOr:**

![BitOr](diagram/BitOr.png)

```
BitOr    ::= BitXor ( '|' BitXor )*
```

referenced by:

* LogicAnd

**BitXor:**

![BitXor](diagram/BitXor.png)

```
BitXor   ::= BitAnd ( '^' BitAnd )*
```

referenced by:

* BitOr

**BitAnd:**

![BitAnd](diagram/BitAnd.png)

```
BitAnd   ::= Equality ( '&' Equality )*
```

referenced by:

* BitXor

**Equality:**

![Equality](diagram/Equality.png)

```
Equality ::= Rel ( ( '==' | '!=' ) Rel )*
```

referenced by:

* BitAnd

**Rel:**

![Rel](diagram/Rel.png)

```
Rel      ::= Shift ( ( '<' | '<=' | '>' | '>=' ) Shift )*
```

referenced by:

* Equality

**Shift:**

![Shift](diagram/Shift.png)

```
Shift    ::= Add ( ( '<<' | '>>' ) Add )*
```

referenced by:

* Rel

**Add:**

![Add](diagram/Add.png)

```
Add      ::= Mul ( ( '+' | '-' ) Mul )*
```

referenced by:

* Shift

**Mul:**

![Mul](diagram/Mul.png)

```
Mul      ::= Prefix ( ( '*' | '/' | '%' ) Prefix )*
```

referenced by:

* Add

**Prefix:**

![Prefix](diagram/Prefix.png)

```
Prefix   ::= ( '+' | '-' | '!' | '~' )* ( IncDecPrefix | Postfix )
```

referenced by:

* Mul

**IncDecPrefix:**

![IncDecPrefix](diagram/IncDecPrefix.png)

```
IncDecPrefix
         ::= ( '++' | '--' ) Place
```

referenced by:

* Prefix

**Postfix:**

![Postfix](diagram/Postfix.png)

```
Postfix  ::= Place PostOp*
           | Primary
```

referenced by:

* Prefix

**PostOp:**

![PostOp](diagram/PostOp.png)

```
PostOp   ::= '++'
           | '--'
```

referenced by:

* Postfix

**Primary:**

![Primary](diagram/Primary.png)

```
Primary  ::= Ident
           | Literal
           | '(' Expr ')'
```

referenced by:

* Postfix

**FuncDef:**

![FuncDef](diagram/FuncDef.png)

```
FuncDef  ::= 'func' Ident '(' ParamList? ')' RetType Block
```

referenced by:

* TopLevelDecl

**ExternDecl:**

![ExternDecl](diagram/ExternDecl.png)

```
ExternDecl
         ::= 'extern' StringLiteral? 'func' Ident '(' ParamList? ')' R
```

referenced by:

* TopLevelDecl

##  

![rr-2.5](diagram/rr-2.5.png) <sup>generated by [RR - Railroad Diagram Generator][RR]</sup>

[RR]: https://www.bottlecaps.de/rr/ui
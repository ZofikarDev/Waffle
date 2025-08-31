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

**Ident:**

![Ident](diagram/Ident.png)

```
Ident    ::= ( Letter | '_' ) ( Letter | Digit | '_' )*
```

referenced by:

* Decl
* FuncDecl
* Param
* PkgId
* Ref
* TailNoCall
* TailWithCall
* Type
* UseDecl

**Letter:**

![Letter](diagram/Letter.png)

```
Letter   ::= [A-Za-z]
```

referenced by:

* Ident

**DecDigit:**

![DecDigit](diagram/DecDigit.png)

```
DecDigit ::= [0-9]
```

referenced by:

* DecDigits

**DecDigits:**

![DecDigits](diagram/DecDigits.png)

```
DecDigits
         ::= DecDigit ( '_'? DecDigit )*
```

referenced by:

* DecimalInteger
* FloatLiteral

**HexDigit:**

![HexDigit](diagram/HexDigit.png)

```
HexDigit ::= [0-9A-Fa-f]
```

referenced by:

* HexDigits

**HexDigits:**

![HexDigits](diagram/HexDigits.png)

```
HexDigits
         ::= HexDigit ( '_'? HexDigit )*
```

referenced by:

* HexInteger

**BinDigit:**

![BinDigit](diagram/BinDigit.png)

```
BinDigit ::= [0-1]
```

referenced by:

* BinDigits

**BinDigits:**

![BinDigits](diagram/BinDigits.png)

```
BinDigits
         ::= BinDigit ( '_'? BinDigit )*
```

referenced by:

* BinInteger

**BooleanLiteral:**

![BooleanLiteral](diagram/BooleanLiteral.png)

```
BooleanLiteral
         ::= 'true'
           | 'false'
```

referenced by:

* Literal

**HexInteger:**

![HexInteger](diagram/HexInteger.png)

```
HexInteger
         ::= ( '0x' | '0X' ) HexDigits
```

referenced by:

* IntegerLiteral

**BinInteger:**

![BinInteger](diagram/BinInteger.png)

```
BinInteger
         ::= ( '0b' | '0B' ) BinDigits
```

referenced by:

* IntegerLiteral

**DecimalInteger:**

![DecimalInteger](diagram/DecimalInteger.png)

```
DecimalInteger
         ::= '0'
           | [1-9] DecDigits?
```

referenced by:

* FloatLiteral
* IntegerLiteral

**IntegerLiteral:**

![IntegerLiteral](diagram/IntegerLiteral.png)

```
IntegerLiteral
         ::= DecimalInteger
           | BinInteger
           | HexInteger
```

referenced by:

* Literal

**FloatLiteral:**

![FloatLiteral](diagram/FloatLiteral.png)

```
FloatLiteral
         ::= DecimalInteger? '.' DecDigits
```

referenced by:

* Literal

**StringLiteral:**

![StringLiteral](diagram/StringLiteral.png)

```
StringLiteral
         ::= '"' ( Escape | CharNoQuoteNoBackslash )* '"'
```

referenced by:

* ExternDecl
* Literal

**Escape:**

![Escape](diagram/Escape.png)

```
Escape   ::= '\' ( '"' | '\' | 'n' | 't' | 'r' )
```

referenced by:

* StringLiteral

**CharNoQuoteNoBackslash:**

![CharNoQuoteNoBackslash](diagram/CharNoQuoteNoBackslash.png)

```
CharNoQuoteNoBackslash
         ::= [^"\n]
```

referenced by:

* StringLiteral

**Literal:**

![Literal](diagram/Literal.png)

```
Literal  ::= BooleanLiteral
           | IntegerLiteral
           | FloatLiteral
           | StringLiteral
```

referenced by:

* Primary

**TypeArg:**

![TypeArg](diagram/TypeArg.png)

```
TypeArg  ::= Type
           | ConstExpr
```

referenced by:

* TypeArgList

**TypeArgList:**

![TypeArgList](diagram/TypeArgList.png)

```
TypeArgList
         ::= TypeArg ( ',' TypeArg )*
```

referenced by:

* Type

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
           | Ident ( '<' TypeArgList '>' )?
```

referenced by:

* Decl
* FuncDecl
* Param
* TypeArg

**Param:**

![Param](diagram/Param.png)

```
Param    ::= 'mut'? Type Ident
```

referenced by:

* ParamList

**ParamList:**

![ParamList](diagram/ParamList.png)

```
ParamList
         ::= Param ( ',' Param )*
```

referenced by:

* FuncDecl

**FuncDecl:**

![FuncDecl](diagram/FuncDecl.png)

```
FuncDecl ::= 'func' Ident '(' ParamList? ')' Type?
```

referenced by:

* ExternDecl
* FuncDef

**FuncDef:**

![FuncDef](diagram/FuncDef.png)

```
FuncDef  ::= FuncDecl Block
```

referenced by:

* TopLevelDecl

**ExternDecl:**

![ExternDecl](diagram/ExternDecl.png)

```
ExternDecl
         ::= 'extern' StringLiteral? FuncDecl ';'
```

referenced by:

* TopLevelDecl

**Decl:**

![Decl](diagram/Decl.png)

```
Decl     ::= 'mut'? ( Type | 'var' ) Ident
```

referenced by:

* DeclInit
* DeclStmt

**DeclInit:**

![DeclInit](diagram/DeclInit.png)

```
DeclInit ::= Decl '=' Expr
```

referenced by:

* ForInit

**DeclStmt:**

![DeclStmt](diagram/DeclStmt.png)

```
DeclStmt ::= Decl ( '=' Expr )? ';'
```

referenced by:

* Stmt

**Stmt:**

![Stmt](diagram/Stmt.png)

```
Stmt     ::= Block
           | DeclStmt
           | ExprStmt
           | IfStmt
           | WhileStmt
           | ForStmt
           | ReturnStmt
```

referenced by:

* Block
* ForStmt
* IfStmt
* WhileStmt

**Block:**

![Block](diagram/Block.png)

```
Block    ::= '{' Stmt* '}'
```

referenced by:

* FuncDef
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
ForInit  ::= ( DeclInit | Expr )?
```

referenced by:

* ForStmt

**ForStep:**

![ForStep](diagram/ForStep.png)

```
ForStep  ::= Expr?
```

referenced by:

* ForStmt

**ReturnStmt:**

![ReturnStmt](diagram/ReturnStmt.png)

```
ReturnStmt
         ::= 'return' Expr? ';'
```

referenced by:

* Stmt

**ExprStmt:**

![ExprStmt](diagram/ExprStmt.png)

```
ExprStmt ::= Expr ';'
```

referenced by:

* Stmt

**TailNoCall:**

![TailNoCall](diagram/TailNoCall.png)

```
TailNoCall
         ::= ( '.' Ident | '[' Expr ']' )*
```

referenced by:

* Ref

**TailWithCall:**

![TailWithCall](diagram/TailWithCall.png)

```
TailWithCall
         ::= '(' ArgList? ')' ( '.' Ident | '[' Expr ']' | '(' ArgList? ')' )*
```

referenced by:

* ParenWithCall
* PlaceWithCall

**Ref:**

![Ref](diagram/Ref.png)

```
Ref      ::= Ident TailNoCall
```

referenced by:

* LValue
* PlaceWithCall

**LValue:**

![LValue](diagram/LValue.png)

```
LValue   ::= Ref
```

referenced by:

* Expr
* IncDecPrefix
* Postfix

**PlaceWithCall:**

![PlaceWithCall](diagram/PlaceWithCall.png)

```
PlaceWithCall
         ::= Ref TailWithCall
```

referenced by:

* Postfix

**ParenWithCall:**

![ParenWithCall](diagram/ParenWithCall.png)

```
ParenWithCall
         ::= '(' Expr ')' TailWithCall
```

referenced by:

* Postfix

**ArgList:**

![ArgList](diagram/ArgList.png)

```
ArgList  ::= Expr ( ',' Expr )*
```

referenced by:

* TailWithCall

**PostSuffix:**

![PostSuffix](diagram/PostSuffix.png)

```
PostSuffix
         ::= '++'
           | '--'
```

referenced by:

* Postfix

**Primary:**

![Primary](diagram/Primary.png)

```
Primary  ::= Literal
           | '(' Expr ')'
```

referenced by:

* Postfix

**Postfix:**

![Postfix](diagram/Postfix.png)

```
Postfix  ::= LValue PostSuffix?
           | PlaceWithCall
           | ParenWithCall
           | Primary
```

referenced by:

* Prefix

**IncDecPrefix:**

![IncDecPrefix](diagram/IncDecPrefix.png)

```
IncDecPrefix
         ::= ( '++' | '--' ) LValue
```

referenced by:

* Prefix

**Prefix:**

![Prefix](diagram/Prefix.png)

```
Prefix   ::= ( '+' | '-' | '!' | '~' )* ( IncDecPrefix | Postfix )
```

referenced by:

* Mul

**Mul:**

![Mul](diagram/Mul.png)

```
Mul      ::= Prefix ( ( '*' | '/' | '%' ) Prefix )*
```

referenced by:

* Add

**Add:**

![Add](diagram/Add.png)

```
Add      ::= Mul ( ( '+' | '-' ) Mul )*
```

referenced by:

* Shift

**Shift:**

![Shift](diagram/Shift.png)

```
Shift    ::= Add ( ( '<<' | '>>' ) Add )*
```

referenced by:

* Rel

**Rel:**

![Rel](diagram/Rel.png)

```
Rel      ::= Shift ( ( '<' | '<=' | '>' | '>=' ) Shift )*
```

referenced by:

* Equality

**Equality:**

![Equality](diagram/Equality.png)

```
Equality ::= Rel ( ( '==' | '!=' ) Rel )*
```

referenced by:

* LogicAnd

**LogicAnd:**

![LogicAnd](diagram/LogicAnd.png)

```
LogicAnd ::= Equality ( '&&' Equality )*
```

referenced by:

* LogicOr

**LogicOr:**

![LogicOr](diagram/LogicOr.png)

```
LogicOr  ::= LogicAnd ( '||' LogicAnd )*
```

referenced by:

* ConstExpr

**ConstExpr:**

![ConstExpr](diagram/ConstExpr.png)

```
ConstExpr
         ::= LogicOr ( '?' Expr ':' LogicOr )*
```

referenced by:

* Expr
* TypeArg

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

* Expr

**Expr:**

![Expr](diagram/Expr.png)

```
Expr     ::= ( LValue AssignOp )* ConstExpr
```

referenced by:

* ArgList
* ConstExpr
* DeclInit
* DeclStmt
* ExprStmt
* ForInit
* ForStep
* ForStmt
* IfStmt
* ParenWithCall
* Primary
* ReturnStmt
* TailNoCall
* TailWithCall
* WhileStmt

##  
![rr-2.5](diagram/rr-2.5.png) <sup>generated by [RR - Railroad Diagram Generator][RR]</sup>

[RR]: https://www.bottlecaps.de/rr/ui
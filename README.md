# EytionLang -- an programming language
[![https://img.shields.io/badge/Eytion-v0.1.45Alpha-red](https://img.shields.io/badge/Eytion-v0.1.45Alpha-red)]()
[![https://img.shields.io/badge/Platform-Windows-blue](https://img.shields.io/badge/Platform-Windows-blue)]()
[![https://img.shields.io/badge/License-MIT-green](https://img.shields.io/badge/License-MIT-green)]()  
***Tip: all documents are translated through translation software. If there is anything inappropriate, please give feedback***  
***The current version is not a stable version. If you want to use eytion, please download the latest version in the release. The open source code is only for learning and communication. It is not recommended to use it in daily life***  

You can select a language to read development documents  
[简体中文开发文档](doc/zh-cn.md)  
[EnglishDevelopDocument](doc/en-us.md)  
  
---

## Grammar  
Eytion is a lightweight interpretation language that currently supports the following syntax:  
```
mulExpr->primExpr (mulOperator primExpr)*
mulOperator-> "*"|"/"|"%""
addExpr->mulExpr (addOperator mulExpr)*
addOperator->"+"|"-""

cmpOp->'=='|'!='|'>'|'<'|'>='|'<='
cmpExpr->addExpr (cmpOp addExpr)
boolOp->'&&' | '||'
boolExpr->cmpExpr (boolOp cmpExpr)*
notExpr->'!'* boolExpr
primExpr->number|string|'('expr')'

expr->notExpr
stmt->outStmt
stat->stmt*

OutStmt->"out" addExpr ";"
VorcStmt->("var"|"const)" "<"type">"iden "=" (value | value_expr) ";"
AssignStmt->iden "=" (value | value_expr) ";"
DeleteStmt->"delete" iden ";"
InputStmt->"input" "(" content ")" "=>" identifier ";"
WhileStmt->"while" boolExpr | notBoolExpr ":" block
IfStmt->"if" boolExpr | notBoolExpr ":" block
BlockStmt->"{" stmts "}"
```
More syntax waiting to be updated  
## Install  
  
You can use the following command to get your first compilation:  
```
cmake --build
```  
In subsequent compilation, you should use the following commands:  
```
cmake --build build
```  
## Project Preview  
  
The following files are available in Eyton's project directory(only src):  
```
SRC
│  eymain.cpp
│  osstd.cpp
│  osstd.h
│
├─eexception
│      eexcp.cpp
│      eexcp.h
│
├─ey
│  ├─scope
│  │      eyscope.cpp
│  │      eyscope.h
│  │
│  └─value
│          eytype.cpp
│          eytype.h
│          eyvalue.cpp
│          eyvalue.h
│
├─eycodegen
│      generator.cpp
│      generator.h
│
├─eyexec
│      executer.cpp
│      executer.h
│
├─eylexer
│      lex.cpp
│      lex.h
│
├─eyparser
│      ast.cpp
│      ast.h
│      parser.cpp
│      parser.h
│
└─eysystem
        command.cpp
        command.h
        econfig.cpp
        econfig.h
```  
Next, briefly describe the functions of all the first level subfolders and internal files under the SRC directory
|Folder name|Important sub files|Role of folders and important sub files|
|:---:|:---:|:---:|
|***eysystem***|econfig.* and ***command.****|Eyton's shell and configuration system are implemented here. Econfig is the configuration file implementation and command is the shell implementation|
|***eylexer***|lex.*|The lexer of eytion is implemented here|
|***eyparser***|parser.*|The parser of eytion is implemented here|
|***eycodegen***|generator.*|The intermediate code generator for eytion is implemented here|
|***eyexec***|executer.*|Eyton's running code system|
|***eexception***|eexcp.*|Eyton's abnormal system|
|***ey***|value/eyvalue.* scope/eyscope.*|Eytion language content implementation part, variable / constant system in value folder, scope scope scope and code block system|

## Run .ey files  
```
eytion -f file.ey
```  
*or*  
```
eytion -shell
```  
```
run file.ey
```
## Examples    
> HelloWorld
```go
out "Hello, World!";
```  
  
> variable/constant  
```go
var <string> str = "eytion";
var <int> n1 = 2;
var <deci> n2 = 1.5;
out str;
str = "test";
const <deci> number = n1 + n2;
out number;
out str;
```  
  
> input  
```go
var <string> str = " ";
input("please input: ")=>str;
out str;
```

> if  
```go
var <string> str = " ";
input("please input: ")=>str;
if str == "hello world!":{
    out "hello!!!!!!";
}
```

> while
```go
var <int> i = 0;
var <string> str = " ";
input("please input: ")=>str;
while i == 5:{
    out str;
    i = i + 1;
}
```

> array
```go
array<[5]string> sl = ["hel", "lo", "wo", "rl", "d"];
var <int> i  = 0;
while i == 5:{
    out sl[i];
    i=i+1;
}
sl[4] = "wwwwwwww";
out sl[4];
```
## Thanks
Thank the following people for their support for this project:  
> theflysong  
> xiaokang00010
  
Third party libraries used in the project:  
> CJsonObject (by Bwar)[CJsonObject](https://github.com/Bwar/CJsonObject)

---
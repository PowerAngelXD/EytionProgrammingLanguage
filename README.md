# EytionLang -- an programming
***tip:All contents of this document obtained by translation. Please forgive me if there are any inappropriate parts***  

select a language to read:  
[简体中文文档](doc/zh-cn.md)  
[EnglishDocument](doc/en-us.md)  
  
---
 
Eytionlang is a lightweight interpretation language. Its syntax is as follows:  
> primExpr->Number|(addExpr);  
> mulExpr->primExpr (mulOperator primExpr)*  
> mulOperator-> "*"|"/"|"%""  
> addExpr->mulExpr (addOperator mulExpr)*  
> addOperator->"+"|"-""  
>   
> cmpOp->'=='|'!='|'>'|'<'|'>='|'<='
> cmpExpr->addExpr (cmpOp addExpr)*  
> boolOp->'&' | '|' | '&&' | '||'  
> boolExpr->cmpExpr (boolOp cmpExpr) 
> notExpr->'!' boolExpr  
> primExpr->number|string|'('expr')'  
>   
> expr->addExpr | boolExpr | notExpr  
> stmt->outStmt  
> stat->stmt*  
>   
> OutStmt->"out" addExpr ";"  
> VorcStmt->("var"|"const)" "<"type">"iden "=" (value | value_expr) ";"  
> AssignStmt->iden "=" (value | value_expr) ";"  
> DeleteStmt->"delete" iden ";"  
> BlockStmt->"{" stmts "}"  
> 
More syntax is waiting for subsequent updates  
  
---
  
For assistance / partners in this project(All are net names):
> theflysong  
> xiaokang00010

---  
  
Referenced third party libraries:  
> CJsonObject (by Bwar)[CJsonObject](https://github.com/Bwar/CJsonObject)

---
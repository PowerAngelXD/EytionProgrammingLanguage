# EytionLang -- an programming language
[![https://img.shields.io/badge/Eytion-v0.1.34Alpha-red](https://img.shields.io/badge/Eytion-v0.1.34Alpha-red)](https://img.shields.io/badge/Eytion-v0.1.34Alpha-red)
[![https://img.shields.io/badge/Copyright\(c\)-CodeAreaDevelopTeam-blue](https://img.shields.io/badge/Copyright\(c\)-CodeAreaDevelopTeam-blue)](https://img.shields.io/badge/Copyright\(c\)-CodeAreaDevelopTeam-blue)
[![https://img.shields.io/badge/Copyright\(c\)-XtherDevTeam-blue](https://img.shields.io/badge/Copyright\(c\)-XtherDevTeam-blue)](https://img.shields.io/badge/Copyright\(c\)-XtherDevTeam-blue)  
[![https://img.shields.io/badge/License-MIT-green](https://img.shields.io/badge/License-MIT-green)](https://img.shields.io/badge/License-MIT-green)  
***提示，所有文档都是通过翻译软件翻译而成，如有不妥当之处欢迎反馈QAQ***  
***当前版本并不是稳定版本，如果您想要使用eytion，请下载release中的最新版本，开源代码仅供学习交流使用，不推荐日常生活使用***  

您可以选择一个语言进行阅读开发文档  
[简体中文开发文档](doc/zh-cn.md)  
[EnglishDevelopDocument](doc/en-us.md)  
  
---
 
Eytion是一个轻量级的解释语言，目前它支持以下语法:    
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
更多的语法等待更新  
  
---  
  
如果您想写一个eytion程序，务必看看这个:  
***eytion程序的注意事项***
|事项|说明|
|:---:|:---:|
|配置文件|eytion的全局配置文件的路径为.\settings\eyconfig.json|  
  
***eytion关键字(以当前版本为准)***
|关键字|用途|关键字|用途|
|:---:|:---:|:---:|:---:|
|***out***|输出内容|***var***|声明变量|
|***delete***|删除标识符|***const***|声明常量|
|***int***|整型|***string***|字符串|
|***deci***|小数|***bool***|布尔|
---
  
感谢下列人员为本项目提供支持(均为网名):
> theflysong  
> xiaokang00010

---  
  
本项目所使用的第三方库:    
> CJsonObject (by Bwar)[CJsonObject](https://github.com/Bwar/CJsonObject)

---
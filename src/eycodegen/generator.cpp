#include "generator.h"
#include "../eyexec/executer.h"

namespace eycodegen {

    CodeGenerator::CodeGenerator() : CPoolIndex(0) {}

    float CodeGenerator::visitNumber(TokenNode* node) {
        return atof(node->token().content.c_str());
    }

    void CodeGenerator::visitMulOperator(MulOperatorNode* node) {
        if (node->MulOperator()->token().symbol == Symbol::Mul)
            instructions.push_back(new_ei(Ins::MUL, node->MulOperator()->token().line, node->MulOperator()->token().column, 0));
        else if (node->MulOperator()->token().symbol == Symbol::Div)
            instructions.push_back(new_ei(Ins::DIV, node->MulOperator()->token().line, node->MulOperator()->token().column, 0));
        else if (node->MulOperator()->token().symbol == Symbol::Mod)
            instructions.push_back(new_ei(Ins::MOD, node->MulOperator()->token().line, node->MulOperator()->token().column, 0));
        else if (node->MulOperator()->token().symbol == Symbol::Comma)
            instructions.push_back(new_ei(Ins::NOP));
    }

    void CodeGenerator::visitAddOperator(AddOperatorNode* node) {
        if (node->AddOperator()->token().symbol == Symbol::Add)
            instructions.push_back(new_ei(Ins::ADD, node->AddOperator()->token().line, node->AddOperator()->token().column, 0));
        else if (node->AddOperator()->token().symbol == Symbol::Sub)
            instructions.push_back(new_ei(Ins::SUB, node->AddOperator()->token().line, node->AddOperator()->token().column, 0));
        else if (node->AddOperator()->token().symbol == Symbol::Comma)
            instructions.push_back(new_ei(Ins::NOP));
    }

    void CodeGenerator::visitPrimExpr(PrimExprNode* node) {
        if (node->AddExpr()) {
            visitAddExpr(node->AddExpr());
        }
        else {
            if(node->Number() != nullptr) {
                if(node->toString().find(".") == node->toString().npos)
                    instructions.push_back(new_ei(Ins::PUSH, 0, 0, visitNumber(node->Number()), 0, "__null__", false, TY_IMM));
                else instructions.push_back(new_ei(Ins::PUSH, 0, 0, visitNumber(node->Number()), 0, "__null__", false, TY_DEC));
            }
            else if(node->ArrayElt() != nullptr) {
                visitAddExpr(node->ArrayElt()->Index());// 可以正常运行
                instructions.push_back(new_ei(Ins::POP, 0, 0, 0, 0, node->ArrayElt()->Iden()->token().content, false, TY_ARR)); // TODO:  bug here
            }
            else if(node->Iden() != nullptr) {
                instructions.push_back(new_ei(Ins::POP, 0, 0, 0, 0, node->Iden()->token().content, false, TY_NULL));
            }
            else if(node->String() != nullptr) {
                visitString(node->String());
            }
            else if(node->ConstBool() != nullptr) {
                visitCBool(node->ConstBool());
            }
        }
    }

    void CodeGenerator::visitAddExpr(AddExprNode* node) {
        if(node->AddOperator().empty()) {visitMulExpr(node->MulExpr(0)); return;}
        visitMulExpr(node->MulExpr(0));
        for (int i = 0 ; i < node->AddOperator().size(); i ++) {// here!!!!!!
            visitMulExpr(node->MulExpr(i + 1));
            visitAddOperator(node->AddOperator(i));
        }
    }

    void CodeGenerator::visitMulExpr(MulExprNode* node) {
        if(node->MulOperator().empty()) {visitPrimExpr(node->PrimExpr(0)); return;}
        visitPrimExpr(node->PrimExpr(0));
        for (int i = 0 ; i < node->MulOperator().size(); i ++) {
            visitPrimExpr(node->PrimExpr(i + 1));
            visitMulOperator(node->MulOperator(i));
        }
    }

    void CodeGenerator::visitCmpOperator(CmpOperatorNode* node){
        if (node->Op()->token().symbol == Symbol::EQ)
            instructions.push_back(new_ei(Ins::EQ, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::NEQ)
            instructions.push_back(new_ei(Ins::NEQ, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::LT)
            instructions.push_back(new_ei(Ins::LEST, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::GT)
            instructions.push_back(new_ei(Ins::MRET, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::LE)
            instructions.push_back(new_ei(Ins::LEREQT, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::GE)
            instructions.push_back(new_ei(Ins::MREQT, node->Op()->token().line, node->Op()->token().column, 0));
    }

    void CodeGenerator::visitCmpExpr(CmpExprNode* node){
        if(node->Op() != nullptr){
            visitAddExpr(node->Expr());
            visitAddExpr(node->Sub());
            visitCmpOperator(node->Op());
        }
        else{
            visitAddExpr(node->Expr());
        }
    }

    void CodeGenerator::visitBoolOperator(BoolOperatorNode* node){
        if (node->Op()->token().symbol == Symbol::LogicAnd)
            instructions.push_back(new_ei(Ins::LAND, node->Op()->token().line, node->Op()->token().column, 0));
        else if (node->Op()->token().symbol == Symbol::LogicOr)
            instructions.push_back(new_ei(Ins::LOR, node->Op()->token().line, node->Op()->token().column, 0));
    }

    void CodeGenerator::visitNotBoolOperator(TokenNode* node){
        if (node->token().symbol == Symbol::Not)
            instructions.push_back(new_ei(Ins::NOT, node->token().line, node->token().column, 0));
    }

    void CodeGenerator::visitBoolExpr(BoolExprNode* node){
        visitCmpExpr(node->Root());
        for (int i = 0 ; i < node->Op().size(); i++) {
            visitCmpExpr(node->Sub(i));
            visitBoolOperator(node->Op(i + 1));
        }
    }

    void CodeGenerator::visitNotBoolExpr(NotBoolExprNode* node){
        visitBoolExpr(node->Boolexpr());
        visitNotBoolOperator(node->Op());
    }

    void CodeGenerator::visitString(TokenNode* node){
        string text = split(" "+node->token().content+" ", "\"").at(1);
        string result;
        for(int i=0; i<text.size(); i++){
            if(text.at(i) == '\\' && text.at(i + 1) == '\\'){
                i++;
                result.push_back(text.at(i));
            }
            else if(text.at(i) == '\\'){
                if(text.at(i + 1) == 'n') {result.push_back('\n');}
                else if(text.at(i + 1) == 't') {result.push_back('\t');}
                else if(text.at(i + 1) == 'r') {result.push_back('\r');}
                else if(text.at(i + 1) == '0') {result.push_back('\0');}
                else{
                    throw "expect '\\'";
                }
                i++;
            }
            else{
                result.push_back(text.at(i));
            }
        }
        ConstantPool.push_back(result);
        CPoolIndex = ConstantPool.size()-1;
        instructions.push_back(new_ei(Ins::PUSH, node->token().line, node->token().column, CPoolIndex, 0, "__null__", false, TY_CON));
    }

    void CodeGenerator::visitCBool(TokenNode* node) {
        int torf;
        if(node->token().content == "true") torf = 1;
        else torf = 0;
        instructions.push_back(new_ei(Ins::PUSH, node->token().line, node->token().column, torf, 0, "__null__", false, TY_BOL));
    }

    void CodeGenerator::visitStmt(StmtNode* node){
        if(node == nullptr) throw "generator down";
        
        if(node->OutStmt() != nullptr) visitOutStmt(node->OutStmt());
        else if(node->VorcStmt() != nullptr) visitVorcStmt(node->VorcStmt());
        else if(node->AssignStmt() != nullptr) visitAssignStmt(node->AssignStmt());
        else if(node->DeleteStmt() != nullptr) visitDelStmt(node->DeleteStmt());
        else if(node->BlockStmt() != nullptr) visitBlockStmt(node->BlockStmt());
        else if(node->InputStmt() != nullptr) visitInputStmt(node->InputStmt());
        else if(node->WhileStmt() != nullptr) visitWhileStmt(node->WhileStmt());
        else if(node->ArrayDefineStmt() != nullptr) visitArrayDefineStmt(node->ArrayDefineStmt());
        else if(node->AssignArrayStmt() != nullptr) visitArrayAssignStmt(node->AssignArrayStmt());
        else if(node->IfStmt() != nullptr) visitIfStmt(node->IfStmt());
        else throw "unknown stmt";
    } 

    void CodeGenerator::visitStat(StatNode* node){
        for(auto stmt : node->Stmt()) {
            visitStmt(stmt);
        }
    }

    void CodeGenerator::visitOutStmt(OutStmtNode* node) {// TODO: bug here
        char op_type = TY_IMM;
        if(node->Expr()->BoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitBoolExpr(node->Expr()->BoolExpr());
        }
        else if(node->Expr()->NotBoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitNotBoolExpr(node->Expr()->NotBoolExpr());
        }
        else {
            if(node->Expr()->AddExpr()->toString().find(".") != node->Expr()->AddExpr()->toString().npos)
                op_type = TY_IMM;
            else if(node->Expr()->AddExpr()->toString().find("string") != node->Expr()->AddExpr()->toString().npos)
                op_type = TY_CON;
            else if(node->Expr()->AddExpr()->toString().find("ArrayElt") != node->Expr()->AddExpr()->toString().npos)
                op_type = TY_ARR;
            else op_type = TY_DEC;
            visitAddExpr(node->Expr()->AddExpr());
        }
        instructions.push_back(new_ei(Ins::OSOUT, node->OutMark()->token().line, node->OutMark()->token().column, 0, 0, "__null__", false, op_type));
    }

    void CodeGenerator::visitVorcStmt(VorcStmtNode* node) {
        char op_type = TY_IMM;
        if(node->VarMark() != nullptr){
            if(node->Type()->token().content == "int") {
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an integer quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                    op_type = TY_CON;
                }
                else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_type = TY_DEC;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "bool") {
                op_type = TY_BOL;
                if(node->Expr()->BoolExpr() != nullptr) {
                    visitBoolExpr(node->Expr()->BoolExpr());
                }
                else if(node->Expr()->NotBoolExpr() != nullptr) {
                    visitNotBoolExpr(node->Expr()->NotBoolExpr());
                }
                else if(node->Expr()->AddExpr() != nullptr && (node->Expr()->AddExpr()->toString().find("true") || node->Expr()->AddExpr()->toString().find("false"))) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an bool quantity, but the value you specified does not correspond to it";}
            }
            else {throw "it is not a right type";}
            instructions.push_back(new_ei(Ins::DEFINE_VORC, node->VarMark()->token().line, node->VarMark()->token().column, 
                                           0, 0, node->IdenName()->token().content, false, op_type));
        }
        else {
            if(node->Type()->token().content == "int") {
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an integer cc, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                    op_type = TY_CON;
                }
                else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_type = TY_DEC;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "bool") {
                op_type = TY_BOL;
                if(node->Expr()->BoolExpr() != nullptr) {
                    visitBoolExpr(node->Expr()->BoolExpr());
                }
                else if(node->Expr()->NotBoolExpr() != nullptr) {
                    visitNotBoolExpr(node->Expr()->NotBoolExpr());
                }
                else if(node->Expr()->AddExpr() != nullptr && (node->Expr()->AddExpr()->toString().find("true") || node->Expr()->AddExpr()->toString().find("false"))) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an bool quantity, but the value you specified does not correspond to it";}
            }
            else {throw "it is not a right type";}
            instructions.push_back(new_ei(Ins::DEFINE_VORC, node->ConstMark()->token().line, node->ConstMark()->token().column, 
                                           0, 0, node->IdenName()->token().content, true, op_type));
        }
    }

    void CodeGenerator::visitArrayDefineStmt(ArrayDefineStmtNode* node) {
        char op_type = TY_IMM;
        int count;
        if(node->Type()->token().content == "int"){
            for(count = 0; count < node->Elts().size(); count++){
                if(node->Elts().at(count)->AddExpr() != nullptr && 
                   node->Elts().at(count)->AddExpr()->toString().find(".") == node->Elts().at(count)->AddExpr()->toString().npos) visitAddExpr(node->Elts().at(count)->AddExpr());
                else throw "Array elements must be your designated type: int";
            }
        }
        else if(node->Type()->token().content == "deci"){
            for(count = 0; count < node->Elts().size(); count++){
                if(node->Elts().at(count)->AddExpr() != nullptr && 
                   node->Elts().at(count)->AddExpr()->toString().find(".") != node->Elts().at(count)->AddExpr()->toString().npos) visitAddExpr(node->Elts().at(count)->AddExpr());
                else throw "Array elements must be your designated type: deci";
            }
            op_type = TY_DEC;
        }
        else if(node->Type()->token().content == "string"){
            for(count = 0; count < node->Elts().size(); count++){
                if(node->Elts().at(count)->AddExpr() != nullptr && 
                   node->Elts().at(count)->AddExpr()->toString().find("string") != node->Elts().at(count)->AddExpr()->toString().npos) visitAddExpr(node->Elts().at(count)->AddExpr());
                else throw "Array elements must be your designated type: string";
            }
            op_type = TY_CON;
        }
        else if(node->Type()->token().content == "bool"){
            for(count = 0; count < node->Elts().size(); count++){
                if(node->Elts().at(count)->AddExpr() != nullptr && 
                   node->Elts().at(count)->AddExpr()->toString().find("true") != node->Elts().at(count)->AddExpr()->toString().npos) visitAddExpr(node->Elts().at(count)->AddExpr());
                else if(node->Elts().at(count)->AddExpr() != nullptr && 
                   node->Elts().at(count)->AddExpr()->toString().find("false") != node->Elts().at(count)->AddExpr()->toString().npos) visitAddExpr(node->Elts().at(count)->AddExpr());

                else if(node->Elts().at(count)->BoolExpr() != nullptr) visitBoolExpr(node->Elts().at(count)->BoolExpr());
                if(node->Elts().at(count)->NotBoolExpr() != nullptr) visitNotBoolExpr(node->Elts().at(count)->NotBoolExpr());
                else throw "Array elements must be your designated type: bool";
            }
            op_type = TY_BOL;
        }
        instructions.push_back(new_ei(Ins::DEFINE_ARRAY, node->ArrayDefineMark()->token().line, node->ArrayDefineMark()->token().column, 0, count, node->Iden()->token().content, false, op_type));
    }

    void CodeGenerator::visitArrayAssignStmt(AssignArrayElementStmtNode* node) {
        char op_type = TY_IMM;
        bool dot = true;
        visitAddExpr(node->ArrayElt()->Index());
        if(node->Expr()->AddExpr() != nullptr) {
            string temp = node->Expr()->AddExpr()->toString();
            if(temp.find('.') != temp.npos){
                op_type = TY_DEC;
            }
            else if(temp.find("string") != temp.npos){
                op_type = TY_CON;
            }
            else if(temp.find("ArrayNode") != temp.npos){
                op_type = TY_ARR;
            }
            else if(temp.find("true") != temp.npos || temp.find("false") != temp.npos){
                op_type = TY_BOL;
            }
            else char op_type = TY_IMM;
            visitAddExpr(node->Expr()->AddExpr());
        }
        else if(node->Expr()->BoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitBoolExpr(node->Expr()->BoolExpr());
        }
        else if(node->Expr()->NotBoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitNotBoolExpr(node->Expr()->NotBoolExpr());
        }
        else{ throw (string)"try to assign an incorrect value to identifier: '" +  node->ArrayElt()->Iden()->token().content + "'"; }
        instructions.push_back(new_ei(Ins::ASSIGN_ARRAY, node->ArrayElt()->Iden()->token().line, node->ArrayElt()->Iden()->token().column, 0, 0, node->ArrayElt()->Iden()->token().content, false, op_type));
    }

    void CodeGenerator::visitAssignStmt(AssignStmtNode* node) {
        char op_type = TY_IMM;
        bool dot = true;
        if(node->Expr()->AddExpr() != nullptr) {
            string temp = node->Expr()->AddExpr()->toString();
            if(temp.find('.') != temp.npos){
                op_type = TY_DEC;
            }
            else if(temp.find("string") != temp.npos){
                op_type = TY_CON;
            }
            else if(temp.find("ArrayNode") != temp.npos){
                op_type = TY_ARR;
            }
            else if(temp.find("true") != temp.npos || temp.find("false") != temp.npos){
                op_type = TY_BOL;
            }
            else char op_type = TY_IMM;
            visitAddExpr(node->Expr()->AddExpr());
        }
        else if(node->Expr()->BoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitBoolExpr(node->Expr()->BoolExpr());
        }
        else if(node->Expr()->NotBoolExpr() != nullptr) {
            op_type = TY_BOL;
            visitNotBoolExpr(node->Expr()->NotBoolExpr());
        }
        else{ throw (string)"try to assign an incorrect value to identifier: '" +  node->Iden()->token().content + "'"; }
        instructions.push_back(new_ei(Ins::ASSIGN, node->Iden()->token().line, node->Iden()->token().column, 0, 0, node->Iden()->token().content, false, op_type));
    }

    void CodeGenerator::visitDelStmt(DeleteStmtNode* node) {
        instructions.push_back(new_ei(Ins::DEL, node->DeleteMark()->token().line, node->DeleteMark()->token().column, 0, 0, node->Iden()->token().content));
    }

    void CodeGenerator::visitInputStmt(InputStmtNode* node){
        if(node->Content()->toString().find("string")){
            visitAddExpr(node->Content());
            instructions.push_back(new_ei(Ins::OSINPUT, node->InputMark()->token().line, node->InputMark()->token().column, 0, 0, node->Iden()->token().content));
        }
        else{
            throw (string)"must put a 'String' value here";
        }
    }

    void CodeGenerator::visitWhileStmt(WhileStmtNode* node){
        instructions.push_back(new_ei(Ins::OSWHILE));
        if(node->Cond()->toString().find("NotBoolExpr") != node->Cond()->toString().npos){
            visitNotBoolExpr(node->Cond()->NotBoolExpr());
        }
        else if(node->Cond()->toString().find("BoolExpr") != node->Cond()->toString().npos){
            visitBoolExpr(node->Cond()->BoolExpr());
        }

        instructions.push_back(new_ei(Ins::GOTO_WITHCOND, node->WhileMark()->token().line, node->WhileMark()->token().column, 0, 0, "WHILE_STMT_ISPASS", true));
        visitBlockStmt(node->Block());
        instructions.push_back(new_ei(Ins::GOTO_WITHCOND, node->WhileMark()->token().line, node->WhileMark()->token().column, 0, 0, "WHILE_STMT_BACK", true));
    }

    void CodeGenerator::visitIfStmt(IfStmtNode* node){
        if(node->Cond()->toString().find("NotBoolExpr") != node->Cond()->toString().npos){
            visitNotBoolExpr(node->Cond()->NotBoolExpr());
        }
        else if(node->Cond()->toString().find("BoolExpr") != node->Cond()->toString().npos){
            visitBoolExpr(node->Cond()->BoolExpr());
        }
        instructions.push_back(new_ei(Ins::GOTO_WITHCOND, node->IfMark()->token().line, node->IfMark()->token().column, 0, 0, "IF_STMT", 0, true));
        visitBlockStmt(node->Block());
    }

    void CodeGenerator::visitBlockStmt(BlockStmtNode* node) {
        instructions.push_back(new_ei(Ins::SCOPE_BEGIN, 0, 0, 0));
        for(auto stmt : node->Stmts()){
            visitStmt(stmt);
        }
        instructions.push_back(new_ei(Ins::SCOPE_END, 0, 0, 0));
    }
}
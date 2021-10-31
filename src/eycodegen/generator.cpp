#include "generator.h"
#include "../eyexec/executer.h"

namespace eycodegen {
    CodeGenerator::CodeGenerator() : CPoolIndex(0) {}

    float CodeGenerator::visitNumber(TokenNode* node) {
        return atof(node->token().content.c_str());
    }

    void CodeGenerator::visitMulOperator(MulOperatorNode* node) {
        if (node->MulOperator()->token().symbol == Symbol::Mul)
            instructions.push_back(Instruction{Instruction::MUL, node->MulOperator()->token().line, node->MulOperator()->token().column, 0});
        else if (node->MulOperator()->token().symbol == Symbol::Div)
            instructions.push_back(Instruction{Instruction::DIV, node->MulOperator()->token().line, node->MulOperator()->token().column, 0});
        else if (node->MulOperator()->token().symbol == Symbol::Mod)
            instructions.push_back(Instruction{Instruction::MOD, node->MulOperator()->token().line, node->MulOperator()->token().column, 0});
    }

    void CodeGenerator::visitAddOperator(AddOperatorNode* node) {
        if (node->AddOperator()->token().symbol == Symbol::Add)
            instructions.push_back(Instruction{Instruction::ADD, node->AddOperator()->token().line, node->AddOperator()->token().column, 0});
        else if (node->AddOperator()->token().symbol == Symbol::Sub)
            instructions.push_back(Instruction{Instruction::SUB, node->AddOperator()->token().line, node->AddOperator()->token().column, 0});
    }

    void CodeGenerator::visitPrimExpr(PrimExprNode* node) {
        if (node->AddExpr()) {
            visitAddExpr(node->AddExpr());
        }
        else {
            if(node->Number() != nullptr) {
                if(node->toString().find(".") == node->toString().npos)
                    instructions.push_back(Instruction{Instruction::PUSH, 0, 0, visitNumber(node->Number()), TY_IMM});
                else instructions.push_back(Instruction{Instruction::PUSH, 0, 0, visitNumber(node->Number()), TY_DEC});
            }
            else if(node->Iden() != nullptr) {
                instructions.push_back(Instruction{Instruction::POP, 0, 0, node->Iden()->token().content});
            }
            else if(node->String() != nullptr) {
                visitString(node->String());
            }
        }
    }

    void CodeGenerator::visitAddExpr(AddExprNode* node) {
        visitMulExpr(node->MulExpr(0));
        for (int i = 0 ; i < node->AddOperator().size() ; i ++) {
            visitMulExpr(node->MulExpr(i + 1));
            visitAddOperator(node->AddOperator(i));
        }
    }

    void CodeGenerator::visitMulExpr(MulExprNode* node) {
        visitPrimExpr(node->PrimExpr(0));
        for (int i = 0 ; i < node->MulOperator().size() ; i ++) {
            visitPrimExpr(node->PrimExpr(i + 1));
            visitMulOperator(node->MulOperator(i));
        }
    }

    void CodeGenerator::visitCmpOperator(CmpOperatorNode* node){
        if (node->Op()->token().symbol == Symbol::EQ)
            instructions.push_back(Instruction{Instruction::EQ, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::NEQ)
            instructions.push_back(Instruction{Instruction::NEQ, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::LT)
            instructions.push_back(Instruction{Instruction::LEST, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::GT)
            instructions.push_back(Instruction{Instruction::MRET, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::LE)
            instructions.push_back(Instruction{Instruction::LEREQT, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::GE)
            instructions.push_back(Instruction{Instruction::MREQT, node->Op()->token().line, node->Op()->token().column, 0});
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
            instructions.push_back(Instruction{Instruction::LAND, node->Op()->token().line, node->Op()->token().column, 0});
        else if (node->Op()->token().symbol == Symbol::LogicOr)
            instructions.push_back(Instruction{Instruction::LOR, node->Op()->token().line, node->Op()->token().column, 0});
    }

    void CodeGenerator::visitNotBoolOperator(TokenNode* node){
        if (node->token().symbol == Symbol::Not)
            instructions.push_back(Instruction{Instruction::NOT, node->token().line, node->token().column, 0});
    }

    void CodeGenerator::visitBoolExpr(BoolExprNode* node){
        visitCmpExpr(node->Root());
        for (int i = 0 ; i < node->Op().size() ; i ++) {
            visitCmpExpr(node->Sub(i + 1));
            visitBoolOperator(node->Op(i));
        }
    }

    void CodeGenerator::visitNotBoolExpr(NotBoolExprNode* node){
        visitNotBoolOperator(node->Op());
        visitBoolExpr(node->Boolexpr());
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
        instructions.push_back(Instruction{Instruction::PUSH, node->token().line, node->token().column, CPoolIndex, TY_CON});
    }

    void CodeGenerator::visitStmt(StmtNode* node){
        if(node->OutStmt() != nullptr) visitOutStmt(node->OutStmt());
        else if(node->VorcStmt() != nullptr) visitVorcStmt(node->VorcStmt());
        else if(node->AssignStmt() != nullptr) visitAssignStmt(node->AssignStmt());
        else if(node->DeleteStmt() != nullptr) visitDelStmt(node->DeleteStmt());
        else if(node->BlockStmt() != nullptr) visitBlockStmt(node->BlockStmt());
        else if(node->InputStmt() != nullptr) visitInputStmt(node->InputStmt());
    } 

    void CodeGenerator::visitStat(StatNode* node){
        for(auto stmt : node->Stmt()) {
            visitStmt(stmt);
        }
    }

    void CodeGenerator::visitOutStmt(OutStmtNode* node) {
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
            if(node->Expr()->AddExpr()->toString().find(".") == node->Expr()->AddExpr()->toString().npos)
                op_type = TY_IMM;
            else if(node->Expr()->AddExpr()->toString().find("string") == node->Expr()->AddExpr()->toString().npos)
                op_type = TY_CON;
            else op_type = TY_DEC;
            visitAddExpr(node->Expr()->AddExpr());
        }
        instructions.push_back(Instruction{Instruction::OSOUT, node->OutMark()->token().line, node->OutMark()->token().column, 0, op_type});
    }

    void CodeGenerator::visitVorcStmt(VorcStmtNode* node) {
        char op_type = TY_IMM;
        int op_value_type = 0;
        if(node->VarMark() != nullptr){
            if(node->Type()->token().content == "int") {
                op_value_type = 0;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an integer quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                op_value_type = 2;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                    op_type = TY_CON;
                }
                else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_value_type = 1;
                op_type = TY_DEC;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "bool") {
                op_value_type = 3;
                op_type = TY_BOL;
                if(node->Expr()->BoolExpr() != nullptr) {
                    visitBoolExpr(node->Expr()->BoolExpr());
                }
                else if(node->Expr()->NotBoolExpr() != nullptr) {
                    visitNotBoolExpr(node->Expr()->NotBoolExpr());
                }
                else {throw "you tried to declare an bool quantity, but the value you specified does not correspond to it";}
            }
            else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            instructions.push_back(Instruction{Instruction::DEFINE_VORC, node->VarMark()->token().line, node->VarMark()->token().column, 
            node->IdenName()->token().content, op_value_type, op_type, true});
        }
        else {
            if(node->Type()->token().content == "int") {
                op_value_type = 0;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an integer cc, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                op_value_type = 2;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                    op_type = TY_CON;
                }
                else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_value_type = 1;
                op_type = TY_DEC;
                if(node->Expr()->AddExpr() != nullptr) {
                    visitAddExpr(node->Expr()->AddExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "bool") {
                op_value_type = 3;
                op_type = TY_BOL;
                if(node->Expr()->BoolExpr() != nullptr) {
                    visitBoolExpr(node->Expr()->BoolExpr());
                }
                else if(node->Expr()->NotBoolExpr() != nullptr) {
                    visitNotBoolExpr(node->Expr()->NotBoolExpr());
                }
                else {throw "you tried to declare an bool quantity, but the value you specified does not correspond to it";}
            }
            else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            instructions.push_back(Instruction{Instruction::DEFINE_VORC, node->ConstMark()->token().line, node->ConstMark()->token().column, 
            node->IdenName()->token().content, op_value_type, op_type, false});
        }
    }

    void CodeGenerator::visitAssignStmt(AssignStmtNode* node) {
        char op_type = TY_IMM;
        bool dot = true;
        if(node->Expr()->AddExpr() != nullptr) {
            if(node->Expr()->AddExpr()->toString().find('.') != node->Expr()->AddExpr()->toString().npos){
                op_type = TY_DEC;
            }
            else if(node->Expr()->AddExpr()->toString().find('string') != node->Expr()->AddExpr()->toString().npos){
                op_type = TY_CON;
            }
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
        instructions.push_back(Instruction{Instruction::ASSIGN, node->Iden()->token().line, node->Iden()->token().column, node->Iden()->token().content, 0, op_type});
    }

    void CodeGenerator::visitDelStmt(DeleteStmtNode* node) {
        instructions.push_back(Instruction{Instruction::DEL,node->DeleteMark()->token().line, node->DeleteMark()->token().column, node->Iden()->token().content});
    }

    void CodeGenerator::visitInputStmt(InputStmtNode* node){
        if(node->Content()->toString().find("string")){
            visitAddExpr(node->Content());
            instructions.push_back(Instruction{Instruction::OSINPUT, node->InputMark()->token().line, node->InputMark()->token().column, node->Iden()->token().content});
        }
        else{
            throw (string)"must put a 'String' value here";
        }
    }

    void CodeGenerator::visitBlockStmt(BlockStmtNode* node) {
        instructions.push_back(Instruction{Instruction::SCOPE_BEGIN, 0, 0, 0});
        for(auto stmt : node->Stmts()){
            visitStmt(stmt);
        }
        instructions.push_back(Instruction{Instruction::SCOPE_END, 0, 0, 0});
    }
}
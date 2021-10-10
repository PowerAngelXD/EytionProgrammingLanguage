#include "generator.h"
#include "../eyexec/executer.h"
#include "../ey/value/eystrparser.h"

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
                instructions.push_back(Instruction{Instruction::PUSH, 0, 0, visitNumber(node->Number())});
            }
            else if(node->Iden() != nullptr) {
                instructions.push_back(Instruction{Instruction::POP, 0, 0, node->Iden()->token().content});
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
        instructions.push_back(Instruction{Instruction::STRING, node->token().line, node->token().column, CPoolIndex});
    }

    void CodeGenerator::visitStmt(StmtNode* node){
        if(node->OutStmt() != nullptr) visitOutStmt(node->OutStmt());
        else if(node->VorcStmt() != nullptr) visitVorcStmt(node->VorcStmt());
        else if(node->AssignStmt() != nullptr) visitAssignStmt(node->AssignStmt());
        else if(node->DeleteStmt() != nullptr) visitDelStmt(node->DeleteStmt());
        else if(node->BlockStmt() != nullptr) visitBlockStmt(node->BlockStmt());
    } 

    void CodeGenerator::visitStat(StatNode* node){
        for(auto stmt : node->Stmt()) {
            visitStmt(stmt);
        }
    }

    void CodeGenerator::visitOutStmt(OutStmtNode* node) {
        char op_type = TY_IMM;
        if(node->String() != nullptr){
            visitString(node->String());
            op_type = TY_CON;
        }
        else {
            visitAddExpr(node->AddExpr());
            op_type = TY_DEC;
        }
        instructions.push_back(Instruction{Instruction::OSOUT, node->OutMark()->token().line, node->OutMark()->token().column, 0, op_type});
    }

    void CodeGenerator::visitVorcStmt(VorcStmtNode* node) {
        char op_type = TY_IMM;
        int op_value_type = 0;
        if(node->VarMark() != nullptr){
            if(node->Type()->token().content == "int") {
                op_value_type = 0;
                if(node->ValueExpr() != nullptr) {
                    visitAddExpr(node->ValueExpr());
                }
                else {throw "you tried to declare an integer quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_value_type = 1;
                op_type = TY_DEC;
                if(node->ValueExpr() != nullptr) {
                    visitAddExpr(node->ValueExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                op_value_type = 2;
                op_type = TY_CON;
                if(node->ValueString() != nullptr) {
                    visitString(node->ValueString());
                
                }
            }
            else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            instructions.push_back(Instruction{Instruction::DEFINE_VORC, node->VarMark()->token().line, node->VarMark()->token().column, 
            node->IdenName()->token().content, op_value_type, op_type, true});
        }
        else {
            if(node->Type()->token().content == "int") {
                op_value_type = 0;
                if(node->ValueExpr() != nullptr) {
                    visitAddExpr(node->ValueExpr());
                }
                else {throw "you tried to declare an integer cc, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "deci") {
                op_value_type = 1;
                op_type = TY_DEC;
                if(node->ValueExpr() != nullptr) {
                    visitAddExpr(node->ValueExpr());
                }
                else {throw "you tried to declare an decimal quantity, but the value you specified does not correspond to it";}
            }
            else if(node->Type()->token().content == "string") {
                op_value_type = 2;
                op_type = TY_CON;
                if(node->ValueString() != nullptr) {
                    visitString(node->ValueString());
                
                }
            }
            else {throw "you tried to declare an string quantity, but the value you specified does not correspond to it";}
            instructions.push_back(Instruction{Instruction::DEFINE_VORC, node->ConstMark()->token().line, node->ConstMark()->token().column, 
            node->IdenName()->token().content, op_value_type, op_type, false});
        }
    }

    void CodeGenerator::visitAssignStmt(AssignStmtNode* node) {
        char op_type = TY_IMM;
        bool dot = true;
        if(node->ValueString() != nullptr) {
            op_type = TY_CON;
            visitString(node->ValueString());
        }
        else if(node->ValueExpr() != nullptr) {
            if(node->ValueExpr()->toString().find('.') != node->ValueExpr()->toString().npos){
                op_type = TY_DEC;
            }
            visitAddExpr(node->ValueExpr());
        }
        else{ throw (string)"try to assign an incorrect value to identifier: '" +  node->Iden()->token().content + "'"; }
        instructions.push_back(Instruction{Instruction::ASSIGN, node->Iden()->token().line, node->Iden()->token().column, node->Iden()->token().content, 0, op_type});
    }

    void CodeGenerator::visitDelStmt(DeleteStmtNode* node) {
        instructions.push_back(Instruction{Instruction::DEL,node->DeleteMark()->token().line, node->DeleteMark()->token().column, node->Iden()->token().content});
    }

    void CodeGenerator::visitBlockStmt(BlockStmtNode* node) {
        instructions.push_back(Instruction{Instruction::SCOPE_BEGIN, 0, 0, 0});
        for(auto stmt : node->Stmts()){
            visitStmt(stmt);
        }
        instructions.push_back(Instruction{Instruction::SCOPE_END, 0, 0, 0});
    }
}
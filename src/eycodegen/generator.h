#pragma once

#include "../eyparser/parser.h"
#include "../eyexec/executer.h"
#include "../eylexer/lex.h"
using namespace eyparser;
using namespace eyexec;
using namespace eylex;

namespace eycodegen {
    class CodeGenerator {
    public:
        std::vector<Instruction> instructions;
        int CPoolIndex;
        std::vector<std::string> ConstantPool;

        CodeGenerator();
        float visitNumber(TokenNode* node);
        void visitMulOperator(MulOperatorNode* node);
        void visitAddOperator(AddOperatorNode* node);
        void visitPrimExpr(PrimExprNode* node);
        void visitAddExpr(AddExprNode* node);
        void visitMulExpr(MulExprNode* node);
        void visitString(TokenNode* node);
        void visitOutStmt(OutStmtNode* node);
        void visitVorcStmt(VorcStmtNode* node);
        void visitAssignStmt(AssignStmtNode* node);
        void visitDelStmt(DeleteStmtNode* node);
        void visitBlockStmt(BlockStmtNode* node);
        void visitStmt(StmtNode* node);
        void visitStat(StatNode* node);
    };
}
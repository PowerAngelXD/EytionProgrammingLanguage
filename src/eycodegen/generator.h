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
        float visitNumber(BasicNode* node);
        void visitMulOperator(MulOperatorNode* node);
        void visitAddOperator(AddOperatorNode* node);
        void visitPrimExpr(PrimExprNode* node);
        void visitAddExpr(AddExprNode* node);
        void visitMulExpr(MulExprNode* node);
        void visitCmpOperator(CmpOperatorNode* node);
        void visitCmpExpr(CmpExprNode* node);
        void visitBoolOperator(BoolOperatorNode* node);
        void visitNotBoolOperator(BasicNode* node);
        void visitBoolExpr(BoolExprNode* node);
        void visitNotBoolExpr(NotBoolExprNode* node);
        void visitString(BasicNode* node);
        void visitCBool(BasicNode* node);
        void visitOutStmt(OutStmtNode* node);
        void visitVorcStmt(VorcStmtNode* node);
        void visitArrayDefineStmt(ArrayDefineStmtNode* node);
        void visitArrayAssignStmt(AssignArrayElementStmtNode* node);
        void visitAssignStmt(AssignStmtNode* node);
        void visitDelStmt(DeleteStmtNode* node);
        void visitInputStmt(InputStmtNode* node);
        void visitWhileStmt(WhileStmtNode* node);
        void visitIfStmt(IfStmtNode* node);
        void visitBlockStmt(BlockStmtNode* node);
        void visitStmt(StmtNode* node);
        void visitStat(StatNode* node);
    };
}
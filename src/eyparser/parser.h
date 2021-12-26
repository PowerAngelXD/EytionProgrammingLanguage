#pragma once

#include "../eylexer/lex.h"
#include "../eexception/eexcp.h"
#include "../eyparser/ast.h"
using namespace eylex;
using namespace eexcp;
using namespace eyast;

namespace eyparser{
    class Parser {
        std::vector<Token> input;
        std::vector<Token> copy;
        int cur_pos = 0;
        int copy_pos = 0;
    public:
        Parser(std::vector<Token> input);
        bool IsToken();
        BasicNode* token();
        eylex::Token peek(int move = 0);
        BasicNode* peeknode();
        BasicNode* tokenIncopy();
        eylex::Token peekIncopy(int move = 0);
        bool IsMulOperator();
        MulOperatorNode* MulOperator();
        bool IsAddOperator();
        AddOperatorNode* AddOperator();
        bool IsArrayElt();
        ArrayNode* ArrayElt();
        bool IsPrimExpr();
        PrimExprNode* PrimExpr();
        bool IsMulExpr();
        MulExprNode* MulExpr();
        bool IsAddExpr();
        AddExprNode* AddExpr();
        bool IsCmpOp();
        CmpOperatorNode* CmpOp();
        bool IsBoolOp();
        BoolOperatorNode* BoolOp();
        bool IsCmpExpr();
        CmpExprNode* CmpExpr();
        bool IsBoolExpr();
        BoolExprNode* BoolExpr();
        bool IsNotBoolExpr();
        NotBoolExprNode* NotBoolExpr();
        bool IsExpr();
        ExprNode* Expr();
        bool IsOutStmt();
        OutStmtNode* OutStmt();
        bool IsArrayDefineStmt();
        ArrayDefineStmtNode* ArrayDefineStmt();
        bool IsArrayAssignStmt();
        AssignArrayElementStmtNode* ArrayAssignStmt();
        bool IsVorcStmt();
        VorcStmtNode* VorcStmt();
        bool IsAssignStmt();
        AssignStmtNode* AssignStmt();
        bool IsDelStmt();
        DeleteStmtNode* DelStmt();
        bool IsInputStmt();
        InputStmtNode* InputStmt();
        bool IsWhileStmt();
        WhileStmtNode* WhileStmt();
        bool IsIfStmt();
        IfStmtNode* IfStmt();
        bool IsBlockStmt();
        BlockStmtNode* BlockStmt();
        bool IsStmt();
        StmtNode* Stmt();
        bool IsStat();
        StatNode* Stat();
    };
};
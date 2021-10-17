#pragma once

#include "..\eylexer\lex.h"
#include "..\eexception\eexcp.h"
using namespace eylex;
using namespace eexcp;

namespace eyparser {
    class Parser;
    class AddExprNode;
    class ExprNode;

    class TokenNode {
        Token _token;
    public:
        TokenNode();
        TokenNode(Token _token);

        Token token();
        std::string toString();

        friend class Parser;
    };


    // 加减法优先级运算符
    class AddOperatorNode {
        TokenNode* _AddOperator;
    public:
        AddOperatorNode();
        AddOperatorNode(TokenNode* _AddOperator);
        ~AddOperatorNode();

        TokenNode* AddOperator();
        std::string toString();

        friend class Parser;
    };

    // 乘除法优先级运算符
    class MulOperatorNode{
        TokenNode* _MulOperator;
    public:
        MulOperatorNode();
        MulOperatorNode(TokenNode* _MulOperator);
        ~MulOperatorNode();

        TokenNode* MulOperator();
        std::string toString();

        friend class Parser;
    };

    // 粗俗理解：大expr里的子expr，优先级最高
    class PrimExprNode {
        TokenNode* _Number = nullptr;
        TokenNode* _Iden = nullptr;
        TokenNode* _LeftParen;
        AddExprNode* _AddExpr;
        TokenNode* _RightParen;
    public:

        PrimExprNode();
        ~PrimExprNode();

        TokenNode* Number();
        TokenNode* Iden();
        TokenNode* LeftParen();
        TokenNode* RightParen();
        AddExprNode* AddExpr();
        std::string toString();

        friend class Parser;
    };

    class MulExprNode {
        std::vector<PrimExprNode*> _PrimExpr;
        std::vector<MulOperatorNode*> _MulOperator;
    public:

        MulExprNode();
        ~MulExprNode();

        std::vector<PrimExprNode*> PrimExpr();
        std::vector<MulOperatorNode*> MulOperator();
        PrimExprNode* PrimExpr(int sub);
        MulOperatorNode* MulOperator(int sub);
        std::string toString();

        friend class Parser;
    };

    // 粗俗理解：大expr
    class AddExprNode {
        std::vector<MulExprNode*> _MulExpr;
        std::vector<AddOperatorNode*> _AddOperator;
    public:

        AddExprNode();
        ~AddExprNode();

        std::vector<MulExprNode*> MulExpr();
        std::vector<AddOperatorNode*> AddOperator();
        MulExprNode* MulExpr(int sub);
        AddOperatorNode* AddOperator(int sub);
        std::string toString();

        friend class Parser;
    };

    // 比较运算符
    class CmpOperatorNode{
        TokenNode* _Op;
    public:
        CmpOperatorNode()=default;
        ~CmpOperatorNode()=default;

        TokenNode* Op();
        string toString();
        friend class Parser;
    };

    // 比较表达式
    class CmpExprNode{
        ExprNode* _Expr;
        CmpOperatorNode* _Op;
        ExprNode* _Target;
    public:
        CmpExprNode()=default;
        ~CmpExprNode()=default;

        ExprNode* Expr();
        CmpOperatorNode* Op();
        ExprNode* Target();

        string toString();

        friend class Parser;
    };

    class ExprNode{
        TokenNode* _String = nullptr;
        AddExprNode* _AddExpr = nullptr;
    public:
        ExprNode()=default;
        ~ExprNode()=default;

        TokenNode* String();
        AddExprNode* AddExpr();
        string toString();

        friend class Parser;
    };

    // 输出语句的ast
    class OutStmtNode {
        TokenNode* _OutMark;
        TokenNode* _StmtEndMark;
        ExprNode* _Expr;
    public:
        OutStmtNode();
        ~OutStmtNode();

        TokenNode* OutMark();
        ExprNode* Expr();
        std::string toString();
        friend class Parser;
    };
    
    // 变量声明语句的ast
    class VorcStmtNode{
        TokenNode* _VarMark = nullptr;
        TokenNode* _ConstMark = nullptr;
        TokenNode* _LT;
        TokenNode* _Type;
        TokenNode* _GT;
        TokenNode* _IdenName;
        TokenNode* _Equ;

        ExprNode* _Expr;

        TokenNode* _EndMark;

    public:
        VorcStmtNode();
        ~VorcStmtNode();

        TokenNode* VarMark();
        TokenNode* ConstMark();
        TokenNode* LT();
        TokenNode* Type();
        TokenNode* GT();
        TokenNode* IdenName();
        TokenNode* Equ();
        ExprNode* Expr();
        std::string toString();
        friend class Parser;
    };

    // 赋值语句的stmt
    class AssignStmtNode{
        TokenNode* _Iden;
        TokenNode* _Equ;

        ExprNode* _Expr;

        TokenNode* _EndMark;
    public:
        AssignStmtNode();
        ~AssignStmtNode();

        TokenNode* Iden();
        TokenNode* Equ();
        ExprNode* Expr();
        TokenNode* EndMark();

        std::string toString();

        friend class Parser;
    };

    class DeleteStmtNode{
        TokenNode* _DeleteMark;
        TokenNode* _Iden;
        TokenNode* _EndMark;
    public:
        DeleteStmtNode()=default;
        ~DeleteStmtNode();

        TokenNode* DeleteMark();
        TokenNode* Iden();
        TokenNode* EndMark();
        string toString();

        friend class Parser;
    };

    class BlockStmtNode;

    class StmtNode {
        OutStmtNode* _OutStmt = nullptr;
        VorcStmtNode* _VorcStmt = nullptr;
        AssignStmtNode* _AssignStmt = nullptr;
        BlockStmtNode* _BlockStmt = nullptr;
        DeleteStmtNode* _DeleteStmt = nullptr;
    public:
        StmtNode()=default;
        OutStmtNode* OutStmt();
        VorcStmtNode* VorcStmt();
        AssignStmtNode* AssignStmt();
        BlockStmtNode* BlockStmt();
        DeleteStmtNode* DeleteStmt();
        std::string toString();

        friend class Parser;
    };

    class StatNode{
        std::vector<StmtNode*> _Stmt;
    public:

        StatNode()=default;
        //~StatNode()=default;

        std::vector<StmtNode*> Stmt();
        StmtNode* Stmt(int sub);
        std::string toString();

        friend class Parser;
    };

    // blockSTMT
    class BlockStmtNode{
        TokenNode* _Left;
        vector<StmtNode*> _Stmt;
        TokenNode* _Right;
    public:
        BlockStmtNode()=default;

        TokenNode* Left();
        vector<StmtNode*> Stmts();
        TokenNode* Right();
        string toString();

        friend class Parser;
    };

    class Parser {
        std::vector<Token> input;
        std::vector<Token> copy;
        int cur_pos = 0;
        int copy_pos = 0;
    public:
        Parser(std::vector<Token> input);
        bool IsToken();
        TokenNode* token();
        eylex::Token peek(int move = 0);
        TokenNode* peeknode();
        TokenNode* tokenIncopy();
        eylex::Token peekIncopy(int move = 0);
        bool IsMulOperator();
        MulOperatorNode* MulOperator();
        bool IsAddOperator();
        AddOperatorNode* AddOperator();
        bool IsPrimExpr();
        PrimExprNode* PrimExpr();
        bool IsMulExpr();
        MulExprNode* MulExpr();
        bool IsAddExpr();
        AddExprNode* AddExpr();
        bool IsExpr();
        ExprNode* Expr();
        bool IsOutStmt();
        OutStmtNode* OutStmt();
        bool IsVorcStmt();
        VorcStmtNode* VorcStmt();
        bool IsAssignStmt();
        AssignStmtNode* AssignStmt();
        bool IsDelStmt();
        DeleteStmtNode* DelStmt();
        bool IsBlockStmt();
        BlockStmtNode* BlockStmt();
        bool IsStmt();
        StmtNode* Stmt();
        bool IsStat();
        StatNode* Stat();
    };
}
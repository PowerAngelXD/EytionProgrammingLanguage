#pragma once

#include "..\eylexer\lex.h"
#include "..\eexception\eexcp.h"
using namespace eylex;
using namespace eexcp;

namespace eyast {
    class AddExprNode;
    class BlockStmtNode;
    class ExprNode;

    class TokenNode {
    public:
        Token _token;

        TokenNode();
        TokenNode(Token _token);

        Token token();
        std::string toString();
    };


    // 加减法优先级运算符
    class AddOperatorNode {
    public:
        TokenNode* _AddOperator;

        AddOperatorNode();
        AddOperatorNode(TokenNode* _AddOperator);
        ~AddOperatorNode();

        TokenNode* AddOperator();
        std::string toString();
    };

    // 乘除法优先级运算符
    class MulOperatorNode{
    public:
        TokenNode* _MulOperator;

        MulOperatorNode();
        MulOperatorNode(TokenNode* _MulOperator);
        ~MulOperatorNode();

        TokenNode* MulOperator();
        std::string toString();
    };

    // 粗俗理解：大expr里的子expr，优先级最高
    class PrimExprNode {
    public:
        TokenNode* _Number = nullptr;
        TokenNode* _Iden = nullptr;
        TokenNode* _String = nullptr;
        TokenNode* _LeftParen;
        AddExprNode* _AddExpr;
        TokenNode* _RightParen;

        PrimExprNode();
        ~PrimExprNode();

        TokenNode* Number();
        TokenNode* Iden();
        TokenNode* String();
        TokenNode* LeftParen();
        TokenNode* RightParen();
        AddExprNode* AddExpr();
        std::string toString();
    };

    class MulExprNode {
    public:
        std::vector<PrimExprNode*> _PrimExpr;
        std::vector<MulOperatorNode*> _MulOperator;

        MulExprNode();
        ~MulExprNode();

        std::vector<PrimExprNode*> PrimExpr();
        std::vector<MulOperatorNode*> MulOperator();
        PrimExprNode* PrimExpr(int sub);
        MulOperatorNode* MulOperator(int sub);
        std::string toString();
    };

    // 粗俗理解：大expr
    class AddExprNode {
    public:
        std::vector<MulExprNode*> _MulExpr;
        std::vector<AddOperatorNode*> _AddOperator;

        AddExprNode();
        ~AddExprNode();

        std::vector<MulExprNode*> MulExpr();
        std::vector<AddOperatorNode*> AddOperator();
        MulExprNode* MulExpr(int sub);
        AddOperatorNode* AddOperator(int sub);
        std::string toString();

        
    };

    // 比较运算符
    class CmpOperatorNode{
    public:
        TokenNode* _Op;

        CmpOperatorNode()=default;
        ~CmpOperatorNode()=default;

        TokenNode* Op();
        string toString();
        
    };

    // 比较表达式
    class CmpExprNode{
    public:
        AddExprNode* _Expr = nullptr;
        CmpOperatorNode* _Op = 0x0;
        AddExprNode* _Tar = 0x0;

        CmpExprNode()=default;
        ~CmpExprNode()=default;

        AddExprNode* Expr();
        CmpOperatorNode* Op();
        AddExprNode* Sub();

        string toString();

        
    };

    class BoolOperatorNode{
    public:
        TokenNode* _Op;

        BoolOperatorNode()=default;
        ~BoolOperatorNode()=default;
        TokenNode* Op();
        string toString();
        
    };

    class BoolExprNode{
    public:
        CmpExprNode* _Root;
        vector<BoolOperatorNode*> _Op;
        vector<CmpExprNode*> _Sub;

        BoolExprNode()=default;
        ~BoolExprNode()=default;
        string toString();
        CmpExprNode* Root();
        BoolOperatorNode* Op(int sub);
        CmpExprNode* Sub(int sub);
        vector<BoolOperatorNode*> Op();
        vector<CmpExprNode*> Sub();
        
    };

    class NotBoolExprNode{
    public:
        TokenNode* _Op;
        BoolExprNode* _Boolexpr;

        NotBoolExprNode()=default;
        ~NotBoolExprNode()=default;
        string toString();
        TokenNode* Op();
        BoolExprNode* Boolexpr();
        
    };

    class ExprNode{
    public:
        AddExprNode* _AddExpr = nullptr;
        BoolExprNode* _BoolExpr = nullptr;
        NotBoolExprNode* _NotBoolExpr = nullptr;

        ExprNode()=default;
        ~ExprNode()=default;

        AddExprNode* AddExpr();
        BoolExprNode* BoolExpr();
        NotBoolExprNode* NotBoolExpr();
        string toString();

        
    };

    // 输出语句的ast
    class OutStmtNode {
    public:
        TokenNode* _OutMark;
        TokenNode* _StmtEndMark;
        ExprNode* _Expr;

        OutStmtNode();
        ~OutStmtNode();

        TokenNode* OutMark();
        ExprNode* Expr();
        std::string toString();
        
    };
    
    // 变量声明语句的ast
    class VorcStmtNode{
    public:
        TokenNode* _VarMark = nullptr;
        TokenNode* _ConstMark = nullptr;
        TokenNode* _LT;
        TokenNode* _Type;
        TokenNode* _GT;
        TokenNode* _IdenName;
        TokenNode* _Equ;

        ExprNode* _Expr;

        TokenNode* _EndMark;

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
        
    };

    // 赋值语句的stmt
    class AssignStmtNode{
    public:
        TokenNode* _Iden;
        TokenNode* _Equ;

        ExprNode* _Expr;

        TokenNode* _EndMark;

        AssignStmtNode();
        ~AssignStmtNode();

        TokenNode* Iden();
        TokenNode* Equ();
        ExprNode* Expr();
        TokenNode* EndMark();

        std::string toString();

        
    };

    class DeleteStmtNode{
    public:
        TokenNode* _DeleteMark;
        TokenNode* _Iden;
        TokenNode* _EndMark;

        DeleteStmtNode()=default;
        ~DeleteStmtNode();

        TokenNode* DeleteMark();
        TokenNode* Iden();
        TokenNode* EndMark();
        string toString();

        
    };

    class InputStmtNode{
    public:
        TokenNode* _InputMark;
        AddExprNode* _Content;
        TokenNode* _Give;
        TokenNode* _Iden;
        TokenNode* _EndMark;

        InputStmtNode()=default;
        TokenNode* InputMark();
        AddExprNode* Content();
        TokenNode* Give();
        TokenNode* Iden();
        TokenNode* EndMark();

        string toString();
        
    };

    class RepeatStmtNode{
    public:
        TokenNode* _RepeatMark;
        AddExprNode* _Times;
        TokenNode* _To;
        BlockStmtNode* _Block;

        RepeatStmtNode()=default;
        TokenNode* RepeatMark();
        AddExprNode* Times();
        TokenNode* To();
        BlockStmtNode* Block();
        string toString();

        
    };

    class IfStmtNode{
    public:
        TokenNode* _IfMark;
        ExprNode* _Cond;
        TokenNode* _To;
        BlockStmtNode* _Block;

        IfStmtNode()=default;
        TokenNode* IfMark();
        ExprNode* Cond();
        TokenNode* To();
        BlockStmtNode* Block();
        string toString();

        
    };

    class StmtNode {
    public:
        OutStmtNode* _OutStmt = nullptr;
        VorcStmtNode* _VorcStmt = nullptr;
        AssignStmtNode* _AssignStmt = nullptr;
        BlockStmtNode* _BlockStmt = nullptr;
        DeleteStmtNode* _DeleteStmt = nullptr;
        InputStmtNode* _InputStmt = nullptr;
        RepeatStmtNode* _RepeatStmt = nullptr;
        IfStmtNode* _IfStmt = nullptr;

        StmtNode()=default;
        OutStmtNode* OutStmt();
        VorcStmtNode* VorcStmt();
        AssignStmtNode* AssignStmt();
        BlockStmtNode* BlockStmt();
        DeleteStmtNode* DeleteStmt();
        InputStmtNode* InputStmt();
        RepeatStmtNode* RepeatStmt();
        IfStmtNode* IfStmt();
        std::string toString();

        
    };

    class StatNode{
    public:
        std::vector<StmtNode*> _Stmt;


        StatNode()=default;
        //~StatNode()=default;

        std::vector<StmtNode*> Stmt();
        StmtNode* Stmt(int sub);
        std::string toString();

        
    };

    // blockSTMT
    class BlockStmtNode{
    public:
        TokenNode* _Left;
        vector<StmtNode*> _Stmt;
        TokenNode* _Right;
    public:
        BlockStmtNode()=default;

        TokenNode* Left();
        vector<StmtNode*> Stmts();
        TokenNode* Right();
        string toString();

        
    };
};
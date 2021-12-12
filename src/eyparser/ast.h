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

    // 数组node
    class ArrayNode {
    public:
        TokenNode* _Iden;
        TokenNode* _Left;
        AddExprNode* _Index;
        TokenNode* _Right;

        ArrayNode()=default;
        TokenNode* Iden();
        TokenNode* Left();
        AddExprNode* Index();
        TokenNode* Right();

        string toString();
    };

    // 粗俗理解：大expr里的子expr，优先级最高
    class PrimExprNode {
    public:
        TokenNode* _Number = nullptr;
        TokenNode* _Iden = nullptr;
        ArrayNode* _ArrayElt = nullptr;
        TokenNode* _String = nullptr;
        TokenNode* _ConstBool = nullptr;
        TokenNode* _LeftParen;
        AddExprNode* _AddExpr;
        TokenNode* _RightParen;

        PrimExprNode();
        ~PrimExprNode();

        TokenNode* Number();
        TokenNode* Iden();
        ArrayNode* ArrayElt();
        TokenNode* String();
        TokenNode* ConstBool();
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

        ExprNode* _Expr = nullptr;

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
    class ArrayDefineStmtNode{
        // array <[3]string> list = ["1","2","3"];
    public:
        TokenNode* _ArrayDefineMark;
        TokenNode* _LeftB;  // <
        TokenNode* _Left;   // [
        TokenNode* _Number;
        TokenNode* _Right;  // ]
        TokenNode* _Type;
        TokenNode* _RightB; // >
        TokenNode* _Iden;
        TokenNode* _Eq;
        TokenNode* _GroupBegin;
        vector<ExprNode*> _Elts;
        vector<TokenNode*> _Dots;
        TokenNode* _GroupEnd;
        TokenNode* _EndMark;

        ArrayDefineStmtNode()=default;
        TokenNode* ArrayDefineMark();
        TokenNode* LeftB();  // <
        TokenNode* Left();   // [
        TokenNode* Number();
        TokenNode* Right();  // ]
        TokenNode* Type();
        TokenNode* RightB(); // >
        TokenNode* Iden();
        TokenNode* Eq();
        TokenNode* GroupBegin();
        vector<ExprNode*> Elts();
        vector<TokenNode*> Dots();
        TokenNode* GroupEnd();
        TokenNode* EndMark();
        string toString();
    };

    class AssignArrayElementStmtNode{
        // list[0] = 1;
    public:
        ArrayNode* _ArrayElt;
        TokenNode* _Eq;
        ExprNode* _Expr;
        TokenNode* _EndMark;

        AssignArrayElementStmtNode()=default;
        ArrayNode* ArrayElt();
        TokenNode* Eq();
        ExprNode* Expr();
        TokenNode* EndMark();
        string toString();
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

    class WhileStmtNode{
    public:
        TokenNode* _WhileMark;
        ExprNode* _Cond;
        TokenNode* _To;
        BlockStmtNode* _Block;

        WhileStmtNode()=default;
        TokenNode* WhileMark();
        ExprNode* Cond();
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
        ArrayDefineStmtNode* _ArrayDefineStmt = nullptr;
        AssignArrayElementStmtNode* _AssignArrayStmt = nullptr;
        AssignStmtNode* _AssignStmt = nullptr;
        BlockStmtNode* _BlockStmt = nullptr;
        DeleteStmtNode* _DeleteStmt = nullptr;
        InputStmtNode* _InputStmt = nullptr;
        WhileStmtNode* _WhileStmt = nullptr;
        IfStmtNode* _IfStmt = nullptr;

        StmtNode()=default;
        OutStmtNode* OutStmt();
        ArrayDefineStmtNode* ArrayDefineStmt();
        AssignArrayElementStmtNode* AssignArrayStmt();
        VorcStmtNode* VorcStmt();
        AssignStmtNode* AssignStmt();
        BlockStmtNode* BlockStmt();
        DeleteStmtNode* DeleteStmt();
        InputStmtNode* InputStmt();
        WhileStmtNode* WhileStmt();
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
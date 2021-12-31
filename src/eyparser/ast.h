#pragma once

#include "../eylexer/lex.h"
#include "../eexception/eexcp.h"
using namespace eylex;
using namespace eexcp;

namespace eyast {
    class AddExprNode;
    class BlockStmtNode;
    class ExprNode;

    class BasicNode {
    public:
        Token _token;

        BasicNode();
        BasicNode(Token _token);

        Token token();
        std::string toString();
    };


    // 加减法优先级运算符
    class AddOperatorNode {
    public:
        BasicNode* _AddOperator;

        AddOperatorNode();
        AddOperatorNode(BasicNode* _AddOperator);
        ~AddOperatorNode();

        BasicNode* AddOperator();
        std::string toString();
    };

    // 乘除法优先级运算符
    class MulOperatorNode{
    public:
        BasicNode* _MulOperator;

        MulOperatorNode();
        MulOperatorNode(BasicNode* _MulOperator);
        ~MulOperatorNode();

        BasicNode* MulOperator();
        std::string toString();
    };

    // 数组node
    class ArrayNode {
    public:
        BasicNode* _Iden;
        BasicNode* _Left;
        AddExprNode* _Index;
        BasicNode* _Right;

        ArrayNode()=default;
        BasicNode* Iden();
        BasicNode* Left();
        AddExprNode* Index();
        BasicNode* Right();

        string toString();
    };

    // 粗俗理解：大expr里的子expr，优先级最高
    class PrimExprNode {
    public:
        BasicNode* _Number = nullptr;
        BasicNode* _Iden = nullptr;
        ArrayNode* _ArrayElt = nullptr;
        BasicNode* _String = nullptr;
        BasicNode* _ConstBool = nullptr;
        BasicNode* _LeftParen;
        AddExprNode* _AddExpr;
        BasicNode* _RightParen;

        PrimExprNode();
        ~PrimExprNode();

        BasicNode* Number();
        BasicNode* Iden();
        ArrayNode* ArrayElt();
        BasicNode* String();
        BasicNode* ConstBool();
        BasicNode* LeftParen();
        BasicNode* RightParen();
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
        BasicNode* _Op;

        CmpOperatorNode()=default;
        ~CmpOperatorNode()=default;

        BasicNode* Op();
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
        BasicNode* _Op;

        BoolOperatorNode()=default;
        ~BoolOperatorNode()=default;
        BasicNode* Op();
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
        BasicNode* _Op;
        BoolExprNode* _Boolexpr;

        NotBoolExprNode()=default;
        ~NotBoolExprNode()=default;
        string toString();
        BasicNode* Op();
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

    // 类型ast
    class TypeExplNode {
    public:
        BasicNode* _Left;
        BasicNode* _ArrLeft = nullptr;
        BasicNode* _ArrLength = nullptr;
        BasicNode* _ArrRight = nullptr;
        BasicNode* _TypeName;
        BasicNode* _Right;
        BasicNode* _Iden;
        bool isArr = false;

        TypeExplNode()=default;
        ~TypeExplNode()=default;

        BasicNode* Left();
        BasicNode* ArrLeft();
        BasicNode* ArrLength();
        BasicNode* ArrRight();
        BasicNode* TypeName();
        BasicNode* Right();
        BasicNode* Iden();

        string toString();
    };

    // 输出语句的ast
    class OutStmtNode {
    public:
        BasicNode* _OutMark;
        BasicNode* _StmtEndMark;
        ExprNode* _Expr;

        OutStmtNode();
        ~OutStmtNode();

        BasicNode* OutMark();
        ExprNode* Expr();
        std::string toString();
        
    };

    // 函数声明ast
    class FunctionDefineStmtNode{
        // func <int>add(<int>a, <int>b):{}
    public:
        BasicNode* _FunctionDefineMark;
        TypeExplNode* _Name;
        BasicNode* _Left;
        vector<TypeExplNode*> _Argvs;
        vector<BasicNode*> _Dots;
        BasicNode* _Right;
        BasicNode* _M;
        BlockStmtNode* _Block;
    };
    
    // 变量声明语句的ast
    class VorcStmtNode{
    public:
        BasicNode* _VarMark = nullptr;
        BasicNode* _ConstMark = nullptr;
        TypeExplNode* _DefineExpl;
        BasicNode* _Equ;

        ExprNode* _Expr = nullptr;

        BasicNode* _EndMark;

        VorcStmtNode();
        ~VorcStmtNode();

        BasicNode* VarMark();
        BasicNode* ConstMark();
        TypeExplNode* DefineExpl();
        BasicNode* Equ();
        ExprNode* Expr();
        std::string toString();
        
    };
    class ArrayDefineStmtNode{
        // array <[3]string> list = ["1","2","3"];
    public:
        BasicNode* _ArrayDefineMark;
        BasicNode* _LeftB;  // <
        BasicNode* _Left;   // [
        BasicNode* _Number;
        BasicNode* _Right;  // ]
        BasicNode* _Type;
        BasicNode* _RightB; // >
        BasicNode* _Iden;
        BasicNode* _Eq;
        BasicNode* _GroupBegin;
        vector<ExprNode*> _Elts;
        vector<BasicNode*> _Dots;
        BasicNode* _GroupEnd;
        BasicNode* _EndMark;

        ArrayDefineStmtNode()=default;
        BasicNode* ArrayDefineMark();
        BasicNode* LeftB();  // <
        BasicNode* Left();   // [
        BasicNode* Number();
        BasicNode* Right();  // ]
        BasicNode* Type();
        BasicNode* RightB(); // >
        BasicNode* Iden();
        BasicNode* Eq();
        BasicNode* GroupBegin();
        vector<ExprNode*> Elts();
        vector<BasicNode*> Dots();
        BasicNode* GroupEnd();
        BasicNode* EndMark();
        string toString();
    };

    class AssignArrayElementStmtNode{
        // list[0] = 1;
    public:
        ArrayNode* _ArrayElt;
        BasicNode* _Eq;
        ExprNode* _Expr;
        BasicNode* _EndMark;

        AssignArrayElementStmtNode()=default;
        ArrayNode* ArrayElt();
        BasicNode* Eq();
        ExprNode* Expr();
        BasicNode* EndMark();
        string toString();
    };

    // 赋值语句的stmt
    class AssignStmtNode{
    public:
        BasicNode* _Iden;
        BasicNode* _Equ;

        ExprNode* _Expr;

        BasicNode* _EndMark;

        AssignStmtNode();
        ~AssignStmtNode();

        BasicNode* Iden();
        BasicNode* Equ();
        ExprNode* Expr();
        BasicNode* EndMark();

        std::string toString();

        
    };

    class DeleteStmtNode{
    public:
        BasicNode* _DeleteMark;
        BasicNode* _Iden;
        BasicNode* _EndMark;

        DeleteStmtNode()=default;
        ~DeleteStmtNode();

        BasicNode* DeleteMark();
        BasicNode* Iden();
        BasicNode* EndMark();
        string toString();

        
    };

    class InputStmtNode{
    public:
        BasicNode* _InputMark;
        AddExprNode* _Content;
        BasicNode* _Give;
        BasicNode* _Iden;
        BasicNode* _EndMark;

        InputStmtNode()=default;
        BasicNode* InputMark();
        AddExprNode* Content();
        BasicNode* Give();
        BasicNode* Iden();
        BasicNode* EndMark();

        string toString();
        
    };

    class WhileStmtNode{
    public:
        BasicNode* _WhileMark;
        ExprNode* _Cond;
        BasicNode* _To;
        BlockStmtNode* _Block;

        WhileStmtNode()=default;
        BasicNode* WhileMark();
        ExprNode* Cond();
        BasicNode* To();
        BlockStmtNode* Block();
        string toString();
    };

    class IfStmtNode{
    public:
        BasicNode* _IfMark;
        ExprNode* _Cond;
        BasicNode* _To;
        BlockStmtNode* _Block;

        IfStmtNode()=default;
        BasicNode* IfMark();
        ExprNode* Cond();
        BasicNode* To();
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
        BasicNode* _Left;
        vector<StmtNode*> _Stmt;
        BasicNode* _Right;
    public:
        BlockStmtNode()=default;

        BasicNode* Left();
        vector<StmtNode*> Stmts();
        BasicNode* Right();
        string toString();

        
    };
};
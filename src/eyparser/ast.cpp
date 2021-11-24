#include "ast.h"
using namespace eyast;
//TokenNode Part

TokenNode::TokenNode() {}
TokenNode::TokenNode(Token _token) : _token(_token) {}

Token TokenNode::token() {
    return _token;
}

std::string TokenNode::toString() {
    return ::toString(this->_token.symbol) + ":\"" + this->_token.content + "\"";
}

// AddOperatorNode Part

AddOperatorNode::AddOperatorNode() : _AddOperator(0) {}
AddOperatorNode::AddOperatorNode(TokenNode* _AddOperator) : _AddOperator(_AddOperator) {}

AddOperatorNode::~AddOperatorNode() {
    delete _AddOperator;
}

TokenNode* AddOperatorNode::AddOperator() {
    return _AddOperator;
}

std::string AddOperatorNode::toString() {
    return (std::string)"AddOp: {" + _AddOperator->toString() + "}";
}

// MulOperator Part

MulOperatorNode::MulOperatorNode() : _MulOperator(0) {}
MulOperatorNode::MulOperatorNode(TokenNode* _MulOperator) : _MulOperator(_MulOperator) {}
MulOperatorNode::~MulOperatorNode() {
    delete _MulOperator;
}

TokenNode* MulOperatorNode::MulOperator() {
    return _MulOperator;
}

std::string MulOperatorNode::toString() {
    return (std::string)"MulOp: {" + _MulOperator->toString() + "}";
}

// PrimExprNode Part

PrimExprNode::PrimExprNode() : _Number(0), _Iden(0), _LeftParen(0), _AddExpr(0), _RightParen(0) {};
PrimExprNode::~PrimExprNode() {
    if(_Number)
        delete _Number;
    else if(_Iden)
        delete _Iden;
    else
        delete _AddExpr;
};

TokenNode* PrimExprNode::Number() {
    return _Number;
}

TokenNode* PrimExprNode::Iden() {
    return _Iden;
}

TokenNode* PrimExprNode::String() {
    return _String;
}

TokenNode* PrimExprNode::LeftParen() {
    return _LeftParen;
}

AddExprNode* PrimExprNode::AddExpr() {
    return _AddExpr;
}

TokenNode* PrimExprNode::RightParen() {
    return _RightParen;
}

std::string PrimExprNode::toString() {
    if (_Number != nullptr) {
        return (std::string)"PrimExpr:{" + _Number->toString() + "}";
    }
    else if (_Iden != nullptr) {
        return (std::string)"PrimExpr:{" + _Iden->toString() + "}";
    }
    else if (_String != nullptr) {
        return (std::string)"PrimExpr:{" + _String->toString() + "}";
    }
    else {
        return (std::string)"PrimExpr:{" + _AddExpr->toString() + "}";
    }
}

//MulExprNode Part

MulExprNode::MulExprNode() {}
MulExprNode::~MulExprNode() {
    for (auto e : _PrimExpr)
        delete e;
    for (auto e : _MulOperator)
        delete e;
}

std::vector<PrimExprNode*> MulExprNode::PrimExpr() {
    return _PrimExpr;
}

std::vector<MulOperatorNode*> MulExprNode::MulOperator() {
    return _MulOperator;
}

PrimExprNode* MulExprNode::PrimExpr(int sub) {
    return _PrimExpr[sub];
}

MulOperatorNode* MulExprNode::MulOperator(int sub) {
    return _MulOperator[sub];
}

std::string MulExprNode::toString() {
    std::string str = "MulExpr: {";
    str += _PrimExpr[0]->toString();
    for (int i = 0 ; i < _MulOperator.size() ; i ++) {
        auto op = _MulOperator[i]->toString();
        auto op_num = _PrimExpr[i + 1]->toString();
        str += "," + op + "," + op_num;
    }
    str += "}";
    return str;
}

// AddExprNode Part

AddExprNode::AddExprNode() {}
AddExprNode::~AddExprNode() {
    for (auto e : _MulExpr)
        delete e;
    for (auto e : _AddOperator)
        delete e;
}

std::vector<MulExprNode*> AddExprNode::MulExpr() {
    return _MulExpr;
}

std::vector<AddOperatorNode*> AddExprNode::AddOperator() {
    return _AddOperator;
}

MulExprNode* AddExprNode::MulExpr(int sub) {
    return _MulExpr[sub];
}

AddOperatorNode* AddExprNode::AddOperator(int sub) {
    return _AddOperator[sub];
}

std::string AddExprNode::toString() {
    std::string str = "AddExpr: {";
    str += _MulExpr[0]->toString();
    for (int i = 0 ; i < _AddOperator.size() ; i ++) {
        auto op = _AddOperator[i]->toString();
        auto op_num = _MulExpr[i + 1]->toString();
        str += "," + op + "," + op_num;
    }
    str += "}";
    return str;
}

//Bool Expr Part
TokenNode* CmpOperatorNode::Op(){return _Op;}
string CmpOperatorNode::toString(){return "CmpOp: {" + _Op->toString() + "}";}

AddExprNode* CmpExprNode::Expr(){return _Expr;}
CmpOperatorNode* CmpExprNode::Op(){return _Op;}
AddExprNode* CmpExprNode::Sub(){return _Tar;}
string CmpExprNode::toString(){
    if(_Op != nullptr)
        return "CmpExpr: {" + _Expr->toString() + ", " + _Op->toString() + ", " + _Tar->toString() + "}";
    return "CmpExpr: {" + _Expr->toString() + "}";    
}

TokenNode* BoolOperatorNode::Op(){return _Op;}
string BoolOperatorNode::toString(){return "BoolOp: {" + _Op->toString() + "}";}

string BoolExprNode::toString(){
    std::string str = "BoolExpr: {";
    str += _Root->toString();
    for (int i = 0 ; i < _Op.size() ; i ++) {
        auto op = _Op[i]->toString();
        auto op_num = _Sub[i + 1]->toString();
        str += "," + op + "," + op_num;
    }
    str += "}";
    return str;
}
CmpExprNode* BoolExprNode::Root(){return _Root;}
BoolOperatorNode* BoolExprNode::Op(int sub){return _Op[sub];}
CmpExprNode* BoolExprNode::Sub(int sub){return _Sub[sub];}
vector<BoolOperatorNode*> BoolExprNode::Op(){return _Op;}
vector<CmpExprNode*> BoolExprNode::Sub(){return _Sub;}

string NotBoolExprNode::toString(){
    return "NotBoolExpr: {" + _Boolexpr->toString() + "}";
}
TokenNode* NotBoolExprNode::Op(){return _Op;}
BoolExprNode* NotBoolExprNode::Boolexpr(){return _Boolexpr;}

//Expr Part
AddExprNode* ExprNode::AddExpr(){
    return _AddExpr;
}
BoolExprNode* ExprNode::BoolExpr(){
    return _BoolExpr;
}
NotBoolExprNode* ExprNode::NotBoolExpr(){
    return _NotBoolExpr;
}
string ExprNode::toString(){
    if(_AddExpr != nullptr){
        return "Expr(AddExpr): {" + _AddExpr->toString() + "}";
    }
    else if(_BoolExpr != nullptr){
        return "Expr(BoolExpr): {" + _BoolExpr->toString() + "}";
    }
    else if(_NotBoolExpr != nullptr){
        return "Expr(NotBoolExpr): {" + _NotBoolExpr->toString() + "}";
    }
    else {
        return "{__null__}";
    }
}

//OutStmt Part

TokenNode* OutStmtNode::OutMark(){return this->_OutMark;}
ExprNode* OutStmtNode::Expr(){return this->_Expr;}

OutStmtNode::OutStmtNode(){}
OutStmtNode::~OutStmtNode(){
    delete _OutMark;
    delete _Expr;
    delete _StmtEndMark;
}

std::string OutStmtNode::toString(){
    return "OutStmt:{" + this->_OutMark->toString() + "," + this->_Expr->toString() + "," + this->_StmtEndMark->toString() + "}";
}

//LetStmt Part

VorcStmtNode::VorcStmtNode(){}
VorcStmtNode::~VorcStmtNode(){
    delete _VarMark;
    delete _IdenName;
    delete _Equ;
    delete _Expr;
    delete _EndMark;
}

TokenNode* VorcStmtNode::VarMark(){return _VarMark;}
TokenNode* VorcStmtNode::ConstMark(){return _ConstMark;}
TokenNode* VorcStmtNode::LT(){return this->_LT;}
TokenNode* VorcStmtNode::Type(){return this->_Type;}
TokenNode* VorcStmtNode::GT(){return this->_GT;}
TokenNode* VorcStmtNode::IdenName(){return _IdenName;}
TokenNode* VorcStmtNode::Equ(){return _Equ;}
ExprNode* VorcStmtNode::Expr(){return _Expr;}
std::string VorcStmtNode::toString(){
    if (this->_VarMark != nullptr)
        return "VorcStmt(var):{" + this->_VarMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
    else
        return "VorcStmt(const):{" + this->_ConstMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
}

//AssignStmt Part

AssignStmtNode::AssignStmtNode(){}
AssignStmtNode::~AssignStmtNode(){
    delete _Iden;
    delete _Equ;
    delete _Expr;
    delete _EndMark;
}

TokenNode* AssignStmtNode::Iden(){return _Iden;}
TokenNode* AssignStmtNode::Equ(){return _Equ;}
ExprNode* AssignStmtNode::Expr(){return _Expr;}
TokenNode* AssignStmtNode::EndMark(){return _EndMark;}

std::string AssignStmtNode::toString(){
    return "AssigntStmt:{" + this->_Iden->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
}

//Delete StmtPart
DeleteStmtNode::~DeleteStmtNode(){
    delete this->_DeleteMark;
    delete this->_EndMark;
    delete this->_Iden;
}

TokenNode* DeleteStmtNode::DeleteMark(){return this->_DeleteMark;}
TokenNode* DeleteStmtNode::Iden(){return this->_Iden;}
TokenNode* DeleteStmtNode::EndMark(){return this->_EndMark;}
string DeleteStmtNode::toString(){
    return "DeleteStmt:{" + this->DeleteMark()->toString() + "," + this->Iden()->toString()  + "," + this->EndMark()->toString() +  "}";
}

//InputStmt Part
TokenNode* InputStmtNode::InputMark(){return _InputMark;}
AddExprNode* InputStmtNode::Content(){return _Content;}
TokenNode* InputStmtNode::Give(){return _Give;}
TokenNode* InputStmtNode::Iden(){return _Iden;}
TokenNode* InputStmtNode::EndMark(){return _EndMark;}

string InputStmtNode::toString(){
    return "InputStmt: {" + _InputMark->toString() + ", " + _Content->toString() + ", " + _Give->toString() + ", " + _Iden->toString() + ", " + _EndMark->toString() + "}";
}

//RepeatStmt Part
TokenNode* RepeatStmtNode::RepeatMark(){return _RepeatMark;}
AddExprNode* RepeatStmtNode::Times(){return _Times;}
TokenNode* RepeatStmtNode::To(){return _To;}
BlockStmtNode* RepeatStmtNode::Block(){return _Block;}
string RepeatStmtNode::toString(){
    return "RepeatStmt: {" + _RepeatMark->toString() + ", Times[" + _Times->toString() + "], " + _To->toString() + ", " + _Block->toString() + "}";
}

//IfStmt Part
TokenNode* IfStmtNode::IfMark(){return _IfMark;}
ExprNode* IfStmtNode::Cond(){return _Cond;}
TokenNode* IfStmtNode::To(){return _To;}
BlockStmtNode* IfStmtNode::Block(){return _Block;}
string IfStmtNode::toString(){return "IfStmt: {" + _IfMark->toString() + ", Cond[" + _Cond->toString() + "], " + _To->toString() + ", " + _Block->toString() + "}";}

//Stmt Part

OutStmtNode* StmtNode::OutStmt() {return _OutStmt;}
VorcStmtNode* StmtNode::VorcStmt() {return _VorcStmt;}
AssignStmtNode* StmtNode::AssignStmt() {return _AssignStmt;}
BlockStmtNode* StmtNode::BlockStmt(){return _BlockStmt;}
DeleteStmtNode* StmtNode::DeleteStmt(){return _DeleteStmt;}
InputStmtNode* StmtNode::InputStmt(){return _InputStmt;}
RepeatStmtNode* StmtNode::RepeatStmt(){return _RepeatStmt;}
IfStmtNode* StmtNode::IfStmt(){return _IfStmt;}

std::string StmtNode::toString() {
    std::string str = "stmt:";
    if (_OutStmt) {
        str += "{" + _OutStmt->toString() + "}";
    }
    else if(_VorcStmt) {
        str += "{" + _VorcStmt->toString() + "}";
    }
    else if(_AssignStmt) {
        str += "{" + _AssignStmt->toString() + "}";
    }
    else if(_BlockStmt) {
        str += "{" + _BlockStmt->toString() + "}";
    }
    else if(_DeleteStmt) {
        str += "{" + _DeleteStmt->toString() + "}";
    }
    else if(_InputStmt) {
        str += "{" + _InputStmt->toString() + "}";
    }
    else if(_RepeatStmt) {
        str += "{" + _RepeatStmt->toString() + "}";
    }
    else if(_IfStmt) {
        str += "{" + _IfStmt->toString() + "}";
    }
    return str;
}

//Stat Part

std::vector<StmtNode*> StatNode::Stmt() {return this->_Stmt;}
StmtNode* StatNode::Stmt(int sub) {return this->_Stmt[sub];}

std::string StatNode::toString(){
    std::string str = "stat:[";
    int i = 0 ;
    for(auto stmt : this->_Stmt){
        str += "{" + stmt->toString() + "},\n";
    }
    str[str.length()-1]=']';
    return str;
}

//BlockStmt Part
TokenNode* BlockStmtNode::Left(){return this->_Left;}
vector<StmtNode*> BlockStmtNode::Stmts(){return this->_Stmt;}
TokenNode* BlockStmtNode::Right(){return this->_Right;}
string BlockStmtNode::toString(){
    std::string str = "blockStmt:{stmts:[";
    for(auto stmt : _Stmt){
        str += "{" + stmt->toString() + "},";
    }
    if (_Stmt.size())
        str[str.size() - 1] = ']';
    else
        str += ']';
    return str;
}
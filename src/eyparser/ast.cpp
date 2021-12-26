#include "ast.h"
using namespace eyast;
//BasicNode Part

BasicNode::BasicNode() {}
BasicNode::BasicNode(Token _token) : _token(_token) {}

Token BasicNode::token() {
    return _token;
}

std::string BasicNode::toString() {
    return ::toString(this->_token.symbol) + ":\"" + this->_token.content + "\"";
}

// AddOperatorNode Part

AddOperatorNode::AddOperatorNode() : _AddOperator(0) {}
AddOperatorNode::AddOperatorNode(BasicNode* _AddOperator) : _AddOperator(_AddOperator) {}

AddOperatorNode::~AddOperatorNode() {
    delete _AddOperator;
}

BasicNode* AddOperatorNode::AddOperator() {
    return _AddOperator;
}

std::string AddOperatorNode::toString() {
    return (std::string)"AddOp: {" + _AddOperator->toString() + "}";
}

// MulOperator Part

MulOperatorNode::MulOperatorNode() : _MulOperator(0) {}
MulOperatorNode::MulOperatorNode(BasicNode* _MulOperator) : _MulOperator(_MulOperator) {}
MulOperatorNode::~MulOperatorNode() {
    delete _MulOperator;
}

BasicNode* MulOperatorNode::MulOperator() {
    return _MulOperator;
}

std::string MulOperatorNode::toString() {
    return (std::string)"MulOp: {" + _MulOperator->toString() + "}";
}

//ArrayElt Part
BasicNode* ArrayNode::Iden(){return _Iden;}
BasicNode* ArrayNode::Left(){return _Left;}
AddExprNode* ArrayNode::Index(){return _Index;}
BasicNode* ArrayNode::Right(){return _Right;}

string ArrayNode::toString(){
    return "ArrayNode: {" + _Iden->toString() + ", " + _Left->toString() + ", " + _Index->toString() + ", " + _Right->toString() + "}";
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

BasicNode* PrimExprNode::Number() {
    return _Number;
}

BasicNode* PrimExprNode::Iden() {
    return _Iden;
}

BasicNode* PrimExprNode::String() {
    return _String;
}

ArrayNode* PrimExprNode::ArrayElt() {
    return _ArrayElt;
}

BasicNode* PrimExprNode::ConstBool() {
    return _ConstBool;
}

BasicNode* PrimExprNode::LeftParen() {
    return _LeftParen;
}

AddExprNode* PrimExprNode::AddExpr() {
    return _AddExpr;
}

BasicNode* PrimExprNode::RightParen() {
    return _RightParen;
}

std::string PrimExprNode::toString() {
    if (_Number != nullptr) {
        return (std::string)"PrimExpr(Number):{" + _Number->toString() + "}";
    }
    else if (_Iden != nullptr) {
        return (std::string)"PrimExpr(Iden):{" + _Iden->toString() + "}";
    }
    else if (_String != nullptr) {
        return (std::string)"PrimExpr(String):{" + _String->toString() + "}";
    }
    else if (_ConstBool != nullptr) {
        return (std::string)"PrimExpr(ConstBool):{" + _ConstBool->toString() + "}";
    }
    else if (_ArrayElt != nullptr) {
        return (std::string)"PrimExpr(ArrayElt):{" + _ArrayElt->toString() + "}";
    }
    else {
        return (std::string)"PrimExpr(AddExpr):{" + _AddExpr->toString() + "}";
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
BasicNode* CmpOperatorNode::Op(){return _Op;}
string CmpOperatorNode::toString(){return "CmpOp: {" + _Op->toString() + "}";}

AddExprNode* CmpExprNode::Expr(){return _Expr;}
CmpOperatorNode* CmpExprNode::Op(){return _Op;}
AddExprNode* CmpExprNode::Sub(){return _Tar;}
string CmpExprNode::toString(){
    if(_Op != nullptr)
        return "CmpExpr: {" + _Expr->toString() + ", " + _Op->toString() + ", " + _Tar->toString() + "}";
    return "CmpExpr: {" + _Expr->toString() + "}";    
}

BasicNode* BoolOperatorNode::Op(){return _Op;}
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
BasicNode* NotBoolExprNode::Op(){return _Op;}
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

BasicNode* OutStmtNode::OutMark(){return this->_OutMark;}
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

BasicNode* VorcStmtNode::VarMark(){return _VarMark;}
BasicNode* VorcStmtNode::ConstMark(){return _ConstMark;}
BasicNode* VorcStmtNode::LT(){return this->_LT;}
BasicNode* VorcStmtNode::Type(){return this->_Type;}
BasicNode* VorcStmtNode::GT(){return this->_GT;}
BasicNode* VorcStmtNode::IdenName(){return _IdenName;}
BasicNode* VorcStmtNode::Equ(){return _Equ;}
ExprNode* VorcStmtNode::Expr(){return _Expr;}
std::string VorcStmtNode::toString(){
    if(this->_Expr == nullptr) {
        if (this->_VarMark != nullptr)
            return "VorcStmt(var):{" + this->_VarMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "}";
        else
            return "VorcStmt(const):{" + this->_ConstMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "}";
    }
    else {
        if (this->_VarMark != nullptr)
            return "VorcStmt(var):{" + this->_VarMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
        else
            return "VorcStmt(const):{" + this->_ConstMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
    }
}
// ArrayDefineStmt Part
BasicNode* ArrayDefineStmtNode::ArrayDefineMark(){return _ArrayDefineMark;}
BasicNode *ArrayDefineStmtNode::LeftB() { return _LeftB; }
BasicNode *ArrayDefineStmtNode::Left() { return _Left; }
BasicNode *ArrayDefineStmtNode::Number() { return _Number; }
BasicNode *ArrayDefineStmtNode::Right() { return _Right; }
BasicNode *ArrayDefineStmtNode::Type() { return _Type; }
BasicNode *ArrayDefineStmtNode::RightB() { return _RightB; }
BasicNode *ArrayDefineStmtNode::Iden() { return _Iden; }
BasicNode *ArrayDefineStmtNode::Eq() { return _Eq; }
BasicNode *ArrayDefineStmtNode::GroupBegin() { return _GroupBegin; }
vector<ExprNode*> ArrayDefineStmtNode::Elts() { return _Elts; }
vector<BasicNode*> ArrayDefineStmtNode::Dots() { return _Dots; }
BasicNode *ArrayDefineStmtNode::GroupEnd() { return _GroupEnd; }
BasicNode *ArrayDefineStmtNode::EndMark() { return _EndMark; }
string ArrayDefineStmtNode::toString() {
    string content = "ArrayDefineStmt: {";
    string sub = "defineList:[";
    for (int i = 0 ; i < _Dots.size() ; i++) {
        auto op = _Dots[i]->toString();
        auto op_elt = _Elts[i + 1]->toString();
        sub += "," + op + "," + op_elt;
    }
    content += _ArrayDefineMark->toString() + ", " + 
                _LeftB->toString() + ", " + _Left->toString() + ", " + _Number->toString() + ", " + _Right->toString() + ", " + _Type->toString() + ", " + _RightB->toString() + ", " + 
                _Iden->toString() + ", " + _Eq->toString() + ", " + _GroupBegin->toString() + ", " + sub + ", " + _GroupEnd->toString() + ", " + _EndMark->toString();
    return content;
}

//ArrayAssign Part
ArrayNode* AssignArrayElementStmtNode::ArrayElt(){return _ArrayElt;}
BasicNode* AssignArrayElementStmtNode::Eq(){return _Eq;}
ExprNode* AssignArrayElementStmtNode::Expr(){return _Expr;}
BasicNode* AssignArrayElementStmtNode::EndMark(){return _EndMark;}
string AssignArrayElementStmtNode::toString(){
    return "ArrayElementAssignStmt: {" + _ArrayElt->toString() + ", " + _Eq->toString() + ", " + _Expr->toString() + ", " + _EndMark->toString() + "}";
}

//AssignStmt Part

AssignStmtNode::AssignStmtNode(){}
AssignStmtNode::~AssignStmtNode(){
    delete _Iden;
    delete _Equ;
    delete _Expr;
    delete _EndMark;
}

BasicNode* AssignStmtNode::Iden(){return _Iden;}
BasicNode* AssignStmtNode::Equ(){return _Equ;}
ExprNode* AssignStmtNode::Expr(){return _Expr;}
BasicNode* AssignStmtNode::EndMark(){return _EndMark;}

std::string AssignStmtNode::toString(){
    return "AssigntStmt:{" + this->_Iden->toString() + "," + this->_Equ->toString() + "," + this->_Expr->toString() + "}";
}

//Delete StmtPart
DeleteStmtNode::~DeleteStmtNode(){
    delete this->_DeleteMark;
    delete this->_EndMark;
    delete this->_Iden;
}

BasicNode* DeleteStmtNode::DeleteMark(){return this->_DeleteMark;}
BasicNode* DeleteStmtNode::Iden(){return this->_Iden;}
BasicNode* DeleteStmtNode::EndMark(){return this->_EndMark;}
string DeleteStmtNode::toString(){
    return "DeleteStmt:{" + this->DeleteMark()->toString() + "," + this->Iden()->toString()  + "," + this->EndMark()->toString() +  "}";
}

//InputStmt Part
BasicNode* InputStmtNode::InputMark(){return _InputMark;}
AddExprNode* InputStmtNode::Content(){return _Content;}
BasicNode* InputStmtNode::Give(){return _Give;}
BasicNode* InputStmtNode::Iden(){return _Iden;}
BasicNode* InputStmtNode::EndMark(){return _EndMark;}

string InputStmtNode::toString(){
    return "InputStmt: {" + _InputMark->toString() + ", " + _Content->toString() + ", " + _Give->toString() + ", " + _Iden->toString() + ", " + _EndMark->toString() + "}";
}

//RepeatStmt Part
BasicNode* WhileStmtNode::WhileMark(){return _WhileMark;}
ExprNode* WhileStmtNode::Cond(){return _Cond;}
BasicNode* WhileStmtNode::To(){return _To;}
BlockStmtNode* WhileStmtNode::Block(){return _Block;}
string WhileStmtNode::toString(){
    return "WhileStmt: {" + _WhileMark->toString() + ", Cond[" + _Cond->toString() + "], " + _To->toString() + ", " + _Block->toString() + "}";
}

//IfStmt Part
BasicNode* IfStmtNode::IfMark(){return _IfMark;}
ExprNode* IfStmtNode::Cond(){return _Cond;}
BasicNode* IfStmtNode::To(){return _To;}
BlockStmtNode* IfStmtNode::Block(){return _Block;}
string IfStmtNode::toString(){return "IfStmt: {" + _IfMark->toString() + ", Cond[" + _Cond->toString() + "], " + _To->toString() + ", " + _Block->toString() + "}";}

//Stmt Part

OutStmtNode* StmtNode::OutStmt() {return _OutStmt;}
VorcStmtNode* StmtNode::VorcStmt() {return _VorcStmt;}
AssignStmtNode* StmtNode::AssignStmt() {return _AssignStmt;}
BlockStmtNode* StmtNode::BlockStmt(){return _BlockStmt;}
DeleteStmtNode* StmtNode::DeleteStmt(){return _DeleteStmt;}
InputStmtNode* StmtNode::InputStmt(){return _InputStmt;}
ArrayDefineStmtNode* StmtNode::ArrayDefineStmt(){return _ArrayDefineStmt;}
AssignArrayElementStmtNode* StmtNode::AssignArrayStmt(){return _AssignArrayStmt;}
WhileStmtNode* StmtNode::WhileStmt(){return _WhileStmt;}
IfStmtNode* StmtNode::IfStmt(){return _IfStmt;}

std::string StmtNode::toString() {
    std::string str = "stmt:";
    if (_OutStmt) {
        str += "{" + _OutStmt->toString() + "}";
    }
    else if(_VorcStmt) {
        str += "{" + _VorcStmt->toString() + "}";
    }
    else if(_ArrayDefineStmt) {
        str += "{" + _ArrayDefineStmt->toString() + "}";
    }
    else if(_AssignArrayStmt) {
        str += "{" + _AssignArrayStmt->toString() + "}";
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
    else if(_WhileStmt) {
        str += "{" + _WhileStmt->toString() + "}";
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
BasicNode* BlockStmtNode::Left(){return this->_Left;}
vector<StmtNode*> BlockStmtNode::Stmts(){return this->_Stmt;}
BasicNode* BlockStmtNode::Right(){return this->_Right;}
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
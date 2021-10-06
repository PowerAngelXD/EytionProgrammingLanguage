#include "parser.h"

using namespace eyparser;

// TokenNode Part

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

//OutStmt Part

TokenNode* OutStmtNode::OutMark(){return this->_OutMark;}
AddExprNode* OutStmtNode::AddExpr(){return this->_AddExpr;}
TokenNode* OutStmtNode::String(){return this->_String;}

OutStmtNode::OutStmtNode(){}
OutStmtNode::~OutStmtNode(){
    delete _OutMark;
    delete _AddExpr;
    delete _StmtEndMark;
}

std::string OutStmtNode::toString(){
    if (this->_String != nullptr)
        return "OutStmt:{" + this->_OutMark->toString() + "," + this->_String->toString() + "," + this->_StmtEndMark->toString() + "}";
    else
        return "OutStmt:{" + this->_OutMark->toString() + "," + this->_AddExpr->toString() + "," + this->_StmtEndMark->toString() + "}";
}

//LetStmt Part

VorcStmtNode::VorcStmtNode(){}
VorcStmtNode::~VorcStmtNode(){
    delete _VarMark;
    delete _IdenName;
    delete _Equ;
    delete _ValueExpr;
    delete _ValueString;
    delete _EndMark;
}

TokenNode* VorcStmtNode::VarMark(){return _VarMark;}
TokenNode* VorcStmtNode::ConstMark(){return _ConstMark;}
TokenNode* VorcStmtNode::LT(){return this->_LT;}
TokenNode* VorcStmtNode::Type(){return this->_Type;}
TokenNode* VorcStmtNode::GT(){return this->_GT;}
TokenNode* VorcStmtNode::IdenName(){return _IdenName;}
TokenNode* VorcStmtNode::Equ(){return _Equ;}
AddExprNode* VorcStmtNode::ValueExpr(){return _ValueExpr;}
TokenNode* VorcStmtNode::ValueString(){return _ValueString;}
std::string VorcStmtNode::toString(){
    if (this->_ValueString != nullptr){
        if (this->_VarMark != nullptr)
            return "VorcStmt(var):{" + this->_VarMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_ValueString->toString() + "}";
        else
            return "VorcStmt(const):{" + this->_ConstMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_ValueString->toString() + "}";
    }
    else{
        if (this->_VarMark != nullptr)
            return "VorcStmt(var):{" + this->_VarMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_ValueExpr->toString() + "}";
        else
            return "VorcStmt(const):{" + this->_ConstMark->toString() + ",Type:" + this->_Type->toString() + "," + this->_IdenName->toString() + "," + this->_Equ->toString() + "," + this->_ValueExpr->toString() + "}";
    }
}

//AssignStmt Part

AssignStmtNode::AssignStmtNode(){}
AssignStmtNode::~AssignStmtNode(){
    delete _Iden;
    delete _Equ;
    delete _ValueString;
    delete _ValueExpr;
    delete _EndMark;
}

TokenNode* AssignStmtNode::Iden(){return _Iden;}
TokenNode* AssignStmtNode::Equ(){return _Equ;}
TokenNode* AssignStmtNode::ValueString(){return _ValueString;}
AddExprNode* AssignStmtNode::ValueExpr(){return _ValueExpr;}
TokenNode* AssignStmtNode::EndMark(){return _EndMark;}

std::string AssignStmtNode::toString(){
    if (this->_ValueString != nullptr)
        return "AssigntStmt:{" + this->_Iden->toString() + "," + this->_Equ->toString() + "," + this->_ValueString->toString() + "}";
    else
        return "AssigntStmt:{" + this->_Iden->toString() + "," + this->_Equ->toString() + "," + this->_ValueExpr->toString() + "}";
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
//Stmt Part

OutStmtNode* StmtNode::OutStmt() {return _OutStmt;}
VorcStmtNode* StmtNode::VorcStmt() {return _VorcStmt;}
AssignStmtNode* StmtNode::AssignStmt() {return _AssignStmt;}
BlockStmtNode* StmtNode::BlockStmt(){return _BlockStmt;}
DeleteStmtNode* StmtNode::DeleteStmt(){return _DeleteStmt;}

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

//end


//???? PARSER ????//

eylex::Token Parser::peek(int move){
    if (move == 0)
        return input[cur_pos];
    else
        return input[cur_pos+move];
}

TokenNode* Parser::tokenIncopy() {
    if (! IsToken())
        return new TokenNode({"eof", Symbol::Eof});
    return new TokenNode(copy[copy_pos ++]);
}

eylex::Token Parser::peekIncopy(int move) {
    if (move == 0)
        return copy[copy_pos];
    else 
        return copy[copy_pos+move];
}

Parser::Parser(std::vector<Token> input) : input(input), copy(input), cur_pos(0), copy_pos(0) {}

bool Parser::IsToken() {
    return cur_pos < input.size();
}

TokenNode* Parser::token() {
    if (! IsToken())
        return new TokenNode({"eof", Symbol::Eof});
    return new TokenNode(input[cur_pos ++]);
}

TokenNode* Parser::peeknode(){
    if (! IsToken())
        return new TokenNode({"eof", Symbol::Eof});
    return new TokenNode(input[cur_pos]);
}

bool Parser::IsAddOperator() {
    if (! IsToken())
        return false;
    return input[cur_pos].symbol == Symbol::Add || input[cur_pos].symbol == Symbol::Sub;
}

AddOperatorNode* Parser::AddOperator() {
    if (! IsAddOperator())
        throw "it is not an operator!";
    TokenNode* op = token();
    if (op->_token.symbol == Symbol::Add || op->_token.symbol == Symbol::Sub) {
        return new AddOperatorNode(op);
    }
    delete op;
    throw "it is not an operator!"; //不是operator则抛出异常
}

bool Parser::IsMulOperator() {
    if (! IsToken())
        return false;
    return input[cur_pos].symbol == Symbol::Mul || input[cur_pos].symbol == Symbol::Div || input[cur_pos].symbol == Symbol::Mod;
}

MulOperatorNode* Parser::MulOperator() {
    if (!IsMulOperator())
        throw "it is not an operator!";
    TokenNode* op = token();
    if (op->_token.symbol == Symbol::Mul || op->_token.symbol == Symbol::Div || op->_token.symbol == Symbol::Mod) {
        return new MulOperatorNode(op);
    }
    delete op;
    throw "it is not an operator!";
}

bool Parser::IsPrimExpr() {
    if (! IsToken())
        return false;
    return input[cur_pos].symbol == Symbol::Number || input[cur_pos].symbol == Symbol::LeftParen || input[cur_pos].symbol == Symbol::Identifier;
}

PrimExprNode* Parser::PrimExpr() {
    if (! IsPrimExpr())
        throw "it is not a primexpr!";
    PrimExprNode* node = new PrimExprNode;
    if (input[cur_pos].symbol == Symbol::Number) {
        node->_Number = token();
        return node;
    }
    else if (input[cur_pos].symbol == Symbol::Identifier) {
        node->_Iden = token();
        return node;
    }
    else if (input[cur_pos].symbol == Symbol::LeftParen) {
        node->_LeftParen = token();
        node->_AddExpr = AddExpr();
        if (input[cur_pos].symbol != Symbol::RightParen) {
            delete node;
            throw "expect ')'";
        }
        node->_RightParen = token();
        return node;
    }
    delete node;
    throw "error!";
}

bool Parser::IsMulExpr() {
    return IsPrimExpr();
}

MulExprNode* Parser::MulExpr() {
    if(!IsMulExpr())
        throw "it is not a mulexpr!";
    MulExprNode* node = new MulExprNode;
    node->_PrimExpr.push_back(PrimExpr());
    while(true) {
        try {
            if (! IsMulOperator())
                break;
            node->_MulOperator.push_back(MulOperator());
            node->_PrimExpr.push_back(PrimExpr());
        }
        catch (const char* e) {
            std::cerr << e <<std::endl;
            delete node;
        }
    }
    return node;
}

bool Parser::IsAddExpr() {
    return IsMulExpr();
}

AddExprNode* Parser::AddExpr() {
    if (!IsAddExpr())
        throw "it is not an addexpr!";
    AddExprNode* node = new AddExprNode;
    node->_MulExpr.push_back(MulExpr());
    while (true) {
        try {
            if (! IsAddOperator())
                break;
            node->_AddOperator.push_back(AddOperator());
            node->_MulExpr.push_back(MulExpr());
        }
        catch (const char* e) {
            std::cerr << e << std::endl;
            delete node;
        }
    }
    return node;
}

bool Parser::IsOutStmt() {
    if (!IsToken()) return false;
    if(peek().content == "out"){
        return true;
    }
    return false;
}

OutStmtNode* Parser::OutStmt() {
    if (!IsOutStmt())
        throw "it is not a out stmt!";
    OutStmtNode* node = new OutStmtNode;
    string line = to_string(peek().line);
    string column = to_string(peek().column);
    if(peek().content == "out"){
        node->_OutMark = token();
    }

    if(IsAddExpr()) node->_AddExpr = AddExpr();
    else if(peek().symbol == Symbol::String) node->_String = token();
    else
        throw (string)("is not an available value!\nline: " + line + ", col: " + column);
    if(peek().content == ";"){
        node->_StmtEndMark = token();
    }
    else throw (string)("expect ';'\nline: " + line + ", col: " + column);
    return node;
}

bool Parser::IsVorcStmt(){
    if (!IsToken()) return false;
    if(peek().content == "var" || peek().content == "const"){
        return true;
    }
    return false;
}

VorcStmtNode* Parser::VorcStmt(){
    if (!IsVorcStmt())
        throw "it is not a out stmt!";
    VorcStmtNode* node = new VorcStmtNode;
    string line = to_string(peek().line);
    string column = to_string(peek().column);
    if(peek().content == "var")
        node->_VarMark = token();
    else
        node->_ConstMark = token();
    
    if(peek().symbol == eylex::Symbol::LT) {node->_LT = token();}
    else {throw (string)("the defined type must be complete\nline: " + line + ", col: " + column);}
    if(peek().symbol == eylex::Symbol::KeyWord) {node->_Type = token();}
    else {throw (string)("the defined type must be complete\nline: " + line + ", col: " + column);}
    if(peek().symbol == eylex::Symbol::GT) {node->_GT = token();}
    else {throw (string)("the defined type must be complete\nline: " + line + ", col: " + column);}

    if(peek().symbol == eylex::Symbol::Identifier) {node->_IdenName = token();}
    else {throw (string)("please put the (Vorciable/constant) behind the 'var' or 'const'(sometimes behind the 'const')\nline: " + line + ", col: " + column);}

    if(peek().content == "=") {node->_Equ = token();}

    if(IsAddExpr()) {node->_ValueExpr = AddExpr();}
    else if(peek().symbol == eylex::Symbol::String) {node->_ValueString = token();}
    else if((IsAddExpr() || (peek().symbol == eylex::Symbol::EQ))) {throw (string)("expect '=' behind the identifier\nline: " + line + ", col: " + column);}

    if(peek().content == ";") {node->_EndMark = token();}
    else {throw (string)("expect ';'\nline: " + line + ", col: " + column);}

    return node;
}

bool Parser::IsAssignStmt() {
    if (!IsToken()) return false;
    if(peek().symbol == eylex::Symbol::Identifier && peek(1).symbol == eylex::Symbol::Assign){
        return true;
    }
    return false;
}

AssignStmtNode* Parser::AssignStmt() {
    if (!IsAssignStmt())
        throw "it is not a assign stmt!"; 
    AssignStmtNode* node = new AssignStmtNode;
    string line = to_string(peek().line);
    string column = to_string(peek().column);

    node->_Iden = token();
    node->_Equ = token();
    
    if(IsAddExpr()) {node->_ValueExpr = AddExpr();}
    else if(peek().symbol == eylex::Symbol::String) {node->_ValueString = token();}
    else {throw (string)("the value (or expression of the value) must be taken with the assignment operation\nline: " + line + ", col: " + column);}

    if(peek().content == ";") {node->_EndMark = token();}
    else {throw (string)("expect ';'\nline: " + line + ", col: " + column);}

    return node;
}

bool Parser::IsDelStmt(){
    if (!IsToken())
        throw "it is not a delete stmt!";
    if(peek().content == "delete")
        return true;
    return false;
}

DeleteStmtNode* Parser::DelStmt(){
    if(!IsDelStmt())
        throw "it is not a delete stmt!";
    DeleteStmtNode* node = new DeleteStmtNode;
    string line = to_string(peek().line);
    string column = to_string(peek().column);

    node->_DeleteMark = token();//not this!
    if(peek().symbol == eylex::Symbol::Identifier)
        node->_Iden = token();
    else
        throw (string)("expect identifier behind the 'delete'\nline: " + line + ", col: " + column);
    if(peek().content == ";")
        node->_EndMark = token();
    else
        throw (string)("expect ';'\nline: " + line + ", col: " + column);
    return node;
}

bool Parser::IsBlockStmt(){
    if (!IsToken()) return false;
    return peek().content == "{";
}

BlockStmtNode* Parser::BlockStmt(){
    if (!IsBlockStmt())
        throw "it is not a block stmt!";
    BlockStmtNode* node = new BlockStmtNode;
    string line = to_string(peek().line);
    string column = to_string(peek().column);

    node->_Left = token();
    vector<StmtNode*> stmts;
    while(peek().content != "}"){
        if(!IsStmt())
            throw (string)("an unknown sentence\nline: " + line + ", col: " + column);
        stmts.push_back(Stmt());
    }
    node->_Stmt = stmts;
    node ->_Right = token();
    return node;
}

bool Parser::IsStmt() {
    if (!IsToken()) return false;
    return IsOutStmt() || IsVorcStmt() || IsAssignStmt() || IsBlockStmt() || IsDelStmt();
}

StmtNode* Parser::Stmt() {
    if (!IsStmt())
        throw "it is not any stmt!";
    StmtNode* stmt = new StmtNode;
    if (IsOutStmt()) {
        stmt->_OutStmt = OutStmt();
        return stmt;
    }
    else if (IsVorcStmt()) {
        stmt->_VorcStmt = VorcStmt();
        return stmt;
    }
    else if (IsAssignStmt()) {
        stmt->_AssignStmt = AssignStmt();
        return stmt;
    }
    else if (IsBlockStmt()) {
        stmt->_BlockStmt = BlockStmt();
        return stmt;
    }
    else if (IsDelStmt()) {
        stmt->_DeleteStmt = DelStmt();
        return stmt;
    }
    throw "unknown type of stmt!";
    return NULL;
}

bool Parser::IsStat() {
    if (!IsToken()) return false;
    return IsStmt();
}

StatNode* Parser::Stat(){
    StatNode* stat = new StatNode;
    while(true){
        if (!IsStmt()) break;
        stat->_Stmt.push_back(Stmt());
    }
    return stat;
}

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

//Stmt Part

OutStmtNode* StmtNode::OutStmt() {return _OutStmt;}
VorcStmtNode* StmtNode::VorcStmt() {return _VorcStmt;}
AssignStmtNode* StmtNode::AssignStmt() {return _AssignStmt;}
BlockStmtNode* StmtNode::BlockStmt(){return _BlockStmt;}
DeleteStmtNode* StmtNode::DeleteStmt(){return _DeleteStmt;}
InputStmtNode* StmtNode::InputStmt(){return _InputStmt;}

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
    int line = peek().line;
    int col = peek().column;
    if (! IsAddOperator())
        throw EyparseError("SymbolError", "expect '+' or '-'", line, col);
    TokenNode* op = token();
    if (op->_token.symbol == Symbol::Add || op->_token.symbol == Symbol::Sub) {
        return new AddOperatorNode(op);
    }
    delete op;
    throw EyparseError("SymbolError", "expect '+' or '-'", line, col); //不是operator则抛出异常
}

bool Parser::IsMulOperator() {
    int line = peek().line;
    int col = peek().column;
    if (! IsToken())
        return false;
    return input[cur_pos].symbol == Symbol::Mul || input[cur_pos].symbol == Symbol::Div || input[cur_pos].symbol == Symbol::Mod;
}

MulOperatorNode* Parser::MulOperator() {
    int line = peek().line;
    int col = peek().column;
    if (!IsMulOperator())
        throw EyparseError("SymbolError", "expect '*', '/' or '%'", line, col);
    TokenNode* op = token();
    if (op->_token.symbol == Symbol::Mul || op->_token.symbol == Symbol::Div || op->_token.symbol == Symbol::Mod) {
        return new MulOperatorNode(op);
    }
    delete op;
    throw EyparseError("SymbolError", "expect '*', '/' or '%'", line, col);
}

bool Parser::IsPrimExpr() {
    if (! IsToken())
        return false;
    return input[cur_pos].symbol == Symbol::Number || input[cur_pos].symbol == Symbol::LeftParen || input[cur_pos].symbol == Symbol::Identifier || input[cur_pos].symbol == Symbol::String;
}

PrimExprNode* Parser::PrimExpr() {
    int line = peek().line;
    int col = peek().column;
    if (! IsPrimExpr())
        throw EyparseError("ExprError", "not a primexpr", line ,col);
    PrimExprNode* node = new PrimExprNode;
    if (input[cur_pos].symbol == Symbol::Number) {
        node->_Number = token();
        return node;
    }
    else if (input[cur_pos].symbol == Symbol::Identifier) {
        node->_Iden = token();
        return node;
    }
    else if (input[cur_pos].symbol == Symbol::String) {
        node->_String = token();
        return node;
    }
    else if (input[cur_pos].symbol == Symbol::LeftParen) {
        node->_LeftParen = token();
        node->_AddExpr = AddExpr();
        if (input[cur_pos].symbol != Symbol::RightParen) {
            delete node;
            throw EyparseError("SymbolError", "expect ')'", line ,col);;
        }
        node->_RightParen = token();
        return node;
    }
    delete node;
    throw EyparseError("ExprError", "not a primexpr", line ,col);
}

bool Parser::IsMulExpr() {
    return IsPrimExpr();
}

MulExprNode* Parser::MulExpr() {
    int line = peek().line;
    int col = peek().column;
    if(!IsMulExpr())
        throw EyparseError("ExprError", "not a mulexpr", line ,col);
    MulExprNode* node = new MulExprNode;
    node->_PrimExpr.push_back(PrimExpr());
    while(true) {
        try {
            if (! IsMulOperator())
                break;
            node->_MulOperator.push_back(MulOperator());
            node->_PrimExpr.push_back(PrimExpr());
        }
        catch (EyparseError e){
            cerr<<e.what()<<endl;
            delete node;
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
    int line = peek().line;
    int col = peek().column;
    if (!IsAddExpr())
        throw EyparseError("ExprError", "not a addexpr", line ,col);
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

bool Parser::IsCmpOp(){
    if(!IsToken())return false;
    if(input[cur_pos].symbol == eylex::Symbol::EQ || input[cur_pos].symbol == eylex::Symbol::NEQ ||
       input[cur_pos].symbol == eylex::Symbol::GT || input[cur_pos].symbol == eylex::Symbol::LT ||
       input[cur_pos].symbol == eylex::Symbol::GE || input[cur_pos].symbol == eylex::Symbol::LE) {
           return true;
       }
    else return false;
}

CmpOperatorNode* Parser::CmpOp(){
    int line = peek().line;
    int col = peek().column;
    if(!IsCmpOp())
        throw EyparseError("SymbolError", "Not a right Compare Symbol!", line, col);
    CmpOperatorNode* node = new CmpOperatorNode;
    node->_Op = token();
    return node;
}

bool Parser::IsBoolOp(){
    if(!IsToken())return false;
    if(input[cur_pos].symbol == eylex::Symbol::LogicOr || input[cur_pos].symbol == eylex::Symbol::LogicAnd) {
           return true;
       }
    else return false;
}

BoolOperatorNode* Parser::BoolOp(){
    int line = peek().line;
    int col = peek().column;
    if(!IsBoolOp())
        throw EyparseError("SymbolError", "Not a right Bool Symbol!", line, col);
    BoolOperatorNode* node = new BoolOperatorNode;
    node->_Op = token();
    return node;
}

bool Parser::IsCmpExpr(){
    if(!IsToken()) return false;
    int temp_index = cur_pos;
    if(IsAddExpr()) {
        AddExprNode* temp = AddExpr();
        if(IsCmpOp()) {cur_pos = temp_index; return true;}
        else {cur_pos = temp_index; return false;}
    }
    else {cur_pos = temp_index; return false;}
}

CmpExprNode* Parser::CmpExpr(){
    int line = peek().line;
    int col = peek().column;
    if (! IsCmpExpr())
        throw EyparseError("ExprError", "not a Cmpexpr", line ,col);
    CmpExprNode* node = new CmpExprNode;
    node->_Expr = AddExpr();
    if(IsCmpOp())
        node->_Op = CmpOp();
    
    if(IsAddExpr())
        node->_Tar = AddExpr();

    if(node->_Op != nullptr && node->_Tar == nullptr)
        throw EyparseError("SyntaxError", "expect identifier behind the symbol: '" + node->_Op->_Op->token().content + "'", line, col);
    return node;
}

bool Parser::IsBoolExpr(){
    return IsCmpExpr();
}

BoolExprNode* Parser::BoolExpr(){
    int line = peek().line;
    int col = peek().column;
    if(!IsBoolExpr())
        throw EyparseError("ExprError", "Not a right Boolexpr!", line, col);
    BoolExprNode* node = new BoolExprNode;
    node->_Root = CmpExpr();//problem here
    while(true) {
        try {
            if (! IsBoolOp())
                break;
            node->_Op.push_back(BoolOp());
            node->_Sub.push_back(CmpExpr());
        }
        catch (EyparseError e){
            cerr<<e.what()<<endl;
            delete node;
        }
        catch (const char* e) {
            std::cerr << e <<std::endl;
            delete node;
        }
    }
    return node;
}

bool Parser::IsNotBoolExpr(){
    if(!IsToken()) return false;
    return input[cur_pos].symbol == eylex::Symbol::Not;
}

NotBoolExprNode* Parser::NotBoolExpr(){
    int line = peek().line;
    int col = peek().column;
    if(!IsNotBoolExpr())
        throw EyparseError("ExprError", "Not a right Boolexpr!", line, col);
    NotBoolExprNode* node = new NotBoolExprNode;
    node->_Op = token();
    node->_Boolexpr = BoolExpr();
    return node;
}

bool Parser::IsExpr(){
    if(!IsToken()) return false;
    return IsAddExpr() || input[cur_pos].symbol == eylex::Symbol::String || IsBoolExpr() || IsNotBoolExpr();
}

ExprNode* Parser::Expr(){
    int line = peek().line;
    int col = peek().column;
    ExprNode* node = new ExprNode;
    if(!IsExpr()) throw EyparseError("ExprError", "It is not any expr!", line ,col);
    if(IsBoolExpr()) {node->_BoolExpr = BoolExpr();}
    else if(IsAddExpr()){
        node->_AddExpr = AddExpr();
    }
    else if(IsNotBoolExpr()){
        node->_NotBoolExpr = NotBoolExpr();
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
    int line = peek().line;
    int col = peek().column;
    if (!IsOutStmt())
        throw EyparseError("StmtError", "not out-stmt", line ,col);
    OutStmtNode* node = new OutStmtNode;
    node->_OutMark = token();
    if(IsBoolExpr()) {node->_Expr = Expr();}
    else if(IsAddExpr()) node->_Expr = Expr();
    else if(peek().symbol == Symbol::String) node->_Expr = Expr();
    else
        throw EyparseError("TypeError", "not avaliable", line ,col);
    if(peek().content == ";"){
        node->_StmtEndMark = token();
    }
    else throw EyparseError("SymbolError", "expect ';'", line ,col);
    return node;
}

bool Parser::IsInputStmt(){
    if (!IsToken()) return false;
    if(peek().content == "input"){
        return true;
    }
    return false;
}

InputStmtNode* Parser::InputStmt(){
    int line = peek().line;
    int col = peek().column;
    if (!IsInputStmt())
        throw EyparseError("StmtError", "not input-stmt", line ,col);
    InputStmtNode* node = new InputStmtNode;
    node->_InputMark = token();
    if(IsAddExpr()) node->_Content = AddExpr();
    else throw EyparseError("TypeError", "must give a value that type is 'String'",line ,col);

    if(peek().content == "=>") node->_Give = token();
    else throw EyparseError("SyntaxError", "expect '=>'", line, col);

    if(peek().symbol == eylex::Symbol::Identifier) node->_Iden = token();
    else throw EyparseError("SyntaxError", "must give an identifier", line, col);

    if(peek().content == ";") node->_EndMark = token();
    else throw EyparseError("SymbolError", "expect ';'", line ,col);
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
    int line = peek().line;
    int col = peek().column;
    if (!IsVorcStmt())
        throw EyparseError("StmtError", "not vorc-stmt", line ,col);
    VorcStmtNode* node = new VorcStmtNode;
    if(peek().content == "var")
        node->_VarMark = token();
    else
        node->_ConstMark = token();
    
    if(peek().symbol == eylex::Symbol::LT) {node->_LT = token();}
    else {throw EyparseError("SyntaxError", "the defined type must be complete", line ,col);}
    if(peek().symbol == eylex::Symbol::KeyWord) {node->_Type = token();}
    else {throw EyparseError("SyntaxError", "the defined type must be complete", line ,col);}
    if(peek().symbol == eylex::Symbol::GT) {node->_GT = token();}
    else {throw EyparseError("SyntaxError", "the defined type must be complete", line ,col);}

    if(peek().symbol == eylex::Symbol::Identifier) {node->_IdenName = token();}
    else {throw EyparseError("SyntaxError", "please put the (Vorciable/constant) behind the 'var' or 'const'(sometimes behind the 'const')", line ,col);}

    if(peek().content == "=") {node->_Equ = token();}

    if(IsBoolExpr()) {node->_Expr = Expr();}
    else if(IsAddExpr()) {node->_Expr = Expr();}
    else if((IsAddExpr() || (peek().symbol == eylex::Symbol::EQ))) {throw EyparseError("SymbolError", "expect '=' behind the identifier", line, col);}

    if(peek().content == ";") {node->_EndMark = token();}
    //else {throw EyparseError("SymbolError", "expect ';'", line ,col);}

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
    int line = peek().line;
    int col = peek().column;
    if (!IsAssignStmt())
        throw EyparseError("StmtError", "it is not a assign stmt!", line, col); 
    AssignStmtNode* node = new AssignStmtNode;

    node->_Iden = token();
    node->_Equ = token();
    
    if(IsAddExpr()) {node->_Expr = Expr();}
    else if(IsBoolExpr()) {node->_Expr = Expr();}
    else if(IsNotBoolExpr()) {node->_Expr = Expr();}
    else {throw EyparseError("SyntaxError", "the value (or expression of the value) must be taken with the assignment operation", line, col);}

    if(peek().content == ";") {node->_EndMark = token();}
    else {throw EyparseError("SymbolError", "expect ';'", line ,col);}

    return node;
}

bool Parser::IsDelStmt(){
    if (!IsToken())
        return false;
    if(peek().content == "delete")
        return true;
    return false;
}

DeleteStmtNode* Parser::DelStmt(){
    int line = peek().line;
    int col = peek().column;
    if(!IsDelStmt())
        throw EyparseError("StmtError", "it is not a del-stmt", line ,col);
    DeleteStmtNode* node = new DeleteStmtNode;

    node->_DeleteMark = token();//not this!
    if(peek().symbol == eylex::Symbol::Identifier)
        node->_Iden = token();
    else
        throw EyparseError("SyntaxError", "expect identifier behind the 'delete'", line, col);
    if(peek().content == ";")
        node->_EndMark = token();
    else
        throw EyparseError("SymbolError", "expect ';'", line ,col);
    return node;
}

bool Parser::IsBlockStmt(){
    if (!IsToken()) return false;
    return peek().content == "{";
}

BlockStmtNode* Parser::BlockStmt(){
    int line = peek().line;
    int col = peek().column;
    if (!IsBlockStmt())
        throw EyparseError("StmtError", "it is not a block-stmt", line ,col);
    BlockStmtNode* node = new BlockStmtNode;

    node->_Left = token();
    vector<StmtNode*> stmts;
    while(peek().content != "}"){
        if(!IsStmt())
            throw EyparseError("Syntax", "an unknown sentence", line ,col);
        stmts.push_back(Stmt());
    }
    node->_Stmt = stmts;
    node ->_Right = token();
    return node;
}

bool Parser::IsStmt() {
    if (!IsToken()) return false;
    return IsOutStmt() || IsVorcStmt() || IsAssignStmt() || IsBlockStmt() || IsDelStmt() || IsInputStmt();
}

StmtNode* Parser::Stmt() {
    int line = peek().line;
    int col = peek().column;
    if (!IsStmt())
        throw EyparseError("StmtError", "it is not any stmt", line ,col);
    StmtNode* stmt = new StmtNode;
    if (IsOutStmt()) {
        stmt->_OutStmt = OutStmt();
        return stmt;
    }
    else if (IsInputStmt()) {
        stmt->_InputStmt = InputStmt();
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
    throw EyparseError("StmtError", "it is not any stmt", line ,col);
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

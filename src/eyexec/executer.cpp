#include "executer.h"

using namespace eyexec;

Instruction eyexec::new_ei(Ins itype, int l, int c, float op, int opint, string opstr, bool opbool, int opty) {
    return Instruction{itype, l, c, op, opint, opstr, opbool, opty};
}

string eyexec::to_string(Instruction ins){
    string type;
    switch (ins.ins_type)
    {
    case Ins::ADD:
        type = "add";
        break;
    case Ins::NOP:
        type = "__none__";
        break;
    case Ins::PUSH:
        type = "push";
        break;
    case Ins::POP:
        type = "pop";
        break;
    case Ins::SUB:
        type = "sub";
        break;
    case Ins::MUL:
        type = "mul";
        break;
    case Ins::DIV:
        type = "div";
        break;
    case Ins::MOD:
        type = "mod";
        break;
    case Ins::MRET:
        type = "mret(>)";
        break;
    case Ins::LEST:
        type = "lest(<)";
        break;
    case Ins::EQ:
        type = "eq(==)";
        break;
    case Ins::NEQ:
        type = "neq(!=)";
        break;
    case Ins::MREQT:
        type = "mreqt(>=)";
        break;
    case Ins::LEREQT:
        type = "lereqt(<=)";
        break;
    case Ins::LAND:
        type = "And(&&)";
        break;
    case Ins::LOR:
        type = "Or(||)";
        break;
    case Ins::NOT:
        type = "not(!)";
        break;
    case Ins::STRING:
        type = "__string__";
        break;
    case Ins::IDEN:
        type = "__identifier__";
        break;
    case Ins::OSOUT:
        type = "out";
        break;
    case Ins::OSINPUT:
        type = "input";
        break;
    case Ins::ASSIGN:
        type = "assign";
        break;
    case Ins::DEL:
        type = "delete";
        break;
    case Ins::SCOPE_BEGIN:
        type = "scope_begin";
        break;
    case Ins::SCOPE_END:
        type = "scope_end";
        break;
    case Ins::DEFINE_VORC:
        type = "variable or constant define";
        break;
    case Ins::OSWHILE:
        type = "while";
        break;
    case Ins::GOTO:
        type = "goto";
        break;
    case Ins::GOTO_WITHCOND:
        type = "goto_with_condition";
        break;
    default:
        break;
    }
    return "{ins: [type: " + type + "][op_str: " + ins.op_str + "][op(float): " + std::to_string(ins.op) + "][op_bool: " + std::to_string((int)ins.op_bool) + "][op_vty:" + std::to_string((int)ins.op_type) + "]}\n";
}

std::vector<Instruction> Environment::clone(int start, int end){
    std::vector<Instruction> cloned;
    for(int i = start; i < end; i++){
        cloned.push_back(this->instructions.at(i));
    }
    return cloned;
}

string Environment::toString(){
    string s;
    for(int i = 0; i < this->instructions.size(); i++){
        s += to_string(instructions.at(i)) + ", ";
    }
    return s;
}

Environment::SysPool Environment::where(Pool p){
    switch (p) {
    case Pool::__A__:
        return Environment::SysPool::_A_;
        break;
    case Pool::__B__:
        return Environment::SysPool::_B_;
        break;
    case Pool::__C__:
        return Environment::SysPool::_C_;
        break;
    case Pool::__D__:
        return Environment::SysPool::_D_;
        break;
    case Pool::__E__:
        return Environment::SysPool::_E_;
        break;
    case Pool::__F__:
        return Environment::SysPool::_F_;
        break;
    case Pool::__G__:
        return Environment::SysPool::_G_;
        break;
    default:
        break;
    }
}

void Environment::reset() {
    runtime_stack.clear();
    stack_top = 0;
    instructions.clear();
}

Environment::runit Environment::pop() {
    return runtime_stack[-- stack_top];
}

void Environment::push(Environment::runit op) {
    if (runtime_stack.size() <= stack_top) {
        for (int i = runtime_stack.size() ; i <= stack_top ; i ++)
            runtime_stack.push_back(Environment::runit(ValueType::NULLTYPE, 0));
    }
    runtime_stack[stack_top ++] = op;
}

Environment& Executer::getEnvironment() { return this->env;}

void Executer::setInstructions(std::vector<Instruction> instructions) {
    env.reset();
    env.instructions = instructions;
}

void Executer::run() {
    int pos = 0;
    while (pos < env.instructions.size()) {
        Instruction ins = env.instructions[pos];
        switch (ins.ins_type) {
            case Ins::NOP: {
                break;
            }
            case Ins::PUSH: {
                if(ins.op_type == TY_CON) env.push(Environment::runit(Environment::ValueType::STRING, ins.op)); //cout<<"\n"<<ins.op<<"-string\n";}
                else if(ins.op_type == TY_IMM) env.push(Environment::runit(Environment::ValueType::INT, ins.op));
                else if(ins.op_type == TY_DEC) env.push(Environment::runit(Environment::ValueType::DECI, ins.op));
                else if(ins.op_type == TY_BOL) env.push(Environment::runit(Environment::ValueType::BOOL, ins.op));
                else env.push(Environment::runit(Environment::ValueType::NULLTYPE, ins.op));
                break;
            }
            case Ins::POP: {
                if(env.ScopeUnit.findAll(ins.op_str)){
                    auto current = env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str);
                    if(current.getArray() == true){
                        auto aindex = (int)env.pop().second;
                        if(current.getType() == eytype::EybType::String) {
                            current.setApoint(aindex);
                            env.ConstantPool.push_back(current.getArrayValueAsString());
                            env.push(Environment::runit(Environment::ValueType::STRING, env.ConstantPool.size()-1));

                            if(env.instructions.at(pos + 1).ins_type == Ins::OSOUT){
                                cout<<current.getValueAsString();
                                pos += 1;
                            }
                        }
                        else if(current.getType() == eytype::EybType::Integer){
                            current.setApoint(aindex);
                            env.push(Environment::runit(Environment::ValueType::INT, current.getArrayValueAsInt()));
                        }

                        else if (current.getType() == eytype::EybType::Decimal){
                            current.setApoint(aindex);
                            env.push(Environment::runit(Environment::ValueType::DECI, current.getArrayValueAsDecimal()));
                        }
                    
                        else if (current.getType() == eytype::EybType::Bool){
                            current.setApoint(aindex);
                            env.push(Environment::runit(Environment::ValueType::BOOL, current.getArrayValueAsBool()));
                        }
                    }
                    else {
                        if(current.getType() == eytype::EybType::String) {
                            env.ConstantPool.push_back(current.getValueAsString());
                            env.push(Environment::runit(Environment::ValueType::STRING, env.ConstantPool.size()-1));

                            if(env.instructions.at(pos + 1).ins_type == Ins::OSOUT){
                                cout<<current.getValueAsString();
                                pos += 1;
                            }
                        }
                        else if(current.getType() == eytype::EybType::Integer){
                            env.push(Environment::runit(Environment::ValueType::INT, current.getValueAsInt()));}

                        else if (current.getType() == eytype::EybType::Decimal)
                            env.push(Environment::runit(Environment::ValueType::DECI, current.getValueAsDecimal()));
                    
                        else if (current.getType() == eytype::EybType::Bool)
                            env.push(Environment::runit(Environment::ValueType::BOOL, current.getValueAsBool()));
                    }
                }
                else{throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Ins::ADD: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 + op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 + op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 + op2));
                break;
            }
            case Ins::SUB: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 - op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 - op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 - op2));
                break;
            }
            case Ins::DIV: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 / op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 / op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 / op2));
                break;
            }
            case Ins::MUL: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 * op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 * op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 * op2));
                break;
            }
            case Ins::EQ: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::DECI && op2.first == Environment::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second == op2.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second == op2.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING){
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op1.second] == env.ConstantPool[op2.second]));}
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::NEQ: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second != op2.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second != op2.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op1.second] != env.ConstantPool[op2.second]));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::MRET: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op2.second > op1.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op2.second > op1.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op2.second].size() > env.ConstantPool[op1.second].size()));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::LEST: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op2.second < op1.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op2.second < op1.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op2.second].size() < env.ConstantPool[op1.second].size()));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::MREQT: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second > op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second > op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (env.ConstantPool[op1.second].size() > env.ConstantPool[op2.second].size()) || (env.ConstantPool[op1.second].size() > env.ConstantPool[op2.second].size())));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::LEREQT: {
                auto op2 = env.pop();
                auto op1 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second < op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second < op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (env.ConstantPool[op1.second].size() < env.ConstantPool[op2.second].size()) || (env.ConstantPool[op1.second].size() < env.ConstantPool[op2.second].size())));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Ins::LAND: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                if (op1 == 1){
                    if (op2 == 1) env.push(Environment::runit(Environment::ValueType::BOOL, 1));
                }
                env.push(Environment::runit(Environment::ValueType::BOOL, 0));
                break;
            }
            case Ins::LOR: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                break;
            }
            case Ins::NOT: {
                auto op = env.pop();
                if(op.second == 1)
                    env.push(Environment::runit(Environment::ValueType::BOOL, 0));
                else {env.push(Environment::runit(Environment::ValueType::BOOL, 1));}
                break;
            }
            case Ins::MOD: {
                long op2 = env.pop().second;
                long op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 % op2));
                break;
            }
            case Ins::STRING: {
                env.push(Environment::runit(Environment::ValueType::STRING, ins.op));
                break;
            }
            case Ins::IDEN: {
                env.push(Environment::runit(Environment::ValueType::NULLTYPE, ins.op));
                break;
            }
            case Ins::OSOUT: {
                auto temp = env.pop();
                if (ins.op_type == TY_CON)
                    std::cout << env.ConstantPool[temp.second];
                else if (temp.first == Environment::STRING)
                    std::cout << env.ConstantPool[temp.second];
                else if (ins.op_type == TY_IMM)
                    std::cout << temp.second;
                else if (ins.op_type == TY_DEC)
                    std::cout << temp.second;
                else if (ins.op_type == TY_BOL)
                    std::cout << temp.second;
                else if (temp.first == Environment::DECI)
                    std::cout << temp.second;
                else if (temp.first == Environment::INT)
                    std::cout << temp.second;
                else if (temp.first == Environment::BOOL)
                    std::cout << temp.second;
                else std::cout << env.ConstantPool[temp.second];
                break;
            }
            case Ins::OSINPUT: {
                auto temp = env.pop();
                if(temp.first == Environment::ValueType::STRING){
                    if(env.ScopeUnit.findAll(ins.op_str)){
                        std::cout << env.ConstantPool[temp.second];
                        string input;
                        getline(cin, input);
                        if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::String)
                            env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(input);
                        else
                            throw EyparseError("[Runtime]TypeError", "must be a 'string' type identifier", ins.line, ins.col);
                    }
                    else throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);
                }
                else throw EyparseError("[Runtime]TypeError", "The type of value is not a string and cannot be used as an 'input' argument", 0, 0);
                break;
            }
            case Ins::DEFINE_VORC: {
                std::string value_str;
                int value_int;
                float value_deci;
                bool value_bool;
                bool icst = ins.op_bool; // is const

                int opt;

                if (ins.op_type == TY_CON) {
                    value_str = env.ConstantPool[env.pop().second];
                    opt = 0; // string ->0
                }
                else if (ins.op_type == TY_IMM){ 
                    value_int = env.pop().second;
                    opt = 1; // int -> 1
                }
                else if (ins.op_type == TY_BOL){ 
                    value_bool = env.pop().second;
                    opt = 3; // bool -> 3
                }
                else {
                    value_deci = env.pop().second;
                    opt = 2; // deci -> 2
                }
                env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).IdenTable.push_back(ins.op_str);
                if (opt == 0) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_str, icst)));}
                else if (opt == 1) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_int, icst)));}
                else if (opt == 2) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_deci, icst)));}
                else if (opt == 3) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_bool, icst)));}
                break;
            }
            case Ins::DEFINE_ARRAY: {
                std::vector<int> value_int;
                std::vector<float> value_deci;
                std::vector<string> value_string;
                std::vector<bool> value_bool;

                auto max_length = ins.op_int;
                if(ins.op_type == TY_IMM){
                    for(int i = 0; i < max_length; i++){
                        value_int.push_back((int)env.pop().second);
                    }
                    reverse(value_int.begin(), value_int.end());
                }
                else if(ins.op_type == TY_CON){
                    for(int i = 0; i < max_length; i++){
                        value_string.push_back(env.ConstantPool[env.pop().second]);
                    }
                    reverse(value_string.begin(), value_string.end());
                }
                else if(ins.op_type == TY_DEC){
                    for(int i = 0; i < max_length; i++){
                        value_deci.push_back(env.pop().second);
                    }
                    reverse(value_deci.begin(), value_deci.end());
                }
                else if(ins.op_type == TY_BOL){
                    for(int i = 0; i < max_length; i++){
                        value_bool.push_back((bool)env.pop().second);
                    }
                    reverse(value_bool.begin(), value_bool.end());
                }
                env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).IdenTable.push_back(ins.op_str);
                if(ins.op_type == TY_IMM) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(&value_int, max_length)));}
                else if(ins.op_type == TY_CON) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(&value_string, max_length)));}
                else if(ins.op_type == TY_DEC) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(&value_deci, max_length)));}
                else if(ins.op_type == TY_BOL) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(&value_bool, max_length)));}
                break;
            }
            case Ins::ASSIGN_ARRAY: {
                std::string value = "__null__";
                int value_int = 0;
                float value_deci = 0;
                bool value_bool = false;
                auto target_index = (int)env.pop().second;
                if(ins.op_type == TY_CON){
                    value = env.ConstantPool[env.pop().second];
                }
                else if(ins.op_type == TY_IMM){
                    value_int = env.pop().second;
                }
                else if(ins.op_type == TY_BOL){
                    if(env.pop().second == 0){
                        value_bool = false;
                    }
                    else value_bool = true;
                }
                else if(ins.op_type == TY_DEC){
                    value_deci = env.pop().second;
                }
                else throw EyparseError("[Runtime]TypeError", "Unknown Type", ins.line, ins.col);
                env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setApoint(target_index);
                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(ins.op_type == TY_IMM) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setArrayValue(value_int);log((string)"y");}
                    else if(ins.op_type == TY_DEC) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setArrayValue(value_deci);}
                    else if(ins.op_type == TY_CON) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setArrayValue(value);}
                    else if(ins.op_type == TY_BOL) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setArrayValue(value_bool);}
                    else throw EyparseError("[Runtime]OtherError", "Unknown control", ins.line, ins.col);
                }
                else {throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Ins::ASSIGN: {
                std::string *value = nullptr;
                int value_int;
                float value_deci;
                bool value_bool;
                if(ins.op_type == TY_CON){
                    value = &env.ConstantPool[env.pop().second];
                }
                else if(ins.op_type == TY_IMM){
                    value_int = env.pop().second;
                }
                else if(ins.op_type == TY_BOL){
                    if(env.pop().second == 0){
                        value_bool = false;
                    }
                    else value_bool = true;
                }
                else if(ins.op_type == TY_DEC){
                    value_deci = env.pop().second;
                }
                else throw EyparseError("[Runtime]TypeError", "Unknown Type", ins.line, ins.col);

                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(ins.op_type == TY_IMM) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_int);}
                    else if(ins.op_type == TY_DEC) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_deci);}
                    else if(ins.op_type == TY_CON) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(*value);}
                    else if(ins.op_type == TY_BOL) {env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_bool);}
                    else throw EyparseError("[Runtime]OtherError", "Unknown control", ins.line, ins.col);
                }
                else {throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Ins::DEL: {
                if(env.ScopeUnit.findAll(ins.op_str)){
                    for(auto it = env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.begin(); it != env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.end(); ){
                        if(it->first == ins.op_str)
                            env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.erase(it++);
                        else
                            it++;
                    }
                }
                else {throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Ins::OSWHILE: break;
            case Ins::GOTO_WITHCOND: {
                if(ins.op_str == "IF_STMT") {
                    if(env.pop().second == 1) {
                    }
                    else {
                        int sign = 0;
                        bool ed = false;
                        while(true) {
                            if (env.instructions.at(pos).ins_type == Ins::SCOPE_BEGIN) {sign++; ed = true;}
                            else if (env.instructions.at(pos).ins_type == Ins::SCOPE_END) {sign--;}
                            if (sign == 0 && ed == true) break;
                            pos++;
                        }
                    }
                }
                else if(ins.op_str == "WHILE_STMT_ISPASS") {
                    if(env.pop().second == 1) {
                        int sign = 0;
                        bool ed = false;
                        while(true) {
                            if (env.instructions.at(pos).ins_type == Ins::SCOPE_BEGIN) {sign++; ed = true;}
                            else if (env.instructions.at(pos).ins_type == Ins::SCOPE_END) {sign--;}
                            if (sign == 0 && ed == true) break;
                            pos++;
                        }
                        pos++;
                    }
                    else;
                }
                else if(ins.op_str == "WHILE_STMT_BACK") {
                    int sign = 0;
                    bool ed = false;
                    while(true) {
                        if (env.instructions.at(pos).ins_type == Ins::SCOPE_BEGIN) {sign--;}
                        else if (env.instructions.at(pos).ins_type == Ins::SCOPE_END) {sign++; ed = true;}
                        if (sign == 0 && ed == true) break;
                        pos--;
                    }
                    while(true) {
                        if (env.instructions.at(pos).ins_type != Ins::OSWHILE) pos--;
                        else break;
                    }
                }
                break;
            }
            case Ins::SCOPE_BEGIN: {
                env.ScopeUnit.add();
                break;
            }
            case Ins::SCOPE_END: {
                env.ScopeUnit.pop();
                break;
            }
        }
        pos ++;
    }
}
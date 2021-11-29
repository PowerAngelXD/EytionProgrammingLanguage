#include "executer.h"

using namespace eyexec;

Instruction::Instruction(Ins ins_type, int l, int c, float op, char op_type) : ins_type(ins_type), line(l), col(c), op(op), op_type(op_type) {}
Instruction::Instruction(Ins ins_type, int l, int c, string op1, float op, char op_type, bool op_bool, Pool pp) : ins_type(ins_type), line(l), col(c), op_str(op1), op(op), op_type(op_type), op_bool(op_bool), p(pp) {}
string Instruction::toString(){
    string type;
    switch (this->ins_type)
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
        type = "add";
        break;
    case Ins::OSREPEAT:
        type = "repeat";
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
    return "{ins: [type: " + type + "][op_str: " + op_str + "][op(float): " + std::to_string(op) + "][op_bool: " + std::to_string((int)op_bool) + "]}\n";
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
        s += instructions.at(i).toString() + ", ";
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
            case Instruction::NOP: {
                break;
            }
            case Instruction::PUSH: {
                if(ins.op_type == TY_CON) env.push(Environment::runit(Environment::ValueType::STRING, ins.op)); //cout<<"\n"<<ins.op<<"-string\n";}
                else if(ins.op_type == TY_IMM) env.push(Environment::runit(Environment::ValueType::INT, ins.op));
                else if(ins.op_type == TY_DEC) env.push(Environment::runit(Environment::ValueType::DECI, ins.op));
                else env.push(Environment::runit(Environment::ValueType::NULLTYPE, ins.op));
                break;
            }
            case Instruction::POP: {
                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::String) {
                        env.ConstantPool.push_back(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsString());
                        env.push(Environment::runit(Environment::ValueType::STRING, env.ConstantPool.size()-1));

                        if(env.instructions.at(pos + 1).ins_type == Instruction::OSOUT){
                            cout<<env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsString();
                            pos += 1;
                        }
                    }
                    else if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Integer){
                        env.push(Environment::runit(Environment::ValueType::INT, env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsInt()));}

                    else if (env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Decimal)
                        env.push(Environment::runit(Environment::ValueType::DECI, env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsDecimal()));
                    
                    else if (env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Bool)
                        env.push(Environment::runit(Environment::ValueType::BOOL, env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsBool()));
                }
                else{throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Instruction::ADD: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 + op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 + op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 + op2));
                break;
            }
            case Instruction::SUB: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 - op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 - op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 - op2));
                break;
            }
            case Instruction::DIV: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 / op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 / op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 / op2));
                break;
            }
            case Instruction::MUL: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                if(isInt(op1 * op2))
                    env.push(Environment::runit(Environment::ValueType::INT, op1 * op2));
                else
                    env.push(Environment::runit(Environment::ValueType::DECI, op1 * op2));
                break;
            }
            case Instruction::EQ: {
                auto op1 = env.pop();
                auto op2 = env.pop();
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
            case Instruction::NEQ: {
                auto op1 = env.pop();
                auto op2 = env.pop();
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
            case Instruction::MRET: {
                auto op1 = env.pop();
                auto op2 = env.pop();
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
            case Instruction::LEST: {
                auto op1 = env.pop();
                auto op2 = env.pop();
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
            case Instruction::MREQT: {
                auto op1 = env.pop();
                auto op2 = env.pop();
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
            case Instruction::LEREQT: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second < op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (op1.second < op2.second) || (op1.second == op2.second)));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, (env.ConstantPool[op1.second].size() < env.ConstantPool[op2.second].size()) || (env.ConstantPool[op1.second].size() > env.ConstantPool[op2.second].size())));
                else
                    throw EyparseError("[Runtime]TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Instruction::LAND: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                if (op1 == 1){
                    if (op2 == 1) env.push(Environment::runit(Environment::ValueType::BOOL, 1));
                }
                env.push(Environment::runit(Environment::ValueType::BOOL, 0));
                break;
            }
            case Instruction::LOR: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                break;
            }
            case Instruction::NOT: {
                auto op = env.pop();
                if(op.second == 1)
                    env.push(Environment::runit(Environment::ValueType::BOOL, 0));
                else {env.push(Environment::runit(Environment::ValueType::BOOL, 1));}
                break;
            }
            case Instruction::MOD: {
                long op2 = env.pop().second;
                long op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 % op2));
                break;
            }
            case Instruction::STRING: {
                env.push(Environment::runit(Environment::ValueType::STRING, ins.op));
                break;
            }
            case Instruction::IDEN: {
                env.push(Environment::runit(Environment::ValueType::NULLTYPE, ins.op));
                break;
            }
            case Instruction::OSOUT: {
                auto temp = env.pop();
                if (ins.op_type == TY_CON)
                    std::cout << env.ConstantPool[temp.second];
                else if (temp.first == Environment::STRING)
                    std::cout << env.ConstantPool[temp.second];
                else if (ins.op_type == TY_IMM)
                    std::cout << temp.second;
                else if (ins.op_type == TY_DEC)
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
            case Instruction::OSINPUT: {
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
            case Instruction::DEFINE_VORC: {
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
                if (opt == 0) {
                   env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_str, icst)));
                }
                else if (opt == 1) {
                    env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_int, icst)));
                }
                else if (opt == 2) {
                    env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_deci, icst)));
                }
                else if (opt == 3) {
                    env.ScopeUnit.ScopeStack.at(env.ScopeUnit.now).BoardPool.insert(std::pair<std::string, eyv::EyValue>(ins.op_str, eyv::EyValue(value_bool, icst)));
                }
                break;
            }
            case Instruction::ASSIGN: {
                std::string *value = nullptr;
                int value_int = 9999999;
                float value_deci = 9999999;
                bool value_bool = false;
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
                else{
                    value_deci = env.pop().second;
                }

                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(value != nullptr) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(*value);
                    else if(value_int != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_int);
                    else if(value_deci != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_deci);
                    else env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_bool);
                }
                else {throw EyparseError("[Runtime]NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Instruction::DEL: {
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
            case Instruction::OSREPEAT: {
                cout<<env.toString()<<endl;
                int ttimes = env.pop().second;
                int times = 0, _endpos = 0, sign = 0;
                int _beginpos = pos;
                bool ed = false;
                while(true) {
                    if (env.instructions.at(pos).ins_type == Instruction::SCOPE_BEGIN) {sign++; ed = true;}
                    else if (env.instructions.at(pos).ins_type == Instruction::SCOPE_END) {sign--;}
                    if (sign == 0 && ed == true) break;
                    pos++;
                }
                _beginpos ++; pos++;
                std::vector<Instruction> c = env.clone(_beginpos, pos);
                //log(_beginpos);
                //log(pos);
                eyexec::Executer e;
                while (true) {
                    e.env.reset();
                    if(times == ttimes) break;
                    log(times);
                    e.run();
                    times++;
                }
                break;
            }
            case Instruction::GOTO_WITHCOND: {
                if(ins.op_str == "IF_STMT") {
                    if(env.pop().second == 1) {
                    }
                    else {
                        int sign = 0;
                        bool ed = false;
                        while(true) {
                            if (env.instructions.at(pos).ins_type == Instruction::SCOPE_BEGIN) {sign++; ed = true;}
                            else if (env.instructions.at(pos).ins_type == Instruction::SCOPE_END) {sign--;}
                            if (sign == 0 && ed == true) break;
                            pos++;
                        }
                    }
                }
                break;
            }
            case Instruction::SCOPE_BEGIN: {
                env.ScopeUnit.add();
                break;
            }
            case Instruction::SCOPE_END: {
                env.ScopeUnit.pop();
                break;
            }
        }
        pos ++;
    }
}
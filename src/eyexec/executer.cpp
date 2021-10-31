#include "executer.h"

using namespace eyexec;

Instruction::Instruction(Ins ins_type, int l, int c, float op, char op_type) : ins_type(ins_type), line(l), col(c), op(op), op_type(op_type) {}
Instruction::Instruction(Ins ins_type, int l, int c, string op1, float op, char op_type, bool op_bool) : ins_type(ins_type), line(l), col(c), op_str(op1), op(op), op_type(op_type), op_bool(op_bool) {}

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
                        env.push(Environment::runit(Environment::ValueType::STRING, env.ConstantPool.size()));

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
                else{throw EyparseError("NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
                break;
            }
            case Instruction::ADD: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 + op2));
                break;
            }
            case Instruction::SUB: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 - op2));
                break;
            }
            case Instruction::DIV: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 / op2));
                break;
            }
            case Instruction::MUL: {
                float op2 = env.pop().second;
                float op1 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 * op2));
                break;
            }
            // case Instruction::EQ: {
            //     auto op1 = env.pop().second;
            //     auto op2 = env.pop().second;
            //     env.push(Environment::runit(Environment::ValueType::BOOL, op1 == op2));
            //     break;
            // }
            // case Instruction::NEQ: {
            //     auto op1 = env.pop().second;
            //     auto op2 = env.pop().second;
            //     env.push(Environment::runit(Environment::ValueType::BOOL, op1 != op2));
            //     break;
            // }
            case Instruction::EQ: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                if(op1.first == Environment::DECI && op2.first == Environment::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second == op2.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second == op2.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op1.second] == env.ConstantPool[op2.second]));
                else
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
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
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
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
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
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
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Instruction::MREQT: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second >= op2.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second >= op2.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op1.second].size() >= env.ConstantPool[op2.second].size()));
                else
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Instruction::LEREQT: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                if(op1.first == Environment::ValueType::DECI && op2.first == Environment::ValueType::DECI)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second <= op2.second));
                else if(op1.first == Environment::INT && op2.first == Environment::INT)
                    env.push(Environment::runit(Environment::ValueType::BOOL, op1.second <= op2.second));
                else if(op1.first == Environment::STRING && op2.first == Environment::STRING)
                    env.push(Environment::runit(Environment::ValueType::BOOL, env.ConstantPool[op1.second].size() <= env.ConstantPool[op2.second].size()));
                else
                    throw EyparseError("TypeError", "Cannot compare two diffierent type value", 0, 0);
                break;
            }
            case Instruction::LAND: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                env.push(Environment::runit(Environment::ValueType::BOOL, bool(op1.second) && bool(op2.second)));
            }
            case Instruction::LOR: {
                auto op1 = env.pop();
                auto op2 = env.pop();
                env.push(Environment::runit(Environment::ValueType::BOOL, bool(op1.second) || bool(op2.second)));
            }
            case Instruction::NOT: {
                auto op = env.pop();
                env.push(Environment::runit(Environment::ValueType::BOOL, !bool(op.second)));
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
                else {throw EyparseError("NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
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
                else {throw EyparseError("NameError", "Unknown Identifier:" + ins.op_str, ins.line, ins.col);}
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
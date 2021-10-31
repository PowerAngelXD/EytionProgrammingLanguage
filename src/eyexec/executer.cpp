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
                if(ins.op_type == TY_CON) env.push(Environment::runit(Environment::ValueType::STRING, ins.op));
                else if(ins.op_type == TY_IMM) env.push(Environment::runit(Environment::ValueType::INT, ins.op));
                else if(ins.op_type == TY_DEC) env.push(Environment::runit(Environment::ValueType::DECI, ins.op));
                else env.push(Environment::runit(Environment::ValueType::NULLTYPE, ins.op));
                break;
            }
            case Instruction::POP: {
                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Integer){
                        env.push(Environment::runit(Environment::ValueType::INT, env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsInt()));}

                    else if (env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Decimal)
                        env.push(Environment::runit(Environment::ValueType::DECI, env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsDecimal()));

                    else if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::String) {
                        env.ConstantPool.push_back(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsString());
                        env.push(Environment::runit(Environment::ValueType::STRING, env.ConstantPool.size()));

                        if(env.instructions.at(pos + 1).ins_type == Instruction::OSOUT){
                            cout<<env.ConstantPool.at(env.ConstantPool.size() - 1);
                            pos += 1;
                        }
                    }
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
            case Instruction::EQ: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 == op2));
                break;
            }
            case Instruction::NEQ: {
                auto op1 = env.pop().second;
                auto op2 = env.pop().second;
                env.push(Environment::runit(Environment::ValueType::DECI, op1 != op2));
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
                if (ins.op_type == TY_CON)
                    std::cout << env.ConstantPool[env.pop().second];
                else
                    std::cout << env.pop().second;
                break;
            }
            case Instruction::DEFINE_VORC: {
                std::string value_str;
                int value_int;
                float value_deci;
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
                break;
            }
            case Instruction::ASSIGN: {
                std::string *value = nullptr;
                int value_int = 9999999;
                float value_deci = 9999999;
                if(ins.op_type == TY_CON){
                    value = &env.ConstantPool[env.pop().second];
                }
                else if(ins.op_type == TY_IMM){
                    value_int = env.pop().second;
                }
                else{
                    value_deci = env.pop().second;
                }

                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(value != nullptr) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(*value);
                    else if(value_int != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_int);
                    else if(value_deci != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_deci);
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
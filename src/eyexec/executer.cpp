#include "executer.h"

using namespace eyexec;

Instruction::Instruction(Ins ins_type, float op, char op_type) : ins_type(ins_type), op(op), op_type(op_type) {}
Instruction::Instruction(Ins ins_type, string op1, float op, char op_type, bool op_bool) : ins_type(ins_type), op_str(op1), op(op), op_type(op_type), op_bool(op_bool) {}

void Environment::reset() {
    runtime_stack.clear();
    stack_top = 0;
    instructions.clear();
}

float Environment::pop() {
    return runtime_stack[-- stack_top];
}

void Environment::push(float op) {
    if (runtime_stack.size() <= stack_top) {
        for (int i = runtime_stack.size() ; i <= stack_top ; i ++)
            runtime_stack.push_back(0);
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
                env.push(ins.op);
                break;
            }
            case Instruction::PUSHAS: {
            }
            case Instruction::POP: {
                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Integer){
                        env.push(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsInt());}

                    else if (env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::Decimal)
                        env.push(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsDecimal());

                    else if(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getType() == eytype::EybType::String) {
                        env.ConstantPool.push_back(env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).getValueAsString());


                        if(env.instructions.at(pos + 1).ins_type == Instruction::OSOUT){
                            cout<<env.ConstantPool.at(env.ConstantPool.size() - 1);
                            pos += 1;
                        }
                    }
                }
                else{throw (std::string)"Unknown identifier: '" + ins.op_str + "'";}
                break;
            }
            case Instruction::ADD: {
                float op2 = env.pop();
                float op1 = env.pop();
                env.push(op1 + op2);
                break;
            }
            case Instruction::SUB: {
                float op2 = env.pop();
                float op1 = env.pop();
                env.push(op1 - op2);
                break;
            }
            case Instruction::DIV: {
                float op2 = env.pop();
                float op1 = env.pop();
                env.push(op1 / op2);
                break;
            }
            case Instruction::MUL: {
                float op2 = env.pop();
                float op1 = env.pop();
                env.push(op1 * op2);
                break;
            }
            case Instruction::MOD: {
                long op2 = env.pop();
                long op1 = env.pop();
                env.push(op1 % op2);
                break;
            }
            case Instruction::STRING: {
                env.push(ins.op);
                break;
            }
            case Instruction::IDEN: {
                env.push(ins.op);
                break;
            }
            case Instruction::OSOUT: {
                if (ins.op_type == TY_CON)
                    std::cout << env.ConstantPool[env.pop()];
                else
                    std::cout << env.pop();
                break;
            }
            case Instruction::DEFINE_VORC: {
                std::string value_str;
                int value_int;
                float value_deci;
                bool icst = ins.op_bool; // is const

                int opt;

                if (ins.op_type == TY_CON) {
                    value_str = env.ConstantPool[env.pop()];
                    opt = 0; // string ->0
                }
                else if (ins.op_type == TY_IMM){ 
                    value_int = env.pop();
                    opt = 1; // int -> 1
                }
                else {
                    value_deci = env.pop();
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
                    value = &env.ConstantPool[env.pop()];
                }
                else if(ins.op_type == TY_IMM){
                    value_int = env.pop();
                }
                else{
                    value_deci = env.pop();
                }

                if(env.ScopeUnit.findAll(ins.op_str)){
                    if(value != nullptr) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(*value);
                    else if(value_int != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_int);
                    else if(value_deci != 9999999) env.ScopeUnit.ScopeStack.at(env.ScopeUnit.findWhere(ins.op_str)).BoardPool.at(ins.op_str).setValue(value_deci);
                }
                else {throw (std::string)"Unknown identifier: '" + ins.op_str + "'";}
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
                else {throw (std::string)"Unknown identifier: '" + ins.op_str + "'";}
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
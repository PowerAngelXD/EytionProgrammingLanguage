#pragma once

#include "../ey/value/eyvalue.h"
#include "../ey/scope/eyscope.h"
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <map>

namespace eyexec {
    class Instruction {
    public:
        // os 内置命令
        enum Ins {
            NOP = 0,
            PUSH,
            PUSHAS,
            POP,
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            STRING,
            IDEN,
            OSOUT,
            OSINPUT,
            DEL,
            ASSIGN,
            SCOPE_BEGIN,
            SCOPE_END,
            DEFINE_VORC
        };
        Ins ins_type;
        float op;
        string op_str;
        bool op_bool;
        char op_type;
        #define TY_IMM 0 //立即数
        #define TY_DEC 1 //小数
        #define TY_CON 2 //常量池编号
        Instruction()=default;
        Instruction(Ins ins_type, float op = 0, char op_type = TY_DEC);
        Instruction(Ins ins_type, string op1 = " ", float op = 0, char op_type = TY_DEC, bool op_bool = false);
    };

    std::string to_string(Instruction::Ins ins); 

    class Executer;

    class Environment {
        std::vector<Instruction> instructions;
        std::vector<float> runtime_stack;
        int stack_top;
    public:
        std::vector<std::string> ConstantPool;
        eyscope::EyScopeUnit ScopeUnit;
        void reset();     // 重置环境
        float pop();
        void push(float op);
        friend class Executer;
    };

    class Executer {
        Environment env;
    public:
        Environment& getEnvironment();
        void setInstructions(std::vector<Instruction> instructions);
        void run();    // 运行一个.ey文件
    };
}
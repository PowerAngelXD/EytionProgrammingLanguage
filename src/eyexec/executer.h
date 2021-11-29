#pragma once

#include "../ey/value/eyvalue.h"
#include "../ey/scope/eyscope.h"
#include "../eexception/eexcp.h"
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <map>
using namespace eexcp;

namespace eyexec {
    enum Pool {
        __null__,
        __A__, __B__, __C__, __D__, __E__, __F__, __G__,
    };
    class Instruction {
    public:
        // os 内置命令
        enum Ins {
            NOP = 0,
            PUSH,
            POP,
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            //cmp
            MRET, LEST, // > and <
            EQ, NEQ, // == and !=
            MREQT, LEREQT, // >= and <=
            //bool
            LAND, LOR, // && and ||
            NOT, // !
            STRING,
            IDEN,
            OSOUT,
            OSINPUT,
            OSREPEAT, 
            DEL,
            ASSIGN,
            GOTO, 
            GOTO_WITHCOND, 
            SCOPE_BEGIN,
            SCOPE_END,
            DEFINE_VORC
        };
        Ins ins_type = NOP;
        float op = 0.0;
        string op_str = "__null__";
        bool op_bool = false;
        char op_type = 9;
        int line, col;
        Pool p;
        #define TY_IMM 0 //立即数
        #define TY_DEC 1 //小数
        #define TY_CON 2 //常量池编号
        #define TY_BOL 3 // bool
        Instruction()=default;
        Instruction(Ins ins_type, int l = 0, int c = 0, float op = 0, char op_type = TY_DEC);
        Instruction(Ins ins_type, int l = 0, int c = 0, string op1 = " ", float op = 0, char op_type = TY_DEC, bool op_bool = false, Pool pp = Pool::__null__);
        string toString();
    };

    std::string to_string(Instruction::Ins ins); 

    class Executer;

    class Environment {
        enum ValueType{
            NULLTYPE = -1,
            DECI,
            INT,
            STRING,
            BOOL
        };
        enum SysPool {
            _A_, _B_, _C_, _D_, _E_, _F_, _G_
        };
        typedef pair<ValueType, float> runit;
        typedef pair<SysPool, int> spunit;
        std::vector<Instruction> instructions;
        std::vector<spunit> system_pool = {spunit(SysPool::_A_, 0), spunit(SysPool::_B_, 0), spunit(SysPool::_C_, 0), spunit(SysPool::_D_, 0), spunit(SysPool::_E_, 0), spunit(SysPool::_F_, 0), spunit(SysPool::_G_, 0)};
        std::vector<runit> runtime_stack;
        int stack_top;
    public:
        std::vector<std::string> ConstantPool;
        eyscope::EyScopeUnit ScopeUnit;
        string toString();
        std::vector<Instruction> clone(int start, int end);
        SysPool where(Pool p);
        void reset();     // 重置环境
        runit pop();
        void push(runit op);
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
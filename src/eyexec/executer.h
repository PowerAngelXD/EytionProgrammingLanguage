#pragma once

#include "../ey/value/eyvalue.h"
#include "../ey/scope/eyscope.h"
#include "../eexception/eexcp.h"
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <map>

#define TY_NULL 0 //空类型
#define TY_IMM 1 //立即数
#define TY_DEC 2 //小数
#define TY_CON 3 //常量池编号
#define TY_BOL 4 // bool
#define TY_ARR 5 //数组


using namespace eexcp;

namespace eyexec {
    enum Pool {
        __null__,
        __A__, __B__, __C__, __D__, __E__, __F__, __G__,
    };
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
        OSWHILE, 
        DEL,
        ASSIGN,
        GOTO, 
        GOTO_WITHCOND, 
        SCOPE_BEGIN,
        SCOPE_END,
        DEFINE_VORC,
        DEFINE_ARRAY,
        ASSIGN_ARRAY,
    };
    struct Instruction {
    public:
        Ins ins_type = NOP;
        int line = 0;
        int col = 0;
        float op = 0.0;
        int op_int = 0;
        string op_str = "__null__";
        bool op_bool = false;
        int op_type = 9;
        Instruction()=default;
        string toString();
    };

    std::string to_string(Instruction ins); 
    /**
     * @brief new_ei: Create a eytion inside command
     * @param itype command type
     * @param l line
     * @param c column
     * @param op float argument
     * @param opint int argument
     * @param opstr string argument
     * @param opbool bool argument
     * @param opty value type (not command type!!)
     * 
     * @return eyexec::Instruction
     */
    Instruction new_ei(Ins itype=NOP, int l=0, int c=0, float op=0.0, int opint=0, string opstr="__null__", bool opbool=false, int opty=TY_NULL);

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
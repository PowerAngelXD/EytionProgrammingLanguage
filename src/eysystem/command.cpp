#ifndef __COMMAND_CPP__
#define __COMMAND_CPP__
#include "command.h"
#include "../eylexer/lex.h"
#include "../eyparser/parser.h"
#include "../eycodegen/generator.h"
#include "../eyexec/executer.h"
//#define DEBUG
using namespace osstd;

void cmdrun(string argv){
    ifstream file(argv + ".ey");
    eylex::Lexer lexer(file);
    auto tokens = lexer.getTokenGroup();
    eyparser::Parser p(tokens);
    auto stat = p.Stat();
    eycodegen::CodeGenerator gen;
    gen.visitStat(stat);

    #ifdef DEBUG
    log((string)"log for tokens:");
    for(auto tok : tokens) {
        log(tok.format());
    }
    log((string)"log for token's content:");
    for(auto tok : tokens) {
        log(tok.content);
    }
    log((string)"stat:");
    log(stat->toString());
    log((string)"log for inside command");
    for(auto i : gen.instructions){
        string t = "cmd: " + i.op_type;
        log(t);
    }
    #endif // DEBUG

    eyexec::Executer exe;
    exe.setInstructions(gen.instructions);
    exe.getEnvironment().ConstantPool = gen.ConstantPool;
    exe.run();
}
void cmdview(string argv){
    ifstream file(argv + ".ey");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<_FONT_YELLOW<<"file["<<argv<<".ey] content:\n"<<_FONT_GREEN<<content<<_NORMAL<<endl;
}

eysys::eycommand cmdlist[2] = {eysys::eycommand("run", &cmdrun, true), eysys::eycommand("view", &cmdview, true)};

void eysys::start_ezcmd(std::string text){
    if(text == "quit" || text == "exit" || text == "q" || text =="e") exit(0);
    string head, argv;
    head = split(text, " ").at(0);
    argv = split(text, " ").at(1);
    bool outside = false;
    for (eysys::eycommand cmd : cmdlist){
        if(head == cmd._cond && cmd._active == true){
            cmd.run(argv);
        }
        else {
            outside = true;
        }
    }
    if (outside){
        ofstream wt("~temp.ey");
        wt<<text;
        wt.close();
        ifstream file("~temp.ey");
        eylex::Lexer lexer(file);
        auto tokens = lexer.getTokenGroup();
        eyparser::Parser p(tokens);
        auto stat = p.Stat();
        eyexec::Executer exe;
        eycodegen::CodeGenerator gen;
        gen.visitStat(stat);
        exe.setInstructions(gen.instructions);
        exe.getEnvironment().ConstantPool = gen.ConstantPool;
        exe.run();
        file.close();
        remove("~temp.ey");
    }
}

eysys::eycommand::eycommand(string cond, eysys::command_event event, bool active){
    this->_cond = cond;
    this->_event = event;
    this->_active = active;
}

void eysys::eycommand::run(string argv){
    this->_event(argv);
}

void eysys::eycommand::toggle_active(){
    if(this->toggle == 0){
        this->toggle++;
        this->_active = true;
    }
    else if(this->toggle == 1){
        this->toggle--;
        this->_active = false;
    }
}

#endif // !__COMMAND_CPP__
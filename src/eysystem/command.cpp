#ifndef __COMMAND_CPP__
#define __COMMAND_CPP__
#include "command.h"
using namespace osstd;
using namespace eysys;
eysys::settings eysetting;

void cmdrun(string argv){
    ifstream file(argv + ".ey");
    eylex::Lexer lexer(file);
    auto tokens = lexer.getTokenGroup();
    eyparser::Parser p(tokens);
    auto stat = p.Stat();
    eycodegen::CodeGenerator gen;
    gen.visitStat(stat);
    eyexec::Executer eysysenv;

    if(eysetting.DebugMode == true){
        log((string)"because you turned on the debug mode, now show some debug details");
        log((string)"log for tokens:");
        for(auto tok : tokens) {
            log(tok.format());
        }
        log((string)"stat:");
        log(stat->toString());
    }

    eysysenv.setInstructions(gen.instructions);
    eysysenv.getEnvironment().ConstantPool = gen.ConstantPool;
    eysysenv.run();
}
void cmdview(string argv){
    ifstream file(argv + ".ey");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<_FONT_YELLOW<<"file["<<argv<<".ey] content:\n"<<_FONT_GREEN<<content<<_NORMAL<<endl;
}
void cmdinfo(string argv){
    cout<<"Eytion Language"<<_FONT_GREEN<<endl;
    cout<<"------------                                 "<<endl;
    cout<<"--                                             "<<endl;
    cout<<"--                                             "<<endl;
    cout<<"------------                                 "<<endl;
    cout<<"--            \\         //             "<<endl;
    cout<<"--             \\       //              "<<endl;
    cout<<"------------    \\     //                 "<<endl;
    cout<<"                 \\   //                  "<<endl;
    cout<<"                  \\ //                 "<<endl;
    cout<<"                   //                  "<<endl;
    cout<<"                  //                   "<<endl;
    cout<<"                 //                   "<<endl;
    cout<<"                //                   "<<_NORMAL<<endl;
    cout<<"Copyright (c)CodeAreaDevTeam, XtherDevTeam, PowerAngelXD"<<endl;
    cout<<"License: MIT"<<endl;
    ifstream file("./data/docs/License");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<"License content:"<<endl;
    cout<<_FONT_GREEN<<content<<_NORMAL<<endl;
    cout<<"now version: v0.1.3-alpha-20211017"<<endl;
}
void cmdhelp(string argv){
    ifstream file("./data/docs/help.txt");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<_FONT_GREEN<<content<<_NORMAL<<endl;
}

eysys::eycommand cmdlist[4] = {eysys::eycommand("run", &cmdrun, true),
                               eysys::eycommand("view", &cmdview, true),
                               eysys::eycommand("info", &cmdinfo, true),
                               eysys::eycommand("help", &cmdhelp, true)};

void eysys::start_ezcmd(std::string text, settings setting){
    eysetting = setting;
    if(text == "quit" || text == "exit" || text == "q" || text =="e") {
        if(eysetting.ExitTip == true){
            cout<<"Do you really want to quit?(You can set it in the settings file in './settings/eyconfig.json' without this prompt)"<<endl;
            cout<<"[Yes(y)]     [No(n)]"<<endl;
            if (_getch() == 'y'){
                exit(0);
            }
            return;
        }
        exit(0);
    }
    else if(text == "info"){
        cmdlist[2].run(" ");
        return;
    }
    else if(text == "help"){
        cmdlist[3].run(" ");
        return;
    }
    string head, argv;
    head = split(text, " ").at(0);
    argv = split(text, " ").at(1);
    for (eysys::eycommand cmd : cmdlist){
        if(head == cmd._cond && cmd._active == true){
            cmd.run(argv);
        }
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
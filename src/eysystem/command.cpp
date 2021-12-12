#ifndef __COMMAND_CPP__
#define __COMMAND_CPP__
#include "command.h"
using namespace osstd;
using namespace eysys;
using namespace econfig;

econfig::EyConfig _efig;

void cmdrun(string argv){
    ifstream file(argv);
    eylex::Lexer lexer(file);
    auto tokens = lexer.getTokenGroup();
    if(_efig.IsDebug == true && _efig.DebugMode.lex_TokenGroupMsg == true){
        log((string)"because you turned on the debug mode, now show some debug details");
        log((string)"log for tokens:");
        for(auto tok : tokens) {
            log(tok.format());
        }
    }
    eyparser::Parser p(tokens);
    auto stat = p.Stat();
    if (_efig.IsDebug == true && _efig.DebugMode.ast_StatMsg == true){
        log((string)"log for stat:");
        log(stat->toString());
    }
    eycodegen::CodeGenerator gen;
    gen.visitStat(stat);
    eyexec::Executer eysysenv;
    eysysenv.setInstructions(gen.instructions);
    eysysenv.getEnvironment().ConstantPool = gen.ConstantPool;
    if (_efig.IsDebug == true && _efig.DebugMode.cgen_OutputGeneratoringCode == true){
        log(eysysenv.getEnvironment().toString());
    }
    eysysenv.run();
}
void cmdview(string argv){
    ifstream file(argv);
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<_FONT_YELLOW<<"file["<<argv<<"] content:\n"<<_FONT_GREEN<<content<<_NORMAL<<endl;
}
void cmdinfo(string argv){
    string path = getenv("EY");
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
    ifstream file(path + "/data/docs/License");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<"License content:"<<endl;
    cout<<_FONT_GREEN<<content<<_NORMAL<<endl;
    cout<<"now version: v0.1.45-alpha-20211205"<<endl;
}
void cmdhelp(string argv){
    string path = getenv("EY");
    ifstream file(path + "/data/docs/help.txt");
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    cout<<_FONT_GREEN<<content<<_NORMAL<<endl;
}
void cmdtest(string argv){
    cmdrun("tests/" + argv);
}

eysys::eycommand cmdlist[5] = {eysys::eycommand("run", &cmdrun, true),
                               eysys::eycommand("view", &cmdview, true),
                               eysys::eycommand("info", &cmdinfo, true),
                               eysys::eycommand("help", &cmdhelp, true),
                               eysys::eycommand("test", &cmdtest, true)};

void eysys::run(std::string text, econfig::EyConfig fig){
    _efig = fig;
    std::cout<<_FONT_BLUE<<"build date: "<<_FONT_GREEN<<__DATE__<<endl;
    cout<<_NORMAL;
    cout<<"Eytion [Shell]"<<endl;
    cout<<"You can enter 'help' to get console help"<<endl;
    while(true){
        try{
            cout<<"\ney > ";
            getline(cin, text);
            if(text == "quit" || text == "exit" || text == "q" || text =="e") {
                if(_efig.ExitTip == true){
                    cout<<"Do you really want to quit?(You can set it in the settings file in './settings/eyconfig.json' without this prompt)"<<endl;
                    cout<<"[Yes(y)]     [No(n)]"<<endl;
                    if (_getch() == 'y'){
                        exit(0);
                    }
                }
                else exit(0);
            }
            else if(text == "info"){
                cmdlist[2].run(" ");
            }
            else if(text == "help"){
                cmdlist[3].run(" ");
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
        catch(char const* e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
        catch(std::string e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
        catch(std::logic_error e){}
        catch(eexcp::EyparseError eyerr){
            cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<eyerr.what()<<_NORMAL<<endl;
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
#include<stdlib.h> // use cmake
#include "osstd.h"
#include "eylexer/lex.h"
#include "eysystem/command.h"
#include "ey/value/eyvalue.h"
#include "eexception/eexcp.h"
#include "../include/CJsonObject.hpp"
#define __RELEASE
#define __INSIDE_DEBUG
using namespace neb;
using namespace osstd;
using namespace std;
void segcontrol(int i){
    cout<<_FONT_RED<<"Because of some exceptions, eytion stop! code: "<<i<<_NORMAL<<endl;
    getch();
}

int main(int argc, char *argv[]){
    signal(SIGSEGV, segcontrol);
    string env = getenv("EY");
    if(env.empty()){
        cout<<_FONT_RED<<"cannot found the ENVIRONMENT VARIABLE 'EY'"<<_NORMAL<<endl;
        exit(0);
    }
    econfig::EyConfig efig;
    econfig::ConfigReader read(env + "/settings/eyconfig.json");
    efig = read.Get();
#ifdef __RELEASE
    system("title EytionLang Shell (20211017a-v0.1.45-alpha)");
    if(argc == 2){
        if(strcmp(argv[1], "-shell") == 0){
            string cmd;
            eysys::run(cmd, efig);
            system("pause");
        }
        else if(strcmp(argv[1], "-v") == 0){
            cout<<"now eytion version: alpha-0.1.45"<<endl;
            cout<<"you can use argument '-shell' to run the EytionShell"<<endl;
        }
        else{
            cout<<"'"<<argv[1]<<"' not a right argument for eytion"<<endl;
        }
    }
    else if(argc == 3){
        if(strcmp(argv[1], "-f") == 0){
            try{
                ifstream file(argv[2]);
                eylex::Lexer lexer(file);
                auto tokens = lexer.getTokenGroup();

                if(efig.IsDebug == true && efig.DebugMode.lex_TokenGroupMsg == true){
                    log((string)"because you turned on the debug mode, now show some debug details");
                    log((string)"log for tokens:");
                    for(auto tok : tokens) {
                        log(tok.format());
                    }
                }

                eyparser::Parser p(tokens);
                auto stat = p.Stat();

                if (efig.IsDebug == true && efig.DebugMode.ast_StatMsg == true){
                    log((string)"log for stat:");
                    log(stat->toString());
                }
                
                eycodegen::CodeGenerator gen;
                gen.visitStat(stat);
                eyexec::Executer eysysenv;
                eysysenv.setInstructions(gen.instructions);
                eysysenv.getEnvironment().ConstantPool = gen.ConstantPool;
                eysysenv.run();
            }
            catch(char const* e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
            catch(std::string e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
            catch(std::logic_error e){}
            catch(eexcp::EyparseError eyerr){
                cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<eyerr.what()<<_NORMAL<<endl;
            }
        }
        else{
            cout<<"'"<<argv[1]<<"' not a right argument for eytion"<<endl;
        }
    }
#endif
#ifdef __DEBUG
    string cmd;
    eysys::run(cmd, efig);
    system("pause");
#endif // DEBUG
    return 0;
}
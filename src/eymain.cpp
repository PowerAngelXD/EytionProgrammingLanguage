#include<stdlib.h>
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
int main(int argc, char *argv[]){
#ifdef __RELEASE
    string env = getenv("EY");
    if(env.empty()){
        cout<<_FONT_RED<<"cannot found the ENVIRONMENT VARIABLE 'EY'"<<_NORMAL<<endl;
        exit(0);
    }
    system("title EytionLang Shell (20211017a-v0.1.45-alpha)");
    if(argc == 2){
        if(strcmp(argv[1], "-shell") == 0){
            string cmd;
            eysys::run(cmd);
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

                #ifdef __INSIDE_DEBUG
                for(auto tok : tokens){
                    log(tok.format());
                }
                #endif

                eyparser::Parser p(tokens);
                auto stat = p.Stat();

                #ifdef __INSIDE_DEBUG
                log(stat->toString());
                #endif
                
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
    eysys::run(cmd);
    system("pause");
#endif // DEBUG
    return 0;
}
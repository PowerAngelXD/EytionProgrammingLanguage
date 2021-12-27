#include<stdlib.h> // use cmake
#include "osstd.h"
#include "eylexer/lex.h"
#include "eysystem/eshell.h"
#include "ey/value/eyvalue.h"
#include "eexception/eexcp.h"
#include "../include/CJsonObject.hpp"
//#define __DEBUG
#define __RELEASE
//#define __WORK_ON_LINUX
using namespace neb;
using namespace osstd;
using namespace std;
void segcontrol(int i){
    cout<<_FONT_RED<<"Because of some exceptions, eytion stop! code: "<<i<<_NORMAL<<endl;
}

int main(int argc, char *argv[]){
#ifndef __WORK_ON_LINUX
    signal(SIGSEGV, segcontrol);
    string env = getenv("EY");
    if(env.empty()){
        cout<<_FONT_RED<<"cannot found the ENVIRONMENT VARIABLE 'EY'(You need to manually add the environment variable 'ey' to the current directory)"<<_NORMAL<<endl;
        exit(0);
    }
#endif
    econfig::EyConfig efig;
#ifdef __WORK_ON_LINUX
    econfig::ConfigReader read("/settings/eyconfig.json");
#else
    econfig::ConfigReader read(env + "/settings/eyconfig.json");
#endif
    efig = read.Get();
#ifdef __RELEASE
    system("title EytionLang Shell (20211226-v0.1.47-alpha)");
    if(argc == 2){
        if(strcmp(argv[1], "-shell") == 0|| strcmp(argv[1], "-s") == 0){
            string cmd;
            eysys::run(cmd, efig);
            system("pause");
        }
        else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-ver") == 0 || strcmp(argv[1], "-version") == 0){
            cout<<"now eytion version: alpha-0.1.47"<<endl;
            cout<<"you can use argument '-shell' to run the EytionShell"<<endl;
        }else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0){
            ifstream file(env + "/data/docs/helpforcmd.txt");
            istreambuf_iterator<char> begin(file);
            istreambuf_iterator<char> end;
            string content(begin, end);
            cout<<_FONT_GREEN<<content<<_NORMAL<<endl;
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
                eyparser::Parser p(tokens);
                auto stat = p.Stat();
                eycodegen::CodeGenerator gen;
                gen.visitStat(stat);
                eyexec::Executer eysysenv;
                eysysenv.setInstructions(gen.instructions);
                eysysenv.getEnvironment().ConstantPool = gen.ConstantPool;
                eysysenv.run();
            }
            catch(char const* e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
            catch(std::string e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
            catch(std::logic_error e){
                cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e.what()<<_NORMAL<<endl;
            }
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
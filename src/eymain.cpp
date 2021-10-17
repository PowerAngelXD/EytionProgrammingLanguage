#include<stdlib.h>
#include "osstd.h"
#include "eylexer/lex.h"
#include "eysystem/command.h"
#include "ey/value/eyvalue.h"
#include "eexception/eexcp.h"
#include "../include/CJsonObject.hpp"
using namespace neb;
using namespace osstd;
using namespace std;

int main(){
    system("title EytionLang Shell (20211017a-v0.11)");
    //check part

    //end
    std::cout<<_FONT_BLUE<<"build date: "<<_FONT_GREEN<<__DATE__<<endl;
    cout<<_NORMAL;
    cout<<"Eytion [Shell]"<<endl;
    cout<<"You can enter 'help' to get console help"<<endl;
    string cmd;
    while(true){
        try{
            cout<<"\n"<<_FONT_YELLOW<<"> "<<_NORMAL;
            getline(cin, cmd);
            eysys::start_ezcmd(cmd);
        }
        catch(char const* e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
        catch(std::string e){cout<<_FONT_YELLOW<<"\nEytionScript has some error:\n    "<<_FONT_RED<<e<<_NORMAL<<endl;}
        catch(std::logic_error e){
            if(e.what() == "map::at") cout<<_FONT_RED<<"\nEytionScript has some error:\n    "<<_FONT_RED<<"Try to access an identifier that does not exist"<<_NORMAL<<endl;
            else cout<<_FONT_RED<<"\nException:\n    "<<_FONT_YELLOW<<e.what()<<_NORMAL<<endl;
        }
        catch(eexcp::EyparseError eyerr){
            cout<<_FONT_RED<<_BG_WHITE<<"\n"<<eyerr.what()<<_NORMAL<<endl;
        }
    }
    system("pause");

    return 0;
}
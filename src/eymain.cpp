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
    system("title EytionLang Shell (20211017a-v0.1.3-alpha)");
    string cmd;
    eysys::run(cmd);
    system("pause");

    return 0;
}
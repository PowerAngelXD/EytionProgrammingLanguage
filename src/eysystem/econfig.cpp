#include "econfig.h"
using namespace econfig;

//config
string EyConfig::toString(){
    string content = "config:";
    if(this->IsDebug == true){
        content += "\nDebugMode: {";
        content += "\nlex_TokenContent : " + bool_to_string(this->DebugMode.lex_TokenContent) + ", lex_TokenGroupMsg : " + bool_to_string(this->DebugMode.lex_TokenGroupMsg);
        content += "\nast_StatMsg : " + bool_to_string(this->DebugMode.ast_StatMsg) + ", ast_SaveAnyTimesAstTreeToLog : " + bool_to_string(this->DebugMode.ast_SaveAnyTimesAstTreeToLog);
        content += "\ncgen_OutputGeneratoringCode : " + bool_to_string(this->DebugMode.cgen_OutputGeneratoringCode);
        content += "}";
    }
    else{
        content += "\nDebugMode : " + bool_to_string(this->IsDebug);
    }
    content += "\nExitTip : " + bool_to_string(this->ExitTip);
    return content;
}

//reader
ConfigReader::ConfigReader(string path){
    ifstream file(path);
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    CJsonObject object(content);
    this->Json = object;
    this->Path = path;
    dump();
}

void ConfigReader::dump(){
    CJsonObject j = this->Json;
    EyConfig ec;
    if(j.KeyExist("Debug")){
        ec.IsDebug = true;
        CJsonObject sub;
        j.Get("DebugMode", sub);
        if(!sub.IsEmpty()){
            if(sub.KeyExist("lex_TokenGroupMsg")) sub.Get("lex_TokenGroupMsg", ec.DebugMode.lex_TokenGroupMsg);
            else if(sub.KeyExist("lex_TokenContent")) sub.Get("lex_TokenContent", ec.DebugMode.lex_TokenContent);
            else if(sub.KeyExist("ast_StatMsg")) sub.Get("ast_StatMsg", ec.DebugMode.ast_StatMsg);
            else if(sub.KeyExist("ast_SaveAnyTimesAstTreeToLog")) sub.Get("ast_SaveAnyTimesAstTreeToLog", ec.DebugMode.ast_StatMsg);
            else if(sub.KeyExist("cgen_OutputGeneratoringCode")) sub.Get("cgen_OutputGeneratoringCode", ec.DebugMode.ast_StatMsg);
        }
    }
    if(j.KeyExist("ExitTip")) j.Get("ExitTip", ec.ExitTip);
    this->Config = ec;
}

EyConfig ConfigReader::Get(){return this->Config;}

bool ConfigReader::IsExist(string key){
    if(this->Json.KeyExist(key)){
        return true;
    }
    else{
        return false;
    }
}

bool ConfigReader::Read(string path){
    ifstream file(path);
    istreambuf_iterator<char> begin(file);
    istreambuf_iterator<char> end;
    string content(begin, end);
    CJsonObject object(content);
    this->Json = object;
    this->Path = path;
    return true;
}

#include "eystrparser.h"

using namespace eystrpas;

string parse(string text){
    string result;
    for(int i=0; i<text.size(); i++){
        if(text.at(i) == '\\'){
            if(text.at(i + 1) == '\\') result.push_back('\\\\');
            else if(text.at(i + 1) == 'n') result.push_back('\n');
            else{
                throw "expect '\'";
            }
            i++;
        }
        else{
            result.push_back(text.at(i));
        }
    }
    return result;
}
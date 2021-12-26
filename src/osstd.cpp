#include "osstd.h"

void osstd::log(std::string text)
{
    std::cout << text << std::endl;
}

void osstd::log(int text)
{
    std::cout << text << std::endl;
}


void log(std::vector<std::string> sg) {
    for(auto str : sg){
        std::cout << str << std::endl;
    }
}

std::vector<std::string> osstd::split(const std::string &str, const std::string &pattern)
{
    //const char* convert to char*
    char *strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str());
    std::vector<std::string> resultVec;
    char *tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(std::string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }
    delete[] strc;

    return resultVec;
}

template<class T>
T osstd::strToNumber(string str){
    istringstream ss(str);
    T n;
    ss>>n;
    return n;
}

bool osstd::isInt(float num){
    if(num - (int)num == 0)
        return true;
    else return false;
}

string osstd::bool_to_string(bool b){
    ostringstream osm;
    osm << b;
    return string(osm.str());
}
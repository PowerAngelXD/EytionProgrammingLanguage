#include "osstd.h"

void osstd::log(std::string text)
{
    _OC(8);
    std::cout << "[Debug]: " << text << std::endl;
    _OC(7);
}

void osstd::log(int text)
{
    _OC(8);
    std::cout << "[Debug]: " << text << std::endl;
    _OC(7);
}

void log(char const* text){
    _OC(8);
    std::cout << "[Debug]: " << text << std::endl;
    _OC(7);
}

void log(std::vector<std::string> sg) {
    _OC(8);
    for(auto str : sg){
        std::cout << "[Debug]: " << str << std::endl;
    }
    _OC(7);
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
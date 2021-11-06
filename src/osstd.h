#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include <exception>
#include <conio.h>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <functional>
#include <string>
#include <time.h>
#include <thread>
#include <math.h>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <string_view>
#include <regex>
#include <sstream>
#include "../include/CJsonObject.hpp"
using namespace neb;
using namespace std;

#define _FONT_RED "\033[31m"
#define _FONT_GREEN "\033[32m"
#define _FONT_YELLOW "\033[33m"
#define _FONT_BLUE "\033[34m"
#define _FONT_PURPLE "\033[35m"
#define _FONT_WHITE "\033[37m"
#define _BG_RED "\033[41m"
#define _BG_GREEN "\033[42m"
#define _BG_YELLOW "\033[43m"
#define _BG_BLUE "\033[44m"
#define _BG_PURPLE "\033[45m"
#define _BG_WHITE "\033[47m"
#define _NORMAL "\033[0m"

/*
设置文字颜色
附录: 颜色表
1>深蓝
2>浅绿
3>天蓝
4>红色
5>深紫
6>黄色
7>白色
8>灰色
9>比天蓝深一点
10>比浅绿还要浅
16>蓝底黑字
17>纯蓝色的条
14>浅黄色
*/
#define _OC(e) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), e);

/*033Helper
字色              背景              颜色
---------------------------------------
30                40              黑色
31                41              紅色
32                42              綠色
33                43              黃色
34                44              藍色
35                45              紫紅色
36                46              青藍色
37                47              白色

0 终端默认设置（黑底白字）
1 高亮显示
4 使用下划线
5 闪烁
7 反白显示
8 不可见

*/

namespace osstd{
    const std::string ocn_ver = "20210816d9";
    std::vector<std::string> split(const std::string &str, const std::string &pattern);
    template<class T>
    T strToNumber(string str);
    bool isInt(float num);
    string bool_to_string(bool b);
    //输出调试信息
    void log(std::string text);
    void log(int text);
    void log(char const* text);
};

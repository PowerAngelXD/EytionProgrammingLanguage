# Eytion中文开发文档
---
## 1，目录结构
Eytion的目录结构不是特别复杂，以目前版本为例，它的整个目录结构是这样的
```
src
│  eymain.cpp
│  osstd.cpp
│  osstd.h
├─eexception
│      eexcp.cpp
│      eexcp.h
├─ey
│  ├─scope
│  │      eyscope.cpp
│  │      eyscope.h
│  └─value
│          eytype.cpp
│          eytype.h
│          eyvalue.cpp
│          eyvalue.h
├─eycodegen
│      generator.cpp
│      generator.h
├─eyexec
│      executer.cpp
│      executer.h
├─eylexer
│      lex.cpp
│      lex.h
├─eyparser
│      parser.cpp
│      parser.h
└─eysystem
        command.cpp
        command.h
        econfig.cpp
        econfig.h
```
接下来，将目录src下的所有第一级子文件夹和内部文件的作用略述一下  
|文件夹名称|重要子文件|文件夹和重要子文件的作用|
|:---:|:---:|:---:|
|***eysystem***|econfig.* 和 ***command.****|Eytion的shell和配置系统在此实现，econfig是配置文件实现，command是shell实现|
|***eylexer***|lex.*|Eytion的lexer在此实现|
|***eyparser***|parser.*|Eytion的parser在此实现|
|***eycodegen***|generator.*|此处实现Eytion的中间代码生成器|
|***eyexec***|executer.*|Eytion的运行代码系统|
|***eexception***|eexcp.*|Eytion的异常系统|
|***ey***|value/eyvalue.* scope/eyscope.*|Eytion语言内容实现部分，value文件夹内实现变/常量系统，scope实现作用域，代码块系统|
---
## 2，各个目录下重要文件及其类/函数的详解
#### 1，非第一级子文件
1，文件 ``osstd``  
该文件是方便开发过程中所创建的文件，引用了很多的头文件，并且自带一些方便的函数，，它的命名空间为``osstd``，下面教您如何使用它里面的函数和宏：
如果您要引用该库，您应该这样做：
```C++
#influde "src/osstd.h"
using namespace osstd; // 如果为了方便，您可以引用该命名空间
```
下面，介绍几个库中常用的函数/宏
  
```C++
_FONT_XXX
_BG_XXX
```
这种宏运用于控制台字体，背景的颜色控制，‘XXX'部分填写对应的颜色，目前支持的颜色有(对应的XXX部分均大写)：
|颜色|英文名|
|:---:|:---:|
|红色|red|
|绿色|green|
|黄色|yellow|
|蓝色|blue|
|紫色|purple|
|白色|white|  
  
    
```C++
void log(std::string text);
void log(int text);
void log(char const* text);
```
四种输出调试信息函数，可用于输出调试信息，输出字体颜色为灰色  
  
```C++
std::vector<std::string> split(const std::string &str, const std::string &pattern);
```
字符串分割函数，只能用于字符串当中，按照某个字符把整个字符串分割成一个vector；其中参数``pattern``是’需要按照哪种字符进行分割‘  
示例：
```C++
#influde "src/osstd.h"
int main(){
    string str = "1,2,345,6,7,89";
    std::vector<std::string> list = osstd::split(str, ','); // 按照逗号分割
    for(auto s : list){
        cout<<s<<endl;
    }
}
```
如上程序，输出结果应该为：
```
1
2
345
6
7
89
```
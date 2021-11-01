# Eytion Chinese development document
---
## 1，directory structure
The directory structure of eytion is not particularly complex. Take the current version as an example, its entire directory structure is like this
```
src-
   |
   -eexception
     |-eexcp.h
     |-eexcp.cpp
   -ey
     |-scope
      |-eyscope.h
     | -eyscope.cpp
     |-value
      |-eytype.cpp
      |-eytype.h
      |-eyvalue.cpp
      |-eyvalue.h
   -eycodegen
     |-generator.h
     |-generator.cpp
   -eyexec
     |-executer.h
     |-executer.cpp
   -eylexer
     |-lex.h
     |-lex.cpp
   -eyparser
     |-parser.h
     |-parser.cpp
   -eysystem
     |-command.h
     |-command.cpp
     |-econfig.h
     |-econfig.cpp
   -eymain.cpp
   -osstd.cpp
   -osstd.h
```
Next, briefly describe the functions of all the first level subfolders and internal files under the SRC directory
|Folder name|Important sub files|Role of folders and important sub files|
|:---:|:---:|:---:|
|***eysystem***|econfig.* and ***command.****|Eyton's shell and configuration system are implemented here. Econfig is the configuration file implementation and command is the shell implementation|
|***eylexer***|lex.*|The lexer of eytion is implemented here|
|***eyparser***|parser.*|The parser of eytion is implemented here|
|***eycodegen***|generator.*|The intermediate code generator for eytion is implemented here|
|***eyexec***|executer.*|Eyton's running code system|
|***eexception***|eexcp.*|Eyton's abnormal system|
|***ey***|value/eyvalue.* scope/eyscope.*|Eytion language content implementation part, variable / constant system in value folder, scope scope scope and code block system|
---
## 2. Detailed description of important files and their classes / functions in each directory
#### 1. Non first level sub file
1，file ``osstd``  
This file is a file created during the development process. It references a lot of header files and comes with some convenient functions. Its namespace is `` osstd ``. Here is how to use its functions and macros:
```C++
#influde "src/osstd.h"
using namespace osstd; // You can refer to this namespace for convenience
```
Next, several functions / macros commonly used in libraries are introduced
  
```C++
_FONT_XXX
_BG_XXX
```
This macro is used for console font and background color control. Fill in the corresponding color in the 'xxx' part. At present, the supported colors are (the corresponding XXX part is capitalized):
|colour|English name|
|:---:|:---:|
|gules|red|
|green|green|
|yellow|yellow|
|blue|blue|
|purple|purple|
|white|white|  
  
    
```C++
void log(std::string text);
void log(int text);
void log(char const* text);
```
Four functions for outputting debugging information can be used to output debugging information, and the output font color is gray
  
```C++
std::vector<std::string> split(const std::string &str, const std::string &pattern);
```
String segmentation function, which can only be used in strings, divides the whole string into a vector according to a character; Where the parameter `` pattern `` yes' is the character to be divided according to‘
```C++
#influde "src/osstd.h"
int main(){
    string str = "1,2,345,6,7,89";
    std::vector<std::string> list = osstd::split(str, ','); // Separated by commas
    for(auto s : list){
        cout<<s<<endl;
    }
}
```
According to the above procedure, the output result should be:
```
1
2
345
6
7
89
```
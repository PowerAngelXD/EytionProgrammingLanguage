#include "lex.h"
#include <map>

using namespace eylex;

std::string eylex::toString(Symbol symbol) {
    switch (symbol) {
    case Symbol::Eof:
        return "eof";
    case Symbol::None:
        return "none";
    case Symbol::Add:
        return "add";
    case Symbol::Sub:
        return "sub";
    case Symbol::Mul:
        return "mul";
    case Symbol::Div:
        return "div";
    case Symbol::Mod:
        return "mod";
    case Symbol::EndMark:
        return "endMark";
    case Symbol::Number:
        return "number";
    case Symbol::LeftParen:
        return "leftparen";
    case Symbol::RightParen:
        return "rightparen";
    case Symbol::GT:
        return "GT";
    case Symbol::LT:
        return "LT";
    case Symbol::KeyWord:
        return "keyword";
    case Symbol::Identifier:
        return "identifier";
    case Symbol::String:
        return "string";
    default:
        return "unknown";
    }
}

/*lexer原理：根据目前字符来进行转移([a]代表开头字符为a, =>代表转移, <a>代表词性为a, back代表将目前字符放回流内, <>代表空单词, a代表状态a, skip代表跳过该字符)
Start:
    [0]=>OctHex
    [1-9]=>Num
    [a-z|A-Z|_]=>Identifier
    [#]=>Comment
    [\n|\r|\t| ]=>skip
Num:
    [0-9]=>Num
    [Other]=><Number>, back
OctHex:
    [0-7]=>back, Oct
    [x]=>Hex
Oct:
    [0-7]=>Oct
    [Other]=><OctNumber>, back
Hex:
    [0-9|a-f|A-F]=>Hex
    [Other]=><HexNumber>, back
Comment:
    [\n\r]=><>
    [Other]=>skip
Identifier:
    [a-z|A-Z|0-9|_]=>Identifier
    [Other]=><Identifier>, back
*/

string Token::format(){
    return "<" + this->content + ", symbol: " + toString(this->symbol) + ", ln: " + to_string(this->line) + ", col: " + to_string(this->column) + ">";
}

Token Lexer::Num() {
    char ch = get();
    std::string str = "";
    while (((!input.eof()) && isdigit(ch)) || ch == '.') {
        str += ch;
        ch = get();
    }
    put(ch);
    return {str, Symbol::Number, line, column};
}

Token Lexer::Hex() {
    char ch = get();
    std::string str = "";
    while ((! input.eof()) && (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
        str += ch;
        ch = get();
    }
    put(ch);
    return {str, Symbol::HexNumber, line, column};
}

Token Lexer::Oct() {
    char ch = get();
    std::string str = "";
    while ((! input.eof()) && (ch >= '0' && ch <= '7')) {
        str += ch;
        ch = get();
    }
    put(ch);
    return {str, Symbol::OctNumber, line, column};
}

Token Lexer::OctHex() {
    char ch = get();
    if (ch == 'x') {
        Token t = Hex();
        std::string content = "";
        content += ch;
        return {content + t.content, t.symbol, line, column};
    }
    else {
        put(ch);
        return Oct();
    }
}

void Lexer::Comment() {
    char ch = get();
    while (ch != '\n' && ch != '\r' && (! input.eof())) {
        ch = get();
    }
}

Token Lexer::Sign() { //符号
    char ch = get();
    std::string str = "";
    str += ch;
    if (ch == '=') {
        ch = get();
        if (ch == '=')
            str += ch;
        else if (ch == '>')
            str += ch;
        else
            put(ch);
    }
    if (ch == '!') { //以=, !开头，后可接=
        ch = get();
        if (ch == '=')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '&') { //以&开头，后可接&
        ch = get();
        if (ch == '&')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '|') { //以|开头，后可接|
        ch = get();
        if (ch == '|')
            str += ch;
        else
            put(ch);
    }
    else if (ch == '>') {//以>开头，后可接=,>
        ch = get();
        if (ch == '=' || ch == '>')
            str += ch;
        else
            put(ch);
    }
    else if(ch == '<') {//以<开头，后可接=,<
        ch = get();
        if (ch == '=' || ch == '<')
            str += ch;
        else
            put(ch);
    }

    if (str == "=") return {str, Symbol::Assign, line, column};
    else if (str == ">") return {str, Symbol::GT, line, column};
    else if (str == "<") return {str, Symbol::LT, line, column};
    else if (str == "!") return {str, Symbol::Not, line, column};
    else if (str == "&") return {str, Symbol::And, line, column};
    else if (str == "|") return {str, Symbol::Or, line, column};
    else if (str == "==") return {str, Symbol::EQ, line, column};
    else if (str == "=>") return {str, Symbol::Point, line, column};
    else if (str == ">=") return {str, Symbol::GE, line, column};
    else if (str == "<=") return {str, Symbol::LE, line, column};
    else if (str == "!=") return {str, Symbol::NEQ, line, column};
    else if (str == "&&") return {str, Symbol::LogicAnd, line, column};
    else if (str == "||") return {str, Symbol::LogicOr, line, column};
    else if (str == ">>") return {str, Symbol::RightShift, line, column};
    else if (str == "<<") return {str, Symbol::LeftShift, line, column};
    return {};
}

namespace _InnerUtil {
    std::string keywordTable[] = {"false", "true", "null", "out", "input", "if", "while", "var", "delete", "int", "deci", "string", "bool"};
    // 字典树
    struct TrieNode {
        bool isEnd = false;
        std::map<char, TrieNode*> children;
        ~TrieNode() {
            for (auto child : children)
                delete child.second;
            children.clear();
        }
    };
    TrieNode root;
    void init() {
        TrieNode* current;
        for (std::string str : keywordTable) {
            current = &root;
            for (int i = 0 ; i < str.length() ; i ++) {
                current->children[str[i]] = new TrieNode;
                current = current->children[str[i]];
            }
            current->isEnd = true;
        }
    }
    bool match(std::string str) {
        TrieNode* current = &root;
        for (int i = 0 ; i < str.length() ; i ++) {
            if (current->children[str[i]] == NULL) {
                return false;
            }
            current = current->children[str[i]];
        }
        return current->isEnd;
    }
}

Token Lexer::Identifier() {
    char ch = get();
    std::string str = "";
    _InnerUtil::init();
    while (((! input.eof()) && (isalnum(ch) || ch == '_'))) {
        str += ch;
        ch = get();
    }
    put(ch);
    if (_InnerUtil::match(str)) {return {str, Symbol::KeyWord, line, column};}
    return {str, Symbol::Identifier, line, column};
}

Token Lexer::String(){
    char ch = get();
    std::string str = "";
    while (ch != '"'){
        str += ch;
        ch = get();
    }
    return {"\"" + str + "\"", Symbol::String, line, column};
}

Token Lexer::Start() {
    if (input.peek() == EOF) { //以eof开头, 单词为eof
        return {"eof", Symbol::Eof, line, column};
    }
    char ch = get();
    if (ch >= '0' && ch <= '9') { //以0-9开头，必定为10进制数
        put(ch);
        return Num();
    }
    else if (ch == '0') { //以0开头，可能是8进制或16进制数
        Token t = OctHex();
        std::string content = "";
        content += ch;
        return {content + t.content, t.symbol, line, column};
    }
    else if (ch == '\"'){
        //put(ch);
        return String();
    }
    else if (ch == '#') { //以/开头，也许是注释
        ch = get();
        Comment();
        return {};
    }
    else if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') { //以\n, \r, ,\t开头，空白符，跳过
        return {};
    }
    else if (isalpha(ch) || ch == '_') { //以字母或_开头,标识符
        put(ch);
        return Identifier();
    }
    else {
        switch (ch) {
        case '+':
            return {"+", Symbol::Add, line, column};
        case '-':
            return {"-", Symbol::Sub, line, column};
        case '*':
            return {"*", Symbol::Mul, line, column};
        case '/':
            return {"/", Symbol::Div, line, column};
        case '%':
            return {"%", Symbol::Mod, line, column};
        case '{':
            return {"{", Symbol::LeftBrace, line, column};
        case '}':
            return {"}", Symbol::RightBrace, line, column};
        case '[':
            return {"[", Symbol::LeftBracket, line, column};
        case ']':
            return {"]", Symbol::RightBracket, line, column};
        case '(':
            return {"(", Symbol::LeftParen, line, column};
        case ')':
            return {")", Symbol::RightParen, line, column};
        case '^':
            return {"^", Symbol::Xor, line, column};
        case '$':
            return {"$", Symbol::Dollar, line, column};
        case ':':
            return {":", Symbol::Colon, line, column}; //符号
        case ';':
            return {";", Symbol::EndMark, line, column};
        case ',':
            return {";", Symbol::Comma, line, column};
        case '=':
        case '>':
        case '<':
        case '!':
        case '&':
        case '|':
            put(ch); //以= > < ! & |开头，有可能继续接着
            return Sign();
        }
    }
    return {};
}

Lexer::Lexer(std::istream& input) : input(input), line(1), column(0) {
    _InnerUtil::init();
}

char Lexer::get(){
    char ch = input.get();
    if (ch == '\n' || ch == '\r')
        line ++, column = 1;
    column ++;
    return ch;
}

void Lexer::put(char ch) {
    if (ch == '\n' || ch == '\r')
        line --, column = 1;
    column --;
    input.putback(ch);
}

std::vector<Token> Lexer::getTokenGroup(){
    std::vector<Token> tg;
    while (true) {
        Token t = Start(); //Start of token
        if (t.symbol == Symbol::Eof || input.eof())
            break;
        if (t.symbol != Symbol::None)
            tg.push_back(t);
    }
    return tg;
}
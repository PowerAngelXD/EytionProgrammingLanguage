#pragma once

#include "../osstd.h"
using namespace osstd;

namespace eylex{
    enum class Symbol {
        Eof = -1, // end of file
        None, //empty token
        Add, // +
        Sub, // -
        Mul, // *
        Div, // /
        Mod, // %
        LeftParen, // (
        RightParen, // )
        LeftBracket, // [
        RightBracket, // ]
        LeftBrace, // {
        RightBrace, // }
        Assign, // =
        Not, // !
        And, // &
        Or, // |
        Xor, //^
        LogicOr, // ||
        LogicAnd, // &&
        GT, // >
        LT, // <
        EQ, // ==
        NEQ, // !=
        GE, // >=
        LE, // <=
        LeftShift, // <<
        RightShift, // >>
        Comma, //,
        Dot, //.
        Semi, //;
        Colon, // :
        Dollar, //$
        EndMark, // ;
        
        Number, // 123456789
        String,
        OctNumber, // 01234567
        HexNumber, // 0x123456789ABCDEF
        KeyWord, // if
        Identifier // a
    };

    std::string toString(Symbol symbol);

    struct Token {
        std::string content;
        Symbol symbol;
        int line=1;
        int column=0;

        string format();
    };

    class Lexer{
        std::istream& input;
        int line=2;
        int column=1;
        Token Start();
        Token Num();
        Token Oct();
        Token Hex();
        Token OctHex();
        void Comment();
        Token Identifier();
        Token Sign();
        Token String();
        char get();
        void put(char ch);
    public:
        Lexer(std::istream& input);
        std::vector<Token> getTokenGroup();
    };

    //vector<oslex::_Token> makeToken(string content);
};
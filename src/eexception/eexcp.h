#pragma once
#include "../osstd.h"
using namespace osstd;

namespace eexcp{
    class EyparseError : public std::exception{
        string message;
    public:
        EyparseError(string title, string content, int line, int col);
        ~EyparseError();
        virtual const char* what();
    };
}
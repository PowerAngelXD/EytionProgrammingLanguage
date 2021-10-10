#include "eexcp.h"

using namespace eexcp;

EyparseError::EyparseError(string title, string content, int line ,int col){
    message = (string)("EytionError {" + title + "}: " + content + "\nline: " + to_string(line) + "\ncol: " + to_string(col));
}

EyparseError::~EyparseError() throw() {}

const char* EyparseError::what(){
    return message.c_str();
}
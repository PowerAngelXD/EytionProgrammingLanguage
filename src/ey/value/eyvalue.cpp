#include "eyvalue.h"

using namespace eyv;

EyValue::EyValue(int value, bool isconst) : type(eytype::Integer), isconst(isconst), maxlen(1), apoint(0), isarray(false) {int_value = new int(value);}
EyValue::EyValue(float value, bool isconst) : type(eytype::Decimal), isconst(isconst), maxlen(1), apoint(0), isarray(false) {decimal_value = new float(value);}
EyValue::EyValue(std::string value, bool isconst) : type(eytype::String), isconst(isconst), maxlen(1), apoint(0), isarray(false) {string_value = new std::string(value);}
EyValue::EyValue(bool value, bool isconst) : type(eytype::Bool), isconst(isconst), maxlen(1), apoint(0), isarray(false) {bool_value = new bool(value);}

EyValue::EyValue(std::vector<int>* value, int max, bool isconst) : type(eytype::Bool), isconst(isconst), maxlen(max), apoint(0), isarray(true) {array_int_value = value;}
EyValue::EyValue(std::vector<float>* value, int max, bool isconst) : type(eytype::Bool), isconst(isconst), maxlen(max), apoint(0), isarray(true) {array_decimal_value = value;}
EyValue::EyValue(std::vector<std::string>* value, int max, bool isconst) : type(eytype::Bool), isconst(isconst), maxlen(max), apoint(0), isarray(true) {array_string_value = value;}
EyValue::EyValue(std::vector<bool>* value, int max, bool isconst) : type(eytype::Bool), isconst(isconst), maxlen(max), apoint(0), isarray(true) {array_bool_value = value;}

#define chkty(except) if (eytype::except != type) throw "cannot control a not avaliable type value!";

int EyValue::getValueAsInt() {
    chkty(Integer);
    return *int_value;
}
float EyValue::getValueAsDecimal() {
    chkty(Decimal);
    return *decimal_value;
}
std::string EyValue::getValueAsString() {
    chkty(String);
    return *string_value;
}
bool EyValue::getValueAsBool(){
    chkty(Bool);
    return *bool_value;
}

int EyValue::getArrayValueAsInt() {}
float EyValue::getArrayValueAsDecimal() {}
std::string EyValue::getArrayValueAsString() {}
bool EyValue::getArrayValueAsBool() {}

void EyValue::setValue(int value) {
    if(isconst != false){
        chkty(Integer);
        *int_value = value;   
    }
    else{throw "Cannot assign a value to a constant";}
}
void EyValue::setValue(float value) {
    if(isconst != false){
        chkty(Decimal);
        *decimal_value = value;   
    }
    else{throw "Cannot assign a value to a constant";}
}
void EyValue::setValue(std::string value) {
    if(isconst != false){
        chkty(String);
        *string_value = value;   
    }
    else{throw "Cannot assign a value to a constant";}
}
void EyValue::setValue(bool value){
    if(isconst != false){
        chkty(Bool);
        *bool_value = value;   
    }
    else{throw "Cannot assign a value to a constant";}
}

eytype::EybType EyValue::getType() {
    return type;
}

bool EyValue::getConst(){
    return isconst;
}

#undef chkty
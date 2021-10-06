#include "eyvalue.h"

using namespace eyv;

EyValue::EyValue(int value, bool isconst) : type(eytype::Integer), isconst(isconst) {
    int_value = new int(value);
}

EyValue::EyValue(float value, bool isconst) : type(eytype::Decimal), isconst(isconst) {
    decimal_value = new float(value);
}

EyValue::EyValue(std::string value, bool isconst) : type(eytype::String), isconst(isconst) {
    string_value = new std::string(value);
}

//EyValue::~EyValue() {
 //   switch (type) {
//        case eytype::Integer:
 //           delete int_value;
 //           break;
  //      case eytype::Decimal:
  //          delete decimal_value;
 //          break;
  //      case eytype::String:
  //          delete string_value;
  //          break;
  //      default:
   //         delete value;
   //         break;
  //  }
//}

#define chkty(except) if (eytype::except != type) throw "cannot control a not avaliable type value!"

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

eytype::EybType EyValue::getType() {
    return type;
}

bool EyValue::getConst(){
    return isconst;
}

#undef chkty
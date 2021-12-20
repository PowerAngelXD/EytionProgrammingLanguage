#pragma once

#include "../../osstd.h"
#include "eytype.h"
#include "../../eyparser/parser.h"
using namespace osstd;

namespace eyv{
    class EyValue{
        struct {
            void* value = nullptr;
            int* int_value = nullptr;
            float* decimal_value = nullptr;
            std::string* string_value = nullptr;
            bool* bool_value = nullptr;

            vector<int>* array_int_value = nullptr;
            vector<float>* array_decimal_value = nullptr;
            vector<std::string>* array_string_value = nullptr;
            vector<bool>* array_bool_value = nullptr;
        };
        int const maxlen;
        int apoint;    // current array element
        bool isarray = false;
        bool isconst = false;
        eytype::EybType type;
    public:
        EyValue(int value, bool isconst);
        EyValue(float value, bool isconst);
        EyValue(std::string value, bool isconst);
        EyValue(bool value, bool isconst);

        EyValue(vector<int>* value, int max);
        EyValue(vector<float>* value, int max);
        EyValue(vector<std::string>* value, int max);
        EyValue(vector<bool>* value, int max);
        //~EyValue();

        int getValueAsInt();
        float getValueAsDecimal();
        std::string getValueAsString();
        bool getValueAsBool();

        int getArrayValueAsInt();
        float getArrayValueAsDecimal();
        std::string getArrayValueAsString();
        bool getArrayValueAsBool();

        void setValue(int value);
        void setValue(float value);
        void setValue(std::string value);
        void setValue(bool value);

        void setArrayValue(int value);
        void setArrayValue(float value);
        void setArrayValue(std::string value);
        void setArrayValue(bool value);

        void setApoint(int p);
        int getApoint();
        bool getArray();
        bool getConst();

        eytype::EybType getType();
    };

}
#pragma once

#include "../../osstd.h"
#include "eytype.h"
#include "../../eyparser/parser.h"
using namespace osstd;

namespace eyv{
    class EyValue{
        union {
            void* value = nullptr;
            int* int_value;
            float* decimal_value;
            std::string* string_value;
            bool bool_value;
        };
        const bool isconst = false;
        const eytype::EybType type;
    public:
        EyValue(int value, bool isconst);
        EyValue(float value, bool isconst);
        EyValue(std::string value, bool isconst);
        //~EyValue();

        int getValueAsInt();
        float getValueAsDecimal();
        std::string getValueAsString();
        void setValue(int value);
        void setValue(float value);
        void setValue(std::string value);
        bool getConst();

        eytype::EybType getType();
    };

}
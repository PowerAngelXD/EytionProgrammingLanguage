#pragma once

#include "../../osstd.h"

using namespace osstd;

namespace eytype{
    enum EybType{
        Integer = 0,
        Decimal,
        String,
        Bool
    };
    EybType typeMapping(int type);
}
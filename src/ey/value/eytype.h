#pragma once

#include "../../osstd.h"

using namespace osstd;

namespace eytype{
    enum EybType{
        NullType = -1,
        Integer,
        Decimal,
        String,
        Bool
    };
    EybType typeMapping(int type);
}
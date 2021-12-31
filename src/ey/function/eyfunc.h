#pragma once
#include "../../../eydevkit/edk.h"
#include "../scope/eyscope.h"
#include "../../eycodegen/generator.h"
#include "../../eyexec/executer.h"
#include "../../eyparser/parser.h"
#include "../../eyparser/ast.h"

namespace eyfn{
    class Efunction{
    public:
        eycodegen::CodeGenerator codes;
        vector<pair<string, any>> argvs;

        Efunction() = default;
        void run();
    };
}
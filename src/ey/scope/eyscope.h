#pragma once
#include "../../osstd.h"
#include "../../ey/value/eyvalue.h"
using namespace osstd;

namespace eyscope{
    class EyScope{
    public:
        string name; //not very important
        std::vector<std::string> IdenTable;
        std::map<std::string, eyv::EyValue> BoardPool;
        EyScope* Father = nullptr;

        EyScope()=default;
        EyScope(string n);
        ~EyScope()=default;

        bool Find(string n);

    };
    class EyScopeUnit{
    public:
        std::vector<eyscope::EyScope> ScopeStack;
        int now = 0;

        EyScopeUnit();
        ~EyScopeUnit()=default;

        void add(string n = " ");
        void pop();
        void back(int i = 1);
        void next(int i = 1);
        bool findAll(string name);
        int findWhere(string name);
    };
}
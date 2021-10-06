#include "eyscope.h"
using namespace eyscope;

EyScope::EyScope(string n){
    this->name = n;
}

bool EyScope::Find(string n){
    for(auto i : IdenTable){
        if(n == i){
            return true;
        }
    }
    return false;
}


EyScopeUnit::EyScopeUnit(){
    ScopeStack.push_back(EyScope("__global__"));
}

void EyScopeUnit::add(string n){
    ScopeStack.push_back(EyScope(n));
    next();
}

void EyScopeUnit::pop(){
    if(ScopeStack.at(ScopeStack.size() - 1).name == "__global__") return;
    ScopeStack.pop_back();
    back();
}

void EyScopeUnit::back(int i){
    if(now == 0){
        now = 0;
    }
    now -= i;
}

void EyScopeUnit::next(int i){
    now += i;
}

bool EyScopeUnit::findAll(string name){
    int temp = now;
    if(ScopeStack.at(temp).Find(name) == true){
        return true;
    }
    while(temp >= 0){
        if(ScopeStack.at(temp).Find(name) == true){
            return true;
        }
        temp--;
    }
    return false;
}

int EyScopeUnit::findWhere(string name){
    int temp = now;
    if(ScopeStack.at(temp).Find(name) == true){
        return temp;
    }
    while(temp >= 0){
        if(ScopeStack.at(temp).Find(name) == true){
            return temp;
        }
        temp--;
    }
    return temp;
}
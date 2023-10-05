#pragma once

#include <string>
#include <unordered_map>
#include "sval.hpp"

extern Environment global;

class Environment{
    public:
        Environment(Environment* p = nullptr);
        void add(const std::string &symbol, SVal value);
        SVal get(const std::string &symbol);
    protected:
        Environment* parent = nullptr;
        std::unordered_map<std::string, SVal> env;
};

Environment init();
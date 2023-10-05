#pragma once

#include <utility>
#include <string>
#include <variant>
#include <functional>
#include <vector>

class Environment;
struct Closure;

struct Nil{};
struct Err{};

struct SVal{
    SVal();
    SVal(int n);
    SVal(const std::string &s);
    SVal(SVal a, SVal b);
    SVal(std::function<SVal(SVal)> func);
    SVal(Closure *closure);

    // Create an SVal that stores an error code, nothing else.
    static SVal error();

    std::variant<int, std::string, std::pair<SVal, SVal>*, Nil, Err, Environment*, std::function<SVal(SVal)>, Closure*> value;
};

struct Closure{
    Environment* env;
    std::vector<std::string> symbols;
    SVal s;
};

SVal car(SVal s);
SVal cdr(SVal s);
// SVal get_cons(SVal s);
int get_number(SVal s);
std::string get_symbol(SVal s);
std::function<SVal(SVal)> get_builtin(SVal s);
Closure* get_closure(SVal s);
Environment* get_environment(SVal s);

bool is_nil(SVal s);
bool is_cons(SVal s);
bool is_number(SVal s);
bool is_symbol(SVal s);
bool is_error(SVal s);
bool is_list(SVal s);
bool is_closure(SVal s);
bool is_environment(SVal s);
bool is_builtin(SVal s);

SVal symbol(const std::string &s);
SVal number(int num);
SVal nil();
SVal cons(SVal a, SVal b);

std::string to_string(SVal s);
SVal eval(Environment *env, SVal expr);

SVal add(SVal s);
SVal subtract(SVal s);
SVal multiply(SVal s);
SVal divide(SVal s);
SVal less_than(SVal s);
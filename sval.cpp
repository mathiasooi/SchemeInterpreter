#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <iostream>

class Environment;

#include "sval.hpp"
#include "environment.hpp"
#include "gc.hpp"

using namespace std;

// SVal initialziers
SVal::SVal(){
    value = Nil();
}
SVal::SVal(int n){
    value = n;
}
SVal::SVal(const string &s){
    value = s;
}
SVal::SVal(SVal a, SVal b){
    value = new pair<SVal, SVal>(a, b);
    log_cons_alloc(get<pair<SVal, SVal>*>(value));
}
SVal::SVal(function<SVal(SVal)> func){
    value = func;
}
SVal::SVal(Closure *closure){
    value = closure;
}

SVal SVal::error(){
    SVal s;
    s.value = Err();
    return s;
}
// SVal Getters
SVal car(SVal s) {return get<pair<SVal, SVal>*>(s.value)->first;}
SVal cdr(SVal s) {return get<pair<SVal, SVal>*>(s.value)->second;}
// SVal get_cons(SVal s) {return get<pair<SVal, SVal>*>(s.value);}
int get_number(SVal s) {return get<int>(s.value);}
string get_symbol(SVal s) {return get<string>(s.value);}
function<SVal(SVal)> get_builtin(SVal s) {return get<function<SVal(SVal)>>(s.value);}
Closure* get_closure(SVal s) {return get<Closure*>(s.value);}
Environment* get_environment(SVal s) {return get<Environment*>(s.value);}


// SVal type checker
bool is_nil(SVal s) {return holds_alternative<Nil>(s.value);}
bool is_number(SVal s) {return holds_alternative<int>(s.value);}
bool is_symbol(SVal s) {return holds_alternative<string>(s.value);}
bool is_cons(SVal s) {return holds_alternative<pair<SVal, SVal>*>(s.value);}
bool is_error(SVal s) {return holds_alternative<Err>(s.value);}
bool is_list(SVal s) {
    if (is_number(s) || is_symbol(s) || is_error(s)) return false;
    if (is_nil(s)) return true;
    return is_list(cdr(s));
}
bool is_builtin(SVal s) {return holds_alternative<function<SVal(SVal)>>(s.value);}
bool is_environment(SVal s) {return holds_alternative<Environment*>(s.value);}
bool is_closure(SVal s) {return holds_alternative<Closure*>(s.value);}


// Create SVal
SVal symbol(const string &s) {return SVal(s);}
SVal number(int num) {return SVal(num);}
SVal nil() {return SVal();}
SVal cons(SVal a, SVal b) {return SVal(a, b);}

// SVal -> String
string to_string(SVal s){
    ostringstream oss;
    if (is_list(s)) {
        oss << "(";
        string sep = "";
        while (is_cons(s)) {
            oss << sep + to_string(car(s));
            s = cdr(s);
            sep = " ";
        }
        oss << ")";
    }
    else if (is_nil(s)) oss << "()";
    else if (is_number(s)) oss << get_number(s);
    else if (is_symbol(s)) oss << get_symbol(s);
    else if (is_cons(s)) oss << "(" << to_string(car(s)) << "." << to_string(cdr(s)) << ")";
    else if(is_error(s)) oss << "error";
    return oss.str();
}

// Evaluate SVal
SVal eval(Environment *env, SVal expr){
    if (is_number(expr) || is_nil(expr)) return expr;
    else if (is_symbol(expr)) return env->get(get_symbol(expr));
    else if (is_list(expr)){
        SVal op = car(expr); 
        SVal operands = cdr(expr);
        if (is_symbol(op)){ 
            string sym = get_symbol(op);
            // special forms
            if (sym == "if"){
                // (if conditional then else)
                SVal sconditional = car(operands); operands = cdr(operands);
                SVal sthen = car(operands); operands = cdr(operands);
                SVal selse = car(operands); operands = cdr(operands);
                // cout << to_string(eval(env, sconditional));
                return is_nil(eval(env, sconditional)) ? eval(env, selse) : eval(env, sthen);
            }
            else if (sym == "define"){
                // (define var expr)
                env->add(get_symbol(car(operands)), eval(env, car(cdr(operands))));
                return SVal();
            }
            else if (sym == "lambda"){
                // (lambda (params) (body))
                vector<string> symbols;
                SVal params = car(operands); operands = cdr(operands);
                SVal body = car(operands);
                while (is_cons(params)){
                    symbols.push_back(get_symbol(car(params)));
                    params = cdr(params);
                }
                SVal temp = SVal(new Closure{env, symbols, body});
                log_closure_alloc(get_closure(temp));
                return temp;

            }
            else {
                SVal func = env->get(sym);
                if (is_builtin(func)){
                    vector<SVal> v;
                    while (!is_nil(operands)){
                        v.push_back(eval(env, car(operands)));
                        operands = cdr(operands);
                    }
                    SVal args = SVal();
                    for (int i = v.size() - 1; i >= 0; --i){
                        args = cons(v[i], args);
                    }
                    // cout << "AA" << to_string(get_builtin(func)(args)) << endl;
                    return get_builtin(func)(args);
                }

                Closure *closure = get_closure(func);
                Environment* child = new Environment(closure->env);
                log_env_alloc(child);
                for (string symbol : closure->symbols){
                    child->add(symbol, eval(env, car(operands)));
                    operands = cdr(operands);
                }
                return eval(child, closure->s);
            }
        }
    }
    return SVal();
}

// Builtin Functions
SVal add(SVal s){
    int sum = 0;
    while (!is_nil(s)){
        sum += get_number(car(s));
        s = cdr(s);
    }
    return sum;
};
SVal subtract(SVal s){
    int diff = get_number(car(s));
    if (is_nil(cdr(s))) return -diff;
    s = cdr(s);
    while (!is_nil(s)){
        diff -= get_number(car(s));
        s = cdr(s);
    }
    return diff;

}
SVal multiply(SVal s){
    int prod = 1;
    while (!is_nil(s)){
        prod *= get_number(car(s));
        s = cdr(s);
    }
    return prod;
}
SVal divide(SVal s){
    int div = get_number(car(s));
    s = cdr(s);
    while (!is_nil(s)){
        div /= get_number(car(s));
        s = cdr(s);
    }
    return div;
}
SVal less_than(SVal s){
    SVal a = car(s);
    SVal b = car(cdr(s));
    // cout << to_string(a) << " " << to_string(b);
    return get_number(a) < get_number(b) ? SVal(0) : SVal();
}
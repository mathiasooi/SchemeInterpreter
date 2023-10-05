#include <string>
#include <deque>
#include <vector>

#include <iostream>

#include "sval.hpp"

using namespace std;

bool string_is_number(string s){
    if (s.size() == 1 && (s == "+" || s == "-")) return false;
    if (!(isdigit(s.front()) || s.front() == '-' || s.front() == '+')) return false;
    for (char c : s.substr(1)) if (!(isdigit(c))) return false;
    return true;
}
deque<string> tokenize(string input){
    deque<string> v;
    string s;

    for (char c : input){
        if (c == '(') v.push_back("(");
        else if (c == ')'){
            if (!s.empty()) v.push_back(s); s.clear();
            v.push_back(")");
        }
        else if (c == ' ' && !s.empty()) {v.push_back(s); s.clear();}
        else if (c != ' ') s.push_back(c);
    }
    
    v.push_back(s);
    return v;
}

SVal parse(deque<string> &tokens){
    string token = tokens.front(); tokens.pop_front();
    if (token == "("){
        vector<SVal> temp; 
        // Recursively build SVal at the current depth
        while (tokens.front() != ")") temp.push_back(parse(tokens));
        tokens.pop_front();

        // Iterate over all SVal (backwards) at current depth and chain them together
        if (temp.size() == 0) return SVal();
        SVal s(temp.back(), SVal());
        for (int i = temp.size() - 2; i >= 0; --i) {s = cons(temp[i], s);}

        return s;

    }
    else return string_is_number(token) ? SVal(stoi(token)) : SVal(token);

}

SVal parse(string input){
    deque<string> tokens = tokenize(input);
    return parse(tokens);
}
#include <iostream>
#include <string>
#include <vector>

#include "sval.hpp"
#include "parser.hpp"
#include "environment.hpp"
#include "gc.hpp"

using namespace std;

int main(){
    global = init();

    // Test
    vector<string> tests = {"3", "x", "()", "(+ 3 4)", "(define x 4)", "x", "(+ x x)", "(if 3 4 5)", "(if () 4 5)", "(define abs (lambda (x) (if (< x 0) (- x) x)))", "(abs 3)", "(abs -3)"};
    for (string test : tests){
        cout << test << endl << "   -> " << to_string(eval(&global, parse(test))) << endl;
        cout << "   Pointers: " << get_cons_alloc().size() + get_closure_alloc().size() + get_env_alloc().size() << endl;
    }

    //REPL
    string input;
    while (true){
        getline(cin, input);
        cout << "   -> " << to_string(eval(&global, parse(input))) << endl;
        cout << "   Pointers: " << get_cons_alloc().size() + get_closure_alloc().size() + get_env_alloc().size() << endl;
    }
}
    
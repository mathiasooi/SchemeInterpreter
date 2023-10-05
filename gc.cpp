#include <unordered_set>

#include "gc.hpp"

using namespace std;

unordered_set<pair<SVal, SVal>*> cons_alloc;
unordered_set<Closure*> closure_alloc;
unordered_set<Environment*> env_alloc;

void log_cons_alloc(pair<SVal, SVal>* cons) {cons_alloc.insert(cons);}
const std::unordered_set<std::pair<SVal, SVal>*>& get_cons_alloc() {return cons_alloc;}
void del_cons_alloc(std::pair<SVal, SVal>* cons) {cons_alloc.erase(cons); delete cons;}

void log_closure_alloc(Closure* closure) {closure_alloc.insert(closure);}
const std::unordered_set<Closure*>& get_closure_alloc() {return closure_alloc;}
void del_closure_alloc(Closure* closure) {closure_alloc.erase(closure); delete closure;}

void log_env_alloc(Environment* env) {env_alloc.insert(env);}
const std::unordered_set<Environment*>& get_env_alloc() {return env_alloc;}
void del_env_alloc(Environment* env) {env_alloc.erase(env); delete env;}
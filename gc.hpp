#include <iostream>
#include <unordered_set>

#include "sval.hpp"
#include "environment.hpp"

extern std::unordered_set<std::pair<SVal, SVal>*> cons_alloc;
extern std::unordered_set<Closure*> closure_alloc;
extern std::unordered_set<Environment*> env_alloc;

void log_cons_alloc(std::pair<SVal, SVal>* cons);
const std::unordered_set<std::pair<SVal, SVal>*>& get_cons_alloc();
void del_cons_alloc(std::pair<SVal, SVal>* cons);

void log_closure_alloc(Closure* closure);
const std::unordered_set<Closure*>& get_closure_alloc();
void del_closure_alloc(Closure* closure);

void log_env_alloc(Environment* env);
const std::unordered_set<Environment*>& get_env_alloc();
void del_env_alloc(Environment* env);
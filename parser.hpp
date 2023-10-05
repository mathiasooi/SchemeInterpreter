#pragma once

#include <string>
#include <deque>

#include "sval.hpp"

// Helper functions for parsing
bool string_is_number(std::string s);
std::deque<std::string> tokenize(std::string input);

// Parser
SVal parse(std::deque<std::string> &tokens);
SVal parse(std::string s);

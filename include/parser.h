#pragma once

#include "defs.h"
#include <sstream>
#include <cctype>

class ArgumentParser {
public:
    size_t row_number = 100;
    size_t value_range = 1000;
    std::vector<Predicate> predicates;

    bool parse(int argc, char* argv[]);
private:
    bool parsePredicates(const std::string& predicate_string);
    bool parseNamedArgument(const std::string& arg, const std::string& name, size_t& out_value);
    bool isNumber(const std::string& str) const;
};
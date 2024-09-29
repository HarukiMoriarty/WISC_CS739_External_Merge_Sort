#include "parser.h"

bool ArgumentParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--row_number=") == 0) {
            if (!parseNamedArgument(arg, "--row_number=", row_number)) {
                printf("Error: row_number must be a valid number.\n");
                return false;
            }
        }
        else if (arg.find("--predicates=") == 0) {
            if (!parsePredicates(arg.substr(13))) {
                return false;
            }
        }
        else {
            printf("Unknown argument: %s\n", arg.c_str());
            return false;
        }
    }

    // Ensure required parameters are provided
    if (row_number == 0) {
        printf("Error: row_number can not be zero.\n");
        return false;
    }

    return true;
}

bool ArgumentParser::parsePredicates(const std::string& predicate_string) {
    std::stringstream ss(predicate_string);
    std::string pair;

    // Split by commas to get index:value pairs
    while (std::getline(ss, pair, ',')) {
        size_t delimiter_pos = pair.find(':');
        if (delimiter_pos == std::string::npos) {
            printf("Error: Predicates must be in index:value format.\n");
            return false;
        }
        if (!isNumber(pair.substr(0, delimiter_pos)) || !isNumber(pair.substr(delimiter_pos + 1))) {
            printf("Error: Both index and value in predicates must be valid numbers.\n");
            return false;
        }
        size_t index = std::stoul(pair.substr(0, delimiter_pos));
        size_t value = std::stoul(pair.substr(delimiter_pos + 1));
        predicates.emplace_back(index, value);
    }
    return true;
}

bool ArgumentParser::parseNamedArgument(const std::string& arg, const std::string& name, size_t& out_value) {
    std::string value_str = arg.substr(name.length());
    if (!isNumber(value_str)) {
        return false;
    }
    out_value = std::stoul(value_str);
    return true;
}

bool ArgumentParser::isNumber(const std::string& str) const {
    for (char const& c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}
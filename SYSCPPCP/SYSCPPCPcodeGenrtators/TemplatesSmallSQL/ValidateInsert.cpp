#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>


std::string toUpperCase(const std::string& str);

extern std::vector<std::vector<std::string>> vecOfVecs;

bool ValidateInsert(
    std::vector<std::string>& tokens,
    std::vector<std::string> &tables,
    std::vector<std::string>& columns,
    std::vector<std::string>& values)
{
    // Extract table name and validate it
    std::string tableName = tokens[2];
    tables.clear();
    for (const auto& vec : vecOfVecs) {
        if (vec[0] == tableName) {
            tables.push_back(vec[0]);
            break;
        }
    }
    if (tables.empty()) {
        std::cerr << "Invalid INSERT statement: Table does not exist." << std::endl;
        return false;
    }

    // Check if the next token is "(" to denote the start of column list
    size_t i = 3;
    if (tokens[i] != "(") {
        std::cerr << "Syntax Error: Invalid INSERT statement: Missing opening parenthesis for columns." << std::endl;
        return false;
    }
    ++i;

    // Extract column names
    while (i < tokens.size() && tokens[i] != ")") {
        columns.push_back(tokens[i]);
        ++i;
        if (i < tokens.size() && tokens[i] == ",") {
            ++i; // Skip the comma
        }
    }

    // Check if closing parenthesis is present
    if (i >= tokens.size() || tokens[i] != ")") {
        std::cerr << "Invalid INSERT statement: Missing closing parenthesis for columns." << std::endl;
        return false;
    }
    ++i;

    // Check if the next token is "VALUES"
    if (i >= tokens.size() || toUpperCase(tokens[i]) != "VALUES") {
        std::cerr << "Invalid INSERT statement: Missing VALUES keyword." << std::endl;
        return false;
    }
    ++i;

    // Check if the next token is "(" to denote the start of values list
    if (i >= tokens.size() || tokens[i] != "(") {
        std::cerr << "Invalid INSERT statement: Missing opening parenthesis for values." << std::endl;
        return false;
    }
    ++i;

    // Extract values
    while (i < tokens.size() && tokens[i] != ")") {
        values.push_back(tokens[i]);
        ++i;
        if (i < tokens.size() && tokens[i] == ",") {
            ++i; // Skip the comma
        }
    }

    // Check if closing parenthesis is present
    if (i >= tokens.size() || tokens[i] != ")") {
        std::cerr << "Invalid INSERT statement: Missing closing parenthesis for values." << std::endl;
        return false;
    }

    // Check if the number of columns matches the number of values
    if (columns.size() != values.size()) {
        std::cerr << "Invalid INSERT statement: Mismatch between columns and values." << std::endl;
        return false;
    }
    
    auto wherePos = std::find_if(tokens.begin(), tokens.end(), [](const std::string& token) {
        return toUpperCase(token) == "WHERE";  // Only apply case-insensitive comparison to keywords
        });

    return true;
}

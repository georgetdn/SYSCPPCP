#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>

// External declaration for the vector of vectors containing table data
extern std::vector<std::vector<std::string>> vecOfVecs;

bool ValidateWHERE(std::vector<std::string>& tokens,
    std::vector<std::vector<std::string>>& where,
    std::vector<std::string>::iterator wherePos);

std::string toUpperCase(const std::string& str);


bool ValidateUpdate(
    std::vector<std::string>& tokens,
    std::vector<std::string> &tables,
    std::vector<std::vector<std::string>>& columnsAndValues,
    std::vector<std::vector<std::string>>& where)
{
    // Extract table name and validate it
    std::string tableName = tokens[1];
    tables.clear();
    for (const auto& vec : vecOfVecs) {
        if (vec[0] == tableName) {
            tables.push_back(vec[0]);
            break;
        }
    }
    if (tables.empty()) {
        std::cerr << "Invalid UPDATE statement: Table does not exist." << std::endl;
        return false;
    }


    // Validate column-"value" pairs
    size_t i = 3;
    std::vector<std::string> tableColumns;
    for (const auto& vec : vecOfVecs) {
        if (vec[0] == tableName) {
            tableColumns = std::vector<std::string>(vec.begin() + 1, vec.end());
            break;
        }
    }
    std::vector<std::string> columnValuePair;
    while (i < tokens.size() && toUpperCase(tokens[i]) != "WHERE") {
        std::string column = tokens[i];

        // Check if the column exists in the table
        if (std::find(tableColumns.begin(), tableColumns.end(), column) == tableColumns.end()) {
            std::cerr << "Invalid UPDATE statement: Column " << column << " does not exist in table " << tableName << "." << std::endl;
            return false;
        }

        // Check if the next token is "="
        if (i + 1 >= tokens.size() || tokens[i + 1] != "=") {
            std::cerr << "Invalid UPDATE statement: Missing '=' after column " << column << "." << std::endl;
            return false;
        }

        // Get the "value" to assign to the column
        if (i + 2 >= tokens.size()) {
            std::cerr << "Invalid UPDATE statement: Missing value for column " << column << "." << std::endl;
            return false;
        }
        std::string value = tokens[i + 2];

        // Add the column-"value" pair to the vector
        columnValuePair.push_back(column);
        columnValuePair.push_back(value);
        columnsAndValues.push_back(columnValuePair);
        columnValuePair.clear();

        // Move to the next column-"value" pair
        i += 3;

        // Check if there is a comma and move to the next pair
        if (i < tokens.size() && tokens[i] == ",") {
            ++i;
        }
    }

    // Validate "WHERE" clause if present
    if (i < tokens.size() && tokens[i] == "WHERE") {
        ++i;
        std::vector<std::string> condition;
        while (i < tokens.size()) {
            condition.push_back(tokens[i]);
            ++i;
        }
        where.push_back(condition);
    }

    auto wherePos = std::find_if(tokens.begin(), tokens.end(), [](const std::string& token) {
        return toUpperCase(token) == "WHERE";  // Only apply case-insensitive comparison to keywords
        });
    //Get the next element after WHERE

    if (wherePos != tokens.end()) 
    {
        auto nextPos = std::next(wherePos);
        return ValidateWHERE(tokens, where, wherePos);
    }
    return true;
}


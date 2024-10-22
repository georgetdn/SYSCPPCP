#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>


// External declaration for the vector of vectors containing table data
extern std::vector<std::vector<std::string>> vecOfVecs;
std::string toUpperCase(const std::string& str);
bool ValidateWHERE(std::vector<std::string>& tokens,
    std::vector<std::vector<std::string>>& where,
    std::vector<std::string>::iterator wherePos);

// Function to validate conditions 
bool ValidateCondition(std::vector<std::string>& tokens, size_t startIndex);

bool ValidateDelete(std::vector<std::string>& tokens,
    std::unordered_map<std::string, std::vector<std::string>>& tables,
    std::vector<std::string>& columns,
    std::vector<std::vector<std::string>>& where)
{
    // Find "FROM" (case-insensitive keyword comparison)
    auto fromPos = std::find_if(tokens.begin(), tokens.end(), [](const std::string& token) {
        return toUpperCase(token) == "FROM";  // Only apply case-insensitive comparison to keywords
        });

    if (fromPos == tokens.end() || std::next(fromPos) == tokens.end()) {
        std::cerr << "Syntax Error: DELETE expects 'FROM class(table) name' after the columns." << std::endl;
        return false;
    }

    // Parse the tables from the "FROM" clause

    auto tableStartPos = std::next(fromPos);
    auto wherePos = std::find_if(tableStartPos, tokens.end(), [](const std::string& token) {
        return toUpperCase(token) == "WHERE";  // Only apply case-insensitive comparison to keywords
        });

    // Ensure there's no trailing comma after the last table name
    bool tableFound = false;
    for (auto it = tableStartPos; it != wherePos; ++it) {
        std::string tableName = *it;
        if (tableFound)
        {
            std::cout << std::endl << std::endl << "You are not allowed to delete from multiple classes(tables).\n";
            return false;
        }
        tableFound = true;
        // Check for trailing comma in table name
        if (!tableName.empty() && tableName.back() == ',') {
            // Remove trailing comma and check if it is the last table in the list
            tableName.pop_back();
            if (std::next(it) == wherePos || tableName.empty()) {
                std::cerr << std::endl << "Syntax Error: Unexpected comma after class(table) '" << tableName << "'." << std::endl << std::endl;
                return false;
            }
        }

        // Find the corresponding table in vecOfVecs
        auto tableVec = std::find_if(vecOfVecs.begin(), vecOfVecs.end(), [&](const std::vector<std::string>& vec) {
            return vec[0] == tableName; // Case-sensitive comparison
            });

        if (tableVec == vecOfVecs.end()) {
            std::cerr << std::endl << "Syntax Error: Class(table) '" << tableName << "' does not exist." << std::endl << std::endl;
            return false;
        }

        // Add the table name and its columns to the tables map
        tables[tableName] = std::vector<std::string>(tableVec->begin() + 1, tableVec->end());
    }
    return ValidateWHERE(tokens,where, wherePos);

  
}

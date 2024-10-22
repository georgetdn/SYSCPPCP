#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>

// External declaration for the vector of vectors containing table data
extern std::vector<std::vector<std::string>> vecOfVecs;
std::string toUpperCase(const std::string& str);


// Function to validate conditions 
bool ValidateCondition( std::vector<std::string>& tokens, size_t startIndex);


// Function to split the "table.column" format
std::pair<std::string, std::string> splitTableColumn(const std::string& tableColumn) 
 {
    size_t dotPos = tableColumn.rfind('.');
    if (dotPos != std::string::npos) {
        return { tableColumn.substr(0, dotPos), tableColumn.substr(dotPos + 1) };
    }
    return { "", tableColumn };  // No table prefix, return empty string as table name
}

// Function to validate column names against given tables and columns
static bool validateColumns( std::unordered_map<std::string, std::vector<std::string>>& tables, std::vector<std::string>& columns) {
    for (auto& column : columns) {
        // Check if the string is not empty and ends with a comma
        if (!column.empty() && column.back() == ',') {
            // Remove the last character (comma) using pop_back
            column.pop_back();
        }

 /*       // Split table.column
        auto [tableName, colName] = splitTableColumn(column);

        if (!tableName.empty()) {  // If table is specified with the column
            if (tables.find(tableName) == tables.end()) {
                std::cerr << "Syntax Error: Table '" << tableName << "' does not exist." << std::endl;
                return false;
            }
            if (std::find(tables.at(tableName).begin(), tables.at(tableName).end(), colName) == tables.at(tableName).end()) {
                std::cerr << "Syntax Error: Column '" << colName << "' does not exist in table '" << tableName << "'." << std::endl;
                return false;
            }
        }
        else 
 */
        {  // If no table is specified, check all tables for the column
            bool columnFound = false;
            for (const auto& [table, cols] : tables) {
                if (std::find(cols.begin(), cols.end(), column) != cols.end()) {
                    columnFound = true;
                    break;
                }
            }
            if (!columnFound) {
                std::cerr << std::endl <<"Syntax Error: Column(variable) '" << column << "' does not exist in any table(class)." << std::endl;
                std::cerr  << "Colomn(variable) names are case sensitive." << std::endl;
                std::cerr << "When selecting from dual, please use all lowercases ." << std::endl << std::endl;
                return false;
            }
        }
    }

    return true;  // All columns are valid
}


// External declaration for the vector of vectors containing table data
extern std::vector<std::vector<std::string>> vecOfVecs;

bool ValidateSelect( std::vector<std::string>& tokens,
                        std::unordered_map<std::string, std::vector<std::string>> &tables,
                            std::vector<std::string> &columns,
                             std::vector<std::vector<std::string>>& where)
{
    if (tokens.size() < 4) {
        std::cerr << "Syntax Error: SELECT expects syntax like 'SELECT col FROM class(table) name'." << std::endl;
        return false;
    }

    // Find "FROM" (case-insensitive keyword comparison)
    auto fromPos = std::find_if(tokens.begin(), tokens.end(), [](const std::string& token) {
        return toUpperCase(token) == "FROM";  // Only apply case-insensitive comparison to keywords
        });

    if (fromPos == tokens.end() || std::next(fromPos) == tokens.end()) {
        std::cerr << "Syntax Error: SELECT expects 'FROM table_name' after the columns." << std::endl;
        return false;
    }

    // Collect columns
    
    for (auto it = tokens.begin() + 1; it != fromPos; ++it) {
        std::string columnName = *it;

        // Check for trailing comma in column name
        if (!columnName.empty() && columnName.back() == ',') {
            columnName.pop_back(); // Remove trailing comma

            // Check if the next token is another valid column name
            if (std::next(it) == fromPos || columnName.empty()) {
                std::cerr << std::endl << "Syntax Error: Unexpected comma after column (variable) '" << columnName << "'." << std::endl << std::endl;
                return false;
            }
        }

        if (!columnName.empty()) {
            columns.push_back(columnName);
        }
    }

    if (columns.empty()) {
        std::cerr << std::endl << "Syntax Error: No columns specified for selection." << std::endl << std::endl;
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
            std::cout << std::endl << std::endl << "Only one table(class) can be used in the SELECT statement.\n"
                "Multiple classes(tables) SELECT will be inplemented in future releases.\n" << std::endl << std::endl;
            return false;
        }
        tableFound = true;
        // Check for trailing comma in table name
        if (!tableName.empty() && tableName.back() == ',') {
            // Remove trailing comma and check if it is the last table in the list
            tableName.pop_back();
            if (std::next(it) == wherePos || tableName.empty()) {
                std::cerr << std::endl << "Syntax Error: Unexpected comma after table '" << tableName << "'." << std::endl << std::endl;
                return false;
            }
        }

        // Find the corresponding table in vecOfVecs
        auto tableVec = std::find_if(vecOfVecs.begin(), vecOfVecs.end(), [&](const std::vector<std::string>& vec) {
            return vec[0] == tableName; // Case-sensitive comparison
            });

        if (tableVec == vecOfVecs.end()) {
            std::cerr << std::endl  << "Syntax Error: Table '" << tableName << "' does not exist." << std::endl << std::endl;
            return false;
        }

        // Add the table name and its columns to the tables map
        tables[tableName] = std::vector<std::string>(tableVec->begin() + 1, tableVec->end());
    }

    // Validate table names and columns (case-sensitive validation)
    if (!validateColumns(tables, columns)) {
        return false;  // Invalid table or columns
    }
 
    // Validate WHERE clause (optional, case-insensitive keyword comparison)
    if (wherePos != tokens.end()) {
        // Check if there are enough tokens after WHERE for a valid condition
        if (std::distance(wherePos, tokens.end()) < 4) {
            std::cerr << std::endl  << "Syntax Error: 'WHERE' must be followed by a valid condition." << std::endl << std::endl;
            return false;
        }

        // Validate the first condition
        size_t startIndex = std::distance(tokens.begin(), wherePos) + 1;
         where.push_back({ tokens[startIndex], tokens[startIndex + 1], tokens[startIndex + 2] });
       if (!ValidateCondition(tokens, std::distance(tokens.begin(), wherePos) + 1)) {
            return false;
        }


        // Handle additional conditions (e.g., AND, OR)
        auto currentPos = wherePos + 4;  // Move to the next token after the first condition (3 tokens for condition + 1 for AND/OR)
        int idx = 0;
        while (currentPos < tokens.end()) {
            std::string logicalOperator = toUpperCase(*currentPos);  // Case-insensitive comparison for logical operators
            if (logicalOperator != "AND" && logicalOperator != "OR") {
                std::cerr << std::endl << "Syntax Error: Expected 'AND' or 'OR' between conditions." << std::endl << std::endl;
                return false;
            }
            where[idx++].push_back(logicalOperator);
            // Validate the next condition
            if (!ValidateCondition(tokens, std::distance(tokens.begin(), currentPos) + 1)) {
                return false;
            }
            startIndex = std::distance(tokens.begin(), currentPos) + 1;
            where.push_back({ tokens[startIndex], tokens[startIndex + 1], tokens[startIndex + 2] });
            currentPos += 4;  // Move to the next potential logical operator
        }
    }
    return true;  // Command parsed successfully
}

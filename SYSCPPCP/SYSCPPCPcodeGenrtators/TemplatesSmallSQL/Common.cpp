#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <conio.h>  // for _getch()
#include <iomanip>  // For std::setprecision

extern std::vector<std::vector<std::string>> vecOfVecs;
extern std::vector<std::vector<std::string>> vecOfDesc;


std::vector<std::string> split(std::string& s);
void InitializeVector(std::vector<std::vector<std::string>>& vecOfVecs);
void InitializeDesc(std::vector<std::vector<std::string>>& vecOfDesc);
std::string LoadHelp(void);

bool ValidateSelect(std::vector<std::string>& tokens,
    std::unordered_map<std::string, std::vector<std::string>>& tables,
    std::vector<std::string>& columns,
    std::vector<std::vector<std::string>>& where);

bool ProcessSelect(std::vector<std::string>& tokens,
    std::unordered_map<std::string, std::vector<std::string>>& tables,
    std::vector<std::string>& columns,
    std::vector<std::vector<std::string>>& where);

bool ValidateInsert(
    std::vector<std::string>& tokens,
    std::vector<std::string> &tables,
    std::vector<std::string>& columns,
    std::vector<std::string>& values);

bool ProcessInsert(
    std::vector<std::string>& tokens,
    std::vector<std::string> &tables,
    std::vector<std::string>& columns,
    std::vector<std::string>& values);

bool ValidateUpdate(
    std::vector<std::string>& tokens,
    std::vector<std::string> &tables,
    std::vector<std::vector<std::string>>& columnsAndValues,
    std::vector<std::vector<std::string>>& where);

bool ProcessUpdate(
    std::vector<std::string> tokens,
    std::vector<std::string> tables,
    std::vector<std::vector<std::string>> columnValuePair,
    std::vector<std::vector<std::string>> where);

bool ValidateDelete(std::vector<std::string>& tokens,
    std::unordered_map<std::string, std::vector<std::string>>& tables,
    std::vector<std::string>& columns,
    std::vector<std::vector<std::string>>& where);

bool ProcessDelete(std::vector<std::string>& tokens,
    std::unordered_map<std::string, std::vector<std::string>>& tables,
    std::vector<std::vector<std::string>>& where);

// Function to validate condition syntax (e.g., `col = val`)
bool ValidateCondition( std::vector<std::string>& tokens, size_t startIndex) {
    // Ensure there are at least 3 tokens for a valid condition: column, operator, and value
    if (startIndex + 2 >= tokens.size()) {
        std::cerr << "Syntax Error: Incomplete condition in WHERE clause." << std::endl;
        return false;
    }

    std::string conditionColumn   = tokens[startIndex];
    std::string conditionOperator = tokens[startIndex + 1];
    std::string conditionValue    = tokens[startIndex + 2];

    // Simple check for operator (only '=' for now, can be extended)
    if (!(conditionOperator == "=" || conditionOperator == "!=" || conditionOperator == "<" || conditionOperator == ">"
                                                       || conditionOperator == "<=" || conditionOperator == ">"))
    {
        std::cerr << "Syntax Error: Unsupported operator in WHERE clause." << std::endl;
        return false;
    }

    // Ensure the value is non-empty
    if (conditionValue.empty()) {
        std::cerr << "Syntax Error: Missing value in condition." << std::endl;
        return false;
    }

    return true;
}
// Formatting the float with three decimal places
std::string formatFloat(const std::string& val) {
    float fValue = std::stof(val);  // Convert string to float

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << fValue;  // Format to 3 decimal places

    return stream.str();  // Return the formatted string
}
// Formatting the float with three decimal places
std::string formatDouble(const std::string& val) {
    double fValue = std::stod(val);  // Convert string to float

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << fValue;  // Format to 3 decimal places

    return stream.str();  // Return the formatted string
}
// Formatting the float with three decimal places
std::string formatLongdouble(const std::string& val) {
    long double fValue = std::stold(val);  // Convert string to float

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << fValue;  // Format to 3 decimal places

    return stream.str();  // Return the formatted string
}
// Function to convert a string to uppercase (for keyword comparison)
std::string toUpperCase(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
        [](unsigned char c) { return std::toupper(c); });

    return upperStr;
}


// Function to split the input string into tokens (commands)
std::vector<std::string> split(std::string& s) 
{
    std::vector<std::string> tokens;
    std::string token;
    bool insideQuotes = false;
    
    // Add space after '(' and before ')'
    size_t pos = 0;
    while ((pos = s.find('(', pos)) != std::string::npos) {
        s.insert(pos + 1, " ");
        pos += 2; // Move past the inserted space
    }

    pos = 0;
    while ((pos = s.find(')', pos)) != std::string::npos) {
        s.insert(pos, " ");
        pos += 2; // Move past the inserted space
    }

    for (size_t i = 0; i < s.length(); ++i) {
        char currentChar = s[i];

        // Handle quoted strings
        if (currentChar == '"') {
            // Toggle the insideQuotes flag
            insideQuotes = !insideQuotes;

            // Do not include quotes in the token
        }
        else if (insideQuotes) {
            // If inside quotes, add characters to the token including spaces
            token += currentChar;
        }
        else if (std::isspace(currentChar)) {
            // If not inside quotes and encounter a space, finalize the current token
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (currentChar == ',') {
            // Ignore commas (they should not be treated as tokens)
            // Just finalize the current token if it exists
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else {
            // Otherwise, add the character to the current token
            token += currentChar;
        }
    }

    // Add the last token if it exists
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}


void InitializeVector(std::vector<std::vector<std::string>>& vecOfVecs)
{
 
###InitializeVector
    vecOfVecs.push_back(std::vector<std::string>{"DUAL", "CLASSES", "COUNT()", "DATE", "DBNAME", "DUMP"});
    vecOfVecs.push_back(std::vector<std::string>{"dual", "classes", "count()", "date", "dbname", "dump"});

}
void InitializeDesc(std::vector<std::vector<std::string>>& vecOfDesc)
{
    // Initialize the first inner vector with strings and add it to the outer vector
###InitializeDesc
}

std::string LoadHelp(void )
{
    return
        "Only one class(table) can be used in a query\n"
        "Maximum of five where clauses.\n"
        "Except for arrays of char, no arrays can be used in a query\n"
        "For enumerations, you have to use the item name, not the associated integer value\n"
        "You are not allowed to change the class variables declarations\n"
        "Names of classes(tables) and variables(columns) are case-sensitive\n"
        "No functions are supported in queries\n"
        "Queries :\n"
        "cls - clear the screen \n"
        "select dbname from dual\n"
        "select date from dual\n"
        "select classes from dual - lists of classes in the database\n"
        "desc <class name> -lists class declatations\n"
        "SELECT .....\n"
        "INSERT ....\n"
        "UPDATE ....\n"
        "DELETE ...\n"
        "BYE, EXIT, QUIT\n";

}
// Function to check if the command is valid and handle basic syntax checking
bool checkSyntax(std::vector<std::string>& tokens)
{
    if (tokens.empty()) {
        std::cerr << "Error: No command entered." << std::endl;
        return false;
    }

    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper); // Make command case-insensitive
    if (command == "CLS") {
        system("cls");
    }
    else if (command == "DESC") {
        if (tokens.size() != 2) {
            std::cerr << "Syntax Error: DESC expects 1 argument, e.g. DESC class(table) name." << std::endl;
            return false;
        }
        int i;
        for (i = 0; i < vecOfDesc.size(); i++)
        {
            if (vecOfDesc[i][0] == tokens[1])
            {
                std::cout << std::endl << vecOfDesc[i][1] << std::endl;
                break;
            }
        }
        if (i == vecOfDesc.size())
        {
            std::cout << std::endl << "Syntax Error: Invalid class(table) name. Valid values:" << std::endl;
            std::cout << "Use 'select classes from dual' to list all existing classes(tables)." << std::endl << std::endl;;


        }

    }
    else if (command == "HELP")
    {
        std::string help = LoadHelp();
        std::cout << std::endl << help << std::endl << std::endl;
    }
    else if (command == "SELECT")
    {
        std::unordered_map<std::string, std::vector<std::string>> tables;
        std::vector<std::string> columns;
        std::vector<std::vector<std::string>> where;

        if (!ValidateSelect(tokens, tables, columns, where))
            return false;
        if (!ProcessSelect(tokens, tables, columns, where))
            return false;
    }

    else if (command == "INSERT") {
        if (tokens.size() < 5 || toUpperCase(tokens[1]) != "INTO" ) {
            std::cerr << "Syntax Error: INSERT expects syntax like 'INSERT INTO table_name VALUES (...)'." << std::endl;
            return false;
        }
        std::vector<std::string> tables;
        std::vector<std::string> columns;
        std::vector<std::string> values;

        if (!ValidateInsert(tokens, tables, columns, values))
            return false;
        if (!ProcessInsert(tokens, tables, columns, values))
            return false;
    }
    else if (command == "DELETE") {
        if (tokens.size() < 3 || toUpperCase(tokens[1]) != "FROM"  ) {
            std::cerr << std::endl << "Syntax Error: DELETE expects syntax like 'DELETE FROM class(table) name WHERE condition'." << std::endl << std::endl;
            return false;
        }
        if (tokens.size() == 3)
        {
            std::cout << std::endl << "Are you sure you want to remove all instances of " << tokens[2] << " from the database? " << std::endl;
            std::cout << "Press 'C' or 'c' to cancel or any key to continue.  > ";
            // Get user input
            char userInput = _getch();

            // Handle user input
            if (std::tolower(userInput) == 'c') {
                std::cout << std::endl << std::endl << "Operation canceled by the user." << std::endl << std::endl;
                return false;
            }
            else {
                std::cout << std::endl << std::endl << "Proceeding with the operation..." << std::endl << std::endl;
                // You can add the logic for removing the instances here
            }
        }
        std::unordered_map<std::string, std::vector<std::string>> tables;
        std::vector<std::string> columns;
        std::vector<std::vector<std::string>> where;

        if (!ValidateDelete(tokens, tables, columns, where))
            return false;
        if (!ProcessDelete(tokens, tables,  where))
            return false;
    }
    else if (command == "UPDATE") {
        // Accept UPDATE table_name SET col = value WHERE condition
        if (tokens.size() < 6 || toUpperCase(tokens[2]) != "SET" || tokens[4] != "=" ) {
            std::cerr << "Syntax Error: UPDATE expects syntax like 'UPDATE table_name SET col = value WHERE condition'." << std::endl;
            return false;
        }
        std::vector<std::vector<std::string>> columnsAndValues;
        std::vector<std::vector<std::string>> where;
        std::vector<std::string> tables;

        if (!ValidateUpdate(tokens, tables, columnsAndValues, where))
            return false;
        
        if (!ProcessUpdate(tokens, tables, columnsAndValues, where))
            return false;
    }
    else {
        std::cerr << "Syntax Error: Unknown command '" << command << "'." << std::endl;
        return false;
    }

    return true;
}
bool ValidateWHERE(std::vector<std::string>& tokens,
	std::vector<std::vector<std::string>>& where,
	std::vector<std::string>::iterator wherePos)
{

	// Validate WHERE clause (optional, case-insensitive keyword comparison)
	if (wherePos != tokens.end()) {
		// Check if there are enough tokens after WHERE for a valid condition
		if (std::distance(wherePos, tokens.end()) < 4) {
			std::cerr << std::endl << "Syntax Error: 'WHERE' must be followed by a valid condition." << std::endl << std::endl;
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
    return true;
}


#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#ifdef __linux__
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "Database.h"
#include "Record.h"
###includes

std::string toUpperCase(const std::string& str);
void InitializeVector(std::vector<std::vector<std::string>>& vecOfVecs);
void InitializeDesc(std::vector<std::vector<std::string>>& vecOfDesc);
bool checkSyntax(std::vector<std::string>& tokens);
std::vector<std::string> split(std::string& s);

std::vector<std::vector<std::string>> vecOfVecs;
std::vector<std::vector<std::string>> vecOfDesc;
###declarations
Database db;



int main(int argc, char* argv[]) 
{
	if (argc < 2)
	{
		std::cout << "Usage:\n SmallSQL(DB file name)\nIf the file does not exist it will be created" << std::endl;
		return 1;
	}

	db.Connect(argv[1]);

	std::string input;

	std::cout << "Welcome to SmalSQL!" << std::endl;
	std::cout << "Supported commands: DESC, SELECT, INSERT, DELETE, UPDATE, CLS, HELP" << std::endl;
	std::cout << "'select from dual' supports 'classes', 'date', 'dbname' and 'dump' " << std::endl << std::endl;

	InitializeVector(vecOfVecs);
	InitializeDesc(vecOfDesc);
	Record::db->outFile.seekp(0, std::ios::beg);
#ifdef __linux__
    char * in;
#endif
	while (true) {
#ifdef __linux__
        in = readline("SmallSQL> ");
        input = in;
#elif _WIN32	
		std::cout << "SmallSQL> ";
		std::getline(std::cin, input);
#else
		std::cout "Unsupported platform." << std::endl;
#endif

		if (input.empty()) {
			continue;
		}
		if (toUpperCase(input) == "EXIT" || toUpperCase(input) == "QUIT" || toUpperCase(input) == "BYE") {
			break;
		}
#ifdef __linux__ 
        add_history(in);
#endif
		std::vector<std::string> tokens = split(input);

		if (checkSyntax(tokens)) {
			// Further command execution logic can be added here.
		}
	}

	std::cout << "Exiting SmallSQL." << std::endl;
	return 0;
}

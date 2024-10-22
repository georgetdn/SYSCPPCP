#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>

#include "Database.h"
#include "Record.h"
###includes

extern Database db;

bool ProcessInsert(
	std::vector<std::string>& tokens,
	std::vector<std::string>& tables,
	std::vector<std::string>& columns,
	std::vector<std::string>& values)
{
	Record* Rec = 0;
    ###declarations

	###ifs

	for (size_t i = 0; i < values.size(); ++i) {
		Rec->SetValue(columns[i], values[i]);
	}

	if (Rec->Insert())
		std::cout << "Record was inserted." << std::endl << std::endl;
	else
	{
		std::cout << "Failed to insert recordd." << std::endl << std::endl;
		return false;
	}

	return true;
}



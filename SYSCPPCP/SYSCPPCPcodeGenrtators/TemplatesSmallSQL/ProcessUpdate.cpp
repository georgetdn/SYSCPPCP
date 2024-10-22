#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>

#include "Database.h"
#include "Record.h"
###includes

###declarations
extern Database db;

bool ProcessUpdate(
	std::vector<std::string> tokens,
	std::vector<std::string> tables,
	std::vector<std::vector<std::string>> columnValuePair,
	std::vector<std::vector<std::string>> where)
{

	if (where.size() > 4)
	{
		std::cout << std::endl << "Only four where close are allowed." << std::endl << std::endl;
		return false;
	}

	Record* Rec = 0;
	recKey* k[4];
	k[0] = nullptr;
    ###ifs
	int idx = 0;
	for (const auto& vec : where) {
		k[idx] = Rec->GetRecordKey(vec[0]);
		if (k[idx] == nullptr)
			return false;
		k[idx]->comp = vec[1] == "=" ? Comp::Equal : vec[1] == "!=" ? Comp::NotEqual : vec[1] == "<" ? Comp::Smaller :
			vec[1] == "<=" ? Comp::SmallerEq : vec[1] == ">" ? Comp::Greater : Comp::GreaterEq;
		k[idx]->value = vec[2];
		if (vec.size() == 4)
		{
			if (vec[3] == "AND")
				k[idx]->andOr = AndOr::And;
			else
				k[idx]->andOr = AndOr::Or;
		}
		idx++;
	}
	if (Record::db->outFile.is_open()) {
		Record::db->outFile.seekp(0, std::ios::beg);
	}
	else {
		std::cerr << "Error: File is not open." << std::endl;
		return false;
	}
	std::cout << std::endl;

	int cnt = 0;
	while (true)
	{
		if (where.size() == 0)
		{
			if (Rec->Next(nullptr) == OpResult::False)
				break;
		}
		else if (where.size() == 1)
		{
			if (Rec->Next(k[0], nullptr) == OpResult::False)
				break;
		}
		else if (where.size() == 2)
		{
			if (Rec->Next(k[0], k[1], nullptr) == OpResult::False)
				break;
		}
		else if (where.size() == 3)
		{
			if (Rec->Next(k[0], k[1], k[2], nullptr) == OpResult::False)
				break;
		}
		else if (where.size() == 4)
		{
			if (Rec->Next(k[0], k[1], k[2], k[3], nullptr) == OpResult::False)
				break;
		}

		for (size_t i = 0; i < columnValuePair.size(); ++i) {
			Rec->SetValue(columnValuePair[i][0], columnValuePair[i][1]);
		}

		cnt++;
		Rec->Update();
	}
	if (cnt)
	{
		std::cout << "Number of records updated : " << cnt << std::endl << std::endl;
	}
	else
	{
		std::cout << "N0 records found. " << std::endl << std::endl;
	}
	return true;
}
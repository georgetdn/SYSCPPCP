
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>

#include "Record.h"
###includes
###declarations


bool ProcessDelete(std::vector<std::string>& tokens,
	std::unordered_map<std::string, std::vector<std::string>>& tables,
	std::vector<std::vector<std::string>>& where)
{

	// Declare an iterator at global or class level
	std::unordered_map<std::string, std::vector<std::string>>::iterator it = tables.begin();

	Record::db->outFile.seekp(0, std::ios::beg);
	if (it != tables.end())
	{
		Record* Rec = 0;
		recKey* k[4];
		k[0] = nullptr;

		###ifs
		int idx = 0;
		for (const auto& vec : where) {
			k[idx] = Rec->GetRecordKey(vec[0]);
			if (k[idx] == nullptr)
				return false;

			// validate number length
			if (k[idx]->typeInfo == typeid(signed short int) ||
				k[idx]->typeInfo == typeid(signed  int) ||
				k[idx]->typeInfo == typeid(signed long int) ||
				k[idx]->typeInfo == typeid(signed long long int) ||
				k[idx]->typeInfo == typeid(unsigned short int) ||
				k[idx]->typeInfo == typeid(unsigned  int) ||
				k[idx]->typeInfo == typeid(unsigned long int) ||
				k[idx]->typeInfo == typeid(unsigned long long int))
			{

				if (k[idx]->value.length() > 14)
				{
					std::cout << "Number out of range. Max is 14 digits.";
					return false;
				}
			}

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

		Record::db->outFile.seekp(0, std::ios::beg);
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
			cnt++;
			Rec->Delete();
		}
		if (cnt)
		{
			std::cout << "Number of records deleted : " << cnt << std::endl << std::endl;
		}
		else
		{
			std::cout << "N0 records found. " << std::endl << std::endl;
		}
	}
	return true;
}

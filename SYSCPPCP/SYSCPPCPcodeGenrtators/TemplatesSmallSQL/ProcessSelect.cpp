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
###declarations

extern std::vector<std::vector<std::string>> vecOfDesc;
extern std::vector<std::vector<std::string>> vecOfVecs;
std::string formatFloat(const std::string& val);
std::string formatDouble(const std::string& val);
std::string formatLongDouble(const std::string& val);

bool ProcessSelect(std::vector<std::string>& tokens,
	std::unordered_map<std::string, std::vector<std::string>>& tables,
	std::vector<std::string>& columns,
	std::vector<std::vector<std::string>>& where)
{
	std::unordered_map<std::string, std::vector<std::string>>::iterator it = tables.begin();

	if (it->first == "DUAL" || it->first == "dual")
	{
		if (columns[0] == "classes" || columns[0] == "CLASSES")
		{
			std::cout << std::endl;
			std::cout << "    Classes(Tables)\n    ===============" << std::endl;
			for (int i = 0; i < vecOfDesc.size(); i++)
				std::cout << "    " << vecOfDesc[i][0] << std::endl;
			std::cout << std::endl;
		}
		else if (columns[0] == "count()" || columns[0] == "COUNT()")
		{
			std::cout << std::endl << "Number of records in the database:" << db.GetCount() << std::endl << std::endl;
		}
		else if (columns[0] == "date" || columns[0] == "DATE")
		{
			std::cout << std::endl << system("echo %DATE%") << std::endl;
		}
		else if (columns[0] == "dbname" || columns[0] == "DBNAME")
		{
			std::cout << std::endl << "Database file name is " << db.GetDatabaseName() << std::endl << std::endl;
		}
		else if (columns[0] == "dump" || columns[0] == "DUMP")
		{
			db.Dump();
		}
	}
	else
	{
		bool doHeader = true;
		if (where.size() > 4)
		{
			std::cout << std::endl << "Only four where close are allowed." << std::endl << std::endl;
			return false;
		}
		// Declare an iterator at global or class level
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
			if (columns[0] == "*")
			{
				columns.clear();
				for (const auto& vec : vecOfVecs) {
					if (!vec.empty() && vec[0] == it->first) {
						// Print elements excluding the first and the last
						for (size_t i = 1; i < vec.size() - 1; ++i) {
							columns.push_back(vec[i]);
						}

					}
				}
			}
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
				if (doHeader)
				{
					for (const auto& col : columns) {
						Rec->hedcout(col);
					}
					std::cout << std::endl;
					for (const auto& col : columns) {
						Rec->sepcout(col);
					}
					std::cout << std::endl;
					doHeader = false;
				}
				cnt++;
				for (const auto& col : columns) {
					Rec->varcout(col);
				}
				std::cout << std::endl;
			}
			if (cnt)
			{
				std::cout << std::endl << "Number of records found : " << cnt << std::endl << std::endl;
			}
			else
			{
				std::cout << "N0 records found. " << std::endl << std::endl;
			}
		}
		return true;
	}
	return false;
}
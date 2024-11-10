#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

extern std::vector<std::vector<std::string>> enums;
extern std::vector<std::vector<std::string>> structs;
extern std::vector<std::vector<std::string>> variables;

bool isNativeType(std::string type);

bool genSources(std::vector<std::string>& prefixes)
{
	std::ifstream inputFile("../TemplatesSmallSQL/derClasses.cpp");
	if (!inputFile) {
		std::cerr << "Error: Unable to open file for reading." << std::endl;
		return false;
	}
	std::ostringstream fileContent;
	fileContent << inputFile.rdbuf();
	inputFile.close();
	std::string content = fileContent.str();

	for (const std::string& prefix : prefixes)
	{
		std::cout << "    Generating " << prefix << "Der and source files." << std::endl;
		std::string tmp = content;
		while (tmp.find("###prefix") != std::string::npos)
			tmp.replace(tmp.find("###prefix"), 9, prefix);

		std::string GetRecordKey;
		std::string varcout;
		std::string hedcout;
		std::string sepcout;
		std::string SetValue;
		// Use std::find_if to find the inner vector where the first value matches prefix
		auto it = std::find_if(variables.begin(), variables.end(),
			[&prefix](const std::vector<std::string>& vec) {
				return !vec.empty() && vec[0] == prefix;
			});
		std::string Else;
		if (it != variables.end()) {
			int i = 0;
			std::string dataType;
			for (auto& value : *it) {
				if (i == 0)
				{
					i++;
					continue;
				}
				if (i % 2 == 0)
				{
					//varcout
					/////////
					std::string sz;
					if (dataType.find("char") != std::string::npos)
					{
						if (dataType.find("[") != std::string::npos)
						{
							sz = "sizeof(" + dataType + ")";
							varcout += "    else if (var == \"" + value + "\")\n"
								"         std::cout << data." + value + " << std::string(sizeof(\"" + value + "\") < sizeof(" + dataType + ") ? sizeof(" + dataType + ") - std::strlen(data." + value + ") : sizeof(\"" + value + "\") - std::strlen(data." + value + "), ' ') << \" \";\n";
						}
						else
						{
							sz = "sizeof(" + dataType + ")";
							varcout += "    else if (var == \"" + value + "\")\n"
								"         std::cout << data." + value + " << std::string(sizeof(\"" + value + "\") - 1, ' ') << \" \";\n";
						}
					}
					else if (isNativeType(dataType))
					{
						varcout += "    else if (var == \"" + value + "\")\n"
							"         std::cout << std::fixed << std::setprecision(3) << std::right << std::setw(10) << data." + value + " << \" \";\n";
					}
					else
					{ //check if enum and get size
						bool found = false;
						size_t i = 0;
						for (const auto& row : enums) {
							if (row[0] != prefix)
								continue;
							for (const auto& str : row) {
								if (str == dataType)
								{
									found = true;
									continue;
								}
								if (i < str.length())
									i = str.length();
							}
							if (found)
							{
								sz = std::to_string(i + 1);
								varcout += "    else if (var == \"" + value + "\")\n"
									"         std::cout << " + dataType + "Array[data." + value + "] << std::string(" + sz + " -  std::strlen(" + dataType + "Array[data." + value + "]), ' ');\n";
								found = false;
							}
						}
					}
					////////////
					GetRecordKey += "    else if (var == \"" + value + "\")\n"
						"    {\n"
						"        return &";
					std::string stmp = value;
					while (stmp.find(".") != std::string::npos)
						stmp.replace(stmp.find("."), 1, "");

					GetRecordKey += stmp;
					GetRecordKey += "Key;\n"
						"    }\n";
					////////////////
					//hedcout 
					if (dataType.find("char") != std::string::npos)
					{
						sz = "sizeof(\"" + value + "\")";
						hedcout += "    else if (var == \"" + value + "\")\n"
							"         std::cout << \"" + value + "\" << std::string((sizeof(data." + value + ") > sizeof(\"" + value + "\")) ? sizeof(data." + value + ") - sizeof(\"" + value + "\") : 0, ' ') << \"  \";\n";
					}
					else if (isNativeType(dataType))
					{
						hedcout += "    else if (var == \"" + value + "\")\n"
							"         std::cout << std::fixed << std::right << std::setw(10) << \"" + value + "\" << \" \";\n";
					}
					else
					{ //check if enum and get size
						bool found = false;
						size_t i = 0;
						for (const auto& row : enums) {
							if (row[0] != prefix)
								continue;
							for (const auto& str : row) {
								if (str == dataType)
								{
									found = true;
									continue;
								}
								if (i < str.length())
									i = str.length();
							}
							if (found)
							{
								sz = std::to_string(i + 1);
								hedcout += "    else if (var == \"" + value + "\")\n"
									"          std::cout << \"" + value + "\" << std::string(" + sz + " < std::strlen(\"" + value + "\") ? std::strlen(\"" + value + "\") - " + sz + " : " + sz + " - std::strlen(\"" + value + "\"), ' ');\n";
								found = false;
							}
						}
					}

					/////////////////
					//sepcout
					if (dataType.find("char") != std::string::npos)
					{
						sz = "sizeof(" + dataType + ")";
						sepcout += "    else if (var == \"" + value + "\")\n"
							"         std::cout << std::string((" + sz + " > sizeof(\"" + value + "\")) ? " + sz + " : sizeof(\"" + value + "\"), '-') << \" \";\n";
					}
					else if (isNativeType(dataType))
					{
						sepcout += "    else if (var == \"" + value + "\")\n"
							"          std::cout << std::string(10, '-') << \" \";\n";
					}
					else
					{ //check if enum and get size
						bool found = false;
						size_t i = 0;
						for (const auto& row : enums) {
							if (row[0] != prefix)
								continue;
							for (const auto& str : row) {
								if (str == dataType)
								{
									found = true;
									continue;
								}
								if (i < str.length())
									i = str.length();
							}
							if (found)
							{
								sz = std::to_string(i);
								sepcout += "    else if (var == \"" + value + "\")\n"
									"          std::cout << std::string(" + sz + " , '-') << \" \";\n";
								found = false;
							}
						}
					}
					///////////
					//SetValue
					if (dataType.find("char") != std::string::npos)
					{
						if (dataType.find("[") != std::string::npos)
						{
							SetValue += Else + " if (col == \"" + value + "\")\n     INIT_STR(data." + value + ", val.c_str())\n";
							Else = "else";
						}
						else
						{
							SetValue += Else + " if (col == \"" + value + "\")\n     data." + value + " = val[0];\n";
							Else = "else";
						}
					}
					else if (isNativeType(dataType))
					{
						//						if (dataType == "float" || dataType == "double" || dataType == "long double")
						//						{

						if (dataType == "float") {
							SetValue += Else + " if (col == \"" + value + "\")\n {\n"

								"         std::string integerPart;\n"
								"         size_t periodPos = val.find('.');\n"
								"         if (periodPos != std::string::npos) {\n"
								"             integerPart = val.substr(0, periodPos);\n"
								"         }\n"
								"         else\n"
								"         {\n"
								"             integerPart = val;\n"
								"         }\n"
								"         if (integerPart.length() > 5)\n"
								"         {\n"
								"             std::cout << \"Max value for the integral part of " + value + " is 5.\" << std::endl;\n"
								"             return false;\n"
								"         }\n"
								"         val = integerPart +\".\" + val.substr(periodPos + 1, 5);\n"
								"         data." + value + " = std::stof(val) ;\n}\n";
						}
						else if (dataType == "double") {
							SetValue += Else + " if (col == \"" + value + "\")\n    {\n"
								"         std::string integerPart;\n"
								"         size_t periodPos = val.find('.');\n"
								"         if (periodPos != std::string::npos) {\n"
								"             integerPart = val.substr(0, periodPos);\n"
								"         }\n"
								"         else\n"
								"         {\n"
								"             integerPart = val;\n"
								"         }\n"
								"         if (integerPart.length() > 13)\n"
								"         {\n"
								"             std::cout << \"Max value for the integral part of " + value + " is 13.\" << std::endl;\n"
								"             return false;\n"
								"         }\n"
								"         val = integerPart +\".\" + val.substr(periodPos + 1, 5);\n"
								"         data." + value + " = std::stod(val) ;\n   }\n";
						}
						else if (dataType == "long double") {
							SetValue += Else + " if (col == \"" + value + "\")\n{\n"
								"         std::string integerPart;\n"
								"         size_t periodPos = val.find('.');\n"
								"         if (periodPos != std::string::npos) {\n"
								"             integerPart = val.substr(0, periodPos);\n"
								"         }\n"
								"         else\n"
								"         {\n"
								"             integerPart = val;\n"
								"         }\n"
								"         if (integerPart.length() > 14)\n"
								"         {\n"
								"             std::cout << \"Max value for the integral part of " + value + " is 14.\" << std::endl;\n"
								"             return false;\n"
								"         }\n"
								"         val = integerPart +\".\" + val.substr(periodPos + 1, 5);\n"
								"         data." + value + " = std::stold(val) ;\n{\n";
						}
						//					}
						else if (dataType == "int" || dataType == "signed int" || dataType == "unsigned int" ||
							dataType == "short" || dataType == "short int" || dataType == "signed short" || dataType == "signed short int" ||
							dataType == "unsigned short" || dataType == "unsigned short int")
						{
							SetValue += Else + " if (col == \"" + value + "\")\n{\n"
								"    if (val.length() > 9) \n"
								"    {\n    std::cout << \"Max length of " + value + " is 9 digits\" << std::endl;\n"
								"           return false;\n"
								"     }\n"
								"    data." + value + " = std::stoi(val);\n} \n";
							Else = "else";
						}
							
						else if (	dataType == "signed long int" ||
							dataType == "signed long" || dataType == "long" || dataType == "long int" || dataType == "unsigned long int" ||
							dataType == "unsigned long")
						{
							SetValue += Else + " if (col == \"" + value + "\")\n{\n"
								"    if (val.length() > 9) \n"
								"    {\n    std::cout << \"Max length of " + value + " is 9 digits\" << std::endl;\n"
								"           return false;\n"
								"     }\n"
								"    data." + value + " = std::stol(val);\n} \n";
							Else = "else";

						}
						else
						{
							SetValue += Else + " if (col == \"" + value + "\")\n{\n"
								"    if (val.length() > 10) \n"
								"    {\n    std::cout << \"Max length of " + value + " is 10 digits\" << std::endl;\n"
								"           return false;\n"
								"     }\n"
								"    data." + value + " = std::stoll(val);\n} \n";
							Else = "else";



						}



					}
					else
					{ //check if enum and get size
						bool found = false;
						size_t i = 0;
						for (const auto& row : enums) {
							if (row[0] != prefix)
								continue;
							if (row[1] != dataType)
								continue;
							for (const auto& str : row) {
								if (str == prefix)
									continue;
								if (str == dataType)
								{
									found = true;
									SetValue += Else + " if (col == \"" + value + "\")\n{\n";
									Else = "else";
									continue;
								}
								SetValue += "        if ( val == \"" + dataType + "::" + str + "\" || val == \"" + str + "\")\n              data." + value + " = " + dataType + "::" + str + ";\n";

							}
							SetValue += "}\n";
							if (found)
								break;
						}

					}
				}
				else
				{
					dataType = value;

				}

				i++;
			}

		}
		else {
			std::cout << "No inner vector found with the prefix: " << prefix << std::endl;
			return false;
		}

		size_t Pos = tmp.find("###varcout");
		if (Pos != std::string::npos) {
			tmp.replace(Pos, std::string("###varcout").length(), varcout);
		}
		Pos = tmp.find("###GetRecordKey");
		if (Pos != std::string::npos) {
			tmp.replace(Pos, std::string("###GetRecordKey").length(), GetRecordKey);
		}
		Pos = tmp.find("###hedcout");
		if (Pos != std::string::npos) {
			tmp.replace(Pos, std::string("###hedcout").length(), hedcout);
		}
		Pos = tmp.find("###sepcout");
		if (Pos != std::string::npos) {
			tmp.replace(Pos, std::string("###sepcout").length(), sepcout);
		}
		Pos = tmp.find("###SetValue");
		if (Pos != std::string::npos) {
			tmp.replace(Pos, std::string("###SetValue").length(), SetValue);
		}


		// Write the updated content back to the file
		std::string outFileName = "../../SmallSQLsource/" + prefix + "Der.cpp";
		std::ofstream outputFile(outFileName);
		if (!outputFile) {
			std::cerr << "Error: Unable to open file for writing." << std::endl;
			return false;
		}

		outputFile << tmp;
		outputFile.close();
	}
	return true;
}


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
	std::ifstream inputFile("..\\TemplatesSmallSQL\\derClasses.cpp");
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
						sz = "(sizeof(" + dataType + ") * 2+4)";
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
									"         std::cout << "+dataType + "Array[data." + value + "] << std::string(" + sz + " -  std::strlen("+dataType + "Array[data." + value + "]), ' ');\n";
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
							// "         std::cout << (std::string(\"" + value + "\").length() > 10 ? std::string(\"" + value + "\").substr(0, 10) : std::string(\"" + value + "\")) << (std::string(\"" + value + "\").length() > 10 ? \"\" : std::string(10 - std::string(\"" + value + "\").length(), ' ')) << \" \";\n";


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
								sz = std::to_string(i+1);
								hedcout += "    else if (var == \"" + value + "\")\n"
									"          std::cout << \""+ value + "\" << std::string(" + sz + " < std::strlen(\"" + value + "\") ? std::strlen(\"" + value + "\") - " + sz + " : " + sz + " - std::strlen(\"" + value + "\"), ' ');\n";
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
						if (dataType == "float" || dataType == "double" || dataType == "long double")
						{
							if (dataType == "float") {
								SetValue += Else + " if (col == \"" + value + "\")\n     data." + value + " = std::stof(val) ;\n";
							}
							else if (dataType == "double") {
								SetValue += Else + " if (col == \"" + value + "\")\n     data." + value + " = std::stod(val) ;\n";
							}
							else if (dataType == "long double") {
								SetValue += Else + " if (col == \"" + value + "\")\n     data." + value + " = std::stold(val) ;\n";
							}
						}
						else
						{
							SetValue += Else + " if (col == \"" + value + "\")\n     data." + value + " = std::stoi(val);\n";
						}
						Else = "else";
					}
					else
					{ //check if enum and get size
						bool found = false;
						size_t i = 0;
						for (const auto& row : enums) {
							if (row[0] != prefix)
								continue;
							if(row[1] != dataType)
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
		std::string outFileName = "..\\..\\SmallSQLsource\\" + prefix + "Der.cpp";
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


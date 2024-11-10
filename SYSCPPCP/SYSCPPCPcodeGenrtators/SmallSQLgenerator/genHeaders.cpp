#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
 
std::string tmpl = 
"#pragma once \n"
"#include \"Record.h\" \n"
"#include \"###prefix.h\" \n\n"
"class ###prefixDer :\n"
"    public ###prefix\n"
"{\n"
"public:\n"
"    ###prefixDer() = default;\n"
"    ~###prefixDer() override = default;\n"
"public:\n"
"    bool varcout(std::string var) override;\n"
"    bool hedcout(std::string var) override;\n"
"    bool sepcout(std::string var) override;\n"
"    bool SetValue(std::string col, std::string val)  override;\n"
"    recKey* GetRecordKey(std::string var) override;\n"
"};";

bool genHeaders(std::vector<std::string>& prefixes)
{

	for (const std::string& prefix : prefixes)
	{
		std::string tmp = tmpl;
		while (tmp.find("###prefix") != std::string::npos)
			tmp.replace(tmp.find("###prefix"), 9, prefix);
		//write header file
		std::string filePath = "../../SmallSQLHeaders/" + prefix + "Der.h";

		std::ofstream outputFile(filePath);
		if (!outputFile) {
			std::cerr << "Error: Unable to open file for writing." << std::endl;
			return false;
		}

		outputFile << tmp;
		outputFile.close();
	}
	return true;
}


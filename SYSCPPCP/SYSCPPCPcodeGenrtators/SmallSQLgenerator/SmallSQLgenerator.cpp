#include <iostream>
#include <filesystem>
#include <vector>
#include <set>
#include <string>
#include <fstream>

std::vector<std::vector<std::string>> enums;
std::vector<std::vector<std::string>> structs;
std::vector<std::vector<std::string>> variables;

bool parseTemplates(std::vector<std::vector<std::string>>& enums,
	std::vector<std::vector<std::string>>& structs,
	std::vector<std::vector<std::string>>& variables,
	std::vector<std::string>& prefixes);

bool genSmallSQL(std::vector<std::string> &prefixes);
bool genHeaders(std::vector<std::string>& prefixes);
bool genProcessDelete(std::vector<std::string>& prefixes);
bool genProcessInsert(std::vector<std::string>& prefixes);
bool genProcessUpdate(std::vector<std::string>& prefixes);
bool genProcessSelect(std::vector<std::string>& prefixes);
bool genSources(std::vector<std::string>& prefixes);
bool genCommon(std::vector<std::string>& prefixes);
bool genProjectFile(std::vector<std::string>& prefixes);

std::string toLower(const std::string& str);

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

	std::set<std::string> uniquePrefixes;  // Using set to ensure uniqueness
	std::vector<std::string> result;       // Final vector of unique prefixes
	std::string directoryPath = "../templates";
	// Create the directory for the source files
	std::string cmmandLn;
	cmmandLn = "mkdir \"../../SmallSQLSource\"";
	system(cmmandLn.c_str());
	cmmandLn = "mkdir \"../../SmallSQLheaders\"";
	system(cmmandLn.c_str());

	// Iterate through the files in the directory
	for (const auto& entry : fs::directory_iterator(directoryPath)) {
		std::string filename = entry.path().filename().string();

		// Convert filename to lowercase for case-insensitive comparison
		std::string lowerFilename = toLower(filename);

		// Check if the file ends with one of the required suffixes (case-insensitive, no periods)
		if (lowerFilename.ends_with("enum.tmpl") || lowerFilename.ends_with("struct.tmpl") || lowerFilename.ends_with("variables.tmpl")) {
			// Extract the prefix (before the suffix, no periods)
			size_t pos = lowerFilename.find("enum.tmpl");
			if (pos == std::string::npos) {
				pos = lowerFilename.find("struct.tmpl");
			}
			if (pos == std::string::npos) {
				pos = lowerFilename.find("variables.tmpl");
			}

			// Extract the prefix from the filename
			std::string prefix = filename.substr(0, pos);
			uniquePrefixes.insert(prefix);  // Add the prefix to the set
		}
	}

	// Copy the unique prefixes from the set to the vector
	result.assign(uniquePrefixes.begin(), uniquePrefixes.end());

	// Output the unique prefixes
	std::cout << "Unique prefixes:" << std::endl;
	std::vector<std::string> prefixes;
	for (const auto& recName : result) {
		prefixes.push_back(recName);
	}


	if(! parseTemplates(enums,structs,variables, prefixes))
		return 1;
	// copy common files
	std::cout << "Copying Command.cpp." << std::endl;
	system("copy ..\\TemplatesSmallSQL\\Common.cpp ..\\..\\SmallSQLsource\\Common.cpp");

	std::cout << std::endl << "Generating SmallSQL.cpp." << std::endl;
	if (!genSmallSQL(prefixes))
		return 1;

	std::cout << std::endl << "Generating header files." << std::endl;
	if (!genHeaders(prefixes))
		return 1;
	std::cout << std::endl << "Generating ProcessDelete.cpp." << std::endl;
	if (!genProcessDelete(prefixes))
		return 1;
	std::cout << "Copying validateDelete.cpp." << std::endl;
	system("copy ..\\TemplatesSmallSQL\\validateDelete.cpp ..\\..\\SmallSQLsource\\validateDelete.cpp");

	std::cout << std::endl << "Generating ProcessInsert.cpp." << std::endl;
	if (!genProcessInsert(prefixes))
		return 1;
	std::cout <<  "Copying validateInsert.cpp." << std::endl;
	system("copy ..\\TemplatesSmallSQL\\validateInsert.cpp ..\\..\\SmallSQLsource\\validateInsert.cpp");

	std::cout << std::endl << "Generating ProcessUpdate.cpp." << std::endl;
	if (!genProcessUpdate(prefixes))
		return 1;
	std::cout <<  "Copying validateUpdate.cpp." << std::endl;
	system("copy ..\\TemplatesSmallSQL\\validateUpdate.cpp ..\\..\\SmallSQLsource\\validateUpdate.cpp");

	std::cout << std::endl << "Generating ProcessSelect.cpp." << std::endl;
	if (!genProcessSelect(prefixes))
		return 1;
	std::cout <<  "Copying validateSelect.cpp." << std::endl;
	system("copy ..\\TemplatesSmallSQL\\validateSelect.cpp ..\\..\\SmallSQLsource\\validateSelect.cpp");

	std::cout << std::endl << "Generating classes source files." << std::endl;
	if (!genSources(prefixes))
		return 1;

	std::cout << std::endl << "Generating common.cpp." << std::endl;
	if (!genCommon(prefixes))
		return 1;

	std::cout << std::endl << "Generating SmallSQL.vcxproj." << std::endl;
	if (!genProjectFile(prefixes))
		return 1;
	//build
	cmmandLn = "msbuild ../../SYSCPPCPvcxproj/SmallSQL.vcxproj /p:Configuration=Debug /p:Platform=x64";
	system(cmmandLn.c_str());
	cmmandLn = "msbuild ../../SYSCPPCPvcxproj/SmallSQL.vcxproj /p:Configuration=Release /p:Platform=x64";
	system(cmmandLn.c_str());

	return 0;
}

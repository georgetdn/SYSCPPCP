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

	std::string command;
#ifdef __linux__
	command = "cp ../TemplatesSmallSQL/ValidateDelete.cpp ../../SmallSQLSource/ValidateDelete.cpp";
#else
	command = "copy ..\\TemplatesSmallSQL\\ValidateDelete.cpp ..\\..\\SmallSQLSource\\ValidateDelete.cpp\"";
#endif
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "Failed to copy the file. Error code: " << res << std::endl;
	}
	else {
		std::cout << "File copied successfully." << std::endl;
	}

	std::cout << std::endl << "Generating ProcessInsert.cpp." << std::endl;
	if (!genProcessInsert(prefixes))
		return 1;
	std::cout <<  "Copying validateInsert.cpp." << std::endl;
#ifdef __linux__
	command = "cp ../TemplatesSmallSQL/ValidateInsert.cpp ../../SmallSQLSource/ValidateInsert.cpp";
#else
	command = "copy ..\\TemplatesSmallSQL\\ValidateInsert.cpp ..\\..\\SmallSQLSource\\ValidateInsert.cpp\"";
#endif
	res = system(command.c_str());
	if (res != 0) {
		std::cerr << "Failed to copy the file. Error code: " << res << std::endl;
	}
	else {
		std::cout << "File copied successfully." << std::endl;
	}
	std::cout << std::endl << "Generating ProcessUpdate.cpp." << std::endl;
	if (!genProcessUpdate(prefixes))
		return 1;
	std::cout <<  "Copying validateUpdate.cpp." << std::endl;
#ifdef __linux__
	command = "cp ../TemplatesSmallSQL/ValidateUpdate.cpp ../../SmallSQLSource/ValidateUpdate.cpp";
#else
	command = "copy ..\\TemplatesSmallSQL\\ValidateUpdate.cpp ..\\..\\SmallSQLSource\\ValidateUpdate.cpp\"";
#endif
	command += " ../TemplatesSmallSQL/ValidateUpdate.cpp ../../SmallSQLSource/ValidateUpdate.cpp";
	res = system(command.c_str());
	if (res != 0) {
		std::cerr << "Failed to copy the file. Error code: " << res << std::endl;
	}
	else {
		std::cout << "File copied successfully." << std::endl;
	}

	std::cout << std::endl << "Generating ProcessSelect.cpp." << std::endl;
	if (!genProcessSelect(prefixes))
		return 1;
	std::cout <<  "Copying validateSelect.cpp." << std::endl;
#ifdef __linux__
	command = "cp ../TemplatesSmallSQL/ValidateSelect.cpp ../../SmallSQLSource/ValidateSelect.cpp";
#else
	command = "copy ..\\TemplatesSmallSQL\\ValidateSelect.cpp ..\\..\\SmallSQLSource\\ValidateSelect.cpp\"";
#endif
	res = system(command.c_str());
	if (res != 0) {
		std::cerr << "Failed to copy the file. Error code: " << res << std::endl;
	}
	else {
		std::cout << "File copied successfully." << std::endl;
	}

	std::cout << std::endl << "Generating classes source files." << std::endl;
	if (!genSources(prefixes))
		return 1;

	std::cout << std::endl << "Generating common.cpp." << std::endl;
	if (!genCommon(prefixes))
		return 1;
#ifdef __linux__
	std::cout << std::endl << "Generating SmallSQL makefile." << std::endl;
	if (!genProjectFile(prefixes))
		return 1;
	//build
	int ret = system("make -C ../../SYSCPPCPmake -f makefile_SQL debug release");
	if (ret == 0) {
		std::cout << "Makefile executed successfully." << std::endl;
	}
	else {
		std::cerr << "Error executing Makefile. Return code: " << ret << std::endl;
	}
#elif _WIN32
	std::cout << std::endl << "Generating SmallSQL.vcxproj." << std::endl;
	if (!genProjectFile(prefixes))
		return 1;
	//build
	cmmandLn = "msbuild ../../SYSCPPCPvcxproj/SmallSQL.vcxproj /p:Configuration=Debug /p:Platform=x64";
	system(cmmandLn.c_str());
	cmmandLn = "msbuild ../../SYSCPPCPvcxproj/SmallSQL.vcxproj /p:Configuration=Release /p:Platform=x64";
	system(cmmandLn.c_str());
#else
	std::cout << "Unknown platform " << std::endl;
#endif
	return 0;
}

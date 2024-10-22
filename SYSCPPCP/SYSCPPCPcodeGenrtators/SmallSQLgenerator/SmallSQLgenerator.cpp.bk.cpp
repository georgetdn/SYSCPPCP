#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

// Function to convert a string to uppercase (for keyword comparison)
std::string toUpperCase(const std::string& str) {
	std::string upperStr = str;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
		[](unsigned char c) { return std::toupper(c); });

	return upperStr;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <prefix>" << std::endl;
		return 1;
	}

	std::vector<std::string> prefixes;
	for (int i = 1; i < argc; ++i) {
		prefixes.push_back(argv[i]);
	}
	std::string directoryPath = "Templates";
	std::string extension = ".tmpl";
	std::vector<std::vector<std::string>> enums;
	std::vector<std::vector<std::string>> structs;
	std::vector<std::vector<std::string>> variables;
	std::vector<std::string> fileContents;

	try {
		for (const auto& entry : fs::directory_iterator(directoryPath)) {
			if (entry.is_regular_file()) {
				std::string fileName = entry.path().filename().string();
				if (std::any_of(prefixes.begin(), prefixes.end(), [&](const std::string& prefix) { return fileName.starts_with(prefix); }) && entry.path().extension() == extension) {
					std::string baseName = fileName.substr(0, fileName.find_last_of('.'));
					std::ifstream file(entry.path());

					if (file) {
						fileName = toUpperCase(fileName);
						if (fileName.ends_with("ENUM.TMPL")) {
							baseName = baseName.substr(0, baseName.size() - 4);

							std::vector<std::string> enumData;
							std::string line;
							while (std::getline(file, line))
							{
								//is line empty
								line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
								if (line.empty() || line.find('{') != std::string::npos) {
									continue; // Skip empty, whitespace-only lines 
								}
								if (line.starts_with("enum")) {
									line = line.substr(4); // Remove "enum" from the line
									line.erase(0, line.find_first_not_of(" ")); // Trim leading spaces
									line.erase(line.find_last_not_of(" ") + 1); // Trim trailing spaces
									enumData.clear();
									enumData.push_back(baseName);
									enumData.push_back(line);
									continue;
								}
								if (line.find('}') != std::string::npos)
								{
									enums.push_back(enumData);
									continue;
								}
								else
								{
									line.erase(std::remove(line.begin(), line.end(), ','), line.end());
									line.erase(0, line.find_first_not_of(" ")); // Trim leading spaces
									line.erase(line.find_last_not_of(" ") + 1); // Trim trailing spaces
								}

								enumData.push_back(line);
							}
						}
						else if (fileName.ends_with("STRUCT.TMPL")) {
							baseName = baseName.substr(0, baseName.size() - 6);

							std::vector<std::string> structData;
							std::string line;
							while (std::getline(file, line))
							{
								std::string dataType, variableName;

								//is line empty
								if (line.empty() || line.find('{') != std::string::npos) {
									continue; // Skip empty, whitespace-only lines 
								}
								if (line.starts_with("struct")) {
									line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
									line = line.substr(6); // Remove "struct" from the line
									line.erase(0, line.find_first_not_of(" ")); // Trim leading spaces
									line.erase(line.find_last_not_of(" ") + 1); // Trim trailing spaces
									structData.clear();
									structData.push_back(baseName);
									structData.push_back(line);
									continue;
								}
								if (line.find('}') != std::string::npos)
								{
									structs.push_back(structData);
									continue;
								}
								else
								{
									while (line.find(" [") != std::string::npos)
										line.replace(line.find(" ["), 2, "[");
									// Trim leading and trailing spaces
									line.erase(0, line.find_first_not_of(" "));
									line.erase(line.find_last_not_of(" ") + 1);

									// Split line into data type and variable name
									std::istringstream lineStream(line);
									lineStream >> dataType;

									// Handle multi-word data types (e.g., long long, unsigned int)
									std::string word;
									while (lineStream >> word && word.back() != ';') {
										dataType += " " + word;
									}

									// Remove the semicolon if present
									if (word.back() == ';') {
										word.pop_back();
									}
									variableName = word;

									// Handle array types like char test[12]
									size_t bracketPos = variableName.find('[');
									if (bracketPos != std::string::npos) {
										dataType += variableName.substr(bracketPos);
										variableName = variableName.substr(0, bracketPos);
									}
								}

								structData.push_back(dataType);
								structData.push_back(variableName);

							}
						}
						else {
							std::vector<std::string> variablesData;
							std::string line;
							while (std::getline(file, line))
							{
								std::string dataType, variableName;

								//is line empty
								if (line.empty() || line.find('{') != std::string::npos) {
									continue; // Skip empty, whitespace-only lines 
								}
								if (line.starts_with("struct")) {
									line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
									line = line.substr(6); // Remove "struct" from the line
									line.erase(0, line.find_first_not_of(" ")); // Trim leading spaces
									line.erase(line.find_last_not_of(" ") + 1); // Trim trailing spaces
									variablesData.clear();
									variablesData.push_back(baseName);
									variablesData.push_back(line);
									continue;
								}
								if (line.find('}') != std::string::npos)
								{
									variables.push_back(variablesData);
									continue;
								}
								else
								{
									while (line.find(" [") != std::string::npos)
										line.replace(line.find(" ["), 2, "[");
									// Trim leading and trailing spaces
									line.erase(0, line.find_first_not_of(" "));
									line.erase(line.find_last_not_of(" ") + 1);

									// Split line into data type and variable name
									std::istringstream lineStream(line);
									lineStream >> dataType;

									// Handle multi-word data types (e.g., long long, unsigned int)
									std::string word;
									while (lineStream >> word && word.back() != ';') {
										dataType += " " + word;
									}

									// Remove the semicolon if present
									if (word.back() == ';') {
										word.pop_back();
									}
									variableName = word;

									// Handle array types like char test[12]
									size_t bracketPos = variableName.find('[');
									if (bracketPos != std::string::npos) {
										dataType += variableName.substr(bracketPos);
										variableName = variableName.substr(0, bracketPos);
									}
								}

								variablesData.push_back(dataType);
								variablesData.push_back(variableName);

							}
						}
						std::cout << "Read file: " << fileName << std::endl;
					}
					else {
						std::cerr << "Failed to open file: " << fileName << std::endl;
					}
				}
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
		return 1;
	}

	// Print contents of all read files
	for (const auto& content : fileContents) {
		std::cout << "File content: " << std::endl << content << std::endl;
	}

	// Print enum data
	for (const auto& enumData : enums) {
		std::cout << "Enum: " << enumData[0] << std::endl;
		for (size_t i = 2; i < enumData.size(); ++i) {
			std::cout << "  " << enumData[i] << std::endl;
		}
	}

	// Print struct data
	for (const auto& structData : structs) {
		std::cout << "Struct: " << structData[0] << std::endl;
		for (size_t i = 2; i < structData.size(); i += 2) {
			std::cout << "  Type: " << structData[i] << ", Name: " << structData[i + 1] << std::endl;
		}
	}

	// Print variable data
	for (const auto& varData : variables) {
		std::cout << "Variables: " << varData[0] << std::endl;
		for (size_t i = 2; i < varData.size(); i += 2) {
			std::cout << "  Type: " << varData[i] << ", Name: " << varData[i + 1] << std::endl;
		}
	}

	return 0;
}

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

// Function to convert a string to uppercase (for keyword comparison)
std::string toUpperCase(const std::string& str);
bool isNativeType(std::string type);
bool isEnum(std::vector<std::vector<std::string>>& enums, std::string type);
bool isStruct(std::vector<std::vector<std::string>>& structs, std::string type);

std::string directoryPath = "../Templates";
std::string extension = ".tmpl";
std::string baseName;
std::ifstream fileEnum;
std::ifstream fileStruct;
std::ifstream fileVariables;
std::string fileNameOrig;


bool parseTemplates(std::vector<std::vector<std::string>>& enums,
	std::vector<std::vector<std::string>>& structs,
	std::vector<std::vector<std::string>>& variables,
	std::vector<std::string> &prefixes)
{
	try {
		for (const std::string& prefix : prefixes)
		{
			for (const auto& entry : fs::directory_iterator(directoryPath)) {
				if (entry.is_regular_file())
				{
					std::string fileName = entry.path().filename().string();
					if (fileName.find(prefix) == std::string::npos)
						continue;

					fileNameOrig = fileName;
					fileName = toUpperCase(fileName);
					if (fileName.ends_with("ENUM.TMPL"))
					{
						baseName = baseName.substr(0, baseName.size() - 4);
						if (fileEnum.is_open())
							fileEnum.close();
						fileEnum.open(entry.path());
						if (!fileEnum)
						{
							std::cerr << "Failed to open file: " << fileNameOrig << std::endl;
							return false;
						}
					}
					else if (fileName.ends_with("STRUCT.TMPL"))
					{
						baseName = baseName.substr(0, baseName.size() - 6);
						if (fileStruct.is_open())
							fileStruct.close();
						fileStruct.open(entry.path());
						if (!fileStruct)
						{
							std::cerr << "Failed to open file: " << fileNameOrig << std::endl;
							return false;
						}
					}
					else  if (fileName.ends_with("VARIABLES.TMPL"))
					{
						baseName = baseName.substr(0, baseName.size() - 6);
						if (fileVariables.is_open())
							fileVariables.close();
						fileVariables.open(entry.path());
						if (!fileVariables)
						{
							std::cerr << "Failed to open file: " << fileNameOrig << std::endl;
							return false;
						}

					}
					else
					{
						std::cout << "Invalid file: " << fileNameOrig << std::endl;
						return false;
					}
					std::cout << "Parsing template: " << fileNameOrig << std::endl;
				}
			}
			if (fileEnum)
			{
				std::vector<std::string> enumData;
				std::string line;
				while (std::getline(fileEnum, line))
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
						enumData.push_back(prefix);
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
				fileEnum.close();
			}
			if (fileStruct)
			{
				std::vector<std::string> structData;
				std::string line;
				while (std::getline(fileStruct, line))
				{
					std::string dataType, variableName;
					while (line.find(" [") != std::string::npos)
						line.replace(line.find(" ["), 2, "[");
					while (line.find("[ ") != std::string::npos)
						line.replace(line.find("[ "), 2, "[");
					while (line.find(" ]") != std::string::npos)
						line.replace(line.find(" ]"), 2, "]");
					while (line.find(" ;") != std::string::npos)
						line.replace(line.find(" ;"), 2, "");
					// Trim leading and trailing spaces
					line.erase(0, line.find_first_not_of(" "));
					line.erase(line.find_last_not_of(" ") + 1);

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
						structData.push_back(prefix);
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
				fileStruct.close();
			}
			if (fileVariables)
			{
				std::vector<std::string> variablesData;
				std::string line;
				variablesData.push_back(prefix);
				while (std::getline(fileVariables, line))
				{
					std::string dataType, variableName;
					while (line.find("\t") != std::string::npos)
						line.replace(line.find("\t"), 1, "");

					while (line.find(" [") != std::string::npos)
						line.replace(line.find(" ["), 2, "[");
					while (line.find("[ ") != std::string::npos)
						line.replace(line.find("[ "), 2, "[");
					while (line.find(" ]") != std::string::npos)
						line.replace(line.find(" ]"), 2, "]");
					while (line.find(" ;") != std::string::npos)
						line.replace(line.find(" ;"), 2, ";");
					// Trim leading and trailing spaces
					line.erase(0, line.find_first_not_of(" "));
					line.erase(line.find_last_not_of(" ") + 1);
					std::string tmp = line;
					if (tmp.find("[") != std::string::npos)
					{
						size_t startPos = tmp.find_first_not_of(' ');
						size_t pos = tmp.find(' ', startPos);
						tmp = tmp.substr(startPos, pos - startPos);
						if (line.find("char") == std::string::npos && isNativeType(tmp))
							continue;
					}

					//is line empty
					if (line.empty() || line.find('{') != std::string::npos) {
						continue; // Skip empty, whitespace-only lines
					}
					else
					{
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

						if (!isNativeType(dataType))
						{
							if (!isEnum(enums, dataType))
							{
								if (isStruct(structs, dataType))
								{
									for (const auto& var : structs)
									{
										if (var.size() > 1 && var[1] == dataType)
										{
											int i = 0;
											for (const auto& iner : var)
											{
												if (i < 2)
												{
													i++;
													continue;
												}
												if (i % 2 != 0)
												{
													variablesData.push_back(variableName + "." + iner);
												}
												else
												{
													variablesData.push_back(iner);
												}
												i++;
											}
										}
									}
								}
								else
								{
									std::cout << "Invalid data type '" << dataType << "'. Please fix " << fileNameOrig << " template" << std::endl;
									return false;
								}
							}
						}
					}
					if (!isStruct(structs, dataType)) // structure variables were already pushed
					{
						variablesData.push_back(dataType);
						variablesData.push_back(variableName);
					}
				}
				variables.push_back(variablesData);
				continue;

			}
			else
			{
				std::cout << "Template " << prefix << "Variables.tmpl is missing" << std::endl;
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
		return false;
	}
	return true;
}
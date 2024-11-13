// OODBGenerator.cpp :  This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <filesystem> 
#include <algorithm> 
#include <set>
#include <vector>

std::string headerFl;
std::string sourceFl;
std::string projectFl;

void loadHeaderTempl();
void loadSourceTemplate();
void loadProjectTempl();
std::string toLower(const std::string& str);

int FormatDisplay(std::string& display,
	std::string  enumsFl,
	std::string structsFl,
	std::string vars);
std::string datatypes =
" short int long long int"
" signed short signed int signed long signed long int signed long long int"
" unsigned short unsigned int unsigned long unsigned long int unsigned long long int"
" float long double";

void paddSpaces(std::string& arrayFl);
namespace fs = std::filesystem;

///////////////////////////////
int main(int argc, char* argv[])
{
	std::string enumMap;
	std::string enumsFl;
	std::string arrayFl;
	std::string varsFl;
	std::string structsFl;

	// Create the directory for the source and header files
	std::string cmmandLn;
	cmmandLn = "mkdir \"../../SYSCPPCPSource\"";
	system(cmmandLn.c_str());
#ifdef  __linux__
	cmmandLn = "mkdir \"../../SYSCPPCPmake\"";
#elif _WIN32
	cmmandLn = "mkdir \"../../SYSCPPCPvcxproj\"";
#endif
	system(cmmandLn.c_str());

	std::set<std::string> uniquePrefixes;  // Using set to ensure uniqueness
	std::vector<std::string> result;       // Final vector of unique prefixes
	std::string directoryPath = "../templates";

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
	for (const auto& recName : result) {
		std::cout << recName << std::endl;

		enumMap.clear();
		enumsFl.clear();
		arrayFl.clear();
		varsFl.clear();
		structsFl.clear();
		headerFl.clear();
		sourceFl.clear();
		projectFl.clear();

		loadHeaderTempl();
		loadSourceTemplate();
		loadProjectTempl();

		/////////
		std::string commandLn;

		namespace fs = std::filesystem;

		std::string enums = "../templates/" + recName + "Enum.tmpl";
		std::string structs = "../templates/" + recName + "Struct.tmpl";
		std::string vars = "../templates/" + recName + "Variables.tmpl";
		std::string header = "../../SYSCPPCPheaders/" + recName + ".h";
		std::string source = "../../SYSCPPCPSource/" + recName + ".cpp";
#ifdef __linux__	
		std::string project = "../../SYSCPPCPmake/makefile_" + recName;
#elif _WIN32
		std::string project = "../../SYSCPPCPvcxproj/" + recName + ".vcxproj";
#else
		std::cout << "Unknown platform" << std::endl;
#endif
		std::string headerEx = "../SYSCPPCPheaders/" + recName + ".h";
		std::string sourceEx = "../SYSCPPCPSource/" + recName + ".cpp";

		std::cerr << "Templates used to generate the code:" << std::endl;
		std::ifstream file;
		if (fs::exists(vars))
		{
			std::cout << "       " + vars << std::endl;
			file.open(vars);
			if (!file) {
				std::cerr << "Error: Could not open template '" + vars + "'" << std::endl;
				return 1;
			}
			std::string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			varsFl = tmp;
			file.close();
			while (varsFl.find("\r") != std::string::npos)
				varsFl.replace(varsFl.find("\r"), 1, "");
		}
		else {
			std::cout << "File " + vars + "was not found.\n";
			return 1;
		}

		if (fs::exists(enums))
		{
			std::cout << "       " + enums << std::endl;

			file.open(enums);
			if (!file) {
				std::cerr << "Error: Could not open template.'" + enums + "'" << std::endl;
				return 1;
			}
			std::string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			enumsFl = tmp;

			while (enumsFl.find("\r") != std::string::npos)
				enumsFl.replace(enumsFl.find("\r"), 1, "");
			std::string line;
			file.seekg(0, std::ios::beg);
			std::string temp;
			while (std::getline(file, line))
			{
				while (line.find(" ") != std::string::npos)
					line.replace(line.find(" "), 1, "");
				while (line.find("\t") != std::string::npos)
					line.replace(line.find("\t"), 1, "");
				while (line.find(",") != std::string::npos)
					line.replace(line.find(","), 1, "");
				if (line == "{" || line.empty())
					continue;
				if (line.substr(0, 4) == "enum")
					arrayFl += "static const char* " + line.substr(4, line.size() - 4) + "Array [] = {\n";
				else if (line == "};")
				{
					arrayFl[arrayFl.size() - 3] = ' ';
					arrayFl += "};\n";
				}
				else
					arrayFl += "\t\"" + line + "\", \n";
				//create map

				if (line.find("enum") != std::string::npos)
				{
					temp = line;
					temp.replace(temp.find("enum"), 4, "");
					continue;
				}
				if (line == "};")
					continue;

				enumMap += "enumMap[\"" + temp + "::" + line + "\"] = " + line + ";\n";

			}
			paddSpaces(arrayFl);
			file.close();

		}

		if (fs::exists(structs))
		{
			std::cout << "       " + structs << std::endl;
			file.open(structs);
			if (!file) {
				std::cerr << "Error: Could not open template. '" + structs + "'" << std::endl;
				return 1;
			}
			std::string tmp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			structsFl = tmp;
			file.close();
			while (structsFl.find("\r") != std::string::npos)
				structsFl.replace(structsFl.find("\r"), 1, "");

		}
		std::cerr << "Files generated:" << std::endl;
		std::cout << "       " + header << std::endl;
		std::cout << "       " + source << std::endl;
		std::cout << "       " + project << std::endl;
		std::cerr << "Compiled library for '" + recName + "' record" << std::endl;

		std::cout << "       ../SYSCPPCPlibs/" + recName + ".lib" << std::endl;


		std::string key;
		std::string keys;
		std::string keysDef;
		std::string tmpl;
		std::string display;

		// Create project direcotry
#ifdef __linux__
while (projectFl.find("+++ProjectName") != std::string::npos)
{
	projectFl.replace(projectFl.find("+++ProjectName"), strlen("+++ProjectName"), recName);
}
#elif _WIN32
projectFl.replace(projectFl.find("+++ProjectName"), strlen("+++ProjectName"), recName);
projectFl.replace(projectFl.find("+++Header"), strlen("+++Header"), headerEx);
projectFl.replace(projectFl.find("+++Source"), strlen("+++Source"), sourceEx);
#else
std::cout << "Unknown platform" << std::endl;
#endif
std::ofstream fileOut(project, std::ios::out | std::ios::trunc);
// Check if the file was successfully opened
if (!fileOut) {
	std::cerr << "Error: Could not open the file for writing." << std::endl;
	return 1;
}
fileOut << projectFl;
fileOut.close();

while (headerFl.find("+++NewRecord") != std::string::npos)
headerFl.replace(headerFl.find("+++NewRecord"), strlen("+++NewRecord"), recName);
while (sourceFl.find("+++NewRecord") != std::string::npos)
sourceFl.replace(sourceFl.find("+++NewRecord"), strlen("+++NewRecord"), recName);

headerFl.replace(headerFl.find("+++enums"), strlen("+++enums"), "\r\n" + enumsFl + "\r\n");
headerFl.replace(headerFl.find("+++arrays"), strlen("+++arrays"), "\r\n" + arrayFl + "\r\n");
headerFl.replace(headerFl.find("+++structures"), strlen("+++structures"), structsFl + "\r\n");
headerFl.replace(headerFl.find("+++variables"), strlen("+++variables"), "\r\n" + varsFl + "\r\n");
// recKeys
while (varsFl.find("\r") != std::string::npos)
varsFl.replace(varsFl.find("\r"), 1, "");
while (varsFl.find("\n") != std::string::npos)
varsFl.replace(varsFl.find("\n"), 1, "");
while (varsFl.find("\t") != std::string::npos)
varsFl.replace(varsFl.find("\t"), 1, " ");
while (varsFl.find("  ") != std::string::npos)
varsFl.replace(varsFl.find("  "), 2, " ");
while (varsFl.find(" ;") != std::string::npos)
varsFl.replace(varsFl.find(" ;"), 2, ";");
while (varsFl.find(" ]") != std::string::npos)
varsFl.replace(varsFl.find(" ]"), 2, "]");
while (varsFl.find(" [") != std::string::npos)
varsFl.replace(varsFl.find(" ["), 2, "[");
while (varsFl.find(" Key") != std::string::npos)
varsFl.replace(varsFl.find(" Key"), 4, "Key");
if (varsFl.find("const") != std::string::npos)
{
	std::cout << "'const' variables are not supported.  Please remove from template" << std::endl;
	return 1;
}
if (varsFl.find("static") != std::string::npos)
{
	std::cout << "'static' variables are not supported. Please remove from template" << std::endl;
	return 1;
}

std::string token;
for (int i = 0; i < varsFl.length(); i++)
{
	if (i == 0 && varsFl[i] == ' ')
		continue;
	if (varsFl[i] == ' ')
	{
		if (token.find(";") != std::string::npos ||
			enumsFl.find(" " + token) != std::string::npos ||
			structsFl.find(" " + token) != std::string::npos ||
			token == "char" ||
			token == "int" ||
			token == "short" ||
			token == "signed" ||
			token == "unsigned" ||
			token == "long" ||
			token == "float" ||
			token == "doble")
		{
			token = "";
			continue;
		}
		else
		{
			std::cout << "'" << token << "' datatype is not supported. Fix '" << vars << "'" << std::endl;
			return 1;
		}
	}
	token += varsFl[i];


}
bool flg = false;
for (int i = 0; i < varsFl.length(); i++)
{
	if (flg || varsFl[i] == '[' || varsFl[i] == ']')
	{
		if (varsFl[i] == '[')
			flg = true;
		if (varsFl[i] == ']')
			flg = false;
		varsFl.replace(i, 1, "");
		i--;
	}
}

std::istringstream stream(varsFl);
std::string tmp;

// Split the string based on the delimiter ';'
while (std::getline(stream, token, ';'))
{
	if (token.size() < 3)
		continue;
	// Display the extracted token
	if (!token.empty())
	{
		// get var name and type
		std::string type;
		std::string var;

		// Find the position of the last space
		size_t pos = token.rfind(' ');

		// Split the string into two parts
		type = token.substr(0, pos);        // Left part before the space
		while (type.find(" ") != std::string::npos)
			type.replace(type.find(" "), 1, "");
		var = token.substr(pos + 1);      // Right part after the space
		if (structsFl.find("struct " + type) != std::string::npos)
		{

			// Struct name to look for
			std::string structName = "struct " + type;
			size_t structPos = structsFl.find(structName);

			if (structPos != std::string::npos) {
				// Find the opening and closing braces for the struct
				size_t openBracePos = structsFl.find('{', structPos);
				size_t closeBracePos = structsFl.find('}', openBracePos);

				if (openBracePos != std::string::npos && closeBracePos != std::string::npos) {
					// Extract the body of the struct
					std::string structBody = structsFl.substr(openBracePos + 1, closeBracePos - openBracePos - 1);

					// Split the struct body by lines to extract member variables
					std::vector<std::string> members;
					size_t pos = 0;
					while ((pos = structBody.find(';')) != std::string::npos) {
						std::string line = structBody.substr(0, pos);
						structBody.erase(0, pos + 1);

						// Trim whitespace from the line
						line.erase(0, line.find_first_not_of(" \t\n\r"));
						line.erase(line.find_last_not_of(" \t\n\r") + 1);

						// Check if the line declares a char array
						if (line.find("char") == 0) {
							// Extract the member name
							size_t nameStart = line.find(' ', 4); // Find space after "char "
							if (nameStart != std::string::npos) {
								std::string memberName = line.substr(nameStart + 1);
								// Remove array notation if present
								size_t arrayStart = memberName.find('[');
								if (arrayStart != std::string::npos) {
									memberName = memberName.substr(0, arrayStart);
								}
								// Trim any extra whitespace
								memberName.erase(0, memberName.find_first_not_of(" \t\n\r"));
								memberName.erase(memberName.find_last_not_of(" \t\n\r") + 1);

								// Add to the members list
								if (!memberName.empty()) {
									members.push_back(memberName);
								}
							}
						}
						else
						{
							if (line.find("[") != std::string::npos)   // Only arrays of characters are supported
								continue;

							// Extract the member name
							size_t nameStart = line.find(' ', 0); // Find space after type
							if (nameStart != std::string::npos) {
								std::string memberName = line.substr(nameStart + 1);
								// Remove array notation if present

								// Trim any extra whitespace
								memberName.erase(0, memberName.find_first_not_of(" \t\n\r"));
								memberName.erase(memberName.find_last_not_of(" \t\n\r") + 1);

								// Add to the members list
								if (!memberName.empty()) {
									members.push_back(memberName);
								}
							}
						}
					}

					// Display the results as "var.memberName"
					for (const auto& memberName : members) {
						keys += "recKey ";
						keys += var;
						keys += memberName;
						keys += "Key;\n";
						keysDef += ", ";
						keysDef += var;
						keysDef += memberName;
						keysDef += "Key";
						keysDef += "(offsetof(DBData,";
						keysDef += var;
						keysDef += ".";
						keysDef += memberName;
						keysDef += ") - sizeof(int) - REC_NAME_SIZE, sizeof(data.";
						keysDef += var;
						keysDef += ".";
						keysDef += memberName;
						keysDef += "), typeid(data.";
						keysDef += var;
						keysDef += ".";
						keysDef += memberName;
						keysDef += "))\n";

					}
				}
				else {
					std::cout << "Struct body not found." << std::endl;
				}
			}
			else {
				std::cout << "Struct Name not found in the input string." << std::endl;
			}
		}
		else
		{
			keys += "recKey ";
			keys += var;
			keys += "Key;\n";
			keysDef += ", ";
			keysDef += var;
			keysDef += "Key";
			keysDef += "(offsetof(DBData,";
			keysDef += var;
			keysDef += ") - sizeof(int) - REC_NAME_SIZE, sizeof(data.";
			keysDef += var;
			keysDef += "), typeid(data.";
			keysDef += var;
			keysDef += "))\n";
		}
	}
}

if (FormatDisplay(display, enumsFl, structsFl, vars) != 0)
return 1;
sourceFl.replace(sourceFl.find("+++enumMap"), strlen("+++enumMap"), enumMap);
sourceFl.replace(sourceFl.find("+++Display"), strlen("+++Display"), display);
headerFl.replace(headerFl.find("+++recKeys"), strlen("+++recKeys"), keys);
sourceFl.replace(sourceFl.find("+++recKeysDef"), strlen("+++recKeysDef"), keysDef);
sourceFl.replace(sourceFl.find("+++recKeysDef"), strlen("+++recKeysDef"), keysDef);

// Open the file in overwrite mode (default is std::ios::out, 
// which includes std::ios::trunc)
std::ofstream file6(header, std::ios::out | std::ios::trunc);
// Check if the file was successfully opened
if (!file6) {
	std::cerr << "Error: Could not open the file for writing." << std::endl;
	return 1;
}
else
std::cerr << "File " << header << "opened for writing." << std::endl;

// Write the content to the file
file6 << headerFl;

// Close the file
file6.close();

std::ofstream file7(source, std::ios::out | std::ios::trunc);
// Check if the file was successfully opened
if (!file7) {
	std::cerr << "Error: Could not open the file for writing." << std::endl;
	return 1;
}
else
std::cerr << "File " << source << "opened for writing." << std::endl;


// Write the content to the file
file7 << sourceFl;

// Close the file
file7.close();

//build
#ifdef __linux__		
commandLn = "make -C ../../SYSCPPCPmake/ -f makefile_" + recName + " debug";
#elif _WIN32
commandLn = "msbuild ../../SYSCPPCPvcxproj/" + recName + ".vcxproj /p:Configuration=Debug /p:Platform=x64";
#else
std::cout << "Unknown platform" << std::endl;
#endif
int res = system(commandLn.c_str());

// Check if the command executed successfully
if (res == 0) {
	std::cout << "Build succeeded!" << std::endl;
}
else {
	std::cerr << "Build failed with error code: " << res << std::endl;
	return 1;
}
#ifdef __linux__		
commandLn = "make -C ../../SYSCPPCPmake/ -f makefile_" + recName + " release";
#elif _WIN32
commandLn = "msbuild ../../SYSCPPCPvcxproj/" + recName + ".vcxproj /p:Configuration=Release /p:Platform=x64";
#else
std::cout << "Unknown platform" << std::endl;
#endif
res = system(commandLn.c_str());

// Check if the command executed successfully
if (res == 0) {
	std::cout << "Build succeeded!" << std::endl;
}
else {
	std::cerr << "Build failed with error code: " << res << std::endl;
	return 1;
}
	}


	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////
// Helper function to convert a string to lowercase
std::string toLower(const std::string& str) {
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lowerStr;
}
std::string extractVariableName(const std::string& declaration) {
	std::istringstream iss(declaration);
	std::vector<std::string> tokens;
	std::string token;

	// Split the declaration by spaces and store tokens
	while (iss >> token) {
		tokens.push_back(token);
	}

	if (!tokens.empty()) {
		std::string variableName = tokens[tokens.size() - 1]; // Last token might include array size or semicolon

		// Handle cases with array sizes (e.g., "variableName[20];")
		size_t bracketPos = variableName.find('[');
		if (bracketPos != std::string::npos) {
			// The variable name already includes the array size, just remove the semicolon if present
			if (variableName.back() == ';') {
				variableName.pop_back();  // Remove trailing semicolon
			}
		}
		else {
			// If it's not an array, remove the semicolon if present
			if (!variableName.empty() && variableName.back() == ';') {
				variableName.pop_back();  // Remove trailing semicolon
			}
		}

		return variableName;
	}

	return "";  // Return empty string if no variable name found
}

int FormatDisplay(std::string& display,
	std::string  enumsFl,
	std::string structsFl,
	std::string vars)
{
	display = "std::cout << \" Primary Key     \" << data.primaryKey << std::endl;\n";
	// Open the file for reading
	std::ifstream inputFile(vars);

	// Check if the file was opened successfully
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open the file." << std::endl;
	}
	std::string line;
	std::string varName;
	std::string type;
	// Read the file line by line
	while (std::getline(inputFile, line))
	{
		while (line.find("\t") != std::string::npos)
			line.replace(line.find("\t"), 1, " ");
		while (line.find("  ") != std::string::npos)
			line.replace(line.find("  "), 2, " ");
		while (line.find(" ;") != std::string::npos)
			line.replace(line.find(" ;"), 2, ";");
		while (line.find("; ") != std::string::npos)
			line.replace(line.find("; "), 2, ";");
		while (line.find(" ]") != std::string::npos)
			line.replace(line.find(" ]"), 2, "]");
		while (line.find(" [") != std::string::npos)
			line.replace(line.find(" ["), 2, "[");
		while (line.find("[ ") != std::string::npos)
			line.replace(line.find("[ "), 2, "[");

		size_t start = line.find_first_not_of(" \t\n\r\f\v");  // Find first non-whitespace character
		line = (start == std::string::npos) ? "" : line.substr(start);  // Return trimmed string
		varName = extractVariableName(line);
		size_t i = line.length();
		if (i != 0)
		{
			if (line[i - 2] != ']')
			{
				while (line[i] != ' ')
				{
					if (i == 0)
					{
						std::cout << "Invalid " << vars << ", missing datatype for '" << line << "'" << std::endl;
						return 1;
					}
					i--;
				}

				type = line.substr(0, i);
				if (datatypes.find(" " + type + " ") != std::string::npos)
				{
					std::string tmp = "std::cerr << \"" + type + "  " + varName + "   \"  << data." + varName + "<< std::endl;\n";
					display += tmp;
				}
				else
				{
					while (enumsFl.find("  ") != std::string::npos)
						enumsFl.replace(enumsFl.find("  "), 2, " ");
					if (enumsFl.find("enum " + type) != std::string::npos)
					{
						std::string tmp = "std::cout << \"" + type + "  " + varName + "\" <<    " + type + "Array[data." + varName + "] << std::endl;\n";
						display += tmp;
					}
					else
					{
						size_t pos;
						while (structsFl.find("\t") != std::string::npos)
							structsFl.replace(structsFl.find("\t"), 1, " ");
						while (structsFl.find("  ") != std::string::npos)
							structsFl.replace(structsFl.find("  "), 2, " ");
						while (structsFl.find("\r") != std::string::npos)
							structsFl.replace(structsFl.find("\r"), 1, " ");
						while (structsFl.find(" \n") != std::string::npos)
							structsFl.replace(structsFl.find(" \n"), 2, "\n");
						while (structsFl.find(" [") != std::string::npos)
							structsFl.replace(structsFl.find(" ["), 2, "[");

						if (pos = structsFl.find("struct " + type) != std::string::npos)
						{
							std::string ln, l;
							ln = "struct " + type + "\n";
							int i = 0;
							bool flg = false;
							while (i < structsFl.size())
							{
								if (structsFl[i] == '\n')
								{
									if (l == "struct " + type)
									{
										display += "std::cout << \"struct " + type + "\" <<  std::endl;\n";
										flg = true;
									}
									else if (flg && l != "")
									{
										std::string t, q;
										int j = l.size() - 1;
										while (j)
										{
											if (l[j] != ';' && l[j] != ' ')
												t += l[j];
											if (l[j] == ' ')
												break;
											j--;
										}
										std::reverse(t.begin(), t.end());
										j = 0;
										while (j < t.size())
										{
											if (t[j] == ';' || t[j] == '[' || t[j] == ']')
												break;
											q += t[j];
											j++;
										}

										display += "std::cout << \"    " + l + "       \" << data." + varName + "." + q + " << std::endl;\n";
									}
									l = "";
								}
								else
								{

									if (structsFl[i] == '}' && flg)
										break;
									else if (structsFl[i] != '{')
										l += structsFl[i];


								}
								i++;
							}
						}
						else
						{
							std::cerr << "Invalid data type " << type << ".Please fix the templates" << std::endl;
						}
					}

				}
			}
			else
			{
				if (line.find("char") != std::string::npos)
				{
					i = 5;
					while (line[i] != '[')
						i++;
					varName = line.substr(5, i - 5);
					std::string tmp = "std::cerr << \"" + line + "   \"  <<data." + varName + "<< std::endl;\n";
					display += tmp;
				}
				else
				{
					std::string arrSz;
					size_t arrSzNum;
					i = line.length();

					while (line[i] != '[')
						i--;
					size_t j = i;
					while (line[j] != ' ')
						j--;
					varName = line.substr(j + 1, i - j - 1);
					arrSz = line.substr(i + 1, line.length() - i - 3);
					type = line.substr(0, j);
					arrSzNum = std::stoi(arrSz);
					if (datatypes.find(" " + type + " ") != std::string::npos)
					{
						std::string tmp = "std::cerr << \"" + line + "   \" << std::endl;\n";
						display += tmp;
						display += "std::cerr << \"------------------\" << std::endl;\n";
						for (i = 0; i < arrSzNum; i++)
						{
							std::string tmp = "std::cerr << \"    " + varName + "[" + std::to_string(i) + "]\"  << data." + varName + "[" + std::to_string(i) + "]" + " << std::endl;\n";
							display += tmp;
						}
						display += "std::cerr << \"------------------\" << std::endl;\n";
					}
					else
					{

					}
				}
			}
		}
	}
	// Close the file
	inputFile.close();
	return 0;
}

void paddSpaces(std::string& arrayFl)
{
	std::vector<std::vector<std::string>> stringMatrix;
	// Use a string stream to parse the string by '\n'
	std::istringstream stream(arrayFl);
	std::string line;
	std::vector<std::string> lines;

	// Parse the string line by line
	while (std::getline(stream, line)) {
		lines.push_back(line);
	}
	bool start = false;
	std::vector<std::string> row;
	std::vector<size_t> maxLn;
	size_t  sz = 0;
	for (auto& l : lines) {
		if (!start && l.find("{") != std::string::npos)
			start = true;
		else if (start && l.find("}") != std::string::npos)
		{
			start = false;
			stringMatrix.push_back(row);
			row.clear();
			maxLn.push_back(sz);
			sz = 0;
		}
		else if (l.empty() || std::all_of(l.begin(), l.end(), [](unsigned char c) { return std::isspace(c); }))
		{
			continue;
		}
		else
		{
			while (l.find(" ") != std::string::npos)
				l.replace(l.find(" "), 1, "");
			while (l.find("\"") != std::string::npos)
				l.replace(l.find("\""), 1, "");
			while (l.find("\n") != std::string::npos)
				l.replace(l.find("\n"), 1, "");
			while (l.find("\t") != std::string::npos)
				l.replace(l.find("\t"), 1, "");
			while (l.find(",") != std::string::npos)
				l.replace(l.find(","), 1, "");
			if (sz < l.length())
				sz = l.length();
			row.push_back(l);
			row.push_back(l);
		}
		// padd 
		int i = 0;
		int j = 0;
		for (auto& row : stringMatrix) {
			for (auto& str : row) {
				if (j % 2 != 0)
					str = str + std::string(maxLn[i] - str.length(), ' ');
				j++;
			}
			i++;//std::cout << std::endl;  // Newline after each row
		}
	}
	int j = 0;
	std::string toFind;

	for (auto& row : stringMatrix) {
		for (const auto& str : row) {
			if (j % 2 != 0)
				arrayFl.replace(arrayFl.find(toFind), toFind.length(), str);
			else
				toFind = str;
			j++;
		}
		j = 0;
	}
}
void loadHeaderTempl()
{

	headerFl =
		"#pragma once   \n"
		"#include \"Record.h\"   \n\n"
		"#include <unordered_map>\n"
		"//Boilerplate code starts here \n"
		"//Do not modify this code. \n"
		"+++enums\n"
		"+++arrays\n"
		"+++structures\n"
		"class +++NewRecord : public Record   \n"
		"{   \n"
		"public:   \n"
		"    +++NewRecord();   \n"
		"    +++NewRecord(const +++NewRecord& other);   \n"
		"    ~+++NewRecord() override = default;   \n"
		"	 static int AddFactory(); //Used internaly by the library\n"
		"    static Record * factory(void); // Used internaly by the library.  Do not call directly \n"
		"    struct DBData   \n"
		"    {   \n"
		"        DBData()   \n"
		"        {   \n"
		"            RecSize = sizeof(DBData);   \n"
#ifdef __linux__	
		"            std::string prettyFunction = __PRETTY_FUNCTION__;  //     \n"
		"            std::string outerClassName = prettyFunction.substr(0, prettyFunction.find(\"::DBData\"));   \n"
		"   		 std::strncpy(RecName, outerClassName.c_str(), sizeof(RecName) - 1);\n"
		"            RecName[sizeof(RecName) - 1] = 0; // Ensure null termination \n"
#else
		"            std::string prettyFunction = __FUNCTION__;  //     \n"
		"            std::string outerClassName = prettyFunction.substr(0, prettyFunction.find(\"::DBData\"));   \n"
		"            strncpy_s(RecName, sizeof(RecName), outerClassName.c_str(), _TRUNCATE);   \n"
#endif	
		"            std::memset(reinterpret_cast<char*>(this) + sizeof(RecSize) + REC_NAME_SIZE, 0, sizeof(DBData) - sizeof(RecSize) - REC_NAME_SIZE);      \n"
		"        }   \n"
		"        int RecSize;  // size of DBData \n"
		"       char RecName[REC_NAME_SIZE];  // The name of this class \n"
		"        long long int primaryKey; // unique key generated by the system when the record is inserted in the database or zero if not in the database\n"
		"//IMPORTANT: Do not add or remove variables after records have been written to the database.\n"
		" //  By doing so you will break the database.  \n"
		" +++variables\t} data; // This structure will ve stored to the database using Record::Insert\n"
		"    recKey  pidxKey;   \n"
		" +++recKeys // Keys used by Record::Seek base class function\n\n"
		"    char recName[REC_NAME_SIZE]; // Name of this class  \n"
		"   \n"
		"protected:   \n"
		"    char *  GetDataAddress(void) override; // Called from base class Record  \n"
		"    long    GetDataSize(void) override ;   // Called from base class Record\n"
		"    const char* GetRecName(void) override ;  // Called from base class Record \n"
		"    void SetPrimaryKey(long long int pidx)  override;// Called from base class Record\n"
		"    int GetEnumValue(std::string key)  override;// Called from base class Record\n"
		"public:   \n"
		"    int Dump(void) override; //print the declarations and values of the DBData structure.  \n"
		"    long long int GetPrimaryKey(void) override;\n"
		"//Boilerplate code ends here \n\n"
		"//NOTE:\n//  You may add other members to the class here.\n"
		"//To avoid modifying this code unintentionally it is recommended to write the application code in a derived class.\n"
		"};\n";


}

void loadSourceTemplate()
{
	sourceFl =

		"#include \"+++NewRecord.h\"    \n"
		"#include <string>    \n"
		"//Boilerplate code starts here. \n"
		"//Do not modify this code. \n"
		"static int dummy = +++NewRecord::AddFactory();\n"
		"+++NewRecord::+++NewRecord() :                                                                          \n"
		"	Record(),    \n"
		"		pidxKey(offsetof(DBData, primaryKey) - sizeof(int) - REC_NAME_SIZE, sizeof(data.primaryKey), typeid(data.primaryKey))  \n"
		"//NOTE:\n//   You can add or remove keys initialization here.  \n"
		"//   Update the header file accordingly.\n"
		"+++recKeysDef   \n"
		"{                                                                   \n"
#ifdef __linux__
		"	std::strncpy(recName, data.RecName, sizeof(recName) - 1); \n"
		"   recName[sizeof(recName) - 1] = 0; // Ensure null termination \n"
#else 
		"	strncpy_s(recName, sizeof(recName), data.RecName, _TRUNCATE);    \n"
#endif
		"   data.primaryKey = -1;\n"
		"}                                                                   \n"
		"//copy constructor                                                 \n"
		"+++NewRecord::+++NewRecord(const +++NewRecord & other)         \n"
		": Record(other),    \n"
		"		pidxKey(offsetof(DBData, primaryKey) - sizeof(int) - REC_NAME_SIZE, sizeof(data.primaryKey), typeid(data.primaryKey))  \n"
		" +++recKeysDef"
		"{																	 \n"
		"     // Copy DBData structure										 \n"
		"     std::memcpy(&this->data, &other.data, sizeof(DBData));		 \n"
		"																	 \n"
		"     // Copy the record name										 \n"
#ifdef __linux__
		" 	std::strncpy(this->recName, other.recName, sizeof(this->recName) - 1); \n"
		"   this->recName[sizeof(this->recName) - 1] = 0; // Ensure null termination \n"
#else
		"     strncpy_s(this->recName, sizeof(this->recName), other.recName, _TRUNCATE);\n"
#endif		
		"}\n"
		"\n"
		"char* +++NewRecord::GetDataAddress(void)       \n"
		"{                                                \n"
		"	return reinterpret_cast<char *>(&data);       \n"
		"}                                                \n"
		"long +++NewRecord::GetDataSize(void)             \n"
		"{                                                \n"
		"	return sizeof(DBData);                        \n"
		"}                                                \n"
		"const char* +++NewRecord::GetRecName(void)       \n"
		"{                                                \n"
		"	return recName;                               \n"
		"}                                                \n"
		"long long +++NewRecord::GetPrimaryKey(void)    \n"
		"{                                                \n"
		"	return data.primaryKey;                     \n"
		"}                                                           \n"
		"void +++NewRecord::SetPrimaryKey(long long int pidx)      \n"
		"{                                                           \n"
		"	data.primaryKey = pidx;       \n"
		"}  \n"
		"int +++NewRecord::GetEnumValue(std::string key) \n"
		"{\n"
		"std::unordered_map<std::string, int> enumMap;\n"
		"+++enumMap\n"
		"    auto it = enumMap.find(key);\n"
		"    if (it != enumMap.end()) {\n"
		"	    return it->second;\n"
		"    }\n"
		"    return -1;\n"
		"}\n"
		"int +++NewRecord::AddFactory()\n"
		"{\n"
		"      Record::getRecordFactory()[\"+++NewRecord\"] = &(+++NewRecord::factory);\n"
		"      return 0;\n"
		"}\n"
		"Record * +++NewRecord::factory()\n"
		"{\n"
		"    +++NewRecord * rec = new +++NewRecord(); \n"
		"    std::string val = std::to_string(Record::PrIdx); \n"
		"    rec->pidxKey.value = val;\n"
		"    rec->Seek(&rec->pidxKey, nullptr);\n"
		"    return rec;\n"
		"}\n"
		"//Boilerplate code ends here. \n"
		"//NOTE:\n//   You may modify the Display function or add member functions as needed here.\n"
		"int +++NewRecord::Dump(void)                             \n"
		"{                                                           \n"
		"	std::cout << \"Class Name '\" << this->GetRecName() << \"'\" << std::endl;     \n"
		"   if(IsDeleted()) \n"
		"   {\n"
		"         std::cout << \"This record was deleted\" << std::endl;\n"
		"   }\n"
		"   else \n"
		"   {\n"
		"	+++Display\n"
		"   }\n"
		"	std::cout << \"========================= \" << std::endl;                   \n"
		"	return 1;                                                                  \n"
		"} \n";
}

void loadProjectTempl()
{
	projectFl =
#ifdef __linux__
		"# Makefile for Linux (Ubuntu) Project Converted from Visual Studio Project\n"
		"\n"
		"# Compiler\n"
		"CXX = g++\n"
		"\n"
		"# Compiler flags\n"
		"CXXFLAGS = -Wall -Wextra -std=c++2a  -Wno-unused-parameter -Wno-unused-variable    \n"
		"DEBUGFLAGS = -g -DDEBUG\n"
		"RELEASEFLAGS = -O2 -DNDEBUG\n"
		"\n"
		"# Directories\n\n"
		"SRCDIR = ../SYSCPPCPSource\n"
		"INCLUDEDIR = ../SYSCPPCPheaders\n"
		"DEBUGDIR = ../SYSCPPCPlibs/debug\n"
		"RELEASEDIR = ../SYSCPPCPlibs/release\n"
		"\n"
		"# Source and object files\n"
		"SRC = $(SRCDIR)/+++ProjectName.cpp\n"
		"OBJ_DEBUG = $(patsubst $(SRCDIR)/%.cpp,$(DEBUGDIR)/%.o,$(SRC))\n"
		"OBJ_RELEASE = $(patsubst $(SRCDIR)/%.cpp,$(RELEASEDIR)/%.o,$(SRC))\n"
		"\n"
		"# Targets\n"
		"TARGET_DEBUG = $(DEBUGDIR)/lib+++ProjectNamed.a\n"
		"TARGET_RELEASE = $(RELEASEDIR)/lib+++ProjectName.a\n"
		"\n"
		"# Default target\n"
		".PHONY: all\n"
		"default: debug\n"
		"\n"
		"# Debug target\n"
		"debug: $(TARGET_DEBUG)\n"
		"\n"
		"$(TARGET_DEBUG): $(OBJ_DEBUG)\n"
		"	@mkdir -p $(DEBUGDIR)\n"
		"	ar rcs $@ $^\n"
		"\n"
		"$(DEBUGDIR)/%.o: $(SRCDIR)/%.cpp\n"
		"	@mkdir -p $(DEBUGDIR)\n"
		"	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -I$(INCLUDEDIR) -c $< -o $@\n"
		"\n"
		"# Release target\n"
		"release: $(TARGET_RELEASE)\n"
		"\n"
		"$(TARGET_RELEASE): $(OBJ_RELEASE)\n"
		"	@mkdir -p $(RELEASEDIR)\n"
		"	ar rcs $@ $^\n"
		"\n"
		"$(RELEASEDIR)/%.o: $(SRCDIR)/%.cpp\n"
		"	@mkdir -p $(RELEASEDIR)\n"
		"	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -I$(INCLUDEDIR) -c $< -o $@\n"
		"\n"
		"# Clean\n"
		".PHONY: clean\n"
		"clean:\n"
		"	find $(DEBUGDIR) $(RELEASEDIR) -type f ! -name 'libSYSCPPCPd.a' ! -name 'libSYSCPPCP.a' -exec rm -f {} +\n";
#else
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">   \n"
		"    <PropertyGroup>\n"
		"       <NoWarn> MSB8028</NoWarn>\n"
		"    </PropertyGroup> \n"

		"  <ItemGroup Label=\"ProjectConfigurations\">                \n"
		"    <ProjectConfiguration Include=\"Debug|Win32\">           \n"
		"      <Configuration>Debug</Configuration>\n"
		"      <Platform>Win32</Platform>          \n"
		"    </ProjectConfiguration>               \n"
		"    <ProjectConfiguration Include=\"Release|Win32\">         \n"
		"      <Configuration>Release</Configuration>                 \n"
		"      <Platform>Win32</Platform>          \n"
		"    </ProjectConfiguration>               \n"
		"    <ProjectConfiguration Include=\"Debug|x64\">             \n"
		"      <Configuration>Debug</Configuration>\n"
		"      <Platform>x64</Platform>            \n"
		"    </ProjectConfiguration>               \n"
		"    <ProjectConfiguration Include=\"Release|x64\">           \n"
		"      <Configuration>Release</Configuration>                 \n"
		"      <Platform>x64</Platform>            \n"
		"    </ProjectConfiguration>               \n"
		"  </ItemGroup>         \n"
		"  <PropertyGroup Label=\"Globals\">       \n"
		"    <VCProjectVersion>17.0</VCProjectVersion>                \n"
		"    <Keyword>Win32Proj</Keyword>          \n"
		"    <ProjectGuid>{71cd25f1-d323-4e8b-8068-69c9a79dca6b}</ProjectGuid>           \n"
		"    <RootNamespace>+++ProjectName</RootNamespace>            \n"
		"    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>           \n"
		"  </PropertyGroup>     \n"
		"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />          \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">                 \n"
		"    <ConfigurationType>StaticLibrary</ConfigurationType>     \n"
		"    <UseDebugLibraries>true</UseDebugLibraries>              \n"
		"    <PlatformToolset>v143</PlatformToolset>                  \n"
		"    <CharacterSet>Unicode</CharacterSet>  \n"
		"  </PropertyGroup>     \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">               \n"
		"    <ConfigurationType>StaticLibrary</ConfigurationType>     \n"
		"    <UseDebugLibraries>false</UseDebugLibraries>             \n"
		"    <PlatformToolset>v143</PlatformToolset>                  \n"
		"    <WholeProgramOptimization>true</WholeProgramOptimization>\n"
		"    <CharacterSet>Unicode</CharacterSet>  \n"
		"  </PropertyGroup>     \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">\n"
		"    <ConfigurationType>StaticLibrary</ConfigurationType>     \n"
		"    <UseDebugLibraries>true</UseDebugLibraries>              \n"
		"    <PlatformToolset>v143</PlatformToolset>                  \n"
		"    <CharacterSet>Unicode</CharacterSet>  \n"
		"  </PropertyGroup>     \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">                 \n"
		"    <ConfigurationType>StaticLibrary</ConfigurationType>     \n"
		"    <UseDebugLibraries>false</UseDebugLibraries>             \n"
		"    <PlatformToolset>v143</PlatformToolset>                  \n"
		"    <WholeProgramOptimization>true</WholeProgramOptimization>\n"
		"    <CharacterSet>Unicode</CharacterSet>  \n"
		"  </PropertyGroup>     \n"
		"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />                  \n"
		"  <ImportGroup Label=\"ExtensionSettings\">                  \n"
		"  </ImportGroup>       \n"
		"  <ImportGroup Label=\"Shared\">          \n"
		"  </ImportGroup>       \n"
		"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">                  \n"
		"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />       \n"
		"  </ImportGroup>       \n"
		"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">                \n"
		"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />       \n"
		"  </ImportGroup>       \n"
		"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\"> \n"
		"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />       \n"
		"  </ImportGroup>       \n"
		"  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">                  \n"
		"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />       \n"
		"  </ImportGroup>       \n"
		"  <PropertyGroup Label=\"UserMacros\" />  \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">     \n"
		"    <OutDir>..\\SYSCPPCPlibs\\debug\\</OutDir>               \n"
		"    <TargetName>$(ProjectName)d</TargetName>                 \n"
		"    <IntDir>.\\temp\\$(Platform)\\$(Configuration)\\</IntDir>\n"
		"  </PropertyGroup>     \n"
		"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">   \n"
		"    <OutDir>..\\SYSCPPCPlibs\\release\\</OutDir>             \n"
		"    <IntDir>.\\temp\\$(Platform)\\$(Configuration)\\</IntDir> \n"
		"  </PropertyGroup>     \n"
		"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">                \n"
		"    <ClCompile>        \n"
		"      <WarningLevel>Level3</WarningLevel> \n"
		"      <SDLCheck>true</SDLCheck>           \n"
		"      <PreprocessorDefinitions>WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>                 \n"
		"      <ConformanceMode>true</ConformanceMode>                \n"
		"      <PrecompiledHeader>NotUsing</PrecompiledHeader>        \n"
		"      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>   \n"
		"    </ClCompile>       \n"
		"    <Link>             \n"
		"      <SubSystem>      \n"
		"      </SubSystem>     \n"
		"      <GenerateDebugInformation>true</GenerateDebugInformation>                 \n"
		"    </Link>            \n"
		"  </ItemDefinitionGroup>                  \n"
		"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">              \n"
		"    <ClCompile>        \n"
		"      <WarningLevel>Level3</WarningLevel> \n"
		"      <FunctionLevelLinking>true</FunctionLevelLinking>      \n"
		"      <IntrinsicFunctions>true</IntrinsicFunctions>          \n"
		"      <SDLCheck>true</SDLCheck>           \n"
		"      <PreprocessorDefinitions>WIN32;NDEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>                 \n"
		"      <ConformanceMode>true</ConformanceMode>                \n"
		"      <PrecompiledHeader>NotUsing</PrecompiledHeader>        \n"
		"      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>   \n"
		"    </ClCompile>       \n"
		"    <Link>             \n"
		"      <SubSystem>      \n"
		"      </SubSystem>     \n"
		"      <EnableCOMDATFolding>true</EnableCOMDATFolding>        \n"
		"      <OptimizeReferences>true</OptimizeReferences>          \n"
		"      <GenerateDebugInformation>true</GenerateDebugInformation>                 \n"
		"    </Link>            \n"
		"  </ItemDefinitionGroup>                  \n"
		"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">                  \n"
		"    <ClCompile>        \n"
		"      <WarningLevel>Level3</WarningLevel> \n"
		"      <SDLCheck>true</SDLCheck>           \n"
		"      <PreprocessorDefinitions>_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>    \n"
		"      <ConformanceMode>true</ConformanceMode>                \n"
		"      <PrecompiledHeader>NotUsing</PrecompiledHeader>        \n"
		"      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>   \n"
		"      <AdditionalIncludeDirectories>../SYSCPPCPheaders</AdditionalIncludeDirectories>              \n"
		"    </ClCompile>       \n"
		"    <Link>             \n"
		"      <SubSystem>      \n"
		"      </SubSystem>     \n"
		"      <GenerateDebugInformation>true</GenerateDebugInformation>                 \n"
		"    </Link>            \n"
		"  </ItemDefinitionGroup>                  \n"
		"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">                \n"
		"    <ClCompile>        \n"
		"      <WarningLevel>Level3</WarningLevel> \n"
		"      <FunctionLevelLinking>true</FunctionLevelLinking>      \n"
		"      <IntrinsicFunctions>true</IntrinsicFunctions>          \n"
		"      <SDLCheck>true</SDLCheck>           \n"
		"      <PreprocessorDefinitions>NDEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>    \n"
		"      <ConformanceMode>true</ConformanceMode>                \n"
		"      <PrecompiledHeader>NotUsing</PrecompiledHeader>        \n"
		"      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>   \n"
		"      <AdditionalIncludeDirectories>../SYSCPPCPheaders/</AdditionalIncludeDirectories>             \n"
		"    \n"
		"    </ClCompile>       \n"
		"    <Link>             \n"
		"      <SubSystem>      \n"
		"      </SubSystem>     \n"
		"      <EnableCOMDATFolding>true</EnableCOMDATFolding>        \n"
		"      <OptimizeReferences>true</OptimizeReferences>          \n"
		"      <GenerateDebugInformation>true</GenerateDebugInformation>                 \n"
		"    </Link>            \n"
		"  </ItemDefinitionGroup>                  \n"
		"  <ItemGroup>          \n"
		"    <ClInclude Include=\"+++Header\" />   \n"
		"  </ItemGroup>         \n"
		"  <ItemGroup>          \n"
		"    <ClCompile Include=\"+++Source\" />    \n"
		"  </ItemGroup>         \n"
		"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />                \n"
		"  <ImportGroup Label=\"ExtensionTargets\">\n"
		"  </ImportGroup>       \n"
		"</Project>             \n";

#endif


}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

extern std::vector<std::vector<std::string>> enums;
extern std::vector<std::vector<std::string>> structs;
extern std::vector<std::vector<std::string>> variables;

bool isNativeType(std::string type);

bool genCommon(std::vector<std::string>& prefixes)
{
    std::ifstream inputFile("..\\TemplatesSmallSQL\\common.cpp");
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }
    std::string InitializeVector;
    std::string InitializeDesc;

    for (const std::string& prefix : prefixes)
    {

        InitializeVector += "    vecOfVecs.push_back(std::vector<std::string>{\"" + prefix + "\",";

        for (const auto& row : variables) {
            if (row[0] != prefix)
                continue;
            int i = 0;
            InitializeVector += "\"primaryKey\"";
            std::string dataType;
            for (const auto& str : row) {
                if (i == 0 || i % 2 != 0)
                {
                    dataType = str;
                    i++;
                    continue;
                }
                if (isNativeType(dataType)&& dataType.find("char") == std::string::npos)
                {
                    if (dataType.find("[") != std::string::npos)
                    {
                        i++;
                        continue;
                    }
                }
                InitializeVector += ", \"" + str + "\"";
                i++;
            }
            InitializeVector += ", \"*\"";
            InitializeVector += "});\n";
        }

        ////////////////////////////////
        InitializeDesc += "\nvecOfDesc.push_back(std::vector<std::string>{\"" + prefix + "\",\n";
        std::string fileName = "..\\templates\\" + prefix + "Enum.tmpl";
        std::ifstream inputFile(fileName);
        std::string buffer2;
        if (inputFile) {
            std::ostringstream buffer1;
            buffer2 += "Enumerations\n===========\n";
            buffer1 << inputFile.rdbuf();  // Read file 1 into buffer
            buffer2 += buffer1.str();  // Append content to combinedContent
            buffer2 += "\n";
            inputFile.close();
        }
        fileName = "..\\templates\\" + prefix + "Struct.tmpl";
        inputFile.clear();
        inputFile.open(fileName);
        if (inputFile) {
            std::ostringstream buffer1;
            buffer2 += "Structures\n=========\n";
            buffer1 << inputFile.rdbuf();  // Read file 1 into buffer
            buffer2 += buffer1.str();  // Append content to combinedContent
            buffer2 += "\n";
            inputFile.close();
        }

        fileName = "..\\templates\\" + prefix + "Variables.tmpl";
        inputFile.clear();
        inputFile.open(fileName);
        if (inputFile) {
            std::ostringstream buffer1;
            buffer2 += "Variables(columns)\n==================\n";
            buffer1 << inputFile.rdbuf();  // Read file 1 into buffer
            buffer2 += buffer1.str();  // Append content to combinedContent
            buffer2 += "\n";
            inputFile.close();
        }
        else
        {
               std::cout << "Missing template " << fileName << std::endl;
        }
        // Step 1: Replace all occurrences of "\r\n" with "\n"
        std::string::size_type pos = 0;
        while ((pos = buffer2.find("\r\n", pos)) != std::string::npos) {
            buffer2.replace(pos, 2, "\n");
            pos += 1;  // Move past the replacement
        }
        // Step 2: Enclose each line with quotes, replace '\n' with "\n", and remove blank lines
        std::string outputText;
        std::istringstream stream(buffer2);
        std::string line;

        while (std::getline(stream, line)) {
            // Remove lines that are empty or contain only whitespace
            if (line.empty() || std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); })) {
                continue;
            } 

            // Enclose each line in quotes and add a newline character
            while (line.find("\t") != std::string::npos)
                line.replace(line.find("\t"), 1, "    ");
            while (line.find("     ") != std::string::npos)
                line.replace(line.find("     "), 5, "    ");
            // check here for array of native types
            std::string tmp = line;
            if (tmp.find("[") != std::string::npos)
            {
                size_t startPos = tmp.find_first_not_of(' ');
                size_t pos = tmp.find(' ', startPos);
                tmp = tmp.substr(startPos, pos - startPos);
                if (line.find("char") == std::string::npos && isNativeType(tmp))
                    line += "   // Not supported in queries";
            }

            outputText += "\"" + line + "\\n\"\n";
           
        }

        // Step 3: Remove the final newline character to handle the last line properly
        if (!outputText.empty() && outputText.back() == '\n') {
            outputText.pop_back();
        }


        InitializeDesc += outputText;
        
        InitializeDesc += " });\n";
    }

    std::ostringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    std::string content = fileContent.str();

    size_t InitializeVectorPos = content.find("###InitializeVector");
    if (InitializeVectorPos != std::string::npos) {
        content.replace(InitializeVectorPos, std::string("###InitializeVector").length(), InitializeVector);
    }

    size_t InitializeDescPos = content.find("###InitializeDesc");
    if (InitializeDescPos != std::string::npos) {
        content.replace(InitializeDescPos, std::string("###InitializeDesc").length(), InitializeDesc);
    }

    // Write the updated content back to the file
    std::ofstream outputFile("..\\..\\SmallSQLsource\\common.cpp");
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    outputFile << content;
    outputFile.close();

    return true;
}
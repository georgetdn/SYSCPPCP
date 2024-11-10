#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

bool genSmallSQL(std::vector<std::string> &prefixes)
{
    std::ifstream inputFile("../TemplatesSmallSQL/SmallSQL.cpp");
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }
    std::string includes;
    std::string declarations;
    int i = 1;
    for (const std::string& prefix : prefixes)
    {
        includes += "#include \"" + prefix + "Der.h\"\n";
        declarations += prefix + "Der Rec" + std::to_string(i) + ";\n";

            i++;
    }
    std::ostringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    std::string content = fileContent.str();

    size_t includePos = content.find("###includes");
    if (includePos != std::string::npos) {
        content.replace(includePos, std::string("###includes").length(), includes);
    }

    size_t declarationPos = content.find("###declarations");
    if (declarationPos != std::string::npos) {
        content.replace(declarationPos, std::string("###declarations").length(), declarations);
    }

    // Write the updated content back to the file
    std::ofstream outputFile("../../SmallSQLsource/SmallSQL.cpp");
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    outputFile << content;
    outputFile.close();
    return true;
}

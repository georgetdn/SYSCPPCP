#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

bool genProcessDelete(std::vector<std::string>& prefixes)
{
    std::ifstream inputFile("../TemplatesSmallSQL/ProcessDelete.cpp");
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }
    std::string includes;
    std::string declarations;
    std::string ifs;
    int i = 1;
    for (const std::string& prefix : prefixes)
    {
        includes += "#include \"" + prefix + "Der.h\"\n";
        declarations += "extern "+ prefix + "Der Rec" + std::to_string(i) + ";\n";
        if (i > 1)
            ifs += "    else if";
        else 
            ifs += "if ";
        ifs += "(it->first == \""+prefix+"\")\n    {\n        Rec = &Rec" + std::to_string(i) + ";\n    }\n";

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
    size_t ifsPos = content.find("###ifs");
    if (ifsPos != std::string::npos) {
        content.replace(ifsPos, std::string("###ifs").length(), ifs);
    }




    // Write the updated content back to the file
    std::ofstream outputFile("../../SmallSQLSource/ProcessDelete.cpp");
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    outputFile << content;
    outputFile.close();


    return true;
}
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

bool genProjectFile(std::vector<std::string>& prefixes)
{
    std::ifstream inputFile("../TemplatesSmallSQL/makefile");
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }
    std::string SourceFiles;
    std::string LibsDebug;
    std::string LibsRelease;
    
    int i = 1;
    for (const std::string& prefix : prefixes)
    {
        SourceFiles += "   $(SRCDIR)/"+ prefix +"Der.cpp ";
        LibsDebug += "-l" + prefix + "d ";
        LibsRelease +=  "-l" + prefix + " ";
        if(i == 3)
        {
             i = 1;
             SourceFiles += " \\\n";
             continue;        
        }
	i++;
    }
    std::ostringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    std::string content = fileContent.str();

    size_t ItemGroupSourcePos = content.find("###SourceFiles");
    if (ItemGroupSourcePos != std::string::npos) {
        content.replace(ItemGroupSourcePos, std::string("###SourceFiles").length(), SourceFiles);
    }

    size_t ItemGroupHeadersPos = content.find("###LibsDebug");
    if (ItemGroupHeadersPos != std::string::npos) {
        content.replace(ItemGroupHeadersPos, std::string("###LibsDebug").length(), LibsDebug);
    }
    size_t AdditionalDepDebugPos = content.find("###LibsRelease");
    if (AdditionalDepDebugPos != std::string::npos) {
        content.replace(AdditionalDepDebugPos, std::string("###LibsRelease").length(), LibsRelease);
    }

    // Write the updated content back to the file
    std::ofstream outputFile("../../SYSCPPCPmake/makefile_SQL");
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    outputFile << content;
    outputFile.close();


    return true;
}

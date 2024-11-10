#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

bool genProjectFile(std::vector<std::string>& prefixes)
{
    std::ifstream inputFile("../TemplatesSmallSQL/SmallSQL.vcxproj");
    if (!inputFile) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return false;
    }
    std::string ItemGroupSource;
    std::string ItemGroupHeaders;
    std::string AdditionalDepDebug;
    std::string AdditionalDepRelease;
    int i = 1;
    for (const std::string& prefix : prefixes)
    {
        ItemGroupSource += "    <ClCompile Include=\"../SmallSQLsource/"+ prefix +"Der.cpp\" />\n";
        ItemGroupHeaders +=  "    <ClInclude Include=\"../SmallSQLHeaders/" + prefix + "Der.h\" />\n";
        AdditionalDepDebug += prefix + "d.lib;";
        AdditionalDepRelease += prefix + ".lib;";
    }
    std::ostringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    std::string content = fileContent.str();

    size_t ItemGroupSourcePos = content.find("###ItemGroupSource");
    if (ItemGroupSourcePos != std::string::npos) {
        content.replace(ItemGroupSourcePos, std::string("###ItemGroupSource").length(), ItemGroupSource);
    }

    size_t ItemGroupHeadersPos = content.find("###ItemGroupHeaders");
    if (ItemGroupHeadersPos != std::string::npos) {
        content.replace(ItemGroupHeadersPos, std::string("###ItemGroupHeaders").length(), ItemGroupHeaders);
    }
    size_t AdditionalDepDebugPos = content.find("###AdditionalDepDebug");
    if (AdditionalDepDebugPos != std::string::npos) {
        content.replace(AdditionalDepDebugPos, std::string("###AdditionalDepDebug").length(), AdditionalDepDebug);
    }
    size_t AdditionalDepReleasePos = content.find("###AdditionalDepRelease");
    if (AdditionalDepReleasePos != std::string::npos) {
        content.replace(AdditionalDepReleasePos, std::string("###AdditionalDepRelease").length(), AdditionalDepRelease);
    }


    // Write the updated content back to the file
    std::ofstream outputFile("../../SYSCPPCPvcxproj/SmallSQL.vcxproj");
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    outputFile << content;
    outputFile.close();


    return true;
}
#include "###prefixDer.h"
#include <string>

recKey * ###prefixDer::GetRecordKey(std::string var)
{

    if (var == "primaryKey")
    {
        return &pidxKey;
    }
    ###GetRecordKey
 
    else
    {
        std::cout << std::endl << "Syntax error: '" << var << "' is not a valid variable name. \n Use 'desc ###prefix' to see the member variables." << std::endl << std::endl;
        return nullptr;
    }
    return nullptr;

}
bool ###prefixDer::varcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << std::to_string(data.primaryKey) << std::string(11 - std::to_string(data.primaryKey).length(), ' ');

    ###varcout
    else
        return false;

    return true;
}
bool ###prefixDer::hedcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "primaryKey" << " ";
   ###hedcout
    else
        return false;

    return true;
}
bool ###prefixDer::sepcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "---------- ";
    ###sepcout

    else
        return false;

    return true;
}
bool ###prefixDer::SetValue(std::string col, std::string val)
{
    if (col == "primaryKey")
    {
        std::cout << std::endl << "Syntax error: 'primaryKey' variable cannot be set" << std::endl << std::endl;
        return false;
    }
    ###SetValue
    else
    {
        std::cout << std::endl << "Syntax error: '" << col << "' is not a valid variable name. \n Use 'desc ###prefix' to see the member variables." << std::endl << std::endl;
        return false;
    }

    return true;
}
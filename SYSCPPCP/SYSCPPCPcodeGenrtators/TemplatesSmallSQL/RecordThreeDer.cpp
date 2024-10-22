#include "RecordThreeDer.h"
#include <string>

recKey* RecordThreeDer::GetRecordKey(std::string var)
{

    if (var == "primaryKey")
    {
        return &pidxKey;
    }
    else if (var == "Number")
    {
        return &NumberKey;
    }
    else if (var == "CustomerID")
    {
        return &CustomerIDKey;
    }
    else if (var == "Date")
    {
        return & DateKey;
    }
    else
    {
        std::cout << std::endl << var << " is not a valid variable name. \n Use 'desc RecordOne' to see the member variables." << std::endl << std::endl;
        return nullptr;
    }
    return nullptr;

}
bool RecordThreeDer::varcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << std::to_string(data.primaryKey) << std::string(11 - std::to_string(data.primaryKey).length(), ' ');
    else if (var == "Number")
        std::cout << std::to_string(data.Number) << std::string(9 - std::to_string(data.Number).length(), ' ');
    else if (var == "CustomerID")
        std::cout << data.CustomerID << std::string(41 - strlen(data.CustomerID), ' ');
    else if (var == "Date")
        std::cout << data.Date << std::string(11 - strlen(data.Date), ' ');
    else
        return false;

    return true;
}
bool RecordThreeDer::hedcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "primaryKey" << " ";
    else if (var == "Number")
        std::cout << "Number" << "   ";
    else if (var == "CustomerID")
        std::cout << "CustomerID" << "                               ";
    else if (var == "Date")
        std::cout << "Date" << "       ";
    else
        return false;

    return true;
}
bool RecordThreeDer::sepcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "----------" << " ";
    else if (var == "Number")
        std::cout << "--------" << " ";
    else if (var == "CustomerID")
        std::cout << "----------------------------------------" << " ";
    else if (var == "Date")
        std::cout << "----------" << " ";
    else
        return false;

    return true;
}
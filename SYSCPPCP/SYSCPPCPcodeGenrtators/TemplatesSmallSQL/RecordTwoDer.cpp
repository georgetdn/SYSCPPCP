#include "RecordTwoDer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

recKey* RecordTwoDer::GetRecordKey(std::string var)
{

    if (var == "primaryKey")
    {
        return &pidxKey;
    }
    else if (var == "Number")
    {
        return &NumberKey;
    }
    else if (var == "ItemID")
    {
        return &ItemIDKey;
    }
    else if (var == "Quantity")
    {

    }
    else
    {
        std::cout << std::endl << var << " is not a valid variable name. \n Use 'desc RecordOne' to see the member variables." << std::endl << std::endl;
        return nullptr;
    }
    return nullptr;

}
bool RecordTwoDer::varcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << std::to_string(data.primaryKey) << std::string(11 - std::to_string(data.primaryKey).length(), ' ');
    else if (var == "Number")
        std::cout << std::to_string(data.Number) << std::string(11 - std::to_string(data.Number).length(), ' ');
    else if (var == "ItemID")
        std::cout << std::to_string(data.ItemID) << std::string(11 - std::to_string(data.ItemID).length(), ' ');
    else if (var == "Quantity")
        std::cout << std::to_string(data.Quantity) << std::string(11 - std::to_string(data.Quantity).length(), ' ');
    else
        return false;

    return true;
}
bool RecordTwoDer::hedcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "primaryKey" << " ";
    else if (var == "Number")
        std::cout << "Number" << "     ";
    else if (var == "ItemID")
        std::cout << "ItemID" << "     ";
    else if (var == "Quantity")
        std::cout << "Quantity" << "    ";
    else
        return false;

    return true;
}bool RecordTwoDer::sepcout(std::string var)
{
    if (var == "primaryKey")
        std::cout << "----------" << " ";
    else if (var == "Number")
        std::cout << "----------" << " ";
    else if (var == "ItemID")
        std::cout << "----------" << " ";
    else if (var == "Quantity")
        std::cout << "----------" << " ";
    else
        return false;

    return true;
}
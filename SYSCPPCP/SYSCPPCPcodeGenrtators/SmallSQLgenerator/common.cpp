#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> nativeDataTypes = {
	"bool", "char", "char", "wchar_t", "char16_t", "char32_t",
	"short", "int", "long", "float", "double", "long double"
};


std::string toLower(const std::string& str) {
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lowerStr;
}

std::string toUpperCase(const std::string& str) {
	std::string upperStr = str;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
		[](unsigned char c) { return std::toupper(c); });

	return upperStr;
}
bool isNativeType(std::string type)
{
	// Check if one of the native data types is found in dataType
	bool found = false;
	for (const auto& nativeType : nativeDataTypes) {
		if (type.find(nativeType) != std::string::npos) {
			return true;
		}
	}
	return false;
}
bool isEnum(std::vector<std::vector<std::string>> &enums, std::string type)
{
	for (const auto& var : enums) {
		if (var.size() > 1 && var[1] == type) {
			return true;
		}
	}
	return false;
}
bool isStruct(std::vector<std::vector<std::string>> &structs, std::string type)
{
	for (const auto& var : structs) {
		if (var.size() > 1 && var[1] == type) {
			return true;
		}
	}
	return false;
}


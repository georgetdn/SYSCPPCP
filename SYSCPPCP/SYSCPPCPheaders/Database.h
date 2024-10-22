#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>


constexpr auto REC_NAME_SIZE = 20;


class Database 
{
public:
    // Constructor
    Database() = default;
    Database(std::string fileName);
    // Destructor
    ~Database();

    const std::string  GetDatabaseName();
    // Method to connect to the file
    std::fstream& Connect(std::string outFileName);
    bool IsOpen(void);
    int Close(void);
    int Dump();
    int Dump(std::string recName);
    long GetCount(void);
    std::fstream outFile;
private:
    std::string FileName;
};

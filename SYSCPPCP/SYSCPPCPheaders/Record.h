#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "Database.h"
#include <cstddef>  // For std::size_t and offsetof
#include <map>

//macro to initialize char [] strings
// for val use quotes
#define INIT_STR(varName, val) \
    strncpy_s(varName, sizeof(varName), val, sizeof(varName) - 1);

enum class Comp
{
    Equal,
    NotEqual,
    Greater,
    Smaller,
    GreaterEq,
    SmallerEq
};
enum class AndOr
{
    And,
    Or,
    Null  //for last recKey in the Seek statement
};
enum class OpResult
{
	True,
	False,
	Null   // error
};
class Database;
struct recKey
{
    recKey() : offset(0), sz(0), typeInfo(typeid(void)) // Provide a default type for typeInfo
    {
        comp = Comp::Equal;
        andOr = AndOr::Null;
    }
    const std::size_t offset;       // initialized in the derived class constrictor
    const int sz;                   // initialized in the derived class constrictor
    const std::type_info& typeInfo; // initialized in the derived class constrictor
    std::string value; // initialized when the key is initiated
    Comp comp;         // initialized when the key is initiated
    AndOr andOr;       // initialized when the key is initiated
    // Constructor to initialize members
    recKey(std::size_t offset, int sz,const std::type_info& typeInfo)
        : offset(offset), sz(sz), typeInfo(typeInfo)
    {
        comp = Comp::Equal;
        andOr = AndOr::Null; // last key in the comparison 
    }
    // Copy constructor
    recKey(const recKey& other)
        : offset(other.offset), sz(other.sz), typeInfo(other.typeInfo), value(other.value),
        comp(other.comp), andOr(other.andOr)
    {
        // All members are copied
    }
}; 

class Record
{
public:
    Record();
    Record(const Record& other);
    virtual ~Record()  = default;
public:
    // commands
    bool IsSaved(void);
    bool IsDeleted(void);
    bool Insert(void);
    bool Update(void);
    bool Delete(void);
    OpResult Seek(recKey* k1, ...);
    OpResult Next(recKey* k1, ...);
    OpResult  GetRecordByName(void);
    virtual int Dump(void) = 0;

    static Database * db;
    static void setDatabase(Database& db);
        
    static Record* GetRecordByIndex(long long ptIdx);
    // Static function to access the map (lazy initialization)
    static std::map<std::string, Record* (*)()>& getRecordFactory();
    static long long PrIdx;
    std::streampos recordDBAddress; // correcpondent record in the DB or NULL


protected:
    virtual char *  GetDataAddress(void) = 0;
    virtual long GetDataSize(void) = 0;
    virtual const char* GetRecName(void) = 0;
    virtual long long int GetPrimaryKey(void) = 0;
    virtual void SetPrimaryKey(long long int pidx) = 0;
    virtual int GetEnumValue(std::string key) = 0;;

private:
    OpResult processSeek(recKey* k, const  char  *);
    OpResult LastOpResult;
    AndOr LastAndOr;
    static std::string GetRecordName(long long prIdx);
public:
    //Functions used by SmallSQL
    virtual bool varcout(std::string var) { return false; };
    virtual bool hedcout(std::string var) { return false; };
    virtual bool sepcout(std::string var) { return false; };
    virtual bool SetValue(std::string col, std::string val) { return false; };
    virtual recKey* GetRecordKey(std::string var) { return nullptr; };
};

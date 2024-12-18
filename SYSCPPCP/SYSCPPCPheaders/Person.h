#pragma once   
#include "Record.h"   

#include <unordered_map>
//Boilerplate code starts here 
//Do not modify this code. 

enum MaritalStatus
{
    UnknownMaritalStatus,
	Maried,
    Divorced,
    Single,
	Widow
};
enum Relasionship
{
	UnknownRelasionship,
	Husband,
	Wife,
	GrandFather,
	GrandMother,
	Uncle,
	Aunt
};
enum PersonGender
{
	PeronUnknown,
	PersonMale,
	PersonFemale
};

enum PersonEyeColor
{
	PersUnknownEyeColor,
	PersBrownEyeColor,
	PersBlueEyeColor ,
	PersHazelEyeColor ,
	PersGreenEyeColor ,
	PersAmberEyeColor ,
	PersGrayEyeColor ,
	PersVioletEyeColor ,
	PersRedPinkEyeColor,
	PersBlackEyeColor 
};
enum PersonHairColor
{
	PersUnknownHairColor,
	PersBlackHairColor ,
	PersBrownHairColor ,
	PersBlondeHairColor ,
	PersRedHairColor ,
	PersGrayHairColor ,
	PersWhiteHairColor ,
	PersAuburnHairColor,
	PersChestnutHairColor ,
	PersBlueHairColor 
};


static const char* MaritalStatusArray [] = {
	"UnknownMaritalStatus", 
	"Maried              ", 
	"Divorced            ", 
	"Single              ", 
	"Widow               "  
};
static const char* RelasionshipArray [] = {
	"UnknownRelasionship", 
	"Husband            ", 
	"Wife               ", 
	"GrandFather        ", 
	"GrandMother        ", 
	"Uncle              ", 
	"Aunt               "  
};
static const char* PersonGenderArray [] = {
	"PeronUnknown", 
	"PersonMale  ", 
	"PersonFemale"  
};
static const char* PersonEyeColorArray [] = {
	"PersUnknownEyeColor", 
	"PersBrownEyeColor  ", 
	"PersBlueEyeColor   ", 
	"PersHazelEyeColor  ", 
	"PersGreenEyeColor  ", 
	"PersAmberEyeColor  ", 
	"PersGrayEyeColor   ", 
	"PersVioletEyeColor ", 
	"PersRedPinkEyeColor", 
	"PersBlackEyeColor  "  
};
static const char* PersonHairColorArray [] = {
	"PersUnknownHairColor ", 
	"PersBlackHairColor   ", 
	"PersBrownHairColor   ", 
	"PersBlondeHairColor  ", 
	"PersRedHairColor     ", 
	"PersGrayHairColor    ", 
	"PersWhiteHairColor   ", 
	"PersAuburnHairColor  ", 
	"PersChestnutHairColor", 
	"PersBlueHairColor    "  
};


struct PersonAge
{
     int years;
	 int months;
};
struct PersonName
{
	char salutation[6];
	char first[20];
	char middleInitial;
	char last [20];
	char title [6];
};


class Person : public Record   
{   
public:   
    Person();   
    Person(const Person& other);   
    ~Person() override = default;   
	 static int AddFactory(); //Used internaly by the library
    static Record * factory(void); // Used internaly by the library.  Do not call directly 
    struct DBData   
    {   
        DBData()   
        {   
            RecSize = sizeof(DBData);   
            std::string prettyFunction = __FUNCTION__;  //     
            std::string outerClassName = prettyFunction.substr(0, prettyFunction.find("::DBData"));   
            strncpy_s(RecName, sizeof(RecName), outerClassName.c_str(), _TRUNCATE);   
            std::memset(reinterpret_cast<char*>(this) + sizeof(RecSize) + REC_NAME_SIZE, 0, sizeof(DBData) - sizeof(RecSize) - REC_NAME_SIZE);      
        }   
        int RecSize;  // size of DBData 
       char RecName[REC_NAME_SIZE];  // The name of this class 
        long long int primaryKey; // unique key generated by the system when the record is inserted in the database or zero if not in the database
//IMPORTANT: Do not add or remove variables after records have been written to the database.
 //  By doing so you will break the database.  
 
		PersonName name;  
		PersonGender gender; 
		PersonAge age;
		MaritalStatus maritalStatus; 
		long long spouse;
		Relasionship relasionship;   
        char address1 [41];   
        char address2 [21];
		char city  [21];    
		char state  [3];    
		char zip [6];   
		char job[20];    
		float weight;   
		float hight;    
		PersonEyeColor eyeColor; 
		PersonHairColor hairColor; 

	} data; // This structure will ve stored to the database using Record::Insert
    recKey  pidxKey;   
 recKey namesalutationKey;
recKey namefirstKey;
recKey namemiddleInitialKey;
recKey namelastKey;
recKey nametitleKey;
recKey genderKey;
recKey ageyearsKey;
recKey agemonthsKey;
recKey maritalStatusKey;
recKey spouseKey;
recKey relasionshipKey;
recKey address1Key;
recKey address2Key;
recKey cityKey;
recKey stateKey;
recKey zipKey;
recKey jobKey;
recKey weightKey;
recKey hightKey;
recKey eyeColorKey;
recKey hairColorKey;
 // Keys used by Record::Seek base class function

    char recName[REC_NAME_SIZE]; // Name of this class  
   
protected:   
    char *  GetDataAddress(void) override; // Called from base class Record  
    long    GetDataSize(void) override ;   // Called from base class Record
    const char* GetRecName(void) override ;  // Called from base class Record 
    void SetPrimaryKey(long long int pidx)  override;// Called from base class Record
    int GetEnumValue(std::string key)  override;// Called from base class Record
public:   
    int Dump(void) override; //print the declarations and values of the DBData structure.  
    long long int GetPrimaryKey(void) override;
//Boilerplate code ends here 

//NOTE:
//  You may add other members to the class here.
//To avoid modifying this code unintentionally it is recommended to write the application code in a derived class.
};

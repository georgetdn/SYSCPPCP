# SYSCPPCP #
## Simple Yet Sophisticated C++ Class Persistence or SYSCPPCP ##  
Store C++ class data in a file and manipulate it programmatically or using SmallSQL(included)  
## Requirements: ##
Linux or Windows 10, 11 and Visual Studio Community 2022 (64-bit) - Current Version 17.11.5  (VS Code is not supported).
## Note ##
This version of SYSCPPCP can store only native types to the database.  For a version handling std::string, std::vecotr and std::map send an email to george@tdnsys.com  
## Installing ## 
To install and build the Windows framework and SmallSQL application clone the project, go to SYSCPPCP directory in a Command Prompt or Power Shell window and execute the build.bat batch file. (./build for PS). The build will take about five minutes.  For Linux execute build.sh in a Bash shell. The sctipt first will install GNU Readline library requiring sudo.  
  
The build process will build a framework based on the sample templates from _\SYSCPPCP\SYSCPPCPcodeGenrtators\templates_ .
When you develop your application you will create a set of templates defining the classes in your application, build the framework and add code specific to your application.  More on this later.
  
## Getting familiar with the framework ##  
The best way to familiarize yourself with the framework is to execute the sample applications.
In Windows Explorer go to _\SYSCPPCP\SYSCPPCPtest\QuickStart_ and start _QuickStart.vcxproj_
Step through the code in debug mode.  You should be able to get an understanding of what actions you can perform.

Next, open a command prompt or power shell window and go to _SYSCPPCP\SmallSQL\releae and type_  __smallsql ../../syscppcp.dat__ (__./smallsql ../../syscppcp.dat__ for PS)  
You should get a SmallSQL prompt  
Type __cls__ to clean the screen  
Type SmallSQL> __select classes from dual__
```
    Classes(Tables)
    ===============
    Cat
    Child
    Customer
    Dog
    Family
    Invoice
    Item
    Person
```
This is a list the classes created during build based on the sample templates.  For each class a header _\SYSCPPCP\SYSCPPCPheaders_, a source file in _\SYSCPPCP\SYSCPPCPSource_, a project file in _\SYSCPPCP\SYSCPPCPvcxproj_ and a static library in _\SYSCPPCP\SYSCPPCPlibs_.  You will need them to build your application.
Type SmallSQL> __desc Customer__  
```
Enumerations
===========
  enum Type
   {
    Retail,
    WholeSale,
    OneTime
   };

Structures
========
struct Name
{
    char First[20];
    char Last [20];
};
Variables(columns)
==================
   int ID;
   Name name;
   char Address1 [41];
   char Address2 [21];
   char City  [21];
   char State  [3];
   char Zip [6];
   Type type ;
```
You will get the data members of the class - Variables(columns) -  an enumeration and structure declarations used to declare the variables 'type' and 'name'.
Try SmallSQL> __select * from Cat__
You will get a listing of all records of type Cat stored (serialized) to the database. The database file is _\SYSCPPCP\syscppcp.dat_.
Try
SmallSQL> __select name.First, name.Last from customer__  
Syntax Error: Table 'customer' does not exist.  
You get an error because class (table) names and variable(columns) names are case sensitive.  
The correct statement is   
__select name.First, name.Last from Customer__  
Notice that the column names are variable names as you would use them in a program.  
name is declared as a structure  
```
Name {
      char  First[20; 
      char Last [20];
    };
```
You can also try  
__select name.First, name.Last from Customer where State = NY and City = "New York"__  
Notice that only strings containing spaces need to be enclosed in quotes.  For  numbers and enumerations, no quotes or transformations are needed.
A few more rules,  
* Only one class(table) can be used in a query
* Maximum of five where clauses.
* Except for arrays of char, no arrays can be used in a query
* For enumerations you have to use the item name not the associated integer value
* You are not allowed to change the class variables declarations  

More examples  
SmallSQL> __update Customer set name.First = "Milton",  name.Last = "Gimbles" where ID = 4597__  
SmallSQL> __delete from Customer where type = OneTime__  
To exit type __bye__ or __exit__
__Note__: In your applications, as long as you don't change the generated code, you can declare other variables and functions.  The newly declared variables will not be stored in the database.
If you need more help read the tutorials.  You can also email me at george@tdnsys.com


 

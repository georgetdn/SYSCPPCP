#include "Database.h"
#include "Record.h"
#include "Customer.h"


int main() {
	//Open or create a new database
	Database db("../../syscppcp.dat"); 
	if (!db.IsOpen())
	{
		std::cout << "The database " << "../../syscppcp.dat" << " could not be opened. " << std::endl;
		return 1;
	}

	//Insert a record in the database
	std::cout << "Create a new instance of the Customer class and initialize the data members." << std::endl << std::endl;
	Customer rec;
	//Initialize the data members
	rec.data.ID = 4597;
	INIT_STR(rec.data.name.First, "David")
	INIT_STR(rec.data.name.Last, "Parks")
	INIT_STR(rec.data.Address1, "123 Park Ave")
	INIT_STR(rec.data.Address2, "Apt 11")
	INIT_STR(rec.data.City, "New York")
	INIT_STR(rec.data.State, "NY")
	INIT_STR(rec.data.Zip, "10009")
		std::cout << "Here is a dump of the record:" << std::endl << std::endl;
    ///////////
	rec.Dump();
	std::cout << std::endl <<  "A record that was not inserted in the database"  << std::endl; 
	std::cout << "has rec.data.primaryKey variable set to -1" << std::endl;
		std::cout << "and rec.recordDBaddress is std::streampos(-1)" << std::endl;
	std::cout << "You can check if the record is stored or not using rec.IsSaved(). " << std::endl; 

	if (rec.IsSaved())
		std::cout << "      Record was inserted in the database" << std::endl;
	else
		std::cout << "      Record was not inserted in the database" << std::endl;

	//Imsert the record///////////////////////////////////////////
	std::cout << std::endl << "Inserting the record in the database" << std::endl;
	////////////
	if (!rec.Insert())
		std::cout << "     ERROR inserting the record" << std::endl;
	else
		std::cout << "     Record was inserted in the database." << std::endl;
	std::cout << std::endl << "When the record is inserted in the database the rec.data.primaryKey"  << std::endl; 
	std::cout << "is set to an unique number."  << std::endl  << std::endl; 
	rec.Dump();

	// Same result can be achive using SmqllSQL
	// insert in Customer (ID, name.First, name.Last, Address1, Address2, State, Zip) values
	//                     (4597, David, Parks, "123 Park Ave", "Apt 11", NY, 10009)
	// Notice that SmallSQL requires quotes only for string with spaces
	
	// Update a record ///////////////////////////////////////////////
	// Set the values for the variables to be updated
	INIT_STR(rec.data.Address1, "59 5th ave.")
		INIT_STR(rec.data.Address2, "Suite 508")
		// and call update function
		std::cout << std::endl << "Updating the record." << std::endl;
		/////////////
		if (!rec.Update())
			std::cout << "ERROR updating the record" << std::endl;
		else
			std::cout << "Record was updated." << std::endl;

	// Or in SmqllSQL
	// UPDATE Customer set Address1 = "59 5th ave." , Address2 = "Suite 508"
	
	// Look up a record///////////////////////////////////////////////
	// set the keys
	std::cout << std::endl << "Serching for a record." << std::endl;
	rec.nameFirstKey.value = "David";
	rec.nameFirstKey.comp = Comp::Equal;
	rec.nameFirstKey.andOr = AndOr::And;

	rec.nameLastKey.value = "Parks";
	rec.nameLastKey.comp = Comp::Equal;

	// And preform the search.  It is important to notice that the record retrieved from the database
	// will override all the values in the record initiating the search
	    ////////////////////////////////////////////////////
	if (rec.Seek(&rec.nameFirstKey, rec.nameLastKey, nullptr) == OpResult::True)
	{
		std::cout << "Seek function returned a record " << std::endl;
		rec.Dump();   // to see the result
	}
	else
		std::cout << "Record not found." << std::endl;
	// Check if there are more
	std::cout << std::endl << "Serching the next record" <<  std::endl;
	       /////////////////////////////////////////////////////
	while (rec.Next(&rec.nameFirstKey, rec.nameLastKey, nullptr) == OpResult::True)
	{
		std::cout << std::endl << "Found another record" << std::endl << std::endl;
		rec.Dump();
	}
	//This is simillar to 
	// select * from Customer where name.First = David and name.Last = Parks
	// in SmallSQL

	// Delete the record///////////////////////////////////////
	std::cout << std::endl << "Deleting the record." << std::endl ;
    /////////////
	if(!rec.Delete())
		std::cout << "     ERROR deleted the record" << std::endl;
	else
		std::cout << "     Record was deleted from the database." << std::endl;

    // In SmallSQL  delete from Customer where name.First = David and name.Last = Parks
	std::cout << "When a record is deleted all variables a set to 0. " << std::endl;

	// 
	std::cout << "You can check if a record was deleted using rec.isDeleted()" << std::endl;
	if (rec.IsDeleted())
		std::cout << "Record was deleted from the database" << std::endl;
	else
		std::cout << "Record was not deleted in the database" << std::endl;

	std::cout << "Try to delete an already deleted record" << std::endl << std::endl;
	rec.Delete();
	std::cout << "Try to updating an already deleted record" << std::endl;
	rec.Update();
        ////////////////////////////////////////
	//Storring a std::string in the database
	std::string customerAddress = "555 Fifth Ave";
	Customer rec2;
	//Initialize the data members
	rec2.data.ID = 2345;
	INIT_STR(rec2.data.name.First, "Michael")
	INIT_STR(rec2.data.name.Last, "Serkins")
	INIT_STR(rec2.data.Address1, customerAddress.c_str())
	INIT_STR(rec2.data.Address2, "Apt 114")
	INIT_STR(rec2.data.City, "New York")
	INIT_STR(rec2.data.State, "NY")
	INIT_STR(rec2.data.Zip, "10009")
	rec2.Insert();
	// retreive the record and initialize the string
	Customer rec3;
	rec3.IDKey.value = "2345";
	if (rec3.Seek(&rec3.IDKey, nullptr) == OpResult::True)
	{
		std::cout << "Seek function returned a record " << std::endl;
		rec3.Dump();   // to see the result
	}
	else
		std::cout << "Record not found." << std::endl;
	// Rec2 and Rec3 point to the same record in the database
	customerAddress = rec3.data.Address1; // continue using Address1 as an stdL::string
	return 0;
}

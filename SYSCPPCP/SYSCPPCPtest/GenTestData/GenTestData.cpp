#include "Database.h"
#include "Record.h"
#include "Item.h"
#include "Customer.h"
#include "Invoice.h"
#include "Person.h"
#include "Child.h"
#include "Family.h"
#include "Cat.h"
#include "Dog.h"

#include <string>
void CreateTestData1(void);
void CreateData2(void);
void CreateData3(void);
void CreateTheSimpsonsFamily();

long long prKey;
long long prKey2;

long long prKey3;
std::streampos reco;
std::streampos reco2;
std::streampos reco3;

int main() {

#ifdef __linux__
	Database db("../../../../syscppcp.dat");
#else
	Database db("..\\..\\syscppcp.dat");
#endif
	if (!db.IsOpen())
	{
		std::cout << "DB not opened" << std::endl;
		return 1;
	}
	CreateTestData1();
	CreateData2();
	CreateData3();
	CreateTheSimpsonsFamily();

	Item rec;
	rec.data.Bin = 10;
	rec.data.ItemID = 5;
	INIT_STR(rec.data.Name, "New Item")
		rec.data.price = 12.35;
	rec.data.category = Category::Beverages;
	rec.data.UM = UnitsOfMeasure::Liter;
	rec.Insert();
	rec.Dump();


	/*
	Customer rec;
	rec.data.ID = 1234;
	rec.data.type = Type::Retail;
	strncpy_s(rec.data.name.First, sizeof(rec.data.name.First), "John", sizeof(rec.data.name.First) - 1);
	strncpy_s(rec.data.name.Last, sizeof(rec.data.name.Last), "Fender", sizeof(rec.data.name.Last) - 1);
	strncpy_s(rec.data.Address1, sizeof(rec.data.Address1), "1411 W. Kenedy dr", sizeof(rec.data.Address1) - 1);
	strncpy_s(rec.data.Address2, sizeof(rec.data.Address2), "Suit 16", sizeof(rec.data.Address2) - 1);
	strncpy_s(rec.data.City, sizeof(rec.data.City), "Wilmington", sizeof(rec.data.City) - 1);
	strncpy_s(rec.data.State, sizeof(rec.data.State), "MA", sizeof(rec.data.State) - 1);
	strncpy_s(rec.data.Zip, sizeof(rec.data.Zip), "23145", sizeof(rec.data.Zip) - 1);

	rec.Insert();

	//Use INIT_STR instead of strncpy_s

	rec.data.ID = 9567;
	INIT_STR(rec.data.name.First, "Vincent")
	INIT_STR(rec.data.name.Last, "Minelli")
	INIT_STR(rec.data.Address1, "12 Grand Ave")
	INIT_STR(rec.data.Address2, "")
	INIT_STR(rec.data.City, "Brooklin")
	INIT_STR(rec.data.State, "Fl")
	INIT_STR(rec.data.Zip, "90349")
	rec.data.type = Type::WholeSale;
	rec.Insert();






	std::cout << "Record count  " << db.GetCount() << std::endl;
	//RecordOne rec;
	db.Dump();
	RecordOne rec;
	//Insert a record in the database
	//Use INIT_STR instead of strncpy_s
	rec.data.ID = 0100;


		rec.Address1Key.value = "123 park ave";
	rec.Address1Key.comp = Comp::Equal;
	rec.Seek(&rec.Address1Key, nullptr);
	rec.Dump();

	if(rec.IsSaved())
		std::cout << "Record is saved" << std::endl;
	else
		std::cout << "Record is not saved" << std::endl;

	if (rec.IsDeleted())
		std::cout << "Record is deleted" << std::endl;
	else
		std::cout << "Record is not deleted" << std::endl;

	rec.Insert();

	if (rec.IsSaved())
		std::cout << "Record is saved" << std::endl;
	else
		std::cout << "Record is not saved" << std::endl;

	if (rec.IsDeleted())
		std::cout << "Record is deleted" << std::endl;
	else
		std::cout << "Record is not deleted" << std::endl;
	rec.Dump();
	rec.Delete();
	rec.Delete();

	rec.Dump();

	if (rec.IsSaved())
		std::cout << "Record is saved" << std::endl;
	else
		std::cout << "Record is not saved" << std::endl;
	if (rec.IsDeleted())
		std::cout << "Record is deleted" << std::endl;
	else
		std::cout << "Record is not deleted" << std::endl;

	// reuse the deletd instance of ReocrdOne


	rec.Update();
	rec.Dump();


	if (rec.IsDeleted())
		std::cout << "Record is deleted" << std::endl;
	else
		std::cout << "Record is not deleted" << std::endl;



		// retreive the rcord with prKey
	rec.Dump();
	RecordOne sick;
	sick.pidxKey.comp = Comp::Equal;
	sick.pidxKey.value = std::to_string(prKey);
	sick.pidxKey.andOr = AndOr::Null;
	sick.Seek(&sick.pidxKey, nullptr);
	sick.Dump();
		Record* rec = Record::GetRecordByIndex(prKey);
		RecordOne* r = dynamic_cast<RecordOne*>(rec);
		if (r)
		{
			r->Dump();

			INIT_STR(r->data.Address1, "Updated")
			INIT_STR(r->data.Address2, "Test")
			INIT_STR(r->data.City, "**************")
			INIT_STR(r->data.State, "CA")
			INIT_STR(r->data.Zip, "7654321")
			r->Update();
		}
		rec = Record::GetRecordByIndex(prKey);
		if (rec)
		{
			rec->Dump();
			rec->Delete();
			rec->Dump();
			rec = Record::GetRecordByIndex(prKey2);
			rec->Dump();
			rec = Record::GetRecordByIndex(prKey3);
			rec->Dump();
		}
		RecordOne results;
		results.StateKey.comp = Comp::Equal;
		results.StateKey.andOr = AndOr::And;
		results.StateKey.value = "NY";
		results.ZipKey.comp = Comp::Equal;
		results.ZipKey.value = "12345";
		results.Dump();

		if (results.Seek( nullptr) == OpResult::True)
		{
			std::cout << "Record retrieved using seek function" << std::endl;
			results.Dump();
		}
		else
		{
			std::cout << "Record not found" << std::endl;
		}
		while (results.Next(&results.StateKey, nullptr) == OpResult::True)
		{
			results.Dump();
			results.Delete();
		}
		db.Dump();
		// retreive the rcord with prKey
	 //   rec = Record::GetRecordByIndex(prKey);
	//	r = dynamic_cast<RecordOne*>(rec);
	*/
	return 0;
}

void CreateTestData1(void)
{
	Customer rec;
	Item rec2;

	rec.data.ID = 4597;
	INIT_STR(rec.data.name.First, "David")
		INIT_STR(rec.data.name.Last, "Parks")
		INIT_STR(rec.data.Address1, "123 Park Ave")
		INIT_STR(rec.data.Address2, "Apt 11")
		INIT_STR(rec.data.City, "New York")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "10009")

		rec.Insert();
	rec.data.ID = 4347;
	INIT_STR(rec.data.name.First, "MIke")
		INIT_STR(rec.data.name.Last, "Monty")
		INIT_STR(rec.data.Address1, "13 Carol street")
		INIT_STR(rec.data.Address2, "Suite 1")
		INIT_STR(rec.data.City, "New York")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "10091")

		rec.Insert();


	rec.data.ID = 3587;
	INIT_STR(rec.data.name.First, "Milan")
		INIT_STR(rec.data.name.Last, "Vidal")
		INIT_STR(rec.data.Address1, "123 Cester Ave")
		INIT_STR(rec.data.City, "Arlington")
		INIT_STR(rec.data.State, "VA")
		INIT_STR(rec.data.Zip, "23112")

		rec.Insert();

	rec.data.ID = 3456;  // Random number for ID
	INIT_STR(rec.data.name.First, "John")
		INIT_STR(rec.data.name.Last, "Doe")
		INIT_STR(rec.data.Address1, "15 Park Street")
		INIT_STR(rec.data.Address2, "Apt 2")
		INIT_STR(rec.data.City, "New York")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "10001")
		rec.data.type = Type::WholeSale;  // Type set to WholeSale
	rec.Insert();

	rec.data.ID = 7893;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Sarah")
		INIT_STR(rec.data.name.Last, "Connor")
		INIT_STR(rec.data.Address1, "500 Main Blvd")
		INIT_STR(rec.data.Address2, "Unit 12")
		INIT_STR(rec.data.City, "Los Angeles")
		INIT_STR(rec.data.State, "CA")
		INIT_STR(rec.data.Zip, "90210")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();

	rec.data.ID = 1298;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Bruce")
		INIT_STR(rec.data.name.Last, "Wayne")
		INIT_STR(rec.data.Address1, "100 Gotham Ave")
		INIT_STR(rec.data.Address2, "Wayne Manor")
		INIT_STR(rec.data.City, "Gotham")
		INIT_STR(rec.data.State, "NJ")
		INIT_STR(rec.data.Zip, "07001")
		rec.data.type = Type::OneTime;  // Type set to OneTime
	rec.Insert();

	rec.data.ID = 4587;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Clark")
		INIT_STR(rec.data.name.Last, "Kent")
		INIT_STR(rec.data.Address1, "300 Metropolis Rd")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Metropolis")
		INIT_STR(rec.data.State, "IL")
		INIT_STR(rec.data.Zip, "60605")
		rec.data.type = Type::WholeSale;  // Type set to WholeSale
	rec.Insert();

	rec.data.ID = 5469;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Peter")
		INIT_STR(rec.data.name.Last, "Parker")
		INIT_STR(rec.data.Address1, "20 Queens Plaza")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "New York")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "11101")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();

	rec.data.ID = 8321;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Diana")
		INIT_STR(rec.data.name.Last, "Prince")
		INIT_STR(rec.data.Address1, "10 Amazon Ave")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Themyscira")
		INIT_STR(rec.data.State, "FL")
		INIT_STR(rec.data.Zip, "32003")
		rec.data.type = Type::OneTime;  // Type set to OneTime
	rec.Insert();

	rec.data.ID = 1023;  // Random number for ID
	INIT_STR(rec.data.name.First, "Tony")
		INIT_STR(rec.data.name.Last, "Stark")
		INIT_STR(rec.data.Address1, "10880 Malibu Point")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Malibu")
		INIT_STR(rec.data.State, "CA")
		INIT_STR(rec.data.Zip, "90265")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();

	rec.data.ID = 7598;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Natasha")
		INIT_STR(rec.data.name.Last, "Romanoff")
		INIT_STR(rec.data.Address1, "123 Shield Rd")
		INIT_STR(rec.data.Address2, "Unit 7")
		INIT_STR(rec.data.City, "Washington")
		INIT_STR(rec.data.State, "DC")
		INIT_STR(rec.data.Zip, "20001")
		rec.data.type = Type::WholeSale;  // Type set to WholeSale
	rec.Insert();

	rec.data.ID = 6675;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Steve")
		INIT_STR(rec.data.name.Last, "Rogers")
		INIT_STR(rec.data.Address1, "569 Liberty Street")
		INIT_STR(rec.data.Address2, "Apt 1")
		INIT_STR(rec.data.City, "Brooklyn")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "11201")
		rec.data.type = Type::OneTime;  // Type set to OneTime
	rec.Insert();

	rec.data.ID = 9085;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Bruce")
		INIT_STR(rec.data.name.Last, "Banner")
		INIT_STR(rec.data.Address1, "45 Science Lane")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Dayton")
		INIT_STR(rec.data.State, "OH")
		INIT_STR(rec.data.Zip, "45402")
		rec.data.type = Type::WholeSale;  // Type set to WholeSale
	rec.Insert();

	rec.data.ID = 3201;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Wanda")
		INIT_STR(rec.data.name.Last, "Maximoff")
		INIT_STR(rec.data.Address1, "322 Witches Lane")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Westview")
		INIT_STR(rec.data.State, "NJ")
		INIT_STR(rec.data.Zip, "07450")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();

	rec.data.ID = 4257;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Scott")
		INIT_STR(rec.data.name.Last, "Lang")
		INIT_STR(rec.data.Address1, "100 Tiny Street")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "San Francisco")
		INIT_STR(rec.data.State, "CA")
		INIT_STR(rec.data.Zip, "94111")
		rec.data.type = Type::OneTime;  // Type set to OneTime
	rec.Insert();

	rec.data.ID = 1290;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Peter")
		INIT_STR(rec.data.name.Last, "Quill")
		INIT_STR(rec.data.Address1, "999 Galaxy Lane")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Star-Lord")
		INIT_STR(rec.data.State, "IN")
		INIT_STR(rec.data.Zip, "46201")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();

	rec.data.ID = 7843;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Stephen")
		INIT_STR(rec.data.name.Last, "Strange")
		INIT_STR(rec.data.Address1, "177A Bleecker Street")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "New York")
		INIT_STR(rec.data.State, "NY")
		INIT_STR(rec.data.Zip, "10012")
		rec.data.type = Type::WholeSale;  // Type set to WholeSale
	rec.Insert();

	rec.data.ID = 8732;  // Another random number for ID
	INIT_STR(rec.data.name.First, "Carol")
		INIT_STR(rec.data.name.Last, "Danvers")
		INIT_STR(rec.data.Address1, "42 Sky High Rd")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "New Orleans")
		INIT_STR(rec.data.State, "LA")
		INIT_STR(rec.data.Zip, "70112")
		rec.data.type = Type::OneTime;  // Type set to OneTime
	rec.Insert();

	rec.data.ID = 6583;  // Another random number for ID
	INIT_STR(rec.data.name.First, "T'Challa")
		INIT_STR(rec.data.name.Last, "Udaku")
		INIT_STR(rec.data.Address1, "108 Wakanda Blvd")
		INIT_STR(rec.data.Address2, "")
		INIT_STR(rec.data.City, "Wakanda")
		INIT_STR(rec.data.State, "GA")
		INIT_STR(rec.data.Zip, "30301")
		rec.data.type = Type::Retail;  // Type set to Retail
	rec.Insert();


}
void CreateData2(void)
{
	Invoice rec;
	rec.data.CustomerID = 4347;
	INIT_STR(rec.data.Date, "12/23/2024")
		rec.data.InvoiceNumber = 1000;
	rec.data.items[0] = 125;
	rec.data.items[1] = 116;
	rec.data.items[1] = 124;
	rec.Insert();// Test Data 1
	rec.data.CustomerID = 4597;
	INIT_STR(rec.data.Date, "01/12/2024")
		rec.data.InvoiceNumber = 1001;
	rec.data.items[0] = 101;
	rec.data.items[1] = 105;
	rec.data.items[2] = 123;
	rec.Insert();

	// Test Data 2
	rec.data.CustomerID = 4347;
	INIT_STR(rec.data.Date, "02/15/2024")
		rec.data.InvoiceNumber = 1002;
	rec.data.items[0] = 112;
	rec.data.items[1] = 125;
	rec.data.items[2] = 116;
	rec.Insert();

	// Test Data 3
	rec.data.CustomerID = 3587;
	INIT_STR(rec.data.Date, "03/18/2024")
		rec.data.InvoiceNumber = 1003;
	rec.data.items[0] = 110;
	rec.data.items[1] = 130;
	rec.data.items[2] = 119;
	rec.Insert();

	// Test Data 4
	rec.data.CustomerID = 3456;
	INIT_STR(rec.data.Date, "04/22/2024")
		rec.data.InvoiceNumber = 1004;
	rec.data.items[0] = 114;
	rec.data.items[1] = 118;
	rec.data.items[2] = 109;
	rec.Insert();

	// Test Data 5
	rec.data.CustomerID = 7893;
	INIT_STR(rec.data.Date, "05/05/2024")
		rec.data.InvoiceNumber = 1005;
	rec.data.items[0] = 126;
	rec.data.items[1] = 102;
	rec.data.items[2] = 111;
	rec.Insert();

	// Test Data 6
	rec.data.CustomerID = 1298;
	INIT_STR(rec.data.Date, "06/12/2024")
		rec.data.InvoiceNumber = 1006;
	rec.data.items[0] = 108;
	rec.data.items[1] = 124;
	rec.data.items[2] = 107;
	rec.Insert();

	// Test Data 7
	rec.data.CustomerID = 4587;
	INIT_STR(rec.data.Date, "07/19/2024")
		rec.data.InvoiceNumber = 1007;
	rec.data.items[0] = 116;
	rec.data.items[1] = 128;
	rec.data.items[2] = 125;
	rec.Insert();

	// Test Data 8
	rec.data.CustomerID = 5469;
	INIT_STR(rec.data.Date, "08/25/2024")
		rec.data.InvoiceNumber = 1008;
	rec.data.items[0] = 103;
	rec.data.items[1] = 127;
	rec.data.items[2] = 105;
	rec.Insert();

	// Test Data 9
	rec.data.CustomerID = 8321;
	INIT_STR(rec.data.Date, "09/29/2024")
		rec.data.InvoiceNumber = 1009;
	rec.data.items[0] = 118;
	rec.data.items[1] = 121;
	rec.data.items[2] = 106;
	rec.Insert();

	// Test Data 10
	rec.data.CustomerID = 1023;
	INIT_STR(rec.data.Date, "10/02/2024")
		rec.data.InvoiceNumber = 1010;
	rec.data.items[0] = 120;
	rec.data.items[1] = 115;
	rec.data.items[2] = 101;
	rec.Insert();
	// Test Data 11 (5 items)
	rec.data.CustomerID = 7598;
	INIT_STR(rec.data.Date, "10/15/2024")
		rec.data.InvoiceNumber = 1011;
	rec.data.items[0] = 103;
	rec.data.items[1] = 107;
	rec.data.items[2] = 125;
	rec.data.items[3] = 120;
	rec.data.items[4] = 105;
	rec.Insert();

	// Test Data 12 (2 items)
	rec.data.CustomerID = 6675;
	INIT_STR(rec.data.Date, "11/03/2024")
		rec.data.InvoiceNumber = 1012;
	rec.data.items[0] = 102;
	rec.data.items[1] = 115;
	rec.Insert();

	// Test Data 13 (8 items)
	rec.data.CustomerID = 9085;
	INIT_STR(rec.data.Date, "12/18/2024")
		rec.data.InvoiceNumber = 1013;
	rec.data.items[0] = 110;
	rec.data.items[1] = 124;
	rec.data.items[2] = 108;
	rec.data.items[3] = 125;
	rec.data.items[4] = 103;
	rec.data.items[5] = 119;
	rec.data.items[6] = 106;
	rec.data.items[7] = 114;
	rec.Insert();

	// Test Data 14 (1 item)
	rec.data.CustomerID = 3201;
	INIT_STR(rec.data.Date, "01/09/2025")
		rec.data.InvoiceNumber = 1014;
	rec.data.items[0] = 101;
	rec.Insert();

	// Test Data 15 (4 items)
	rec.data.CustomerID = 4257;
	INIT_STR(rec.data.Date, "02/14/2025")
		rec.data.InvoiceNumber = 1015;
	rec.data.items[0] = 118;
	rec.data.items[1] = 112;
	rec.data.items[2] = 126;
	rec.data.items[3] = 110;
	rec.Insert();

	// Test Data 16 (6 items)
	rec.data.CustomerID = 1290;
	INIT_STR(rec.data.Date, "03/23/2025")
		rec.data.InvoiceNumber = 1016;
	rec.data.items[0] = 113;
	rec.data.items[1] = 128;
	rec.data.items[2] = 116;
	rec.data.items[3] = 105;
	rec.data.items[4] = 109;
	rec.data.items[5] = 124;
	rec.Insert();

	// Test Data 17 (10 items)
	rec.data.CustomerID = 7843;
	INIT_STR(rec.data.Date, "04/30/2025")
		rec.data.InvoiceNumber = 1017;
	rec.data.items[0] = 111;
	rec.data.items[1] = 130;
	rec.data.items[2] = 127;
	rec.data.items[3] = 114;
	rec.data.items[4] = 108;
	rec.data.items[5] = 119;
	rec.data.items[6] = 125;
	rec.data.items[7] = 120;
	rec.data.items[8] = 122;
	rec.data.items[9] = 104;
	rec.Insert();

	// Test Data 18 (3 items)
	rec.data.CustomerID = 8732;
	INIT_STR(rec.data.Date, "05/17/2025")
		rec.data.InvoiceNumber = 1018;
	rec.data.items[0] = 103;
	rec.data.items[1] = 126;
	rec.data.items[2] = 105;
	rec.Insert();

	// Test Data 19 (9 items)
	rec.data.CustomerID = 6583;
	INIT_STR(rec.data.Date, "06/22/2025")
		rec.data.InvoiceNumber = 1019;
	rec.data.items[0] = 107;
	rec.data.items[1] = 121;
	rec.data.items[2] = 106;
	rec.data.items[3] = 111;
	rec.data.items[4] = 124;
	rec.data.items[5] = 129;
	rec.data.items[6] = 105;
	rec.data.items[7] = 108;
	rec.data.items[8] = 110;
	rec.Insert();

	// Test Data 20 (7 items)
	rec.data.CustomerID = 4597;
	INIT_STR(rec.data.Date, "07/01/2025")
		rec.data.InvoiceNumber = 1020;
	rec.data.items[0] = 120;
	rec.data.items[1] = 102;
	rec.data.items[2] = 128;
	rec.data.items[3] = 105;
	rec.data.items[4] = 111;
	rec.data.items[5] = 124;
	rec.data.items[6] = 109;
	rec.Insert();


}

void CreateData3()
{


	// Test Data 1
	Item rec1;
	rec1.data.Bin = 25;
	rec1.data.ItemID = 101;
	INIT_STR(rec1.data.Name, "Orange Juice")
		rec1.data.price = 4.99;
	rec1.data.category = Category::Beverages;
	rec1.data.UM = UnitsOfMeasure::Liter;
	rec1.Insert();
	rec1.Dump();

	// Test Data 2
	Item rec2;
	rec2.data.Bin = 30;
	rec2.data.ItemID = 102;
	INIT_STR(rec2.data.Name, "Apple")
		rec2.data.price = 0.89;
	rec2.data.category = Category::Produce;
	rec2.data.UM = UnitsOfMeasure::Kilogram;
	rec2.Insert();
	rec2.Dump();

	// Test Data 3
	Item rec3;
	rec3.data.Bin = 12;
	rec3.data.ItemID = 103;
	INIT_STR(rec3.data.Name, "Chicken Breast")
		rec3.data.price = 7.25;
	rec3.data.category = Category::Poultry;
	rec3.data.UM = UnitsOfMeasure::Pound;
	rec3.Insert();
	rec3.Dump();

	// Test Data 4
	Item rec4;
	rec4.data.Bin = 18;
	rec4.data.ItemID = 104;
	INIT_STR(rec4.data.Name, "Whole Wheat Bread")
		rec4.data.price = 2.75;
	rec4.data.category = Category::Bakery;
	rec4.data.UM = UnitsOfMeasure::Gram;
	rec4.Insert();
	rec4.Dump();

	// Test Data 5
	Item rec5;
	rec5.data.Bin = 40;
	rec5.data.ItemID = 105;
	INIT_STR(rec5.data.Name, "Milk")
		rec5.data.price = 3.10;
	rec5.data.category = Category::Dairy;
	rec5.data.UM = UnitsOfMeasure::Liter;
	rec5.Insert();
	rec5.Dump();

	// Test Data 6
	Item rec6;
	rec6.data.Bin = 8;
	rec6.data.ItemID = 106;
	INIT_STR(rec6.data.Name, "Ground Beef")
		rec6.data.price = 9.99;
	rec6.data.category = Category::Meet;
	rec6.data.UM = UnitsOfMeasure::Kilogram;
	rec6.Insert();
	rec6.Dump();

	// Test Data 7
	Item rec7;
	rec7.data.Bin = 14;
	rec7.data.ItemID = 107;
	INIT_STR(rec7.data.Name, "Butter")
		rec7.data.price = 1.99;
	rec7.data.category = Category::Dairy;
	rec7.data.UM = UnitsOfMeasure::Pound;
	rec7.Insert();
	rec7.Dump();

	// Test Data 8
	Item rec8;
	rec8.data.Bin = 5;
	rec8.data.ItemID = 108;
	INIT_STR(rec8.data.Name, "Shrimp")
		rec8.data.price = 15.75;
	rec8.data.category = Category::Seafood;
	rec8.data.UM = UnitsOfMeasure::Pound;
	rec8.Insert();
	rec8.Dump();

	// Test Data 9
	Item rec9;
	rec9.data.Bin = 20;
	rec9.data.ItemID = 109;
	INIT_STR(rec9.data.Name, "Red Wine")
		rec9.data.price = 12.99;
	rec9.data.category = Category::Alcohol;
	rec9.data.UM = UnitsOfMeasure::Liter;
	rec9.Insert();
	rec9.Dump();

	// Test Data 10
	Item rec10;
	rec10.data.Bin = 22;
	rec10.data.ItemID = 110;
	INIT_STR(rec10.data.Name, "Potato Chips")
		rec10.data.price = 3.50;
	rec10.data.category = Category::Snacks;
	rec10.data.UM = UnitsOfMeasure::Gram;
	rec10.Insert();
	rec10.Dump();
	// Test Data 11
	Item rec11;
	rec11.data.Bin = 15;
	rec11.data.ItemID = 111;
	INIT_STR(rec11.data.Name, "Soda Can")
		rec11.data.price = 1.50;
	rec11.data.category = Category::Beverages;
	rec11.data.UM = UnitsOfMeasure::Liter;
	rec11.Insert();
	rec11.Dump();

	// Test Data 12
	Item rec12;
	rec12.data.Bin = 26;
	rec12.data.ItemID = 112;
	INIT_STR(rec12.data.Name, "Salmon Fillet")
		rec12.data.price = 14.75;
	rec12.data.category = Category::Seafood;
	rec12.data.UM = UnitsOfMeasure::Pound;
	rec12.Insert();
	rec12.Dump();

	// Test Data 13
	Item rec13;
	rec13.data.Bin = 35;
	rec13.data.ItemID = 113;
	INIT_STR(rec13.data.Name, "Chocolate Cake")
		rec13.data.price = 7.50;
	rec13.data.category = Category::Bakery;
	rec13.data.UM = UnitsOfMeasure::Gram;
	rec13.Insert();
	rec13.Dump();

	// Test Data 14
	Item rec14;
	rec14.data.Bin = 9;
	rec14.data.ItemID = 114;
	INIT_STR(rec14.data.Name, "Cheddar Cheese")
		rec14.data.price = 5.99;
	rec14.data.category = Category::Dairy;
	rec14.data.UM = UnitsOfMeasure::Kilogram;
	rec14.Insert();
	rec14.Dump();

	// Test Data 15
	Item rec15;
	rec15.data.Bin = 48;
	rec15.data.ItemID = 115;
	INIT_STR(rec15.data.Name, "Ground Pork")
		rec15.data.price = 8.40;
	rec15.data.category = Category::Meet;
	rec15.data.UM = UnitsOfMeasure::Kilogram;
	rec15.Insert();
	rec15.Dump();

	// Test Data 16
	Item rec16;
	rec16.data.Bin = 5;
	rec16.data.ItemID = 116;
	INIT_STR(rec16.data.Name, "Greek Yogurt")
		rec16.data.price = 1.20;
	rec16.data.category = Category::Dairy;
	rec16.data.UM = UnitsOfMeasure::Ounce;
	rec16.Insert();
	rec16.Dump();

	// Test Data 17
	Item rec17;
	rec17.data.Bin = 28;
	rec17.data.ItemID = 117;
	INIT_STR(rec17.data.Name, "Frozen Peas")
		rec17.data.price = 2.10;
	rec17.data.category = Category::Groceries;
	rec17.data.UM = UnitsOfMeasure::Pound;
	rec17.Insert();
	rec17.Dump();

	// Test Data 18
	Item rec18;
	rec18.data.Bin = 14;
	rec18.data.ItemID = 118;
	INIT_STR(rec18.data.Name, "Frozen Pizza")
		rec18.data.price = 4.99;
	rec18.data.category = Category::Groceries;
	rec18.data.UM = UnitsOfMeasure::SquareFoot;
	rec18.Insert();
	rec18.Dump();

	// Test Data 19
	Item rec19;
	rec19.data.Bin = 32;
	rec19.data.ItemID = 119;
	INIT_STR(rec19.data.Name, "Grapes")
		rec19.data.price = 3.25;
	rec19.data.category = Category::Produce;
	rec19.data.UM = UnitsOfMeasure::Pound;
	rec19.Insert();
	rec19.Dump();

	// Test Data 20
	Item rec20;
	rec20.data.Bin = 11;
	rec20.data.ItemID = 120;
	INIT_STR(rec20.data.Name, "Almonds")
		rec20.data.price = 9.50;
	rec20.data.category = Category::Snacks;
	rec20.data.UM = UnitsOfMeasure::Gram;
	rec20.Insert();
	rec20.Dump();

	// Test Data 21
	Item rec21;
	rec21.data.Bin = 8;
	rec21.data.ItemID = 121;
	INIT_STR(rec21.data.Name, "Lemonade")
		rec21.data.price = 2.50;
	rec21.data.category = Category::Beverages;
	rec21.data.UM = UnitsOfMeasure::Quart;
	rec21.Insert();
	rec21.Dump();

	// Test Data 22
	Item rec22;
	rec22.data.Bin = 29;
	rec22.data.ItemID = 122;
	INIT_STR(rec22.data.Name, "Turkey Breast")
		rec22.data.price = 11.75;
	rec22.data.category = Category::Poultry;
	rec22.data.UM = UnitsOfMeasure::Pound;
	rec22.Insert();
	rec22.Dump();

	// Test Data 23
	Item rec23;
	rec23.data.Bin = 18;
	rec23.data.ItemID = 123;
	INIT_STR(rec23.data.Name, "Bagels")
		rec23.data.price = 3.75;
	rec23.data.category = Category::Bakery;
	rec23.data.UM = UnitsOfMeasure::Gram;
	rec23.Insert();
	rec23.Dump();

	// Test Data 24
	Item rec24;
	rec24.data.Bin = 36;
	rec24.data.ItemID = 124;
	INIT_STR(rec24.data.Name, "Whiskey")
		rec24.data.price = 25.00;
	rec24.data.category = Category::Alcohol;
	rec24.data.UM = UnitsOfMeasure::Gallon;
	rec24.Insert();
	rec24.Dump();

	// Test Data 25
	Item rec25;
	rec25.data.Bin = 19;
	rec25.data.ItemID = 125;
	INIT_STR(rec25.data.Name, "Pasta")
		rec25.data.price = 1.80;
	rec25.data.category = Category::Groceries;
	rec25.data.UM = UnitsOfMeasure::Pound;
	rec25.Insert();
	rec25.Dump();

	// Test Data 26
	Item rec26;
	rec26.data.Bin = 6;
	rec26.data.ItemID = 126;
	INIT_STR(rec26.data.Name, "Butter Croissant")
		rec26.data.price = 2.10;
	rec26.data.category = Category::Bakery;
	rec26.data.UM = UnitsOfMeasure::Gram;
	rec26.Insert();
	rec26.Dump();

	// Test Data 27
	Item rec27;
	rec27.data.Bin = 45;
	rec27.data.ItemID = 127;
	INIT_STR(rec27.data.Name, "Red Apples")
		rec27.data.price = 0.95;
	rec27.data.category = Category::Produce;
	rec27.data.UM = UnitsOfMeasure::Kilogram;
	rec27.Insert();
	rec27.Dump();

	// Test Data 28
	Item rec28;
	rec28.data.Bin = 3;
	rec28.data.ItemID = 128;
	INIT_STR(rec28.data.Name, "Beer")
		rec28.data.price = 8.50;
	rec28.data.category = Category::Alcohol;
	rec28.data.UM = UnitsOfMeasure::Quart;
	rec28.Insert();
	rec28.Dump();

	// Test Data 29
	Item rec29;
	rec29.data.Bin = 17;
	rec29.data.ItemID = 129;
	INIT_STR(rec29.data.Name, "Beef Jerky")
		rec29.data.price = 6.25;
	rec29.data.category = Category::Snacks;
	rec29.data.UM = UnitsOfMeasure::Gram;
	rec29.Insert();
	rec29.Dump();

	// Test Data 30
	Item rec30;
	rec30.data.Bin = 27;
	rec30.data.ItemID = 130;
	INIT_STR(rec30.data.Name, "Watermelon")
		rec30.data.price = 4.99;
	rec30.data.category = Category::Produce;
	rec30.data.UM = UnitsOfMeasure::Kilogram;
	rec30.Insert();
	rec30.Dump();
}

void CreateTheSimpsonsFamily()
{
	Family family;
	Person husband;
	Person wife;
	Child boy;
	Child girl;
	Child baby;
	Cat cat;
	Dog dog;

	INIT_STR(family.data.name, "The Simpsons")
		INIT_STR(family.data.address1, "742 Evergreen Terrace")
		INIT_STR(family.data.address2, "")
		INIT_STR(family.data.city, "Springfield")
		INIT_STR(family.data.state, "NY")
		INIT_STR(family.data.zip, "23432")
		family.Insert();


	INIT_STR(husband.data.name.first, "Homer")
		INIT_STR(husband.data.name.last, "Simpson")
		INIT_STR(husband.data.address1, "742 Evergreen Terrace")
		INIT_STR(husband.data.address2, "")
		INIT_STR(husband.data.city, "Springfield")
		INIT_STR(husband.data.state, "NY")
		INIT_STR(husband.data.zip, "23432")
		husband.data.age.years = 44;
	husband.data.age.months = 6;
	INIT_STR(husband.data.job, "Nuclear engineer")
		husband.data.eyeColor = PersBrownEyeColor;
	husband.data.gender = PersonMale;
	husband.data.hairColor = PersBlackHairColor;
	husband.data.hight = 63;
	husband.data.weight = 250;
	husband.data.maritalStatus = Maried;
	husband.Insert();
	// Add Homer to family
	family.data.members[0] = husband.GetPrimaryKey();
	family.Update();

	INIT_STR(wife.data.name.first, "Marge")
		INIT_STR(wife.data.name.last, "Simpson")
		INIT_STR(wife.data.address1, "742 Evergreen Terrace")
		INIT_STR(wife.data.address2, "")
		INIT_STR(wife.data.city, "Springfield")
		INIT_STR(wife.data.state, "NY")
		INIT_STR(wife.data.zip, "23432")
		wife.data.age.years = 44;
	wife.data.age.months = 6;
	INIT_STR(wife.data.job, "Housewife")
		wife.data.eyeColor = PersBlueEyeColor;
	wife.data.gender = PersonFemale;
	wife.data.hairColor = PersBlueHairColor;
	wife.data.hight = 55;
	wife.data.weight = 250;
	wife.data.maritalStatus = Maried;
	wife.data.spouse = husband.GetPrimaryKey();
	wife.Insert();
	// Add Marge to family
	family.data.members[1] = wife.GetPrimaryKey();
	family.Update();
	husband.data.spouse = wife.GetPrimaryKey();
	husband.Update();

	INIT_STR(boy.data.name.first, "Bart")
		INIT_STR(boy.data.name.last, "Simpson")
		boy.data.age.years = 10;
	boy.data.age.months = 2;
	boy.data.eyeColor = HazelEyeColor;
	boy.data.gender = ChildMale;
	boy.data.hairColor = BlondeHairColor;
	boy.data.hight = 40;
	boy.data.weight = 120;
	boy.data.father = husband.GetPrimaryKey();
	boy.data.mother = wife.GetPrimaryKey();

	boy.Insert();
	// Add Bart to family
	family.data.members[2] = boy.GetPrimaryKey();
	family.Update();

	INIT_STR(girl.data.name.first, "Lisa")
		INIT_STR(girl.data.name.last, "Simpson")
		girl.data.age.years = 9;
	girl.data.age.months = 4;
	girl.data.eyeColor = HazelEyeColor;
	girl.data.gender = ChildFemale;
	girl.data.hairColor = BlondeHairColor;
	girl.data.hight = 37;
	girl.data.weight = 110;
	girl.data.father = husband.GetPrimaryKey();
	girl.data.mother = wife.GetPrimaryKey();

	girl.Insert();
	// Add Lisa to family
	family.data.members[3] = girl.GetPrimaryKey();
	family.Update();

	INIT_STR(baby.data.name.first, "Maggie")
		INIT_STR(baby.data.name.last, "Simpson")
		baby.data.age.years = 1;
	baby.data.age.months = 2;
	baby.data.eyeColor = HazelEyeColor;
	baby.data.gender = ChildFemale;
	baby.data.hairColor = BlondeHairColor;
	baby.data.hight = 1.5;
	baby.data.weight = 15;
	baby.data.father = husband.GetPrimaryKey();
	baby.data.mother = wife.GetPrimaryKey();

	baby.Insert();
	// Add Maggie to family
	family.data.members[4] = baby.GetPrimaryKey();
	family.Update();

	INIT_STR(cat.data.name, "Snowball II ")
		cat.data.gender = CatMale;
	cat.data.age.years = 3;
	cat.data.age.months = 3;
	cat.data.weight = 9;
	cat.data.eyeColor = CatGreenEyeColor;
	cat.data.hairColor = CatBlackHairColor;
	cat.data.owner = girl.GetPrimaryKey();
	cat.Insert();


	INIT_STR(dog.data.name, "Santa's Little Helper ")
		dog.data.gender = DogMale;
	dog.data.age.years = 9;
	dog.data.age.months = 5;
	dog.data.weight = 9;
	dog.data.eyeColor = DogGreenEyeColor;
	dog.data.hairColor = DogBlackHairColor;
	dog.data.owner = boy.GetPrimaryKey();
	dog.Insert();



	INIT_STR(family.data.name, "The Griffins")
		INIT_STR(family.data.address1, "31 Spooner Street")
		INIT_STR(family.data.address2, "")
		INIT_STR(family.data.city, "Quahog")
		INIT_STR(family.data.state, "RI")
		INIT_STR(family.data.zip, "02860")
		family.Insert();

	INIT_STR(husband.data.name.first, "Peter")
		INIT_STR(husband.data.name.last, "Griffin")
		INIT_STR(husband.data.address1, "31 Spooner Street")
		INIT_STR(husband.data.address2, "")
		INIT_STR(husband.data.city, "Quahog")
		INIT_STR(husband.data.state, "RI")
		INIT_STR(husband.data.zip, "02860")
		husband.data.age.years = 43;
	husband.data.age.months = 2;

	INIT_STR(husband.data.job, "Brewery Worker")
		husband.data.eyeColor = PersBrownEyeColor;
	husband.data.gender = PersonMale;
	husband.data.hairColor = PersBrownHairColor;
	husband.data.hight = 68;
	husband.data.weight = 270;
	husband.data.maritalStatus = Maried;
	husband.Insert();

	// Add Peter to family

	INIT_STR(wife.data.name.first, "Lois")
		INIT_STR(wife.data.name.last, "Griffin")
		INIT_STR(wife.data.address1, "31 Spooner Street")
		INIT_STR(wife.data.address2, "")
		INIT_STR(wife.data.city, "Quahog")
		INIT_STR(wife.data.state, "RI")
		INIT_STR(wife.data.zip, "02860")
		wife.data.age.years = 40;
	wife.data.age.months = 5;
	INIT_STR(wife.data.job, "Piano Teacher")
		wife.data.eyeColor = PersBlueEyeColor;
	wife.data.gender = PersonFemale;
	wife.data.hairColor = PersRedHairColor;
	wife.data.hight = 60;
	wife.data.weight = 150;
	wife.data.maritalStatus = Maried;
	wife.data.spouse = husband.GetPrimaryKey();
	wife.Insert();

	INIT_STR(boy.data.name.first, "Chris")
		INIT_STR(boy.data.name.last, "Griffin")
		boy.data.age.years = 14;
	boy.data.age.months = 7;
	boy.data.eyeColor = BlueEyeColor;
	boy.data.gender = ChildMale;
	boy.data.hairColor = RedHairColor;
	boy.data.hight = 65;
	boy.data.weight = 190;
	boy.data.father = husband.GetPrimaryKey();
	boy.data.mother = wife.GetPrimaryKey();
	boy.Insert();

	INIT_STR(girl.data.name.first, "Meg")
		INIT_STR(girl.data.name.last, "Griffin")
		girl.data.age.years = 17;
	girl.data.age.months = 3;
	girl.data.eyeColor = BlueEyeColor;
	girl.data.gender = ChildFemale;
	girl.data.hairColor = RedHairColor;
	girl.data.hight = 62;
	girl.data.weight = 145;
	girl.data.father = husband.GetPrimaryKey();
	girl.data.mother = wife.GetPrimaryKey();
	girl.Insert();

	INIT_STR(baby.data.name.first, "Stewie")
		INIT_STR(baby.data.name.last, "Griffin")
		baby.data.age.years = 1;
	baby.data.age.months = 1;
	baby.data.eyeColor = BlueEyeColor;
	baby.data.gender = ChildMale;
	baby.data.hairColor = RedHairColor;
	baby.data.hight = 28;
	baby.data.weight = 20;
	baby.data.father = husband.GetPrimaryKey();
	baby.data.mother = wife.GetPrimaryKey();
	baby.Insert();

	INIT_STR(cat.data.name, "Puddy Tat")
		cat.data.gender = CatFemale;
	cat.data.age.years = 4;
	cat.data.age.months = 2;
	cat.data.weight = 10;
	cat.data.eyeColor = CatGreenEyeColor;
	cat.data.hairColor = CatGrayHairColor;
	cat.data.owner = girl.GetPrimaryKey();
	cat.Insert();

	INIT_STR(dog.data.name, "Brian")
		dog.data.gender = DogMale;
	dog.data.age.years = 8;
	dog.data.age.months = 4;
	dog.data.weight = 30;
	dog.data.eyeColor = DogBrownEyeColor;
	dog.data.hairColor = DogWhiteHairColor;
	dog.data.owner = husband.GetPrimaryKey();
	dog.Insert();


	INIT_STR(family.data.name, "The Andersons")
		INIT_STR(family.data.address1, "15 Oakwood Drive")
		INIT_STR(family.data.address2, "")
		INIT_STR(family.data.city, "Shelbyville")
		INIT_STR(family.data.state, "NY")
		INIT_STR(family.data.zip, "12345")
		family.Insert();

	INIT_STR(husband.data.name.first, "John")
		INIT_STR(husband.data.name.last, "Anderson")
		INIT_STR(husband.data.address1, "15 Oakwood Drive")
		INIT_STR(husband.data.address2, "")
		INIT_STR(husband.data.city, "Shelbyville")
		INIT_STR(husband.data.state, "NY")
		INIT_STR(husband.data.zip, "12345")
		husband.data.age.years = 40;
	husband.data.age.months = 8;
	INIT_STR(husband.data.job, "Architect")
		husband.data.eyeColor = PersBrownEyeColor;
	husband.data.gender = PersonMale;
	husband.data.hairColor = PersBlackHairColor;
	husband.data.hight = 70;
	husband.data.weight = 180;
	husband.data.maritalStatus = Maried;
	husband.Insert();

	INIT_STR(wife.data.name.first, "Jane")
		INIT_STR(wife.data.name.last, "Anderson")
		INIT_STR(wife.data.address1, "15 Oakwood Drive")
		INIT_STR(wife.data.address2, "")
		INIT_STR(wife.data.city, "Shelbyville")
		INIT_STR(wife.data.state, "NY")
		INIT_STR(wife.data.zip, "12345")
		wife.data.age.years = 38;
	wife.data.age.months = 3;
	INIT_STR(wife.data.job, "Teacher")
		wife.data.eyeColor = PersBlueEyeColor;
	wife.data.gender = PersonFemale;
	wife.data.hairColor = PersBlueHairColor;
	wife.data.hight = 65;
	wife.data.weight = 140;
	wife.data.maritalStatus = Maried;
	wife.data.spouse = husband.GetPrimaryKey();
	wife.Insert();

	INIT_STR(boy.data.name.first, "Mike")
		INIT_STR(boy.data.name.last, "Anderson")
		boy.data.age.years = 12;
	boy.data.age.months = 5;
	boy.data.eyeColor = HazelEyeColor;
	boy.data.gender = ChildMale;
	boy.data.hairColor = BlondeHairColor;
	boy.data.hight = 50;
	boy.data.weight = 100;
	boy.data.father = husband.GetPrimaryKey();
	boy.data.mother = wife.GetPrimaryKey();
	boy.Insert();

	INIT_STR(girl.data.name.first, "Emily")
		INIT_STR(girl.data.name.last, "Anderson")
		girl.data.age.years = 10;
	girl.data.age.months = 2;
	girl.data.eyeColor = HazelEyeColor;
	girl.data.gender = ChildFemale;
	girl.data.hairColor = BlondeHairColor;
	girl.data.hight = 48;
	girl.data.weight = 90;
	girl.data.father = husband.GetPrimaryKey();
	girl.data.mother = wife.GetPrimaryKey();
	girl.Insert();

	INIT_STR(baby.data.name.first, "Lucy")
		INIT_STR(baby.data.name.last, "Anderson")
		baby.data.age.years = 1;
	baby.data.age.months = 6;
	baby.data.eyeColor = HazelEyeColor;
	baby.data.gender = ChildFemale;
	baby.data.hairColor = BlondeHairColor;
	baby.data.hight = 28;
	baby.data.weight = 20;
	baby.data.father = husband.GetPrimaryKey();
	baby.data.mother = wife.GetPrimaryKey();
	baby.Insert();

	INIT_STR(cat.data.name, "Whiskers")
		cat.data.gender = CatMale;
	cat.data.age.years = 4;
	cat.data.age.months = 1;
	cat.data.weight = 12;
	cat.data.eyeColor = CatGreenEyeColor;
	cat.data.hairColor = CatBlackHairColor;
	cat.data.owner = girl.GetPrimaryKey();
	cat.Insert();

	INIT_STR(dog.data.name, "Buddy")
		dog.data.gender = DogMale;
	dog.data.age.years = 7;
	dog.data.age.months = 10;
	dog.data.weight = 35;
	dog.data.eyeColor = DogGreenEyeColor;
	dog.data.hairColor = DogBlackHairColor;
	dog.data.owner = boy.GetPrimaryKey();
	dog.Insert();


	INIT_STR(family.data.name, "The Flynns")
		INIT_STR(family.data.address1, "22 Maple Street")
		INIT_STR(family.data.address2, "")
		INIT_STR(family.data.city, "Capital City")
		INIT_STR(family.data.state, "NY")
		INIT_STR(family.data.zip, "54321")
		family.Insert();

	INIT_STR(husband.data.name.first, "James")
		INIT_STR(husband.data.name.last, "Flynn")
		INIT_STR(husband.data.address1, "22 Maple Street")
		INIT_STR(husband.data.address2, "")
		INIT_STR(husband.data.city, "Capital City")
		INIT_STR(husband.data.state, "NY")
		INIT_STR(husband.data.zip, "54321")
		husband.data.age.years = 42;
	husband.data.age.months = 10;
	INIT_STR(husband.data.job, "Lawyer")
		husband.data.eyeColor = PersBrownEyeColor;
	husband.data.gender = PersonMale;
	husband.data.hairColor = PersBlackHairColor;
	husband.data.hight = 71;
	husband.data.weight = 190;
	husband.data.maritalStatus = Maried;
	husband.Insert();

	INIT_STR(wife.data.name.first, "Sarah")
		INIT_STR(wife.data.name.last, "Flynn")
		INIT_STR(wife.data.address1, "22 Maple Street")
		INIT_STR(wife.data.address2, "")
		INIT_STR(wife.data.city, "Capital City")
		INIT_STR(wife.data.state, "NY")
		INIT_STR(wife.data.zip, "54321")
		wife.data.age.years = 40;
	wife.data.age.months = 4;
	INIT_STR(wife.data.job, "Doctor")
		wife.data.eyeColor = PersBlueEyeColor;
	wife.data.gender = PersonFemale;
	wife.data.hairColor = PersBlueHairColor;
	wife.data.hight = 64;
	wife.data.weight = 145;
	wife.data.maritalStatus = Maried;
	wife.data.spouse = husband.GetPrimaryKey();
	wife.Insert();

	INIT_STR(boy.data.name.first, "Tom")
		INIT_STR(boy.data.name.last, "Flynn")
		boy.data.age.years = 13;
	boy.data.age.months = 1;
	boy.data.eyeColor = HazelEyeColor;
	boy.data.gender = ChildMale;
	boy.data.hairColor = BlondeHairColor;
	boy.data.hight = 52;
	boy.data.weight = 110;
	boy.data.father = husband.GetPrimaryKey();
	boy.data.mother = wife.GetPrimaryKey();
	boy.Insert();

	INIT_STR(girl.data.name.first, "Sophia")
		INIT_STR(girl.data.name.last, "Flynn")
		girl.data.age.years = 11;
	girl.data.age.months = 3;
	girl.data.eyeColor = HazelEyeColor;
	girl.data.gender = ChildFemale;
	girl.data.hairColor = BlondeHairColor;
	girl.data.hight = 49;
	girl.data.weight = 95;
	girl.data.father = husband.GetPrimaryKey();
	girl.data.mother = wife.GetPrimaryKey();
	girl.Insert();

	INIT_STR(baby.data.name.first, "Ella")
		INIT_STR(baby.data.name.last, "Flynn")
		baby.data.age.years = 2;
	baby.data.age.months = 2;
	baby.data.eyeColor = HazelEyeColor;
	baby.data.gender = ChildFemale;
	baby.data.hairColor = BlondeHairColor;
	baby.data.hight = 30;
	baby.data.weight = 22;
	baby.data.father = husband.GetPrimaryKey();
	baby.data.mother = wife.GetPrimaryKey();
	baby.Insert();

	INIT_STR(cat.data.name, "Shadow")
		cat.data.gender = CatMale;
	cat.data.age.years = 5;
	cat.data.age.months = 6;
	cat.data.weight = 13;
	cat.data.eyeColor = CatGreenEyeColor;
	cat.data.hairColor = CatBlackHairColor;
	cat.data.owner = girl.GetPrimaryKey();
	cat.Insert();

	INIT_STR(dog.data.name, "Max")
		dog.data.gender = DogMale;
	dog.data.age.years = 6;
	dog.data.age.months = 4;
	dog.data.weight = 40;
	dog.data.eyeColor = DogGreenEyeColor;
	dog.data.hairColor = DogBlackHairColor;
	dog.data.owner = boy.GetPrimaryKey();
	dog.Insert();

}

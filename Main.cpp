#include <iostream>
#include <vector>
#include <string>;
#include "sqlite/sqlite3.h"	//header to include sqlite functionality

using namespace std;

//popfigure class to help with attributes of pop figures
class popFigure	
{
private:
	//private attributes of pop figures
	string popName;
	int popNum;

public:
	//default cosntructor
	popFigure(string p, int num)
	{
		popName = p;
		popNum = num;
	}
	//Setters
	void setPopName(const string& p)
	{
		popName = p;
	}
	void setPopNumber(const int& num)
	{
		popNum = num;
	}

	//Getters
	string getPopName() const
	{
		return popName;
	}
	int getPopNum() const
	{
		return popNum;
	}
	string getInfo() const 
	{
		return popName + to_string(popNum);
	}
};

//class that handles collection functionality
class popFigureCollection {
private:
	vector<popFigure> PopFigures;
	sqlite3* db;

public:
	//open the sqlite database
	popFigureCollection()
	{
		int rc = sqlite3_open("pop_figures.db", &db); //result code

		if (rc != SQLITE_OK)
		{
			cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
			exit(1);
		}

		//create table if it does not exist
		const char* createTableSQL = "CREATE TABLE IF NOT EXISTS PopFigures (id INTEGER PRIMARY KEY AUTOINCREMENT, popName TEXT, popNum INTEGER);";
		rc = sqlite3_exec(db, createTableSQL, NULL, 0, NULL);
		
		if (rc != SQLITE_OK) {
			cerr << "Cannot create table: " << sqlite3_errmsg(db) << endl;
			exit(1);
		}
	}

	//destructor to close sqlite database
	~popFigureCollection()
	{
		sqlite3_close(db);
	}

	//functions of application
	void menu();
	void addPop();
	void deletePop();
	void findPop();

};

void popFigureCollection::menu()
{
	int userChoice = 0;
	while (userChoice < 1 || userChoice > 4)
	{
		cout << "---------------------------------" << endl;
		cout << "Choose an option below: " << endl;
		cout << "1: Administrator" << endl;
		cout << "2: Add a pop figure" << endl;
		cout << "3: Delete a pop figure" << endl;
		cout << "4: Find a pop figure" << endl;
		cin >> userChoice;
		switch (userChoice) {
		case 1:
			cout << "Admin" << endl;
			break;
		case 2:
			popFigureCollection::addPop();
			break;
		case 3:
			popFigureCollection::deletePop();
			break;
		case 4:
			popFigureCollection::findPop();
			break;
		default:
			cout << "Incorrect option" << endl;
			break;
		}
	}
}

void popFigureCollection::addPop()
{
	string popName;
	int popNum;

	cout << "Adding pop figure" << endl;
	cout << "What is the name of the pop?: ";
	cin >> popName;
	cout << "\nWhat is the pop number?: ";
	cin >> popNum;

	popFigure newPop(popName, popNum);
	PopFigures.push_back(newPop);

	string insertSQL = "INSERT INTO PopFigures (popName, popNum) VALUES ('" + popName + "', " + to_string(popNum) + ");";
	int rc = sqlite3_exec(db, insertSQL.c_str(), NULL, 0, NULL);

	if (rc != SQLITE_OK)
	{
		cerr << "Cannot insert into database: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}

	cout << "Pop figure data succesfully added" << endl;
	return menu();
}

void popFigureCollection::deletePop()
{
	cout << "Deleting a pop figure" << endl;
	 
	// Fetch and display existing pop figures from the SQLite database
	cout << "Existing pop figures (from database):" << endl;
	const char* selectSQL = "SELECT * FROM PopFigures;";
	sqlite3_stmt* stmt;

	int rc_prep = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
	if (rc_prep != SQLITE_OK) {
		cerr << "Cannot prepare statement: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string popName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		int popNum = sqlite3_column_int(stmt, 2);
		cout << popName << " - " << popNum << endl;
	}

	sqlite3_finalize(stmt);

	// Prompt user for input (e.g., pop name or pop number to delete)
	string deleteInput;
	cout << "Enter the name or number of the pop figure to delete: ";
	cin >> deleteInput;

	// Delete the pop figure from the SQLite database
	string deleteSQL = "DELETE FROM PopFigures WHERE popName = '" + deleteInput + "' OR popNum = " + deleteInput + ";";
	int rc = sqlite3_exec(db, deleteSQL.c_str(), NULL, 0, NULL);

	if (rc != SQLITE_OK) {
		cerr << "Cannot delete from database: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}

	cout << "Pop figure deleted from the database." << endl;

	return menu();  // Return to the menu

}

void popFigureCollection::findPop()
{
	cout << "Finding a pop figure" << endl;

	// Prompt user for input (e.g., pop name or pop number to find)
	string findInput;
	cout << "Enter the name or number of the pop figure to find: ";
	cin >> findInput;

	// Find and display matching pop figures from the SQLite database using a prepared statement
	cout << "Matching pop figures from database:" << endl;
	const char* selectSQL = "SELECT * FROM PopFigures WHERE popName = ? OR popNum = ?;";
	sqlite3_stmt* stmt;

	int rc_prepare = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
	if (rc_prepare != SQLITE_OK) {
		cerr << "Cannot prepare statement: " << sqlite3_errmsg(db) << endl;
		exit(1);
	}

	// Bind values to the prepared statement parameters
	sqlite3_bind_text(stmt, 1, findInput.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, stoi(findInput));

	// Execute the prepared statement
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		string popName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		int popNum = sqlite3_column_int(stmt, 2);
		cout << popName << " - " << popNum << endl;
	}

	sqlite3_finalize(stmt);

	return menu();  // Return to the menu
}



int main()
{
	popFigureCollection s;
	s.menu();
	return 0;
}
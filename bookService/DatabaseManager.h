#include "Book.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver

using namespace std;


class DatabaseManager 
{
private: 
	string databaseName;
	string booksTableName;
	mongo::DBClientConnection connection;
public:
    void run();
	DatabaseManager();
    string getDatabaseName();
	vector<string> getAllBooks();
//	void addBook (Book);
//	void updateRating (Book);
	string getBookById (const string&);
};
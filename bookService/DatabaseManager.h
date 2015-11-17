//#include "Book.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string.h>
#include <memory>
#include "mongo/client/dbclient.h"

using namespace std;
using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;


class DatabaseManager 
{
private: 
	string databaseName;
	string booksTableName;
	mongo::DBClientConnection connection;


public:
    	DatabaseManager();
	vector<string> getAllBooks();
    	void run();

//	void addBook(Book book);
//	void updateRating(Book book);
};
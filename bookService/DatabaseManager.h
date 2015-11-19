#include "Book.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver

using namespace std;


class DatabaseManager 
{
private: 
	string databaseName = "test_books";
	string booksTableName = "book";
	mongo::DBClientConnection connection;

	void run();
public:
	DatabaseManager();
	vector<Book> getAllBooks();
	void addBook (Book book);
	void updateRating (Book book);
	string getBookById (string id);

};
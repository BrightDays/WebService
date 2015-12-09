#pragma once
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
	string userTableName;
	string bookUserTableName;
	mongo::DBClientConnection connection;
public:
    void run();
	DatabaseManager();
    string getDatabaseName(const string& tableName);
	vector<string> getAllBooks();
	bool addBook(const string&, const string&, const string&, const string&, string &);
//    void addBook(const mongo :: BSONObj &bookBSON);
//	void updateRating (Book);
	string getBookById (const string&);
	vector<string> getBooksByAuthor(const string&);
	vector<string> getBooksByName(const string&);
	string getBookByNameAndAuthor (const string&, const string&);
	bool updateRating(const string& bookId, int rating);
};
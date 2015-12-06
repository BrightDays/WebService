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
	mongo::DBClientConnection connection;
public:
    void run();
	DatabaseManager();
    string getDatabaseName();
	vector<string> getAllBooks();
//	void addBook(const string& title, const string& author, const string& imageUrl, const string& bookUrl);
//	void updateRating (Book);
	string getBookById (const string&);
	vector<string> getBooksByAuthor(const string&);
	vector<string> getBooksByName(const string&);
	string getBookByNameAndAuthor (const string&, const string&);
};
#pragma once
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
	void recountRating(const string& bookId);
    	void run();
	DatabaseManager();
    	string getDatabaseName(const string& tableName);
	vector<string> getAllBooks();
	bool addBook(const string&, const string&, const string&, const string&, string &);
//    void addBook(const mongo :: BSONObj &bookBSON);
//	void updateRating (Book);
	string getBookById (const string&);
    mongo :: BSONObj getBookBsonById (const string& bookId);
	vector<string> getBooksByAuthor(const string&);
	vector<string> getBooksByName(const string&);
	string getBookByNameAndAuthor (const string&, const string&);
	void updateRating(const string& bookId, int rating, const string& userId);
	bool addUser(const string& login, const string& password, string &response);
	bool checkUser (const string& login, const string& password, string &response);
	bool checkLoginExists(const string& login);
	bool userIdExists(string& userId);
};
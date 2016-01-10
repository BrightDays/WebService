#pragma once
#include <vector>
#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h" // for the driver


class DatabaseManager 
{
private: 
	std :: string databaseName;
	std :: string booksTableName;
	std :: string userTableName;
	std :: string bookUserTableName;
	mongo::DBClientConnection connection;
public:
	void recountRating(const std :: string& bookId);
    void run();
	DatabaseManager();
    std :: string getDatabaseName(const std :: string& tableName);
	std :: vector<std :: string> getAllBooks();
	bool addBook(const std :: string&, const std :: string&, const std :: string&, const std :: string&, std :: string &);
//    void addBook(const mongo :: BSONObj &bookBSON);
//	void updateRating (Book);
	std :: string getBookById (const std :: string&);
    mongo :: BSONObj getBookBsonById (const std :: string& bookId);
	std :: vector<std :: string> getBooksByAuthor(const std :: string&);
	std :: vector<std :: string> getBooksByName(const std :: string&);
	std :: string getBookByNameAndAuthor (const std :: string&, const std :: string&);
	void updateRating(const std :: string& bookId, int rating, const std :: string& userId);
	bool addUser(const std :: string& login, const std :: string& password, std :: string &response);
	bool checkUser (const std :: string& login, const std :: string& password, std :: string &response);
	bool checkLoginExists(const std :: string& login);
	bool userIdExists(std :: string& userId);
};
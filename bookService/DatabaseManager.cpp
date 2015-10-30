#include "DatabaseManager.h"


DatabaseManager::DatabaseManager()
{
	databaseName = "books.db";
}

vector<Book> DatabaseManager::getAllBooks()
{
	vector<Book>books;
	try
	{
		// Open a database file
		SQLite::Database db(databaseName);

		// Compile a SQL query, containing one parameter (index 1)
		SQLite::Statement query(db, "SELECT * FROM books");

		// Loop to execute the query step by step, to get rows of result
		while (query.executeStep())
		{
			// Demonstrate how to get some typed column value
			int         id      = query.getColumn(0);
			string title (query.getColumn(1));
			string authorName (query.getColumn(2));
			string imageUrl (query.getColumn(3));
			string bookUrl (query.getColumn(4));
			double rating = query.getColumn(5);
			Book book(title, authorName, imageUrl, bookUrl, rating);
			books.push_back(book);
		}
	}
	catch (std::exception& e)
	{
		//TODO:
	}
	return books;
}


#include "DatabaseManager.h"


DatabaseManager::DatabaseManager()
{
	databaseName = "books.db";
	if(!mongo::client::initialize())
	{
		//TODO: add exception;
	}
}

void DatabaseManager::run() 
{
	connection.connect("localhost");//TODO: change loclahost to ip
}

vector<Book> DatabaseManager::getAllBooks()
{
	vector<string>books;
	auto_ptr<DBClientCursor> cursor = connection.query(booksTableName, BSONObj());
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
}

void DatabaseManager::addBook(Book book) 
{
	BSONObj bookBSON = BSON( GENOID << "title" << book.getTitle() << "author" << book.getAuthor() << "imageUrl" 
	<< book.getImageUrl << "bookUrl" << book.getBookUrl() << "rating" << book.getRating() );
	connection.insert(booksTableName, bookBSON);
}

void DatabaseManager::updateRating(Book book, double rating) //TODO: check!
{
	db.update(booksTableName,
		BSON("title" << book.getTitle() << "author" << book.getAuthor() << "imageUrl" 
			<< book.getImageUrl << "bookUrl" << book.getBookUrl() << "rating" << book.getRating()),
		BSON("$inc" << BSON( "rating" << rating)));
		
}
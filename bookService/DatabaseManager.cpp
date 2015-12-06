#include "DatabaseManager.h"
#include <memory>

using namespace std;


DatabaseManager::DatabaseManager()
{
	mongo::client::initialize();
    databaseName = "test_books";
    booksTableName = "book";
}

void DatabaseManager::run() 
{
	connection.connect("localhost");//TODO: change loclahost to ip
}

string DatabaseManager :: getDatabaseName()
{
    return databaseName + "." + booksTableName;
}

vector<string> DatabaseManager::getAllBooks()
{
	vector<string>books;
    auto_ptr<mongo :: DBClientCursor> cursor = auto_ptr<mongo :: DBClientCursor>(connection.query(getDatabaseName(), mongo :: BSONObj()));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	//if (books.size())
	//	return books;
	//books.push_back(mongo::DBClientWithCommands::getLastError(0,0,0,0));
	return books;
}

string DatabaseManager::getBookById (const string& bookId)
{
	string book;
    auto_ptr<mongo :: DBClientCursor> cursor = auto_ptr<mongo :: DBClientCursor> (connection.query(getDatabaseName(), MONGO_QUERY("_id" << mongo :: OID(bookId) )));
    while (cursor->more()) 
	{
        mongo :: BSONObj p = cursor->next();
        book = p.jsonString();
    }
	return book;
}


string DatabaseManager::getBookByNameAndAuthor (const string& title, const string& author)
{
	string book;
    auto_ptr<mongo :: DBClientCursor> cursor = auto_ptr<mongo :: DBClientCursor> (connection.query(getDatabaseName(), MONGO_QUERY("title" << title << "author" << author)));
    if (cursor->more()) 
	{
        mongo :: BSONObj p = cursor->next();
        book = p.jsonString();
    }
	return book;
}

vector<string> DatabaseManager::getBooksByAuthor(const string& author)
{
	vector<string>books;
    auto_ptr<mongo :: DBClientCursor> cursor = auto_ptr<mongo :: DBClientCursor>(connection.query(getDatabaseName(),  
				MONGO_QUERY("author" << author)));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	return books;
}

vector<string> DatabaseManager::getBooksByName(const string& bookName)
{
	vector<string>books;
    auto_ptr<mongo :: DBClientCursor> cursor = auto_ptr<mongo :: DBClientCursor>(connection.query(getDatabaseName(),  
				MONGO_QUERY("title" << bookName)));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	return books;
}

bool DatabaseManager :: addBook(const string& title, const string& author, const string& imageUrl, const string& bookUrl, string &response)
{

    mongo :: BSONObj bookBSON = BSON(mongo :: GENOID << "title" << title << "author" << author << "image_url" << imageUrl << "book_url" << bookUrl << "rating" << "0" );
    
    try
    {
        connection.insert(getDatabaseName(), bookBSON);
    }
    catch(exception &e)
    {
        response = "Incorrect request body.";
        return false;
    }
    response = bookBSON.jsonString();
    return true;
}


//void DatabaseManager::updateRating(Book book, double rating) //TODO: check!
//{
//	db.update(booksTableName,
//		BSON("title" << book.getTitle() << "author" << book.getAuthor() << "imageUrl" 
//			<< book.getImageUrl << "bookUrl" << book.getBookUrl() << "rating" << book.getRating()),
//		BSON("$inc" << BSON( "rating" << rating)));
//		
//}
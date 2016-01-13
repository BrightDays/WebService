#include "DatabaseManager.h"
#include <memory>
#include "mongo/util/net/hostandport.h"


DatabaseManager::DatabaseManager()
{
	mongo::client::initialize();
    databaseName = "test_books";
    booksTableName = "book";
	userTableName = "user";
	bookUserTableName = "book_user";
	replicaSetName = "rs1";	
}

void DatabaseManager::run() 
{
	std :: vector<mongo :: HostAndPort> servers;
	mongo :: HostAndPort s1("192.168.56.101:27017");
	mongo :: HostAndPort s2("192.168.56.102:27017");
	mongo :: HostAndPort s3("192.168.56.103:27017");
	servers.push_back(s1);
	servers.push_back(s2);
	servers.push_back(s3);
	connection = new mongo :: DBClientReplicaSet(replicaSetName, servers);
	connection->connect();//TODO: change loclahost to ip
}

std :: string DatabaseManager :: getDatabaseName(const std :: string& tableName)
{
    return databaseName + "." + tableName;
}


std :: vector<std :: string> DatabaseManager::getAllBooks()
{
	std :: vector<std :: string> books;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor>(connection->query(getDatabaseName(booksTableName), mongo :: BSONObj()));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	//if (books.size())
	//	return books;
	//books.push_back(mongo::DBClientWithCommands::getLastError(0,0,0,0));
	return books;
}

std :: string DatabaseManager::getBookById (const std :: string& bookId)
{
	std :: string book;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(booksTableName), 
	MONGO_QUERY("_id" << mongo :: OID(bookId) )));
    while (cursor->more()) 
	{
        mongo :: BSONObj p = cursor->next();
        book = p.jsonString();
    }
	return book;
}

mongo :: BSONObj DatabaseManager::getBookBsonById (const std :: string& bookId)
{
    mongo :: BSONObj p;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(booksTableName), MONGO_QUERY("_id" << mongo :: OID(bookId) )));
    while (cursor->more())
    {
        p = cursor->next();
    }
    return p;
}


std :: string DatabaseManager::getBookByNameAndAuthor (const std :: string& title, const std :: string& author)
{
	std :: string book;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(booksTableName), MONGO_QUERY("title" << title << "author" << author)));
    if (cursor->more()) 
	{
        mongo :: BSONObj p = cursor->next();
        book = p.jsonString();
    }
	return book;
}

std :: vector<std :: string> DatabaseManager::getBooksByAuthor(const std :: string& author)
{
	std :: vector<std :: string> books;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor>(connection->query(getDatabaseName(booksTableName),  
				MONGO_QUERY("author" << author)));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	return books;
}

std :: vector<std :: string> DatabaseManager::getBooksByName(const std :: string& bookName)
{
	std :: vector<std :: string>books;
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor>(connection->query(getDatabaseName(booksTableName),  
				MONGO_QUERY("title" << bookName)));
	while (cursor->more())
	{
	   books.push_back(cursor->next().jsonString());
	}
	return books;
}

bool DatabaseManager :: addBook(const std :: string& title, const std :: string& author, 
const std :: string& imageUrl, const std :: string& bookUrl, std :: string &response)
{

    mongo :: BSONObj bookBSON = BSON(mongo :: GENOID << "title" << title << "author" << author << "image_url" << imageUrl << "book_url" << bookUrl << "rating" << 0 );
    
    try
    {
        connection->insert(getDatabaseName(booksTableName), bookBSON);
    }
    catch(std :: exception &e)
    {
        response = "Incorrect request body.";
        return false;
    }
    response = bookBSON.jsonString();
    return true;
}


void DatabaseManager :: updateRating(const std :: string& bookId, int rating, const std :: string& userId)
{
	std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor>(connection->query(getDatabaseName(bookUserTableName),  
				MONGO_QUERY("bookId" << bookId << "userId" << userId)));
	//int previousUserRating = 0;
	if (cursor->more()) // if he voted
	{
		connection->update(getDatabaseName(bookUserTableName), MONGO_QUERY("bookId" << bookId << "userId" << userId), 
		BSON("$set" << BSON( "rating" << rating)));
	}
	else
	{
		//add rating, he never voted
		//add to Book_User
		connection->insert(getDatabaseName(bookUserTableName), BSON("bookId" << bookId << "userId" << userId << "rating" << rating));
		recountRating(bookId);
	}    
}

void DatabaseManager :: recountRating(const std :: string& bookId)
{
	std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor>(connection->query(getDatabaseName(bookUserTableName),  
				MONGO_QUERY("bookId" << bookId)));
	int votesCount = 0;
	long commonRating = 0;
	int ratingNumber = 0;
	while(cursor->more())
	{
		mongo :: BSONObj p = cursor->next();
        	mongo :: BSONElement rating = p.getField("rating");
		if (rating.isNumber())
			ratingNumber = rating.Int();
		else
			ratingNumber = atoi(rating.String().c_str());
		commonRating += ratingNumber;
		votesCount++;
	}
	if (votesCount > 0)
	{
		double resultRating = commonRating / votesCount;
		connection->update(getDatabaseName(booksTableName), MONGO_QUERY("_id" << mongo :: OID(bookId)), 
													BSON("$set" << BSON( "rating" << resultRating)));
	}
}

bool DatabaseManager :: addUser(const std :: string& login, const std :: string& password, std :: string &response)
{
    mongo :: BSONObj userBSON = BSON(mongo :: GENOID << "login" << login << "password" << password);
    
    try
    {
        connection->insert(getDatabaseName(userTableName), userBSON);
    }
    catch(std :: exception &e)
    {
        response = "Incorrect request body.";
        return false;
    }
    response = userBSON.jsonString();
    return true;
}


bool DatabaseManager :: checkUser (const std :: string& login, const std :: string& password, std :: string &response) // response = user id 
{
    std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(userTableName), MONGO_QUERY("login" << login << "password" << password)));
    if (cursor->more()) 
	{
		mongo :: BSONObj user = cursor->next();
		response = user.jsonString();
		return true;
    }
	return false;
}

bool DatabaseManager :: checkLoginExists(const std :: string& login) 
{
	std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(userTableName), MONGO_QUERY("login" << login)));
    if (cursor->more()) 
	{
        return true;
    }
	return false;
}


bool DatabaseManager :: userIdExists(std :: string& userId)
{
	std :: auto_ptr<mongo :: DBClientCursor> cursor = std :: auto_ptr<mongo :: DBClientCursor> (connection->query(getDatabaseName(userTableName), 
	MONGO_QUERY("_id" << mongo :: OID(userId))));
    if (cursor->more()) 
	{
        return true;
    }
	return false;
}

//void DatabaseManager::updateRating(Book book, double rating) //TODO: check!
//{
//	db.update(booksTableName,
//		BSON("title" << book.getTitle() << "author" << book.getAuthor() << "imageUrl" 
//			<< book.getImageUrl << "bookUrl" << book.getBookUrl() << "rating" << book.getRating()),
//		BSON("$inc" << BSON( "rating" << rating)));
//		
//}
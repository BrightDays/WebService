#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include "fastcgi2/stream.h"
#include <fastcgi2/data_buffer.h>

#include "DatabaseManager.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "mongo/client/dbclient.h"

//curl -H "Content-Type: application/json" -X POST -d '{"username":"xyz","password":"xyz"}' http://localhost/api/v1/books
//curl -H "Content-Type: application/json" -X POST -d '{"title" : "1984", "author" : "Orwell", "image_url" : "", "book_url" : "", "rating" : "0" }' http://localhost/api/v1/books
//curl -H "Content-Type: application/json" -X PUT --data '{"rating" : "10"}' http://localhost/api/v1/books?bookid=566aea019f4f815613e5de8a
//curl -H "Content-Type: application/json" -X POST -d '{"login":"admin","password":"admin"}' http://localhost/api/v1/users


class RequestHandler : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
        RequestHandler(fastcgi::ComponentContext *context) :
                fastcgi::Component(context) {
        }
        virtual ~RequestHandler() {
        }

public:
        virtual void onLoad() {
        }
        virtual void onUnload() {
        }
    
#pragma mark - handle request
        virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext *context)
        {
            DatabaseManager manager;
            manager.run();
            std :: string scriptName = req->getScriptName();
            // len("/api/v1/") = 8
            int startPosition = 8;
            std :: string collection = scriptName.substr(startPosition);
            if (collection == "books")
            {
                std :: string requestMethod = req->getRequestMethod();
                if (requestMethod == "POST")
                {
                    handlePostRequest(req, context, manager);
                }
                if (requestMethod == "GET")
                {
                    handleGetRequest(req, context, manager);
                }
                if (requestMethod == "PUT")
                {
                    handlePutRequest(req, context, manager);
                }
            }
            if (collection == "users")
            {
                std :: string requestMethod = req->getRequestMethod();
                if (requestMethod == "POST")
                {
                    handlePostRequestToUsers(req, context, manager);
                }
            }
        }

#pragma mark - books requests
    
        void handlePutRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);

//			std::vector<std::string> names;
//			req->argNames(names);
//			for(std::vector<std::string> :: iterator it = names.begin(); it != names.end(); it++)
//				stream << *it << " ";
//			stream << req->countArgs();

            if (req->hasArg("bookid") && req->hasArg("userid") && req->countArgs() == 3)
            {
                fastcgi :: DataBuffer buffer = req->requestBody();
                std :: string jsonString;
                buffer.toString(jsonString);
                mongo :: BSONObj bookBSON = mongo::fromjson(jsonString);
                mongo :: BSONElement rating = bookBSON.getField("rating");
                std :: string bookId = req->getArg("bookid");
				std :: string userId = req->getArg("userid");
                bool userIdExists = manager.userIdExists(userId);
                if (!userIdExists)
                {
                    std :: string message = "Incorrect user";
                    sendError(req, stream, message, 401);
                    return;
                }
                int ratingNumber = 0;
                try
                {
                    if (rating.isNumber())
                        ratingNumber = rating.Int();
                    else
                        ratingNumber = atoi(rating.String().c_str());
                }
                catch(std :: exception e)
                {
                    sendError(req, stream, 400);
                    return;
                }
                if (ratingNumber >= 0 && ratingNumber <= 10)
                {
                    manager.updateRating(bookId, ratingNumber, userId);//TODO: user id required
                } else
                {
                    sendError(req, stream, 400);
                    return;
                }
            } else
            {
                sendError(req, stream, 400);
                return;
            }
        }
    
        void handlePostRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {        
			fastcgi::RequestStream stream(req);
			
			std::vector<std::string> names;
			req->argNames(names);
			for(std::vector<std::string> :: iterator it = names.begin(); it != names.end(); it++)
				stream << *it << " ";
            fastcgi :: DataBuffer buffer = req->requestBody();
            std :: string jsonString;
            buffer.toString(jsonString);
            
            mongo :: BSONObj bookBSON = mongo::fromjson(jsonString);
            mongo :: BSONElement title = bookBSON.getField("title");
            mongo :: BSONElement author = bookBSON.getField("author");
            mongo :: BSONElement imageUrl = bookBSON.getField("image_url");
            mongo :: BSONElement bookUrl = bookBSON.getField("book_url");
            if (!title.eoo() && !author.eoo())
            {
                if (title.valuestrsize() > 1 && author.valuestrsize() > 1)
                {
                    std :: string response;
                    bool success = manager.addBook(title.str(), author.str(), imageUrl.str(), bookUrl.str(), response);
                    if (success)
                    {
                        stream << response;
                        return;
                    }
                    sendError(req, stream, response, 400);
                } else
                {
                    sendError(req, stream, 400);
                }
            } else
            {
                std :: string message = "Incorrect title or author name.";
                sendError(req, stream, message, 400);
            }
        }
    
        void sendError(fastcgi::Request *req,fastcgi::RequestStream &stream, std :: string &message, int status)
        {
            stream << "{ \"error\" : \"" + message + "\" }";
            req->setStatus(status);
        }
    
        void sendError(fastcgi::Request *req,fastcgi::RequestStream &stream, int status)
        {
            std :: string message = "Incorrect request body";
            sendError(req, stream, message, status);
        }
    
        void handleGetRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);
			if (req->countArgs() == 0)
			{
				std :: vector<std :: string> books = manager.getAllBooks();
				std :: string s;
				for(int i = 0; i < books.size(); i++)
					s += books[i];
				stream << "ALL BOOKS: " << s << " \n";
			}
            
            if (req->hasArg("bookid") && req->countArgs() == 1)
            {
                std :: string bookId = req->getArg("bookid");
                stream << "BOOK : " << manager.getBookById(bookId) << " \n";
            }
            else if (req->hasArg("author")&& req->countArgs() == 1)
            {
                std :: vector<std :: string> books = manager.getBooksByAuthor(req->getArg("author"));
                std :: string booksJSON;
                for(int i = 0; i < books.size(); i++)
                    booksJSON += books[i];
                stream << "Books by author: " << booksJSON;
            }
            else if (req->hasArg("bookname")&& req->countArgs() == 1)
            {
                std :: vector<std :: string> books = manager.getBooksByName(req->getArg("bookname"));
                std :: string booksJSON;
                for(int i = 0; i < books.size(); i++)
                    booksJSON += books[i];
                stream << "Books by name: " << booksJSON;
            }
            else if(req->countArgs() == 2 && req->hasArg("bookname")&& req->hasArg("author"))
            {
                std :: string booksJSON = manager.getBookByNameAndAuthor(req->getArg("bookname"), req->getArg("author"));
                stream << "Books by name and author: " << booksJSON; 
            }
        }
    
#pragma mark - users requests
    
    void handlePostRequestToUsers(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
    {
        fastcgi::RequestStream stream(req);
        
        fastcgi :: DataBuffer buffer = req->requestBody();
        std :: string jsonString;
        buffer.toString(jsonString);
        
        mongo :: BSONObj bookBSON = mongo::fromjson(jsonString);
        mongo :: BSONElement login = bookBSON.getField("login");
        mongo :: BSONElement password = bookBSON.getField("password");
        if (!login.eoo() && !password.eoo())
        {
            if (login.valuestrsize() > 1 && password.valuestrsize() > 1)
            {
                if (!req->hasArg("signup") || (req->hasArg("signup") && req->getArg("signup") == "0"))
                {
                    std :: string response;
                    bool success = manager.checkUser(login.str(), password.str(), response);
                    if (success)
                    {
                        stream << response;
                        return;
                    }
                    std :: string message = "Incorrect login or password";
                    sendError(req, stream, message, 401);
                } else
                {
                    std :: string response;
                    bool loginExists = manager.checkLoginExists(login.str());
                    if (!loginExists)
                    {
                        bool success = manager.addUser(login.str(), password.str(), response);
                        if (success)
                        {
                            stream << response;
                            return;
                        }
                        sendError(req, stream, response, 400);
                    } else
                    {
                        std :: string message = "Such login exists";
                        sendError(req, stream, message, 401);
                    }
                }
            } else
            {
                sendError(req, stream, 400);
            }
        } else
        {
            std :: string message = "Incorrect login or password.";
            sendError(req, stream, message, 401);
        }
    }
    
    
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", RequestHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
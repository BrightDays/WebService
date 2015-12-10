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
//curl -H "Content-Type: application/json" -X PUT --data '{"rating" : "10"}' http://localhost/api/v1/books?bookid=5668975e10a1fafab87227e5


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
        virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext *context)
        {
            DatabaseManager manager;
            manager.run();

            string requestMethod = req->getRequestMethod();
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
    
        void handlePutRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);
            if (req->hasArg("bookid") && req->hasArg("userid") && req->countArgs() == 3)
            {
                fastcgi :: DataBuffer buffer = req->requestBody();
                string jsonString;
                buffer.toString(jsonString);
                mongo :: BSONObj bookBSON = mongo::fromjson(jsonString);
                mongo :: BSONElement rating = bookBSON.getField("rating");
                string bookId = req->getArg("bookid");
				string userId = req->getArg("userid");
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
            fastcgi :: DataBuffer buffer = req->requestBody();
            string jsonString;
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
                    string response;
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
                string message = "Incorrect title or author name.";
                sendError(req, stream, message, 400);
            }
        }
    
        void sendError(fastcgi::Request *req,fastcgi::RequestStream &stream, string &message, int status)
        {
            stream << "{ \"error\" : \"" + message + "\" }";
            req->setStatus(status);
        }
    
        void sendError(fastcgi::Request *req,fastcgi::RequestStream &stream, int status)
        {
            string message = "Incorrect request body";
            sendError(req, stream, message, status);
        }
    
        void handleGetRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);
//            vector<string> books = manager.getAllBooks();
//            string s;
//            for(int i = 0; i < books.size(); i++)
//                s += books[i];
//            stream << "ALL BOOKS: " << s << " \n";
//            return;
            
            if (req->hasArg("bookid") && req->countArgs() == 1)
            {
                string bookId = req->getArg("bookid");
                stream << "BOOK : " << manager.getBookById(bookId) << " \n";
            }
            else if (req->hasArg("author")&& req->countArgs() == 1)
            {
                vector<string> books = manager.getBooksByAuthor(req->getArg("author"));
                string booksJSON;
                for(int i = 0; i < books.size(); i++)
                    booksJSON += books[i];
                stream << "Books by author: " << booksJSON;
            }
            else if (req->hasArg("bookname")&& req->countArgs() == 1)
            {
                vector<string> books = manager.getBooksByName(req->getArg("bookname"));
                string booksJSON;
                for(int i = 0; i < books.size(); i++)
                    booksJSON += books[i];
                stream << "Books by name: " << booksJSON;
            }
            else if(req->countArgs() == 2 && req->hasArg("bookname")&& req->hasArg("author"))
            {
                string booksJSON = manager.getBookByNameAndAuthor(req->getArg("bookname"), req->getArg("author"));
                stream << "Books by name and author: " << booksJSON; 
            }
        }
    
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", RequestHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
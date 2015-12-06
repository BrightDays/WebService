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
        }
    
        void handlePostRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);
            fastcgi :: DataBuffer buffer = req->requestBody();
            string jsonString;
            buffer.toString(jsonString);
            stream << jsonString << "\n";
            
            mongo :: BSONObj bookBSON = mongo::fromjson(jsonString);
            mongo :: BSONElement title = bookBSON.getField("title");
            mongo :: BSONElement author = bookBSON.getField("author");
            mongo :: BSONElement imageUrl = bookBSON.getField("image_url");
            mongo :: BSONElement bookUrl = bookBSON.getField("book_url");
            if (!title.eoo() && !author.eoo())
            {
                if (title.valuestrsize() > 1 && author.valuestrsize() > 1)
                {
                    manager.addBook(title.str(), author.str(), imageUrl.str(), bookUrl.str());
                }
            }
        }
    
        void handleGetRequest(fastcgi::Request *req, fastcgi::HandlerContext *context, DatabaseManager &manager)
        {
            fastcgi::RequestStream stream(req);
            //            vector<string> books = manager.getAllBooks();
            //            string s;
            //            for(int i = 0; i < books.size(); i++)
            //                s += books[i];
            //            stream << "ALL BOOKS: " << s << " \n";
            
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
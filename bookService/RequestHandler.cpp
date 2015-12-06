#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include "fastcgi2/stream.h"
#include "DatabaseManager.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

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
            fastcgi::RequestStream stream(req);
            DatabaseManager manager;
            manager.run();
            vector<string> books = manager.getAllBooks();
            string s;
            for(int i = 0; i < books.size(); i++)
                s += books[i];
            stream << "ALL BOOKS: " << s << " \n";
            
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
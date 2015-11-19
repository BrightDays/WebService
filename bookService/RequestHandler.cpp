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
            
            if (req->hasArg("bookid"))
            {
                string bookId = req->getArg("bookid");
                stream << "BOOK : " << manager.getBookById(bookId) << " \n";
            }
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", RequestHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
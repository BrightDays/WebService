#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include "fastcgi2/stream.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdio.h>

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
        virtual void handleRequest(fastcgi::Request *req, fastcgi::HandlerContext *context) {
		fastcgi::RequestStream stream(req);
		//req->setHeader("Simple-Header", "Reply from csimple");
		stream << "Hello, world!";
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", RequestHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
g++ DatabaseManager.cpp -I $HOME/mongo-client-install/include -L $HOME/mongo-client-install/lib  -pthread -lmongoclient -lboost_thread-mt -lboost_system -lboost_regex -lrt -Wall -c

g++ DatabaseManager.o RequestHandler.cpp -fPIC -lfastcgi-daemon2 -I $HOME/mongo-client-install/include -L $HOME/mongo-client-install/lib -pthread -lmongoclient -lboost_thread-mt -lboost_system -lboost_regex -lrt -shared -o requesthandler.so

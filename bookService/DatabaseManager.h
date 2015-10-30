#include "sqlite3.h"
#include "Book.h"
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

using namespace std;


class DatabaseManager 
{
private: 
	string databaseName;

public:
	DatabaseManager();
	vector<Book> getAllBooks();
};

#include "DatabaseManager.h"
#include <vector>
#include "Book.h"
#include <iostream>

using namespace std;

int main() 
{
	DatabaseManager db;
	vector<Book> v = db.getAllBooks();
	for(int i = 0; i < v.size(); i++) {
		cout << v[i].getAuthor() << ' ';
    }


	return 0;
}
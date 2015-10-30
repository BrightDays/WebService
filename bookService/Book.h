#include <string>
#include <stdio.h>


using namespace std;

class Book
{
private:
	string title;
	string author;
	string imageUrl;
	string bookUrl;
	double rating;
public: 
	Book(string _title, string _author, string _imageUrl, string _bookUrl, double _rating):title(_title), author(_author), 
		imageUrl(_imageUrl), bookUrl(_bookUrl), rating(0){}
	Book(string _title, string _author, string _imageUrl, string _bookUrl, double _rating):title(_title), author(_author), 
		imageUrl(_imageUrl), bookUrl(_bookUrl), rating(_rating){}
	string getTitle();
	string getAuthor();
	string getImageUrl();
	string getBookUrl();
	double getRating();

	~Book(){};
};
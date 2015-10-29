#include "JSONManager.h"




string JSONManager :: makeJsonFromBooks(const vector<Book> &books)
{
    Document document;
    
    string json = "{  }";
    if (document.Parse<0>(json).HasParseError())
        return "";
    
}

Book JSONManager :: getBookFromJson(const string &json)
{
    rapidjson :: Document document;
    document.Parse<0>(json);
    
    string title = "";
    string author = "";
    string imageUrl = "";
    string bookUrl = "";
    if (document.HasMember("title") && document["title"].IsString())
        title = document["title"].GetString();
    else
        return null;
    
    if (document.HasMember("author") && document["author"].IsString())
        author = document["author"].GetString();
    else
        return null;
    
    if (document.HasMember("img_url") && document["img_url"].IsString())
        imageUrl = document["img_url"].GetString();
    
    if (document.HasMember("book_url") && document["book_url"].IsString())
        bookUrl = document["book_url"].GetString();
    
    return new Book(title, author, imageUrl, book_url);
}


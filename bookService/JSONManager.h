#ifndef _JSONMANAGER_H_
#define _JSONMANAGER_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "rapidjson/document.h"
#include "book.h"

class JSONManager{
public:
    string makeJsonFromBooks(const vector<Book> &books);
    Book getBookFromJson(const string &json);
};

#endif
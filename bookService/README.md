
# Books

### PUT /api/v1/books/BOOK_ID 

где BOOK_ID - уже известная книга, тогда обновляем рейтинг, если запрос корректен(rating = [0, 10]), иначе HTTP-error: 403

#### Пример входных данных:

    {
        “rating” : “5”
    }

#### Коды возврата:
200 - Success <br/>
400 - Error. Invalid parameters <br/>
403 - Forbidden. Need authentication <br/>
404 - Invalid url <br/>


### POST /api/v1/books 

#### Пример входных данных:

	{
		“title” : “451”,
        “author” : “Ray”,
        “img_url” : “/api/v1/books/123/icon.jpg”,
        “book_url” : “/api/v1/books/BOOK_ID/123.fb2”
	}


где BOOK_ID - сгенерированный хеш  для книги  <br/>

#### Пример выходных данных:

	{
		“title” : “451”,
        “author” : “Ray”,
        “rating” : “5”,
        “img_url” : “/api/v1/books/123/icon.jpg”,
        “book_url” : “/api/v1/books/BOOK_ID/123.fb2”
	}
		
#### Коды возврата:
200 - Success <br/>
400 - Error <br/>
404 - Invalid url <br/>


### GET /api/v1/books?bookid=BOOK_ID 

Доступ к конкретной книге.

##### Коды возврата:

200 - Success <br/>
400 - Error <br/>
404 - Invalid url <br/>

#### Пример возвращаемых данных:
    {
        “title” : “451”,
        “author” : “Ray”,
        “rating” : “5”,
        “img_url” : “/api/v1/books/123/icon.jpg”,
        “book_url” : “/api/v1/books/BOOK_ID/123.fb2”
    }

img_url - обложка книги, загружается пользователем при создании книги; ее может не быть.
book_url  - ссылка на скачивание книги, ее может не быть, загружается пользователем при создании книги.




#### GET/api/v1/books?page=PAGE_NUMBER&limit=LIMIT&rating=RATING
#### GET /api/v1/books?page=PAGE_NUMBER&limit=LIMIT
#### GET /api/v1/books?bookname=BOOK_NAME&author=AUTHOR_NAME
#### GET /api/v1/books?author=AUTHOR_NAME
#### GET /api/v1/books?bookname=BOOK_NAME


#### Коды возврата:
200 - Success <br/>
400 - Error <br/>
404 - Invalid url <br/>

#### Пример

PAGE_NUMBER = 2
LIMIT = 2
GET /api/v1/books?page=2&limit=2

    {
        “books”: 
        [
            {
            “title” : “451”,
            “author” : “Ray”,
            “rating” : “5”
            “img_url” : “/api/v1/books/123/icon.jpg”
            “book_url” : “/api/v1/books/123/123.fb2”
            } ,
            {
            “title” : “Ann”,
            “author” : “Leo”,
            “rating” : “5”
            “img_url” : “/api/v1/books/124/icon.jpg”
            “book_url” : “/api/v1/books/124/124.fb2”
            }
        ]
    }




# Books

### GET /api/v1/books/BOOK_ID 

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



### GET /api/v1/books

Доступ ко всем книгам.

#### Коды возврата:
200 - Success <br/>
400 - Error <br/>
404 - Invalid url <br/>

#### Пример возвращаемых данных:

    [
        {
            “title” : “451”,
            “author” : “Ray”,
            “rating” : “5”
            “img_url” : “/api/v1/books/123/icon.jpg”
            “book_url” : “/api/v1/books/123/123.fb2”
        },
        {
            “title” : “Ann”,
            “author” : “Lev”,
            “rating” : “4”
            “img_url” : “/api/v1/books/125/icon.jpg”
            “book_url” : “/api/v1/books/125/125.fb2”
        }
    ]

### Другие GET запросы

#### GET /api/v1/books?bookname=BOOK_NAME&author=AUTHOR_NAME
#### GET /api/v1/books?author=AUTHOR_NAME
#### GET /api/v1/books?bookname=BOOK_NAME


#### Коды возврата:
200 - Success <br/>
400 - Error <br/>
404 - Invalid url <br/>

#### Пример

GET /api/v1/books?bookname=451

    [
        {
            “title” : “451”,
            “author” : “Ray”,
            “rating” : “5”
            “img_url” : “/api/v1/books/123/icon.jpg”
            “book_url” : “/api/v1/books/123/123.fb2”
        }
    ]



### PUT /api/v1/books/BOOK_ID?userid=USER_ID

где BOOK_ID - уже известная книга, USER_ID зарегистрированный пользователь. Тогда обновляем рейтинг, если запрос корректен(rating = [0, 10])

#### Пример входных данных:

    {
        “rating” : “5”
    }

или

    {
        “rating” : 8
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

#### Пример выходных данных:

	{
		“title” : “451”,
        “author” : “Ray”,
        “rating” : “5”,
        “img_url” : “/api/v1/books/123/icon.jpg”,
        “book_url” : “/api/v1/books/BOOK_ID/123.fb2”
	}

где BOOK_ID - сгенерированный хеш  для книги  <br/>

#### Коды возврата:
200 - Success <br/>
400 - Error <br/>
Response json:

    {
        "error" : message
    }

404 - Invalid url <br/>


# Users

### POST /api/v1/users/signup

Регистрация

##### Коды возврата:

200 - Success <br/>
400 - Error <br/>
401 - Such login exists <br/>
404 - Invalid url <br/>

#### Пример входных данных:
    {
        "login" : "admin",
        "password" : "admin"
    }

#### Пример возвращаемых данных:
    {
        "login" : "admin",
        "password" : "admin",
        "userid" : 13jklqjkdsa98123
    }



### POST /api/v1/users/signin

Авторизация

##### Коды возврата:

200 - Success <br/>
400 - Error <br/>
401 - Incorrect login or password <br/>
404 - Invalid url <br/>

#### Пример входных данных:
    {
        "login" : "admin",
        "password" : "admin"
    }

#### Пример возвращаемых данных:
    {
        "login" : "admin",
        "password" : "admin",
        "userid" : 13jklqjkdsa98123
    }



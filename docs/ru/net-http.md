[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

Модуль предоставляет инструменты для работы с протоколом HTTP, включая парсинг запросов/ответов, формирование сообщений и функции для создания HTTP-клиентов и серверов.

- [ncpp::http::HttpMsg](#ncpphttphttpmsg)
- [ncpp::http::Req](#ncpphttpreq)
    - [Req.hasLength()](#reqhaslength)
    - [Req.cout()](#reqcout)
- [ncpp::http::Res](#ncpphttpres)
    - [Res.write()](#reswrite)
    - [Res.end()](#resend)
    - [Res.ok()](#resok)
    - [Res.hasLength()](#reshaslength)
    - [Res.Redirect()](#resredirect)
    - [Res.SendCode()](#ressendcode)
    - [Res.SendErr()](#ressenderr)
    - [Res.cout()](#rescout)
- [ncpp::http::urlDecoder()](#ncpphttpurldecoder)
- [ncpp::http::urlEncoder()](#ncpphttpurlencoder)
- [ncpp::http::GetMIMEType()](#ncpphttpgetmimetype)
- [ncpp::http::HttpParse()](#ncpphttphttpparse)
- [ncpp::http::ReqParse()](#ncpphttpreqparse)
- [ncpp::http::RespParse()](#ncpphttprespparse)
- [ncpp::http::request()](#ncpphttprequest)
- [ncpp::http::fetchd()](#ncpphttpfetchd)
- [ncpp::http::SaveStream()](#ncpphttpsavestream)
- [ncpp::http::getbodyreq()](#ncpphttpgetbodyreq)
- [ncpp::http::SendFile()](#ncpphttpsendfile)
- [ncpp::http::AutoIndex()](#ncpphttpautoindex)
- [ncpp::http::RenderHtml()](#ncpphttprenderhtml)

## ncpp::http::HttpMsg
Базовая структура HTTP сообщения.
```cpp
struct HttpMsg { StringMap headers; Buffer body; };
```

## ncpp::http::Req
Структура HTTP запроса, наследуется от `HttpMsg`. Содержит метод, URL и версию протокола.
```cpp
struct Req : HttpMsg { 
    float hver; 
    String method; 
    String url; 
    bool nobody; 
    bool bad; 
};
```

### Req.hasLength()
```cpp
bool hasLength();
```
Проверяет наличие заголовков `content-length` или `transfer-encoding` для определения наличия тела сообщения.

### Req.cout()
```cpp
String cout() const;
```
Возвращает строковое представление запроса (метод, URL, заголовки и тело) для отладки.

---

## ncpp::http::Res
Структура HTTP ответа. Используется как для формирования ответа на сервере, так и для хранения результата запроса клиента.
```cpp
struct Res : HttpMsg { 
    float hver; 
    int status; 
    TCPSocket* socket; 
};
```

### Res.write()
```cpp
void write(const char* ptr, size_t len);
void write(const char* ptr);
void write(const Buffer& data);
```
Отправляет часть данных в сокет. Если это первый вызов `write`, автоматически отправляет HTTP-заголовки (используя chunked encoding, если размер не задан).

### Res.end()
```cpp
void end(const Buffer& data = Buffer());
```
Завершает формирование и отправку ответа. Если данные переданы, они добавляются в тело. Закрывает соединение, если `Connection: close`.

### Res.ok()
```cpp
bool ok();
```
Возвращает `true`, если HTTP статус находится в диапазоне 200-299.

### Res.hasLength()
```cpp
bool hasLength();
```
Проверяет, задана ли длина контента в заголовках.

### Res.Redirect()
```cpp
void Redirect(const CString& url, int code = 302);
```
Отправляет ответ с перенаправлением на указанный URL.

### Res.SendCode()
```cpp
void SendCode(int status1, const CString& codeDescr = "");
```
Отправляет пустой ответ с указанным кодом состояния.

### Res.SendErr()
```cpp
void SendErr(int status1, const CString& errDescr = "");
```
Отправляет код ошибки и немедленно разрывает соединение через `socket->destroy()`.

### Res.cout()
```cpp
String cout() const;
```
Возвращает строковое представление статуса и заголовков ответа.

---

## ncpp::http::urlDecoder()
```cpp
String urlDecoder(const CString& str);
```
Декодирует строку из URL-encoded формата (заменяет `%XX` на символы и `+` on пробелы).

## ncpp::http::urlEncoder()
```cpp
String urlEncoder(const CString& str);
```
Кодирует строку для безопасной передачи в URL.

## ncpp::http::GetMIMEType()
```cpp
String GetMIMEType(const CString& ext);
```
Возвращает строку MIME-типа на основе расширения файла (например, `html` -> `text/html`).

## ncpp::http::HttpParse()
```cpp
HttpMsg HttpParse(const Buffer& rawreq, size_t pos = NPOS);
```
Внутренняя функция для парсинга заголовков и тела из сырого буфера данных.

## ncpp::http::ReqParse()
```cpp
Req ReqParse(const Buffer& rawreq);
```
Парсит сырой буфер данных в структуру запроса `Req`.

## ncpp::http::RespParse()
```cpp
Res RespParse(const Buffer& rawreq);
```
Парсит сырой буфер данных в структуру ответа `Res`.

## ncpp::http::request()
```cpp
template <typename S>
Res request(S& socket, const CString& host, int port, Req& params);
```
Выполняет низкоуровневый HTTP-запрос через предоставленный сокет. Поддерживает Chunked transfer encoding.

## ncpp::http::fetchd()
```cpp
Res fetchd(String url, Req& params);
Res fetchd(const CString& url);
```
Высокоуровневая функция для выполнения GET/POST запросов по URL. На данный момент поддерживает только протокол HTTP (порт 80).

## ncpp::http::SaveStream()
```cpp
bool SaveStream(String url, const CString& fpath, String* err = NULL);
```
Скачивает ресурс по URL напрямую в файл `fpath`. Позволяет работать с большими файлами, не загружая их целиком в память.

## ncpp::http::getbodyreq()
```cpp
Buffer getbodyreq(Req& req, Res& res);
```
Дочитывает тело запроса из сокета, если оно не было получено полностью при начальном парсинге (основываясь на `Content-Length`).

## ncpp::http::SendFile()
```cpp
void SendFile(const CString& fpath, Res& res);
```
Читает файл с диска и отправляет его клиенту с правильным заголовком `Content-Type`. Если файл не найден, отправляет 404.

## ncpp::http::AutoIndex()
```cpp
void AutoIndex(const CString& dirpath, Res& res);
```
Генерирует HTML-страницу со списком файлов в директории `dirpath` (аналог `Options +Indexes` в Apache).

## ncpp::http::RenderHtml()
```cpp
String RenderHtml(const CString& html, const StringMap& tokens, CString oTag = "{{%", CString cTag = "%}}");
```
Простейший шаблонизатор. Заменяет вхождения `{{% key %}}` в строке `html` на значения из `tokens[key]`.
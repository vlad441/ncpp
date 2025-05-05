[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::http::HttpMsg](#ncpphttphttpmsg)
- [ncpp::http::Req](#ncpphttpreq)
	- [Req.cout()](#reqcout)
- [ncpp::http::Res](#ncpphttpres)
	- [Res.write()](#reswrite)
	- [Res.end()](#resend)
	- [Res.ok()](#resok)
	- [Res.hasLength()](#reshaslength)
	- [Res.Redirect()](#resredirect)
	- [Res.SendErr()](#ressenderr)
	- [Res.cout()](#rescout)
- [ncpp::http::request()](#ncpphttprequest)
- [ncpp::http::fetchd()](#ncpphttpfetchd)
- [ncpp::http::SaveStream()](#ncpphttpSaveStream)
	
## ncpp::http::HttpMsg
```cpp
struct HttpMsg { StringMap headers; Buffer body; };
```
Структура, содержащая заголовки и тело `HTTP` запроса. Является `POD` структурой.

## ncpp::http::Req
```cpp
struct Req : HttpMsg { float hver; std::string method; std::string url; bool nobody; Req(); };
```
Структура, содержащая информацию о `HTTP` запросе. Имеет конструктор.

### Req.cout()
```cpp
std::string cout() const;
```
Выводит содержание запроса в читаемой форме.

## ncpp::http::Res
```cpp
struct Res : HttpMsg { float hver; int status; TCPSocket* socket; Res(); }
```
Структура, содержащая информацию о `HTTP` ответе.  Имеет конструктор.

### Res.write()
```cpp
void write(const Buffer& data);
```
Записывает ответ в поток.

### Res.end()
```cpp
void end(const Buffer& data=Buffer());
```
Записывает окончательный ответ в поток и завершает запрос.

### Res.ok()
```cpp
void end(const Buffer& data=Buffer());
```
Возвращает, является ли код статуса запроса успешным.

### Res.ok()
```cpp
bool hasLength();
```
Проверяет, содержат ли заголовки длину запроса, и не является ли запрос `chunked`.

### Res.Redirect()
```cpp
void Redirect(const std::string& url);
```
Выполняет `301` редирект на указанный ресурс.

### Res.SendErr()
```cpp
void SendErr(int status1);
```
Отправляет код статуса ошибки и завершает запрос.

### Res.cout()
```cpp
std::string cout() const;
```
Выводит содержание ответа в читаемой форме.

## ncpp::http::request()
```cpp
template <typename S>	
Res request(S& socket, const std::string& host, int port, Req& params);
Res request(S& socket, const std::string& host, int port=80);
```
Выполняет `HTTP` запрос через предоставленный сокет и ждет ответ.

## ncpp::http::fetchd()
```cpp
Res fetchd(std::string url, Req& params);
Res fetchd(std::string url);
```
Выполняет `HTTP` запрос и ждет ответ. Может принимать классический `url` в виде `http://host/url`.

## ncpp::http::SaveStream()
```cpp
bool SaveStream(std::string url, std::string fpath, String* err=NULL);
```
Выполняет `HTTP` запрос и записывает тело ответа в файл.








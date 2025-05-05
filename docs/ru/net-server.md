[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::TCPServer](#ncpptcpserver)
	- [TCPServer.broadcast()](#tcpserverbroadcast)
	- [TCPServer.run()](#tcpserverrun)
	- [TCPServer.close()](#tcpserverclose)
	- [TCPServer.onConnect](#tcpserveronсonnect)
	- [TCPServer.onData](#tcpserverondata)
	- [TCPServer.onError](#tcpserveronerror)
	- [TCPServer.onClose](#tcpserveronclose)
- [ncpp::HTTPServer](#ncpphttpserver)
	- [HTTPServer.onRequest](#httpserveronrequest)

## ncpp::TCPServer
```cpp
TCPServer();
TCPServer(int port, const std::string& bindip="::");
```
Структура `TCP` сервера. При указании параметров автоматически выполняет `init()`.

### TCPServer.broadcast()
```cpp
void broadcast(const Buffer& buff);
```
Выполняет отправку всем подключенным сокетам.

### TCPServer.run()
```cpp
void run();
```
Запускает сервер, активируя event-loop обработки сокетов. Блокирует поток.

### TCPServer.close()
```cpp
void close();
```
Завершает работу сервера, закрывая дескриптор сокета сервера.

### TCPServer.onConnect
```cpp
void (*onConnect)(TCPSocket& socket);
```
Callback в виде указателя на функцию, который вызывается для сокета при его подключении.

### TCPServer.onData
```cpp
void (*onData)(TCPSocket& socket, const Buffer& data);
```
Callback в виде указателя на функцию, который вызывается для сокета при получении данных.

### TCPServer.onError
```cpp
void (*onError)(TCPSocket& socket);
```
Callback в виде указателя на функцию, который вызывается для сокета при ошибке.

### TCPServer.onClose
```cpp
void (*onClose)(TCPSocket& socket);
```
Callback в виде указателя на функцию, который вызывается для сокета при закрытии соединения.

## ncpp::HTTPServer
```cpp
HTTPServer();
HTTPServer(int port, const std::string& bindip="::");
```
Нследует от `ncpp::TCPServer`.

### HTTPServer.onRequest
```cpp
void (*onRequest)(http::Req req, http::Res res);
```
Callback в виде указателя на функцию, который вызывается при получении http запроса.

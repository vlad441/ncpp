Модуль предоставляет кроссплатформенную реализацию асинхронных серверов.

- [ncpp::SocketPool](#ncppsocketpool)
    - [SocketPool.waitWriteOne()](#socketpoolwaitwriteone)
- [ncpp::TCPServer](#ncpptcpserver)
    - [TCPServer.bind()](#tcpserverbind)
    - [TCPServer.broadcast()](#tcpserverbroadcast)
    - [TCPServer.run()](#tcpserverrun)
	- [TCPServer.close()](#tcpserverclose)
	- [TCPServer.onConnect](#tcpserveronсonnect)
	- [TCPServer.onData](#tcpserverondata)
	- [TCPServer.onError](#tcpserveronerror)
	- [TCPServer.onClose](#tcpserveronclose)
- [ncpp::HTTPServer](#ncpphttpserver)
    - [HTTPServer.run()](#httpserverrun)
	- [HTTPServer.onRequest](#httpserveronrequest)

## ncpp::SocketPool
Базовый класс для управления пулом сокетов и обработки асинхронных событий.

```cpp
struct SocketPool : AsyncIO { HashSet<Socket*> sockets; int servfd;
    // Коллбэки событий
    void (*onConnect)(TCPSocket& socket);
    void (*onData)(TCPSocket& socket, const Buffer& data);
    void (*onError)(TCPSocket& socket);
    void (*onClose)(TCPSocket& socket);
};
```

### SocketPool.waitWriteOne()
```cpp
bool waitWriteOne(int timeout=-1)
```
Ждет пока хотя бы один сокет станет доступен для записи. Возвращает `true` если хотя бы 1 открыт для записи.

## ncpp::TCPServer
Наследуется от `SocketPool`. Реализует логику стандартного TCP сервера.

```cpp
TCPServer();
TCPServer(int port, const CString& bindip="::");
```

### TCPServer.bind()
```cpp
bool bind(int port, const CString& bindip = "::");
```
Привязывает сервер к указанному порту и IP-адресу, после чего переходит в режим прослушивания (`listen`).

### TCPServer.broadcast()
```cpp
void broadcast(const Buffer& buff);
```
Рассылает данные буфера всем подключенным в данный момент клиентам.

### TCPServer.run()
```cpp
void run();
```
Блокирует поток, запуская цикл обработки событий сервера.

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
Специализированный сервер для обработки HTTP запросов, наследуется от `TCPServer`.

```cpp
HTTPServer();
HTTPServer(int port, const CString& bindip="::");
```
Параметры класса:
```cpp
struct HTTPServer : TCPServer {
    unsigned int max_headerlen; // По умолчанию 8 КБ
    unsigned int max_bodylen;   // По умолчанию 4 МБ
	//Если заголовки или тело превышают установленные, сервер автоматически отправляет ошибки `431 Request Header Fields Too Large` или `413 Payload Too Large` и закрывает соединение.
};
```

### HTTPServer.run()
```cpp
void run();
```
Блокирует поток, запуская цикл обработки событий сервера.

### HTTPServer.onRequest
```cpp
void (*onRequest)(http::Req& req, http::Res& res);
```
Callback в виде указателя на функцию, который вызывается при получении http запроса.
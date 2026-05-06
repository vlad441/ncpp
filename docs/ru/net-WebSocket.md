Модуль предоставляет кроссплатформенную реализацию асинхронных серверов.

- [ncpp::WebSocket](#ncppwebsocket)
    - [WebSocket.connect()](#websocketconnect)
    - [WebSocket.send()](#websocketsend)
    - [WebSocket.ping()](#websocketping)
    - [WebSocket.close()](#websocketclose)
    - [WebSocket.recv()](#websocketrecv)
- [ncpp::WServer](#ncppwserver)
    - [WServer.onConnect](#wserveronconnect)
    - [WServer.onMessage](#wserveronmessage)

## ncpp::WebSocket
`WebSocket` - реализует логику фреймов WebSocket, наследует от `TCPSocket`.

```cpp
WebSocket();
WebSocket(const CString& ip, int port, bool toconn=false);
WebSocket(const IPAddr& addr, bool toconn=false);
WebSocket(const Socket& sock);
```

### WebSocket.connect()
```cpp
bool connect(const CString& ip, int port);
bool connect(const IPAddr& a);
bool connect();
```
Выполняет HTTP-рукопожатие с сервером.

### WebSocket.send()
```cpp
int send(const char* cptr, char opcode = 1);      // Текстовый фрейм (opcode 1)
int send(const Buffer& buff, char opcode = 2);   // Бинарный фрейм (opcode 2)
```
Упаковывает данные в RFC-совместимый фрейм. Если объект работает в режиме клиента, данные автоматически маскируются (XOR-шифрование).

### WebSocket.recv()
```cpp
Buffer recv(WSFrameInfo* winfo = NULL);
```
Читает данные из сокета и декодирует их. Если передан указатель на `WSFrameInfo`, в него запишется мета-информация о фрейме (fin-бит, opcode, наличие маски).

### WebSocket.ping()
```cpp
void ping();
void pong();
```
Отправка управляющих фреймов для проверки активности соединения (Keep-Alive).

### WebSocket.close()
```cpp
void close(int code = 1000, const CString& reason = "");
```
Корректное закрытие соединения с отправкой статус-кода (по умолчанию 1000 — Normal Closure) и текстовой причины.

## ncpp::WServer
`WServer` - специализированный сервер, расширяющий `HTTPServer`. Автоматически перехватывает HTTP-запросы на обновление протокола и переводит сокет в режим `WebSocket`.

```cpp
WServer();
WServer(int port, const CString& bindip="::");
```

### WServer.onConnect
```cpp
void (*onConnect)(WebSocket& ws, http::Req& req);
```
-

### WServer.onMessage
```cpp
void (*onMessage)(WebSocket& ws, const Buffer& data, http::Req& req);
```
-


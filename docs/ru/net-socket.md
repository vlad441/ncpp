[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::BindInfo](#ncppbindinfo)
- [ncpp::IPAddr](#ncppipaddr)
	- [IPAddr.toString()](#ipaddrtostring)
- [ncpp::Socket](#ncppsocket)
	- [Socket.getfd()](#socketgetfd)
	- [Socket.own()](#socketown) 
	- [Socket.connect()](#socketconnect)
	- [Socket.connect4()](#socketconnect4)
	- [Socket.bind()](#socketbind)
	- [Socket.listen()](#socketlisten)
	- [Socket.address()](#socketaddress)
	- [Socket.destroy()](#socketdestroy)
	- [Socket.close()](#socketclose)
	- [Socket.recvTimeout()](#socketrecvtimeout)
	- [Socket.setNonBlocking()](#socketsetnonblocking)
	- [Socket.type()](#sockettype)
- [ncpp::TCPSocket](#ncpptcpsocket)
	- [TCPSocket.send()](#tcpsocketsend)
	- [TCPSocket.recv()](#tcpsocketrecv)
	- [TCPSocket.accept()](#tcpsocketaccept)
- [ncpp::UDPSocket](#ncppudpsocket)
	- [UDPSocket.send()](#udpsocketsend)
	- [UDPSocket.recv()](#udpsocketrecv)
- [ncpp::ICMPSocket](#ncppicmpsocket)
	- [UDPSocket.ping4()](#icmpsocketping4)
- [ncpp::UnixSocket](#ncppunixsocket)

	
## ncpp::BindInfo
```cpp
struct BindInfo { std::string ip; int port; std::string family; std::string type; };
```
Структура, содержащая информацию о привязке сокета.

## ncpp::IPAddr
```cpp
IPAddr(const std::string& ip="", int port=0, char ipver=4)
```
Содержит информацию о IP адресе.

### IPAddr.toString()
```cpp
std::string toString() const;
```
Выводит хранящийся адрес в строковом представлении.

## ncpp::Socket
```cpp
Socket();
Socket(const std::string& ip, int port, Type type=TCP);
Socket(int sockfd);
```
Основная структура сокета, содержит реализации базовых операций с дескрипторами сокетов для их наследования в иных структурах.

### Socket.getfd()
```cpp
int getfd();
```
Вернет дескриптор сокета.

### Socket.own()
```cpp
Socket& own(bool en=true);
```
Устанавливает право "владения" данным дескриптором. В данном случае это означает реализацию концепции "права на уничтожение", то есть только "владеющий" сокет может закрыть его при вызове деструктора этого класса.

### Socket.connect()
```cpp
bool connect(const std::string& ip, int port);
bool connect(const IPAddr& a);
bool connect();
```
Выполяет соединение к указанному хосту. Поддерживает IPv4, IPv6 и разрешение имен.
В случае если параметры не переданы, будут использованы параметры переданные в конструктор этого сокета.
При соединении адрес кэшируется, повторное соединение возможно без передачи параметров.

### Socket.connect4()
```cpp
bool connect4(const std::string& ip, int port);
bool connect4();
```
Также как и `connect()` выполяет соединение к указанному хосту, однако поддерживает только IPv4 стек.

### Socket.bind()
```cpp
bool bind(int port = 0, const std::string& bindip = "::");
```
Выполняет привязку сокета к порту и IP адресу. При передаче `0` для `port` сокет будет привязан динамически к случайному порту.

### Socket.listen()
```cpp
bool listen(int maxqueue=0);
```
Запускает прослушивание для этого сокета. Возможно после выполнения `bind()`. Для `SOCK_DGRAM` типа сокета (например, UDP) не имеет смысла и ничего не делает.

### Socket.address()
```cpp
static BindInfo address(int sockfd1);
inline BindInfo address();
```
Возвращает иформацию о привязке данного сокета.

### Socket.destroy()
```cpp
static bool destroy(int sockfd1, bool force=true);
bool destroy();
```
Уничтожает дескриптор данного сокета, закрывая его принудительно. (на сколько это возможно в данной ОС).

### Socket.close()
```cpp
inline static bool close(int sockfd1);
bool close();
```
Закрывает файловый дескриптор данного сокета.

### Socket.recvTimeout()
```cpp
bool recvTimeout(int msecs);
```
Устанавливает таймаут ожидания ответа `recv()` для этого дескриптора сокета. (на сколько это возможно в данной ОС).

### Socket.setNonBlocking()
```cpp
static void setNonBlocking(int sockfd1, bool nb_mode=true);
inline void setNonBlocking(bool nb_mode=true);
```
Устанавливает неблокирующий режим.

### Socket.type()
```cpp
std::string type();
```
Выводит протокол/тип данного сокета в строчном формате.

## ncpp::TCPSocket
```cpp
TCPSocket();
TCPSocket(const std::string& ip, int port, bool toconn=false);
TCPSocket(const IPAddr& addr, bool toconn=false);
TCPSocket(int sockfd1);
TCPSocket(Socket sock);
```
Наследует от `ncpp::Socket` и представляет из себя специализацию для протокола `TCP`.

### TCPSocket.send()
```cpp
int send(const Buffer& buff);
int send(const char* cstr, int len);
int send(const char* cstr);
```
Выполняет отправку данных. Возвращает кол-во отправленных байт. Если возвращенный размер `<=0`, сокет недоступен для записи или был закрыт.

### TCPSocket.recv()
```cpp
int recv(char* ptr, int len);
int recv(Buffer* buff);
Buffer recv();
```
Ожидает новые данные. Возвращает кол-во полученных байт. Если возвращенный размер `<=0` или размер `Buffer` равен `0`, сокет недоступен для чтения или был закрыт.

### TCPSocket.accept()
```cpp
TCPSocket* accept() const;
TCPSocket _accept() const;
```
Ожидает новое подключение к сокету (возможно только после привязки `bind()` и `listen()`). Если `TCPSocket.getfd()` равен `-1`, произошла ошибка.

## ncpp::UDPSocket
```cpp
UDPSocket();
UDPSocket(const std::string& ip, int port, bool toconn=false);
UDPSocket(const IPAddr& addr, bool toconn=false);
UDPSocket(int sockfd1);
UDPSocket(Socket sock);
```
Наследует от `ncpp::Socket` и представляет из себя специализацию для протокола `UDP`.

### UDPSocket.send()
```cpp
int send(const IPAddr& addr, const char* cstr, int len);
int send(const IPAddr& addr, const Buffer& buffer);
inline int send(const IPAddr& addr, const char* cstr);
inline int send(const Buffer& buff);
inline int send(const char* cstr, int len);
inline int send(const char* cstr);
```
Выполняет отправку данных. Возвращает кол-во отправленных байт. Если возвращенный размер `<=0`, сокет недоступен для записи или был закрыт.

### UDPSocket.recv()
```cpp
int recv(char* ptr, int len, IPAddr* rinfo=NULL);
int recv(Buffer* buff, IPAddr* rinfo=NULL);
Buffer recv();
```
Ожидает новые данные. Возвращает кол-во полученных байт. Если возвращенный размер `<=0` или размер `Buffer` равен `0`, сокет недоступен для чтения или был закрыт.

## ncpp::ICMPSocket
Представление сокета для `ICMP` протокола.

### ICMPSocket::ping4()
```cpp
static double ping4(const std::string& ip);
static double ping4(const IPAddr& addr);
```
Выполняет проверку доступности хоста (пинг), работает только с IPv4 протоколом. Возвращает время ответа в миллисекундах.

## ncpp::UnixSocket
```cpp
UnixSocket(const std::string& sockPath, int type=SOCK_STREAM);
UnixSocket(int type=SOCK_STREAM, int sockid1=-1);
```
Наследует от `ncpp::TCPSocket` и представляет из себя специализацию для протокола `UNIX`.
`sockPath` может в себя принять как путь к файлу сокета (например: `/tmp/unixsock_name.sock`)
или же абстрактное имя (которое не будет привязано к файловой системе, например: `@unixsock_name`)
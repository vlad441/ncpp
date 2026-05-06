[![en](https://img.shields.io/badge/docs-%F0%9F%93%84%20EN-lightgrey)](../en/introduction.md)

Документация **ncpp** v0.0.1-0 (Node C++).

[Структура](#структура)
[Начало работы](#начало-работы)
- [Компилятор](#компилятор)
- [Параметры сборки](#параметры-сборки)
- [Примеры](#примеры)

## Структура

- [Base](module-base.md)
	- [Array](base-Array.md) (≈ `std::vector<T>`)
	- [String](base-String.md) (≈ `std::string`)
	- [Buffer](base-Buffer.md) (≈ `Array<unsigned char>`)
	- [HashMap](base-HashMap.md) (≈ `std::unordered_map<T>`)
- System
	- [utils](sys-utils.md)
	- [fs](sys-fs.md)
	- [info](sys-info.md)
	- [process](sys-process.md)
	- [Date](sys-Date.md)
	- [thread](sys-thread.md)
	- [Console](sys-Console.md)
	- mk-input
- Net
	- [Socket](net-Socket.md)
	- [http](net-http.md)
	- [unixsocket](net-unixsocket.md)
	- [server](net-server.md)
	- [WebSocket](net-WebSocket.md)
- Structs
	- [Object](structs-Object.md)
	- [BigInt](structs-BigInt.md)
- [Crypto](module-crypto.md)
- [GUI](module-gui.md)
- [Experimental](module-experimental.md)
	- (Экспериментальные компоненты призваны исследовать различные возможности, а потому могут быть не стабильны до тех пор, пока не будут должным образом интегрированы в основную ветку)

## Начало работы

### Компилятор

Для Windows вы можете использовать готовые сборки компиляторов [GCC](https://gcc.gnu.org/releases.html)/[Clang](https://releases.llvm.org/), например:
 * [MinGW-w64 project](https://www.mingw-w64.org/downloads)
 * * [niXman/mingw-builds-binaries](https://github.com/niXman/mingw-builds-binaries/releases)
 * * [sourceforge.net mingw-w64/files](https://sourceforge.net/projects/mingw-w64/files)
 * TDM-GCC project
 * * [TDM-GCC 10.3.0 installer](https://sourceforge.net/projects/tdm-gcc/files/v10.3.0-tdm-1/tdm-gcc-10.3.0.exe)
 * * [TDM-GCC 4.9.2 installer](https://sourceforge.net/projects/tdm-gcc/files/TDM-GCC%20Installer/Previous/1.1309.0/tdm64-gcc-4.9.2-3.exe)
 * [Clang releases](https://releases.llvm.org/)
 * * [mstorsjo/llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases)
 * Dev-C++ IDE
 * * [Dev-Cpp 5.11 (TDM-GCC 4.9.2) installer](https://soft.mydiv.net/win/dlfile280771_c13ad-DEV-C.html/Dev-Cpp-5.11-TDM-GCC-4.9.2-Setup.exe)
 * * [Dev-C++ 4.9.9.2 (gcc 3.4.2) installer](https://sourceforge.net/projects/dev-cpp/files/Binaries/Dev-C++%204.9.9.2/) (только для очень старых систем)
 
Для Linux вы можете использовать пакетные менеджеры вашей системы для установки GCC/Clang, например:
* Debian/Ubuntu: `apt install g++ -y --no-install-recommends`.

### Параметры сборки
Скрипты `make.bat` и `make.sh` в папке `scripts` представляют собой идентичную функцию `Unity Build` сборки для систем `Windows`/`Linix` соответственно.

Они имеют свои дополнительные _необязательные_ аргументы при сборке:
```bash
make.sh [Версия C++] [Разрядность] [Режим сборки] [Имя точки входа (без расширения)]
#Примеры
make.sh project441
make.sh С++11 example
make.sh С++98 x64 proj2
make.sh С++17 x32 GUI proj5
```
- **Версии С++**: `C++98`, `C++11`, `C++17`, `C++23`
- **Разрядность**: `x32`, `x64`
- **Режимы сборки**:
    - `LIB`: Режим сборки через статическую библиотеку `ncpp.a`
    - `LIB_SO`: Режим сборки через динамическую библиотеку `ncpp.dll`/`ncpp.so`
	- `LIB_DLL`: Алиас для `LIB_SO`
    - `FAST`: Использовать агрессивные `-O3` оптимизации под текущую нативную архитектуру.
    - `SAN`: Включить санитайзеры `ASan` и `UBSan` в бинарный файл.
    - `DBG`: Использовать флаги `-0g` для режима отладки.
	- `MUSL`: (Только Linux) Использовать линковку с `musl` для проекта (может требоваться пакет для `musl-gcc`)
    - `GUI`: Включить модули для использования системной графики (`GDI`/`X11`) с динамической линковкой.
    - `SGUI`: Выполнить `GUI` с статической линковкой.
    - `GL`: Включить возможность использовать расширеную графику (OpenGL/Vulkan)
    - `EXP`: Включить экспериментальные компоненты
    - `DEF`: Режим по умолчанию. Указывать явно не требуется (обычно опускается).
- **Имя точки входа**: Название вашего файла исходного когда, исключая `.cpp` расширение (`proj1.cpp` -> `proj1`). По умолчанию используется `example`

Если у вас кастомный пусть к вашему компилятору или если его нет в переменных средах вашей системы, вы можете вручную указать полный путь к компилятору в параметре `gccpath` в начале скрипта.

### Примеры

**HTTP сервер:**
```cpp
#include "src/ncpp.cpp"
using namespace ncpp;
int port = 3000; HTTPServer srv(port);

void onReq(http::Req& req, http::Res& res){ print("[Request]: "); print(req.method); print(" "); print(req.url); print("\n");
	res.end("The example HTTP response\n<br>by ncpp Project"); }

void onConnect(TCPSocket& socket){ print("[] New connection: "); print(socket.destAddr.toString()); print("\n"); }
void onClose(TCPSocket& socket){ print("[] Closed connection: "); print(socket.destAddr.toString()); print("\n"); }

int main(int argc, char* argv[]){ srv.onRequest=onReq; srv.onConnect=onConnect; srv.onClose=onClose;
	print("HTTP server runned on port: "); print(port); print("\n"); srv.run(); }
```
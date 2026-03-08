[![en](https://img.shields.io/badge/docs-%F0%9F%93%84%20EN-lightgrey)](../en/introduction.md)

[Введение](#введение)
[Начало работы](#начало-работы)
[Примеры](#примеры)

## Введение

Документация по **ncpp** v0.0.0-0 (Node C++) — введение. Модули:

- [ncpp-base](ncpp-base.md)
- [ncpp-system](ncpp-system.md)
	- [system-fs](system-fs.md)
	- [system-thread](system-thread.md)
- net
	- [net-socket](net-socket.md)
	- [net-http](net-socket.md)
	- [net-server](net-server.md)
- [ncpp-crypto](ncpp-crypto.md)
- [ncpp-object](ncpp-object.md)
- [ncpp-bigint](ncpp-bigint.md)
- [ncpp-gui](ncpp-gui.md)

## Начало работы
### Windows
Для Windows вы можете использовать готовые сборки компиляторов GCC/Clang, например:
 * [MinGW-w64 project](https://www.mingw-w64.org/downloads)
 * * [niXman/mingw-builds-binaries](https://github.com/niXman/mingw-builds-binaries/releases)
 * * [sourceforge.net mingw-w64/files](https://sourceforge.net/projects/mingw-w64/files)
 * TDM-GCC project
 * * [TDM-GCC 10.3.0 installer](https://sourceforge.net/projects/tdm-gcc/files/v10.3.0-tdm-1/tdm-gcc-10.3.0.exe)
 * * [TDM-GCC 4.9.2 installer](https://sourceforge.net/projects/tdm-gcc/files/TDM-GCC%20Installer/Previous/1.1309.0/tdm64-gcc-4.9.2-3.exe)
 * [Clang releases](https://releases.llvm.org/)
 * * [mstorsjo/llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases)
 * Dev-C++ IDE
 * * [Dev-Cpp 5.11 (TDM-GCC-4.9.2) installer](https://soft.mydiv.net/win/dlfile280771_c13ad-DEV-C.html/Dev-Cpp-5.11-TDM-GCC-4.9.2-Setup.exe)
 * * [Dev-C++ 4.9.9.2 (gcc 3.4.2) installer](https://sourceforge.net/projects/dev-cpp/files/Binaries/Dev-C++%204.9.9.2/) (только для очень старых систем)
 
### Linux
Для Linux вы можете использовать пакетные менеджеры вашей системы для установки GCC/Clang, например:
* Debian: `apt install g++ --no-install-recommends`.

## Примеры
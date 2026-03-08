![version](https://img.shields.io/badge/version-v0.0.1-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/11) [![ru](https://img.shields.io/badge/%D1%8F%D0%B7%D1%8B%D0%BA-%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%BA%F0%9F%87%B8-white)](README-EN.md) [![docs-ru](https://img.shields.io/badge/docs-ru-white)](docs/ru/introduction.md) [![docs-en](https://img.shields.io/badge/docs-en-white)](docs/en/introduction.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://github.com/vlad441/ncpp/blob/main/UNLICENSE)

**ncpp** (Node C++) is a minimalistic system library that aims to provide a simple and efficient cross-platform multitool, inspired by Node.js documentation and its asynchronous nature.

ncpp offers implementations across various domains for C++98 standards and higher, such as buffer handling, files, networking, encryption, and more, while minimizing external dependencies and maintaining a minimal footprint.

[Features](#features)
[Build](#build)
- [Direct Source Inclusion](#direct-source-inclusion)
- [Building the Library from Source](#building-the-library-from-source)

[Usage](#usage)
- [Linking a Pre-built Library](#linking-a-pre-built-library)
- [Package Managers](#package-managers)
  - [Installing via apt](#installing-via-apt)

You can find more detailed information in the documentation [here](docs/en/introduction.md).

## Features
- **Core:**
  - Standard containers: Array, String, Buffer, HashMap, etc.
  - Standard conversions: numbers <-> strings, etc.
- **System:**
  - Standard I/O (readline/print).
  - File system operations.
  - System information gathering.
  - Process and thread management.
  - Console: console management structure.
  - Networking: Sockets (TCP/UDP/Unix), HTTP client, TCP/HTTP server, WebSockets.
  - Date and system time management.
- **Structures and Algorithms:**
  - CBOR/JSON/BSON support.
  - BigInt: Arbitrary-precision integers.
- **Cryptography:**
  - Hashing (MD5, SHA1/2/3).
  - Symmetric encryption (AES).
- **Graphics:**
  - WinApi/X11 window initialization.
  - OpenGL context initialization.
  - Event handling.

## Build
- **Compiler Support**:
- - [x] GCC/MinGW >= 3.4.2 (recommended >= 4.9.2)
- - [x] Clang >= 3.5.2
- - [ ] ~~MSVC~~ (support not planned)
- **Standard Support**: C++98, C++11 and higher;
- **OS Support**:
- - [x] Windows XP/7/10+
- - [x] Linux >= 2.6.18 (recommended >= 3.10.0)
- - - [x] X11
- - - [ ] Wayland
- - [ ] FreeBSD (not planned in the near future)
- - [ ] Android (not planned in the near future)
- - [ ] ~~MacOS, iOS~~ (support not planned)

### Direct Source Inclusion
This is the simplest method, especially if you want to manually choose the optimization level when compiling each project for a specific platform.
To simplify and speed up the build process, a `Unity Build` style is used, where a single translation unit (master file) is assembled from `.cpp` files in the required sequence. This can help with optimizations, though it may consume more memory compared to a classic modular build.

You can use the `gcc_make.sh`/`gcc_make.bat` scripts in the `scripts` directory for this purpose. (Must be executed from within the `scripts` directory).

Alternatively, you can simply include the master file `#include "src/ncpp.cpp"` in your code. This will include the full library implementation into your file.
(Note: in this case, you may need to manually set compiler flags and link system libraries).

### Building the Library from Source
Currently, building dynamic libraries `.so` (Linux) and `.dll` (Windows) is possible, though header files `.h` are not yet provided separately.
Building static libraries `.a` (Linux) and `.a`/`.lib` (Windows) is also possible, but binary variants may not be fully optimized.

You can build them using the `gcc_build.sh`/`gcc_build.bat` scripts in `scripts`, where the static library will be compiled into a **single** object file.
The compiled libraries will be placed in the `lib` directory.

## Usage

### Linking a Pre-built Library
The libraries themselves are compiled for the most common use cases. However, in environments where these pre-built binaries cannot be used, you can use [Direct Source Inclusion](#direct-source-inclusion) or [Building the Library from Source](#building-the-library-from-source).
At this stage, using a pre-built library may not significantly increase build speed, as part of the codebase uses templates that cannot be pre-compiled.

### Package Managers
For Linux systems, you can connect to package manager repositories to install the library onto your system.

#### Installing via apt
Currently, `.deb` packages are not provided.
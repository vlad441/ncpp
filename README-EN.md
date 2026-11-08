![version](https://img.shields.io/badge/version-v0.0.1-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://ru.cppreference.com/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://ru.cppreference.com/cpp/11) [![ru](https://img.shields.io/badge/%D1%8F%D0%B7%D1%8B%D0%BA-%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%BA%F0%9F%87%B8-white)](README-EN.md) [![docs-ru](https://img.shields.io/badge/docs-%F0%9F%93%84%20RU-lightgrey)](docs/ru/introduction.md) [![docs-en](https://img.shields.io/badge/docs-%F0%9F%93%84%20EN-lightgrey)](docs/ru/introduction.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://github.com/vlad441/ncpp/blob/main/UNLICENSE) 
[![Windows](https://img.shields.io/badge/Windows-✅%20Supported-0078D6?logo=windows)](https://www.microsoft.com/windows) [![Linux](https://img.shields.io/badge/Linux-✅%20Supported-0078D6?logo=linux)](https://kernel.org) ![CI](https://github.com/vlad441/ncpp/actions/workflows/main.yml/badge.svg) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/vlad441/ncpp) ![GitHub repo size](https://img.shields.io/github/repo-size/vlad441/ncpp) ![GitHub all releases](https://img.shields.io/github/downloads/vlad441/ncpp/total) [![Coverage Status](https://coveralls.io/repos/github/vlad441/ncpp/badge.svg?branch=dev)](https://coveralls.io/github/vlad441/ncpp?branch=dev)

**ncpp** (Node C++) is a minimalist system library that aims to provide a simple and efficient cross-platform multi-tool inspired by the Node.js documentation.

ncpp offers implementations from various domains for C++98 and higher standards, such as buffer processing, file handling, networking, cryptography, and much more, while minimizing external dependencies and maintaining a minimal footprint.

[Quick Start](#quick-start)
[Features](#features)

[Building](#building)
- [Dependencies](#dependencies)
- [Direct Source Inclusion](#direct-source-inclusion)
- [Building the Library from Source](#building-the-library-from-source)

[Usage](#usage)
- [Examples](#examples)
- [Linking the Built Library](#linking-the-pre-built-library)
- [Package Managers](#package-managers)
  - [Installing via apt](#installing-via-apt)

[Documentation](#documentation)

## Quick Start
 - If you don't have a compiler installed, you can choose one from this [list](docs/ru/introduction.md#compiler).
 - The [example.cpp](example.cpp) file in the project root already contains an example, but you can modify it to your liking or choose from the [examples](docs/ru/introduction.md#examples).
	- Build your first example from source for your OS: [Windows](#windows) or [Linux](#linux).
		- (Alternatively, you can [download](https://github.com/vlad441/ncpp/archive/refs/heads/dev.zip) this repository and run the `make.bat`/`make.sh` script with a double click in the project root).

If you are a beginner, we recommend checking out our [introduction](docs/ru/introduction.md#getting-started) to get started with the language and the library.

## Features
- **Core:**
  - Standard containers: Array, String, Buffer, HashMap.
  - Standard conversions: numbers <-> strings, etc.
- **System:**
  - Standard I/O (readline/print).
  - File system operations.
  - System information gathering.
  - Process and thread management.
  - Console: console management.
  - Network: Sockets (TCP/UDP/Unix), HTTP client, TCP/HTTP server, WebSockets.
  - Date and system time operations.
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

## Building
- **Compiler Support**:
  - [x] GCC/MinGW >= 3.4.2 (recommended >= 4.9.2)
  - [x] Clang >= 3.5.2
  - [ ] ~~MSVC~~ (not planned)
- **Standard Support**: >= C++98
- **OS Support**:
  - [x] Windows XP/7/10+
  - [x] Linux >= 2.6.18 (recommended >= 3.10.0)
    - [x] X11
    - [ ] Wayland
  - [ ] FreeBSD (not planned in the near future)
  - [ ] Android (not planned in the near future)
  - [ ] ~~MacOS, iOS~~ (not planned)
  
### Dependencies
The project strives to minimize external dependencies, but individual components may require additional packages:

- **Windows**: No dependencies required. (All necessary components are already included in the standard OS package).

- **Linux**: Requires `glibc`/`musl` (usually included in the minimal setup of any distribution).
Additionally, `GUI`, `GL`, and `Experimental` components may require the following packages:
```bash
# GUI -> X11 packages: 
apt install libx11-dev libxtst-dev -y --no-install-recommends
# GL -> OpenGL packages: 
apt install libgl1-mesa-dev -y --no-install-recommends
# Experimental -> Audio:
apt install libasound2-dev -y --no-install-recommends
# Experimental -> Images: 
# apt install libpng16-16 libjpeg62-turbo -y --no-install-recommends
# OR: apt install libpng-dev libjpeg-dev -y --no-install-recommends
# OR: Requires enabling the stb_image implementation (not included in the project code by default)
```

### Direct Source Code Inclusion
This is the primary (and currently main) method, especially if you want to manually configure optimization levels when compiling your project for a specific target platform.

To simplify and speed up compilation, a `Unity Build` style approach is used: a single translation unit (master file) is compiled from `.cpp` files in the required order. This can aid compiler optimization, though it may consume more RAM during compilation compared to traditional separate module builds.

You can use the `make.sh`/`make.bat` scripts for this purpose (must be executed from the project root).  
By default, the script compiles `example.cpp` in the root directory.

You can also simply include the master file `#include "src/ncpp.cpp"` in your own code. This includes the full implementation of the library into your compilation unit.  
*(Note: In this case, you may need to set compiler flags manually and link system libraries yourself).*

#### Windows
```batch
git clone --single-branch --branch dev --depth 1 https://github.com/vlad441/ncpp.git
:: Or download zip archive: https://github.com/vlad441/ncpp/archive/refs/heads/dev.zip
:: Since Git converts line endings to LF, downloading via archive might require converting line endings to CRLF for make.bat

cd ncpp
make.bat
```

#### Linux
```bash
git clone --single-branch --branch dev --depth 1 https://github.com/vlad441/ncpp.git
# Or download zip archive: wget https://github.com/vlad441/ncpp/archive/refs/heads/dev.zip

cd ncpp
chmod 755 make.sh
bash make.sh
```

### Building the Library from Source
Full support for building separate library binaries is **currently not provided**.

Building dynamic shared libraries (`.so` on Linux, `.dll` on Windows) is not supported yet, and standalone `.h` headers are not currently provided.  
Static libraries (`.a` on Linux, `.a`/`.lib` on Windows) are also not directly supported at this moment.

However, you can build them using the `build.sh`/`build.bat` scripts located in `scripts`, where the static library will be built into a **single** object file.  
The compiled outputs will be placed in the `lib` directory.

## Usage

### Examples
You can inspect the basic [example.cpp](example.cpp) in the root directory.

Additionally, check out a few examples in the documentation [section](docs/ru/introduction.md#examples).

A more complex example using `Experimental` components—including `GUI` and `OpenGL`—is currently available only on the `dev` branch under `tests/gui`. You can build it by running `make_gui.bat gl-perspective` or `bash make_gui.sh gl-perspective`.

### Linking a Pre-built Library
Pre-built libraries are compiled for common usage scenarios. However, in environments where these pre-built libraries cannot be used, rely on [Direct Source Code Inclusion](#direct-source-code-inclusion) or [Building the Library from Source](#building-the-library-from-source).  
Currently, using pre-built libraries may not significantly reduce build times because parts of the codebase rely on templates, which cannot be pre-compiled.

### Package Managers
For Linux systems, you can add repositories to package managers to install the library system-wide.

#### Installing via apt
Currently, `.deb` packages are not provided.

## Documentation
For detailed information, check out the documentation [here](docs/ru/introduction.md).
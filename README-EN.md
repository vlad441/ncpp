![version](https://img.shields.io/badge/version-v0.0.1-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/11) [![ru](https://img.shields.io/badge/%D1%8F%D0%B7%D1%8B%D0%BA-%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%BA%F0%9F%87%B8-white)](README-EN.md) [![docs-ru](https://img.shields.io/badge/docs-%F0%9F%93%84%20RU-lightgrey)](docs/ru/introduction.md) [![docs-en](https://img.shields.io/badge/docs-%F0%9F%93%84%20EN-lightgrey)](docs/en/introduction.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://github.com/vlad441/ncpp/blob/main/UNLICENSE) [![Windows](https://img.shields.io/badge/Windows-✅%20Supported-0078D6?logo=windows)](https://www.microsoft.com/windows) [![Linux](https://img.shields.io/badge/Linux-✅%20Supported-0078D6?logo=linux)](https://kernel.org)

**ncpp** (Node C++) — a minimalistic system library that aims to provide a simple and efficient cross-platform multitool inspired by Node.js documentation.

ncpp offers implementations across various domains for C++98 standards and above, such as buffer handling, files, networking, encryption, and more, while minimizing external dependencies and maintaining a minimal footprint.

[Features](#features)

[Build](#build)
- [Dependencies](#dependencies)
- [Direct Source Inclusion](#direct-source-inclusion)

[Usage](#usage)
- [Examples](#examples)

[Documentation](#documentation)

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
  - Date and system time utilities.
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
The project strives to minimize external dependencies, though some components may require additional packages:
- **Windows**: - (All necessary components are already included in the standard system setup)
- **Linux**: Requires `glibc`/`musl` (Usually included in the minimal setup of any distribution)
For `GUI`, `GL`, and `Experimental` components, the following packages may be required:
```bash
# GUI -> X11 packages: 
apt install libx11-dev libxtst-dev -y --no-install-recommends
# GL -> OpenGL packages: 
apt install libgl1-mesa-dev -y --no-install-recommends
# Experimental -> Audio:
#apt install libasound2-dev -y --no-install-recommends
# Experimental -> Images: Requires stb_image implementation (Not included in the project code)
```

### Direct Source Inclusion
This is the primary (and currently only) method, especially if you want to manually choose the optimization level when compiling each project for a specific platform.

To simplify and speed up the build, a `Unity Build` style is used, where a single translation unit (master file) is compiled from `.cpp` files in the required sequence. This helps with optimizations but may consume more memory compared to classical modular builds.

You can use the `make.sh`/`make.bat` scripts in the `scripts` directory for this purpose. (Must be run from the `scripts` directory).
By default, the script will build the `example.cpp` located in the project root.

Additionally, you can simply include the master file `#include "src/ncpp.cpp"` in your code. This will include the full library implementation into your file.
(Note: in this case, you may need to manually set compiler flags and link system libraries).

#### Windows
```batch
git clone --single-branch --branch main --depth 1 https://github.com/vlad441/ncpp.git
::Or: you can download the archive: https://github.com/vlad441/ncpp/archive/refs/heads/main.zip
::Since Git automatically converts line endings to LF, you may need to change the line ending format to CRLF for make.bat when downloading the archive

cd ncpp/scripts
make.bat
```

#### Linux
```bash
git clone --single-branch --branch main --depth 1 https://github.com/vlad441/ncpp.git
#Or: you can download the archive: https://github.com/vlad441/ncpp/archive/refs/heads/main.zip

cd ncpp/scripts
chmod 755 make.sh
bash make.sh
```

## Usage

### Examples
You can check out the simple [example.cpp](example.cpp) in the root of the project.

You can also find a few examples in the [examples section](docs/ru/introduction.md#examples) of the documentation.

## Documentation
For more detailed information, you can find the documentation [here](docs/en/introduction.md).
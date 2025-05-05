![version](https://img.shields.io/badge/version-v0.0.0-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/11) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%AC%F0%9F%87%A7-white)](README-EN.md) [![ru](https://img.shields.io/badge/lang-Russian%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://raw.githubusercontent.com/rigtorp/HashMap/master/LICENSE) ![coverage](https://img.shields.io/badge/coverage-0%25-yellowgreen)

**ncpp** (Node C++) is a minimalist system library that aims to provide a simple and efficient cross-platform toolkit, inspired by the documentation and asynchronous nature of Node.js.

ncpp offers implementations in various areas for C++98 and higher standards, such as buffer handling, files, networking, cryptography, and more, minimizing external dependencies and maintaining a minimal size.

[Build](#build)
- [Direct Source Code Inclusion (Header-Only Style)](#direct-source-code-inclusion-header-only-style)
- [Building the Library from Source](#building-the-library-from-source)

[Usage](#usage)
- [Linking the Binary Library](#linking-the-binary-library)
- [Package Managers](#package-managers)
  - [Installing via apt](#installing-via-apt)

## Build
Requirements:
- **Compiler Support**: `gcc` >= 3.4.2 (recommended >= 4.9.2); `clang` >= 3.5.2;
- **Standard Support**: C++98, C++11 and higher;
- **OS Support**: `Linux` >= 2.6.18 (recommended >= 3.10.0); Windows XP+;
### Direct Source Code Inclusion (Header-Only Style)
This is the simplest method, especially if you want to choose the optimization level yourself when compiling each of your projects for a specific platform.
However, this might increase compilation time.

You can use the `gcc_make.sh`/`gcc_make.bat` script in the `scripts` directory for this purpose.

Alternatively, you can simply include `#include "src/ncpp.cpp"` in your code. This will include the full library implementation into your file.
(However, in this case, you might need to manually set compiler flags and link system libraries).
### Building the Library from Source
Currently, building `.a` and `.so` (Linux), as well as `.lib` and `.dll` (Windows) libraries is not supported, and header files (`.h`) are not yet provided.
You can build them using the `gcc_build.sh`/`gcc_build.bat` script in `scripts`, however, the resulting static libraries will not be optimized.
The built libraries will be placed in the `lib` directory.

## Usage

### Linking the Binary Library
The pre-compiled libraries themselves are compiled using the C++11 standard, which should provide better performance compared to C++98. However, in environments where using these library builds is not possible, you can use [Direct Source Code Inclusion](#direct-source-code-inclusion-header-only-style) or [Building the Library from Source](#building-the-library-from-source).
### Package Managers
For Linux systems, you can add repositories for package managers to install the library into your system.
#### Installing via apt
Currently, `.deb` packages are not provided.
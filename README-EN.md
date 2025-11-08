![version](https://img.shields.io/badge/version-v0.0.1-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/11) [![ru](https://img.shields.io/badge/%D1%8F%D0%B7%D1%8B%D0%BA-%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%BA%F0%9F%87%B8-white)](README-EN.md) [![docs-ru](https://img.shields.io/badge/docs-ru-white)](docs/ru/introduction.md) [![docs-en](https://img.shields.io/badge/docs-en-white)](docs/en/introduction.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://github.com/vlad441/ncpp/blob/main/UNLICENSE)

**ncpp** (Node C++) is a minimalistic system library that aims to provide a simple and efficient cross-platform multi-tool, inspired by the documentation and asynchronicity of Node.js.

ncpp offers implementations for various domains for C++98 and later standards, such as buffer handling, files, networking, encryption, and much more, **minimizing external dependencies** and maintaining a minimal size.

[Building](#building)
- [Direct Source Inclusion (Source-Included / Header-Only Style)](#direct-source-inclusion-source-included--header-only-style)
- [Building the Library from Source](#building-the-library-from-source)

[Usage](#usage)
- [Linking a Pre-Built Library](#linking-a-pre-built-library)
- [Package Managers](#package-managers)
  - [Installing the package via apt](#installing-the-package-via-apt)

You can find more detailed documentation [here](docs/ru/introduction.md).

---

## Building
Requirements:
- **Compiler Support**: `gcc` >= 3.4.2 (recommended >= 4.9.2); `clang` >= 3.5.2;
- **Standard Support**: C++98, C++11 and later;
- **OS Support**: `Linux` >= 2.6.18 (recommended >= 3.10.0); Windows XP+;

### Direct Source Inclusion (Source-Included / Header-Only Style)
This is the simplest method, especially if you want to manually select the optimization level when compiling each of your projects for a specific platform.
However, this may increase compilation time.

You can use the `gcc_make.sh`/`gcc_make.bat` script in `scripts` for these purposes. (Must be run from the `scripts` directory).

Alternatively, you can simply include `#include "src/ncpp.cpp"` in your code. This will include the **full implementation** of the library into your file.
(However, in this case, you might need to manually set compiler flags and link system libraries).

### Building the Library from Source
Currently, only building **dynamic libraries** (`.so` on Linux and `.dll` on Windows) is supported; header files (`.h`) are not provided yet.
Building static libraries (`.a` on Linux and `.a`/`.lib` on Windows) is currently not optimized and is under development.
You can build them using the `gcc_build.sh`/`gcc_build.bat` script in `scripts`, where static libraries will be compiled into a **single object file**.
The built libraries will be placed in the `lib` directory.

---

## Usage

### Linking a Pre-Built Library
The libraries themselves are compiled for the most common use cases, but in environments where using these pre-built libraries is not possible, you can use [Direct Source Inclusion](#direct-source-inclusion-source-included--header-only-style) or [Building the Library from Source](#building-the-library-from-source).

### Package Managers
For Linux systems, you can connect repositories for package managers to install the library into your system.

#### Installing the package via apt
Currently, `.deb` packages are not provided.
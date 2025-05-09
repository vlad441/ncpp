![version](https://img.shields.io/badge/version-v0.0.0-blue) [![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/98) [![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=c%2B%2B)](https://ru.cppreference.com/w/cpp/11) [![en](https://img.shields.io/badge/lang-English%20%F0%9F%87%AC%F0%9F%87%A7-white)](README-EN.md) [![ru](https://img.shields.io/badge/%D1%8F%D0%B7%D1%8B%D0%BA-%D0%A0%D1%83%D1%81%D1%81%D0%BA%D0%B8%D0%B9%20%F0%9F%87%B7%F0%9F%87%BA-white)](README.md) [![Github link](https://img.shields.io/badge/github-gray)](https://github.com/vlad441/ncpp) [![License](https://img.shields.io/badge/license-PD-blue.svg)](https://raw.githubusercontent.com/rigtorp/HashMap/master/LICENSE) ![coverage](https://img.shields.io/badge/coverage-0%25-yellowgreen)

**ncpp** (Node C++) — минималистичная системная библиотека, которая стремится предоставить простой и эффективный кроссплатформенный инструментарий, вдохновленный документацией и асинхронностью Node.js.

ncpp предлагает реализации из различных областей для стандартов C++98 и выше, такие как обработка буферов, файлов, сети, шифрования и многое другое, минимизируя внешние зависимости и сохраняя минимальный размер.

[Сборка](#сборка)
- [Прямое включение исходного кода](#прямое-включение-исходного-кода-header-only-style)
- [Сборка библиотеки из исходников](#сборка-библиотеки-из-исходников)

[Использование](#использование)
- [Линковка бинарной библиотеки](#линковка-бинарной-библиотеки)
- [Пакетные менеджеры](#пакетные-менеджеры)
  - [Установка пакета через apt](#установка-пакета-через-apt)

Более подробно ознакомиться с документацией вы можете [здесь](docs/ru/introduction.md).

## Сборка
Требования:
- **Поддержка компиляторов**: `gcc` >= 3.4.2 (рекомм. >= 4.9.2); `clang` >= 3.5.2;
- **Поддержка стандарта**: C++98, C++11 и выше;
- **Поддержка ОС**: `Linux` >= 2.6.18 (рекомм. >= 3.10.0); Windows XP+;
### Прямое включение исходного кода (header-only style)
Это самый простой способ, особенно если вы хотите самостоятельно выбрать уровень оптимизации при компиляции каждого вашего проекта под конкретную платформу. 
Однако это может увеличить время компиляции.

Вы можете использовать скрипт `gcc_make.sh`/`gcc_make.bat` в `scripts` для этих целей.

Также, можете просто подключить `#include "src/ncpp.cpp"` в ваш код. Это подключит в ваш файл полную реализацию библиотеки.
(Однако, в этом случае может понадобиться вручную выставлять флаги компилятора и линковать системные библиотеки).
### Сборка библиотеки из исходников
На данный момент сборка `.a` и `.so` (Linux), а также `.lib` и `.dll` (Windows) не поддерживается, заголовочные файлы `.h` пока не предоставляются.
Вы можете их собрать через скрипт `gcc_build.sh`/`gcc_build.bat` в `scripts`, однако статические библиотеки не будут оптимизироваными.
Собраные библиотеки будут в директории `lib`.

## Использование

### Линковка бинарной библиотеки
Сами библиотеки скомпилированы с использованием стандарта C++11, что должно обеспечить лучшую производительность по сравнению с C++98, однако в окружениях где использование этих сборок библиотек невозможно, вы можете использовать [Прямое включение исходного кода](#прямое-включение-исходного-кода-header-only-style) или [Сборка библиотеки из исходников](#сборка-библиотеки-из-исходников).
### Пакетные менеджеры
Для Linux систем вы можете подключить репозитории для пакетных менеджеров чтобы установить библиотеку в вашу систему.
#### Установка пакета через apt
На данный момент `.deb` пакеты не предоставляются.
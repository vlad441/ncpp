[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

Экземпляр приложения: [ncpp::GUI::App](#ncppguiapp)

Окна: [ncpp::GUI::Window](#ncppguiwindow), [ncpp::GUI::GLWindow](#ncppguiglwindow)

Элементы управления: [ncpp::GUI::Button](#ncppguibutton), [ncpp::GUI::Label](#ncppguilabel), [ncpp::GUI::Input](#ncppguiinput), [ncpp::GUI::TextArea](#ncppguitextarea), [ncpp::GUI::CheckBox](#ncppguicheckbox), 
	
Элементы управления наследуют от `Window` и имеют те же конструкторы и методы.

- [ncpp::GUI::WEvent](#ncppguiwevent)
- [ncpp::GUI::App](#ncppguiapp)
	- [App.run()](#apprun)
	- [App.waitOneEvent()](#appwaitoneevent)
    - [App.hasEvents()](#apphasevents)
    - [App.nextEvent()](#appnextevent)
- [ncpp::GUI::Window](#ncppguiwindow)
	- [Window.setTitle()](#windowsettitle)
	- [Window.getText()](#windowgettext)
	- [Window.setText()](#windowsettext)
	- [Window.setHandler()](#windowsethandler)
	- [Window.getSize()](#windowgetsize)
	- [Window.resize()](#windowresize)
	- [Window.setpos()](#windowsetpos)
	- [Window.Show()](#windowshow)
	- [Window.Hide()](#windowhide)
	- [Window.setVisible()](#window)
	- [Window.setDisabled()](#window)
	- [Window.getClass()](#windowgetclass)
	- [Window.Update()](#windowupdate)
	- [Window.destroy()](#windowdestroy)
	- [Window.setFullscreen()](#windowsetfullscreen)
	- [Window.putImage()](#windowputimage)
- [ncpp::GUI::GLWindow](#ncppguiwindow)
	- [GLWindow.createGLContext()](#glwindowcreateglcontext)
	- [GLWindow.destroyGLContext()](#glwindowdestroyglcontext)
	- [GLWindow.setContext()](#glwindowsetcontext)
	- [GLWindow.hasContext()](#glwindowhascontext)
	- [GLWindow.resetContext()](#glwindowresetcontext)
	- [GLWindow.swapBuffers()](#glwindowswapbuffers)
	
Определения:
```cpp
typedef void (*HANDLE_FUNC)(const WEvent&);
```

## ncpp::GUI::WEvent
`WEvent` - структура для хранения информации о событии.

```cpp
struct WEvent { Window* wnd; int id; int key, x, y; String name; };
```
Параметры:
`id`: хранит `EvType` события.
`key`: параметр события (Например, код клавиши или номер клавиши мыши `MouseBtn`).
`x` и `y`: доп. параметры события (Например, координаты мыши).

Определения `enum` для `WEvent`:
```cpp
enum EvType { OTHER, CLICK, MOUSEUP, MOUSEMOVE, KEYDOWN, KEYUP };
enum MouseBtn { LBTN, RBTN, MBTN };
};
```

## ncpp::GUI::App
Структура, которая представляет из себя экземпляр приложения, в котором происходит цикл обработки событий приложения.

### App.run()
```cpp
void run();
```
Блокирует поток, запуская цикл обработки событий.

### App.waitOneEvent()
```cpp
void waitOneEvent();
```
Ждет пока придет хотя бы одно событие в очередь.

### App.hasEvents()
```cpp
bool hasEvents();
```
Проверяет, есть ли события в очереди.

### App.nextEvent()
```cpp
bool nextEvent();
```
Обрабатывает одно событие из очереди. Возвращает `false` если событий в очереди нет.

## ncpp::GUI::Window
```cpp
Window(App* app1=NULL, WndID wndID1=NULL) : app(app1), wndID(wndID1);
Window(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT);
Window(Window* window, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT);
Window(const char* name, int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT);
```
При создании структуры `Window` немедленно создается соответствующий графический элемент на экране.
Если вы передадите указатель на `App*`, будет создано независимое окно (без родителя), и позиционирование будет абсолютным.
Если вы передадите указатель на `Window*`, это окно станет родительским для нового окна, и позиционирование будет относительно внутри этого окна.
Если вы передадите только `const char*`, будет использована скрытая глобальная ссылка на последний созданный экземпляр приложения `App`.
Если вы ничего не передадите, окно не будет инициализировано.

### Window.setTitle()
```cpp
void setTitle(const String& title);
```
Устанавливает название окна.

### Window.getText()
```cpp
String getText();
```
Достает текущий текст окна/элемента.

### Window.setText()
```cpp
void setText(const String& text);
```
Установить текущий текст окна/элемента.

### Window.setHandler()
```cpp
void setHandler(HANDLE_FUNC handler, const CString& type="");
```
Установить обработчик события элемента на функцию типа `HANDLE_FUNC` (aka `void (*)(const WEvent&)`). По умолчанию обработчик регистрируется на все события.

### Window.getSize()
```cpp
void getSize(int& width, int& height);
```
Записывает в переменные текущий резмер окна.

### Window.resize()
```cpp
void resize(int width, int height);
```
Изменяет резмер окна.

### Window.setPos()
```cpp
void setPos(int x, int y);
```
Устанавливает позицию окна.

### Window.setPos()
```cpp
void setPos(int x, int y);
```
Устанавливает позицию окна.

### Window.Show()
```cpp
void Show(); 
```
Показывает окно.

### Window.Hide()
```cpp
void Hide();
```
Скрывает окно.

### Window.setVisible()
```cpp
void setVisible(bool visible=true);
```
Установить видимость окна.

### Window.setDisabled()
```cpp
void setDisabled(bool disabled=true);
```
Деактивация окна/элемента.

### Window.getClass()
```cpp
String getClass();
```
Вернуть текущее название класса акна.

### Window.Update()
```cpp
void Update();
```
Выполнить досрочное обновление состояния окна.

### Window.destroy()
```cpp
bool destroy();
```
Уничтожение окна.

### Window.setFullscreen()
```cpp
void setFullscreen(bool fullscreen);
```
Установить полноэкранный режим.

### Window.putImage()
```cpp
void putImage(const void* px, int w, int h);
//void putImage(const Image& img);
```
Скопировать фреймбуфер картинки в текущее окно. Поддержка масштабирования не гарантируется.

## ncpp::GUI::GLWindow
Наследует от `Window`. Окно, в котором может быть инициализирован контекст графического API видеокарты (в данном случае, OpenGL).

### GLWindow.createGLContext()
```cpp
void createGLContext(bool useDoubleBuff=true);
```
Создает графический контекст.

### GLWindow.destroyGLContext()
```cpp
void destroyGLContext();
```
Уничтожает графический контекст.

### GLWindow.setContext()
```cpp
void setContext();
```
Переключает контекст на текущее окно.

### GLWindow.hasContext()
```cpp
bool hasContext();
```
Проверяет создан ли контекст у окна.

### GLWindow.resetContext()
```cpp
void resetContext();
```
Сбросить контекст.

### GLWindow.swapBuffers()
```cpp
void swapBuffers();
```
Обмен буферами (при двойной буферизации).


















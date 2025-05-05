[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

Экземпляр приложения: [ncpp::GUI::App](#ncppguiapp)

Окна: [ncpp::GUI::Window](#ncppguiwindow), [ncpp::GUI::GLWindow](#ncppguiglwindow)

Элементы управления: [ncpp::GUI::Button](#ncppguibutton), [ncpp::GUI::Label](#ncppguilabel), [ncpp::GUI::Input](#ncppguiinput), [ncpp::GUI::TextArea](#ncppguitextarea), [ncpp::GUI::CheckBox](#ncppguicheckbox), 
	
Элементы управления наследуют от `Window` и имеют те же конструкторы и методы.

- [ncpp::GUI::App](#ncppguiapp)
	- [App.run()](#apprun)
- [ncpp::GUI::Window](#ncppguiwindow)
	- [Window.setTitle()](#windowsettitle)
	- [Window.getText()](#windowgettext)
	- [Window.setText()](#windowsettext)
	- [Window.onEvent()](#windowonevent)
	- [Window.onClick()](#windowonclick)
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
- [ncpp::GUI::GLWindow](#ncppguiwindow)
	- [GLWindow.createGLContext()](#glwindowcreateglcontext)
	- [GLWindow.destroyGLContext()](#glwindowdestroyglcontext)
	- [GLWindow.setContext()](#glwindowsetcontext)
	- [GLWindow.resetContext()](#glwindowresetcontext)
	- [GLWindow.swapBuffers()](#glwindowswapbuffers)
	
## ncpp::GUI::App
Структура, которая представляет из себя экземпляр приложения, в котором происходит главный event-loop.

### App.run()
```cpp
void run();
```
Активирует event-loop обработку событий элементов управления. Блокирует поток.

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
void setTitle(const std::string& title);
```
Устанавливает название окна.

### Window.getText()
```cpp
std::string getText();
```
Достает текущий текст окна/элемента.

### Window.setText()
```cpp
void setText(const std::string& text);
```
Установить текущий текст окна/элемента.

### Window.onEvent()
```cpp
void onEvent(std::string type, HANDLER_PTR);
```
Установить обработчик элемента с указанием необходимого события.

### Window.onClick()
```cpp
void onClick(HANDLER_PTR);
```
Установить обработчик элемента на событие клика.

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
std::string getClass();
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


















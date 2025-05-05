namespace ncpp { namespace GUI { struct App; App* mainApp=NULL; typedef HWND WndID;
	#if __cplusplus >= 201103L
	typedef std::unordered_map<HWND, HANDLER_PAIR_TYPE> EventMap;
	#else
	typedef std::map<HWND, HANDLER_PAIR_TYPE> EventMap;
	#endif
    LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	struct App { HINSTANCE hInstance; WNDCLASSEX wc0; MSG Msg;
		App() : hInstance(GetModuleHandle(NULL)){ wc0.cbClsExtra = 0; wc0.cbWndExtra = 0; // Нет дополнительных данных класса, окна
			   wc0.cbSize = sizeof(WNDCLASSEX); wc0.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
			   wc0.hCursor = LoadCursor(NULL, IDC_ARROW); // подгрузка курсора в окно
			   wc0.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Стандартная иконка
			   wc0.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // Стандартная иконка
			   wc0.hInstance = hInstance;
			   wc0.lpfnWndProc = WndProc; // функция окна
			   wc0.lpszClassName = "MyWindowClass";  // название класса окна
			   wc0.lpszMenuName = NULL; // указатель на меню
			   wc0.style = CS_VREDRAW | CS_HREDRAW;  // стиль окна
			   if(!RegisterClassEx(&wc0)){ MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } mainApp = this; }
			   
        ~App(){  }
        
        static App create(){ return App(); }
        
        void run(){ while(GetMessage(&Msg, NULL, 0, 0) > 0){ TranslateMessage(&Msg); DispatchMessage(&Msg); } }
        
        void setEventHandler(HWND hwnd, std::string type, HANDLER_PTR){ eventHandlers[hwnd] = std::make_pair(type, handler); }
		void clearAllHandlers(){ eventHandlers.clear(); }
		EventMap eventHandlers;	
		//void regWnd(Window* wnd){}
		//getElementById()
    };
	
	struct Window { App* app; WndID wndID; 
		Window(App* app1=NULL, WndID wndID1=NULL) : app(app1), wndID(wndID1){}
		Window(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(app1, name, x, y, width, height); }
		Window(Window* window, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(window->app, name, x, y, width, height, window->wndID); }
		Window(const char* name, int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(mainApp, name, x, y, width, height); }
		~Window(){ destroy(); }
		
		void setTitle(const std::string& title){ if(wndID==NULL) return; SetWindowText(wndID, title.c_str()); }
		std::string getText(){ if(wndID==NULL) return ""; int len = GetWindowTextLength(wndID)+1; 
			std::vector<char> txt(len); GetWindowText(wndID, &txt[0], len); return std::string(&txt[0]); }
		void setText(const std::string& text){ if(wndID==NULL) return; SetWindowText(wndID, text.c_str()); }
		void onEvent(std::string type, HANDLER_PTR){ app->setEventHandler(wndID, type, handler); }
		void onClick(HANDLER_PTR){ onEvent("click", handler); }
		void getSize(int& width, int& height){ if(wndID == 0) return; RECT rect;
			if(GetWindowRect(wndID, &rect)){ width = rect.right - rect.left; height = rect.bottom - rect.top; } }
		void resize(int width, int height){ if(wndID == 0) return;
			SetWindowPos(wndID, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER); SendMessage(wndID, WM_SIZE, 0, 0); }
		void setPos(int x, int y){ if(wndID == 0) return; SetWindowPos(wndID, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER); }
		void Show(){ ShowWindow(wndID, SW_SHOW); } void Hide(){ ShowWindow(wndID, SW_HIDE); }
		void setVisible(bool visible=true){ if(wndID == NULL) return; ShowWindow(wndID, visible ? SW_SHOW : SW_HIDE); }
		void setDisabled(bool disabled=true){ if(wndID == NULL) return; EnableWindow(wndID, disabled ? FALSE : TRUE); }
		std::string getClass(){ char clName[32]; GetClassName(wndID, clName, 32); return clName; }

		void Update(){ UpdateWindow(wndID); }
		bool destroy(){ if(wndID==NULL) return false; DestroyWindow(wndID); wndID = NULL; return true; }
		
		private:
			void createWindow(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT, HWND hWndParent=NULL)
			{ app=app1; wndID = CreateWindowEx(WS_EX_CLIENTEDGE, "MyWindowClass", name, WS_OVERLAPPEDWINDOW, x, y, width, height, hWndParent, NULL, app->hInstance, NULL);
			  if(wndID==NULL){ MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } Show(); //UpdateWindow(wndID); 			
			}
		
	};
	
	struct Button : Window {
		Button(Window* parent, const char* text="", int x=0, int y=0, int width=100, int height=25, int id=0){ createButton(parent, text, x, y, width, height, id); }
		
		private:
		void createButton(Window* parent, const char* text="", int x=0, int y=0, int width=50, int height=20, int id=1)
		{ app=parent->app; wndID = CreateWindowEx(0, "BUTTON", text, WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
			x, y, width, height, parent->wndID, (HMENU)id, app->hInstance, NULL);
		  if(wndID==NULL){ MessageBox(NULL, "Button Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};
	
	struct Label : Window {
		Label(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 100, int height = 20){
			createLabel(parent, text, x, y, width, height); }

		private:
		void createLabel(Window* parent, const char* text, int x, int y, int width, int height){
			app = parent->app; wndID = CreateWindowEx(0, "STATIC", text, WS_CHILD | WS_VISIBLE, 
				x, y, width, height, parent->wndID, NULL, app->hInstance, NULL);
			if(wndID == NULL){ MessageBox(NULL, "Label Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};
	
	struct Input : Window {
		Input(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 120, int height = 24){
			createInput(parent, text, x, y, width, height); }

		private:
		void createInput(Window* parent, const char* text, int x, int y, int width, int height){
			app = parent->app; wndID = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", text, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 
				x, y, width, height, parent->wndID, NULL, app->hInstance, NULL);
			if(wndID == NULL){ MessageBox(NULL, "Input Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};
	
	struct TextArea : Window {
		TextArea(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 200, int height = 100){
			createTextArea(parent, text, x, y, width, height); }

		private:
		void createTextArea(Window* parent, const char* text, int x, int y, int width, int height){
			app = parent->app; wndID = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", text,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL,
				x, y, width, height, parent->wndID, NULL, app->hInstance, NULL);
				
			if(wndID == NULL){ MessageBox(NULL, "TextArea Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};
	
	struct CheckBox : Window {
		CheckBox(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 120, int height = 20, int id = 0){
			createCheckBox(parent, text, x, y, width, height, id); }

		bool isChecked() const { if(wndID == NULL) return false; return (SendMessage(wndID, BM_GETCHECK, 0, 0) == BST_CHECKED); }
		void setChecked(bool checked){ if(wndID == NULL) return; SendMessage(wndID, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0); }

		private:
		void createCheckBox(Window* parent, const char* text, int x, int y, int width, int height, int id){
			app = parent->app; wndID = CreateWindowEx(0, "BUTTON", text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
				x, y, width, height, parent->wndID, (HMENU)id, app->hInstance, NULL);

			if(wndID == NULL){ MessageBox(NULL, "CheckBox Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};

	
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Обработка сообщений, переданных из цикла (MSG msg{};);
	{	switch (uMsg){
			case WM_CREATE: {  } break;
			case WM_COMMAND: { if(mainApp==NULL) break; int wmEvent = HIWORD(wParam); //int wmId = LOWORD(wParam); 
				HWND hElem = (HWND)lParam; if(lParam==0){ hElem=hWnd; }
			    // Вывод всех элементов eventHandlers
				for(EventMap::const_iterator it1 = mainApp->eventHandlers.begin(); it1 != mainApp->eventHandlers.end(); ++it1){
					std::cout << it1->first << " = (" << it1->second.first << ", " << reinterpret_cast<void*>(it1->second.second) << "); "; }
					std::cout << std::endl;
					
				char clName[32]; GetClassName(hElem, clName, 32); std::string cName(clName); std::cout << "cName: " << cName << std::endl;
				EventMap::iterator it = mainApp->eventHandlers.find(hElem); if(it == mainApp->eventHandlers.end()){ break; }
				std::string event="other";
				if(cName=="Edit"){ switch(wmEvent){
					case EN_CHANGE: event = "change"; break;
					case EN_UPDATE: event = "update"; break;
					case EN_SETFOCUS: event = "focus"; break;
					case EN_KILLFOCUS: event = "unfocus"; break;
					case EN_MAXTEXT: event = "maxtext"; break;
					case EN_ERRSPACE: event = "err"; break;
					case EN_HSCROLL: event = "hscroll"; break;
					case EN_VSCROLL: event = "vscroll"; break;
				}}
				else{ switch(wmEvent){
					case BN_CLICKED: { event="click"; } break;
					case BN_DBLCLK: { event="dbclick"; } break;
					case BN_PUSHED: { event="press"; } break;
					case BN_UNPUSHED: { event="unpress"; } break;
					case BN_SETFOCUS: { event="focus"; } break;
					case BN_KILLFOCUS: { event="unfocus"; } break;
					case BN_PAINT: { event="redraw"; } break;
				}} 
				if(event==it->second.first||it->second.first==""||it->second.first=="all"){ it->second.second(event); }
			} break;
			case WM_DESTROY: { PostQuitMessage(EXIT_SUCCESS); } return 0; }
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // вызывается в случае если сообщение не обрабатывается
	}
} }

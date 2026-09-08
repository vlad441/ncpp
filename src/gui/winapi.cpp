namespace ncpp { namespace GUI { //typedef HWND WndID; 
	typedef HashMap<HWND, WNDHandlerInfo> EventMap;
    LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	struct App { HINSTANCE hInstance; WNDCLASSEX wc0; MSG Msg; bool evSeen; unsigned char lastKeyCode;
		App() : hInstance(GetModuleHandle(NULL)), evSeen(false){ wc0.cbClsExtra = 0; wc0.cbWndExtra = 0; // Нет дополнительных данных класса, окна
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
        
        void run(){ while(GetMessage(&Msg, NULL, 0, 0) > 0){ TranslateMessage(&Msg); DispatchMessage(&Msg); } } //Блокирующий Event-Loop
		void waitOneEvent(){ if(GetMessage(&Msg, NULL, 0, 0) > 0) evSeen = true; }
		bool hasEvents(){ if(evSeen) return true; if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)){ evSeen = true; return true; } return false; }
		bool nextEvent(){ if(!evSeen){ if(!PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) return false; }
			evSeen = false; TranslateMessage(&Msg); DispatchMessage(&Msg); return true; }
        
        void setEventHandler(Window& w, const CString& type, HANDLE_FUNC handler);
		void clearAllHandlers(){ eventHandlers.clear(); }
		EventMap eventHandlers;
		static void callHandler(const WNDHandlerInfo& hinfo, const WEvent& ev){ if(ev.name==hinfo.event||hinfo.event==""||hinfo.event=="all"){ hinfo.handler(ev); } }
		//void regWnd(Window* wnd){}
		//getElementById()
    };
	
	struct Window { App* app; HWND wndID; 
		Window(App* app1=NULL, HWND wndID1=NULL) : app(app1), wndID(wndID1){}
		Window(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(app1, name, x, y, width, height); }
		Window(Window* window, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(window->app, name, x, y, width, height, window->wndID); }
		Window(const char* name, int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(mainApp, name, x, y, width, height); }
		~Window(){ destroy(); }
		
		//void setID(const CString& id);
		void setTitle(const CString& title){ if(wndID==NULL) return; SetWindowText(wndID, title.c_str()); }
		String getText(){ if(wndID==NULL) return ""; int len = GetWindowTextLength(wndID)+1; 
			String txt(len); GetWindowText(wndID, &txt[0], len); return txt; }
		void setText(const CString& text){ if(wndID==NULL) return; SetWindowText(wndID, text.c_str()); }
		void setHandler(HANDLE_FUNC handler, const CString& type=""){ app->setEventHandler(*this, type, handler); }
		//void setClickHandler(HANDLE_FUNC handler){ setHandler(handler, "click"); }
		void getWndSize(int& width, int& height) const { if(wndID==0) return; RECT rect;
			if(GetWindowRect(wndID, &rect)){ width = rect.right-rect.left; height = rect.bottom-rect.top; } }
		void getSize(int& width, int& height) const { if(wndID==0) return; RECT rect;
			if(GetClientRect(wndID, &rect)){ width = rect.right-rect.left; height = rect.bottom-rect.top; } }
		void resize(int width, int height){ if(wndID==0) return;
			SetWindowPos(wndID, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER); SendMessage(wndID, WM_SIZE, 0, 0); }
		void setPos(int x, int y){ if(wndID == 0) return; SetWindowPos(wndID, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER); }
		void Show(){ ShowWindow(wndID, SW_SHOW); } void Hide(){ ShowWindow(wndID, SW_HIDE); }
		void setVisible(bool visible=true){ if(wndID == NULL) return; ShowWindow(wndID, visible ? SW_SHOW : SW_HIDE); }
		void setDisabled(bool disabled=true){ if(wndID == NULL) return; EnableWindow(wndID, disabled ? FALSE : TRUE); }
		String getClass() const { char clName[32]; GetClassName(wndID, clName, 32); return clName; }
		void getMousePos(int& x, int& y) const { POINT m; GetCursorPos(&m); ScreenToClient(wndID, &m); x=m.x; y=m.y; }
		void setMousePos(int x, int y){ POINT m = { x, y }; ClientToScreen(wndID, &m); SetCursorPos(m.x, m.y); }

		void Update(){ UpdateWindow(wndID); }
		bool destroy(){ if(wndID==NULL) return false; DestroyWindow(wndID); app->eventHandlers.erase(wndID); wndID = NULL; return true; }
		
		void setFullscreen(bool fullscreen){ if(wndID == NULL) return; static RECT oldRect; static DWORD oldStyle;
			if(fullscreen){
				GetWindowRect(wndID, &oldRect); oldStyle = GetWindowLong(wndID, GWL_STYLE); // Сохраняем текущее положение и стиль, чтобы вернуться назад

				// Убираем рамки, заголовки и меню
				SetWindowLong(wndID, GWL_STYLE, oldStyle & ~(WS_CAPTION | WS_THICKFRAME));
				SetWindowLong(wndID, GWL_EXSTYLE, GetWindowLong(wndID, GWL_EXSTYLE) & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
				int width = GetSystemMetrics(SM_CXSCREEN); int height = GetSystemMetrics(SM_CYSCREEN);

				SetWindowPos(wndID, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_FRAMECHANGED); // Растягиваем окно на весь экран
			}else{ SetWindowLong(wndID, GWL_STYLE, oldStyle); // Возвращаем как было
				SetWindowPos(wndID, NULL, oldRect.left, oldRect.top, oldRect.right - oldRect.left, oldRect.bottom - oldRect.top, SWP_NOZORDER | SWP_FRAMECHANGED); }
		}
		
		void putImage(const void* px0, int w, int h){ if(wndID == NULL || px0 == NULL) return;
			int wndW=0, wndH=0; getSize(wndW, wndH); HDC hdc = GetDC(wndID); if(hdc==NULL) return;
			// RGBA -> BGRA
			Array<uint32_t> pxArr(w*h); const unsigned char* src = (const unsigned char*)px0; unsigned char* px = (unsigned char*)pxArr.data();
			for(int i=0; i<w*h; ++i){ px[0] = src[2]; px[1] = src[1]; px[2] = src[0]; px[3] = src[3]; src+=4; px+=4; } px = (unsigned char*)pxArr.data();

			BITMAPINFO bmi; ZeroMemory(&bmi, sizeof(BITMAPINFO)); // Описываем структуру Bitmap Info для DIB (Device-Independent Bitmap)
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bmi.bmiHeader.biWidth = w; bmi.bmiHeader.biHeight = -h; //Отрицательная высота означает пиксели сверху-вниз (top-down)
			bmi.bmiHeader.biPlanes = 1; bmi.bmiHeader.biBitCount = 32; // 32 бита на пиксель (RGBA / BGRA)
			bmi.bmiHeader.biCompression = BI_RGB; //SetDIBitsToDevice?
			StretchDIBits(hdc, 0, 0, wndW, wndH, 0, 0, w, h, px, &bmi, DIB_RGB_COLORS, SRCCOPY); ReleaseDC(wndID, hdc); }
		//void putImage(const Image& img){ void putImage(img.px.data(), img.w, img.h); }
			
		#if __cplusplus >= 201103L //move for C++11
		Window(Window&& tmp) noexcept : wndID(NULL){ move(*this, tmp); }
		Window& operator=(Window&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
		Window(const Window&) = delete; Window& operator=(const Window&) = delete; //Запрет копирования.
		Window& steal(Window& tmp){ move(*this, tmp); return *this; }
		Window& steal(Window&& tmp){ move(*this, tmp); return *this; }
		friend void move(Window& dst, Window&& tmp){ move(dst, (Window&)tmp); }
		#else //move for C++98
		private: Window(const Window&); Window& operator=(const Window&); public: //Скрытие копирования.
		Window& steal(const Window& victim){ move(*this, (Window&)victim); return *this; }
		friend void move(Window& dst, const Window& victim){ move(dst, (Window&)victim); }
		#endif
		friend void move(Window& dst, Window& tmp){ if(&dst==&tmp) return; dst.destroy(); dst.app = tmp.app; dst.wndID = tmp.wndID; tmp.wndID = NULL; }
		
		private:
			void createWindow(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT, HWND hWndParent=NULL)
			{ 	app=app1; wndID = CreateWindowEx(WS_EX_CLIENTEDGE, "MyWindowClass", name, hWndParent?(WS_CHILD | WS_VISIBLE):WS_OVERLAPPEDWINDOW, 
					x, y, width, height, hWndParent, NULL, app->hInstance, NULL);
				if(wndID==NULL){ MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } Show(); //UpdateWindow(wndID); 			
			}
	};
	
	void App::setEventHandler(Window& w, const CString& type, HANDLE_FUNC handler){ eventHandlers[w.wndID] = WNDHandlerInfo(&w, handler, type); }
	
	struct Button : Window {
		Button(Window* parent, const char* text="", int x=0, int y=0, int width=100, int height=25, int id=0){ createButton(parent, text, x, y, width, height, id); }
		
		private:
		void createButton(Window* parent, const char* text="", int x=0, int y=0, int width=50, int height=20, int id=1)
		{ app=parent->app; wndID = CreateWindowEx(0, "BUTTON", text, WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
			x, y, width, height, parent->wndID, (HMENU)(size_t)id, app->hInstance, NULL);
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
				x, y, width, height, parent->wndID, (HMENU)(size_t)id, app->hInstance, NULL);

			if(wndID == NULL){ MessageBox(NULL, "CheckBox Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; } }
	};

	
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Обработка сообщений, переданных из цикла (MSG msg{};);
	{	//print("uMsg: "); print(uMsg); print("\n");
		if(mainApp==NULL) return DefWindowProc(hWnd, uMsg, wParam, lParam); EventMap::iterator evIt; 
		switch(uMsg){ case WM_CREATE: break;
			case WM_COMMAND: { // Вывод всех элементов eventHandlers
				//for(EventMap::const_iterator it1 = mainApp->eventHandlers.begin(); it1 != mainApp->eventHandlers.end(); ++it1){
					//cons << it1->first << " = (" << it1->second.first << ", " << reinterpret_cast<void*>(it1->second.second) << "); "; 
				//}	cons << "\n";
				HWND hElem = (lParam!=0) ? (HWND)lParam : hWnd; evIt = mainApp->eventHandlers.find(hElem);
				if(evIt == mainApp->eventHandlers.end()){ break; } WEvent ev(evIt->second.wnd); int wmEvent = HIWORD(wParam); //int wmId = LOWORD(wParam);
					
				char clName[32]; GetClassName(hElem, clName, 32); String cName(clName); //print(&clName[0]); print("\n");
				if(cName=="Edit"){ switch(wmEvent){
					case EN_CHANGE: ev.name = "change"; break;
					case EN_UPDATE: ev.name = "update"; break;
					case EN_SETFOCUS: ev.name = "focus"; break;
					case EN_KILLFOCUS: ev.name = "unfocus"; break;
					case EN_MAXTEXT: ev.name = "maxtext"; break;
					case EN_ERRSPACE: ev.name = "err"; break;
					case EN_HSCROLL: ev.name = "hscroll"; break;
					case EN_VSCROLL: ev.name = "vscroll"; break;
				}}
				else{ switch(wmEvent){
					case BN_CLICKED: ev.id=WEvent::CLICK; ev.name="click"; break;
					case BN_DBLCLK: ev.name="dbclick"; break;
					case BN_PUSHED: ev.name="press"; break;
					case BN_UNPUSHED: ev.name="unpress"; break;
					case BN_SETFOCUS: ev.name="focus"; break;
					case BN_KILLFOCUS: ev.name="unfocus"; break;
					case BN_PAINT: ev.name="redraw"; break;
				}} App::callHandler(evIt->second, ev); break; }
			case WM_KEYDOWN: case WM_KEYUP: { HWND hElem = hWnd; evIt = mainApp->eventHandlers.find(hElem); 
				if(evIt == mainApp->eventHandlers.end()){ break; } WEvent ev(evIt->second.wnd);
				switch(uMsg){
					case WM_KEYDOWN: { ev.id=WEvent::KEYDOWN; ev.key=_FromNativeKeyID((int)wParam); ev.name = "keydown"; 
						MSG charMsg; if(PeekMessage(&charMsg, hWnd, WM_CHAR, WM_CHAR, PM_REMOVE)){ ev.x = (int)charMsg.wParam; } break; }
					case WM_KEYUP: ev.id=WEvent::KEYUP; ev.key=_FromNativeKeyID((int)wParam); ev.name = "keyup"; break;
					//case WM_CHAR: { bool isRepeat = (lParam & (1 << 30)); if(isRepeat) return 0; ev.key=(int)wParam; ev.name = "keychar"; break; } 
				} App::callHandler(evIt->second, ev); break; }
			case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: {
				evIt = mainApp->eventHandlers.find(hWnd); if(evIt == mainApp->eventHandlers.end()) break; WEvent ev(evIt->second.wnd, WEvent::CLICK);
				switch(uMsg){
					case WM_LBUTTONDOWN: ev.key=WEvent::LBTN; ev.name = "click"; break; 
					case WM_RBUTTONDOWN: ev.key=WEvent::RBTN; ev.name = "rclick"; break; 
					case WM_MBUTTONDOWN: ev.key=WEvent::MBTN; ev.name = "mclick"; break;
					case WM_LBUTTONUP: ev.id=WEvent::MOUSEUP; ev.key=WEvent::LBTN; ev.name = "mouseup"; break;
					case WM_RBUTTONUP: ev.id=WEvent::MOUSEUP; ev.key=WEvent::RBTN; ev.name = "rmouseup"; break;
					case WM_MBUTTONUP: ev.id=WEvent::MOUSEUP; ev.key=WEvent::MBTN; ev.name = "mmouseup"; break; }  
				ev.x = LOWORD(lParam); ev.y = HIWORD(lParam); App::callHandler(evIt->second, ev); break; }
			case WM_MOUSEMOVE: break;
			case WM_DESTROY: { PostQuitMessage(EXIT_SUCCESS); } return 0; } 
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // вызывается в случае если сообщение не обрабатывается
	}
} }

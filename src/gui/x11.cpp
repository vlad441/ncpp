namespace ncpp { namespace GUI { //typedef XID XWindowID;
	typedef HashMap<XWindowID, WNDHandlerInfo> EventMap;
	typedef HashMap<String, Window*> WndIDList;
	typedef HashMap<XWindowID, Window*> WndList;
	
	enum WndClass { NONE, BUTTON, LABEL, INPUT, CHECKBOX };
	
	void _RedrawWindow(Window* wnd, const WEvent& ev);
	int _XKeyDecode(XEvent& xevent){ int code = (int)XLookupKeysym(&xevent.xkey, 0); if(code >= 97 && code <= 122) return code - 32; return _FromNativeKeyID(code); }
	int _XKeyCharDecode(XEvent& xevent){ char buffer[32]; KeySym keysym; XComposeStatus compose;
		int count = XLookupString(&xevent.xkey, buffer, sizeof(buffer), &keysym, &compose); // Она учитывает раскладку (Shift, Caps) для символов
		if(count > 0){ return buffer[0]; } return 0; }
	//KeyCodes: A-Z, a-z, 0-9, Space = WinApi = X11
	
    struct App { Display *display; int screen; XWindowID rootID; GC gc; XEvent xMsg; bool evSeen; unsigned char lastKeyCode;
        App() : evSeen(false){ display = XOpenDisplay(NULL); if(display == NULL){ print("(!) Cannot open display!\n"); exit(1); }
            screen = DefaultScreen(display); rootID = RootWindow(display, screen); gc = XCreateGC(display, rootID, 0, NULL); mainApp = this; }

        ~App(){ XFreeGC(display, gc); XCloseDisplay(display); }

        static App create(){ return App(); }

        void run(){ while(true){ XNextEvent(display, &xMsg); handleEvent(xMsg); } } //if(xMsg.type == DestroyNotify) break;
		void waitOneEvent(){ if(evSeen) return; XNextEvent(display, &xMsg); evSeen = true; }
		bool hasEvents(){ if(evSeen) return true; if(XPending(display)>0){ XNextEvent(display, &xMsg); evSeen = true; return true; } return false; }
		bool nextEvent(){ if(!evSeen&&!hasEvents()){ return false; } evSeen = false; handleEvent(xMsg); return true; }

        void setEventHandler(Window& w, const CString& type, HANDLE_FUNC handler);
        void clearAllHandlers(){ eventHandlers.clear(); }
        EventMap eventHandlers; WndList _wndlist;
        void regWnd(Window* wnd);
        void delWnd(Window* wnd);
		static void callHandler(const WNDHandlerInfo& hinfo, const WEvent& ev){ if(ev.name==hinfo.event||hinfo.event==""||hinfo.event=="all"){ hinfo.handler(ev); } }

    private:
        void handleEvent(XEvent& xevent){ XWindowID wndID = xevent.xany.window; WndList::iterator wit = _wndlist.find(wndID);
			if(wit!=_wndlist.end()){ _RedrawWindow(wit->second, _EventDecode(xevent)); } EventMap::iterator it = eventHandlers.find(wndID); 
			if(it == eventHandlers.end()){ return; } WEvent ev = _EventDecode(xevent); ev.wnd=it->second.wnd; App::callHandler(it->second, ev);
        }
        WEvent _EventDecode(XEvent& xevent){ WEvent ev; ev.id=WEvent::OTHER;
			switch(xevent.type){
				case Expose: ev.name = "expose"; break;
				case ButtonPress: { ev.id=WEvent::CLICK; ev.x = xevent.xbutton.x; ev.y = xevent.xbutton.y;
					switch(xevent.xbutton.button){ case 1: ev.key=WEvent::LBTN; ev.name = "click"; break; 
						case 2: ev.key=WEvent::MBTN; ev.name = "mclick"; break; case 3: ev.key=WEvent::RBTN; ev.name = "rclick"; break;
						case 4: ev.key=4; ev.name = "scrollup"; break; case 5: ev.key=5; ev.name = "scrolldown"; break; 
						default: ev.key=xevent.xbutton.button; ev.name = "click"; break; } break; }
				case ButtonRelease: { ev.id=WEvent::MOUSEUP; ev.x = xevent.xbutton.x; ev.y = xevent.xbutton.y;
					switch(xevent.xbutton.button){ case 1: ev.key=WEvent::LBTN; ev.name = "mouseup"; break; 
						case 2: ev.key=WEvent::MBTN; ev.name = "mmouseup"; break; case 3: ev.key=WEvent::RBTN; ev.name = "rmouseup"; break;
						default: ev.key=xevent.xbutton.button; ev.name = "click"; break; } break; }
				case KeyPress: { ev.id=WEvent::KEYDOWN; ev.key = _XKeyDecode(xevent); ev.x = _XKeyCharDecode(xevent); ev.name = "keydown"; break; }
				case KeyRelease: {
					if(XEventsQueued(display, QueuedAfterReading)){ XEvent next_ev; XPeekEvent(display, &next_ev);
						if(next_ev.type == KeyPress && next_ev.xkey.time == xevent.xkey.time && next_ev.xkey.keycode == xevent.xkey.keycode){ 
							break; } } // Это автоповтор. Пожираем событие нажатия из очереди и выходим.
					ev.id=WEvent::KEYUP; ev.key = _XKeyDecode(xevent); ev.x = _XKeyCharDecode(xevent); ev.name = "keyup"; break; }
				case MotionNotify: ev.name = "move"; break;
				case FocusIn: ev.name = "focus"; break;
				case FocusOut: ev.name = "unfocus"; break;
				case EnterNotify: ev.name = "EnterNotify"; break;
				case LeaveNotify: ev.name = "LeaveNotify"; break;
				case KeymapNotify: ev.name = "KeymapNotify"; break;
				case DestroyNotify: ev.name = "DestroyNotify"; break;
				case ConfigureNotify: ev.name = "ConfigureNotify"; break;
				case MapNotify: ev.name = "MapNotify"; break;
				case UnmapNotify: ev.name = "UnmapNotify"; break;
				case ClientMessage: ev.name = "ClientMessage"; break;
			} return ev; }
    };

    struct Window { App* app; XWindowID wndID; WndClass cls;
        Window(App* app1=NULL, XWindowID wndID1=0) : app(app1), wndID(wndID1){}
        Window(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(app1, name, x, y, width, height); }
		Window(Window* window, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(window->app, name, x, y, width, height, window->wndID); }
		Window(const char* name, int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(mainApp, name, x, y, width, height); }
		~Window(){ destroy(); }
		
		//void setID(const CString& id);
        void setTitle(const CString& text){ XStoreName(app->display, wndID, text.c_str()); XClearWindow(app->display, wndID); XFlush(app->display); }
		String getText(){ if(wndID == 0) return ""; char* name = NULL; String result;
			if(XFetchName(app->display, wndID, &name) > 0 && name != NULL){ result = name; XFree(name); } return result; }
		void setText(const CString& text){ setTitle(text); }
        void setHandler(HANDLE_FUNC handler, const CString& type=""){ app->setEventHandler(*this, type, handler); }
		//void setClickHandler(HANDLE_FUNC handler){ setHandler(handler, "click"); }
		void getWndSize(int& width, int& height) const { getSize(width, height); }
        void getSize(int& width, int& height) const { if(wndID == 0) return; XWindowAttributes attr; XGetWindowAttributes(app->display, wndID, &attr); width = attr.width; height = attr.height; }
        void resize(int width, int height) {if(wndID == 0) return; XResizeWindow(app->display, wndID, width, height); } //XFlush(app->display); // Сбрасываем очередь команд
		void setPos(int x, int y){ if(wndID == 0) return; XMoveWindow(app->display, wndID, x, y); } //XFlush(app->display); // Сбрасываем очередь команд, чтобы изменения вступили в силу
		void Show(){ XMapWindow(app->display, wndID); } void Hide(){ XUnmapWindow(app->display, wndID); }
		void setVisible(bool visible = true){ if(wndID == 0) return;
			if(visible) XMapWindow(app->display, wndID); else XUnmapWindow(app->display, wndID); }
		void setDisabled(bool disabled = true){ if(wndID == 0) return;
			long mask = disabled ? NoEventMask : (ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
			XSelectInput(app->display, wndID, mask); }
		String getClass() const { return ""; }
		void getMousePos(int& x, int& y) const { if(wndID == 0) return;
			XWindowID root_return, child_return; int root_x, root_y, win_x, win_y; unsigned int mask_return;
			Bool result = XQueryPointer(app->display, wndID, &root_return, &child_return, &root_x, &root_y, &win_x, &win_y, &mask_return);
			if(result){ x = win_x; y = win_y; } }
		void setMousePos(int x, int y){ if(wndID == 0) return; XWarpPointer(app->display, None, wndID, 0, 0, 0, 0, x, y); XFlush(app->display); }
		
		void Update(){ if(wndID == 0) return; XClearWindow(app->display, wndID); XFlush(app->display); }
		bool destroy(){ if(wndID == 0) return false; XDestroyWindow(app->display, wndID); wndID = 0; app->delWnd(this); return true; }
		
		void setFullscreen(bool fullscreen){ if(wndID == 0) return;
			//Атомы — это идентификаторы свойств в X-сервере
			Atom wmState = XInternAtom(app->display, "_NET_WM_STATE", False);
			Atom fullscreenAtom = XInternAtom(app->display, "_NET_WM_STATE_FULLSCREEN", False);

			XEvent xev; memset(&xev, 0, sizeof(xev)); xev.type = ClientMessage; xev.xclient.window = wndID;
			xev.xclient.message_type = wmState; xev.xclient.format = 32;
			// Сообщение согласно протоколу EWMH:
			xev.xclient.data.l[0] = fullscreen?1:0; xev.xclient.data.l[1] = fullscreenAtom; xev.xclient.data.l[2] = 0; xev.xclient.data.l[3] = 1;
			XSendEvent(app->display, app->rootID, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev); XFlush(app->display); }
			
		void putImage(const void* px, int w, int h){ if(wndID == 0 || px == NULL) return; //int wndW=0, wndH=0; getSize(wndW, wndH);
			XImage* ximg = XCreateImage(app->display, DefaultVisual(app->display, app->screen), DefaultDepth(app->display, app->screen), 
				ZPixmap, 0, (char*)px, w, h, 32, // выравнивание (32 бита)
				0); // bytes_per_line (0 = автовычисление w * 4)
			if(ximg==NULL) return; // Базовая XPutImage не поддерживает масштабирование.
			// Возможный скейл через XGetSubImage / XPutImage, либо задействуют XShm / XScale, но классический прямой вывод:
			XPutImage(app->display, wndID, app->gc, ximg, 0, 0, 0, 0, w, h); ximg->data = NULL; XDestroyImage(ximg); XFlush(app->display); }
		//void putImage(const Image& img){ void putImage(img.px.data(), img.w, img.h); }
			
		#if __cplusplus >= 201103L //move for C++11
		Window(Window&& tmp) noexcept : wndID(0){ move(*this, tmp); }
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
		friend void move(Window& dst, Window& tmp){ if(&dst==&tmp) return; dst.destroy(); dst.app = tmp.app; dst.wndID = tmp.wndID; tmp.wndID = 0; }

		//protected: String text;
		private:
			void createWindow(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT, XWindowID wIDParent=0){ app = app1;
				wndID = XCreateSimpleWindow(app->display, app->rootID, x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
				XSetStandardProperties(app->display, wndID, name, name, None, NULL, 0, NULL); XSelectInput(app->display, wndID, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | StructureNotifyMask);
				cls=NONE; Show(); }
    };
	
	void App::setEventHandler(Window& w, const CString& type, HANDLE_FUNC handler){ eventHandlers[w.wndID] = WNDHandlerInfo(&w, handler, type); }
    void App::regWnd(Window* wnd){ _wndlist[wnd->wndID]=wnd; }
    void App::delWnd(Window* wnd){ _wndlist.erase(wnd->wndID); wnd->app->eventHandlers.erase(wnd->wndID); }

    struct Button : Window {
        Button(Window* parent, const char* text="", int x=0, int y=0, int width=100, int height=25, int id=0){ createButton(parent, text, x, y, width, height, id); }

		private: 
        void createButton(Window* parent, const char* text, int x, int y, int width, int height, int id){ app = parent->app;
            wndID = XCreateSimpleWindow(app->display, parent->wndID, x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
            XStoreName(app->display, wndID, text); XSelectInput(app->display, wndID, ExposureMask | ButtonPressMask); cls=BUTTON; app->regWnd(this); XClearWindow(app->display, wndID); Show(); }
        public: void render(){ String name = getText();
			XClearWindow(app->display, wndID);
			XDrawRectangle(app->display, wndID, app->gc, 0, 0, 99, 24); // рамка
			XDrawString(app->display, wndID, app->gc, 10, 17, name.c_str(), name.length());
			XFlush(app->display); }
    };
    
    struct Label : Window { 
		Label(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 100, int height = 20){
			text = txt; createLabel(parent, x, y, width, height); }
		
		void setTitle(const CString& txt){ setText(txt); }
		String getText() const { return text; }	
		void setText(const CString& txt){ text = txt; XClearWindow(app->display, wndID); render(); }

		private: String text;
		void createLabel(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 0, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask); cls=LABEL; app->regWnd(this); Show(); }
		public: void render(){ XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.size()); }
	};
	
	struct Input : Window { 
		Input(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 120, int height = 24){
			text = txt; createInput(parent, x, y, width, height); }

		String getText() const { return text; }	
		void setText(const CString& txt){ text = txt; XClearWindow(app->display, wndID); render(); }

		private: String text;
		void createInput(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask | KeyPressMask); cls=INPUT; app->regWnd(this); Show(); }
		public: void render(const CString& ev=""){ 
			if(ev == "keydown"){ XEvent event; XPeekEvent(app->display, &event);
				if(event.type == KeyPress){
					KeySym key; char buffer[8];
					XLookupString(&event.xkey, buffer, 7, &key, 0);
					if(key == XK_BackSpace && !text.empty()) text.pop();
					else if(buffer[0] >= 32 && buffer[0] < 127) text += buffer[0];
					XClearWindow(app->display, wndID); XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.length());
				}
			}else{ XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.size()); }
		}
	};
	
	struct TextArea : Window {  
		TextArea(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 200, 
			int height = 100){ createInput(parent, x, y, width, height); } 
		private: String text;
			void createInput(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask | KeyPressMask); cls=INPUT; app->regWnd(this); Show(); } };
	
	struct CheckBox : Window { 

		CheckBox(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 140, int height = 20){
			checked = false; createCheckBox(parent, text, x, y, width, height); }

		bool isChecked() const { return checked; }
		void setChecked(bool val){ checked = val; XClearWindow(app->display, wndID); }

		private: bool checked; String label;
		void createCheckBox(Window* parent, const char* text, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, x, y, width, height, 0,
				BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XStoreName(app->display, wndID, text); XSelectInput(app->display, wndID, ExposureMask | ButtonPressMask); cls=CHECKBOX; app->regWnd(this); Show();
		}
		public: void render(bool clicked=false){ String name = getText();
			XClearWindow(app->display, wndID); XDrawRectangle(app->display, wndID, app->gc, 3, 3, 12, 12);
			if(clicked){ checked = !checked; }
			if(checked){
				XDrawLine(app->display, wndID, app->gc, 5, 9, 8, 14);
				XDrawLine(app->display, wndID, app->gc, 8, 14, 12, 4);
			}
			XDrawString(app->display, wndID, app->gc, 22, 14, name.c_str(), name.size()); }
	};
		
	void _DrawCheckBox(CheckBox* wnd, bool clicked=false){}
		
	void _RedrawWindow(Window* wnd, const WEvent& ev){ if(wnd==NULL) return;
		//cons << "_RedrawWindow: cls = " << wnd->cls << " | wndID: " << wnd->wndID << "\n";
		switch(wnd->cls){ case NONE: { break; } case BUTTON: ((Button*)wnd)->render(); break;
			case LABEL: ((Label*)wnd)->render(); break;
			case INPUT: ((Input*)wnd)->render(ev.name); break;  //(Input*)
			case CHECKBOX: ((CheckBox*)wnd)->render((ev.name=="click"?true:false)); break; 
		} }
} }

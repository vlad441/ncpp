namespace ncpp { namespace GUI { struct App; App* mainApp = NULL;
    typedef XID XWindowID; struct Window; typedef XWindowID WndID;
    #if __cplusplus >= 201103L
	typedef std::unordered_map<XWindowID, HANDLER_PAIR_TYPE> EventMap;
	typedef std::unordered_map<XWindowID, Window*> WndList;
	#else
	typedef std::map<XWindowID, HANDLER_PAIR_TYPE> EventMap;
	typedef std::map<XWindowID, Window*> WndList;
	#endif
	
	enum WndClass { NONE, BUTTON, LABEL, INPUT, CHECKBOX };
	
	struct Event {
		Window* wnd;
		std::string type;
		void (*func)(Event); };
	
	void _RedrawWindow(Window*, std::string);
    struct App { Display *display; int screen; XWindowID rootID; GC gc;
        App() { display = XOpenDisplay(NULL); if(display == NULL){ std::cerr << "Cannot open display!" << std::endl; exit(1); }
            screen = DefaultScreen(display); rootID = RootWindow(display, screen); gc = XCreateGC(display, rootID, 0, NULL); mainApp = this; }

        ~App(){ XFreeGC(display, gc); XCloseDisplay(display); }

        static App create(){ return App(); }

        void run(){ XEvent event; while(true){ XNextEvent(display, &event); if(event.type == DestroyNotify) break; handleEvent(event); } }

        void setEventHandler(XWindowID xwdID, std::string type, HANDLER_PTR){ eventHandlers[xwdID] = std::make_pair(type, handler); }
        void clearAllHandlers(){ eventHandlers.clear(); }
        EventMap eventHandlers; WndList wndlist;
        void regWnd(Window* wnd);
        void delWnd(Window* wnd);
        

    private:
        void handleEvent(XEvent& event){ XWindowID wndID = event.xany.window; WndList::iterator wit = wndlist.find(wndID);
			if(wit!=wndlist.end()){ _RedrawWindow(wit->second, _EventDecode(event)); }
					
			EventMap::iterator it = eventHandlers.find(wndID); if(it != eventHandlers.end()){ std::string eventType = _EventDecode(event);
				if(eventType == it->second.first || it->second.first == "" || it->second.first == "all"){ it->second.second(eventType); } }
        }
        std::string _EventDecode(XEvent& xevent){ std::string event="other";
			switch(xevent.type){
				case Expose: event = "expose"; break;
				case ButtonPress: event = "click"; break;
				case ButtonRelease: event = "release"; break;
				case FocusIn: event = "focus"; break;
				case FocusOut: event = "unfocus"; break;
				case KeyPress: event = "key_down"; break;
				case KeyRelease: event = "key_up"; break;
				case MotionNotify: event = "move"; break;
				case EnterNotify: event = "EnterNotify"; break;
				case LeaveNotify: event = "LeaveNotify"; break;
				case KeymapNotify: event = "KeymapNotify"; break;
				case DestroyNotify: event = "DestroyNotify"; break;
				case ConfigureNotify: event = "ConfigureNotify"; break;
				case MapNotify: event = "MapNotify"; break;
				case UnmapNotify: event = "UnmapNotify"; break;
				case ClientMessage: event = "ClientMessage"; break;
			} return event; }
    };

    struct Window { App* app; WndID wndID; WndClass cls;
        Window(App* app1=NULL, WndID wndID1=0) : app(app1), wndID(wndID1){}
        Window(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(app1, name, x, y, width, height); }
		Window(Window* window, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(window->app, name, x, y, width, height, window->wndID); }
		Window(const char* name, int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT){ createWindow(mainApp, name, x, y, width, height); }
		~Window(){ destroy(); }

        void setTitle(const std::string& text){ XStoreName(app->display, wndID, text.c_str()); XClearWindow(app->display, wndID); XFlush(app->display); }
		std::string getText(){ if(wndID == 0) return ""; char* name = NULL; std::string result;
			if(XFetchName(app->display, wndID, &name) > 0 && name != NULL){ result = name; XFree(name); } return result; }
		void setText(const std::string& text){ setTitle(text); }
        void onEvent(std::string type, HANDLER_PTR){ app->setEventHandler(wndID, type, handler); }
        void onClick(HANDLER_PTR){ onEvent("click", handler); }
        void getSize(int& width, int& height){ if(wndID == 0) return; XWindowAttributes attr; XGetWindowAttributes(app->display, wndID, &attr); width = attr.width; height = attr.height; }
        void resize(int width, int height) {if(wndID == 0) return; XResizeWindow(app->display, wndID, width, height); //XFlush(app->display); // Сбрасываем очередь команд
		}
		void setPos(int x, int y){ if(wndID == 0) return; XMoveWindow(app->display, wndID, x, y); //XFlush(app->display); // Сбрасываем очередь команд, чтобы изменения вступили в силу
		}
		void Show(){ XMapWindow(app->display, wndID); } void Hide(){ XUnmapWindow(app->display, wndID); }
		void setVisible(bool visible = true){ if(wndID == 0) return;
			if(visible) XMapWindow(app->display, wndID); else XUnmapWindow(app->display, wndID); }
		void setDisabled(bool disabled = true){ if(wndID == 0) return;
			long mask = disabled ? NoEventMask : (ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
			XSelectInput(app->display, wndID, mask); }
		std::string getClass(){ return ""; }
		void Update(){ if(wndID == 0) return; XClearWindow(app->display, wndID); XFlush(app->display); }
		bool destroy(){ if(wndID == 0) return false; XDestroyWindow(app->display, wndID); wndID = 0; app->delWnd(this); return true; }

		//protected: std::string text;
		private:
			void createWindow(App* app1, const char* name="", int x=DEF_HWND_X, int y=DEF_HWND_Y, int width=DEF_HWND_WIDTH, int height=DEF_HWND_HEIGHT, XWindowID wIDParent=0){ app = app1;
				wndID = XCreateSimpleWindow(app->display, app->rootID, x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
				XSetStandardProperties(app->display, wndID, name, name, None, NULL, 0, NULL); XSelectInput(app->display, wndID, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask); 
				cls=NONE; Show(); }
    };
    
    void App::regWnd(Window* wnd){ wndlist[wnd->wndID]=wnd; }
    void App::delWnd(Window* wnd){ wndlist.erase(wnd->wndID); }

    struct Button : Window {
        Button(Window* parent, const char* text="", int x=0, int y=0, int width=100, int height=25, int id=0){ createButton(parent, text, x, y, width, height, id); }

		private: 
        void createButton(Window* parent, const char* text, int x, int y, int width, int height, int id){ app = parent->app;
            wndID = XCreateSimpleWindow(app->display, parent->wndID, x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
            XStoreName(app->display, wndID, text); XSelectInput(app->display, wndID, ExposureMask | ButtonPressMask); cls=BUTTON; app->regWnd(this); XClearWindow(app->display, wndID); Show(); }
        public: void draw(){ std::string name = getText();
			XClearWindow(app->display, wndID);
			XDrawRectangle(app->display, wndID, app->gc, 0, 0, 99, 24); // рамка
			XDrawString(app->display, wndID, app->gc, 10, 17, name.c_str(), name.length());
			XFlush(app->display); }
    };
    
    struct Label : Window { 
		Label(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 100, int height = 20){
			text = txt; createLabel(parent, x, y, width, height); }
		
		void setTitle(const std::string& txt){ setText(txt); }
		std::string getText() const { return text; }	
		void setText(const std::string& txt){ text = txt; XClearWindow(app->display, wndID); draw(); }

		private: std::string text;
		void createLabel(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 0, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask); cls=LABEL; app->regWnd(this); Show(); }
		public: void draw(){ XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.size()); }
	};
	
	struct Input : Window { 
		Input(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 120, int height = 24){
			text = txt; createInput(parent, x, y, width, height); }

		std::string getText() const { return text; }	
		void setText(const std::string& txt){ text = txt; XClearWindow(app->display, wndID); draw(); }

		private: std::string text;
		void createInput(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask | KeyPressMask); cls=INPUT; app->regWnd(this); Show(); }
		public: void draw(std::string ev=""){ 
			if(ev == "key_down"){ XEvent event; XPeekEvent(app->display, &event);
				if(event.type == KeyPress){
					KeySym key; char buffer[8];
					XLookupString(&event.xkey, buffer, 7, &key, 0);
					if(key == XK_BackSpace && !text.empty()) text.erase(text.size() - 1);
					else if(buffer[0] >= 32 && buffer[0] < 127) text += buffer[0];
					XClearWindow(app->display, wndID); XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.length());
				}
			}else{ XDrawString(app->display, wndID, app->gc, 5, 15, text.c_str(), text.size()); }
		}
	};
	
	struct TextArea : Window {  
		TextArea(Window* parent, const char* txt = "", int x = 0, int y = 0, int width = 200, 
			int height = 100){ createInput(parent, x, y, width, height); } 
		private: std::string text;
			void createInput(Window* parent, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, 
				x, y, width, height, 1, BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XSelectInput(app->display, wndID, ExposureMask | KeyPressMask); cls=INPUT; app->regWnd(this); Show(); } };
	
	struct CheckBox : Window { 

		CheckBox(Window* parent, const char* text = "", int x = 0, int y = 0, int width = 140, int height = 20){
			checked = false; createCheckBox(parent, text, x, y, width, height); }

		bool isChecked() const { return checked; }
		void setChecked(bool val){ checked = val; XClearWindow(app->display, wndID); }

		private: bool checked; std::string label;
		void createCheckBox(Window* parent, const char* text, int x, int y, int width, int height){
			app = parent->app; wndID = XCreateSimpleWindow(app->display, parent->wndID, x, y, width, height, 0,
				BlackPixel(app->display, app->screen), WhitePixel(app->display, app->screen));
			XStoreName(app->display, wndID, text); XSelectInput(app->display, wndID, ExposureMask | ButtonPressMask); cls=CHECKBOX; app->regWnd(this); Show();
		}
		public: void draw(bool clicked=false){ std::string name = getText();
			XClearWindow(app->display, wndID); XDrawRectangle(app->display, wndID, app->gc, 3, 3, 12, 12);
			if(clicked){ checked = !checked; }
			if(checked){
				XDrawLine(app->display, wndID, app->gc, 5, 9, 8, 14);
				XDrawLine(app->display, wndID, app->gc, 8, 14, 12, 4);
			}
			XDrawString(app->display, wndID, app->gc, 22, 14, name.c_str(), name.size()); }
	};
		
	void _DrawCheckBox(CheckBox* wnd, bool clicked=false){}
		
	void _RedrawWindow(Window* wnd, std::string ev){ if(wnd==NULL) return;
		std::cout << "_RedrawWindow: cls = " << wnd->cls << " | wndID: " << wnd->wndID << std::endl;
		switch(wnd->cls){ case NONE: { break; } case BUTTON: ((Button*)wnd)->draw(); break;
			case LABEL: ((Label*)wnd)->draw(); break;
			case INPUT: ((Input*)wnd)->draw(ev); break;  //(Input*)
			case CHECKBOX: ((CheckBox*)wnd)->draw((ev=="click"?true:false)); break; 
		} }

} }

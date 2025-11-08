namespace ncpp{
	namespace Mouse {
	#ifdef _WIN32
		bool SetPos(int x, int y){ return ::SetCursorPos(x, y); }
		void GetPos(int& x, int& y){ POINT m; GetCursorPos(&m); x=m.x; y=m.y; }
		void Click(){ mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); }
		void RClick(){ mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0); }
		void MClick(){ mouse_event(MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0); }
	#elif USE_GUI //X11
	#include <X11/extensions/XTest.h>
		bool SetPos(int x, int y){ Display* display = XOpenDisplay(NULL); if(!display) return false; 
			XTestFakeMotionEvent(display, -1, x, y, CurrentTime); XCloseDisplay(display); return true; }

		void GetPos(int& x, int& y){ Display* display = XOpenDisplay(NULL); if(!display){ x = -1; y = -1; return; }
			int root_x, root_y, win_x, win_y; unsigned int mask; Window root_window, child_window;
			XQueryPointer(display, XDefaultRootWindow(display), &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);
			x = root_x; y = root_y; XCloseDisplay(display); }

		void Click(){ Display* display = XOpenDisplay(NULL); if(!display) return;
			XTestFakeButtonEvent(display, 1, True,  CurrentTime); XTestFakeButtonEvent(display, 1, False, CurrentTime); XCloseDisplay(display); }

		void RClick(){ Display* display = XOpenDisplay(NULL); if(!display) return;
			XTestFakeButtonEvent(display, 3, True,  CurrentTime); XTestFakeButtonEvent(display, 3, False, CurrentTime); XCloseDisplay(display); }
			
		void MClick(){ Display* display = XOpenDisplay(NULL); if(!display) return; 
			XTestFakeButtonEvent(display, 2, True,  CurrentTime); XTestFakeButtonEvent(display, 2, False, CurrentTime); XCloseDisplay(display); }
	#else //Only declarations
		bool SetPos(int x, int y);
		void GetPos(int& x, int& y);
		void Click();
		void RClick();
		void MClick();
	#endif
		
	}
	
	namespace Keyboard {
	#ifdef _WIN32
		void PressKey(char key){ keybd_event(key, 0,0,0); keybd_event(key, 0,KEYEVENTF_KEYUP,0); }
	#elif USE_GUI //X11
	#include <X11/extensions/XTest.h>
		void PressKey(char key){ Display* display = XOpenDisplay(NULL); if(!display) return;
			KeySym keysym = XStringToKeysym(&key); if(keysym == NoSymbol){ XCloseDisplay(display); return; }
			unsigned int keycode = XKeysymToKeycode(display, keysym); if(keycode == 0){ XCloseDisplay(display); return; }
			XTestFakeKeyEvent(display, keycode, True, CurrentTime); XTestFakeKeyEvent(display, keycode, False, CurrentTime); XCloseDisplay(display); }
	#else
		void PressKey(char key);
	#endif
		
	}
	
	/*namespace Screen { 
		void GetPxColor(unsigned char rgb[3], int x, int y){
			HDC dc = GetDC(NULL); DWORD hex = GetPixel(dc, x, y); ReleaseDC(NULL, dc);
			rgb[0] = GetRValue(hex); rgb[1] = GetGValue(hex); rgb[2] = GetBValue(hex); }
	}*/
}

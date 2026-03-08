#if !defined(_WIN32) && (defined(USE_GUI) || defined(USE_GUI_MIN))
#define Window XWindowID
#include <X11/extensions/XTest.h> //X11 Extensions
#undef Window
#endif
namespace ncpp{
	namespace Mouse {
	#ifdef _WIN32
		void GetPos(int& x, int& y){ POINT m; GetCursorPos(&m); x=m.x; y=m.y; }
		bool SetPos(int x, int y){ return SetCursorPos(x, y); }
		void Click(){ mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); }
		void RClick(){ mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0); }
		void MClick(){ mouse_event(MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0); }
	#elif defined(USE_GUI) || defined(USE_GUI_MIN) //X11
		void GetPos(int& x, int& y){ Display* display = XOpenDisplay(NULL); if(!display){ x = -1; y = -1; return; }
			int win_x, win_y; unsigned int mask; XWindowID root_window, child_window;
			XQueryPointer(display, XDefaultRootWindow(display), &root_window, &child_window, &x, &y, &win_x, &win_y, &mask); XCloseDisplay(display); }
		//XTest.h
		bool SetPos(int x, int y){ Display* display = XOpenDisplay(NULL); if(!display) return false; 
			XTestFakeMotionEvent(display, -1, x, y, CurrentTime); XCloseDisplay(display); return true; }

		void Click(){ Display* display = XOpenDisplay(NULL); if(!display) return;
			XTestFakeButtonEvent(display, 1, True,  CurrentTime); XTestFakeButtonEvent(display, 1, False, CurrentTime); XCloseDisplay(display); }

		void RClick(){ Display* display = XOpenDisplay(NULL); if(!display) return;
			XTestFakeButtonEvent(display, 3, True,  CurrentTime); XTestFakeButtonEvent(display, 3, False, CurrentTime); XCloseDisplay(display); }
			
		void MClick(){ Display* display = XOpenDisplay(NULL); if(!display) return; 
			XTestFakeButtonEvent(display, 2, True,  CurrentTime); XTestFakeButtonEvent(display, 2, False, CurrentTime); XCloseDisplay(display); }
	#else //Only declarations
		void GetPos(int& x, int& y);
		bool SetPos(int x, int y);
		void Click();
		void RClick();
		void MClick();
	#endif
		
	}
	
	namespace Keyboard {
	#ifdef _WIN32
		void PressKey(char key){ keybd_event(key, 0,0,0); keybd_event(key, 0,KEYEVENTF_KEYUP,0); }
		bool isKeyDown(char key){ return (GetAsyncKeyState(key) & 0x8000) != 0; }
	#elif defined(USE_GUI) || defined(USE_GUI_MIN) //X11
		//TODO: Вывод: /dev/input/eventX; struct input_event; <linux/input.h>. libinput?
		//Ввод: Эмуляция через uinput?
		bool isKeyDown(char key){ Display* display = XOpenDisplay(NULL); if(!display) return false;
			char keys_return[32]; XQueryKeymap(display, keys_return); // Получаем состояние всех клавиш (битмап)
			KeySym keysym = XStringToKeysym(&key); KeyCode kc = XKeysymToKeycode(display, keysym);
			bool pressed = !!(keys_return[kc >> 3] & (1 << (kc & 7))); // Проверяем нужный бит в массиве из 32 байт
			XCloseDisplay(display); return pressed; }
		//XTest.h
		void PressKey(char key){ Display* display = XOpenDisplay(NULL); if(!display) return;
			KeySym keysym = XStringToKeysym(&key); if(keysym == NoSymbol){ XCloseDisplay(display); return; }
			unsigned int keycode = XKeysymToKeycode(display, keysym); if(keycode == 0){ XCloseDisplay(display); return; }
			XTestFakeKeyEvent(display, keycode, False, CurrentTime); XCloseDisplay(display); }
	#else
		bool isKeyDown(char key);
		void PressKey(char key);
	#endif
		
	}
	
	/*namespace Screen { 
		void GetPxColor(unsigned char rgb[3], int x, int y){
			HDC dc = GetDC(NULL); DWORD hex = GetPixel(dc, x, y); ReleaseDC(NULL, dc);
			rgb[0] = GetRValue(hex); rgb[1] = GetGValue(hex); rgb[2] = GetBValue(hex); }
	}*/
}

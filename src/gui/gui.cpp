namespace ncpp { namespace GUI { struct App; App* mainApp=NULL; struct Window; struct GLWindow;
#define DEF_HWND_X 50
#define DEF_HWND_Y 50
#define DEF_HWND_WIDTH 300
#define DEF_HWND_HEIGHT 200
struct WEvent { Window* wnd; int id; int key, x, y; String name;
	WEvent(Window* wnd1=NULL, unsigned char id1=0) : wnd(wnd1), id(id1), key(-1), x(0), y(0){};
	enum EvType { OTHER, CLICK, MOUSEUP, MOUSEMOVE, KEYDOWN, KEYUP };
	enum MouseBtn { LBTN, RBTN, MBTN };
	//String decode(unsigned char id);
};
typedef void (*HANDLE_FUNC)(const WEvent&);

struct WNDHandlerInfo { Window* wnd; HANDLE_FUNC handler; String event;
	WNDHandlerInfo() : wnd(NULL), handler(NULL){};
	WNDHandlerInfo(Window* wnd1, HANDLE_FUNC handler1, String event1) 
		: wnd(wnd1), handler(handler1), event(event1){}; };

char detectDE(){ //enum DEType { WINAPI, X11, WAYLAND }; //VEDROID? //CString session = getenv("XDG_SESSION_TYPE");
	#ifdef _WIN32
	return 0;
	#else
	if(getenv("WAYLAND_DISPLAY")) return 2; else return 1;
	#endif
} //struct WParams {}; //typedef WParams WOpts?
#define NCPP_VKEYS 300
#define NCPP_VKEYS_FN (NCPP_VKEYS+50)
int _FromNativeKeyID(int kid){
	#ifdef _WIN32
	if(kid >= 112 && kid <= 135) return NCPP_VKEYS_FN+(kid-112); //F1-F24 (диапазон 350-373)
	if(kid >= 96 && kid <= 105) return NCPP_VKEYS_FN+24+(kid-96); //Num 0-Num 9 (диапазон 374-383)
	switch(kid){
		case 27: return NCPP_VKEYS; //Escape
		case 192: return NCPP_VKEYS+1; // (`)
		case 9: return NCPP_VKEYS+2; //Tab
		case 20: return NCPP_VKEYS+3; //Caps Lock
		case 16: return NCPP_VKEYS+4; //LShift (160)
		//case 16: return NCPP_VKEYS+5; //RShift (161)
		case 17: return NCPP_VKEYS+6; //LCtrl (162)
		//case 17: return NCPP_VKEYS+7; //RCtrl (163)
		case 18: return NCPP_VKEYS+8; //LAlt (164)
		//case 18: return NCPP_VKEYS+9; //RAlt (165)
		case 91: return NCPP_VKEYS+10; //Win Key
		case 93: return NCPP_VKEYS+11; //Menu Key
		case 189: return NCPP_VKEYS+12; // (-) Minus
        case 187: return NCPP_VKEYS+13; // (=) Plus/Equal
        case 8:   return NCPP_VKEYS+14; // Backspace
        case 219: return NCPP_VKEYS+15; // ([)
        case 221: return NCPP_VKEYS+16; // (])
        case 220: return NCPP_VKEYS+17; // (\)
        case 186: return NCPP_VKEYS+18; // (;)
        case 222: return NCPP_VKEYS+19; // (')
        case 13:  return NCPP_VKEYS+20; // Enter
        case 188: return NCPP_VKEYS+21; // (,)
        case 190: return NCPP_VKEYS+22; // (.)
        case 191: return NCPP_VKEYS+23; // (/)
        case 45:  return NCPP_VKEYS+24; // Insert
        case 46:  return NCPP_VKEYS+25; // Delete
        case 145: return NCPP_VKEYS+26; // Scroll Lock
        case 36:  return NCPP_VKEYS+27; // Home
        case 35:  return NCPP_VKEYS+28; // End
        case 19:  return NCPP_VKEYS+29; // Pause
        case 33:  return NCPP_VKEYS+30; // Page Up
        case 34:  return NCPP_VKEYS+31; // Page Down
		//case 32: return NCPP_VKEYS+32; //Space
        case 144: return NCPP_VKEYS+33; // Num Lock
        case 38:  return NCPP_VKEYS+34; // Up
        case 40:  return NCPP_VKEYS+35; // Down
        case 37:  return NCPP_VKEYS+36; // Left
        case 39:  return NCPP_VKEYS+37; // Right
		case 107: return NCPP_VKEYS_FN+34; // Num +
		case 109: return NCPP_VKEYS_FN+35; // Num -
		case 106: return NCPP_VKEYS_FN+36; // Num *
		case 111: return NCPP_VKEYS_FN+37; // Num /
		case 110: return NCPP_VKEYS_FN+38; // Num . (Del)
	}
	#else //X11
	if(kid >= 65470 && kid <= 65493) return NCPP_VKEYS_FN+(kid-65470); //F1-F24 (диапазон 350-373)
	switch(kid){ 
		case 65307: return NCPP_VKEYS; //Escape
		case 96: return NCPP_VKEYS+1; // (`)
		case 65289: return NCPP_VKEYS+2; //Tab
		case 65509: return NCPP_VKEYS+3; //Caps Lock
		case 65505: return NCPP_VKEYS+4; //LShift
		case 65506: return NCPP_VKEYS+5; //RShift
		case 65507: return NCPP_VKEYS+6; //LCtrl
		case 65508: return NCPP_VKEYS+7; //RCtrl
		case 65513: return NCPP_VKEYS+8; //LAlt
		case 65514: return NCPP_VKEYS+9; //RAlt
		case 65515: return NCPP_VKEYS+10; //Win Key
		case 65383: return NCPP_VKEYS+11; //Menu Key
		case 45: return NCPP_VKEYS+12; //  (-)
		case 61: return NCPP_VKEYS+13; // (=)
		case 65288: return NCPP_VKEYS+14; // Backspace
		case 91: return NCPP_VKEYS+15; // ([)
		case 93: return NCPP_VKEYS+16; // (])
		case 92: return NCPP_VKEYS+17; // (\)
		case 59: return NCPP_VKEYS+18; // (;)
		case 39: return NCPP_VKEYS+19; // (')
		case 65293: case 65421: return NCPP_VKEYS+20; //Enter
		case 44: return NCPP_VKEYS+21; // (,)
		case 46: return NCPP_VKEYS+22; // (.)
		case 47: return NCPP_VKEYS+23; // (/)
		case 65379: return NCPP_VKEYS+24; // Insert (0xFF63)
		case 65535: return NCPP_VKEYS+25; // Delete (0xFFFF)
		case 65300: return NCPP_VKEYS+26; // Scroll Lock
		case 65360: return NCPP_VKEYS+27; // Home
		case 65367: return NCPP_VKEYS+28; // End
		case 65299: return NCPP_VKEYS+29; // Pause
		case 65365: return NCPP_VKEYS+30; // Page Up
		case 65366: return NCPP_VKEYS+31; // Page Down
		//case 32: return NCPP_VKEYS+32; //Space
		case 65407: return NCPP_VKEYS+33; // Num Lock
		case 65362: return NCPP_VKEYS+34; // Up
		case 65364: return NCPP_VKEYS+35; // Down
		case 65361: return NCPP_VKEYS+36; // Left
		case 65363: return NCPP_VKEYS+37; // Right 
		
		case 65438: return NCPP_VKEYS_FN+24; //Num 0 
		case 65436: return NCPP_VKEYS_FN+25; // Num 1
		case 65433: return NCPP_VKEYS_FN+26; // Num 2
		case 65435: return NCPP_VKEYS_FN+27; // Num 3
		case 65430: return NCPP_VKEYS_FN+28; // Num 4
		case 65437: return NCPP_VKEYS_FN+29; // Num 5
		case 65432: return NCPP_VKEYS_FN+30; // Num 6
		case 65429: return NCPP_VKEYS_FN+31; // Num 7
		case 65431: return NCPP_VKEYS_FN+32; // Num 8
		case 65434: return NCPP_VKEYS_FN+33; // Num 9
		case 65451: return NCPP_VKEYS_FN+34; // Num +
		case 65453: return NCPP_VKEYS_FN+35; // Num -
		case 65450: return NCPP_VKEYS_FN+36; // Num *
		case 65455: return NCPP_VKEYS_FN+37; // Num /
		case 65439: return NCPP_VKEYS_FN+38; // Num . (Del)
		
	}
	#endif
	return kid; 
}
	String NKeyDecode(int key){ if(key == 32) return "Space";
		if(key >= 'A' && key <= 'Z') return String((char*)&key, 1); if(key >= '0' && key <= '9') return String((char*)&key, 1);
		if(key >= NCPP_VKEYS_FN && key <= NCPP_VKEYS_FN+23){ return "F"+dtos(key-NCPP_VKEYS_FN+1); }
		if(key >= NCPP_VKEYS_FN+24 && key <= NCPP_VKEYS_FN+33){ return "Num "+dtos(key-(NCPP_VKEYS_FN+24)); }
		switch(key){ case NCPP_VKEYS: return "Escape";
			case NCPP_VKEYS+1: return "`"; case NCPP_VKEYS+2:  return "Tab"; case NCPP_VKEYS+3: return "Caps Lock"; 
			case NCPP_VKEYS+4: return "LShift"; case NCPP_VKEYS+5: return "RShift"; case NCPP_VKEYS+6: return "LCtrl";
			case NCPP_VKEYS+7: return "RCtrl"; case NCPP_VKEYS+8: return "LAlt"; case NCPP_VKEYS+9: return "RAlt"; 
			case NCPP_VKEYS+10: return "Win Key"; case NCPP_VKEYS+11: return "Menu Key"; case NCPP_VKEYS+12: return "-"; 
			case NCPP_VKEYS+13: return "="; case NCPP_VKEYS+14: return "Backspace"; case NCPP_VKEYS+15: return "["; 
			case NCPP_VKEYS+16: return "]"; case NCPP_VKEYS+17: return "\\"; case NCPP_VKEYS+18: return ";";
			case NCPP_VKEYS+19: return "'"; case NCPP_VKEYS+20: return "Enter"; case NCPP_VKEYS+21: return ","; 
			case NCPP_VKEYS+22: return "."; case NCPP_VKEYS+23: return "/"; case NCPP_VKEYS+24: return "Insert"; 
			case NCPP_VKEYS+25: return "Delete"; case NCPP_VKEYS+26: return "Scroll Lock"; case NCPP_VKEYS+27: return "Home";
			case NCPP_VKEYS+28: return "End"; case NCPP_VKEYS+29: return "Pause"; case NCPP_VKEYS+30: return "Page Up";
			case NCPP_VKEYS+31: return "Page Down"; case NCPP_VKEYS+33: return "Num Lock"; case NCPP_VKEYS+34: return "Up"; 
			case NCPP_VKEYS+35: return "Down"; case NCPP_VKEYS+36: return "Left"; case NCPP_VKEYS+37: return "Right"; 
			case NCPP_VKEYS_FN+34: return "Num +"; case NCPP_VKEYS_FN+35: return "Num -"; case NCPP_VKEYS_FN+36: return "Num *"; 
			case NCPP_VKEYS_FN+37: return "Num /"; case NCPP_VKEYS_FN+38: return "Num . (Del)"; } return "(Unknown)"; }
} }

// - **Поддержка графики:**
// - - [x] OpenGL 2.0+
// - - [ ] Vulkan
// - - [x] GDI/GDI+ (частично)
// - - [x] X11 (частично)
// - - [ ] Wayland

#ifdef _WIN32
#include "winapi.cpp"

#elif USE_WAYLAND //XDG Portals?
#include <wayland-client.h>

#else //USE_X11_ONLY? Dependencies:
//X11 needs packages: libx11-dev libxtst-dev
//OpenGL needs packages: libgl1-mesa-dev
#define Window XWindowID //Подмена Window для устранения конфликта имен.
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#undef Window
#include "x11.cpp"

#endif

#ifdef USE_VULKAN
#include "../experimental/gl/vulkan.cpp"
#elif !defined(NOUSE_GL)	
#include "opengl-loader.cpp"
#include "opengl.cpp"
#endif

//#include "../experimental/gl/gl-math.cpp"
//#include "../experimental/gl/images.cpp"

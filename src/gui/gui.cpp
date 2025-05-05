namespace ncpp { namespace GUI {
#define DEF_HWND_X 50
#define DEF_HWND_Y 50
#define DEF_HWND_WIDTH 300
#define DEF_HWND_HEIGHT 200
#define HANDLER_PTR void (*handler)(std::string type)
typedef std::pair<std::string, void (*)(std::string)> HANDLER_PAIR_TYPE;
//enum { X11, WAYLAND } GUI_ENGINE;
#ifndef _WIN32
std::string get_session_type(){ //x11, wayland
    std::string session = std::getenv("XDG_SESSION_TYPE");
    if(session == "x11"){}
    else if(session == "wayland"){} return session; }
#endif
struct WParams {};
//typedef WParams WOpts?
}}

#ifdef _WIN32
#include "winapi.cpp"

#elif USE_WAYLAND 
#include <wayland-client.h>

#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "x11.cpp"

#endif

#include "opengl.cpp"
#include "images.cpp"

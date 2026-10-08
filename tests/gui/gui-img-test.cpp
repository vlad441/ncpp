#ifndef USE_EXPERIMENTAL
#define USE_EXPERIMENTAL
#endif
#include "../../src/ncpp.cpp"
using namespace ncpp;
using namespace ncpp::GUI;

#include "../tests-header.h"
Console cons;

App app; Button* g_btn1; 
Window g_wnd("Img test app", 100, 100, 400, 300); 
Window area1(&g_wnd, "Image area", 5, 60, 150, 150);

void buttonClick(const WEvent& ev){ cons << "Btn event: " << ev.name << "\n"; 
	const Image& img = Image::fromFile("../../index"); g_wnd.putImage(img.px.data(), img.w, img.h); //area1.putImage(img.px.data(), img.w, img.h); 
}

int main(){ g_btn1 = new Button(&g_wnd, "loagImg", 10, 25); g_btn1->setHandler(buttonClick);
	int wndW=0, wndH=0; g_wnd.getSize(wndW, wndH);
	print("Wnd sz:"); print(wndW); print(";"); print(wndH); print("\n");
	app.run(); }
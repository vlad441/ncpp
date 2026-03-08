#include "../../src/ncpp.cpp"
using namespace ncpp;
using namespace ncpp::GUI;

#include "../tests-header.h"
Console cons;

App app;

void buttonClick(const WEvent& ev){ cons << "Btn event: " << ev.name << "\n"; }
void wndEvent(const WEvent& ev){ if(ev.id==WEvent::KEYDOWN){ cons << "VKEY: " << ev.key << " | " << NKeyDecode(ev.key) << "\n"; }
	else if(ev.id==WEvent::CLICK){ cons << "click: (" << ev.x << ", " << ev.y << ") | " << ev.key << "\n"; }
	else{ cons << "Wnd event: " << ev.id << " | " << ev.key << " | " << ev.name << "\n"; } }
void glwndEvent(const WEvent& ev){ cons << "GLWnd event: " << ev.name << "\n"; }

/*void OpenGl_ArcTest(GLWindow& glwnd){ for(int endAngle=100;endAngle<=450;endAngle+=10){ GL::glArc(100, 100, 30, 90, endAngle, 1); glwnd.draw(); Sleep(50); } }

void OpenGl_Test(){ Window wnd1("Wasso"); wnd1.setHandler(wndEvent);	
	Button btn1(&wnd1, "battons1");
	btn1.setTitle("btn1"); btn1.setHandler(buttonClick);
	GLWindow glwnd(&app, "OpenGL Test Window", 600, 100, 600, 600); glwnd.setContext(); glwnd.setHandler(glwndEvent);
    int width=0, height=0; glwnd.getSize(width, height); cons << "Window size0: " << width << "; " << height << "\n";
	//glwnd.resize(400,400); glwnd.setPos(800,200); glwnd.getSize(width, height); cons << "Window size1: " << width << "; " << height << "\n";

	//glwnd.resetContext(); glwnd.setContext();
	glwnd.setMatrix2DPreset();  
	GL::clear(0,0,0); OpenGl_ArcTest(glwnd); GL::glArc(0.6f, 0.6f, 0.2f);
	// Рисуем что-то, например, треугольник
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); // Красный
	glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0f, 1.0f, 0.0f); // Зеленый
	glVertex2f(0.5f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f); // Синий
	glVertex2f(0.0f, 0.5f);
	glEnd();
 
	glwnd.swapBuffers(); // Меняем буферы для отображения результата
	app.run(); }*/
	
Button* g_btn1; Label* g_label; Input* g_inp1;
void btn1Click(const WEvent& ev){ cons << "Btn1 event: " << ev.name << " | Edited." << "\n"; 
	g_label->setTitle(g_inp1->getText()); }
void btn2Click(const WEvent& ev){ cons << "Btn2 event: " << ev.name << " | Getted." << "\n"; 
	g_inp1->setText(g_label->getText()); }
void inp1Event(const WEvent& ev){ cons << "inp event: " << ev.name << "\n"; }
void chk1Click(const WEvent& ev){ CheckBox* chk=(CheckBox*)ev.wnd; cons << "chk1 event: " << ev.name << " | Checked: " << chk->isChecked() << "\n";
	g_btn1->setDisabled(chk->isChecked()); }
	
void Dealult_Test(){
	Window wnd1("Wasso App1", 100, 100, 400, 300);
	g_label = new Label(&wnd1, "Label1"); 
	g_inp1 = new Input(&wnd1, "Texts1_", 100, 0); g_inp1->setHandler(inp1Event);
	
	g_btn1 = new Button(&wnd1, "setLabel", 10, 25); g_btn1->setHandler(btn1Click);
	Button btn2(&wnd1, "getLabel", 115, 25); btn2.setHandler(btn2Click);
	TextArea area1(&wnd1, "TextArea text example", 5, 60); area1.setHandler(inp1Event);
	CheckBox chk1(&wnd1, "Disable btn1", 210, 60); chk1.setHandler(chk1Click);
	app.run();
}

int main(){
	//OpenGl_Test(); 
	Dealult_Test();
	return 0; }
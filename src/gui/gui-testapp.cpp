#include <iostream>
#include "ncpp-test.h"
#include "gui.cpp"
#include <GL/gl.h>
using namespace ncpp;
using namespace ncpp::GUI;

App app = App(); 

void OpenGl_ArcTest(GLWindow& glwnd){ for(int endAngle=100;endAngle<=450;endAngle+=10){ GL::glArc(100, 100, 30, 90, endAngle, 1); glwnd.draw(); Sleep(50); } }

void buttonClick(std::string event){ if(true){ std::cout << "Btn event: " << event << std::endl; } }

void OpenGl_Test(){ 
	GUI::Window wnd1("Wasso"); Button btn1(&wnd1, "battons1");
	btn1.setTitle("btn1"); btn1.onEvent("", buttonClick);
	GLWindow glwnd(&app, "OpenGL Test Window", 600, 100, 600, 600);
    int width=0, height=0; glwnd.getSize(width, height); std::cout << "Window size0: " << width << "; " << height << "\n";
	//glwnd.resize(400,400); glwnd.setPos(800,200); glwnd.getSize(width, height); std::cout << "Window size1: " << width << "; " << height << "\n";

	//glwnd.resetContext(); glwnd.setContext();
	glwnd.setOrthoStyle();  
	GL::clear(); OpenGl_ArcTest(glwnd); GL::glArc(0.6f, 0.6f, 0.2f);
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
	app.run(); }
	
Button* g_btn1; Label* g_label; Input* g_inp1; CheckBox* g_chk1;
void btn1Click(std::string event){ std::cout << "Btn1 event: " << event << " | Edited." << std::endl; 
	g_label->setTitle(g_inp1->getText()); }
void btn2Click(std::string event){ std::cout << "Btn2 event: " << event << " | Getted." << std::endl; 
	g_inp1->setText(g_label->getText()); }
void inp1Event(std::string event){ std::cout << "inp event: " << event << std::endl; }
void chk1Click(std::string event){ std::cout << "chk1 event: " << event << " | Checked: " << g_chk1->isChecked() << std::endl; 
	g_btn1->setDisabled(g_chk1->isChecked()); }
	
void Dealult_Test(){ 
	GUI::Window wnd1("Wasso App1", 100, 100, 400, 300);
	g_label = new Label(&wnd1, "Label1"); 
	g_inp1 = new Input(&wnd1, "Texts1_", 100, 0); g_inp1->onEvent("", inp1Event);
	
	g_btn1 = new Button(&wnd1, "setLabel", 10, 25); g_btn1->onEvent("", btn1Click);
	Button btn2(&wnd1, "getLabel", 115, 25); btn2.onEvent("", btn2Click);
	TextArea area1(&wnd1, "TextArea text example", 5, 60); area1.onEvent("", inp1Event);
	g_chk1 = new CheckBox(&wnd1, "Disable btn1", 210, 60); g_chk1->onEvent("", chk1Click);
	app.run();
}

void Styles_Test(){ 
	GUI::Window wnd1("Wasso App1", 100, 100, 400, 300);
	g_label = new Label(&wnd1, "Label1"); 
	g_inp1 = new Input(&wnd1, "Texts1_", 100, 0); g_inp1->onEvent("", inp1Event);
	
	g_btn1 = new Button(&wnd1, "setLabel", 10, 25); g_btn1->onEvent("", btn1Click);
	Button btn2(&wnd1, "getLabel", 115, 25); btn2.onEvent("", btn2Click);
	TextArea area1(&wnd1, "TextArea text example", 5, 60); area1.onEvent("", inp1Event);
	g_chk1 = new CheckBox(&wnd1, "Disable btn1", 210, 60); g_chk1->onEvent("", chk1Click);
	app.run();
}

int main(){
	//OpenGl_Test(); 
	Dealult_Test();
	return 0; }

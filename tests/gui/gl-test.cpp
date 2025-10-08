#include "../../src/ncpp.cpp"
using namespace ncpp;
using namespace ncpp::GUI;

#include "../tests-header.h"
Console cons;

void Timer1s(){};

void OpenGl_Test(GLWindow& glwnd){ 
	print("GL::InitDefautShaders: "); print(GL::InitDefautShaders()); print("\n");
	print("glCreateShader: "); print((int)glCreateShader); print("\n");

	GL::clear();
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
}

int main(){ cpp_version(); gcc_version(); ncpp_version();
	App app; GLWindow glwnd(&app, "OpenGL Test Window", 600, 100, 600, 600);
	int width=0, height=0; glwnd.getSize(width, height); cons << "Window size0: " << width << "; " << height << "\n";
	//glwnd.resize(400,400); glwnd.setPos(800,200); glwnd.getSize(width, height); cons << "Window size1: " << width << "; " << height << "\n";

	//glwnd.resetContext(); glwnd.setContext();
	//glwnd.setMatrix2DPreset();
	OpenGl_Test(glwnd);
	
	//setInterval(Timer1s, 1000); Timers::wait();
	app.run();
	return 0; }

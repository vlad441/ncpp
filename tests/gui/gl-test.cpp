#include "../../src/ncpp.cpp"
#include "../../src/experimental/gl/gl-engine.cpp"
using namespace ncpp;
using namespace ncpp::GUI;
using namespace ncpp::GL;
#include "../tests-header.h"

App app; GLWindow glwnd;
Matrix4 projection; Matrix4 view;
int FPS = 60;
	
void onKeyPress(const WEvent& ev){ cons << "KEY: " << ev.key << " | " << NKeyDecode(ev.key) << "\n"; 
	static int delta=5;
	switch(ev.key){ 
		case 82: { view.identity(); break; } //R
		case 313: { delta++; break; } //+
		case 312: { delta--; break; } //-
		case 87: { view.translate(0,delta,0); break; } //W
		case 65: { view.translate(-delta,0,0); break; } //A
		case 83: { view.translate(0,-delta,0); break; } //S
		case 68: { view.translate(delta,0,0); break; } //D
		
	} }

void onKeyUp(const WEvent& ev){ cons << "KEYUP: " << ev.key << " | " << NKeyDecode(ev.key) << "\n"; }

void onMouseEvent(const WEvent& ev){ cons << ev.name << ": (" << ev.x << ", " << ev.y << ")" << "\n"; }

void glEvent(const WEvent& ev){ 
	switch(ev.id){ case WEvent::KEYDOWN: onKeyPress(ev); break; case WEvent::KEYUP: onKeyUp(ev); break;
		case WEvent::CLICK: case WEvent::MOUSEUP: onMouseEvent(ev); break; } }
	
void GPURender(){ while(app.nextEvent()); //Обработать события окон
    Array<Vertex> vertices; // 1. Создаем треугольник (координаты в пикселях, раз у нас Ortho)
    vertices.push(Vertex(150.0f, 50.0f, 0.0f, 0xFF0000));  // Красный
	vertices.push(Vertex(50.0f, 200.0f, 0.0f, 0x00FF00));  // Зеленый
	vertices.push(Vertex(250.0f, 200.0f, 0.0f, 0x0000FF)); // Синий

    Matrix4 model; Matrix4 mvp = projection*view*model;

    GL::clear(0x222222); DrawTriangles(vertices, mvp);
	DrawString("A",0,0); 
}

void GLRenderFunc(){ glwnd.setContext(); glwnd.setHandler(glEvent);
	int width=0,height=0; glwnd.getSize(width,height); cons << "GLWindow size0: " << width << "; " << height << "\n";
	//glwnd.resize(400,400); glwnd.setPos(800,200); glwnd.getSize(width, height); cons << "Window size1: " << width << "; " << height << "\n";	
	bool loaded = GL::InitDefaultShaders();
	cons << "GL::InitDefaultShaders: " << loaded << "\n"; if(!loaded){ exit(0); }
	cons << "glCreateShader: " << (size_t)glCreateShader << "\n";
	cons << "glDrawArrays: " << (size_t)glDrawArrays << "\n";
	cons << "OpenGLVersion(): " << GL::OpenGLVersion() << "\n";
	cons << "sizeof(Vertex): " << sizeof(Vertex) << "\n";
	
	projection.setMatrix2DPreset(0, width, height);
	
	GL::clear(0x000099); glEnable(GL_BLEND);
	//GPURender(); glwnd.swapBuffers(); Sleep(500);
	while(true){ GPURender(); glwnd.swapBuffers(); if(FPS>0) Sleep(1000/FPS); } 
}

int main(){ cpp_version(); gcc_version(); ncpp_version(); print("\n");
	move(glwnd, GLWindow(&app, "OpenGL Test Window", 600, 100, 600, 600));
	
	GLRenderFunc();
	//glwnd.resetContext(); Thread renderThr(GLRenderFunc); renderThr.detach();
	//app.run();
	return 0; }
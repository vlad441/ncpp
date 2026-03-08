#include "../../src/ncpp.cpp"
#include "../../src/experimental/gl/gl-engine.cpp"
using namespace ncpp;
using namespace ncpp::GUI;
using namespace ncpp::GL;
#include "../tests-header.h"

App app; GLWindow glwnd; int FPS = 60;

Matrix4 projection; Camera camera(0, 0, 50.0);
Console cons; GConsole gcons;
bool fullscreen=false;
bool showInfo=true;

volatile int fpscnt=0; int fps=0; void fpsCalc(){ fps=fpscnt; fpscnt=0; }

float aspect=1;
void updateProjection(int w=0, int h=0){ if(w==0&&h==0){ glwnd.getSize(w,h); } aspect=(float)w/(float)h; projection.perspective(60.0f, aspect); }

Matrix4 CalcPerspectiveMatrix(){ projection.perspective(60.0f, 1920.0f/1080.0f, 0.1f, 1000.0f);
	// 2. Создаем камеру (откатываемся назад на 5 единиц)
	Matrix4 view; view.translate(0,0,-5.0f); // Перенос по Z в Column-Major (двигаем мир от себя)
	//view.lookAt(Vector3(0,0,-5.0f), 0, 0);
	Matrix4 model; // 3. Создаем модель (позиция и поворот объекта)
	Matrix4 mvp = projection * view * model; return mvp; } // Итоговая матрица MVP (Model-View-Projection) (Умножаем справа налево)
	
void onKeyPress(const WEvent& ev){ cons << "KEY: " << ev.key << " | " << NKeyDecode(ev.key) << " | keychar: " << String().push(ev.x) << "\n"; if(gcons.keysHandle(ev)) return;
	if(camera.Controls_FreeCam(ev)) return;
	switch(ev.key){
		//case 82: { view.identity(); break; } //R
		case 300: camera.mouseCapt = (camera.mouseCapt==0)?1:0; glwnd.hideCursor(camera.mouseCapt); updateProjection(); break; //Esc
		case 352: showInfo = !showInfo; break; //F3
		case 361: fullscreen = !fullscreen; glwnd.setFullscreen(fullscreen); updateProjection(); break; //F12
		case 85: { updateProjection(); break; } //U: updateProjection
	}
}

void onKeyUp(const WEvent& ev){ cons << "KEYUP: " << ev.key << " | " << NKeyDecode(ev.key) << "\n"; }

void onMouseEvent(const WEvent& ev){ cons << ev.name << ": (" << ev.x << ", " << ev.y << ")" << "\n"; }

void glEvent(const WEvent& ev){ switch(ev.id){
		case WEvent::KEYDOWN: onKeyPress(ev); break; case WEvent::KEYUP: onKeyUp(ev); break;
		case WEvent::CLICK: case WEvent::MOUSEUP: onMouseEvent(ev); break; } }
		
void onConsoleCmd(const String& cmdtxt){ Array<String> cmd = cmdtxt.split(" ");
	if(cmdtxt=="help"){ gcons.clear(); gcons << " Supported commands: \n"
		"cls/clear - clear area\n" "echo - echo text in area\n" "tp - teleport\n" "show ... - show commands\n" "set ... - set commands\n"; }
	if(cmd[0]=="show"){ if(cmd.size()<2||cmd[1]=="help"){ gcons << " show commands: \n" "show fps - show fps limit\n"; return; }
		if(cmd[1]=="fps"){ gcons << "fps limit: " << FPS << "\n"; return; }
	}
	if(cmd[0]=="set"){ if(cmd.size()<2||cmd[1]=="help"){ gcons << " set commands: \n" "set fps - set fps limit\n"; return; }
		if(cmd[1]=="fps"){ if(cmd.size()<3||cmd[1]=="help"){ gcons << " Syntax: set fps [fps]\n"; return; } FPS=stoin(cmd[2]); gcons << "New fps limit: " << FPS << "\n"; return; }
	}
	if(cmd[0]=="tp"){ if(cmd.size()<4||cmd[1]=="help"){ gcons << " Syntax: tp [x] [y] [z]\n"; return; }
		camera.setPos(stolln(cmd[1]), stolln(cmd[2]), stolln(cmd[3])); gcons << "Teleported to " << cmd.slice(1).join(" ") << "\n"; }
	
	GConsole::defCmdHandle(cmdtxt, gcons); }
	
void drawSetka(const Matrix4& mvp=Matrix4().setMatrix2DPreset()){ Array<Vertex> v; float len=50.0f; unsigned int color = 0xFF00FF00;
	v.push(Vertex(0,0,0,color)); v.push(Vertex(len,0,0,color));  
	v.push(Vertex(0,0,0,color)); v.push(Vertex(0,len,0,color)); 
	v.push(Vertex(0,0,0,color)); v.push(Vertex(0,0,len,color)); RenderLines(v, mvp); }
	
void drawInfo(){ if(!showInfo) return; DrawString("FPS: "+dtos(fps)+"\n"
		"X: "+dtos(camera.x, 2)+" | Y: "+dtos(camera.y, 2)+" | Z: "+dtos(camera.z, 2)+" - "
		"aX: "+dtos(camera.aX, 2)+" | aY: "+dtos(camera.aY, 2)+" | aZ: "+dtos(camera.aZ, 2)+"\n"
		"ncpp powered",0.005f,0.90f, 0.015f, 0xFFFFFF, aspect); }
		
Model rect, cube, cola;
	
void GPURender(){ fpscnt++; while(app.nextEvent()); //Обработать события окон
    Array<Vertex> v; // 1. Создаем треугольник (координаты в пикселях, раз у нас Ortho)
    v.push(Vertex( 15.0f,  5.0f, 0.0f, 0xFFFF0000)); // Красный
	v.push(Vertex(  5.0f, 20.0f, 0.0f, 0xFF00FF00)); // Зеленый
	v.push(Vertex( 25.0f, 20.0f, 0.0f, 0xFF0000FF)); // Синий
	int width=0,height=0; glwnd.getSize(width,height); glViewport(0, 0, width, height); updateProjection(width, height);
	
	camera.captureMouse(glwnd);
	Matrix4 view=camera.getMatrix();
	Matrix4 vp = projection*view; //Matrix4 mvp = projection*view*model;
	Matrix4 model; Matrix4 mvp = vp*model;

    GL::clear(0x222222); RenderTriangles(v, mvp); drawSetka(mvp); 
	drawInfo(); gcons.render(aspect);
	
	rect.rotate(1, 0, 0); rect.render(vp); cube.rotate(1, 1, 1); cube.render(vp); //cola.render(vp);

	/*DrawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890\n" 
		"abcdefghijklmnopqrstuvwxyz ?\n"
		"!\"#$%&'()*+,-./ :;<=>?@ [\\]^_` {|}~\n"
		"FPS: "+dtos(fps)+"\n"
		"NCPP POWERED",0,0.5f, 0.05f, 0xFFFFFF, aspect);*/
}

void GLInitFunc(){ glwnd.setContext(); glwnd.setHandler(glEvent);
	int width=0,height=0; glwnd.getSize(width,height); cons << "GLWindow size0: " << width << "; " << height << "\n";
	//glwnd.resize(400,400); glwnd.setPos(800,200); glwnd.getSize(width, height); cons << "Window size1: " << width << "; " << height << "\n";	
	bool loaded = GL::InitDefaultShaders();
	cons << "GL::InitDefaultShaders: " << loaded << "\n"; if(!loaded){ exit(1); }
	cons << "[OpenGL String]: " << GL::OGLVersion() << "\n"; int oglv[2]; GL::OGLVersion(oglv);
	cons << "[OpenGL Version]: " << oglv[0] << "." << oglv[1] << "\n";
	
	updateProjection(); glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);
	
	addRect(rect.v, 10, 5, 0xFFFFFFFF, 0, 0);
	addRect(rect.l, 10, 5, 0xFFFF0000, 0, 0, false); //border
	
	addCube(cube.v, cube.idx, 5, 5, 5, 0xFFFFFFFF, -20, 10, -20, true, true);
	addCube(cube.l, cube.idx, 5, 5, 5, 0xFFFF0000, -20, 10, -20, false, true, false); //border
	//cube.setPos(-20, 10, -20);
	
	//cola.loadOBJ("cola.obj");
	//cons << "cola vertices: " << cola.v.size() << "\ncola indices: " << cola.idx.size() << "\n";
	
	GL::clear(0x000099); glEnable(GL_BLEND); setInterval(fpsCalc, 1000);
	while(true){ GPURender(); glwnd.swapBuffers(); if(FPS>0) Sleep(1000/FPS); }
}

int main(){ cpp_version(); gcc_version(); ncpp_version(); print("\n");
	gcons.onCommand=onConsoleCmd;
	glwnd.steal(GLWindow(&app, "OpenGL Perspective Test", 600, 100, 600, 600)); GLInitFunc(); return 0; }
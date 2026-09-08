#ifndef USE_EXPERIMENTAL
#define USE_EXPERIMENTAL
#endif
#define NCPP_GL_VERTEX_EXT
//#include "src/experimental/gl/gl-engine.cpp"
#include "../../src/ncpp.cpp"
using namespace ncpp;
using namespace ncpp::GUI;
using namespace ncpp::GL;
#include "../tests-header.h"

#ifdef _WIN32
#define TEST_LOAD_TEXTURES
#endif

struct GLApp { GLApp(App* _app=NULL) : app(_app), camera(0, 5, 50.0){ FPS=0; FPSLim=60; ticks=25; fullscreen=false; }
	int FPS, FPSLim; int ticks; bool fullscreen; enum ProjectionType { PERSPECTIVE, ORTHO };
	App* app; GLWindow glwnd; Matrix4 projection; Camera camera;
	
	void (*GPURender)();
	void (*CPULogic)();
	//void (*glEvent)(const WEvent& ev);
	//void _glEvent(const WEvent& ev);
	GLWindow& getGLWnd(){ return glwnd; }
	
	void setFPS(int fps){ FPSLim=fps; }
	void setTicks(int _ticks){ ticks=_ticks; }
	void setFullscreen(bool isfull){ fullscreen=isfull; glwnd.setFullscreen(fullscreen); }
	void toogleFullscreen(){ fullscreen=!fullscreen; glwnd.setFullscreen(fullscreen); }
	
	void createWindow(const GLWindow& wnd){ glwnd.steal(wnd); }
	void setHandler(GUI::HANDLE_FUNC handler){ glwnd.setHandler(handler); }
	
	void updateProjection(int w=0, int h=0){ if(w==0&&h==0){ glwnd.recalcWndSize(); } projection.perspective(camera.angleFog, glwnd.aspect); }
	
	bool init(){ glwnd.setContext(); bool loaded = GL::InitDefaultShaders(); if(!loaded) return false;
		updateProjection(); glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS); glEnable(GL_BLEND); return true; }
		
	int getFPSLimitMs(){ int sleepMs = 1000/FPSLim;
		#ifdef _WIN32
		if(FPSLim>=30) sleepMs/=2; //Сраное квантование Sleep() винды ~9-15 мсек
		#endif
		return sleepMs>=1?sleepMs:1; }
	
	void run(){ if(!app){ print("(!) GLApp -> App* app = NULL\n"); exit(1); }
		if(!glwnd.hasContext()){ print("(!) GLApp -> GLWindow: glContext not created\n"); exit(1); }
		if(!GPURender){ print("(!) GLApp -> GPURender: handler has not set\n"); exit(1); }
		//if(!CPULogic){ print("(!) GLApp -> CPULogic: handler has not set\n"); exit(1); } //timeBeginPeriod(1);
		performance::start(); double oldTime = performance::now(); double accumulator = 0.0;
		
		while(true){ double nowTime=performance::now(); accumulator+=nowTime-oldTime; oldTime=nowTime; int tickTime = 1000/ticks;
			while(app->nextEvent()); //Обработать события окон
			if(CPULogic){ while(accumulator>=tickTime){ CPULogic(); accumulator-=tickTime; } }
			//double frameStart=performance::now();
			GPURender(); glwnd.swapBuffers(); //double frameMs=performance::now()-frameStart;
			if(FPSLim>0) Sleep(getFPSLimitMs()); }
	}
	//volatile int _fpscnt; void fpsCalc(){ FPS=_fpscnt; _fpscnt=0; }
	
};

App app; GLApp glApp(&app); GLWindow& glwnd = glApp.glwnd; GConsole gcons(&glwnd); Console cons; bool showInfo=true;

Model rect, arc, hexagon, cube, sphere, cylinder, cylinder1, cone, cone1, cola;
Entity rectE, cubeE;

Texture tyan;

void drawSetka(const Matrix4& mvp=Matrix4().setMatrix2DPreset()){ Array<Vertex> v; float len=50.0f; unsigned int color = 0xFF00FF00;
	v.push(Vertex(0,0,0,color)); v.push(Vertex(len,0,0,color));  
	v.push(Vertex(0,0,0,color)); v.push(Vertex(0,len,0,color)); 
	v.push(Vertex(0,0,0,color)); v.push(Vertex(0,0,len,color)); RenderLines(v, mvp); }
	
void drawInfo(){ if(!showInfo) return; RenderString("FPS: "+dtos(glApp.FPS)+"\n"
		"X: "+dtos(glApp.camera.x, 2)+" | Y: "+dtos(glApp.camera.y, 2)+" | Z: "+dtos(glApp.camera.z, 2)+" - "
		"aX: "+dtos(glApp.camera.aX, 2)+" | aY: "+dtos(glApp.camera.aY, 2)+" | aZ: "+dtos(glApp.camera.aZ, 2)+"\n"
		"ncpp powered",0.005f,0.025f, 0.015f, 0xFFFFFF, 0, glwnd.aspect); }
	
void onKeyPress(const WEvent& ev){ cons << "KEY: " << ev.key << " | " << NKeyDecode(ev.key) << " | keychar: " << String().push(ev.x) << "\n"; 
	if(gcons.keysHandle(ev)) return; if(glApp.camera.Controls(ev)) return;
	switch(ev.key){
		//case 82: { view.identity(); break; } //R
		case 300: glApp.camera.Capture(glwnd, !glApp.camera.isCaptured()); glApp.updateProjection(); break; //Esc
		case 352: showInfo = !showInfo; break; //F3
		case 361: glApp.toogleFullscreen(); glApp.updateProjection(); break; //F12
		case 85: { glApp.updateProjection(); break; } //U: glApp.updateProjection
	}
}

void onKeyUp(const WEvent& ev){ cons << "KEYUP: " << ev.key << " | " << NKeyDecode(ev.key) << "\n"; }

bool mouseTestRayTraceEnabled = false;
void onMouseEvent(const WEvent& ev){ cons << ev.name << ": (" << ev.x << ", " << ev.y << ")" << "\n"; 
	if(ev.id==WEvent::CLICK){ mouseTestRayTraceEnabled=!mouseTestRayTraceEnabled; } }

void glEvent(const WEvent& ev){ switch(ev.id){
		case WEvent::KEYDOWN: onKeyPress(ev); break; case WEvent::KEYUP: onKeyUp(ev); break;
		case WEvent::CLICK: case WEvent::MOUSEUP: onMouseEvent(ev); break; } }
		
void MouseTestRayTrace(const Matrix4& vp){ Vector3 outHit; glApp.camera.getMouseRayIntersection(outHit, glwnd, glApp.projection);
	RenderLine(Vector3(), outHit, 0xFFaa8855, vp);
	//cons << "getMouseRayIntersection: " << dtos(outHit.x, 2) << " " << dtos(outHit.y, 2) << " " << dtos(outHit.z, 2) << "\n";
}

void TriangleTestRayTrace(const Matrix4& vp){ Vector3 outHit; glApp.camera.getMouseRayIntersection(outHit, glwnd, glApp.projection); 
	Array<Vertex> v; genTriangle(v, 3, 0xFFFFFFFF, 15, 20); Vertex::swapAxis(v, 1, 2);
	
	if(isPXInTriangle2(outHit.x, outHit.z, v[0].x, v[0].z, v[1].x, v[1].z, v[2].x, v[2].z)){ Vertex::setColors(v, 0xFFcccccc); }
	else{ Vertex::setColors(v, 0xFF00cccc); } RenderTriangles(v, vp);
}

volatile int fpscnt=0; void fpsCalc(){ glApp.FPS=fpscnt; fpscnt=0; }
	
void GPURender(){ fpscnt++;
    Array<Vertex> v; // 1. Создаем треугольник (координаты в пикселях, раз у нас Ortho)
    v.push(Vertex(15.0f,  5.0f, 0.0f, 0xFFFF0000)); // Красный
	v.push(Vertex( 5.0f, 20.0f, 0.0f, 0xFF00FF00)); // Зеленый
	v.push(Vertex(25.0f, 20.0f, 0.0f, 0xFF0000FF)); // Синий
	//glApp.updateProjection();
	
	glApp.camera.captureMouse(glwnd); Matrix4 view=glApp.camera.getMatrix(); 
	Matrix4 vp = glApp.projection*view; //Matrix4 mvp = projection*view*model; //Matrix4 model; Matrix4 mvp = vp*model;

    GL::clear(0x222222); RenderTriangles(v, vp); drawSetka(vp); 
	drawInfo(); gcons.render();
	
	//rectE.rotate(1, 0, 0); cubeE.rotate(1, 1, 1);
	rect.render(rectE, vp); arc.render(vp); hexagon.render(vp); cube.render(cubeE, vp);
	#define TEST_LOAD_3D_TEXTURES
	#if !defined(TEST_LOAD_TEXTURES)||!defined(TEST_LOAD_3D_TEXTURES)
	sphere.render(vp); cylinder.render(vp); cylinder1.render(vp); cone.render(vp); cone1.render(vp);
	#endif
	
	//cola.render(vp);
	
	#ifdef TEST_LOAD_TEXTURES
	tyan.renderRect(5, 71, 50, -50, vp); //Inverse Y
	//tyan.renderRect(5, 21, 50, 50, vp*Matrix4().rotate(0,0,-30)); //Matrix Rotate
	//tyan.renderRect(0.5f, 0.5f, 0.2f, 0.2f); //2D Test
	#ifdef TEST_LOAD_3D_TEXTURES
	sphere.renderTexture(tyan, vp); //Texture Render
	cylinder.renderTexture(tyan, vp); cylinder1.renderTexture(tyan, vp); cone.renderTexture(tyan, vp); cone1.renderTexture(tyan, vp);
	#endif
	#endif
	
	
	Array<Vertex> l_tst0; l_tst0.push(Vertex(50,10,0,0xFFcc7766)).push(Vertex(60,15,0,0xFFccFF99));
	Array<Vertex> l_tst; genLines(l_tst, l_tst0, 2.5f, true); RenderTriangles(l_tst, vp);
	l_tst.clear(); Vertex::setColors(l_tst0, 0xFFFF0000); genLines(l_tst, l_tst0, 2.5f, false); RenderLines(l_tst, vp);
	
	if(mouseTestRayTraceEnabled){ MouseTestRayTrace(vp); TriangleTestRayTrace(vp); }
	
	/*RenderString("ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890\n" 
		"abcdefghijklmnopqrstuvwxyz ?\n"
		"!\"#$%&'()*+,-./ :;<=>?@ [\\]^_` {|}~\n"
		"FPS: "+dtos(glApp.FPS)+"\n"
		"NCPP POWERED",0,0.5f, 0.05f, 0xFFFFFF, 0, glwnd.aspect); //ASCII test*/
	//RenderString("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ \nабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
		//"\nЄІЇҐ єіїґ - Ти будеш їсти їжака чи ні?",0,0.5f, 0.05f, 0xFFFFFF, 0, glwnd.aspect); //Тест Кирилтицы
}

void CPULogic(){ rectE.rotate(1, 0, 0); cubeE.rotate(1, 1, 1); //rectE.lookAt(glApp.camera); //rectE.moveForward(0.2f);
}

void InitVertices(){
	genRect(rect.v, 10, 5, 0xFFFFFFFF, 0, 0);
	genRect(rect.l, 10, 5, 0xFFFF0000, 0, 0, false); //border
	
	//genCube(cube.v, cube.idx, 5, 5, 5, 0xFFFFFFFF, -20, 10, -20, true, true);
	//genCube(cube.l, cube.idx, 5, 5, 5, 0xFFFF0000, -20, 10, -20, false, true, false); //border
	//cube.setPos(-20, 10, -20);
	
	genCube(cube.v, cube.idx, 1, 8, 5, 0xFFFFFFFF, -20, 10, -20, true, true);
	genCube(cube.l, cube.idx, 1, 8, 5, 0xFFFF0000, -20, 10, -20, false, true, false); //border
	cube.randVertexColors();
	
	genArc(arc.v, 5, 0xFFFFFFFF, 30, 10, true); genArc(arc.l, 5, 0xFFFF0000, 30, 10, false);
	//genArc(hexagon.v, 5, 0xFFFFFFFF, 40, 10, true, 6); genArc(hexagon.l, 5, 0xFFFF0000, 40, 10, false, 6);
	genHexagon(hexagon.v, 5, 0xFF00FFFF, 40, 10, true); genHexagon(hexagon.l, 5, 0xFFFF0000, 40, 10, false);
	hexagon.randVertexColors();
	
	float Xparad=25, Zparad=10; bool waveforms=false;
	//if(!waveforms){ genSphere(sphere.v, 5, 0xFFCCCCFF, Xparad, 2.5f, Zparad, true); }
	if(!waveforms){ genSphere(sphere.v, 5, 0x00CCCCFF, Xparad, 2.5f, Zparad, true); }
	genSphere(sphere.l, 5, 0xFFFF0000, Xparad, 2.5f, Zparad, false); //border
	Xparad+=10;
	if(!waveforms){ genCylinder(cylinder.v, 5, 5, 0xFFCCCCFF, Xparad, 0, Zparad, true, false); }
	genCylinder(cylinder.l, 5, 5, 0xFFFF0000, Xparad, 0, Zparad, 2, false); //border
	Xparad+=10;
	if(!waveforms){ genCylinder(cylinder1.v, 5, 5, 0xFFCCCCFF, Xparad, 0, Zparad, true, false, 6); }
	genCylinder(cylinder1.l, 5, 5, 0xFFFF0000, Xparad, 0, Zparad, false, false, 6); //border
	Xparad+=10;
	if(!waveforms){ genCone(cone.v, 5, 5, 0xFFCCCCFF, Xparad, 0, Zparad, true); }
	genCone(cone.l, 5, 5, 0xFFFF0000, Xparad, 0, Zparad, false); //border
	Xparad+=10;
	if(!waveforms){ genCone(cone1.v, 5, 5, 0xFFCCCCFF, Xparad, 0, Zparad, true, 4); }
	genCone(cone1.l, 5, 5, 0xFFFF0000, Xparad, 0, Zparad, false, 4); //border
	
	// cola.loadOBJ("cola.obj"); cola.randVertexColors(); cola.pushVRAM();
	// cons << "cola vertices: " << cola.v.size() << "\ncola indices: " << cola.idx.size() << "\n";
	// --- ---
	
	#ifdef TEST_LOAD_TEXTURES
	const Image& img = Image::fromFile("../../index"); tyan.load(img); //tyan.load(img.px.data(), img.w, img.h);
	print("img: "); print(img.w); print("; "); print(img.h); print("\n");
	#endif
	
	//while(true){ GPURender(); glwnd.swapBuffers(); if(glApp.FPSLim>0) Sleep(1000/glApp.FPSLim); }
}

void onConsoleCmd(const String& cmdtxt){ Array<String> cmd = cmdtxt.split(" ");
	if(cmdtxt=="help"){ gcons.clear(); gcons << " Supported commands: \n"
		"cls/clear - clear area\n" "echo - echo text in area\n" "tp - teleport\n" "show ... - show commands\n" "set ... - set commands\n"
		"look - set camera lookAt\n" ; }
	if(cmd[0]=="show"){ if(cmd.size()<2||cmd[1]=="help"){ gcons << " show commands: \n" "show fps - show fps limit\n"; return; }
		if(cmd[1]=="fps"){ gcons << "fps limit: " << glApp.FPSLim << "\n"; return; }
	}
	if(cmd[0]=="set"){ if(cmd.size()<2||cmd[1]=="help"){ gcons << " set commands: \n" "set fps - set fps limit\n" 
			"set fog - set camera Fog angle\n" "set speed - set camera speed\n" "set angle - set camera angle\n"; return; }
		if(cmd[1]=="fps"){ if(cmd.size()<3||cmd[1]=="help"){ gcons << " Syntax: set fps [fps]\n"; return; } glApp.setFPS(stoin(cmd[2])); gcons << "New fps limit: " << glApp.FPSLim << "\n"; return; }
		if(cmd[1]=="fog"){ if(cmd.size()<3||cmd[1]=="help"){ gcons << " Syntax: set fog [angle]\n"; return; } glApp.camera.angleFog=stoin(cmd[2]); gcons << "New angleFog: " << glApp.camera.angleFog << "\n"; return; }
		if(cmd[1]=="speed"){ if(cmd.size()<3||cmd[1]=="help"){ gcons << " Syntax: set speed [speed]\n"; return; } glApp.camera.speed=stoin(cmd[2]); gcons << "New camera speed: " << glApp.camera.speed << "\n"; return; }
		if(cmd[1]=="angle"){ if(cmd.size()<5||cmd[1]=="help"){ gcons << " Syntax: set angle [aX] [aY] [aZ]\n"; return; } 
			glApp.camera.setAngle(stoin(cmd[2]), stoin(cmd[3]), stoin(cmd[4])); gcons << "New camera angle: " << cmd[2] << " " << cmd[3] << " " << cmd[4] << " " << "\n"; return; }
	}
	if(cmd[0]=="tp"){ if(cmd.size()<4||cmd[1]=="help"){ gcons << " Syntax: tp [x] [y] [z]\n"; return; }
		glApp.camera.setPos(stolln(cmd[1]), stolln(cmd[2]), stolln(cmd[3])); gcons << "Teleported to " << cmd.slice(1).join(" ") << "\n"; }
	if(cmd[0]=="look"){ if(cmd.size()<4||cmd[1]=="help"){ gcons << " Syntax: look [x] [y] [z]\n"; return; }
		glApp.camera.lookAt(stolln(cmd[1]), stolln(cmd[2]), stolln(cmd[3])); gcons << "Camera look to " << cmd.slice(1).join(" ") << "\n"; }
	
	GConsole::defCmdHandle(cmdtxt, gcons); }

int main(){ print(NCPP_COMPILE_INFO); os_version(); 
	glApp.createWindow(GLWindow(&app, "OpenGL Perspective Test", 600, 100, 600, 600));
	bool glInited=glApp.init(); cons << "glApp.init(): " << glInited << "\n"; if(!glInited){ exit(1); } 
	glApp.setHandler(glEvent); glApp.GPURender=GPURender; glApp.CPULogic=CPULogic;
	InitVertices(); gcons.onCommand=onConsoleCmd;  setInterval(fpsCalc, 1000);
	
	// Info Logs
	int width=0,height=0; glwnd.getSize(width,height); cons << "GLWindow size0: " << width << "; " << height << "\n";
	cons << "[GL String]: " << GL::GLVersion() << "\n"; int oglv[2]; GL::OGLVersion(oglv);
	cons << "[OpenGL Version]: " << oglv[0] << "." << oglv[1] << "\n";
	cons << "sizeof(Model): " << sizeof(Model) << "\n";
	cons << "sizeof(Vertex): " << sizeof(Vertex) << "\n";
	
	glApp.run(); return 0; }
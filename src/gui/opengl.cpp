#include <GL/gl.h> //OpenGL v1.1
#include <GL/glu.h> //OpenGL Utility Library (deprecated)
#ifndef _WIN32 
#include <GL/glx.h>
#endif
namespace ncpp { namespace GUI {
	struct GLWindow : Window { 
#ifdef _WIN32
		HDC hDC; HGLRC glCTX; char bmode;
		void createGLContext(bool useDoubleBuff=true){ DWORD pfdFlags; if(useDoubleBuff){ pfdFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; }
			else{ pfdFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; } useDoubleBuff?bmode=2:bmode=1;
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, pfdFlags, PFD_TYPE_RGBA, 32,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 }; 

			hDC = GetDC(wndID); int pixelFormat = ChoosePixelFormat(hDC, &pfd);
			if(pixelFormat == 0){ MessageBox(NULL, "ChoosePixelFormat Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			if(!SetPixelFormat(hDC, pixelFormat, &pfd)){ MessageBox(NULL, "SetPixelFormat Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			glCTX = wglCreateContext(hDC); if(!glCTX){ MessageBox(NULL, "wglCreateContext Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			if(setContext()){ print("(#) createGLContext() successful.\n"); }
			// glewInit(); Инициализация GLEW или другой библиотеки загрузки функций OpenGL, если используется
		}
		void destroyGLContext(){ if(glCTX){ wglMakeCurrent(NULL, NULL); wglDeleteContext(glCTX); glCTX = NULL; } if(hDC && wndID){ ReleaseDC(wndID, hDC); hDC = NULL; } }
		bool setContext(){ if(!glCTX){ print("(!) setContext(): glContext not created\n"); return false; }
			if(!wglMakeCurrent(hDC, glCTX)){ print("(!) setContext(): wglMakeCurrent OpenGL set context current failed.\n"); return false; } return true; }
		void resetContext(){ wglMakeCurrent(NULL, NULL); }
				
		void swapBuffers(){ SwapBuffers(hDC); }

#else
#ifdef USE_WAYLAND
#else
		GLXContext glCTX; char bmode;
        
        void createGLContext(bool useDoubleBuff=false){ GLint attr[] = { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, (useDoubleBuff)?True:False, None };
			useDoubleBuff?bmode=2:bmode=1; int fbConfigCount; GLXFBConfig* fbConfigs = glXChooseFBConfig(app->display, DefaultScreen(app->display), attr, &fbConfigCount);
			if(fbConfigs == NULL || fbConfigCount == 0){ print("(!) createGLContext: No appropriate GLXFBConfig found\n"); return; }
			glCTX = glXCreateNewContext(app->display, fbConfigs[0], GLX_RGBA_TYPE, NULL, True); if(glCTX == NULL){ print("(!) Failed to create GL context.\n"); return; }
			if(setContext()){ print("(#) X11 createGLContext() successful.\n"); }
			
			int dflag; glXGetFBConfigAttrib(app->display, fbConfigs[0], GLX_DOUBLEBUFFER, &dflag);
			if(dflag){ bmode=2; print("(#) Double buffering is enabled.\n"); }else{ bmode=1; print("(#) Double buffering is not enabled.\n"); } XFree(fbConfigs);
		}

        /*void createGLContext(){ GLint attr[] = { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, None };
			XVisualInfo* vi = glXChooseVisual(app->display, app->screen, attr); if(vi == NULL) { print("(!) createGLContext: No appropriate visual found\n"); return; }
			glCTX = glXCreateContext(app->display, vi, NULL, True); if(glCTX == NULL){ print("(!) Failed to create OpenGL context\n"); return; }
			if(setContext()){ print("(#) X11 createGLContext() successful.\n"); }
			
			int dflag; if(glXGetConfig(app->display, vi, GLX_DOUBLEBUFFER, &dflag)) { print("(!) Error querying GLX configuration\n"); return; }
			if(dflag){ bmode=2; print("(#) Double buffering is enabled.\n"); }else{ bmode=1; print("(#) Double buffering is not enabled.\n"); } XFree(vi);
		}*/
		void destroyGLContext(){ if(glCTX){ glXMakeCurrent(app->display, None, NULL); glXDestroyContext(app->display, glCTX); glCTX = NULL; } }
		bool setContext(){ if(!glCTX){ print("(!) setContext(): glContext not created\n"); return false; }
			if(!glXMakeCurrent(app->display, wndID, glCTX)){ print("(!) setContext(): glXMakeCurrent OpenGL context current failed.\n"); return false; } return true; }
		void resetContext(){ glXMakeCurrent(app->display, None, NULL); }

        void swapBuffers(){ glXSwapBuffers(app->display, wndID); }

#endif
#endif
		GLWindow() : Window(), glCTX(NULL){}

		GLWindow(App* app1, const char* name = "", int x = DEF_HWND_X, int y = DEF_HWND_Y, unsigned int width = DEF_HWND_WIDTH, unsigned int height = DEF_HWND_HEIGHT) 
			: Window(app1, name, x, y, width, height), glCTX(NULL){ createGLContext(); }
		
		GLWindow(const char* name, int x = DEF_HWND_X, int y = DEF_HWND_Y, unsigned int width = DEF_HWND_WIDTH, unsigned int height = DEF_HWND_HEIGHT) 
			: Window(name, x, y, width, height), glCTX(NULL){ createGLContext(); }

		~GLWindow(){ destroyGLContext(); }
		bool isContext(){ return (glCTX)?true:false; }
		void draw(){ if(bmode<=1){ glFlush(); }else{ swapBuffers(); } }
		//InvalidateRect(this->wndID, NULL, TRUE);
		
		void resetMatrixMode(){ glMatrixMode(GL_PROJECTION); glLoadIdentity(); glMatrixMode(GL_MODELVIEW); glLoadIdentity(); } //resetOrtho
		void setOrtho(float x0, float x1, float y0, float y1, float zNear=-1, float zFar=1){ glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glOrtho(x0, x1, y0, y1, zNear, zFar); glMatrixMode(GL_MODELVIEW); glLoadIdentity(); }
		void setMatrix2DPreset(char rotate=0){ int width=0, height=0; getSize(width, height);
		switch(rotate){
			case 1: setOrtho(0.0, width, height, 0.0); //слева-сверху
			case 2: setOrtho(-width, 0.0, 0.0, -height); //справа-сверху
			case 3: setOrtho(-width, 0.0, -height, 0.0); //справа-снизу
			default: setOrtho(0.0, width, 0.0, height); //слева-снизу
			} }
		void setMatrix3DPreset(){ glEnable(GL_DEPTH_TEST); 
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // Настраиваем перспективную проекцию, так как это 3D-куб

			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -5.0f); // Позиционируем камеру, чтобы видеть куб
		}
	};
} }

namespace ncpp { namespace GL { //ncpp::GL funcs
	//=== Legacy OpenGL 1.1 API ===
	void clear(float redf, float greenf, float bluef, float alphaf=1){ glClearColor(redf, greenf, bluef, alphaf); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }	
	void clear(unsigned int hex=0, float alphaf=1){ if(hex==0){ clear(0.0f, 0.0f, 0.0f, alphaf); return; } RGB rgb=HexToRGBFloat(hex); clear(rgb.r, rgb.g, rgb.b, alphaf); }
	void glArc(float x, float y, float radius, float startAngle=0, float endAngle=360, float lsize=1, bool fill=false, int segments=100){
		if(segments < 1){ segments = 1; } startAngle*=M_PI / 180.0f; endAngle*=M_PI / 180.0f;
		
		// Убедимся, что начальный угол меньше конечного угла для плавного перехода
		if (startAngle > endAngle){ endAngle += 2.0f * M_PI; } // Добавляем 2π, чтобы дуга была нарисована правильно

		glLineWidth(lsize); glColor3f(1.0f, 0.0f, 0.0f); if(fill){ glBegin(GL_TRIANGLE_FAN); glVertex2f(x, y); }else{ glBegin(GL_LINE_STRIP); }

		// Рисуем сегменты окружности
		for (int i = 0; i <= segments; ++i){
			float angle = startAngle + (float)i / segments * (endAngle - startAngle); // Вычисляем угол
			float x1 = x + radius * cosf(angle); float y1 = y + radius * sinf(angle); glVertex2f(x1, y1); } glEnd();
	}
	
	//=== OpenGL 2.0 API ===
	unsigned int vShader=0, fShader=0; //ID вершинного и фрагментного шейдеров.
	
	bool InitDefautShaders(){ //Использует GLSL 1.10
		//Вершинный шейдер: Возвращает вершины без изменений
		const char* vertexShaderSrc = "#version 110\n"\
			"attribute vec4 in_position;\n"\
			"void main(){ gl_Position = in_position; }";

		//Фрагментный шейдер: Красит все пиксели в красный цвет
		const char* fragmentShaderSrc = "#version 110\n"\
			"void main(){ gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";
			
		if(!glCreateShader){ if(!LoadOpenGL_2_0()){ print("(!) GL::InitDefautShaders failed: OpenGL 2.0 extensions unavailable."); return false; } 
			if(!glCreateShader){ print("(!) GL::InitDefautShaders ERROR: glCreateShader is a NULL."); exit(1); } }
		
		vShader = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vShader, 1, &vertexShaderSrc, NULL); glCompileShader(vShader);
		fShader = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fShader, 1, &fragmentShaderSrc, NULL); glCompileShader(fShader);
		
		if(!vShader||!fShader){ return false; } GLint isCompiled;
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE) return false;
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE) return false;
		return true; }
	
} }


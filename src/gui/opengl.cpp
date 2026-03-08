#include <GL/gl.h> //OpenGL v1.1
#ifndef _WIN32
#define Window XWindowID
#include <GL/glx.h>
#undef Window
#endif
//libEGL.so.1

namespace ncpp { namespace GUI {
	struct GLWindow : Window {
#ifdef _WIN32
		private: HDC hDC; HGLRC glCTX; char bmode; public:
		void createGLContext(bool useDoubleBuff=true){ DWORD pfdFlags; if(useDoubleBuff){ pfdFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; }
			else{ pfdFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; } useDoubleBuff?bmode=2:bmode=1;
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, pfdFlags, PFD_TYPE_RGBA, 32,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 }; 

			hDC = GetDC(wndID); int pixelFormat = ChoosePixelFormat(hDC, &pfd);
			if(pixelFormat == 0){ MessageBox(NULL, "ChoosePixelFormat Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			if(!SetPixelFormat(hDC, pixelFormat, &pfd)){ MessageBox(NULL, "SetPixelFormat Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			glCTX = wglCreateContext(hDC); if(!glCTX){ MessageBox(NULL, "wglCreateContext Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK); return; }
			//if(setContext()){ print("(#) Win32 createGLContext() successful.\n"); }
		}
		void destroyGLContext(){ if(glCTX){ wglMakeCurrent(NULL, NULL); wglDeleteContext(glCTX); glCTX = NULL; } if(hDC && wndID){ ReleaseDC(wndID, hDC); hDC = NULL; } }
		bool setContext(){ if(!glCTX){ print("(!) setContext(): glContext not created\n"); return false; }
			if(!wglMakeCurrent(hDC, glCTX)){ print("(!) setContext(): wglMakeCurrent OpenGL set context current failed.\n"); return false; } return true; }
		void resetContext(){ wglMakeCurrent(NULL, NULL); }
				
		void swapBuffers(){ SwapBuffers(hDC); }
		
		void hideCursor(bool hide=true){ ShowCursor(!hide ? TRUE : FALSE); }

#elif USE_WAYLAND
#else
		private: GLXContext glCTX; char bmode; public:
        
        void createGLContext(bool useDoubleBuff=false){ GLint attr[] = { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, 
				GLX_DOUBLEBUFFER, (useDoubleBuff)?True:False, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, None };
			useDoubleBuff?bmode=2:bmode=1; int fbConfigCount; GLXFBConfig* fbConfigs = glXChooseFBConfig(app->display, DefaultScreen(app->display), attr, &fbConfigCount);
			if(fbConfigs == NULL || fbConfigCount == 0){ print("(!) createGLContext: No appropriate GLXFBConfig found\n"); return; }
			glCTX = glXCreateNewContext(app->display, fbConfigs[0], GLX_RGBA_TYPE, NULL, True); if(glCTX == NULL){ print("(!) Failed to create GL context.\n"); return; }
			//if(setContext()){ print("(#) X11 createGLContext() successful.\n"); }
			
			int dflag; glXGetFBConfigAttrib(app->display, fbConfigs[0], GLX_DOUBLEBUFFER, &dflag);
			if(dflag){ bmode=2; print("(#) Double buffering is enabled.\n"); }else{ bmode=1; print("(#) Double buffering is not enabled.\n"); } XFree(fbConfigs); }
			
		void destroyGLContext(){ if(glCTX){ glXMakeCurrent(app->display, None, NULL); glXDestroyContext(app->display, glCTX); glCTX = NULL; } }
		bool setContext(){ if(!glCTX){ print("(!) setContext(): glContext not created\n"); return false; }
			if(!glXMakeCurrent(app->display, wndID, glCTX)){ print("(!) setContext(): glXMakeCurrent OpenGL context current failed.\n"); return false; } return true; }
		void resetContext(){ glXMakeCurrent(app->display, None, NULL); }

        void swapBuffers(){ glXSwapBuffers(app->display, wndID); }
		
		void hideCursor(bool hide=true){ Display* dpy = app->display;
			if(hide){ Cursor invisibleCursor; Pixmap bitmapNoData; XColor black;
				static char noData[] = { 0,0,0,0,0,0,0,0 }; black.red = black.green = black.blue = 0;
				bitmapNoData = XCreateBitmapFromData(dpy, wndID, noData, 8, 8);
				invisibleCursor = XCreatePixmapCursor(dpy, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
				XDefineCursor(dpy, wndID, invisibleCursor); XFreePixmap(dpy, bitmapNoData); XFreeCursor(dpy, invisibleCursor);
			}else{ XUndefineCursor(dpy, wndID); } XFlush(dpy); }
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
		
		//--- Legacy OpenGL 1.1 Matrix ---
		static void resetMatrixMode(){ glMatrixMode(GL_PROJECTION); glLoadIdentity(); glMatrixMode(GL_MODELVIEW); glLoadIdentity(); } //resetOrtho?
		static void setOrtho(float x0, float x1, float y0, float y1, float zNear=-1, float zFar=1){ glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glOrtho(x0, x1, y0, y1, zNear, zFar); glMatrixMode(GL_MODELVIEW); glLoadIdentity(); }
		void setMatrix2DPreset(char rotate=0){ int width=0, height=0; getSize(width, height); //setOrtho2DPreset?
			switch(rotate){
				case 1: setOrtho(0.0, width, height, 0.0); break; //слева-сверху
				case 2: setOrtho(-width, 0.0, 0.0, -height); break; //справа-сверху
				case 3: setOrtho(-width, 0.0, -height, 0.0); break; //справа-снизу
				default: setOrtho(0.0, width, 0.0, height); break; //слева-снизу
			} }
		void setMatrix3DPreset(){ glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			//gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // Настраиваем перспективную проекцию, так как это 3D-куб

			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -5.0f); // Позиционируем камеру, чтобы видеть куб
		}
		
		/*#if __cplusplus >= 201103L //move for C++11
		GLWindow(GLWindow&& tmp){ move(*this, tmp); }
		GLWindow& operator=(GLWindow&& tmp){ if(this==&tmp) return; move(*this, tmp); return *this; }
		private: GLWindow(const GLWindow&) = delete; GLWindow& operator=(const GLWindow&) = delete; public:
		GLWindow& steal(GLWindow& tmp){ move(*this, tmp); return *this; }
		GLWindow& steal(GLWindow&& tmp){ move(*this, tmp); return *this; }
		friend void move(GLWindow& dst, GLWindow& tmp){ move(dst, (GLWindow&&)tmp); }
		friend void move(GLWindow& dst, GLWindow&& tmp)
		#else //move for C++98
		private: GLWindow(const GLWindow&); GLWindow& operator=(const GLWindow&); public: //Запрет копирования.
		GLWindow& steal(const GLWindow& victim){ move(*this, victim); return *this; }
		friend void move(GLWindow& dst, const GLWindow& victim)
		#endif
		{ 	
			#if __cplusplus < 201103L
			GLWindow& tmp = (GLWindow&)victim;
			#endif
			dst.destroyGLContext(); dst.destroy(); dst.app = tmp.app; dst.wndID = tmp.wndID; tmp.wndID = NULL; 
			dst.glCTX = tmp.glCTX; tmp.glCTX = NULL; dst.bmode = tmp.bmode;
			#ifdef _WIN32
			dst.hDC = tmp.hDC; tmp.hDC = NULL;
			#endif	
		}*/
		
		#if __cplusplus >= 201103L //move for C++11
		GLWindow(GLWindow&& tmp) noexcept { move(*this, tmp); }
		GLWindow& operator=(GLWindow&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
		GLWindow(const GLWindow&) = delete; GLWindow& operator=(const GLWindow&) = delete; //Запрет копирования.
		GLWindow& steal(GLWindow& tmp){ move(*this, tmp); return *this; }
		GLWindow& steal(GLWindow&& tmp){ move(*this, tmp); return *this; }
		friend void move(GLWindow& dst, GLWindow&& tmp){ move(dst, (GLWindow&)tmp); }
		#else //move for C++98
		private: GLWindow(const GLWindow&); GLWindow& operator=(const GLWindow&); public: //Скрытие копирования.
		GLWindow& steal(const GLWindow& victim){ move(*this, (GLWindow&)victim); return *this; }
		friend void move(GLWindow& dst, const GLWindow& victim){ move(dst, (GLWindow&)victim); }
		#endif
		friend void move(GLWindow& dst, GLWindow& tmp){
			dst.destroyGLContext(); dst.destroy(); dst.app = tmp.app; dst.wndID = tmp.wndID; tmp.wndID = 0;
			dst.glCTX = tmp.glCTX; tmp.glCTX = NULL; dst.bmode = tmp.bmode;
			#ifdef _WIN32
			dst.hDC = tmp.hDC; tmp.hDC = NULL;
			#endif	
		}
	};
} //using GUI::move; 
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ncpp { namespace GL { //ncpp::GL funcs
	void HexToRGBf(float rgb[3], unsigned int hex);
	const char* OGLVersion(){ return (const char*)glGetString(GL_VERSION); }
	void OGLVersion(int oglv[2]){ oglv[0]=0; oglv[1]=0; glGetIntegerv(GL_MAJOR_VERSION, &oglv[0]); glGetIntegerv(GL_MINOR_VERSION, &oglv[1]); }
	void clear(float redf, float greenf, float bluef, float alphaf=1){ glClearColor(redf, greenf, bluef, alphaf); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }	
	//=== Legacy OpenGL 1.1 API ===
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
	void DrawLegacyPixels(unsigned int* pixels, float width, float height, float offsetX=0, float offsetY=0){
		glRasterPos2f(offsetX, offsetY); glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels); }
	
} }


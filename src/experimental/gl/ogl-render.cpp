#include "gl-math.cpp"
// ========= GL Engine =========
namespace ncpp { namespace GL { unsigned int VBO, IBO, VAO;
	void clear(unsigned int hex=0, float alphaf=1){ if(hex==0){ clear(0.0f, 0.0f, 0.0f, alphaf); return; } float rgb[3]; HexToRGBf(rgb, hex); clear(rgb[0], rgb[1], rgb[2], alphaf); }
	//=== OpenGL 2.0 API ===
	struct ShaderInfo { GLuint program; unsigned int vShader, fShader; GLint mvpLoc, posLoc, colorLoc, rectParamsLoc, uvLoc, vnLoc; 
		ShaderInfo() : program(-1), vShader(0), fShader(0){}
		
		bool compile(const char* vShader_src, const char* fShader_src){
			GLint isCompiled; if(!glCreateShader){ if(!glCreateShader){ Except("ShaderInfo::compile ERROR: glCreateShader is a NULL.\n"); return false; } }
			vShader = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vShader, 1, &vShader_src, NULL); glCompileShader(vShader);
			fShader = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fShader, 1, &fShader_src, NULL); glCompileShader(fShader);
			glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: vShader compile failed.\n"); return false; }
			glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: fShader compile failed.\n"); return false; }
			return true; }
		bool compileVertex(const char* vShader_src){ GLint isCompiled; vShader = glCreateShader(GL_VERTEX_SHADER); 
			glShaderSource(vShader, 1, &vShader_src, NULL); glCompileShader(vShader); glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled); 
			if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: vShader compile failed.\n"); return false; } return true; }
		bool compileFragment(const char* fShader_src){ GLint isCompiled; fShader = glCreateShader(GL_FRAGMENT_SHADER); 
			glShaderSource(fShader, 1, &fShader_src, NULL); glCompileShader(fShader); glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled); 
			if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: fShader compile failed.\n"); return false; } return true; }
		
		bool LinkProg(){ if(program!=(GLuint)-1) return false; program = glCreateProgram(); 
			glAttachShader(program, vShader); glAttachShader(program, fShader); glLinkProgram(program);
			GLint linked; glGetProgramiv(program, GL_LINK_STATUS, &linked); if(!linked) return false;
			glDeleteShader(vShader); glDeleteShader(fShader); return true; }
		bool initLoc(){ if(!LinkProg()) return false;  
			mvpLoc = glGetUniformLocation(program, "uMVP"); posLoc = glGetAttribLocation(program, "pos");
			colorLoc = glGetAttribLocation(program, "color"); //GLint baseColorLoc = glGetUniformLocation(shinfo.program, "uBaseColor");
			return true; }
		GLint getAttrLoc(const char* name){ return glGetAttribLocation(program, name); }
		GLint getULoc(const char* name){   return glGetUniformLocation(program, name); }
		bool initTexRect(){ rectParamsLoc = glGetUniformLocation(program, "uRectParams"); return true; }
		bool initTexUV(){ uvLoc = glGetAttribLocation(program, "uv"); return true; }
		bool initLightVN(){ vnLoc = glGetAttribLocation(program, "vn"); return true; }
	};
	namespace Shaders { ShaderInfo shDef; //стандартный шейдер
		ShaderInfo shDef_sim; //Шейдер-пример
		ShaderInfo shTexRect; //Шейдер с поддержкой прямоугольной текстуры
		ShaderInfo shTex; //Шейдер с поддержкой произвольной текстуры
	}
	
	bool InitExampleShaders(){ //Использует GLSL 1.10
		//Вершинный шейдер: Возвращает вершины без изменений
		const char* vShader_src = "#version 110\n"
			"attribute vec2 pos;\n"
			"void main(){ gl_Position = vec4(pos, 0.0, 1.0); }";
		//Фрагментный шейдер: Красит все пиксели в красный цвет
		const char* fShader_src = "#version 110\n" "void main(){ gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";
		if(!Shaders::shDef_sim.compile(vShader_src, fShader_src)) return false; return true; }
		
	bool InitMinimalShaders(){ using namespace Shaders; //Вершинный шейдер (GLSL 1.10)
		const char* vShader_src = "#version 110\n"
			"attribute vec3 pos;\n"
			"attribute vec4 color;\n"
			"varying vec4 vColor;\n"
			"uniform mat4 uMVP;\n"
			//"varying vec2 vTexCoord;\n"
			"void main(){ gl_Position = uMVP * vec4(pos, 1.0); vColor = color; }\n"; //vTexCoord = pos.xy;

		// Фрагментный шейдер
		const char* fShader_src = "#version 110\n"
			"varying vec4 vColor; // Цвет от вершин (интерполированный)\n"
			//"uniform vec4 uBaseColor; // Базовый цвет из glUniform4f\n"
			//"varying vec2 vTexCoord;\n"
			"void main(){ gl_FragColor = vColor; }\n"; //gl_FragColor = vColor * uBaseColor;
		if(!shDef.compile(vShader_src, fShader_src)) return false; return shDef.initLoc(); }
		
	bool InitOtherShaders(){ using namespace Shaders;
		//=== Шейдер с поддержкой текстур без честной UV ===
		// --- Vetex (Вершинный шейдер прямоугольной текстуры без UV) ---
		const char* vShader_src = "#version 110\n"
			"attribute vec3 pos;\n"
			"attribute vec4 color;\n"
			"uniform mat4 uMVP;\n"
			"uniform vec4 uRectParams; //x: minX, y: minY, z: width, w: height\n"
			"varying vec4 vColor;\n"
			"varying vec2 vTexCoord; //проброс UV cords\n"
			"void main(){\n"
			"  gl_Position = uMVP * vec4(pos, 1.0); vColor = color;\n"
			"  vTexCoord = (pos.xy - uRectParams.xy) / uRectParams.zw; } // Вычисляем UV на основе позиции\n";
			//"  vTexCoord.y = 1.0-((pos.y - uRectParams.y) / uRectParams.w); } // Инверсия UV для Y (опционально)\n";
		// --- Vetex (Вершинный шейдер произвольной текстуры) ---
		const char* vShaderTex_src = "#version 110\n"
			"attribute vec3 pos;\n"
			"attribute vec4 color;\n"
			"attribute vec2 uv; // Принимаем UV\n"
			"uniform mat4 uMVP;\n"
			"varying vec4 vColor;\n"
			"varying vec2 vTexCoord; //проброс UV cords\n"
			"void main(){ gl_Position = uMVP * vec4(pos, 1.0); vColor = color; vTexCoord = uv; }\n";
		// --- Fragment ---
		const char* fShader_src = "#version 110\n"
			"varying vec4 vColor; // Цвет от вершин (интерполированный)\n"
			"varying vec2 vTexCoord;\n"
			"uniform sampler2D uTexture;\n"
			"void main(){\n"
			"  gl_FragColor = texture2D(uTexture, vTexCoord) * vColor;\n"
			"}\n";
		if(!shTexRect.compile(vShader_src, fShader_src)||!shTexRect.initLoc()||!shTexRect.initTexRect()) return false; 
		if(!shTex.compile(vShaderTex_src, fShader_src)||!shTex.initLoc()||!shTex.initTexUV()) return false; return true; }
		
	bool InitDefaultShaders(){ if(!LoadOGL_20()){ print("(!) GL::InitDefaultShaders failed: OpenGL 2.0 extensions unavailable.\n"); return false; }
		if(!LoadOGL_30()){ print("(!) Loading extensions OpenGL 3.0 failed.\n"); }
		if(!LoadOGL_31()){ print("(!) Loading extensions OpenGL 3.1 failed.\n"); }
		if(!InitExampleShaders()) return false; if(!InitMinimalShaders()) return false; 
		if(!InitOtherShaders()){ print("(!) GL::InitOtherShaders: Compile Shaders failed.\n"); }
		
		glGenBuffers(1, &VBO); glBindBuffer(GL_ARRAY_BUFFER, VBO); //glBufferData(GL_ARRAY_BUFFER, MAX_SIZE, NULL, GL_DYNAMIC_DRAW);
		if(glGenVertexArrays){ glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); } //VAO для новых драйверов.
		glGenBuffers(1, &IBO); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); return true; }
	
	bool FillMode=true;
	//void SetLineWidth(float sz=1.0f){ glLineWidth(sz); } //deprecated
	void SetFillMode(bool mode=true){ FillMode=mode; if(!mode){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } } //deprecated
	
	void _applyState(const Vertex* v, size_t len, const Matrix4& mvp, GLenum mode, const ShaderInfo& shinfo){
		glUseProgram(shinfo.program); glUniformMatrix4fv(shinfo.mvpLoc, 1, GL_FALSE, mvp.m); //передача MVP в шейдер.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, len*sizeof(Vertex), v, GL_STREAM_DRAW);
		
		glEnableVertexAttribArray(shinfo.posLoc); glEnableVertexAttribArray(shinfo.colorLoc);
		glVertexAttribPointer(shinfo.posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Указываем откуда брать позицию
		glVertexAttribPointer(shinfo.colorLoc, 4, ColorGL_T, ColorGL_Norm, sizeof(Vertex), (void*)offsetof(Vertex, _color)); } // Указываем откуда брать цвет
	
	//Base Rendering
	void RenderVertices(const Vertex* v, size_t len, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_POINTS, const ShaderInfo& shinfo=Shaders::shDef){ 
		if(len==0) return; _applyState(v, len, mvp, mode, shinfo); glDrawArrays(mode, 0, len); 
		glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc); }
	
	void RenderTriangles(const Vertex* v, size_t len, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v, len, mvp, GL_TRIANGLES, shinfo); } //Main Core Standard
	void RenderTriangles(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v.data(), v.size(), mvp, GL_TRIANGLES, shinfo); }
	
	void RenderLines(const Vertex* v, size_t len, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v, len, mvp, GL_LINES, shinfo); } //Core Standard
	void RenderLines(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v.data(), v.size(), mvp, GL_LINES, shinfo); }
	void RenderLine(const Vertex& v1, const Vertex& v2, int color=0xFFFFFFFF, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){
		Vertex _v[2]; Array<Vertex> v; v.stack(_v); (v[0]=v1).setColor(color); (v[1]=v2).setColor(color); RenderLines(v, mvp, shinfo); }
	
	void RenderTriangleStrip(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v.data(), v.size(), mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStrip(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v.data(), v.size(), mvp, GL_LINE_STRIP, shinfo); } //Vulkan Supported
	void RenderLineLoop(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(v.data(), v.size(), mvp, GL_LINE_LOOP, shinfo); }
	
	//Index Rendering
	void RenderVerticesIdx(const Array<Vertex>& v, const Array<unsigned int>& idx, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_TRIANGLES, const ShaderInfo& shinfo=Shaders::shDef){
		if(v.empty() || idx.empty()) return; _applyState(v.data(), v.size(), mvp, mode, shinfo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size()*sizeof(unsigned int), idx.data(), GL_STREAM_DRAW);
		glDrawElements(mode, idx.size(), GL_UNSIGNED_INT, 0); glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc); 
	}
	
	void RenderTrianglesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& idx, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, idx, mvp, GL_TRIANGLES, shinfo); }
	void RenderLinesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& idx, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, idx, mvp, GL_LINES, shinfo); }
	
	void RenderTriangleStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& idx, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, idx, mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& idx, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, idx, mvp, GL_LINE_STRIP, shinfo); }

	struct Camera : Entity { float sensitivity; float speed; float angleFog; char mouseCapt; 
		enum CamMode { FREECAM, STRATEGY }; char cmode;
		Camera(double x1=0, double y1=0, double z1=0) : Entity(x1, y1, z1), sensitivity(1.0f), speed(1.0f), angleFog(60.0f), mouseCapt(0){}
		
		Camera& rotate(float aX1, float aY1, float aZ1){ return (Camera&)Entity::rotate(aX1, aY1, aZ1); }
		//Matrix4 getMatrix() const { Matrix4 V; V.rotate(-aX, -aY, -aZ).translate(-x, -y, -z); return V; } //Old Abs Pos+Angle
		Matrix4 getMatrix() const { Matrix4 V; V.rotateX(-aX).rotateY(-aY).rotateZ(-aZ).translate(-x, -y, -z); return V; }
		
		bool isCaptured(){ return mouseCapt; }
		void Capture(GUI::GLWindow& glw, bool capt=true){ mouseCapt = capt?1:0; glw.hideCursor(capt); }
		void setCamMode(char mode=FREECAM){ cmode=mode; };
		
		void captureMouse(GUI::GLWindow& glw){ if(!mouseCapt) return; int w=0, h=0; glw.getSize(w, h); int centerX = w/2; int centerY = h/2;
			if(mouseCapt==1){ glw.setMousePos(centerX, centerY); mouseCapt=2; return; }
			int mX=0, mY=0; glw.getMousePos(mX, mY); float dx = (float)(mX - centerX); float dy = (float)(mY - centerY);
			if (dx!=0 || dy!=0){ rotate(-dy*sensitivity, -dx*sensitivity, 0); glw.setMousePos(centerX, centerY); }
			if(aX > 89.9f) aX = 89.9f; if(aX < -89.9f) aX = -89.9f; } // Ограничение, чтобы не было Gimbal Lock
			
		bool Controls(const GUI::WEvent& ev){ switch(cmode){ case FREECAM: default: return _Controls_FreeCam(ev); } }
		
		bool _Controls_FreeCam(const GUI::WEvent& ev){ switch(ev.key){
			case 313: speed++; return true; //+
			case 312: speed--; return true; //-
			case 87:  { moveForward(speed); return true; } //W
			case 65:  { moveRight(-speed); return true; } //A
			case 83:  { moveForward(-speed); return true; } //S
			case 68:  { moveRight(speed); return true; } //D
			case 32:  { y+=speed; return true; } //Space
			case 304: { y+=-speed; return true; } //LShift
			
			case 334: { rotate(sensitivity, 0, 0); //Up //if(aX > 89.9f) aX = 89.9f; // Ограничение, чтобы не было Gimbal Lock
				return true; }
			case 335: { rotate(-sensitivity, 0, 0); //Down //if(aX < -89.9f) aX = -89.9f;
				return true; }
			case 336: { rotate(0, sensitivity, 0); return true; }  //Left
			case 337: { rotate(0, -sensitivity, 0); return true; } //Right
			case 81:  { rotate(0, 0, sensitivity); return true; }  // Q (Крен влево)
			case 69:  { rotate(0, 0, -sensitivity); return true; } // E (Крен вправо)
		} return false; }
		
		bool getMouseRayIntersection(Vector3& outHit, const GUI::GLWindow& gwnd, const Matrix4& projection, char planeAxis=0, float planeOffset=0)
		{   int w=0, h=0; int mX=0, mY=0; gwnd.getSize(w, h); gwnd.getMousePos(mX, mY); if(w==0||h==0) return false;
			float xNDC = (2.0f * mX) / w - 1.0f; float yNDC = 1.0f - (2.0f * mY) / h; // Перевод в NDC
			Matrix4 invVP = (projection*getMatrix()).inverse();
			
			Vector4 start = Vector4(xNDC, yNDC, -1.0f, 1.0f)*invVP; // Начало (на Near плоскости) и конец (на Far плоскости) луча
			Vector4 end   = Vector4(xNDC, yNDC,  1.0f, 1.0f)*invVP;

			Vector3 rayOrigin(start.x / start.w, start.y / start.w, start.z / start.w);
			Vector3 rayEnd(end.x / end.w, end.y / end.w, end.z / end.w); Vector3 dir = (rayEnd-rayOrigin).norm();

			//enum class Plane { XZ, XY, YZ } planeAxis; // (0: XZ, 1: XY, 2: YZ)
			float denom = (planeAxis == 0) ? dir.y : (planeAxis == 1) ? dir.z : dir.x; if(fabs(denom) < 1e-6) return false; // Луч параллелен плоскости

			float numer = (planeAxis == 0) ? (planeOffset - rayOrigin.y) : 
						  (planeAxis == 1) ? (planeOffset - rayOrigin.z) : (planeOffset - rayOrigin.x);
			float t = numer/denom; if(t < 0.0f) return false; outHit = rayOrigin+(dir*t); return true;
		}
		
		void lookAt(double x1=0, double y1=0, double z1=0){
			double dx = x1-x, dy = y1-y, dz = z1-z; double lenXZ = sqrt(dx*dx + dz*dz);
			if(lenXZ==0.0f&&dy==0.0f) return; float rad = 180.0f/M_PIf; setAngle(atan2f(dy, lenXZ)*rad, atan2f(-dx, -dz)*rad, 0.0f);
			if(aX>89.9f) aX=89.9f; if (aX<-89.9f) aX=-89.9f; } // Ограничение угла наклона (Gimbal Lock)
		void lookAt(const Entity& e){ lookAt(e.x, e.y, e.z); }
		void lookAt(const Vector3& v){ lookAt(v.x, v.y, v.z); }
	};
	
} }
#include "gl-geometry.cpp"
#include "gl-vfont.cpp"

namespace ncpp { namespace GL {
	struct Texture { unsigned int tID; Texture() : tID(0){} //~Texture(){ this->free(); }
		void load(void* px, int width, int height){ if(tID!=0) free(); glGenTextures(1, &tID); glBindTexture(GL_TEXTURE_2D, tID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, px); }
		void load(const Image& img){ load(img.px.data(), img.w, img.h); }
		void free(){ if(tID==0) return; glDeleteTextures(1, &tID); tID=0; }
		
		//unsigned int loadImg(const Image& img){ tyan.load(img.px.data(), img.w, img.h); }
		//unsigned int loadImg(const CString& fpath){ loadImg(Image::fromFile(fpath)); }
		
		void _renderRect(const Array<Vertex>& v, float x, float y, float w, float h, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), float scale=1.0f)
		{	glUseProgram(Shaders::shTexRect.program); glUniform4f(Shaders::shTexRect.rectParamsLoc, x, y, w*scale, h*scale);
			glBindTexture(GL_TEXTURE_2D, tID); RenderTriangles(v, mvp, Shaders::shTexRect); }
		
		void renderRect(float x, float y, float w, float h, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), float scale=1.0f, bool center=false)
		{	Array<Vertex> tmpV; genRect(tmpV, w, h, 0xFFFFFFFF, x, y, true, center); _renderRect(tmpV, x, y, w, h, mvp, scale); }
		
	};
	
	struct Model { Array<Vertex> v, l; Array<unsigned int> idx; unsigned int VBO, IBO, LnVBO;
		void (Model::*PFN_render)(const Matrix4& mvp, const ShaderInfo& shinfo) const; // Указатель на метод-член класса
		Model() : VBO(0), IBO(0), LnVBO(0), PFN_render(&Model::renderDef){} ~Model(){ freeVRAM(); }
		
		// struct { const Model* _this;
			// void operator()(const Matrix4& mvp = Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo = Shaders::shDef) const 
			// { (_this->*PFN_render)(mvp, shinfo); } } render; //Функтор-хак для простого вызова указателя члена-класса
		
		void render(const Matrix4& mvp = Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo = Shaders::shDef) const 
		{	(this->*PFN_render)(mvp, shinfo); }
		void render(const Entity& entity, const Matrix4& vp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef) const 
		{   render(vp*entity.getMatrix(), shinfo); }
			
		void renderStatic(const Matrix4& mvp, const ShaderInfo& shinfo=Shaders::shDef) const {
			glUseProgram(shinfo.program); glUniformMatrix4fv(shinfo.mvpLoc, 1, GL_FALSE, mvp.m);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glEnableVertexAttribArray(shinfo.posLoc); glEnableVertexAttribArray(shinfo.colorLoc);
			glVertexAttribPointer(shinfo.posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(shinfo.colorLoc, 4, ColorGL_T, ColorGL_Norm, sizeof(Vertex), (void*)offsetof(Vertex, _color));
			
			if(IBO){ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); glDrawElements(GL_TRIANGLES, idx.size(), GL_UNSIGNED_INT, 0); }
			else{ glDrawArrays(GL_TRIANGLES, 0, v.size()); }
			if(LnVBO){ glBindBuffer(GL_ARRAY_BUFFER, LnVBO); glDrawArrays(GL_LINES, 0, l.size()); }

			glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc);
			glBindBuffer(GL_ARRAY_BUFFER, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
		
		void renderDef(const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef) const {
			if(idx.empty()){ RenderTriangles(v, mvp, shinfo); }else{ RenderTrianglesIdx(v, idx, mvp, shinfo); }
			if(!l.empty()){ RenderLines(l, mvp, shinfo); } }
			
		#ifdef NCPP_GL_VERTEX_EXT
		void renderTexture(const Texture& tx, const Matrix4& mvp=Matrix4().setMatrix2DPreset()) const { const ShaderInfo& shinfo=Shaders::shTex;
			glUseProgram(shinfo.program); glEnableVertexAttribArray(shinfo.uvLoc);
			glVertexAttribPointer(shinfo.uvLoc, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, u));
			glBindTexture(GL_TEXTURE_2D, tx.tID); RenderTriangles(v, mvp, shinfo); glDisableVertexAttribArray(shinfo.uvLoc); }
		
		void renderShadows(const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef) const;
		#else
		void renderTexture(const Texture& tx, const Matrix4& mvp=Matrix4().setMatrix2DPreset()) const;
		void renderShadows(const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef) const;
		#endif
		
		Model& translate(float x, float y, float z=0){ Vertex::translate(v, x, y, z); if(!l.empty()) Vertex::translate(l, x, y, z); return *this; }
		Model& rotateX(float aX){ Vertex::rotateX(v, aX); if(!l.empty()) Vertex::rotateX(l, aX); return *this; }
		Model& rotateY(float aY){ Vertex::rotateY(v, aY); if(!l.empty()) Vertex::rotateY(l, aY); return *this; }
		Model& rotateZ(float aZ){ Vertex::rotateZ(v, aZ); if(!l.empty()) Vertex::rotateZ(l, aZ); return *this; } //rotateX(v, aX); rotateY(v, aY); rotateZ(v, aZ);
		Model& rotate(float aX, float aY, float aZ){ Vertex::rotate(v, aX, aY, aZ); if(!l.empty()) Vertex::rotate(l, aX, aY, aZ); return *this; }
		
		Model& applyMatrix(const Matrix4& M){ Matrix4::applyMatrix(v, M); if(!l.empty()) Matrix4::applyMatrix(l, M); return *this; }
		
		Model& setVertexColor(unsigned int hex){ Vertex::setColors(v, hex); return *this; }
		Model& setLinesColor(unsigned int hex){ Vertex::setColors(l, hex); return *this; }
		Model& randVertexColors(){ Vertex::randColors(v); return *this; }
		Model& randLinesColors(){ Vertex::randColors(l); return *this; }
		
		bool inVRAM(){ return VBO||IBO||LnVBO; }
		
		Model& pushVRAM(){ PFN_render = &Model::renderStatic; //if(inVRAM()) return *this;
			if(!v.empty()){ if(!VBO) glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(Vertex), v.data(), GL_STATIC_DRAW); }
			if(!idx.empty()){ if(!IBO) glGenBuffers(1, &IBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size()*sizeof(unsigned int), idx.data(), GL_STATIC_DRAW); }
			if(!l.empty()){ if(!LnVBO) glGenBuffers(1, &LnVBO);
				glBindBuffer(GL_ARRAY_BUFFER, LnVBO);
				glBufferData(GL_ARRAY_BUFFER, l.size()*sizeof(Vertex), l.data(), GL_STATIC_DRAW); }

			glBindBuffer(GL_ARRAY_BUFFER, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); return *this; }

		Model& freeVRAM(){ PFN_render = &Model::renderDef; if(!inVRAM()) return *this; 
			if(VBO){ glDeleteBuffers(1, &VBO); VBO=0; }
			if(IBO){ glDeleteBuffers(1, &IBO); IBO=0; }
			if(LnVBO){ glDeleteBuffers(1, &LnVBO); LnVBO=0; } return *this; }
			
		Model& freeRAM(){ v.clear().shrink(); l.clear().shrink(); idx.clear().shrink(); return *this; }
		
		bool loadOBJ(const CString& fname);
	};
	
	struct GConsole { String inpbuff, outbuff; GUI::GLWindow* gwnd; bool enabled;
		GConsole(GUI::GLWindow* glwnd=NULL) : gwnd(glwnd), enabled(false), onCommand(NULL){}
		
		bool keysHandle(const GUI::WEvent& ev){ if(ev.key==301){ enabled=!enabled; return false; } if(!enabled) return false; // (`)
			if(ev.key==314){ inpbuff.pop(); return true; } //Backspace
			if(ev.key==320){ if(onCommand) onCommand(inpbuff); inpbuff.clear(); return true; } //Enter
			if(ev.x!=0){ inpbuff.push(ev.x); } return true; }
		
		void clear(){ outbuff.clear(); }
		void log(const String& s){ *this << s << "\n"; }
		GConsole& operator<<(const String& s){ outbuff << s; return *this; }
		GConsole& operator<<(long long num){ outbuff << num; return *this; }
		
		void render(){ if(!enabled) return; float aspect=1.0f; if(gwnd){ aspect=gwnd->aspect; }
			RenderString("Console> "+inpbuff+"\n\n"+outbuff,0.005f,0.05f, 0.02f, 0xFFFFFFFF, 0, aspect); }
		
		void (*onCommand)(const String& cmdtxt);
		static void defCmdHandle(const String& cmdtxt, GConsole& cons){ if(cmdtxt=="cls"||cmdtxt=="clear"){ cons.outbuff.clear(); }
			if(cmdtxt.startsWith("echo")){ Array<String> cmd = cmdtxt.splitTokens(); cons.outbuff << cmd.slice(1).join(" ") << "\n"; } }
		
	};
	
	bool Model::loadOBJ(const CString& fname){ Array<String> lines = fs::readLines(fname);
	#ifdef NCPP_GL_VERTEX_EXT
		Array<Vector2> vt; Array<Vector3> vn; //Array<unsigned int> _vtIdx, _vnIdx;
	#endif
		for(size_t i=0;i<lines.size();i++){ String& line = lines[i]; if(line[0]=='#') continue;
			if(line.startsWith("v ")){ Array<String> parts = line.splitTokens(); if(parts.size()<4) continue;
				v.push(Vertex(stofn(parts[1]), stofn(parts[2]), stofn(parts[3]))); }
			#ifdef NCPP_GL_VERTEX_EXT
			else if(line.startsWith("vt ")){ Array<String> parts = line.splitTokens(); if(parts.size()<3) continue;
				vt.push(Vector2(stofn(parts[1]), stofn(parts[2]))); }
			else if(line.startsWith("vn ")){ Array<String> parts = line.splitTokens(); if(parts.size()<4) continue;
				vn.push(Vector3(stofn(parts[1]), stofn(parts[2]), stofn(parts[3]))); }
			#endif
			else if(line.startsWith("f ")){ Array<String> parts = line.splitTokens(); if(parts.size()<4) continue; //f v/vt/vn
				for(size_t f=1;f<=3;f++){ if(line.indexOf("/")==NPOS){ idx.push(stoin(parts[f])-1); continue; }
					Array<String> sub = parts[f].split("/"); unsigned int vIdx = stoin(sub[0])-1; idx.push(vIdx);
				#ifdef NCPP_GL_VERTEX_EXT
					unsigned int vtIdx = sub.size()>1?(stoin(sub[1])-1):0, vnIdx = sub.size()>2?(stoin(sub[2])-1):0;
					v[vIdx].setUV(vt[vtIdx].x, vt[vtIdx].y); v[vIdx].setVN(vn[vnIdx].x, vn[vnIdx].y, vn[vnIdx].z);
				#endif
				} }
			else if(line.startsWith("l ")){ Array<String> parts = line.splitTokens(); if(parts.size()<3) continue; 
				for(size_t f=1;f<=parts.size()-1;f++){ l.push(v[stoin(parts[f])-1]); } }
			else{ continue; }
		} return true; }
} }

//Свет: Модель Блинна-Фонга?

//Вместо того чтобы делать мир огромным вокруг нулевой точки (0,0,0), мы делаем камеру центром вселенной.
//dvec3 playerPos = {12500000.5, 64.0, -800000.2}; 
//dvec3 objectPos = {12500000.9, 65.0, -800000.1};
//vec3 relativePos = vec3(objectPos - playerPos); // Вычисляем разницу (результат всегда маленький float)
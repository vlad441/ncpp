#include "gl-math.cpp"
// ========= GL Engine =========
namespace ncpp { namespace GL { unsigned int VBO, IBO, VAO;
	void clear(unsigned int hex=0, float alphaf=1){ if(hex==0){ clear(0.0f, 0.0f, 0.0f, alphaf); return; } float rgb[3]; HexToRGBf(rgb, hex); clear(rgb[0], rgb[1], rgb[2], alphaf); }
	//=== OpenGL 2.0 API ===
	struct ShaderInfo { GLuint program; unsigned int vShader, fShader; GLint mvpLoc, posLoc, colorLoc, rectParamsLoc; 
		ShaderInfo() : program(-1), vShader(0), fShader(0){}
		
		bool compile(const char* vShader_src, const char* fShader_src){ //Вершинный шейдер (GLSL 1.10)
			GLint isCompiled; if(!glCreateShader){ if(!glCreateShader){ Except("ShaderInfo::compile ERROR: glCreateShader is a NULL.\n"); return false; } }
			vShader = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vShader, 1, &vShader_src, NULL); glCompileShader(vShader);
			fShader = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fShader, 1, &fShader_src, NULL); glCompileShader(fShader);
			glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: vShader compile failed.\n"); return false; }
			glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled); if(isCompiled == GL_FALSE){ Except("ShaderInfo::compile: fShader compile failed.\n"); return false; }
			return true; }
		bool LinkProg(){ if(program!=(GLuint)-1) return false; program = glCreateProgram(); 
			glAttachShader(program, vShader); glAttachShader(program, fShader); glLinkProgram(program);
			GLint linked; glGetProgramiv(program, GL_LINK_STATUS, &linked); if(!linked) return false;
			glDeleteShader(vShader); glDeleteShader(fShader); return true; }
		bool initLoc(){ if(!LinkProg()) return false;  
			mvpLoc = glGetUniformLocation(program, "uMVP");
			posLoc = glGetAttribLocation(program, "pos");
			colorLoc = glGetAttribLocation(program, "color"); 
			//GLint baseColorLoc = glGetUniformLocation(shinfo.program, "uBaseColor");
			return true; }
		bool initRectParamsLoc(){ rectParamsLoc = glGetUniformLocation(program, "uRectParams"); return true; }
	};
	namespace Shaders { ShaderInfo shDef; //стандартные шейдеры
		ShaderInfo shDef_sim; //Шейдер-пример
		ShaderInfo shDefTex; //Шейдер с поддержкой текстур
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
		//=== Шейдер с поддержкой текстур ===
		// --- Vetex ---
		const char* vShader_src = "#version 110\n"
			"attribute vec3 pos;\n"
			"attribute vec4 color;\n"
			"varying vec4 vColor;\n"
			"uniform mat4 uMVP;\n"
			"varying vec2 vTexCoord; //UV cords\n"
			"uniform vec4 uRectParams; //x: minX, y: minY, z: width, w: height\n"
			"void main(){\n"
			"  gl_Position = uMVP * vec4(pos, 1.0); vColor = color;\n"
			"  vTexCoord = (pos.xy - uRectParams.xy) / uRectParams.zw; // Вычисляем UV на основе позиции\n"
			"  vTexCoord.y = 1.0-((pos.y - uRectParams.y) / uRectParams.w); } // Инверсия UV для y\n";
		// --- Fragment ---
		const char* fShader_src = "#version 110\n"
			"varying vec4 vColor; // Цвет от вершин (интерполированный)\n"
			"varying vec2 vTexCoord;\n"
			"uniform sampler2D uTexture;\n"
			"void main(){\n"
			"  gl_FragColor = texture2D(uTexture, vTexCoord) * vColor;\n"
			"}\n";
		if(!shDefTex.compile(vShader_src, fShader_src)) return false; return shDefTex.initLoc()&&shDefTex.initRectParamsLoc(); }
		
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
	void SetFillMode(bool mode=true){ FillMode=mode; if(!mode){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } }
	
	void _applyState(const Array<Vertex>& vertices, const Matrix4& mvp, GLenum mode, const ShaderInfo& shinfo){
		glUseProgram(shinfo.program); glUniformMatrix4fv(shinfo.mvpLoc, 1, GL_FALSE, mvp.m); //передача MVP в шейдер.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);
		
		glEnableVertexAttribArray(shinfo.posLoc); glEnableVertexAttribArray(shinfo.colorLoc);
		glVertexAttribPointer(shinfo.posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Указываем откуда брать позицию
		glVertexAttribPointer(shinfo.colorLoc, 4, ColorGL_T, ColorGL_Norm, sizeof(Vertex), (void*)offsetof(Vertex, _color)); } // Указываем откуда брать цвет
	
	void RenderVertices(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_POINTS, const ShaderInfo& shinfo=Shaders::shDef){ 
		if(vertices.empty()) return; _applyState(vertices, mvp, mode, shinfo); 
		glDrawArrays(mode, 0, vertices.size()); glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc); 
	}
	void RenderTriangles(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_TRIANGLES, shinfo); }
	void RenderLines(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_LINES, shinfo); }
	
	void RenderTriangleStrip(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStrip(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_LINE_STRIP, shinfo); }
		
	void RenderVerticesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_TRIANGLES, const ShaderInfo& shinfo=Shaders::shDef){
		if(vertices.empty() || indices.empty()) return; _applyState(vertices, mvp, mode, shinfo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STREAM_DRAW);
		glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0); glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc); 
	}
	void RenderLinesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_LINES, shinfo); }
	void RenderTrianglesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_TRIANGLES, shinfo); }
	
	void RenderTriangleStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_LINE_STRIP, shinfo); }
		
	struct Entity { double x, y, z; float aX, aY, aZ; float sX, sY, sZ;
		Entity(double x1=0, double y1=0, double z1=0) : x(x1), y(y1), z(z1), aX(0), aY(0), aZ(0), sX(1), sY(1), sZ(1){}
		Entity& setPos(double x1, double y1, double z1){ x=x1; y=y1; z=z1; return *this; }
		Entity& setAngle(float aX1, float aY1, float aZ1){ aX=aX1; aY=aY1; aZ=aZ1; return *this; }
		Entity& move(float x1, float y1, float z1){ x+=x1; y+=y1; z+=z1; return *this; }
		Entity& move(const Vector3& v){ x+=v.x; y+=v.y; z+=v.z; return *this; }
		Entity& rotate(float aX1, float aY1, float aZ1){ aX+=aX1; aY+=aY1; aZ+=aZ1; 
			if(aY>360.0f) aY-=360; if(aY<-360.0f) aY+=360; if(aZ>360.0f) aZ-=360; if(aZ<-360.0f) aZ+=360; return *this; }
		Entity& scale(float scaleVal){ sX=sY=sZ=scaleVal; return *this; }
		Entity& scale(float sX1, float sY1, float sZ1){ sX=sX1; sY=sY1; sZ=sZ1; return *this; }
			
		Matrix4 getMatrix() const { Matrix4 m; m.translate(x, y, z).rotate(aX, aY, aZ).scale(sX, sY, sZ); return m; } // M = T * R * S
		
		Vector3 getForward() const { Matrix4 m; m.rotate(aX, aY, aZ); return Vector3(-m.m[8], -m.m[9], -m.m[10]).norm(); }
		Vector3 getRight() const { Matrix4 m; m.rotate(aX, aY, aZ); return Vector3(m.m[0], m.m[1], m.m[2]).norm(); }
		Vector3 getUp() const { Matrix4 m; m.rotate(aX, aY, aZ); return Vector3(m.m[4], m.m[5], m.m[6]).norm(); }
		
		Entity& moveForward(float dist){ return move(getForward()*dist); }
		Entity& moveRight(float dist){ return move(getRight()*dist); }
		Entity& moveUp(float dist){ return move(getUp()*dist); }
	};

	struct Camera : Entity { float sensitivity; float speed; float angleFog; float aspect; char mouseCapt; 
		Camera(double x1=0, double y1=0, double z1=0) : Entity(x1, y1, z1), sensitivity(1.0f), speed(1.0f), angleFog(60.0f), aspect(1.0f), mouseCapt(0){}
		
		Camera& rotate(float aX1, float aY1, float aZ1){ return (Camera&)Entity::rotate(aX1, aY1, aZ1); }
		//Matrix4 getMatrix() const { Matrix4 V; V.rotate(-aX, -aY, -aZ).translate(-x, -y, -z); return V; } //Old Abs Pos+Angle
		Matrix4 getMatrix() const { Matrix4 V; V.rotateX(-aX).rotateY(-aY).rotateZ(-aZ).translate(-x, -y, -z); return V; }
		
		void captureMouse(GUI::GLWindow& glw){ if(!mouseCapt) return; int w=0, h=0; glw.getSize(w, h); int centerX = w/2; int centerY = h/2;
			if(mouseCapt==1){ glw.setMousePos(centerX, centerY); mouseCapt=2; return; }
			int mX=0, mY=0; glw.getMousePos(mX, mY); float dx = (float)(mX - centerX); float dy = (float)(mY - centerY);
			if (dx!=0 || dy!=0){ rotate(-dy*sensitivity, -dx*sensitivity, 0); glw.setMousePos(centerX, centerY); }
			if(aX > 89.9f) aX = 89.9f; if(aX < -89.9f) aX = -89.9f; } // Ограничение, чтобы не было Gimbal Lock
		
		bool Controls_FreeCam(const GUI::WEvent& ev){ switch(ev.key){
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
	};
	
	struct Model : Entity { Array<Vertex> v, l; Array<unsigned int> idx; enum RenderMode { TRIANGLES, LINES } mode; //unsigned int VBO, IBO;
		Model(double x1=0, double y1=0, double z1=0) : Entity(x1, y1, z1), mode(TRIANGLES){}
		void render(const Matrix4& model, const Matrix4& vp, const ShaderInfo& shinfo=Shaders::shDef){
			switch(mode){ 
				case TRIANGLES: if(idx.empty()){ RenderTriangles(v, vp*model, shinfo); }else{ RenderTrianglesIdx(v, idx, vp*model, shinfo); } break;
				case LINES: if(idx.empty()){ RenderLines(v, vp*model, shinfo); }else{ RenderLinesIdx(v, idx, vp*model, shinfo); } break; break; } 
			if(!l.empty()){ RenderLines(l, vp*model, shinfo); } } //borders (lines)
		void render(const Entity& entity, const Matrix4& vp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef)
		{ Matrix4 model=entity.getMatrix(); render(model, vp, shinfo); }
		void render(const Matrix4& vp=Matrix4().setMatrix2DPreset()){ Matrix4 model=getMatrix(); render(model, vp); }
		
		static void moveVertices(Array<Vertex>& v, float x, float y, float z=0){ for(size_t i=0;i<v.size();i++){ v[i].x+=x; v[i].y+=y; v[i].z+=z; } }
		static void rotateVerticesX(Array<Vertex>& v, float aX){ float rad = aX * (float)M_PI / 180.0f; float c = cosf(rad);  float s = sinf(rad);
			for(size_t i=0; i<v.size(); i++){ float oldY = v[i].y; float oldZ = v[i].z; v[i].y = oldY*c-oldZ*s; v[i].z = oldY*s+oldZ*c; } }
		static void rotateVerticesY(Array<Vertex>& v, float aY){ float rad = aY * M_PI / 180.0f; float c = cosf(rad); float s = sinf(rad);
			for(size_t i=0; i<v.size(); i++){ float oldX = v[i].x; float oldZ = v[i].z; v[i].x = oldX*c-oldZ*s; v[i].z = oldX*s+oldZ*c; } }
		static void rotateVerticesZ(Array<Vertex>& v, float aZ){ float rad = aZ * (float)M_PI / 180.0f; float c = cosf(rad); float s = sinf(rad);
			for(size_t i=0; i<v.size(); i++){ float oldX = v[i].x; float oldY = v[i].y; v[i].x = oldX*c - oldY*s; v[i].y = oldX*s + oldY*c; } }
		static void rotateVertices(Array<Vertex>& v, float aX, float aY, float aZ){ rotateVerticesX(v, aX); rotateVerticesY(v, aY); rotateVerticesZ(v, aZ); }
		
		Model& moveVertices(float x1, float y1, float z1=0){ moveVertices(v, x1, y1, z1); return *this; }
		Model& applyMatrix(const Matrix4& M){ Vertex::applyMatrix(v, M); return *this; }
		
		Model& setVertexColor(unsigned int hex){ for(size_t i=0;i<v.size();i++){ v[i].setColor(hex); } return *this; }
		Model& setLinesColor(unsigned int hex){ for(size_t i=0;i<l.size();i++){ l[i].setColor(hex); } return *this; }
		Model& randVertexColor(){ for(size_t i=0;i<v.size();i++){ v[i].setColor(0xFF000000+randInt(0, 0xFFFFFF)); } return *this; }
		Model& randLinesColor(){ for(size_t i=0;i<v.size();i++){ l[i].setColor(0xFF000000+randInt(0, 0xFFFFFF)); } return *this; }
		
		Model& pushVRAM();
		Model& freeVRAM();
		Model& freeRAM(){ v.clear().shrink(); l.clear().shrink(); idx.clear().shrink(); return *this; }
		
		bool loadOBJ(const CString& fname);
	};
	
} }
#include "gl-vfont.cpp"
#include "gl-geometry.cpp"

namespace ncpp { namespace GL {
	struct Texture { unsigned int tID; Texture() : tID(0){}
		void load(void* px, int width, int height){ if(tID!=0) free(); glGenTextures(1, &tID); glBindTexture(GL_TEXTURE_2D, tID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, px); }
		void free(){ if(tID==0) return; glDeleteTextures(1, &tID); tID = 0; }
		
		//unsigned int loadImg(const Image& img){ tyan.load(img.px.data(), img.w, img.h); }
		//unsigned int loadImg(const CString& fpath){ loadImg(Image::fromFile(fpath)); }
		
		void render(const Array<Vertex>& v, float x, float y, float w, float h, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), float scale=1.0f){
			glUseProgram(Shaders::shDefTex.program); glUniform4f(Shaders::shDefTex.rectParamsLoc, x, y, w*scale, h*scale);
			glBindTexture(GL_TEXTURE_2D, tID); RenderTriangles(v, mvp, Shaders::shDefTex); }
		
		void renderRect(float x, float y, float w, float h, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), float scale=1.0f)
		{	Array<Vertex> tmpV; genRect(tmpV, w, h, 0xFFFFFFFF, x, y, true, false); render(tmpV, x, y, w, h, mvp, scale); }
	};
	
	struct GConsole { String inpbuff, outbuff; bool enabled;
		GConsole() : enabled(false), onCommand(NULL){}
		
		bool keysHandle(const GUI::WEvent& ev){ if(ev.key==301){ enabled=!enabled; return false; } if(!enabled) return false; // (`)
			if(ev.key==314){ inpbuff.pop(); return true; } //Backspace
			if(ev.key==320){ if(onCommand) onCommand(inpbuff); inpbuff.clear(); return true; } //Enter
			if(ev.x!=0){ inpbuff.push(ev.x); } return true; }
		
		void clear(){ outbuff.clear(); }
		void log(const String& s){ *this << s << "\n"; }
		GConsole& operator<<(const String& s){ outbuff << s; return *this; }
		GConsole& operator<<(long long num){ outbuff << num; return *this; }
		
		void render(float aspect=1.0f){ if(!enabled) return; RenderString("Console> "+inpbuff+"\n\n"+outbuff,0.005f,0.85f, 0.02f, 0xFFFFFFFF, aspect); }
		
		void (*onCommand)(const String& cmdtxt);
		static void defCmdHandle(const String& cmdtxt, GConsole& cons){ if(cmdtxt=="cls"||cmdtxt=="clear"){ cons.outbuff.clear(); }
			if(cmdtxt.startsWith("echo")){ Array<String> cmd = cmdtxt.splitTokens(); cons.outbuff << cmd.slice(1).join(" ") << "\n"; } }
		
	};
	
	bool Model::loadOBJ(const CString& fname){ Array<String> lines = fs::readLines(fname);
		for(size_t i=0;i<lines.size();i++){ String& line = lines[i]; if(line[0]=='#') continue;
			if(line.startsWith("v ")){ Array<String> parts = line.splitTokens(); if(parts.size() < 4) continue;
				v.push(Vertex(stofn(parts[1]), stofn(parts[2]), stofn(parts[3]))); }
			else if(line.startsWith("f ")){ Array<String> parts = line.splitTokens(); if(parts.size() < 4) continue;
				for(size_t f=1;f<=3;f++){ if(line.indexOf("/")!=NPOS){ Array<String> sub = parts[f].split("/"); idx.push(stoin(sub[0])-1); continue; }
					idx.push(stoin(parts[f])-1); } }
			else if(line.startsWith("l ")){ Array<String> parts = line.splitTokens(); if(parts.size() < 3) continue; 
				for(size_t f=1;f<=parts.size()-1;f++){ l.push(v[stoin(parts[f])-1]); } }
			else{ continue; }
		} return true; }
} }

//Свет: Модель Блинна-Фонга?

//Вместо того чтобы делать мир огромным вокруг нулевой точки (0,0,0), мы делаем камеру центром вселенной.
//dvec3 playerPos = {12500000.5, 64.0, -800000.2}; 
//dvec3 objectPos = {12500000.9, 65.0, -800000.1};
//vec3 relativePos = vec3(objectPos - playerPos); // Вычисляем разницу (результат всегда маленький float)
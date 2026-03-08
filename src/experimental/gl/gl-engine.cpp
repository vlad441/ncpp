#include "gl-math.cpp"
// ========= GL Engine =========
namespace ncpp { namespace GL { unsigned int VBO, VAO, IBO;
	void clear(unsigned int hex=0, float alphaf=1){ if(hex==0){ clear(0.0f, 0.0f, 0.0f, alphaf); return; } float rgb[3]; HexToRGBf(rgb, hex); clear(rgb[0], rgb[1], rgb[2], alphaf); }
	//=== OpenGL 2.0 API ===
	struct ShaderInfo { GLuint program; unsigned int vShader, fShader; GLint mvpLoc, posLoc, colorLoc; 
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
	};
	namespace Shaders { ShaderInfo shDef; //стандартные шейдеры
		ShaderInfo shDef_sim; } //примитивные шейдеры
	
	bool InitSimpleShaders(){ //Использует GLSL 1.10
		//Вершинный шейдер: Возвращает вершины без изменений
		const char* vShader_src = "#version 110\n"
			"attribute vec2 pos;\n"
			"void main(){ gl_Position = vec4(pos, 0.0, 1.0); }";
		//Фрагментный шейдер: Красит все пиксели в красный цвет
		const char* fShader_src = "#version 110\n" "void main(){ gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";
		if(!Shaders::shDef_sim.compile(vShader_src, fShader_src)) return false; return true; }
		
	bool InitMinimalShaders(){ //Вершинный шейдер (GLSL 1.10)
		const char* vShader_src = "#version 110\n"
			"attribute vec3 pos;\n"
			"attribute vec4 color;\n"
			"varying vec4 vColor;\n"
			"uniform mat4 uMVP;\n"
			"void main(){ gl_Position = uMVP * vec4(pos, 1.0); vColor = color; }\n";

		// Фрагментный шейдер
		const char* fShader_src = "#version 110\n"
			"varying vec4 vColor; // Цвет от вершин (интерполированный)\n"
			"//uniform vec4 uBaseColor; // Базовый цвет из glUniform4f\n"
			"void main(){ gl_FragColor = vColor; }\n"; //gl_FragColor = vColor * uBaseColor;
		if(!Shaders::shDef.compile(vShader_src, fShader_src)) return false; return Shaders::shDef.initLoc(); }
		
	bool InitDefaultShaders(){ if(!LoadOGL_20()){ print("(!) GL::InitDefaultShaders failed: OpenGL 2.0 extensions unavailable.\n"); return false; }
		if(!LoadOGL_30()){ print("(!) Loading extensions OpenGL 3.0 failed.\n"); }
		if(!LoadOGL_31()){ print("(!) Loading extensions OpenGL 3.1 failed.\n"); }
		if(!InitSimpleShaders()) return false; if(!InitMinimalShaders()) return false; 
		
		glGenBuffers(1, &VBO); glBindBuffer(GL_ARRAY_BUFFER, VBO); //glBufferData(GL_ARRAY_BUFFER, MAX_SIZE, NULL, GL_DYNAMIC_DRAW);
		if(glGenVertexArrays){ glGenVertexArrays(1, &VAO); glBindVertexArray(VAO); } //VAO для новых драйверов.
		glGenBuffers(1, &IBO); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); return true; }
	
	bool FillMode=true;
	//void SetLineWidth(float sz=1.0f){ glLineWidth(sz); } //deprecated
	void SetFillMode(bool mode=true){ FillMode=mode; if(!mode){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); } }
	
	//void _applyState(const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_POINTS, const ShaderInfo& shinfo=Shaders::shDef);
	
	void RenderVertices(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_POINTS, const ShaderInfo& shinfo=Shaders::shDef){ 
		if(vertices.empty()) return; glUseProgram(shinfo.program); glUniformMatrix4fv(shinfo.mvpLoc, 1, GL_FALSE, mvp.m); //передача MVP в шейдер.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);
		
		glEnableVertexAttribArray(shinfo.posLoc); glEnableVertexAttribArray(shinfo.colorLoc);
		glVertexAttribPointer(shinfo.posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Указываем откуда брать позицию
		glVertexAttribPointer(shinfo.colorLoc, 4, ColorGL_T, ColorGL_Norm, sizeof(Vertex), (void*)offsetof(Vertex, _color)); // Указываем откуда брать цвет
		
		glDrawArrays(mode, 0, vertices.size()); glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc);
	}
	void RenderTriangles(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_TRIANGLES, shinfo); }
	void RenderLines(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_LINES, shinfo); }
	
	void RenderTriangleStrip(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStrip(const Array<Vertex>& vertices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVertices(vertices, mvp, GL_LINE_STRIP, shinfo); }
		
	void RenderVerticesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), GLenum mode=GL_TRIANGLES, const ShaderInfo& shinfo=Shaders::shDef){
		if(vertices.empty() || indices.empty()) return; glUseProgram(shinfo.program); glUniformMatrix4fv(shinfo.mvpLoc, 1, GL_FALSE, mvp.m); //передача MVP в шейдер.
		glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STREAM_DRAW);

		glEnableVertexAttribArray(shinfo.posLoc); glEnableVertexAttribArray(shinfo.colorLoc);
		glVertexAttribPointer(shinfo.posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(shinfo.colorLoc, 4, ColorGL_T, ColorGL_Norm, sizeof(Vertex), (void*)offsetof(Vertex, _color));

		glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0); glDisableVertexAttribArray(shinfo.posLoc); glDisableVertexAttribArray(shinfo.colorLoc); 
	}
	void RenderLinesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_LINES, shinfo); }
	void RenderTrianglesIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_TRIANGLES, shinfo); }
	
	void RenderTriangleStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_TRIANGLE_STRIP, shinfo); }
	void RenderLineStripIdx(const Array<Vertex>& vertices, const Array<unsigned int>& indices, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){ RenderVerticesIdx(vertices, indices, mvp, GL_LINE_STRIP, shinfo); }
	
	void addRect(Array<Vertex>& v, float width=1, float height=1, unsigned int color=0xFFFFFFFF, float x=0, float y=0, bool fill=true, bool center=true){
		Vertex v1(x, y, 0); Vertex v2(x + width, y + height, 0); unsigned int color1=color, color2=color;
		if(center){ v1.x -= width/2.0f; v2.x -= width/2.0f; v1.y -= height/2.0f; v2.y -= height/2.0f; }
		if(fill){
			v.push(Vertex(v1.x, v1.y, v1.z, color1)); // ЛВ
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); // ПВ (цвет от ЛВ)
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); // ЛН (цвет от ПН)
			// Треугольник 2
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); // ПВ
			v.push(Vertex(v2.x, v2.y, v2.z, color2)); // ПН
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); // ЛН
		}else{
			v.push(v1); v.push(Vertex(v2.x, v1.y, v1.z, color1)); // Верхняя линия
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); v.push(v2); // Правая линия
			v.push(v2); v.push(Vertex(v1.x, v2.y, v2.z, color2)); // Нижняя линия
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); v.push(v1); } // Левая линия
	}
	
	void addCube(Array<Vertex>& v, Array<unsigned int>& idx, float width=1, float height=1, float l=1, unsigned int color=0xFFFFFFFF, 
		float x=0, float y=0, float z=0, bool fill=true, bool center=true, bool indexed=true)
	{	float x1 = x, y1 = y, z1 = z; float x2 = x + width, y2 = y + height, z2 = z + l;

		if(center){ float dx = width/2.0f; float dy = height/2.0f; float dz = l/2.0f; x1-=dx; x2-=dx; y1-=dy; y2-=dy; z1-=dz; z2-=dz; }
		Vertex vertices[8] = { // 8 вершин куба
			Vertex(x1, y1, z1, color), // 0: ЛНБ (Лево-Низ-Ближ)
			Vertex(x2, y1, z1, color), // 1: ПНБ
			Vertex(x2, y2, z1, color), // 2: ПВБ
			Vertex(x1, y2, z1, color), // 3: ЛВБ
			Vertex(x1, y1, z2, color), // 4: ЛНД (Лево-Низ-Даль)
			Vertex(x2, y1, z2, color), // 5: ПН开
			Vertex(x2, y2, z2, color), // 6: ПВ开
			Vertex(x1, y2, z2, color)  // 7: ЛВ开
		};

		if(fill){
			unsigned int indices[] = { // Индексы для 12 треугольников (6 граней)
				0, 1, 2,  2, 3, 0,   // Передняя
				1, 5, 6,  6, 2, 1,   // Правая
				7, 6, 5,  5, 4, 7,   // Задняя
				4, 0, 3,  3, 7, 4,   // Левая
				4, 5, 1,  1, 0, 4,   // Нижняя
				3, 2, 6,  6, 7, 3 }; // Верхняя
			if(indexed){ for(int i=0; i<36; i++) idx.push((unsigned int)v.size() + indices[i]); for(int i=0; i<8; i++) v.push(vertices[i]); }
			else{ for(int i=0; i<36; i++){ v.push(vertices[indices[i]]); } } // No Indexed (Native)
		}else{
			unsigned int edges[] = { // Проволочный каркас (12 ребер)
				0, 1, 1, 2, 2, 3, 3, 0,   // Передний квадрат
				4, 5, 5, 6, 6, 7, 7, 4,   // Задний квадрат
				0, 4, 1, 5, 2, 6, 3, 7 }; // Перемычки
			if(indexed){ for(int i=0; i<24; i++) idx.push((unsigned int)v.size() + edges[i]); for(int i=0; i<8; i++) v.push(vertices[i]); }
			else{ for(int i=0; i<24; i++){ v.push(vertices[edges[i]]); } } // No Indexed (Native)
		}
	}
	
	void RenderRects(const Array<Vertex>& v, const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef){
		if(FillMode){ RenderTriangles(v, mvp, shinfo); }else{ RenderLines(v, mvp, shinfo); } }
	
	// void TestDraw(){ for(int i = 0; i < 100; i++){ 
		// glUniformMatrix4fv(location, 1, GL_FALSE, &players[i].mvp); // Обновляем матрицу 
		// glDrawArrays(GL_TRIANGLES, 0, 10000); } } // Рисуем одного игрока
		
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

	struct Camera : Entity { float sensitivity; float speed; char mouseCapt;
		Camera(double x1=0, double y1=0, double z1=0) : Entity(x1, y1, z1), sensitivity(1.0f), speed(1.0f), mouseCapt(0){}
		
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
	
	struct Model : Entity { Array<Vertex> v, l; Array<unsigned int> idx; enum RenderMode { TRIANGLES, LINES } mode;
		Model(double x1=0, double y1=0, double z1=0) : Entity(x1, y1, z1), mode(TRIANGLES){}
		void render(const Matrix4& vp=Matrix4().setMatrix2DPreset()){ Matrix4 model=getMatrix(); ShaderInfo shinfo=Shaders::shDef;
			switch(mode){ 
				case TRIANGLES: if(idx.empty()){ RenderTriangles(v, vp*model, shinfo); }else{ RenderTrianglesIdx(v, idx, vp*model, shinfo); } break;
				case LINES: if(idx.empty()){ RenderLines(v, vp*model, shinfo); }else{ RenderLinesIdx(v, idx, vp*model, shinfo); } break; break; } 
			if(!l.empty()){ RenderLines(l, vp*model, shinfo); } } //borders (lines)
		Model& setVertexColor(unsigned int hex){ for(size_t i=0;i<v.size();i++){ v[i].setColor(hex); } return *this; }
		Model& setLinesColor(unsigned int hex){ for(size_t i=0;i<l.size();i++){ l[i].setColor(hex); } return *this; }
		bool loadOBJ(const CString& fname);
	};
	
} }
#include "gl-vfont.cpp"

namespace ncpp { namespace GL {
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
		
		void render(float aspect=1.0f){ if(!enabled) return; DrawString("Console> "+inpbuff+"\n\n"+outbuff,0.005f,0.85f, 0.02f, 0xFFFFFF, aspect); }
		
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
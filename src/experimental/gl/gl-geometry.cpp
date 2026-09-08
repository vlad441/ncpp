namespace ncpp { namespace GL {
	// ------ 2D Геометрия ------
	void genTriangle(Array<Vertex>& v, float size=1, unsigned int color=0xFFFFFFFF, float x=0, float y=0, bool fill=true, bool center=true){
		float h = 0.8660254f * size; float x1, y1, x2, y2, x3, y3;
		if(center){ float r_offset = h/3.0f; x1 = x; y1 = y + (h - r_offset); x2 = x - size / 2.0f; y2 = y - r_offset; x3 = x + size / 2.0f; y3 = y - r_offset; } 
		else{ x1 = x + size / 2.0f; y1 = y + h; x2 = x; y2 = y; x3 = x + size; y3 = y; }
		
		Vertex vTop(x1, y1, 0, color); Vertex vLeft(x2, y2, 0, color); Vertex vRight(x3, y3, 0, color);
		if(fill){ v.push(vTop).push(vRight).push(vLeft); }else{ v.push(vTop).push(vRight).push(vRight).push(vLeft).push(vLeft).push(vTop); } }
	
	void genRect(Array<Vertex>& v, float width=1, float height=1, unsigned int color=0xFFFFFFFF, float x=0, float y=0, bool fill=true, bool center=true){
		Vertex v1(x, y); Vertex v2(x + width, y + height); unsigned int color1=color, color2=color;
		if(center){ v1.x -= width/2.0f; v2.x -= width/2.0f; v1.y -= height/2.0f; v2.y -= height/2.0f; }
		if(fill){
			v.push(Vertex(v1.x, v1.y, v1.z, color1)); // ЛВ
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); // ПВ (цвет от ЛВ)
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); // ЛН (цвет от ПН)
			// Треугольник 2
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); // ПВ
			v.push(Vertex(v2.x, v2.y, v2.z, color2)); // ПН
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); // ЛН
		// #ifdef NCPP_GL_VERTEX_EXT
			// v[0].setUV(0.0f, 0.0f); v[1].setUV(1.0f, 0.0f); v[2].setUV(0.0f, 1.0f); v[3].setUV(1.0f, 0.0f); v[4].setUV(1.0f, 1.0f); v[5].setUV(0.0f, 1.0f);
		// #endif
		}else{
			v.push(v1); v.push(Vertex(v2.x, v1.y, v1.z, color1)); // Верхняя линия
			v.push(Vertex(v2.x, v1.y, v1.z, color1)); v.push(v2); // Правая линия
			v.push(v2); v.push(Vertex(v1.x, v2.y, v2.z, color2)); // Нижняя линия
			v.push(Vertex(v1.x, v2.y, v2.z, color2)); v.push(v1); } // Левая линия
	}
	
	//void glArc(float x, float y, float radius, float startAngle=0, float endAngle=360, float lsize=1, bool fill=false, int segments=100);
	void genArc(Array<Vertex>& v, float d=1, unsigned int color=0xFFFFFFFF, float x=0, float y=0, bool fill=true, int segments=32, float drawCoef=1.0f){
		float step = 2.0f*M_PIf/segments; float r=d/2; int drawSegments=segments*drawCoef;
		if(fill){
			for(int i=0; i < drawSegments; i++){ float a1 = i*step; float a2 = (i+1)*step; v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x + cosf(a1) * r, y + sinf(a1) * r, 0, color)); v.push(Vertex(x + cosf(a2) * r, y + sinf(a2) * r, 0, color)); }
		}else{
			for(int i = 0; i < drawSegments; i++){ float a1 = i*step; float a2 = (i+1)*step;
				v.push(Vertex(x + cosf(a1) * r, y + sinf(a1) * r, 0, color)).push(Vertex(x + cosf(a2) * r, y + sinf(a2) * r, 0, color)); } } }
	//void genCircle();
	
	void genHexagon(Array<Vertex>& v, float d, unsigned int color, float x, float y, bool fill){
		float r = d / 2.0f; float px[6]; float py[6]; int i;
		for(i=0; i<6; i++){ float angle = i*(M_PIf/3.0f); px[i]=x+cosf(angle)*r; py[i] = y + sinf(angle)*r; }
		if(fill){
			v.push(Vertex(px[1], py[1], 0, color)).push(Vertex(px[2], py[2], 0, color)).push(Vertex(px[4], py[4], 0, color)); // Центральный прямоугольник 1
			v.push(Vertex(px[1], py[1], 0, color)).push(Vertex(px[4], py[4], 0, color)).push(Vertex(px[5], py[5], 0, color)); // Центральный прямоугольник 2
			v.push(Vertex(px[2], py[2], 0, color)).push(Vertex(px[3], py[3], 0, color)).push(Vertex(px[4], py[4], 0, color)); // Боковой треугольник 1
			v.push(Vertex(px[5], py[5], 0, color)).push(Vertex(px[0], py[0], 0, color)).push(Vertex(px[1], py[1], 0, color)); // Боковой треугольник 2
		
			// Центр 1
			//v.push(Vertex(px[0], py[0], 0, color)).push(Vertex(px[1], py[1], 0, color)).push(Vertex(px[3], py[3], 0, color));
			// Центр 2
			//v.push(Vertex(px[0], py[0], 0, color)).push(Vertex(px[3], py[3], 0, color)).push(Vertex(px[4], py[4], 0, color));
			// Боковой 1
			//v.push(Vertex(px[1], py[1], 0, color)).push(Vertex(px[2], py[2], 0, color)).push(Vertex(px[3], py[3], 0, color));
			// Боковой 2
			//v.push(Vertex(px[4], py[4], 0, color)).push(Vertex(px[5], py[5], 0, color)).push(Vertex(px[0], py[0], 0, color));
		}else{ for(i=0; i<6; i++){ int next = (i+1) % 6; v.push(Vertex(px[i], py[i], 0, color)); v.push(Vertex(px[next], py[next], 0, color)); } }
	}
    void genLines(Array<Vertex>& v, const Array<Vertex>& l, float bold=1.0f, bool fill=true){ 
		if(l.size() < 2) return; float halfBold = bold*0.5f;
        for(size_t i=0; i+1<l.size();i+=2){ const Vertex& p1 = l[i]; const Vertex& p2 = l[i+1];

            float dx = p2.x-p1.x; float dy = p2.y-p1.y; float len = sqrtf(dx*dx + dy*dy); // Вычисляем вектор направления линии в плоскости XY
            if(len < 1e-6f) continue;
			float nx = (-dy/len)*halfBold; float ny = (dx/len)*halfBold; // Нормализуем и поворачиваем на 90 градусов для получения перпендикуляра

            // Четыре вершины утолщенного прямоугольника линии.
            Vertex v1(p1.x - nx, p1.y - ny, p1.z); v1._color = p1._color;
            Vertex v2(p1.x + nx, p1.y + ny, p1.z); v2._color = p1._color;
            Vertex v3(p2.x + nx, p2.y + ny, p2.z); v3._color = p2._color;
            Vertex v4(p2.x - nx, p2.y - ny, p2.z); v4._color = p2._color;
			
			if(fill){ v.push(v1).push(v2).push(v3); v.push(v1).push(v3).push(v4); }
			else{ v.push(v1).push(v2).push(v2).push(v3).push(v3).push(v4).push(v4).push(v1); }
        }
    }
	
	// ------ 3D Геометрия ------
	// --- Платоновы тела ---
	void genCube(Array<Vertex>& v, Array<unsigned int>& idx, float width=1, float height=1, float l=1, unsigned int color=0xFFFFFFFF, 
		float x=0, float y=0, float z=0, bool fill=true, bool center=true, bool indexed=true)
	{	float x1 = x, y1 = y, z1 = z; float x2 = x + width, y2 = y + height, z2 = z + l;

		if(center){ float dx = width/2.0f; float dy = height/2.0f; float dz = l/2.0f; x1-=dx; x2-=dx; y1-=dy; y2-=dy; z1-=dz; z2-=dz; }
		Vertex vertices[8] = { // 8 вершин куба
			Vertex(x1, y1, z1, color), Vertex(x2, y1, z1, color), Vertex(x2, y2, z1, color), Vertex(x1, y2, z1, color),
			Vertex(x1, y1, z2, color), Vertex(x2, y1, z2, color), Vertex(x2, y2, z2, color), Vertex(x1, y2, z2, color) };

		if(fill){ // Индексы для 12 треугольников (6 граней)
			unsigned int indices[] = {
				0, 1, 2,  2, 3, 0,   1, 5, 6,  6, 2, 1,   7, 6, 5,  5, 4, 7,   4, 0, 3,  3, 7, 4,   4, 5, 1,  1, 0, 4,   3, 2, 6,  6, 7, 3 };
			if(indexed){ for(int i=0; i<36; i++) idx.push((unsigned int)v.size() + indices[i]); for(int i=0; i<8; i++) v.push(vertices[i]); }
			else{ for(int i=0; i<36; i++){ v.push(vertices[indices[i]]); } } // No Indexed (Native)
		}else{ unsigned int edges[] = { 0, 1, 1, 2, 2, 3, 3, 0,  4, 5, 5, 6, 6, 7, 7, 4,  0, 4, 1, 5, 2, 6, 3, 7 }; // Проволочный каркас (12 ребер)
			if(indexed){ for(int i=0; i<24; i++) idx.push((unsigned int)v.size() + edges[i]); for(int i=0; i<8; i++) v.push(vertices[i]); }
			else{ for(int i=0; i<24; i++){ v.push(vertices[edges[i]]); } } } // No Indexed (Native)	
	}
	// ---  ---
	void genSphere(Array<Vertex>& v, float d=1, unsigned int color=0xFFFFFFFF, float x=0, float y=0, float z=0, bool fill=true, int segments=32)
	{	float r = d/2.0f; int rings = segments; int sectors = segments; // rings - количество горизонтальных слоев, sectors - количество сегментов в слое
		float R_step = 1.0f / (float)(rings-1); float S_step = 1.0f / (float)(sectors-1);

		for(int r_idx = 0; r_idx < rings - 1; r_idx++){
			for (int s_idx = 0; s_idx < sectors - 1; s_idx++){ Vertex quad[4]; 
				int r_offsets[4] = {0, 1, 1, 0}; int s_offsets[4] = {0, 0, 1, 1};
				for(int i = 0; i < 4; i++){ int curr_r = r_idx + r_offsets[i]; int curr_s = s_idx + s_offsets[i];
					float phi = 2.0f * (float)M_PI * (float)curr_s * S_step;   // от 0 до 2pi
					float theta = (float)M_PI * (float)curr_r * R_step;       // от 0 до pi
					float vx = x + r * sinf(theta) * cosf(phi);
					float vy = y + r * cosf(theta); 
					float vz = z + r * sinf(theta) * sinf(phi);
					quad[i] = Vertex(vx, vy, vz, color);
				#ifdef NCPP_GL_VERTEX_EXT
					quad[i].setUV((float)curr_s*S_step, (float)curr_r*R_step); quad[i].setVN((vx-x)/r, (vy-y)/r, (vz-z)/r);
				#endif
				}
				if(fill){ v.push(quad[0]).push(quad[1]).push(quad[2]); v.push(quad[0]).push(quad[2]).push(quad[3]); } 
				else{ v.push(quad[0]).push(quad[1]); v.push(quad[1]).push(quad[2]); v.push(quad[2]).push(quad[3]); }
			}
		}
	}
	
	void genCylinder(Array<Vertex>& v, float d=1, float height=1, unsigned int color=0xFFFFFFFF,
		float x=0, float y=0, float z=0, char fill=1, bool center=true, int segments=32)
	{	float r = d / 2.0f; float y1 = center?y-height/2.0f:y; float y2 = y1 + height; float step = 2.0f*(float)M_PI / (float)segments;

		for(int i=0; i<segments; i++){ float a1 = i * step; float a2 = (i + 1) * step;
			float cos1 = cosf(a1), sin1 = sinf(a1); float cos2 = cosf(a2), sin2 = sinf(a2);
			// Координаты 4-х точек текущего сегмента стенки
			Vertex b1(x+cos1*r, y1, z+sin1*r, color); // Низ 1
			Vertex b2(x+cos2*r, y1, z+sin2*r, color); // Низ 2
			Vertex t1(x+cos1*r, y2, z+sin1*r, color); // Верх 1
			Vertex t2(x+cos2*r, y2, z+sin2*r, color); // Верх 2
			#ifdef NCPP_GL_VERTEX_EXT
			float u1 = (float)i/segments; float u2 = (float)(i+1)/segments;
			b1.setUV(u1, 1.0f); b1.setVN(cos1, 0.0f, sin1); b2.setUV(u2, 1.0f); b2.setVN(cos2, 0.0f, sin2);
			t1.setUV(u1, 0.0f); t1.setVN(cos1, 0.0f, sin1); t2.setUV(u2, 0.0f); t2.setVN(cos2, 0.0f, sin2);
			#endif

			if(fill==1){ v.push(b1).push(t1).push(t2); v.push(b1).push(t2).push(b2); }
			else if(!fill){ v.push(b1).push(t1); v.push(b1).push(b2); v.push(t1).push(t2); } }
		// Рисование двух кругов (две "крышки")
		Array<Vertex> arcV; genArc(arcV, d, color, 0, 0, fill==1?true:false, segments);
		Vertex::rotateX(arcV, 90); Vertex::translate(arcV, x, y2, z); v.concat(arcV); 
		
		arcV.clear(); genArc(arcV, d, color, 0, 0, fill==1?true:false, segments);
		Vertex::rotateX(arcV, 90); Vertex::translate(arcV, x, y1, z); v.concat(arcV);
	}
	
	void genCone(Array<Vertex>& v, float d=1, float height=1, unsigned int color=0xFFFFFFFF,
		float x=0, float y=0, float z=0, bool fill=true, int segments=32)
	{
		float r = d/2.0f; float y1 = y; float y2 = y+height; float step = 2.0f * (float)M_PI / (float)segments; Vertex top(x, y2, z, color);
		#ifdef NCPP_GL_VERTEX_EXT
		top.setUV(0.5f, 0.0f); top.setVN(0.0f, 1.0f, 0.0f);
		#endif
		
		for(int i = 0; i < segments; i++){ float a1 = i * step; float a2 = (i + 1) * step;
			float cos1 = cosf(a1), sin1 = sinf(a1); float cos2 = cosf(a2), sin2 = sinf(a2);
			Vertex b1(x + cos1 * r, y1, z + sin1 * r, color);
			Vertex b2(x + cos2 * r, y1, z + sin2 * r, color);
			#ifdef NCPP_GL_VERTEX_EXT
			float u1 = (float)i / segments; float u2 = (float)(i + 1) / segments;
			// Нормали боковой поверхности конуса немного приподняты по Y с учетом уклона
			b1.setUV(u1, 1.0f); b1.setVN(cos1, r/height, sin1);
			b2.setUV(u2, 1.0f); b2.setVN(cos2, r/height, sin2);
			#endif
			if(fill){ v.push(b1).push(top).push(b2); }else{ v.push(b1).push(top); v.push(b1).push(b2); } }
		// Рисование основания ("крышка")
		Array<Vertex> arcV; genArc(arcV, d, color, 0, 0, fill, segments); 
		Vertex::rotateX(arcV, 90); Vertex::translate(arcV, x, y1, z); v.concat(arcV);
	}
	// ---  ---
} }
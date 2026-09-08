#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PIf
#define M_PIf 3.1415926535f
//#define M_RAD2DEG 180.0f/M_PIf;
//#define M_DEG2RAD M_PIf/180.0f;
#endif

namespace ncpp { namespace GL {
#ifdef NCPP_GL_FLOAT_COLORS // Содержит RGBA [R,G,B,A]
    struct ColorT { float c[4]; }; const GLenum ColorGL_T = GL_FLOAT; const GLboolean Color_Norm = GL_FALSE; //const int ColorT_Elems = 4;
#else
	struct ColorT { unsigned char c[4]; }; const GLenum ColorGL_T = GL_UNSIGNED_BYTE; const GLboolean ColorGL_Norm = GL_TRUE;
#endif
	
#include "../media/colors.h"

// ========= Vector/Vertex =========
struct Vector3; struct Vector4; struct Vertex; struct Matrix4;

struct Vector2 { float x, y; 
	Vector2(float x1=0.0f, float y1=0.0f) : x(x1), y(y1){}
	Vector2(const Vector3& v);
	Vector2(const Vertex& v);
	
	Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator*(float s) const { return Vector2(x*s, y*s); }
	
    float length() const { return sqrtf(x*x + y*y); }
    Vector2& norm(){ float len = length(); if(len>0){ float invLn=1.0f/len; x*=invLn; y*=invLn; } return *this; }
    float dot(const Vector2& v) const { return x * v.x + y * v.y; } // Скалярное произведение
	
    float distance(const Vector2& v) const { float dx = v.x-x, dy = v.y-y; return sqrtf(dx * dx + dy * dy); } //pifagor2
    float angle() const { return atan2f(y, x) * (180.0f/M_PIf); }
	Vector2& setAngle(float angle){ float rad = angle*(M_PIf/180.0f); x=cosf(rad); y=sinf(rad); return *this; }
	static float angleTo(const Vector2& v1, const Vector2& v2){ float dx = v2.x-v1.x, dy = v2.y-v1.y; return atan2f(dy, dx) * (180.0f/M_PIf); }
	float angleTo(const Vector2& target) const { return angleTo(*this, target); }
};

struct Vector3 { float x, y, z;
    Vector3(float x1=0.0f, float y1=0.0f, float z1=0.0f) : x(x1), y(y1), z(z1){}
	Vector3(const Vector4& v);
	Vector3(const Vertex& v);

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
	
	float length() const { return sqrtf(x*x + y*y + z*z); }
	//Vector3& norm(){ float len = length(); if(len>0){ x/=len; y/=len; z/=len; } return *this; } // Camera::moveForward SegFault?
	Vector3& norm(){ float len = length(); if(len>0){ float invLn=1.0f/len; x*=invLn; y*=invLn; z*=invLn; } return *this; }
	float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; } // Скалярное произведение (Dot Product)
	static Vector3 cross(const Vector3& v1, const Vector3& v2){ // Векторное произведение (Cross Product)
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); }
	Vector3 cross(const Vector3& v) const { return cross(*this, v); }
	
	Vector3 operator*(const Matrix4& M) const;
	Vector3& operator*=(const Matrix4& M);
};
	
struct Vector4 { float x, y, z, w;
    Vector4(float x1=0.0f, float y1=0.0f, float z1=0.0f, float w1=1.0f) : x(x1), y(y1), z(z1), w(w1){}
	Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f){}
	Vector4(const Vertex& v);
	
	// Арифметические операторы
    Vector4 operator+(const Vector4& v) const { return Vector4(x+v.x, y+v.y, z+v.z, w+v.w); }
    Vector4 operator-(const Vector4& v) const { return Vector4(x-v.x, y-v.y, z-v.z, w-v.w); }
    Vector4 operator*(float s) const { return Vector4(x*s, y*s, z*s, w*s); }

    float length() const { return sqrtf(x*x + y*y + z*z + w*w); }
	Vector4& norm(){ float len = length(); if(len>0){ float invLn=1.0f/len; x*=invLn; y*=invLn; z*=invLn; w*=invLn; } return *this; }
    float dot(const Vector4& v) const { return x*v.x + y*v.y + z*v.z + w*v.w; } //Скалярное произведение (Dot Product)
	
	Vector4 operator*(const Matrix4& M) const;
	Vector4& operator*=(const Matrix4& M);
};

struct Vertex { float x, y, z; ColorT _color; // Содержит RGBA
#ifdef NCPP_GL_VERTEX_EXT
	unsigned short u, v; // (vt) UV-координаты текстур (от 0.0 до 1.0)
	char nx, ny, nz; // (vn) Нормали (от -1.0 до 1.0)
	Vertex& setUV(float u1, float v1){ u = (unsigned short)(u1*65535.0f); v = (unsigned short)(v1*65535.0f); return *this; }
	Vertex& setVN(float nx1, float ny1, float nz1){ nx = (char)(nx1*127.0f); ny = (char)(ny1*127.0f); nz = (char)(nz1*127.0f); return *this; }
#endif
	Vertex() : x(0), y(0), z(0){ memset(&_color, 0xFF, sizeof(ColorT)); }
	Vertex(float x, float y, float z=0, unsigned int hex=0xFFFFFFFF) : x(x), y(y), z(z){ setColor(hex); }
	Vertex(unsigned int hex) : x(0), y(0), z(0){ setColor(hex); }
#ifdef NCPP_GL_FLOAT_COLORS
	Vertex& setColor(unsigned int hex){ HexToRGBAf((float*)&_color, hex); return *this; }
	Vertex& setColor(float r, float g, float b, float a=1.0f){ _color.c[0]=r; _color.c[1]=g; _color.c[2]=b; _color.c[3]=a; return *this; }
	unsigned int getColor(){ return RGBAfToHex((float*)&_color); }
#else
	Vertex& setColor(unsigned int hex){ HexToRGBA((unsigned char*)&_color, hex); return *this; }
	Vertex& setColor(float r, float g, float b, float a=1.0f){ float src[4]={r,g,b,a}; RGBAfToUChar((unsigned char*)&_color, src); return *this; }
	unsigned int getColor(){ return RGBAToHex((unsigned char*)&_color); }
	Vertex& randColor(){ setColor(0xFF000000+randInt(0, 0xFFFFFF)); return *this; }
	
#endif
	Vertex(const Vector2& v) : x(v.x), y(v.y), z(0){ setColor(0xFFFFFF); }
	Vertex(const Vector3& v) : x(v.x), y(v.y), z(v.z){ setColor(0xFFFFFF); }
	Vertex(const Vector4& v) : x(v.x), y(v.y), z(v.z){ setColor(0xFFFFFF); }
	
	//static void setColors(Vertex* begin, Vertex* end, unsigned int hex){ for(Vertex* it=begin; it!=end; ++it) (*it).setColor(hex); }
	static void setColors(Vertex* v, size_t len, unsigned int hex){ for(size_t i=0;i<len;i++) v[i].setColor(hex); }
	static void setColors(Array<Vertex>& v, unsigned int hex){ for(size_t i=0;i<v.size();i++) v[i].setColor(hex); }
	
	static void randColors(Vertex* v, size_t len){ for(size_t i=0;i<len;i++) v[i].randColor(); }
	static void randColors(Array<Vertex>& v){ for(size_t i=0;i<v.size();i++) v[i].randColor(); }
	
	//CPU Vertex Tranformations (Translate, Rotate, Scale)
	static void translate(Vertex* v, size_t len, float x, float y, float z=0.0f){ for(size_t i=0;i<len;i++){ v[i].x+=x; v[i].y+=y; v[i].z+=z; } }
	static void translate(Array<Vertex>& v, float x, float y, float z=0.0f){ translate(v.data(), v.size(), x, y, z); }
	
	static void rotateX(Array<Vertex>& v, float aX){ float rad = aX * (float)M_PI / 180.0f; float c = cosf(rad);  float s = sinf(rad);
		for(size_t i=0; i<v.size(); i++){ float oldY = v[i].y; float oldZ = v[i].z; v[i].y = oldY*c-oldZ*s; v[i].z = oldY*s+oldZ*c; } }
	static void rotateY(Array<Vertex>& v, float aY){ float rad = aY * M_PI / 180.0f; float c = cosf(rad); float s = sinf(rad);
		for(size_t i=0; i<v.size(); i++){ float oldX = v[i].x; float oldZ = v[i].z; v[i].x = oldX*c-oldZ*s; v[i].z = oldX*s+oldZ*c; } }
	static void rotateZ(Array<Vertex>& v, float aZ){ float rad = aZ * (float)M_PI / 180.0f; float c = cosf(rad); float s = sinf(rad);
		for(size_t i=0; i<v.size(); i++){ float oldX = v[i].x; float oldY = v[i].y; v[i].x = oldX*c - oldY*s; v[i].y = oldX*s + oldY*c; } }
	
	static void rotate(Vertex* v, size_t len, float aX, float aY, float aZ){ //углы Эйлера
		aX = aX*(M_PIf/180.0f); aY = aY*(M_PIf/180.0f); aZ = aZ*(M_PIf/180.0f);
		float cx = cosf(aX), sx = sinf(aX); float cy = cosf(aY), sy = sinf(aY); float cz = cosf(aZ), sz = sinf(aZ);

        for(size_t i = 0; i < len; i++){ float x = v[i].x; float y = v[i].y; float z = v[i].z;
            float y1 = y * cx - z * sx; float z1 = y * sx + z * cx; float x1 = x; // 1. Вращение вокруг оси X
            float x2 = x1 * cy + z1 * sy; float y2 = y1; float z2 = -x1 * sy + z1 * cy; // 2. Вращение вокруг оси Y
            v[i].x = x2 * cz - y2 * sz; v[i].y = x2 * sz + y2 * cz; v[i].z = z2; } // 3. Вращение вокруг оси Z
    }
	static void rotate(Array<Vertex>& v, float aX, float aY, float aZ){ rotate(v.data(), v.size(), aX, aY, aZ); }
	
	static void scale(Vertex* v, size_t len, float sX, float sY, float sZ){ for(size_t i=0;i<len;i++){ v[i].x*=sX; v[i].y*=sY; v[i].z*=sZ; } }
	static void scale(Array<Vertex>& v, float sX, float sY, float sZ){ scale(v.data(), v.size(), sX, sY, sZ); }
	
	Vertex& swapAxis(unsigned char idx1=1, unsigned char idx2=2){ float* ptrs[3] = {&x,&y,&z};
		if(idx1>=0 && idx1<3 && idx2>=0 && idx2<3){ swap(*ptrs[idx1], *ptrs[idx2]); } return *this; }
	static void swapAxis(Vertex* v, size_t len, char idx1=1, char idx2=2){ for(size_t i=0;i<len;i++) v[i].swapAxis(idx1, idx2); }
	static void swapAxis(Array<Vertex>& v, char idx1=1, char idx2=2){ for(size_t i=0;i<v.size();i++) v[i].swapAxis(idx1, idx2); }
	
	Vertex operator*(const Matrix4& M) const;
	Vertex& operator*=(const Matrix4& M);
	
	//Vertex& _2DTo3D(){ float t=y; y=z; z=t; return *this; }
};

Vector2::Vector2(const Vertex& v) : x(v.x), y(v.y){}
Vector2::Vector2(const Vector3& v) : x(v.x), y(v.y){};
Vector3::Vector3(const Vertex& v) : x(v.x), y(v.y), z(v.z){}
Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z){};
Vector4::Vector4(const Vertex& v) : x(v.x), y(v.y), z(v.z), w(1.0f){}

// ========= Matrix =========
//Model = Translation*Rotation*Scale
//MVP = Projection*View*Model; // Итоговая матрица MVP (Model-View-Projection) (Умножаем справа налево)
//Result = Projection*View*Model*Vertex //Обработанный пиксель
//VP (View-Projection) — для окружения (небо?)

struct Matrix3 { float m[9]; }; // Матрица 3x3
struct Matrix4 { float m[16]; // Матрица 4x4
	Matrix4(){ identity(); }
	Matrix4 operator*(const Matrix4& rhs) const;
	Matrix4& operator*=(const Matrix4& rhs){ *this=(*this)*rhs; return *this; }
	Matrix4& transpose(){ //RowMajor <-> ColumnMajor // Мы меняем местами только 6 пар элементов. Диагональные элементы (0, 5, 10, 15) остаются на месте.
        swap(m[1], m[4]); swap(m[2], m[8]); swap(m[3], m[12]); swap(m[6], m[9]); swap(m[7], m[13]); swap(m[11], m[14]); return *this; }
	// --- Projections ---
	Matrix4& identity(){ memset(this, 0, sizeof(*this)); m[0]=m[5]=m[10]=m[15]=1.0f; return *this; }
	Matrix4& ortho(float left, float right, float bottom, float top, float zNear=-1, float zFar=1);
	Matrix4& perspective(float fovDeg=60.0f, float aspect=1.7778, float zNear=0.1f, float zFar=100.0f);
	Matrix4& frustum(float left, float right, float bottom, float top, float zNear, float zFar);
	// --- Actions ---
	Matrix4& translate(float x, float y, float z=0); // Перемещение
	Matrix4& rotate(float aX, float aY, float aZ=0); // Вращение (углы Эйлера)
	Matrix4& rotateX(float aX){ float rad = aX * M_PIf/180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[5] = c;  r.m[6] = s; r.m[9] = -s; r.m[10] = c; return *this *= r; }
	Matrix4& rotateY(float aY){ float rad = aY * M_PIf/180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[0] = c;  r.m[2] = -s; r.m[8] = s;  r.m[10] = c; return *this *= r; }
	Matrix4& rotateZ(float aZ){ float rad = aZ * M_PIf/180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[0] = c;  r.m[1] = s; r.m[4] = -s; r.m[5] = c; return *this *= r; }
		
	Matrix4& scale(float x, float y, float z); // Масштабирование
	Matrix4& orthonormalize(); //Нормализация матрицы при накоплении ошибок.
	Matrix4 inverse() const; //Инверсия матрицы
	// ---  ---
	Matrix4& setMatrix2DPreset(char rotate=1, int width=1, int height=1){ //getSize(width, height); //setOrtho2DPreset?
		switch(rotate){
			case 0: ortho(0.0, width, 0.0, height); break; //слева-снизу (default for OpenGL)
			case 2: ortho(-width, 0.0, 0.0, -height); break; //справа-сверху
			case 3: ortho(-width, 0.0, -height, 0.0); break; //справа-снизу
			case 1: default: ortho(0.0, width, height, 0.0); break; //слева-сверху (default for Vulkan, 2D, GDI, etc.)
		} return *this; }
		
	Matrix4& remapAxes(int axisX=0, int axisY=1, int axisZ=2, float signX=1.0f, float signY=1.0f, float signZ=1.0f){
		Matrix4 mPermute; mPermute.m[0] = mPermute.m[5] = mPermute.m[10] = 0.0f; // Очищаем диагональ, так как будем расставлять оси вручную
		//float* dest[3] = { &mPermute.m[0], &mPermute.m[5], &mPermute.m[10] }; //Задаем соответствие осей (0->X, 1->Y, 2->Z) и их знаки
		// Устанавливаем матрицу перестановки и масштаба/инверсии
		if (axisX >= 0 && axisX < 3) mPermute.m[axisX * 4 + 0] = signX;
		if (axisY >= 0 && axisY < 3) mPermute.m[axisY * 4 + 1] = signY;
		if (axisZ >= 0 && axisZ < 3) mPermute.m[axisZ * 4 + 2] = signZ; return *this*=mPermute; }
		
	Matrix4& setPerspectivePreset(char layout=0){
		switch(layout){
			case 1: scale(1.0f, -1.0f, 1.0f); break; // Инверсия Y
			case 2: scale(-1.0f, -1.0f, 1.0f); break; // Инверсия X и Y
			case 3: remapAxes(0, 2, 1, 1.0f, 1.0f, 1.0f); break; // Z-up система: Меняем местами Y и Z (Blender/архитектура: X=право, Y=глубина, Z=верх)
			case 4: scale(-1.0f, -1.0f, -1.0f); break; // Инверсия всех осей?
			default: case 0: break; // Стандартный OpenGL (Y смотрит вверх, Z вглубь)
		} return *this; }
	
	static void applyMatrixVec3(Vector3* v, const Matrix4& M){ float oldX=v->x, oldY=v->y, oldZ=v->z;
		v->x = M.m[0]*oldX + M.m[4]*oldY + M.m[8]*oldZ  + M.m[12];
		v->y = M.m[1]*oldX + M.m[5]*oldY + M.m[9]*oldZ  + M.m[13];
		v->z = M.m[2]*oldX + M.m[6]*oldY + M.m[10]*oldZ + M.m[14]; }
	static void applyMatrixVec4(Vector4* v, const Matrix4& M){ float oldX=v->x, oldY=v->y, oldZ=v->z, oldW=v->w;
		v->x = M.m[0]*oldX + M.m[4]*oldY + M.m[8]*oldZ + M.m[12]*oldW;
		v->y = M.m[1]*oldX + M.m[5]*oldY + M.m[9]*oldZ + M.m[13]*oldW;
		v->z = M.m[2]*oldX + M.m[6]*oldY + M.m[10]*oldZ + M.m[14]*oldW;
		v->w = M.m[3]*oldX + M.m[7]*oldY + M.m[11]*oldZ + M.m[15]*oldW; }
	//static void applyMatrix(Vertex* begin, Vertex* end, const Matrix4& M){ for(Vertex* it=begin; it!=end; ++it){ (*it)*=M; } }
	static void applyMatrix(Vertex* v, size_t len, const Matrix4& M){ for(size_t i=0;i<len;i++) v[i]*=M; }
	static void applyMatrix(Array<Vertex>& v, const Matrix4& M){ for(size_t i=0;i<v.size();i++) v[i]*=M; }
};
// ---------  ---------
Vector3  Vector3::operator*(const Matrix4& M) const { return Vector3(*this)*=M; }
Vector3& Vector3::operator*=(const Matrix4& M){ Matrix4::applyMatrixVec3(this, M); return *this; }

Vertex  Vertex::operator*(const Matrix4& M) const { return Vertex(*this)*=M; }
//Vertex& Vertex::operator*=(const Matrix4& M){ Vector3 v(*this); v*=M; x=v.x; y=v.y; z=v.z; return *this; }
Vertex& Vertex::operator*=(const Matrix4& M){ Matrix4::applyMatrixVec3((Vector3*)this, M); return *this; }

Vector4  Vector4::operator*(const Matrix4& M) const { return Vector4(*this)*=M; }
Vector4& Vector4::operator*=(const Matrix4& M){ Matrix4::applyMatrixVec4(this, M); return *this; }

Matrix4& Matrix4::ortho(float left, float right, float bottom, float top, float zNear, float zFar){
	m[0] = 2.0f/(right-left); m[5] = 2.0f/(top-bottom); m[10] = -2.0f/(zFar-zNear);
	m[12] = -(right+left)/(right-left); m[13] = -(top+bottom)/(top-bottom); m[14] = -(zFar+zNear)/(zFar-zNear); return *this; }

Matrix4& Matrix4::perspective(float fovDeg, float aspect, float zNear, float zFar){ float f = 1.0f / tanf(fovDeg * M_PIf/360.0f); 
	m[0] = f/aspect; m[5] = f; m[10] = (zFar+zNear)/(zNear-zFar); m[11] = -1.0f; m[14] = (2.0f*zFar*zNear)/(zNear-zFar); return *this; }
	
Matrix4& Matrix4::frustum(float left, float right, float bottom, float top, float zNear, float zFar){
    m[0] = (2.0f*zNear)/(right-left); m[5] = (2.0f*zNear)/(top-bottom);
    m[8] = (right+left)/(right-left); m[9] = (top+bottom)/(top-bottom);
    m[10] = -(zFar+zNear)/(zFar-zNear); m[11] = -1.0f; m[14] = -(2.0f*zFar*zNear)/(zFar-zNear); return *this; }

Matrix4 Matrix4::operator*(const Matrix4& rhs) const { Matrix4 res; // Column-Major: M[col][row]
    for(int col = 0; col < 4; ++col){
        for (int row = 0; row < 4; ++row){ float sum = 0.0f;
            for(int k = 0; k < 4; ++k){ sum += this->m[k * 4 + row] * rhs.m[col * 4 + k]; }
            res.m[col * 4 + row] = sum; } } return res; }
			
Matrix4& Matrix4::translate(float x, float y, float z){ Matrix4 trans; trans.m[12] = x; trans.m[13] = y; trans.m[14] = z; return *this *= trans; }

Matrix4& Matrix4::scale(float sX, float sY, float sZ){
	m[0] *= sX; m[1] *= sX; m[2] *= sX; m[3] *= sX;
	m[4] *= sY; m[5] *= sY; m[6] *= sY; m[7] *= sY;
	m[8] *= sZ; m[9] *= sZ; m[10] *= sZ; m[11] *= sZ; return *this; }
	
Matrix4& Matrix4::rotate(float aX, float aY, float aZ){ float radX = aX * M_PIf/180.0f; float radY = aY * M_PIf/180.0f; float radZ = aZ * M_PIf/180.0f;
    float cx = cos(radX), sx = sin(radX); float cy = cos(radY), sy = sin(radY); float cz = cos(radZ), sz = sin(radZ);
    Matrix4 r; // Матрица вращения, полученная перемножением Rz * Ry * Rx
    r.m[0] = cy * cz; r.m[1] = cy * sz; r.m[2] = -sy;
    r.m[4] = sx * sy * cz - cx * sz; r.m[5] = sx * sy * sz + cx * cz; r.m[6] = sx * cy;
    r.m[8] = cx * sy * cz + sx * sz; r.m[9] = cx * sy * sz - sx * cz; r.m[10] = cx * cy; return *this *= r; }
	
Matrix4& Matrix4::orthonormalize(){
    Vector3 f = Vector3(m[8], m[9], m[10]).norm(); // 1. Берем вектор Forward (из 3-го столбца) и нормализуем его
    // 2. Считаем новый Right через Cross с мировым Up (или старым Up). Это гарантирует, что Right перпендикулярен Forward
    Vector3 r = Vector3(0, 1, 0).cross(f).norm();
    Vector3 u = f.cross(r).norm(); // 3. Считаем финальный Up, который теперь перпендикулярен и Forward, и Right
    // 4. Записываем результат обратно в матрицу
    m[0]=r.x; m[1]=r.y; m[2]=r.z; m[4]=u.x; m[5]=u.y; m[6]=u.z; m[8]=f.x; m[9]=f.y; m[10]=f.z; return *this; }
	
Matrix4 Matrix4::inverse() const { Matrix4 res;
    res.m[0]=m[5]*m[10]*m[15]-m[5]*m[11]*m[14]-m[9]*m[6]*m[15]+m[9]*m[7]*m[14]+m[13]*m[6]*m[11]-m[13]*m[7]*m[10];
    res.m[4]=-m[4]*m[10]*m[15]+m[4]*m[11]*m[14]+m[8]*m[6]*m[15]-m[8]*m[7]*m[14]-m[12]*m[6]*m[11]+m[12]*m[7]*m[10];
    res.m[8]=m[4]*m[9]*m[15]-m[4]*m[11]*m[13]-m[8]*m[5]*m[15]+m[8]*m[7]*m[13]+m[12]*m[5]*m[11]-m[12]*m[7]*m[9];
    res.m[12]=-m[4]*m[9]*m[14]+m[4]*m[10]*m[13]+m[8]*m[5]*m[14]-m[8]*m[6]*m[13]-m[12]*m[5]*m[10]+m[12]*m[6]*m[9];
    res.m[1]=-m[1]*m[10]*m[15]+m[1]*m[11]*m[14]+m[9]*m[2]*m[15]-m[9]*m[3]*m[14]-m[13]*m[2]*m[11]+m[13]*m[3]*m[10];
    res.m[5]=m[0]*m[10]*m[15]-m[0]*m[11]*m[14]-m[8]*m[2]*m[15]+m[8]*m[3]*m[14]+m[12]*m[2]*m[11]-m[12]*m[3]*m[10];
    res.m[9]=-m[0]*m[9]*m[15]+m[0]*m[11]*m[13]+m[8]*m[1]*m[15]-m[8]*m[3]*m[13]-m[12]*m[1]*m[11]+m[12]*m[3]*m[9];
    res.m[13]=m[0]*m[9]*m[14]-m[0]*m[10]*m[13]-m[8]*m[1]*m[14]+m[8]*m[2]*m[13]+m[12]*m[1]*m[10]-m[12]*m[2]*m[9];
	res.m[2]=m[1]*m[6]*m[15]-m[1]*m[7]*m[14]-m[5]*m[2]*m[15]+m[5]*m[3]*m[14]+m[13]*m[2]*m[7]-m[13]*m[3]*m[6];
	res.m[6]=-m[0]*m[6]*m[15]+m[0]*m[7]*m[14]+m[4]*m[2]*m[15]-m[4]*m[3]*m[14]-m[12]*m[2]*m[7]+m[12]*m[3]*m[6];
	res.m[10]=m[0]*m[5]*m[15]-m[0]*m[7]*m[13]-m[4]*m[1]*m[15]+m[4]*m[3]*m[13]+m[12]*m[1]*m[7]-m[12]*m[3]*m[5];
	res.m[14]=-m[0]*m[5]*m[14]+m[0]*m[6]*m[13]+m[4]*m[1]*m[14]-m[4]*m[2]*m[13]-m[12]*m[1]*m[6]+m[12]*m[2]*m[5];
	res.m[3]=-m[1]*m[6]*m[11]+m[1]*m[7]*m[10]+m[5]*m[2]*m[11]-m[5]*m[3]*m[10]-m[9]*m[2]*m[7]+m[9]*m[3]*m[6];
	res.m[7]=m[0]*m[6]*m[11]-m[0]*m[7]*m[10]-m[4]*m[2]*m[11]+m[4]*m[3]*m[10]+m[8]*m[2]*m[7]-m[8]*m[3]*m[6];
	res.m[11]=-m[0]*m[5]*m[11]+m[0]*m[7]*m[9]+m[4]*m[1]*m[11]-m[4]*m[3]*m[9]-m[8]*m[1]*m[7]+m[8]*m[3]*m[5];
	res.m[15]=m[0]*m[5]*m[10]-m[0]*m[6]*m[9]-m[4]*m[1]*m[10]+m[4]*m[2]*m[9]+m[8]*m[1]*m[6]-m[8]*m[2]*m[5];

    float det = m[0] * res.m[0] + m[1] * res.m[4] + m[2] * res.m[8] + m[3] * res.m[12];
    if(det == 0.0f) return *this; float invDet = 1.0f / det; for(int i=0; i<16; i++){ res.m[i]*=invDet; } return res; }
	
struct Quaternion { float x, y, z, w; //Альтернатива углам Эйлера: Две пары комплексных чисел: q1 = w+xi; q2 = z+yi;
    Quaternion() : x(0), y(0), z(0), w(0){}
	Quaternion(float aX, float aY, float aZ){ setAngle(aX, aY, aZ); }
	
	float length() const { return sqrtf(x*x + y*y + z*z + w*w); }
    Quaternion& norm(){ float len = length(); if(len>0){ x/=len; y/=len; z/=len; w/=len; } return *this; }
	
	Quaternion& rotate(float aX, float aY, float aZ){ (*this)*=Quaternion::fromAngle(aX, aY, aZ); return *this; }
    Quaternion& rotate(const Quaternion& qRot){ *this = (*this)*qRot; return *this; }
		
	void getAngle(float& aX, float& aY, float& aZ) const { float rad = 180.0f/M_PIf; 
		float siny_cosp=2.0f*(w*y+x*z); float cosy_cosp=1.0f-2.0f*(y*y+x*x); aY = atan2f(siny_cosp, cosy_cosp)*rad; // Yaw (Y)
		float sinp=2.0f*(w*x-y*z); if(fabsf(sinp) >= 1.0f) aX = copysignf(M_PIf/2.0f, sinp)*rad; else aX = asinf(sinp)*rad; // Pitch (X)
		float sinr_cosp=2.0f*(w*z+x*y); float cosy_cosp_r=1.0f-2.0f*(x*x+z*z); aZ = atan2f(sinr_cosp, cosy_cosp_r)*rad; // Roll (Z) 
	}
	Vector3 getAngle(){ float aX, aY, aZ; getAngle(aX, aY, aZ); return Vector3(aX, aY, aZ); }
	
	Quaternion& setAngle(float aX, float aY, float aZ){ float rad = M_PIf/180.0f; // Установка углов Эйлера (Yaw, Pitch, Roll)
        float cy = cosf(aY*0.5f*rad); float sy = sinf(aY*0.5f*rad);
        float cp = cosf(aX*0.5f*rad); float sp = sinf(aX*0.5f*rad);
        float cr = cosf(aZ*0.5f*rad); float sr = sinf(aZ*0.5f*rad);
		w=cy*cp*cr+sy*sp*sr; x=cy*sp*cr+sy*cp*sr; y=sy*cp*cr-cy*sp*sr; z=cy*cp*sr-sy*sp*cr; return *this; }
		
	Quaternion operator*(const Quaternion& q) const { // Умножение кватернионов (композиция вращений)
		Quaternion res; res.x=w*q.x+x*q.w+y*q.z-z*q.y; res.y=w*q.y-x*q.z+y*q.w+z*q.x; 
		res.z=w*q.z+x*q.y-y*q.x+z*q.w; res.w=w*q.w-x*q.x-y*q.y-z*q.z; return res; }
	Quaternion& operator*=(const Quaternion& q){ *this=(*this)*q; return *this; }

    static Quaternion fromAngle(float aX, float aY, float aZ){ return Quaternion().setAngle(aX, aY, aZ); }
	//static Quaternion fromMatrix(const Matrix4& M); //сложный матан...

    Matrix4 toMatrix() const { Matrix4 m;
		float x2=x+x, y2=y+y, z2=z+z; float xx=x*x2, xy=x*y2, xz=x*z2;
		float yy=y*y2, yz=y*z2, zz=z*z2; float wx=w*x2, wy=w*y2, wz=w*z2;
        m.m[0]=1-(yy+zz); m.m[1]=xy+wz; m.m[2]=xz-wy;
        m.m[4]=xy-wz; m.m[5]=1-(xx+zz); m.m[6]=yz+wx;
        m.m[8]=xz+wy; m.m[9]=yz-wx; m.m[10]=1-(xx+yy); return m; }
};

struct Entity { double x, y, z; float aX, aY, aZ; float sX, sY, sZ; //Quaternion qRot;
	Entity(double x1=0, double y1=0, double z1=0) : x(x1), y(y1), z(z1), aX(0), aY(0), aZ(0), sX(1), sY(1), sZ(1){}
	Entity& setPos(double x1, double y1, double z1){ x=x1; y=y1; z=z1; return *this; }
	Entity& move(float x1, float y1){ x+=x1; y+=y1; return *this; }
	Entity& move(float x1, float y1, float z1){ x+=x1; y+=y1; z+=z1; return *this; }
	Entity& move(const Vector2& v){ x+=v.x; y+=v.y; return *this; }
	Entity& move(const Vector3& v){ x+=v.x; y+=v.y; z+=v.z; return *this; }
	void getAngle(float& aX1, float& aY1, float& aZ1){ aX1=aX; aY1=aY; aZ1=aZ; }
	Vector3 getAngle(){ float aX, aY, aZ; getAngle(aX, aY, aZ); return Vector3(aX, aY, aZ); }
	Entity& setAngle(float aX1, float aY1, float aZ1){ aX=aX1; aY=aY1; aZ=aZ1; return *this; }
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
	
	void lookAt(double x1=0, double y1=0, double z1=0){
		double dx = x1-x, dy = y1-y, dz = z1-z; double lenXZ = sqrt(dx*dx + dz*dz); 
		if(lenXZ==0.0&&dy==0.0) return; float rad = 180.0f/M_PIf; setAngle(-atan2(dy, lenXZ)*rad, atan2(dx, dz)*rad, 0.0f); }
	void lookAt(const Entity& e) { return lookAt(e.x, e.y, e.z); }
	void lookAt(const Vector3& v) { return lookAt(v.x, v.y, v.z); }
	void lookAt(const Vector2& v) { return lookAt(v.x, v.y); }
};

// ========= 2D Math funcs =========
	
double pifagor2(double x1, double y1, double x2, double y2){ double dx=x2-x1, dy=y2-y1; return ::sqrt(dx*dx+dy*dy); }
double pifagor2(const Vector2& v1, const Vector2& v2){ return pifagor2(v1.x, v1.y, v2.x, v2.y); }
double pifagor2(const Vertex& v1, const Vertex& v2){ return pifagor2(v1.x, v1.y, v2.x, v2.y); }

template <typename T> bool isPXInCircle(T px, T py, T cx, T cy, T r){ T dx=cx-px, dy=cy-py; return (dx*dx+dy*dy)<=r*r; }
bool isPXInCircle(const Vector2& px, const Vector2& cr, float r){ return isPXInCircle(px.x, px.y, cr.x, cr.y, r); }

bool isPXInRect(float px, float py, float rx, float ry, float w=1, float h=1, bool center=true)
{ 	float left, top; if(center){ left = rx-w/2.0; top = ry-h/2.0; }else{ left=rx; top=ry; }
    double right = left+w; double bottom = top+h; return (px>=left && px<=right && py>=top && py<=bottom); }

bool isPXInTriangle2(float px, float py, float ax, float ay, float bx, float by, float cx, float cy){
    float denominator = ((by-cy) * (ax-cx) + (cx-bx) * (ay-cy)); // Барицентрические координаты
    if(denominator == 0.0f) return false; // Вырожденный треугольник (все вершины на одной прямой)

    float w1 = ((by-cy) * (px-cx) + (cx-bx) * (py-cy)) / denominator;
    float w2 = ((cy-ay) * (px-cx) + (ax-cx) * (py-cy)) / denominator;
    float w3 = 1.0f-w1-w2; return (w1 >= 0.0f) && (w2 >= 0.0f) && (w3 >= 0.0f); // Проверяем, находится ли точка внутри треугольника (включая границы)
}
bool isPXInTriangle2(float px, float py, const Vector2& v1, const Vector2& v2, const Vector2& v3){ return isPXInTriangle2(px, py, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y); }

// ========= 3D Math funcs =========

double pifagor3(double x1, double y1, double z1, double x2, double y2, double z2){ double dx=x2-x1, dy=y2-y1, dz=z2-z1; return ::sqrt(dx*dx+dy*dy+dz*dz); }
double pifagor3(const Vector3& v1, const Vector3& v2){ return pifagor3(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z); }
double pifagor3(const Vertex& v1, const Vertex& v2){ return pifagor3(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z); }

template <typename T> bool isPXInSphere(T px, T py, T pz, T cx, T cy, T cz, T r){ T dx=cx-px, dy=cy-py, dz=cz-pz; return (dx*dx+dy*dy+dz*dz) <= r*r; }
bool isPXInSphere(const Vector3& px, const Vector3& sf, float r){ return isPXInSphere(px.x, px.y, px.z, sf.x, sf.y, sf.z, r); }
	
bool isPXInTriangle3(const Vector3& px, const Vertex& v1, const Vertex& v2, const Vertex& v3);

//Маршрутизация: Алгоритм Дейкстры

} }


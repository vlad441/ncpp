#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ncpp { namespace GL {
#ifdef NCPP_GL_FLOAT_COLORS
    struct ColorT { float rgba[4]; }; const GLenum ColorGL_T = GL_FLOAT; const GLboolean Color_Norm = GL_FALSE; //const int ColorT_Elems = 4;
#else
	struct ColorT { unsigned char rgba[4]; }; const GLenum ColorGL_T = GL_UNSIGNED_BYTE; const GLboolean ColorGL_Norm = GL_TRUE;
#endif
	
#include "../media/colors.h"

// ========= Matrix =========
//Model = Translation*Rotation*Scale
//MVP = Projection*View*Model; // Итоговая матрица MVP (Model-View-Projection) (Умножаем справа налево)
//Result = Projection*View*Model*Vertex //Обработанный пиксель
//VP (View-Projection) — для окружения (небо?)
struct Vector3; struct Vector4;

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
	// --- Actions ---
	Matrix4& translate(float x, float y, float z); // Перемещение
	Matrix4& rotate(float aX, float aY, float aZ); // Вращение
	Matrix4& rotateX(float aX){ float rad = aX * M_PI / 180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[5] = c;  r.m[6] = s; r.m[9] = -s; r.m[10] = c; return *this *= r; }
	Matrix4& rotateY(float aY){ float rad = aY * M_PI / 180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[0] = c;  r.m[2] = -s; r.m[8] = s;  r.m[10] = c; return *this *= r; }
	Matrix4& rotateZ(float aZ){ float rad = aZ * M_PI / 180.0f; float c = cos(rad), s = sin(rad);
		Matrix4 r; r.m[0] = c;  r.m[1] = s; r.m[4] = -s; r.m[5] = c; return *this *= r; }
		
	Matrix4& scale(float x, float y, float z); // Масштабирование
	Matrix4& lookAt(Vector3 eye, Vector3 center, Vector3 up); //Установка направления камеры
	Matrix4& orthonormalize(); //Нормализация матрицы при накоплении ошибок.
	// ---  ---
	Matrix4& setMatrix2DPreset(char rotate=0, int width=1, int height=1){ //getSize(width, height); //setOrtho2DPreset?
		switch(rotate){
			case 1: ortho(0.0, width, height, 0.0); break; //слева-сверху
			case 2: ortho(-width, 0.0, 0.0, -height); break; //справа-сверху
			case 3: ortho(-width, 0.0, -height, 0.0); break; //справа-снизу
			default: ortho(0.0, width, 0.0, height); break; //слева-снизу
		} return *this; }
};

Matrix4& Matrix4::ortho(float left, float right, float bottom, float top, float zNear, float zFar){
	m[0] = 2.0f / (right - left);
	m[5] = 2.0f / (top - bottom);
	m[10] = -2.0f / (zFar - zNear);
	m[12] = -(right + left) / (right - left);
	m[13] = -(top + bottom) / (top - bottom);
	m[14] = -(zFar + zNear) / (zFar - zNear); return *this; }

Matrix4& Matrix4::perspective(float fovDeg, float aspect, float zNear, float zFar){
	float f = 1.0f / tanf(fovDeg * (float)M_PI / 360.0f);
	m[0] = f / aspect;
	m[5] = f;
	m[10] = (zFar + zNear) / (zNear - zFar);
	m[11] = -1.0f;
	m[14] = (2.0f * zFar * zNear) / (zNear - zFar); return *this; }

Matrix4 Matrix4::operator*(const Matrix4& rhs) const { Matrix4 res; // Column-Major: M[col][row]
    for(int col = 0; col < 4; ++col){
        for (int row = 0; row < 4; ++row){ float sum = 0.0f;
            for(int k = 0; k < 4; ++k){ sum += this->m[k * 4 + row] * rhs.m[col * 4 + k]; }
            res.m[col * 4 + row] = sum; } } return res; }
			
Matrix4& Matrix4::translate(float x, float y, float z){ Matrix4 trans; // Создаем временную единичную матрицу
    trans.m[12] = x; trans.m[13] = y; trans.m[14] = z; return *this *= trans; }
	
Matrix4& Matrix4::scale(float sX, float sY, float sZ){
	m[0] *= sX; m[1] *= sX; m[2] *= sX; m[3] *= sX;
	m[4] *= sY; m[5] *= sY; m[6] *= sY; m[7] *= sY;
	m[8] *= sZ; m[9] *= sZ; m[10] *= sZ; m[11] *= sZ;
	return *this; }
	
Matrix4& Matrix4::rotate(float aX, float aY, float aZ){ float radX = aX * M_PI / 180.0f; float radY = aY * M_PI / 180.0f; float radZ = aZ * M_PI / 180.0f;
    float cx = cos(radX), sx = sin(radX); float cy = cos(radY), sy = sin(radY); float cz = cos(radZ), sz = sin(radZ);
    Matrix4 r; // Матрица вращения, полученная перемножением Rz * Ry * Rx
    r.m[0] = cy * cz; r.m[1] = cy * sz; r.m[2] = -sy;
    r.m[4] = sx * sy * cz - cx * sz; r.m[5] = sx * sy * sz + cx * cz; r.m[6] = sx * cy;
    r.m[8] = cx * sy * cz + sx * sz; r.m[9] = cx * sy * sz - sx * cz; r.m[10] = cx * cy; return *this *= r; }
			
// ========= Vertex/Vector4 =========

struct Vertex { float x, y, z; ColorT _color; // Содержит RGBA
#ifdef NCPP_GL_UV_CORDS
	float u, v; // Добавлено: UV-координаты текстур (от 0.0 до 1.0)
#endif
	Vertex() : x(0), y(0), z(0){ memset(&_color, 0xFF, sizeof(ColorT)); }
	Vertex(float x, float y, float z, unsigned int hex=0xFFFFFFFF) : x(x), y(y), z(z){ setColor(hex); }
	Vertex(unsigned int hex) : x(0), y(0), z(0){ setColor(hex); }
#ifdef NCPP_GL_FLOAT_COLORS
	Vertex& setColor(unsigned int hex){ HexToRGBAf((float*)&_color, hex); return *this; }
	unsigned int getColor(){ return RGBAfToHex((float*)&_color); }
#else
	Vertex& setColor(unsigned int hex){ HexToRGBA((unsigned char*)&_color, hex); return *this; }
	unsigned int getColor(){ return RGBAToHex((unsigned char*)&_color); }
#endif
	Vertex(const Vector3& v); Vertex(const Vector4& v);
	Vertex operator*(const Matrix4& M) const;
	Vertex& operator*=(const Matrix4& M);
	
	//Vertex& _2DTo3D(){ float t=y; y=z; z=t; return *this; }
	
	static void applyMatrix(Vertex* b, Vertex* e, const Matrix4& M){ for(Vertex* it=b; it!=e; ++it){ (*it)*=M; } }
	static void applyMatrix(Array<Vertex>& v, const Matrix4& M){ for(size_t i=0;i<v.size();i++){ v[i]*=M; } }
};

struct Vector3 { float x, y, z;
    Vector3(float x1=0.0f, float y1=0.0f, float z1=0.0f) : x(x1), y(y1), z(z1){}
	Vector3(const Vertex& v) : x(v.x), y(v.y), z(v.z){}
	Vector3(const Vector4& v);

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
	
	float length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 norm() const { float len = length(); if(len > 0){ float invLen = 1.0f / len; return Vector3(x * invLen, y * invLen, z * invLen); } return Vector3(0, 0, 0); }
	float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; } // Скалярное произведение (Dot Product)
    Vector3 cross(const Vector3& v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); } // 4. Векторное произведение (Cross Product)
	
	Vector3 operator*(const Matrix4& M) const { return Vector3(*this)*=M; }
	Vector3& operator*=(const Matrix4& M);
};
	
struct Vector4 { float x, y, z, w;
    Vector4(float x1=0.0f, float y1=0.0f, float z1=0.0f, float w1=1.0f) : x(x1), y(y1), z(z1), w(w1){}
	Vector4(const Vertex& v) : x(v.x), y(v.y), z(v.z), w(1.0f){}
	Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f){}
	static Vector4 multiply(const Matrix4& M, const Vector4& v);
	Vector4 operator*(const Matrix4& M) const { return multiply(M, *this); }
	Vector4& operator*=(const Matrix4& M);
};

Vertex::Vertex(const Vector3& v) : x(v.x), y(v.y), z(v.z){ setColor(0xFFFFFF); }
Vertex::Vertex(const Vector4& v) : x(v.x), y(v.y), z(v.z){ setColor(0xFFFFFF); }

Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z){};

Vector3& Vector3::operator*=(const Matrix4& M){ float oldX = x, oldY = y, oldZ = z;
    x = M.m[0]*oldX + M.m[4]*oldY + M.m[8]*oldZ  + M.m[12];
    y = M.m[1]*oldX + M.m[5]*oldY + M.m[9]*oldZ  + M.m[13];
    z = M.m[2]*oldX + M.m[6]*oldY + M.m[10]*oldZ + M.m[14];
    //float w = M.m[3]*oldX + M.m[7]*oldY + M.m[11]*oldZ + M.m[15];
    //if(w != 1.0f && w != 0.0f){ float invW = 1.0f / w; x *= invW; y *= invW; z *= invW; }
    return *this; }
	
Vertex Vertex::operator*(const Matrix4& M) const { return Vector3(*this)*=M; }
Vertex& Vertex::operator*=(const Matrix4& M){ Vector3 v(*this); v*=M; x=v.x; y=v.y; z=v.z; return *this; }
				
Vector4 Vector4::multiply(const Matrix4& M, const Vector4& v){ Vector4 vec4;
    vec4.x = M.m[0]*v.x + M.m[4]*v.y + M.m[8]*v.z + M.m[12]*v.w;
    vec4.y = M.m[1]*v.x + M.m[5]*v.y + M.m[9]*v.z + M.m[13]*v.w;
    vec4.z = M.m[2]*v.x + M.m[6]*v.y + M.m[10]*v.z + M.m[14]*v.w;
    vec4.w = M.m[3]*v.x + M.m[7]*v.y + M.m[11]*v.z + M.m[15]*v.w; return vec4; }
	
Vector4& Vector4::operator*=(const Matrix4& M){ float oldX = x, oldY = y, oldZ = z, oldW = w;
    x = M.m[0]*oldX + M.m[4]*oldY + M.m[8]*oldZ + M.m[12]*oldW;
    y = M.m[1]*oldX + M.m[5]*oldY + M.m[9]*oldZ + M.m[13]*oldW;
    z = M.m[2]*oldX + M.m[6]*oldY + M.m[10]*oldZ + M.m[14]*oldW;
    w = M.m[3]*oldX + M.m[7]*oldY + M.m[11]*oldZ + M.m[15]*oldW; return *this; }
	
// =========  =========

Matrix4& Matrix4::lookAt(Vector3 eye, Vector3 center, Vector3 up){ Vector3 f = (center-eye).norm(); Vector3 s = f.cross(up).norm(); Vector3 u = s.cross(f);
    Matrix4 result; result.m[0] = s.x;  result.m[4] = s.y;  result.m[8] = s.z;
    result.m[1] = u.x;  result.m[5] = u.y;  result.m[9] = u.z;
    result.m[2] =-f.x;  result.m[6] =-f.y;  result.m[10]=-f.z;
    result.m[12] = -s.dot(eye); result.m[13] = -u.dot(eye); result.m[14] =  f.dot(eye); return *this *= result; }
	
Matrix4& Matrix4::orthonormalize(){
    Vector3 f = Vector3(m[8], m[9], m[10]).norm(); // 1. Берем вектор Forward (из 3-го столбца) и нормализуем его
    // 2. Считаем новый Right через Cross с мировым Up (или старым Up). Это гарантирует, что Right перпендикулярен Forward
    Vector3 r = Vector3(0, 1, 0).cross(f).norm();
    Vector3 u = f.cross(r).norm(); // 3. Считаем финальный Up, который теперь перпендикулярен и Forward, и Right
    // 4. Записываем результат обратно в матрицу
    m[0]=r.x; m[1]=r.y; m[2]=r.z; m[4]=u.x; m[5]=u.y; m[6]=u.z; m[8]=f.x; m[9]=f.y; m[10]=f.z; return *this; }
	

	
struct Vector2D { double dist; float angle; };
Vector2D pifagor2D(double x1, double y1, double x2, double y2){ double dx = x2 - x1; double dy = y2 - y1; Vector2D result;
    result.dist = ::sqrt(dx * dx + dy * dy); result.angle = ::atan2(dy, dx) * 180.0/M_PI; return result; }
	
double pifagor2(double x1, double y1, double x2, double y2){ double dx = x2 - x1; double dy = y2 - y1; return ::sqrt(dx * dx + dy * dy); }
double pifagor2(const Vertex& v1, const Vertex& v2){ return pifagor2(v1.x, v1.y, v2.x, v2.y); }
double pifagor2(const Vector3& v1, const Vector3& v2){ return pifagor2(v1.x, v1.y, v2.x, v2.y); }

double pifagor3(double x1, double y1, double z1, double x2, double y2, double z2){ double dx = x2 - x1; 
	double dy = y2 - y1; double dz = z2 - z1; return ::sqrt(dx * dx + dy * dy + dz * dz); }
double pifagor3(const Vertex& v1, const Vertex& v2){ return pifagor3(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z); }
double pifagor3(const Vector3& v1, const Vector3& v2){ return pifagor3(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z); }

//Маршрутизация: Алгоритм Дейкстры

} }


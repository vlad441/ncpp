#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ncpp { namespace GL {
	
struct Matrix3 { float m[9]; }; // Матрица 3x3
struct Matrix4 { float m[16]; // Матрица 4x4 
	Matrix4 operator*(const Matrix4& rhs){ const Matrix4& lhs=*this;
		Matrix4 result; for(int i = 0; i < 4; ++i){ for(int j = 0; j < 4; ++j){ 
				result.m[i * 4 + j] = lhs.m[i * 4 + 0] * rhs.m[0 * 4 + j] + lhs.m[i * 4 + 1] * rhs.m[1 * 4 + j] +
					lhs.m[i * 4 + 2] * rhs.m[2 * 4 + j] + lhs.m[i * 4 + 3] * rhs.m[3 * 4 + j]; } } return result; }
	Matrix4& operator*=(const Matrix4& rhs){ *this=(*this)*rhs; return *this; } };

struct Vector2D { double dist; float angle; };
struct RGB { float r; float g; float b; };
//struct vPoint { float x; float y; };

RGB HexToRGBFloat(unsigned int hex){ RGB rgb; rgb.r = ((hex >> 16) & 0xFF) / 255.0f;
	rgb.g = ((hex >> 8) & 0xFF) / 255.0f; rgb.b = (hex & 0xFF) / 255.0f; return rgb; }
void HexToRGB(unsigned char rgb[3], unsigned int hex){ rgb[0] = (hex >> 16) & 0xFF; rgb[1] = (hex >> 8) & 0xFF; rgb[2] = hex & 0xFF; }

Vector2D pifagor2(double x1, double y1, double x2, double y2){ double dx = x2 - x1; double dy = y2 - y1; Vector2D result;
    result.dist = ::sqrt(dx * dx + dy * dy); result.angle = ::atan2(dy, dx) * 180.0/M_PI; return result; }

double pifagor3(double x1, double y1, double z1, double x2, double y2, double z2){ double dx = x2 - x1; 
	double dy = y2 - y1; double dz = z2 - z1; return ::sqrt(dx * dx + dy * dy + dz * dz); }
	
} }


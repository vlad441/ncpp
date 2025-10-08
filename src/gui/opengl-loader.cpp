#include <GL/gl.h>
#ifdef _WIN32
//Универсальный указатель GLGetProcAddress -> GLGetProcAddress/glXGetProcAddress
void* (APIENTRY *GLGetProcAddress)(const char* name) = (void* (APIENTRY *)(const char*))wglGetProcAddress;
#else
#include <GL/glx.h>
void* (*GLGetProcAddress)(const char* name) = (void* (*)(const char* name))glXGetProcAddress;
#endif

// Ручное определение типов и констант из OpenGL 2.0
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
//#define GL_FALSE 0
//#define GL_TRUE 1

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;

//-------------------------------------------------------------------------
// Загрузка функций для работы с шейдерами и программами
//-------------------------------------------------------------------------

typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC) (GLenum shaderType);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

//-------------------------------------------------------------------------
// Загрузка функций для работы с буферами (VBO)
//-------------------------------------------------------------------------

typedef void (APIENTRY *PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);

//-------------------------------------------------------------------------
// Загрузка функций для работы с атрибутами вершин
//-------------------------------------------------------------------------

typedef GLint (APIENTRY *PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

//-------------------------------------------------------------------------
// Загрузка функций для работы с Uniform-переменными
//-------------------------------------------------------------------------

typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

//-------------------------------------------------------------------------
// Объявление указателей
//-------------------------------------------------------------------------

PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;

PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;


namespace ncpp { namespace GL {
	
bool LoadOpenGL_2_0(){
    //-------------------------------------------------------------------------
    // Загрузка функций для шейдеров и программ
    //-------------------------------------------------------------------------
    glCreateShader = (PFNGLCREATESHADERPROC)GLGetProcAddress("glCreateShader"); if(!glCreateShader){ return false; }
    glShaderSource = (PFNGLSHADERSOURCEPROC)GLGetProcAddress("glShaderSource"); if(!glShaderSource){ return false; }
    glCompileShader = (PFNGLCOMPILESHADERPROC)GLGetProcAddress("glCompileShader"); if(!glCompileShader){ return false; }
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)GLGetProcAddress("glCreateProgram"); if(!glCreateProgram){ return false; }
    glAttachShader = (PFNGLATTACHSHADERPROC)GLGetProcAddress("glAttachShader"); if(!glAttachShader){ return false; }
    glLinkProgram = (PFNGLLINKPROGRAMPROC)GLGetProcAddress("glLinkProgram"); if(!glLinkProgram){ return false; }
    glUseProgram = (PFNGLUSEPROGRAMPROC)GLGetProcAddress("glUseProgram"); if(!glUseProgram){ return false; }
    glDetachShader = (PFNGLDETACHSHADERPROC)GLGetProcAddress("glDetachShader"); if(!glDetachShader){ return false; }
    glDeleteShader = (PFNGLDELETESHADERPROC)GLGetProcAddress("glDeleteShader"); if(!glDeleteShader){ return false; }
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)GLGetProcAddress("glDeleteProgram"); if(!glDeleteProgram){ return false; }
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)GLGetProcAddress("glGetProgramiv"); if(!glGetProgramiv){ return false; }
    glGetShaderiv = (PFNGLGETSHADERIVPROC)GLGetProcAddress("glGetShaderiv"); if(!glGetShaderiv){ return false; }
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)GLGetProcAddress("glGetProgramInfoLog"); if(!glGetProgramInfoLog){ return false; }
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GLGetProcAddress("glGetShaderInfoLog"); if(!glGetShaderInfoLog){ return false; }

    //-------------------------------------------------------------------------
    // Загрузка функций для буферов (VBO)
    //-------------------------------------------------------------------------
    glGenBuffers = (PFNGLGENBUFFERSPROC)GLGetProcAddress("glGenBuffers"); if(!glGenBuffers){ return false; }
    glBindBuffer = (PFNGLBINDBUFFERPROC)GLGetProcAddress("glBindBuffer"); if(!glBindBuffer){ return false; }
    glBufferData = (PFNGLBUFFERDATAPROC)GLGetProcAddress("glBufferData"); if(!glBufferData){ return false; }
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GLGetProcAddress("glDeleteBuffers"); if(!glDeleteBuffers){ return false; }

    //-------------------------------------------------------------------------
    // Загрузка функций для атрибутов вершин
    //-------------------------------------------------------------------------
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)GLGetProcAddress("glGetAttribLocation"); if(!glGetAttribLocation){ return false; }
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GLGetProcAddress("glEnableVertexAttribArray"); if(!glEnableVertexAttribArray){ return false; }
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GLGetProcAddress("glDisableVertexAttribArray"); if(!glDisableVertexAttribArray){ return false; }
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GLGetProcAddress("glVertexAttribPointer"); if(!glVertexAttribPointer){ return false; }

    //-------------------------------------------------------------------------
    // Загрузка функций для Uniform-переменных
    //-------------------------------------------------------------------------
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GLGetProcAddress("glGetUniformLocation"); if(!glGetUniformLocation){ return false; }
    glUniform1f = (PFNGLUNIFORM1FPROC)GLGetProcAddress("glUniform1f"); if(!glUniform1f){ return false; }
    glUniform2f = (PFNGLUNIFORM2FPROC)GLGetProcAddress("glUniform2f"); if(!glUniform2f){ return false; }
    glUniform3f = (PFNGLUNIFORM3FPROC)GLGetProcAddress("glUniform3f"); if(!glUniform3f){ return false; }
    glUniform4f = (PFNGLUNIFORM4FPROC)GLGetProcAddress("glUniform4f"); if(!glUniform4f){ return false; }
	return true; }
	
} }
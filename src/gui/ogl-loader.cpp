#include <GL/gl.h> //OpenGL v1.1
#ifdef _WIN32
//Универсальный указатель GLGetProcAddress -> GLGetProcAddress/glXGetProcAddress
void* (APIENTRY *GLGetProcAddress)(const char* name) = (void* (APIENTRY *)(const char*))wglGetProcAddress;
#elif USE_WAYLAND //libEGL.so.1
#else
#define Window XWindowID
#include <GL/glx.h>
#undef Window
void* (*GLGetProcAddress)(const char* name) = (void* (*)(const char* name))glXGetProcAddress;
#endif

//========================================= OpenGL 2.0 API =========================================
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_STREAM_DRAW  0x88E0
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
//#define GL_FALSE 0
//#define GL_TRUE 1

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

// ------ Сигнатуры функций для работы с шейдерами и программами ------
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

// ------ Сигнатуры функций для работы с буферами (VBO) ------
typedef void (APIENTRY *PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRY *PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);

// ------ Сигнатуры функций для работы с атрибутами вершин ------
typedef GLint (APIENTRY *PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

// ------ Сигнатуры функций для работы с Uniform-переменными ------
typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// ------ Объявление указателей ------

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
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
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
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;

namespace ncpp { namespace GL { unsigned char _OGLVer = 11; const unsigned char& OGLVer=_OGLVer;
bool LoadOGL_20(){ //OpenGL 2.0 API
    //------ Загрузка функций для шейдеров и программ ------
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

    //------ Загрузка функций для буферов (VBO) ------
    glGenBuffers = (PFNGLGENBUFFERSPROC)GLGetProcAddress("glGenBuffers"); if(!glGenBuffers){ return false; }
    glBindBuffer = (PFNGLBINDBUFFERPROC)GLGetProcAddress("glBindBuffer"); if(!glBindBuffer){ return false; }
    glBufferData = (PFNGLBUFFERDATAPROC)GLGetProcAddress("glBufferData"); if(!glBufferData){ return false; }
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)GLGetProcAddress("glBufferSubData"); if(!glBufferSubData){ return false; }
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GLGetProcAddress("glDeleteBuffers"); if(!glDeleteBuffers){ return false; }

    //------ Загрузка функций для атрибутов вершин ------
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)GLGetProcAddress("glGetAttribLocation"); if(!glGetAttribLocation){ return false; }
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GLGetProcAddress("glEnableVertexAttribArray"); if(!glEnableVertexAttribArray){ return false; }
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GLGetProcAddress("glDisableVertexAttribArray"); if(!glDisableVertexAttribArray){ return false; }
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GLGetProcAddress("glVertexAttribPointer"); if(!glVertexAttribPointer){ return false; }

    //------ Загрузка функций для Uniform-переменных ------
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)GLGetProcAddress("glGetUniformLocation"); if(!glGetUniformLocation){ return false; }
    glUniform1f = (PFNGLUNIFORM1FPROC)GLGetProcAddress("glUniform1f"); if(!glUniform1f){ return false; }
    glUniform2f = (PFNGLUNIFORM2FPROC)GLGetProcAddress("glUniform2f"); if(!glUniform2f){ return false; }
    glUniform3f = (PFNGLUNIFORM3FPROC)GLGetProcAddress("glUniform3f"); if(!glUniform3f){ return false; }
    glUniform4f = (PFNGLUNIFORM4FPROC)GLGetProcAddress("glUniform4f"); if(!glUniform4f){ return false; }
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)GLGetProcAddress("glUniformMatrix4fv"); if(!glUniformMatrix4fv){ return false; }
	if(_OGLVer<20) _OGLVer=20; return true; }
	
} }

//========================================= OpenGL 3.0 API =========================================
#ifndef GL_VERTEX_ARRAY_BINDING
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#endif

#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D

#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5

// ------ VAO ------
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef GLboolean (APIENTRY *PFNGLISVERTEXARRAYPROC) (GLuint array);

//FBO?

// Объявляем указатели
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLISVERTEXARRAYPROC glIsVertexArray = NULL;

namespace ncpp { namespace GL {
    bool LoadOGL_30(){
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)GLGetProcAddress("glGenVertexArrays"); if(!glGenVertexArrays){ return false; }
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)GLGetProcAddress("glBindVertexArray"); if(!glBindVertexArray){ return false; }
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)GLGetProcAddress("glDeleteVertexArrays"); if(!glDeleteVertexArrays){ return false; }
        glIsVertexArray = (PFNGLISVERTEXARRAYPROC)GLGetProcAddress("glIsVertexArray"); if(!glIsVertexArray){ return false; }
		if(_OGLVer<30) _OGLVer=30; return true; }
} }

//========================================= OpenGL 3.1 API =========================================
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F

// ------ Сигнатуры для Instancing ------
typedef void (APIENTRY *PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (APIENTRY *PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);

// ------ Сигнатуры для UBO ------
typedef GLuint (APIENTRY *PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar *uniformBlockName);
typedef void (APIENTRY *PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (APIENTRY *PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRY *PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

// Объявление указателей
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = NULL;

PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = NULL;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange = NULL;

namespace ncpp { namespace GL {
    bool LoadOGL_31(){
        glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)GLGetProcAddress("glDrawArraysInstanced"); if(!glDrawArraysInstanced) return false;
        glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)GLGetProcAddress("glDrawElementsInstanced"); if(!glDrawElementsInstanced) return false;
        glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)GLGetProcAddress("glGetUniformBlockIndex"); if(!glGetUniformBlockIndex) return false;
        glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)GLGetProcAddress("glUniformBlockBinding"); if(!glUniformBlockBinding) return false;
        glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)GLGetProcAddress("glBindBufferBase"); if(!glBindBufferBase) return false;
        glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)GLGetProcAddress("glBindBufferRange"); if(!glBindBufferRange) return false;
        if(_OGLVer<31) _OGLVer=31; return true;
    }
	bool LoadOGL(){ return LoadOGL_20()&&LoadOGL_30()&&LoadOGL_31(); }
	bool initGL(){ return LoadOGL(); }
} }
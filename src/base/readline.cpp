#ifdef __cplusplus //C compatible
extern "C" {
#endif

#ifdef _WIN32 // === Windows ===
#ifndef STD_INPUT_HANDLE
#define STD_INPUT_HANDLE -10UL
// #include <wchar.h> //for ReadConsoleW, WideCharToMultiByte
// __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle); struct _OVERLAPPED;
// __declspec(dllimport) int __stdcall ReadConsoleW(void* hInput, void* lpBuffer, unsigned long nNumberOfCharsToRead, unsigned long* lpNumberOfCharsRead, void* lpReserved);
// __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int CodePage, unsigned long dwFlags, const wchar_t*, int, char*, int, const char*, int*);
#endif
#else // === Linux ===
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
long read(int fd, void *buf, size_t count);
#endif
#endif //_WIN32

#ifdef __cplusplus
}
namespace ncpp {
	String readline(){ String input; const char BLOCK_SIZE = 64;
	#ifdef _WIN32
		wchar_t wbuff[BLOCK_SIZE]; unsigned long wCharsCnt = 0;
		while(true){ if(!ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), wbuff, BLOCK_SIZE, &wCharsCnt, 0)){ return input; } if(wCharsCnt == 0){ break; }
			int size = WideCharToMultiByte(65001, 0, wbuff, wCharsCnt, NULL, 0, NULL, NULL); if(size <= 0) return input;
			char buf[BLOCK_SIZE*4]; WideCharToMultiByte(65001, 0, wbuff, wCharsCnt, buf, sizeof(buf), NULL, NULL); input.push(buf, size);
			if(input[input.size()-1] == '\n'){ input.resize(input.size()-2); break; } } return input;
	#else
		char buff[BLOCK_SIZE]; unsigned long bytesRead = 0;
		while(true){ if((bytesRead = read(STDIN_FILENO, buff, BLOCK_SIZE)) <= 0){ return input; }
			input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-1); break; } } return input;
	#endif
	} }
#else
#endif

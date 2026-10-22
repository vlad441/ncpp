#ifdef __cplusplus //C compatible
extern "C" {
#endif

#ifdef _WIN32 // === Windows ===
#ifndef STD_INPUT_HANDLE
#define STD_INPUT_HANDLE -10UL
#include <wchar.h> //for ReadConsoleW, WideCharToMultiByte
__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle); struct _OVERLAPPED;
struct _CONSOLE_READCONSOLE_CONTROL;
__declspec(dllimport) int __stdcall ReadConsoleW(void* hInput, void* lpBuffer, unsigned long nNumberOfCharsToRead, unsigned long* lpNumberOfCharsRead, struct _CONSOLE_READCONSOLE_CONTROL* pInputControl);
__declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int CodePage, unsigned long dwFlags, const wchar_t*, int, char*, int, const char*, int*);
//__declspec(dllimport) int __stdcall ReadFile(void* hFile, void* lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long* lpNumberOfBytesRead, struct _OVERLAPPED* lpOverlapped);
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

		/*while(true){ if(!ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), wbuff, BLOCK_SIZE, &wCharsCnt, 0)){ return input; } if(wCharsCnt == 0){ break; }
			for(unsigned long i=0; i<wCharsCnt; ++i){ wchar_t wc = wbuff[i]; if(wc == L'\r') continue; if(wc == L'\n') return input;
				char utf8[4]; int len=0; // Ручное декодирование UTF-16 -> UTF-8
				if(wc < 0x80){ utf8[0] = (char)wc; len=1; }
				else if(wc < 0x800){ utf8[0] = (char)(0xC0 | (wc >> 6)); utf8[1] = (char)(0x80 | (wc & 0x3F)); len = 2; }
				else{ utf8[0] = (char)(0xE0 | (wc >> 12)); utf8[1] = (char)(0x80 | ((wc >> 6) & 0x3F)); utf8[2] = (char)(0x80 | (wc & 0x3F)); len = 3; } 
				input.push(utf8, len); } } return input;*/
	
		// while(true){ if(!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buff, BLOCK_SIZE, &bytesRead, NULL)){ return input; } if(bytesRead <= 0){ break; } 
			// input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-2); break; } } return input;
	#else
		char buff[BLOCK_SIZE]; unsigned long bytesRead = 0;
		while(true){ if((bytesRead = read(STDIN_FILENO, buff, BLOCK_SIZE)) <= 0){ return input; }
			input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-1); break; } } return input;
	#endif
	} }
#else
#endif

#ifdef __cplusplus //C compatible
extern "C" {
#endif

#ifdef _WIN32 // === Windows ===
#ifndef STD_INPUT_HANDLE
#define STD_INPUT_HANDLE -10UL
__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle); struct _OVERLAPPED;
__declspec(dllimport) int __stdcall ReadFile(void* hFile, void* lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long* lpNumberOfBytesRead, struct _OVERLAPPED* lpOverlapped);
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
	String readline(){ String input; const char BLOCK_SIZE = 64; char buff[BLOCK_SIZE]; unsigned long bytesRead = 0;
	#ifdef _WIN32
		while(true){ if(!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buff, BLOCK_SIZE, &bytesRead, NULL)){ return ""; } if(bytesRead <= 0){ break; } 
			input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-2); break; } } return input;
	#else
		while(true){ if((bytesRead = read(STDIN_FILENO, buff, BLOCK_SIZE)) <= 0){ return input; }
			input.push(buff, bytesRead); if(input[input.size()-1] == '\n'){ input.resize(input.size()-1); break; } } return input;
	#endif
	} }
#else
#endif

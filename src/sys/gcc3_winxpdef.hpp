// _popen in WIN32 not avalible for GCC < 4
#define _popen(cmd, mode) NULL
#define _pclose(fp) 0
//GCC 3.4.2 default _WIN32_WINNT 0x0400 (Win95)

// #if _WIN32_WINNT < 0x0501
// #undef _WIN32_WINNT
// #define _WIN32_WINNT 0x0501 // 0x0400 - Win 95; 0x0410 - Win 98; 0x0500 - Win 2000; 0x0501 - WinXP; 0x0601 - Win7; 0x0602 - Win8; 0x0A00 - Win10;
// #endif

//#if _WIN32_WINNT >= 0x0501
HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
// ===== ncpp-system =====
enum LOGICAL_PROCESSOR_RELATIONSHIP { RelationProcessorCore, RelationNumaNode, RelationCache, RelationProcessorPackage };
struct CACHE_DESCRIPTOR { BYTE Level; BYTE Associativity; WORD LineSize; DWORD Size; DWORD Type; };
typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION { ULONG_PTR ProcessorMask; LOGICAL_PROCESSOR_RELATIONSHIP Relationship;
	union { struct { BYTE Flags; } ProcessorCore; struct { DWORD NodeNumber; } NumaNode;
		CACHE_DESCRIPTOR Cache; ULONGLONG Reserved[2]; } DUMMYUNIONNAME;
} SYSTEM_LOGICAL_PROCESSOR_INFORMATION, *PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;

//extern "C" BOOL WINAPI GetLogicalProcessorInformation(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
extern "C" BOOL WINAPI GetSystemTimes(LPFILETIME, LPFILETIME, LPFILETIME);

BOOL WINAPI (*GetLogicalProcessorInformation)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
//BOOL WINAPI (*GetSystemTimes)(LPFILETIME, LPFILETIME, LPFILETIME);

typedef struct _MEMORYSTATUSEX { DWORD dwLength; DWORD dwMemoryLoad; DWORDLONG ullTotalPhys; DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile; DWORDLONG ullAvailPageFile; DWORDLONG ullTotalVirtual; DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual; } MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

extern "C" BOOL WINAPI GlobalMemoryStatusEx(LPMEMORYSTATUSEX lpBuffer);
extern "C" BOOL WINAPI GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);

// ===== net-socket =====
//typedef HANDLE IPHANDLE;
#ifndef IP_OPTION_INFORMATION
typedef struct ip_option_information { unsigned char Ttl; unsigned char Tos; unsigned char Flags; unsigned char OptionsSize; unsigned char FAR *OptionsData; } IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;
#endif
#ifndef WSAID_ACCEPTEX
static const GUID WSAID_ACCEPTEX = { 0xb5367df1, 0xcbac, 0x11cf, { 0x95, 0xca, 0x00, 0xaa, 0x00, 0x68, 0x0d, 0xb4 } };
#endif

typedef struct icmp_echo_reply { unsigned long Address;
    unsigned long Status; unsigned long RoundTripTime;
    unsigned short DataSize;  unsigned short Reserved;
    void *Data; struct ip_option_information Options; } ICMP_ECHO_REPLY, *PICMP_ECHO_REPLY;

extern HANDLE WINAPI IcmpCreateFile(void);
extern BOOL WINAPI IcmpCloseHandle(HANDLE);
extern DWORD WINAPI IcmpSendEcho(HANDLE, unsigned long, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

#ifndef IPV6_V6ONLY
#define IPV6_V6ONLY 27
#endif

struct tcp_keepalive { u_long onoff; u_long keepalivetime; u_long keepaliveinterval; };
#ifndef SIO_KEEPALIVE_VALS
#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR, 4)
#endif

// ===== system-thread =====
HANDLE WINAPI (*OpenThread)(DWORD, BOOL, DWORD);

// ===== net-server =====
typedef BOOL (PASCAL * LPFN_ACCEPTEX)(SOCKET, SOCKET, PVOID, DWORD, DWORD, DWORD, LPDWORD, LPOVERLAPPED);

static void __InitWinXP_SDK(void){ const char name[]="kernel32.dll Init Error";
	GetLogicalProcessorInformation = (BOOL (WINAPI *)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD))GetProcAddress(hKernel32, "GetLogicalProcessorInformation");
	if(GetLogicalProcessorInformation==NULL){ MessageBoxA(0, "GetProcAddress: GetLogicalProcessorInformation() load fail", name, MB_ICONERROR); ExitProcess(1);	}
	OpenThread = (HANDLE (WINAPI *)(DWORD, BOOL, DWORD))GetProcAddress(hKernel32, "OpenThread");
	if(OpenThread==NULL){ MessageBoxA(0, "GetProcAddress: OpenThread() load fail", name, MB_ICONERROR); ExitProcess(1);	}
} 
struct __gcc3_InitWinXP_SDK_ { __gcc3_InitWinXP_SDK_(){ __InitWinXP_SDK(); } } __gcc3_InitWinXP_SDK_struct;
//typedef void (*PVFV)(void);
//__attribute__((used, section(".CRT$XCU"))) static PVFV __InitWinXP_SDK_ptr = __InitWinXP_SDK;

//undefined reference to `IcmpCreateFile'
//undefined reference to `IcmpSendEcho'
//undefined reference to `IcmpCloseHandle'
//#endif //_WIN32_WINNT >= 0x0501
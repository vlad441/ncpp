#ifndef _WIN32
#include <sys/un.h> // sockaddr_un
//#define offsetof(type, member) ((size_t) &(((type *)0)->member))
#endif
namespace ncpp{ struct UnixSocket : TCPSocket { char _utype; String path;
	bool isAbstract() const { return !path.empty() && path[0] == '@'; }
	~UnixSocket(){ destroy(); }
	#ifdef _WIN32
	UnixSocket(const std::string& sockPath, int type=SOCK_STREAM) : TCPSocket(), _utype(type), path(sockPath), hPipe(NULL){ _type=Socket::UNIX; }
	UnixSocket(int type=SOCK_STREAM, HANDLE sockid1=NULL) : TCPSocket(), _utype(type), hPipe(sockid1){ _type=Socket::UNIX; if(hPipe!=NULL) connected=true; }
	HANDLE hPipe;
	bool bind(bool auto_unlink=true){ return true; }
	bool listen(){ return true; }
	
	bool connect(){ pathNormalize(); hPipe = CreateFile(TEXT(path.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		connected = true; return true; }
	
	UnixSocket* accept(){ HANDLE pipe = _bind(); if(pipe==NULL){ return nullptr; } ConnectNamedPipe(pipe, NULL); return new UnixSocket(_utype, pipe); }
	UnixSocket _accept(){ HANDLE pipe = _bind(); if(pipe==NULL){ return UnixSocket(); } ConnectNamedPipe(pipe, NULL); return UnixSocket(_utype, pipe); }
	
	int send(const Buffer& buff){ DWORD wrbytes; WriteFile(hPipe, &buff[0], buff.size(), &wrbytes, NULL); return wrbytes; }
	int recv(Buffer* buff){ if(buff->size()<buff->capacity()){ buff->resize(buff->capacity()); }
			rsetErr(); DWORD rbytes; ReadFile(hPipe, &(*buff)[0], buff->size(), &rbytes, NULL);
			if(rbytes<=0){ buff->resize(0); if(rbytes==0){ destroy(); }else{ GetErr(); } }
			else{ buff->resize(rbytes); } return rbytes; }
	Buffer recv(){ Buffer buff(buffsize); recv(&buff); return buff; }
	private: 
		inline void pathNormalize(){ if(path.startsWith("\\\\.\\pipe\\")) return; 
			if(isAbstract()){ path=path.slice(1); } if(path.startsWith("/tmp/")){ path=path.slice(5); } path="\\\\.\\pipe\\"+path; }
		HANDLE _bind(){ pathNormalize(); return CreateNamedPipe(TEXT(path.c_str()), PIPE_ACCESS_DUPLEX, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, buffsize, buffsize, 0, NULL); }
	
	#else
	UnixSocket(const std::string& sockPath, int type=SOCK_STREAM) : TCPSocket(), _utype(type), path(sockPath){ _type=Socket::UNIX; }
	UnixSocket(int type=SOCK_STREAM, int sockid1=-1) : TCPSocket(sockid1), _utype(type){ _type=Socket::UNIX; }

	bool bind(bool auto_unlink=true){ if(path.empty()){ return false; } if(auto_unlink) ::unlink(path.c_str());
		sockfd = socket(AF_UNIX, (int)_utype, 0); if(sockfd < 0){ perror("socket"); return false; }
		sockaddr_un addr; memset(&addr, 0, sizeof(addr)); addr.sun_family = AF_UNIX;
		if(isAbstract()){ addr.sun_path[0] = '\0'; strncpy(&addr.sun_path[1], path.c_str() + 1, sizeof(addr.sun_path) - 2);
		}else{ strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1); }
		if(::bind(sockfd, (sockaddr*)&addr, sizeof(sockaddr_un)) < 0){ perror("UnixSocket: bind error"); return false; } return true; }
		
		
	bool connect(){ if(path.empty()){ return false; }
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0); if(sockfd < 0){ perror("socket"); return false; }
		sockaddr_un addr; std::memset(&addr, 0, sizeof(addr)); addr.sun_family = AF_UNIX;
		if(path[0] == '@'){ addr.sun_path[0] = '\0'; std::strncpy(addr.sun_path + 1, path.c_str() + 1, sizeof(addr.sun_path)-2);
		}else{ std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path)-1); }
		if(::connect(sockfd, (sockaddr*)&addr, sizeof(sockaddr_un))<0){ perror("UnixSocket: connect error"); destroy(); return false; }
		connected = true; return true; }

	UnixSocket* accept(){ int clientfd = ::accept(sockfd, NULL, NULL);
		if(clientfd < 0){ return nullptr; } return new UnixSocket(SOCK_STREAM, clientfd); }
	UnixSocket _accept(){ int clientfd = ::accept(sockfd, NULL, NULL); 
		if(clientfd < 0){ return UnixSocket(); } return UnixSocket(SOCK_STREAM, clientfd); }
	
	// === only linux, DGRAM mode ===
	int sendto(const char* cstr, int len=0){ sockaddr_un addr; memset(&addr, 0, sizeof(addr)); addr.sun_family = AF_UNIX;
		if (isAbstract()){ addr.sun_path[0] = '\0'; strncpy(&addr.sun_path[1], path.c_str() + 1, sizeof(addr.sun_path) - 2); }
		else{ strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1); }
		return ::sendto(sockfd, cstr, len, 0, (sockaddr*)&addr, sizeof(sockaddr_un)); }
	
	int recvfrom(Buffer* buff, std::string* outSender = nullptr){
		if(buff->size()<buff->capacity()){ buff->resize(buff->capacity()); }
		sockaddr_un addr; socklen_t addrlen = sizeof(addr);
		ssize_t rbytes = ::recvfrom(sockfd, (char*)&(*buff)[0], buff->size(), 0, (sockaddr*)&addr, &addrlen);
		if(outSender && rbytes > 0){ if(addr.sun_path[0] == '\0') *outSender = std::string("\0") + (addr.sun_path + 1);
			else *outSender = std::string(addr.sun_path); }
		return rbytes; }

	static bool socketPair(UnixSocket& a, UnixSocket& b){ // Unnamed socketpair
		int sv[2]; if(::socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) return false;
		a.sockfd = sv[0]; b.sockfd = sv[1]; a.connected = true; b.connected = true; return true; }
	#endif
}; }



#ifdef _WIN32 // == Windows Headers ==

#if !defined(__GNUC__) || __GNUC__ >= 4
#include <iphlpapi.h> //icmp
#include <icmpapi.h>
#endif

#ifndef EINTR
#define EINTR WSAEINTR
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#ifndef EAGAIN
#define EAGAIN WSAEWOULDBLOCK
#endif

#else // == Linux Headers ==
#include <netinet/ip_icmp.h> //icmp
#include <netinet/tcp.h> // Keep-Alive: TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT
#endif

#if defined(_WIN32) && _WIN32_WINNT < 0x0600
int inet_pton(int af, const char *src, void *dst)
{ struct sockaddr_storage ss; int size = sizeof(ss); char src_copy[INET6_ADDRSTRLEN+1];
  ZeroMemory(&ss, sizeof(ss)); // stupid non-const API
  strncpy(src_copy, src, INET6_ADDRSTRLEN+1); src_copy[INET6_ADDRSTRLEN] = 0;

  if(WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0){
	switch(af){
	  case AF_INET: *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr; return 1;
	  case AF_INET6: *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr; return 1; } } return 0; }

const char* inet_ntop(int af, const void *src, char *dst, socklen_t size)
{ struct sockaddr_storage ss; unsigned long s = size;
  ZeroMemory(&ss, sizeof(ss)); ss.ss_family = af;
  switch(af){
	case AF_INET: ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src; break;
	case AF_INET6: ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src; break;
	default: return NULL; } // cannot direclty use &size because of strict aliasing rules
  return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)? dst : NULL; }
#endif

namespace ncpp{ 
#define DEF_SOCK_SIZE 4096 //#define DEF_SOCK_SIZE 16384
	struct BindInfo { String ip; int port; String family; String type; };
	struct IPAddr { String ip; int port; char ipver; //TODO: MMDB LPM (Radix Tree)
		IPAddr(const CString& ip="", int port=0, char ipver=4): ip(ip), port(port), ipver(ipver){}
		String toString() const { if(ipver==4){ return ip+":"+dtos(port); }else{ return "["+ip+"]:"+dtos(port); } }
		static IPAddr fromStr(const CString& host){ Array<String> ip; IPAddr addr; 
			if(host.indexOf("]:")!=NPOS){ ip=host.split("]:", 2, 2); ip[0]=ip[0].slice(1); addr.ipver=6; }
			else if(host.endsWith("]")){ ip.push(host.slice(1, -1)); addr.ipver=6; }else{ ip=host.split(":", 1, 2); addr.ipver=4; }
			addr.ip=ip[0]; if(ip.size()>1){ addr.port=stoin(ip[1]); } return addr; }
	};
	
	struct Socket { int sockfd; bool connected; bool isbind; enum Type { NONE, TCP, UDP, UNIX } _type;
		IPAddr destAddr; bool autodestroy; int errcode; //int buffsize; void* data; volatile unsigned int ref_cnt;
        Socket() : sockfd(-1), connected(false), isbind(false), _type(TCP), autodestroy(true), errcode(0){}
		Socket(const CString& ip, int port, Type type=TCP) : sockfd(-1), connected(false), isbind(false), _type(type), autodestroy(true), errcode(0){ destAddr.ip=ip; destAddr.port=port; }
		Socket(int sockfd) : sockfd(sockfd), connected(true), isbind(false), _type(TCP), autodestroy(true), errcode(0){}
        ~Socket(){ if(autodestroy) destroy(); } //if(data!=NULL){  }
		
		#ifdef _WIN32
		static bool WSAinited;
		static void WIN_WSAinit(){ if(!WSAinited){ WSADATA wsaData; if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ Except("connect: WSAStartup failed.\n"); return; } WSAinited=true; } }
		inline static int GetLastErr(){ return WSAGetLastError(); }
		inline int GetErr(){ return (errcode=WSAGetLastError()); }
		inline void rsetErr(){ errcode=0; }
		inline static int socket(int family, int socktype, int protocol){ return WSASocket(family, socktype, protocol, NULL, 0, WSA_FLAG_OVERLAPPED); }
		#else
		inline static void WIN_WSAinit(){}
		inline static int GetLastErr(){ return errno; }
		inline int GetErr(){ return (errcode=errno); } //_errparse(errcode); std::strerror(errno);
		inline void rsetErr(){ errno=0; errcode=0; }
		#endif
		int getfd(){ return sockfd; }
		Socket& own(bool en=true){ autodestroy=en; return *this; }
		inline static int create(char ipver=4, int type=TCP){ return socket(ipver==4?AF_INET:AF_INET6, type==TCP?SOCK_STREAM:SOCK_DGRAM, 0); }
		
		bool connect(const CString& ip, int port){ if(connected){ return false; } WIN_WSAinit(); errcode=0;
            struct addrinfo hints, *res, *p; memset(&hints, 0, sizeof(hints)); hints.ai_family = AF_UNSPEC; // AF_INET или AF_INET6 для IPv4 или IPv6
            if(_type==TCP){ hints.ai_socktype = SOCK_STREAM; }else{ hints.ai_socktype = SOCK_DGRAM; } // SOCK_STREAM, SOCK_DGRAM
            
			if(getaddrinfo(ip.c_str(), dtos(port).c_str(), &hints, &res) != 0){ Except("connect: getaddrinfo failed.\n"); return false; }
            for(p = res; p != NULL; p = p->ai_next){ sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); if(sockfd<0){ continue; }
				int opt=1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)); // allow SO_REUSEADDR
                if(_type==TCP){ if(::connect(sockfd, p->ai_addr, p->ai_addrlen) == 0){ connected = true; }else{ continue; } }
                destAddr = _from_sockaddr_storage(reinterpret_cast<sockaddr_storage*>(p->ai_addr)); connected = true; break; } freeaddrinfo(res);
			if(sockfd<0){ Except("Socket create failed: "+dtos(GetErr())+"\n"); return false; }
            if(!connected){ Except("Connection failed: "+dtos(GetErr())+"\n"); destroy(sockfd); return false; }
			return true; }
		bool connect(const IPAddr& a){ return connect(a.ip, a.port); };
		bool connect(){ return connect(destAddr.ip, destAddr.port); };
            
        bool connect4(const CString& ip, int port){ if(connected){ return false; } WIN_WSAinit(); errcode=0;
			struct sockaddr_in addr; memset(&addr, 0, sizeof(addr)); addr.sin_family = AF_INET; addr.sin_port = htons(port);
			if (inet_addr(ip.c_str())!=INADDR_NONE){ addr.sin_addr.s_addr = inet_addr(ip.c_str()); } 
			else{ struct hostent *hst; hst = gethostbyname(ip.c_str()); if(hst){ addr.sin_addr = *(struct in_addr*)hst->h_addr; }
				else{ Except("connect4: gethostbyname failed.\n"); return false; } }

			sockfd = socket(AF_INET, SOCK_STREAM, 0); if(sockfd<0){ Except("Socket create failed.\n"); return false; }
			if(_type==TCP){ if(::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){ 
				Except("Connection failed: "+dtos(GetErr())+"\n"); destroy(sockfd); return false; } }
			connected = true; destAddr = _from_sockaddr_in(&addr); return true; }
		bool connect4(){ return connect4(destAddr.ip, destAddr.port); };
			
		bool bind(int port = 0, const CString& bindip = "::"){ WIN_WSAinit(); errcode=0;
			struct addrinfo hints, *res, *p; memset(&hints, 0, sizeof(hints)); hints.ai_family = AF_UNSPEC;
			if(_type==TCP){ hints.ai_socktype = SOCK_STREAM; }else{ hints.ai_socktype = SOCK_DGRAM; } hints.ai_flags = AI_PASSIVE;  // Используем для сокетов сервера

			if(getaddrinfo(bindip.c_str(), dtos(port).c_str(), &hints, &res) != 0){ Except("bind: getaddrinfo failed.\n"); return false; }
			for(p = res; p != NULL; p = p->ai_next){ sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); if(sockfd<0){ continue; }
			    int opt=0; setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&opt, sizeof(opt)); // allow IPv6 accept IPv4.
			    opt=1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)); // allow SO_REUSEADDR
				if(::bind(sockfd, p->ai_addr, p->ai_addrlen) == 0){ isbind = true; break; } }
			freeaddrinfo(res); if(!isbind){ Except("bind failed: "+dtos(GetErr())+"\n"); return false; }
			return true; }
			
		bool listen(int maxqueue=0){ if(maxqueue<=0){ maxqueue=SOMAXCONN; } if(::listen(sockfd, maxqueue) == 0){ return true; } return false; }
		
		static BindInfo address(int sockfd1){ BindInfo addrInfo; sockaddr_storage addr; socklen_t addrLen = sizeof(addr); WIN_WSAinit(); char host[NI_MAXHOST]; char port_str[NI_MAXSERV];
			if (getsockname(sockfd1, (sockaddr*)&addr, &addrLen) == -1){ Except("Socket.address: getsockname failed.\n"); return addrInfo; }
			if(getnameinfo((sockaddr*)&addr, addrLen, host, NI_MAXHOST, port_str, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) != 0){ 
				Except("Socket.address: getnameinfo failed.\n"); return addrInfo; }

			addrInfo.ip = host; addrInfo.port = stodn(String(port_str));
			if (addr.ss_family == AF_INET){ addrInfo.family = "IPv4"; }else if(addr.ss_family == AF_INET6){ addrInfo.family = "IPv6"; }else{ addrInfo.family = "(Unknown)"; }

			int sockType; socklen_t optLen = sizeof(sockType);
			if (getsockopt(sockfd1, SOL_SOCKET, SO_TYPE, (char*)&sockType, &optLen) == -1){ Except("Socket.address: getsockopt failed.\n"); return addrInfo; }

			if (sockType == SOCK_STREAM){ addrInfo.type = "TCP"; }else if (sockType == SOCK_DGRAM){ addrInfo.type = "UDP"; }else{ addrInfo.type = "(Unspec protocol)"; } return addrInfo; }
		BindInfo address(){ return address(sockfd); }
				
		static bool destroy(int sockfd1, bool force=true){
			#ifdef _WIN32
                return ::closesocket(sockfd1)==0; //WSACleanup();
			#else           
				if(!force){ ::shutdown(sockfd1, SHUT_RDWR); } return ::close(sockfd1)==0;
			#endif
		}
        bool destroy(){ if(sockfd>=0){ int fd=sockfd; sockfd=-1; connected=false; return destroy(fd); } return true; }
        bool close(){ if(sockfd>=0){ int fd=sockfd; sockfd=-1; connected=false; return destroy(fd, false); } return true; }
				
		bool recvTimeout(int msecs){
		#ifdef _WIN32
			DWORD timeout = msecs; if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0){ Except("setrecvTimeout: setsockopt failed.\n"); return false; }
		#else
			struct timeval tv; tv.tv_sec = 0; tv.tv_usec = msecs*1000;
			if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){ Except("setrecvTimeout: setsockopt failed.\n"); return false; }
		#endif
			return true; }
		
		static void setNonBlocking(int sockfd1, bool nb_mode=true){
		#ifdef _WIN32
			u_long mode = nb_mode?1:0; ioctlsocket(sockfd1, FIONBIO, &mode);
		#else
			int flags = fcntl(sockfd1, F_GETFL, 0); if(flags < 0) return;
			if(nb_mode){ fcntl(sockfd1, F_SETFL, flags | O_NONBLOCK); }else{ fcntl(sockfd1, F_SETFL, flags & ~O_NONBLOCK); }
		#endif
		}
		inline void setNonBlocking(bool nb_mode=true){ return setNonBlocking(sockfd, nb_mode); }
		
		static bool setKeepAlive(int sockfd1, bool en, int idle_time=720, int interval=5, int cnt=5){
		#ifdef _WIN32
			idle_time*=1000; int bOptLen = sizeof(en); if(setsockopt(sockfd1, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, bOptLen) == SOCKET_ERROR){ Except("setsockopt (SO_KEEPALIVE) failed\n"); return false; }
			if(!en||idle_time<=0) return true; tcp_keepalive ka; DWORD bytesReturned; ka.onoff = 1; ka.keepalivetime = (ULONG)idle_time; ka.keepaliveinterval = (ULONG)interval;
			if(WSAIoctl(sockfd1, SIO_KEEPALIVE_VALS, &ka, sizeof(ka), NULL, 0, &bytesReturned, NULL, NULL) == SOCKET_ERROR){ Except("WSAIoctl (SIO_KEEPALIVE_VALS) failed\n"); return false; }
		#else
			int optval = en; socklen_t optlen = sizeof(optval); if(setsockopt(sockfd1, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0){ perror("setsockopt(SO_KEEPALIVE) failed"); return false; }
			if(!en||idle_time<=0) return true; optval = idle_time;
			if(setsockopt(sockfd1, IPPROTO_TCP, TCP_KEEPIDLE, &optval, optlen) < 0){ perror("setsockopt (TCP_KEEPIDLE) failed"); return false; }
			optval = interval; if(setsockopt(sockfd1, IPPROTO_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) { perror("setsockopt (TCP_KEEPINTVL) failed"); return false; }
			optval = cnt; if(setsockopt(sockfd1, IPPROTO_TCP, TCP_KEEPCNT, &optval, optlen) < 0){ perror("setsockopt (TCP_KEEPCNT) failed"); return false; }
		#endif
			return true; }
		inline bool setKeepAlive(bool en=true, int idle_time=720, int interval=5, int cnt=5){ return setKeepAlive(sockfd, en, idle_time, interval, cnt); }
		
		String type(){ if(_type==TCP){ return "TCP"; }else if(_type==UDP){ return "UDP"; }else if(_type==UNIX){ return "UNIX"; }else{ return "(Unspec protocol)"; } }
        
        // lo-lvl address translation
        static IPAddr _from_sockaddr_in(const sockaddr_in* addr4){ IPAddr result; char ipstr[INET_ADDRSTRLEN]; result.port = ntohs(addr4->sin_port); 
			result.ipver = 4; inet_ntop(AF_INET, (void*)&(addr4->sin_addr), ipstr, sizeof(ipstr)); result.ip = ipstr; return result; }
		static IPAddr _from_sockaddr_in6(const sockaddr_in6* addr6){ IPAddr result; char ipstr[INET6_ADDRSTRLEN]; result.port = ntohs(addr6->sin6_port); 
			result.ipver = 6; inet_ntop(AF_INET6, (void*)&(addr6->sin6_addr), ipstr, sizeof(ipstr)); result.ip = ipstr; return result; }
		static IPAddr _from_sockaddr_storage(const sockaddr_storage* addr){
			if(addr->ss_family == AF_INET){ return _from_sockaddr_in(reinterpret_cast<const sockaddr_in*>(addr)); } 
			else if(addr->ss_family == AF_INET6){ return _from_sockaddr_in6(reinterpret_cast<const sockaddr_in6*>(addr)); } 
			else{ IPAddr result; result.ipver = 0; return result; } }
		sockaddr_storage _to_sockaddr_storage(const IPAddr& addr){ sockaddr_storage storage; memset(&storage, 0, sizeof(storage));
			if(addr.ipver == 4){ sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(&storage);
				addr_in->sin_family = AF_INET; addr_in->sin_port = htons(addr.port);
				if(inet_pton(AF_INET, addr.ip.c_str(), (void*)&(addr_in->sin_addr)) <= 0){ Except("_to_sockaddr_storage: Invalid IPv4 address format\n"); } } 
			else{ sockaddr_in6* addr_in6 = reinterpret_cast<sockaddr_in6*>(&storage);
				addr_in6->sin6_family = AF_INET6; addr_in6->sin6_port = htons(addr.port);
				if(inet_pton(AF_INET6, addr.ip.c_str(), (void*)&(addr_in6->sin6_addr)) <= 0){ Except("_to_sockaddr_storage: Invalid IPv6 address format\n"); } }
			return storage; }
			
		friend void swap(Socket& a, Socket& b){ int tmpfd = a.sockfd; a.sockfd = b.sockfd; b.sockfd = tmpfd;
			bool tmpb = a.connected; a.connected = b.connected; b.connected = tmpb;
			tmpb = a.isbind; a.isbind = b.isbind; b.isbind = tmpb;
			Type _tmpt = a._type; a._type = b._type; b._type = _tmpt;
			IPAddr tmpaddr = a.destAddr; a.destAddr = b.destAddr; b.destAddr = tmpaddr;
			tmpb = a.autodestroy; a.autodestroy = b.autodestroy; b.autodestroy = tmpb;
			tmpfd = a.errcode; a.errcode = b.errcode; b.errcode = tmpfd; }
			
		#if __cplusplus >= 201103L //move for C++11
		Socket(Socket&& tmp) noexcept { move(*this, tmp); }
		Socket& operator=(Socket&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
		Socket(const Socket&) = default; //Явно разрешить копирование (временный костыль).
		//Socket(const Socket&) = delete; Socket& operator=(const Socket&) = delete; //Запрет копирования.
		Socket& steal(Socket& tmp){ move(*this, tmp); return *this; }
		Socket& steal(Socket&& tmp){ move(*this, tmp); return *this; }
		friend void move(Socket& dst, Socket&& tmp){ move(dst, (Socket&)tmp); }
		#else //move for C++98
		//private: Socket(const Socket&); Socket& operator=(const Socket&); public: //Скрытие копирования.
		Socket& steal(const Socket& victim){ move(*this, (Socket&)victim); return *this; }
		friend void move(Socket& dst, const Socket& victim){ move(dst, (Socket&)victim); }
		#endif
		friend void move(Socket& dst, Socket& tmp){ if(&dst==&tmp) return; if(dst.autodestroy) dst.destroy();
			dst.sockfd = tmp.sockfd; tmp.sockfd = -1;
			dst.connected = tmp.connected; tmp.connected = false;
			dst.isbind = tmp.isbind; tmp.isbind = false;
			dst._type = tmp._type; tmp._type = TCP;
			dst.destAddr = tmp.destAddr; tmp.destAddr = IPAddr();
			dst.autodestroy = tmp.autodestroy; tmp.autodestroy = true;
			dst.errcode = tmp.errcode; tmp.errcode = 0; }
	};
    #ifdef _WIN32
    bool Socket::WSAinited = false;
	#endif
    
	
    struct TCPSocket : Socket {
		TCPSocket() : Socket(){}
		TCPSocket(const CString& ip, int port, bool toconn=false) : Socket(ip, port, Socket::TCP){ if(toconn) connect(); }
		TCPSocket(const IPAddr& addr, bool toconn=false) : Socket(addr.ip, addr.port, Socket::TCP){ if(toconn) connect(); }
		TCPSocket(int sockfd1) : Socket(sockfd1){}
		TCPSocket(const Socket& sock) : Socket(sock){ this->autodestroy=false; }
		
		int send(const char* cptr, int len){ return ::send(sockfd, cptr, len, 0); }
		int send(const char* cstr){ return ::send(sockfd, cstr, strlen(cstr), 0); }
		int send(const Buffer& buff){ return ::send(sockfd, (const char*)buff.data(), buff.size(), 0); }
		
		int recv(char* ptr, int len){ rsetErr(); int bytesRead = ::recv(sockfd, ptr, len, 0);
			if(bytesRead<=0){ destroy(); if(bytesRead<0) GetErr(); } return bytesRead; }
		int recv(Buffer* buff){ buff->resize(DEF_SOCK_SIZE); rsetErr(); 
			int bytesRead = ::recv(sockfd, (char*)buff->data(), buff->size(), 0);
			if(bytesRead<=0){ buff->resize(0); destroy(); if(bytesRead<0) GetErr(); }
			else{ buff->resize(bytesRead); } return bytesRead; }
		Buffer recv(){ Buffer buff; recv(&buff); return buff; }
		//inline Buffer read(){ return recv(); }
		
		int acceptFd() const { return ::accept(sockfd, NULL, NULL); }
		int acceptFd(IPAddr* addr) const { sockaddr_storage client_addr; socklen_t addrlen = sizeof(client_addr); 
			int fd = ::accept(sockfd, (sockaddr*)&client_addr, &addrlen); *addr = _from_sockaddr_storage(&client_addr); return fd; }
		
		TCPSocket* accept() const { TCPSocket* s = new TCPSocket(); s->sockfd = acceptFd(&s->destAddr); return s; }
		TCPSocket _accept() const { TCPSocket s; s.sockfd = acceptFd(&s.destAddr); return s; }
			
		//friend void move(TCPSocket& dst, TCPSocket& tmp){ move((Socket&)dst, (Socket&)tmp); }
	};
    
	struct UDPSocket : Socket {
		UDPSocket() : Socket(){ _type=UDP; }
		UDPSocket(const CString& ip, int port, bool toconn=false) : Socket(ip, port, Socket::UDP){ if(toconn) connect(); }
		UDPSocket(const IPAddr& addr, bool toconn=false) : Socket(addr.ip, addr.port, Socket::UDP){ if(toconn) connect(); }
		UDPSocket(int sockfd1) : Socket(sockfd1){ _type=UDP; }
		
		//UDPSocket& setAddr(const IPAddr& addr){ destAddr=addr; return *this; }
		
		int send(const IPAddr& addr, const char* cstr, int len){ struct sockaddr_storage dest_addr = _to_sockaddr_storage(addr);
			return ::sendto(sockfd, cstr, len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); }
		int send(const IPAddr& addr, const Buffer& buffer){ struct sockaddr_storage dest_addr = _to_sockaddr_storage(addr);
			return ::sendto(sockfd, (const char*)buffer.data(), buffer.size(), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); }
		int send(const IPAddr& addr, const char* cstr){ return send(addr, cstr, strlen(cstr)); }
		int send(const Buffer& buff){ return send(destAddr, buff); }
		int send(const char* cstr, int len){ return send(destAddr, cstr, len); }
		int send(const char* cstr){ return send(destAddr, cstr, strlen(cstr)); }
	
		int recv(char* ptr, int len, IPAddr* rinfo=NULL){ rsetErr(); int bytesRead;
			if(rinfo!=NULL){ sockaddr_storage dest_addr; socklen_t addrlen = sizeof(dest_addr); 
				bytesRead = ::recvfrom(sockfd, ptr, len, 0, (sockaddr*)&dest_addr, &addrlen); 
				*rinfo=_from_sockaddr_storage(&dest_addr); }
			else{ bytesRead = ::recvfrom(sockfd, ptr, len, 0, NULL, NULL); }
			if(bytesRead<=0){ destroy(); if(bytesRead<0) GetErr(); } return bytesRead; }
		int recv(Buffer* buff, IPAddr* rinfo=NULL){ buff->resize(DEF_SOCK_SIZE);
			int rbytes = recv((char*)buff->data(), buff->size(), rinfo); buff->resize(rbytes>0?rbytes:0); return rbytes; }
		Buffer recv(){ Buffer buff; recv(&buff); return buff; }
		//inline Buffer read(){ return recv(); }
		
		//friend void move(UDPSocket& dst, UDPSocket& tmp){ move((Socket&)dst, (Socket&)tmp); }
		private:
			bool connect(const CString& ip, int port); //Скрываем ("Запрещаем") connect() для UDP.
			bool connect(const IPAddr& a); bool connect();
	};
	
	//struct UnixSocket {}
	struct ICMPSocket {
		static double ping4(const CString& ip){
		#ifdef _WIN32
			HANDLE hIcmpFile; char sendData[32] = "Data for ICMP packet"; hIcmpFile = IcmpCreateFile();
			if (hIcmpFile == INVALID_HANDLE_VALUE){ Except("ICMPSocket: Create ICMP handle fail: "+dtos(GetLastError())+"\n"); return -1; }
			Buffer rbuff(sizeof(ICMP_ECHO_REPLY)+sizeof(sendData));
			DWORD dwRetVal = IcmpSendEcho(hIcmpFile, inet_addr(ip.c_str()), sendData, sizeof(sendData), NULL, (void*)rbuff.data(), rbuff.size(), 2000);
			if(dwRetVal != 0){ PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)rbuff.data(); IcmpCloseHandle(hIcmpFile); return pEchoReply->RoundTripTime; }
			else{ Except("Ping failed. Error: "+dtos(GetLastError())+"\n"); IcmpCloseHandle(hIcmpFile); return -1; }
		#else           
			int sockfd = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); if(sockfd < 0){ Except("ICMPSocket: Creation failed.\n"); return -1; }
		    struct sockaddr_in dest_addr; memset(&dest_addr, 0, sizeof(dest_addr));
			dest_addr.sin_family = AF_INET; inet_pton(AF_INET, ip.c_str(), &dest_addr.sin_addr);

			char packet[64]; memset(packet, 0, sizeof(packet));
			struct icmphdr* icmp_header = reinterpret_cast<struct icmphdr*>(packet);
			icmp_header->type = ICMP_ECHO; icmp_header->code = 0;
			icmp_header->un.echo.id = htons(getpid()); icmp_header->un.echo.sequence = htons(1);
			icmp_header->checksum = 0; icmp_header->checksum = calc_checksum(packet, sizeof(packet));
			struct timeval tv; tv.tv_sec = 2; tv.tv_usec = 0; if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0){ Except("setsockopt: SO_RCVTIMEO failed.\n"); }
			
			double ptime = performance::now(); ssize_t bytes_sent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
			if(bytes_sent <= 0){ Except("Failed to send packet.\n"); close(sockfd); return -1; }

			char reply[1024]; struct sockaddr_in reply_addr; socklen_t addr_len = sizeof(reply_addr);
			ssize_t bytes_received = recvfrom(sockfd, reply, sizeof(reply), 0, (struct sockaddr*)&reply_addr, &addr_len);
			ptime = bytes_received>0?performance::now()-ptime:-1; close(sockfd); return ptime;
		#endif
		}
		static double ping4(const IPAddr& addr){ return ping4(addr.ip); }
		private: static unsigned short calc_checksum(void* buffer, int length){
			unsigned short* buf = reinterpret_cast<unsigned short*>(buffer);
			unsigned int sum = 0; for(; length > 1; length -= 2){ sum += *buf++; }
			if (length == 1){ sum += *reinterpret_cast<unsigned char*>(buf); }
			sum = (sum >> 16) + (sum & 0xFFFF); sum += (sum >> 16); return static_cast<unsigned short>(~sum); }
	};
}

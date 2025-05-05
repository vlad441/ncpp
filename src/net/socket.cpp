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

#ifdef _WIN32
#if !defined(__GNUC__) || __GNUC__ >= 4
#include <iphlpapi.h> //icmp
#include <icmpapi.h>
#endif
#else
#include <netinet/ip_icmp.h>
#endif

namespace ncpp{ 
#define DEF_BUFF_SIZE 16384
	struct BindInfo { std::string ip; int port; std::string family; std::string type; };
	struct IPAddr { std::string ip; int port; char ipver;
		IPAddr(const std::string& ip="", int port=0, char ipver=4): ip(ip), port(port), ipver(ipver){} 
		std::string toString() const { if(ipver==4){ return ip+":"+dtos(port); }else{ return "["+ip+"]:"+dtos(port); } }
	}; std::ostream& operator<<(std::ostream& os, const IPAddr& addr){ os << addr.toString(); return os; } // Перегрузка std::cout для IPAddr
	
	struct Socket { int sockfd; bool connected; bool isbind; enum Type { NONE, TCP, UDP, UNIX } _type;
		IPAddr destAddr; bool autodestroy; int errcode; int buffsize; //Object* data;
        Socket() : sockfd(-1), connected(false), isbind(false), _type(TCP), autodestroy(true), errcode(0), buffsize(DEF_BUFF_SIZE){}
		Socket(const std::string& ip, int port, Type type=TCP) : sockfd(-1), connected(false), isbind(false), _type(type), autodestroy(true), errcode(0), buffsize(DEF_BUFF_SIZE){ destAddr.ip=ip; destAddr.port=port; }
		Socket(int sockfd) : sockfd(sockfd), connected(true), isbind(false), _type(TCP), autodestroy(true), errcode(0), buffsize(DEF_BUFF_SIZE){}
        ~Socket(){ if(autodestroy) destroy(); }
		
		#ifdef _WIN32
		static bool WSAinited;
		static void WIN_WSAinit(){ if(!WSAinited){ WSADATA wsaData; if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ print("connect: WSAStartup failed.\n"); return; } WSAinited=true; } }
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
		
		bool connect(const std::string& ip, int port){ if(connected){ return false; } WIN_WSAinit(); errcode=0;
            struct addrinfo hints, *res, *p; memset(&hints, 0, sizeof(hints)); hints.ai_family = AF_UNSPEC; // AF_INET или AF_INET6 для IPv4 или IPv6
            if(_type==TCP){ hints.ai_socktype = SOCK_STREAM; }else{ hints.ai_socktype = SOCK_DGRAM; } // SOCK_STREAM, SOCK_DGRAM
            
			if(getaddrinfo(ip.c_str(), dtos(port).c_str(), &hints, &res) != 0){ print(type()); print(" connect: getaddrinfo failed.\n"); return false; }
            for(p = res; p != NULL; p = p->ai_next){ sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); if(sockfd<0){ continue; }
				int opt=1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)); // allow SO_REUSEADDR
                if(_type==TCP){ if(::connect(sockfd, p->ai_addr, p->ai_addrlen) == 0){ connected = true; }else{ continue; } }
                destAddr = _from_sockaddr_storage(reinterpret_cast<sockaddr_storage*>(p->ai_addr)); connected = true; break; } freeaddrinfo(res);
			if(sockfd<0){ print(type()); print(" Socket create failed: "); print(dtos(GetErr())); print("\n"); return false; }
            if(!connected){ print(type()); print(" Connection failed: "); print(dtos(GetErr())); print("\n"); destroy(sockfd); return false; }
			return true; }
		bool connect(const IPAddr& a){ return connect(a.ip, a.port); };
		bool connect(){ return connect(destAddr.ip, destAddr.port); };
            
        bool connect4(const std::string& ip, int port){ if(connected){ return false; } WIN_WSAinit(); errcode=0;
			struct sockaddr_in addr; memset(&addr, 0, sizeof(addr)); addr.sin_family = AF_INET; addr.sin_port = htons(port);
			if (inet_addr(ip.c_str())!=INADDR_NONE){ addr.sin_addr.s_addr = inet_addr(ip.c_str()); } 
			else{ struct hostent *hst; hst = gethostbyname(ip.c_str()); if(hst){ addr.sin_addr = *(struct in_addr*)hst->h_addr; }
				else{ print(type()); print(" connect4: gethostbyname failed.\n"); return false; } }

			sockfd = socket(AF_INET, SOCK_STREAM, 0); if(sockfd<0){ print(type()); print(" Socket create failed.\n"); return false; }
			if(_type==TCP){ if(::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){ 
				print(type()); print(" Connection failed: "); print(dtos(GetErr())); print("\n"); destroy(sockfd); return false; } }
			connected = true; destAddr = _from_sockaddr_in(&addr); return true; }
		bool connect4(){ return connect4(destAddr.ip, destAddr.port); };
			
		bool bind(int port = 0, const std::string& bindip = "::"){ WIN_WSAinit(); errcode=0;
			struct addrinfo hints, *res, *p; memset(&hints, 0, sizeof(hints)); hints.ai_family = AF_UNSPEC;
			if(_type==TCP){ hints.ai_socktype = SOCK_STREAM; }else{ hints.ai_socktype = SOCK_DGRAM; } hints.ai_flags = AI_PASSIVE;  // Используем для сокетов сервера

			if(getaddrinfo(bindip.c_str(), dtos(port).c_str(), &hints, &res) != 0){ print(type()); print(" bind: getaddrinfo failed.\n"); return false; }
			for(p = res; p != NULL; p = p->ai_next){ sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); if(sockfd<0){ continue; }
			    int opt=0; setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&opt, sizeof(opt)); // allow IPv6 accept IPv4.
			    opt=1; setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)); // allow SO_REUSEADDR
				if(::bind(sockfd, p->ai_addr, p->ai_addrlen) == 0){ isbind = true; break; } }
			freeaddrinfo(res); if(!isbind){ print(type()); print(" bind failed: "); print(dtos(GetErr())); print("\n"); return false; }
			return true; }
			
		bool listen(int maxqueue=0){ if(maxqueue<=0){ maxqueue=SOMAXCONN; } if(::listen(sockfd, maxqueue) == 0){ return true; } return false; }
		
		static BindInfo address(int sockfd1){ BindInfo addrInfo; sockaddr_storage addr; socklen_t addrLen = sizeof(addr); WIN_WSAinit(); char host[NI_MAXHOST]; char port_str[NI_MAXSERV];
			if (getsockname(sockfd1, (sockaddr*)&addr, &addrLen) == -1){ print("Socket.address: getsockname failed.\n"); return addrInfo; }
			if(getnameinfo((sockaddr*)&addr, addrLen, host, NI_MAXHOST, port_str, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) != 0){ 
				print("Socket.address: getnameinfo failed.\n"); return addrInfo; }

			addrInfo.ip = host; addrInfo.port = stodn(std::string(port_str));
			if (addr.ss_family == AF_INET){ addrInfo.family = "IPv4"; }else if(addr.ss_family == AF_INET6){ addrInfo.family = "IPv6"; }else{ addrInfo.family = "(Unknown)"; }

			int sockType; socklen_t optLen = sizeof(sockType);
			if (getsockopt(sockfd1, SOL_SOCKET, SO_TYPE, (char*)&sockType, &optLen) == -1){ print("Socket.address: getsockopt failed.\n"); return addrInfo; }

			if (sockType == SOCK_STREAM){ addrInfo.type = "TCP"; }else if (sockType == SOCK_DGRAM){ addrInfo.type = "UDP"; }else{ addrInfo.type = "(Unspec protocol)"; } return addrInfo; }
		inline BindInfo address(){ return address(sockfd); }
				
		static bool destroy(int sockfd1, bool force=true){
			#ifdef _WIN32
                return ::closesocket(sockfd1)==0; //WSACleanup();
			#else           
				if(!force){ ::shutdown(sockfd1, SHUT_RDWR); } return ::close(sockfd1)==0;
			#endif
		}//sockfd>=0&&(connected||isbind)?
        bool destroy(){ if(connected||isbind){ connected=false; return destroy(sockfd); } return true; }
		inline static bool close(int sockfd1){ return destroy(sockfd1, false); }
        bool close(){ if(connected||isbind){ connected=false; return destroy(sockfd, false); } return true; }
				
		bool recvTimeout(int msecs){
		#ifdef _WIN32
			DWORD timeout = msecs; if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0){ print("setrecvTimeout: setsockopt failed.\n"); return false; }
		#else
			struct timeval tv; tv.tv_sec = 0; tv.tv_usec = msecs*1000;
			if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){ print("setrecvTimeout: setsockopt failed.\n"); return false; }
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
		std::string type(){ if(_type==TCP){ return "TCP"; }else if(_type==UDP){ return "UDP"; }else if(_type==UNIX){ return "UNIX"; }else{ return "(Unspec protocol)"; } }
        
        // lo-lvl address translation
        static IPAddr _from_sockaddr_in(const sockaddr_in* addr4){ IPAddr result; char ipstr[INET_ADDRSTRLEN]; result.port = ntohs(addr4->sin_port); 
			result.ipver = 4; inet_ntop(AF_INET, (void*)&(addr4->sin_addr), ipstr, sizeof(ipstr)); result.ip = ipstr; return result; }
		static IPAddr _from_sockaddr_in6(const sockaddr_in6* addr6){ IPAddr result; char ipstr[INET6_ADDRSTRLEN]; result.port = ntohs(addr6->sin6_port); 
			result.ipver = 6; inet_ntop(AF_INET6, (void*)&(addr6->sin6_addr), ipstr, sizeof(ipstr)); result.ip = ipstr; return result; }
		static IPAddr _from_sockaddr_storage(const sockaddr_storage* addr){
			if (addr->ss_family == AF_INET){ return _from_sockaddr_in(reinterpret_cast<const sockaddr_in*>(addr)); } 
			else if (addr->ss_family == AF_INET6){ return _from_sockaddr_in6(reinterpret_cast<const sockaddr_in6*>(addr)); } 
			else { IPAddr result; result.ipver = '0'; return result; } }
		sockaddr_storage _to_sockaddr_storage(const IPAddr& addr){ sockaddr_storage storage; memset(&storage, 0, sizeof(storage));
			if (addr.ipver == 4){ sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(&storage);
				addr_in->sin_family = AF_INET; addr_in->sin_port = htons(addr.port);
				if(inet_pton(AF_INET, addr.ip.c_str(), (void*)&(addr_in->sin_addr)) <= 0){ print("Invalid IPv4 address format\n"); }
			} else{ sockaddr_in6* addr_in6 = reinterpret_cast<sockaddr_in6*>(&storage);
				addr_in6->sin6_family = AF_INET6; addr_in6->sin6_port = htons(addr.port);
				if(inet_pton(AF_INET6, addr.ip.c_str(), (void*)&(addr_in6->sin6_addr)) <= 0){ print("Invalid IPv6 address format\n"); } }
			return storage; }
        protected: void _move(Socket& other){ if(this==&other) return; destroy(); sockfd=other.sockfd; autodestroy=true; other.autodestroy=false; } }; 
    #ifdef _WIN32
    bool Socket::WSAinited = false;
	#endif
    
	
    struct TCPSocket : Socket {  
		TCPSocket(){}
		TCPSocket(const std::string& ip, int port, bool toconn=false) : Socket(ip, port, Socket::TCP){ if(toconn) connect(); }
		TCPSocket(const IPAddr& addr, bool toconn=false) : Socket(addr.ip, addr.port, Socket::TCP){ if(toconn) connect(); }
		TCPSocket(int sockfd1) : Socket(sockfd1){}
		TCPSocket(Socket sock) : Socket(sock){}
		
		int send(const Buffer& buff){ return ::send(sockfd, (const char*)&buff[0], buff.size(), 0); }
		int send(const char* cptr, int len){ return ::send(sockfd, cptr, len, 0); }
		int send(const char* cstr){ return ::send(sockfd, cstr, std::strlen(cstr), 0); }
		
		int recv(char* ptr, int len){ rsetErr(); int bytesRead = ::recv(sockfd, ptr, len, 0);
			if(bytesRead<=0){ destroy(); if(bytesRead<0) GetErr(); } return bytesRead; }
		int recv(Buffer* buff){ buff->resize(buffsize); rsetErr(); 
			int bytesRead = ::recv(sockfd, (char*)&(*buff)[0], buff->size(), 0);
			if(bytesRead<=0){ buff->resize(0); destroy(); if(bytesRead<0) GetErr(); }
			else{ buff->resize(bytesRead); } return bytesRead; }
		Buffer recv(){ Buffer buff(buffsize); recv(&buff); return buff; }
		//inline Buffer read(){ return recv(); }
		
		TCPSocket* accept() const { sockaddr_storage client_addr; socklen_t addrlen = sizeof(client_addr);
			TCPSocket* clsock = new TCPSocket(::accept(sockfd, (sockaddr*)&client_addr, &addrlen));
			clsock->destAddr = _from_sockaddr_storage(&client_addr); return clsock; }
			
		TCPSocket _accept() const { sockaddr_storage client_addr; socklen_t addrlen = sizeof(client_addr);
			TCPSocket clsock(::accept(sockfd, (sockaddr*)&client_addr, &addrlen));
			clsock.destAddr = _from_sockaddr_storage(&client_addr); return clsock; }
	};
    
	struct UDPSocket : Socket { 
		UDPSocket(){ _type=UDP; }
		UDPSocket(const std::string& ip, int port, bool toconn=false) : Socket(ip, port, Socket::UDP){ if(toconn) connect(); }
		UDPSocket(const IPAddr& addr, bool toconn=false) : Socket(addr.ip, addr.port, Socket::UDP){ if(toconn) connect(); }
		UDPSocket(int sockfd1) : Socket(sockfd1){ _type=UDP; }
		UDPSocket(Socket sock) : Socket(sock){ _type=UDP; }
		
		int send(const IPAddr& addr, const char* cstr, int len){ struct sockaddr_storage dest_addr = _to_sockaddr_storage(addr);
			return ::sendto(sockfd, cstr, len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); }
		int send(const IPAddr& addr, const Buffer& buffer){ struct sockaddr_storage dest_addr = _to_sockaddr_storage(addr);
			return ::sendto(sockfd, (const char*)&buffer[0], buffer.size(), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); }
		inline int send(const IPAddr& addr, const char* cstr){ return send(addr, cstr, std::strlen(cstr)); }
		inline int send(const Buffer& buff){ return send(destAddr, buff); }
		inline int send(const char* cstr, int len){ return send(destAddr, cstr, len); }
		inline int send(const char* cstr){ return send(destAddr, cstr, std::strlen(cstr)); }
	
		int recv(char* ptr, int len, IPAddr* rinfo=NULL){ rsetErr(); int bytesRead;
			if(rinfo!=NULL){ sockaddr_storage dest_addr; socklen_t addrlen = sizeof(dest_addr); 
				bytesRead = ::recvfrom(sockfd, ptr, len, 0, (sockaddr*)&dest_addr, &addrlen); 
				*rinfo=_from_sockaddr_storage(&dest_addr); }
			else{ bytesRead = ::recvfrom(sockfd, ptr, len, 0, NULL, NULL); }
			if(bytesRead<=0){ destroy(); if(bytesRead<0) GetErr(); } return bytesRead; }
		int recv(Buffer* buff, IPAddr* rinfo=NULL){ buff->resize(buffsize); rsetErr(); 
			return recv((char*)&(*buff)[0], buff->size(), rinfo); }
		Buffer recv(){ Buffer buff(buffsize); recv(&buff); return buff; }
		//inline Buffer read(){ return recv(); }
	};
	
	//struct UnixSocket {}
	struct ICMPSocket {
		static double ping4(const std::string& ip){ 
		#ifdef _WIN32
			HANDLE hIcmpFile; char sendData[32] = "Data for ICMP packet"; hIcmpFile = IcmpCreateFile();
			if (hIcmpFile == INVALID_HANDLE_VALUE){ print("ICMPSocket: Create ICMP handle fail: "); print(dtos(GetLastError())); return -1; }
			Buffer rbuff(sizeof(ICMP_ECHO_REPLY)+sizeof(sendData));
			DWORD dwRetVal = IcmpSendEcho(hIcmpFile, inet_addr(ip.c_str()), sendData, sizeof(sendData), NULL, (void*)&rbuff[0], rbuff.size(), 2000);
			if(dwRetVal != 0){ PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)&rbuff[0]; IcmpCloseHandle(hIcmpFile); return pEchoReply->RoundTripTime; }
			else{ print("Ping failed. Error: "); print(dtos(GetLastError())); print("\n"); IcmpCloseHandle(hIcmpFile); return -1; }
		#else           
			int sockfd = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); if(sockfd < 0){ print("ICMPSocket: Creation failed.\n"); return -1; }
		    struct sockaddr_in dest_addr; memset(&dest_addr, 0, sizeof(dest_addr));
			dest_addr.sin_family = AF_INET; inet_pton(AF_INET, ip.c_str(), &dest_addr.sin_addr);

			char packet[64]; memset(packet, 0, sizeof(packet));
			struct icmphdr* icmp_header = reinterpret_cast<struct icmphdr*>(packet);
			icmp_header->type = ICMP_ECHO; icmp_header->code = 0;
			icmp_header->un.echo.id = htons(getpid()); icmp_header->un.echo.sequence = htons(1);
			icmp_header->checksum = 0; icmp_header->checksum = calc_checksum(packet, sizeof(packet));
			struct timeval tv; tv.tv_sec = 2; tv.tv_usec = 0; if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0){ print("setsockopt: SO_RCVTIMEO failed.\n"); }
			
			double ptime = performance::now(); ssize_t bytes_sent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
			if(bytes_sent <= 0){ print("Failed to send packet.\n"); close(sockfd); return -1; }

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

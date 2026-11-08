#ifdef _WIN32
#define poll WSAPoll
#else
#include <sys/epoll.h>
#include <unistd.h>
//#include <poll.h>
//#include <fcntl.h>
#endif
//API: select, poll, epoll/IOCP
//#define USE_NSELECT
//#define USE_NPOLL
//#define USE_IO_URING
//#define NCPP_NET_MULTIPLEXER
//EventLoop API:
// run(), waitOneEvent/waitOnce(), hasEvents(), nextEvent()
// Events: sockets, files, console input, GUI, etc.

namespace ncpp{	// Buffer buf; while ((buf = clientSock.recv()).size() > 0){}
    //use epoll()/poll()/IOCP of Berkeley sockets? (epoll() for linux) https://habr.com/ru/companies/infopulse/articles/415259/
    //IOCP: https://habr.com/ru/companies/infopulse/articles/415403/ 
	//IOCP guide: https://stackoverflow.com/questions/35242143/unable-to-accept-a-new-connection-properly-using-iocp-invalid-socket-handle
	// -- https://www.codeproject.com/Articles/10330/A-simple-IOCP-Server-Client-Class
	//AcceptEx question: https://stackoverflow.com/questions/19956186/iocp-acceptex-not-creating-completion-upon-connect
	//IOCP example: https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/netds/winsock/iocp/serverex/IocpServerex.Cpp 
	//short life-time TCP, epoll: https://habr.com/ru/articles/416669/
	//epoll man: https://man7.org/linux/man-pages/man7/epoll.7.html
	//typedef SPtr<Socket> SocketP;
	//typedef SPtr<TCPSocket> TCPSocketP;
	
	struct AsyncIO { enum FDTYPE {FD_SOCK,FD_FILE}; };
	//template <typename S = TCPSocket> struct SocketPool;
	
	struct SocketPool : AsyncIO { HashSet<Socket*> sockets; int servfd; //HashSet<SPtr<S> >; Array<int> srvfds?
		//void* (SocketPool::*PFN_allocSock)() const; // Указатель на метод-член класса
		//void* (SocketPool::*PFN_freeSock)(void* ptr) const;
		Socket* (*_allocSock)(); void  (*_freeSock)(Socket* s);
		static Socket* _allocFNDef(){ return new TCPSocket(); }
		static void _freeFNDef(Socket* s){ delete &s; s=NULL; }
		void setAllocator(Socket* (*allocFN)()=_allocFNDef, void (*freeFN)(Socket*)=_freeFNDef){ _allocSock=allocFN; _freeSock=freeFN; }
		//void setAllocator(SlabAlloc<Socket> slab);
		
		SocketPool() : servfd(-1), onConnect(NULL), onData(NULL), onError(NULL), onClose(NULL){ setAllocator(); _init(); }
	
	#if defined(_WIN32) && defined(USE_NET_NATIVE) && !defined(USE_NPOLL) && !defined(USE_NSELECT) // === Windows - IOCP ===
	#define NCPP_NET_MULTIPLEXER "IOCP"
	//#define EINTR 10004L
		Array<Socket*> acceptSocks; LPFN_ACCEPTEX PAcceptEx;
		LPFN_ACCEPTEX WIN_GetAcceptEx(int sockfd){ LPFN_ACCEPTEX AcceptExPtr = nullptr; GUID guidAcceptEx = WSAID_ACCEPTEX; DWORD bytes; 
			if(WSAIoctl(sockfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
				&AcceptExPtr, sizeof(AcceptExPtr), &bytes, NULL, NULL) != 0){ print("(!) WIN_GetAcceptEx error.\n"); }
			  if(AcceptExPtr == nullptr){ print("(!) AcceptEx pointer is null.\n"); } return AcceptExPtr; }
		void _init(){ PAcceptEx=NULL; _initIOCP(); }
		~SocketPool(){ CloseHandle(hIOCP); }
		enum IOType { IO_READ, IO_WRITE, IO_ACCEPT, IO_CLOSE, IO_NONE };
		struct IOEvent : OVERLAPPED { Buffer buff; };

		//bool addServSocket(Socket* srvsock){ servfd=srvsock->sockfd; return addSocket(srvsock, true); }
		bool addServSocket(Socket* srvsock){ if(srvsock->sockfd <= 0){ print("(!) addServSocket: invalid socket\n"); return false; }
			servfd = srvsock->sockfd; if(!registerIOCP(srvsock)){ return false; } return initServAccept(srvsock); }
		
		bool addSocket(Socket* socket, bool isserv=false){ if(socket->sockfd <= 0){ print("(!) addSocket: invalid socket\n"); return false; }
			if(!registerIOCP(socket)){ return false; } sockets.insert(socket); return isserv?initServAccept(socket):initSockRecv(socket); }
		void removeSocket(int sockfd){} // IOCP не требует явного удаления сокета
		
		void run(int timeout=-1){ DWORD bytesCnt; ULONG_PTR completionKey; IOEvent* ioctx;
			while(true){
				if(GetQueuedCompletionStatus(hIOCP, &bytesCnt, &completionKey, (OVERLAPPED**)&ioctx, (DWORD)timeout)){
					Socket* socket=(Socket*)completionKey; switch((size_t)ioctx->hEvent){
						case IO_ACCEPT: { print("New connection on sockfd: "); print(dtos(socket->sockfd)); print("\n");
							if(!socket || socket->sockfd <= 0){ break; }
							Socket* client = NULL; if(!acceptSocks.empty()){ client = acceptSocks.pop(); }
							if(client && client->sockfd > 0){
								setsockopt(client->sockfd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&socket->sockfd, sizeof(socket->sockfd));
								registerIOCP(client); sockets.insert(client); client->connected = true; initSockRecv(client); _onConnect(*((TCPSocket*)client));
							} break; }
						case IO_READ: { print("Read event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); ioctx->buff.resize(bytesCnt); 
							if(bytesCnt>0){ print(" New Message: "); print(ioctx->buff.cout()); print("\n"); initSockRecv(socket); break; } }
						case IO_CLOSE: print("Close connection on sockfd: "); print(dtos(socket->sockfd)); print("\n"); removeSocket(socket->sockfd); break;
						case IO_WRITE: print("Write event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); break;
						default: print("Other event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); }
					//std::cout << "-- IOCP event on sockfd: " << socket->sockfd << " (servfd: " << servfd << ", completionKey: "<< completionKey <<")" << std::endl;
					delete ioctx; //processSocket(sock1, ioctx->buff); ioctx->buff.clear();
				}else{ print("GetQueuedCompletionStatus failed: "); print(dtos(WSAGetLastError())); print("\n"); } }
		}
		private: HANDLE hIOCP;
		void _initIOCP(){ hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if(hIOCP==NULL){ print("(!) Failed to create IOCP: "); print(dtos(WSAGetLastError())); print("\n"); exit(EXIT_FAILURE); } }
		bool registerIOCP(Socket* socket){ if(socket->sockfd <= 0){ print("(!) registerIOCP: invalid socket\n"); return false; }
			if(CreateIoCompletionPort((HANDLE)(uintptr_t)socket->sockfd, hIOCP, (ULONG_PTR)socket, 0) == NULL){
				int err = WSAGetLastError(); print("(!) Failed IOCP addSocket: "); print(dtos(err)); print("\n"); return false; } return true; }
		bool initSockRecv(Socket* socket){ WSABUF wsabuf; IOEvent* ioctx = new IOEvent(); ZeroMemory(ioctx, sizeof(IOEvent)); 
			ioctx->hEvent = (HANDLE)IO_READ; ioctx->buff.reserve(DEF_SOCK_SIZE); ioctx->buff.resize(DEF_SOCK_SIZE); wsabuf.buf = (char*)&ioctx->buff[0]; 
			wsabuf.len = DEF_SOCK_SIZE; DWORD flags = 0; int result = WSARecv(socket->sockfd, &wsabuf, 1, NULL, &flags, ioctx, NULL);
			
			int err=WSAGetLastError(); if(result==SOCKET_ERROR && err!=WSA_IO_PENDING && err!=ERROR_IO_PENDING){ delete ioctx;
				print("Failed addSocket -> WSARecv failed: "); print(dtos(err)); print("\n"); return false; } return true; }
		bool initServAccept(Socket* socket){  int acceptconn = 0; int optlen = sizeof(acceptconn);
			if(getsockopt(socket->sockfd, SOL_SOCKET, SO_ACCEPTCONN, (char*)&acceptconn, &optlen) != 0 || acceptconn == 0){
				print("(!) initServAccept: socket is not in listening state (SO_ACCEPTCONN == 0)\n"); return false; }
			sockaddr_storage addr; socklen_t addrLen = sizeof(addr); getsockname(socket->sockfd, (sockaddr*)&addr, &addrLen);
			
			int newsock = Socket::create(addr.ss_family==AF_INET6?6:4); IOEvent* ioctx = new IOEvent(); ZeroMemory(ioctx, sizeof(IOEvent));
			ioctx->hEvent = (HANDLE)IO_ACCEPT; ioctx->buff.resize((sizeof(sockaddr_storage)+16)*2); PAcceptEx = WIN_GetAcceptEx(servfd); 
			registerIOCP(new TCPSocket(newsock)); bool result = PAcceptEx(socket->sockfd, newsock, &ioctx->buff[0], 0, sizeof(sockaddr_storage)+16, sizeof(sockaddr_storage)+16, NULL, ioctx); 

			int err=WSAGetLastError(); if(!result && err!=ERROR_IO_PENDING){ delete ioctx; print("Failed initServAccept -> AcceptEx failed: "); 
				print(dtos(err)); print(" (result = "); print(dtos(result)); print(")\n"); return false; } return true; }
	
	#elif !defined(_WIN32) && defined(USE_NET_NATIVE) && !defined(USE_NPOLL) && !defined(USE_NSELECT) // === Linux - epoll ===
	#define NCPP_NET_MULTIPLEXER "epoll"
		void _init(){
		#ifdef EPOLL_CLOEXEC
			epfd = epoll_create1(EPOLL_CLOEXEC); //epfd = epoll_create1(0);
		#else
			epfd = epoll_create(1); fcntl(epfd, F_SETFD, FD_CLOEXEC);
		#endif
			if(epfd == -1){ print("Failed to create epoll file descriptor\n"); exit(EXIT_FAILURE); } }
		~SocketPool(){ close(epfd); } //sockets.clear_ptr();
		
		bool addServSocket(Socket* srvsock){ servfd=srvsock->sockfd; bool ok = addSocket(srvsock); sockets.erase(srvsock); return ok; }
		bool addSocket(Socket* socket){ struct epoll_event event; event.events = EPOLLIN | EPOLLERR | EPOLLHUP; // | EPOLLOUT | EPOLLET
			socket->setNonBlocking(); event.data.ptr = socket; sockets.insert(socket); //event.data.fd = sockfd;
			if(epoll_ctl(epfd, EPOLL_CTL_ADD, socket->sockfd, &event) == -1){
				print("Failed to add socket to epoll: "); print(strerror(errno)); print("\n"); return false; } return true; }
		void removeSocket(int sockfd){ if(epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL) == -1){ 
			print("Failed to remove socket from epoll: "); print(strerror(errno)); print("\n"); } }
		//bool addSocket(int sockfd){ Socket* s = _allocSock(); s->sockfd=sockfd; return addSocket(s); }
		//inline void removeSocket(Socket* socket){ removeSocket(socket->sockfd); }
		
		void run(int timeout=-1){ Array<struct epoll_event> events(10);
			while(true){ int nfds = epoll_wait(epfd, events.data(), events.size(), timeout);
				if (nfds == -1){ print("epoll_wait failed: "); print(strerror(errno)); print("\n"); continue; }
				for(int i = 0; i < nfds; ++i){ TCPSocket& socket=*(TCPSocket*)events[i].data.ptr;
					if(events[i].events & EPOLLIN){ if(servfd==socket.sockfd){ TCPSocket* clsock = socket.accept(_allocSock);
							if(clsock->sockfd<0){  print("(!) Accept error: "); print(dtos(errno)); print("\n"); continue; }
							addSocket((Socket*)clsock); _onConnect(*clsock); continue; }
						else{ //std::cout << "Read event on fd: " << socket.sockfd << " (servfd: " << servfd <<")" << std::endl;
							//Buffer data(socket.buffsize); while(socket.recv(&data)>0){ _onData(socket, data); }
							//std::cout << "]] END Read on fd: " << socket.sockfd << " (" << socket.errcode << ") || " << socket.connected << std::endl;
							Buffer data = socket.recv(); if(data.size()>0){ _onData(socket, data); }
						} }
					if(socket.errcode!=0||events[i].events & EPOLLERR){ if(onError!=NULL) onError(socket); }
					if((!socket.connected&&socket.sockfd!=servfd)||events[i].events & EPOLLHUP){ _onClose(socket);  removeSocket((Socket*)&socket); continue; }
					//if(events[i].events & EPOLLOUT){ std::cout << "Write event on sockfd: " << socket.sockfd << std::endl; }
				} //std::cout << "== POLL while УДАЧА (" << nfds << ") " << sockets << std::endl; 
			} }
		private: int epfd;
	#elif defined(USE_NPOLL) //|| !defined(_WIN32) || (defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x0600) //Linux || >=Windows Vista
	#define NCPP_NET_MULTIPLEXER "poll" // === poll ===
		void _init(){  }
		#ifdef _WIN32
		typedef WSAPOLLFD PollFD;
		#else
		typedef struct pollfd PollFD; //enum { PollRead=POLLIN, PollWrite=POLLOUT, PollError=POLLERR };
		#endif

		int _pollHandle(int timeout = -1){ Array<PollFD> pfds; Array<Socket*> psocks; //TCPSocket servsock(servfd); servsock.own(false);
			PollFD spfd; spfd.fd = servfd; spfd.events = POLLIN; spfd.revents = 0; pfds.push(spfd); psocks.push(NULL);
			
			for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end();){ if(!*it){ ++it; continue; } Socket& s = **it;
				if(s.sockfd == -1){ _freeSock(&s); it = sockets.erase(it); continue; }
				PollFD pfd; pfd.fd = s.sockfd; pfd.events = POLLIN; pfd.revents = 0; pfds.push(pfd); psocks.push(&s); ++it; }
			
			int activity = ::poll(pfds.data(), pfds.size(), timeout);
			if(activity<0){ int err1 = Socket::GetLastErr(); print("(!) poll() error: "); print(err1); print("\n"); if(err1 == EINTR) return 0; return -1; }
			if(activity == 0) return 0;
			
			Buffer buff; buff.reserve(DEF_SOCK_SIZE);
			if(pfds[0].revents & POLLIN){ TCPSocket& socket = *TCPSocket::accept(servfd, _allocSock);
				if(socket.sockfd < 0){ print("(!) poll: Accept error: "); print(dtos(socket.GetErr())); print("\n"); }
				else{ sockets.insert((Socket*)&socket); _onConnect(socket); } }
			
			for(size_t i=1; i < pfds.size(); ++i){ if(!(pfds[i].revents & (POLLIN | POLLERR | POLLHUP))) continue;
				Socket* sk = psocks[i]; if(!sk) continue; TCPSocket& socket = *(TCPSocket*)sk;
				
				if(pfds[i].revents & (POLLERR | POLLHUP)){ socket.GetErr(); if(onError != NULL) onError(socket);
					_onClose(socket); socket.destroy(); _freeSock(&socket); sockets.erase(&socket); continue; }
				
				if(pfds[i].revents & POLLIN){
					buff.resize(DEF_SOCK_SIZE); int bytesRead = ::recv(socket.sockfd, (char*)buff.data(), DEF_SOCK_SIZE, 0);
					if(bytesRead <= 0){ if(bytesRead < 0){ socket.GetErr(); if(onError != NULL) onError(socket); } _onClose(socket); 
						socket.destroy(); _freeSock(&socket); sockets.erase(&socket); continue; }else{ buff.resize(bytesRead); _onData(socket, buff); }
				}
			}
			return activity;
		}

		void run(){ while(true){ _pollHandle(); } }
		//void waitOneEvent(int timeout=-1){ _pollHandle(timeout); }
		//bool hasEvents(){ if(_pollHandle(0) > 0){ return true; } return false; }
		//bool nextEvent(){ if(_pollHandle(0) > 0){ return true; } return false; }
	
	#else // === select ===
	#define NCPP_NET_MULTIPLEXER "select" // addServSocket((Socket*)this); sockets.erase((Socket*)this);
		void _init(){}
		void run(int timeout=0){ Buffer buff; buff.reserve(DEF_SOCK_SIZE); //lsof -p <PID> - descryptors list. std::cout << "DEBUG: Select Run." << std::endl;
			struct timeval tv; if(timeout>=0){ tv.tv_sec = timeout/1000; tv.tv_usec = (timeout%1000)*1000; }
			while(true){ fd_set read_fds; FD_ZERO(&read_fds); FD_SET(servfd, &read_fds); int max_sd = servfd;
				for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end();){ if(!*it) continue; Socket& s = **it;
					if(s.sockfd==-1){ _freeSock(&s); it = sockets.erase(it); continue; }
					FD_SET(s.sockfd, &read_fds); if(s.sockfd > max_sd) max_sd = s.sockfd; ++it; }
				int activity = ::select(max_sd+1, &read_fds, NULL, NULL, timeout>0?&tv:NULL);
				if(activity<0){ int err1=Socket::GetLastErr(); print("(!) SelectPool: select error: "); print(err1); print("\n"); if(err1==EINTR) continue; return; }
				if(FD_ISSET(servfd, &read_fds)){ TCPSocket& socket = *TCPSocket::accept(servfd, _allocSock);
					if(socket.sockfd<0){ print("(!) select(): Accept error: "); print(dtos(socket.GetErr())); print("\n"); break; }
					sockets.insert((Socket*)&socket); _onConnect(socket); }
				for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end();){ Socket& socket = **it; if(!FD_ISSET(socket.sockfd, &read_fds)){ ++it; continue; }
					buff.resize(DEF_SOCK_SIZE); int bytesRead = ::recv(socket.sockfd, (char*)buff.data(), DEF_SOCK_SIZE, 0); //std::cout << "select recv: " << bytesRead << " bytes." << std::endl;
					if(bytesRead<=0){ if(bytesRead<0){ socket.GetErr(); if(onError!=NULL) onError((TCPSocket&)socket); } _onClose((TCPSocket&)socket); 
						socket.destroy(); _freeSock(&socket); it = sockets.erase(it); continue; } buff.resize(bytesRead); _onData((TCPSocket&)socket, buff); ++it; }
				//print("DEBUG: Select HashSet<Socket*> size: "); print(sockets.size()); print("\n");
			} }
		
		bool _waitSelectWriteOne(int timeout=-1){
			while(true){ fd_set write_fds; FD_ZERO(&write_fds); int max_sd = -1; if(sockets.empty()&&servfd == -1) return false;
				for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end(); ++it){ Socket* s = *it;
					if(!s || s->sockfd==-1) continue; FD_SET(s->sockfd, &write_fds); if(s->sockfd > max_sd) max_sd = s->sockfd; }
				if(max_sd == -1) return false; struct timeval tv; if(timeout>=0){ tv.tv_sec = timeout/1000; tv.tv_usec = (timeout%1000)*1000; }
				int activity = ::select(max_sd + 1, NULL, &write_fds, NULL, timeout>0?&tv:NULL);
				if(activity<0){ int err1=Socket::GetLastErr(); if(err1==EINTR) continue; return false; } return activity > 0; } }
		//err == EWOULDBLOCK || err == EAGAIN -> "Места нет, подожди"; err == EINTR -> "Прервано сигналом, попробуй еще раз немедленно"

	#endif //NCPP_NET_MULTIPLEXER
		public: 
		void (*onConnect)(TCPSocket& socket);
		void (*onData)(TCPSocket& socket, const Buffer& data);
		//UDP: onDatagram/onRData/onRInfo/onDgram?
		//Write Callback: onWriteReady/onReady?
		void (*onError)(TCPSocket& socket);
		void (*onClose)(TCPSocket& socket);
		private: virtual void _onData(TCPSocket& socket, const Buffer& data){ if(onData!=NULL) onData(socket, data); }
			virtual void _onConnect(TCPSocket& socket){ if(onConnect!=NULL) onConnect(socket); }
			virtual void _onClose(TCPSocket& socket){ if(onClose!=NULL) onClose(socket); }
	};
	
	struct TCPServer : SocketPool { TCPSocket sock; //threads
		TCPServer(){}
		TCPServer(int port, const CString& bindip="::"){ bind(port, bindip); 
			#ifndef _WIN32
			signal(SIGPIPE, SIG_IGN);
			#endif
		}
		~TCPServer(){ if(sock.autodestroy) close(); }
		bool bind(int port, const CString& bindip="::"){
			if(!sock.bind(port, bindip)){ print("(!) TCPServer: bind failed on port: "+dtos(port)+"\n"); return false; }
			if(!sock.listen()){ print("(!) TCPServer: listen failed on port: "+dtos(port)+"\n"); return false; } servfd=sock.sockfd; return true; }
		BindInfo address(){ return Socket::address(sock.sockfd); }
		void close(){ sock.destroy(); }
		
		//TODO: (TCPSocket*)(Socket*)*it -> (TCPSocket*)*it
		void broadcast(const Buffer& buff){ for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end();++it){ ((TCPSocket*)(Socket*)*it)->send(buff); } }
		void run(){ 
		#ifdef USE_NET_NATIVE
			addServSocket((Socket*)this);
		#endif
			SocketPool::run(); }
		
		#if __cplusplus >= 201103L //move for C++11
		TCPServer(TCPServer&& tmp) noexcept { move(*this, tmp); }
		TCPServer& operator=(TCPServer&& tmp) noexcept { if(this!=&tmp) move(*this, tmp); return *this; }
		TCPServer(const TCPServer&) = delete; TCPServer& operator=(const TCPServer&) = delete; //Запрет копирования.
		TCPServer& steal(TCPServer& tmp){ move(*this, tmp); return *this; }
		TCPServer& steal(TCPServer&& tmp){ move(*this, tmp); return *this; }
		friend void move(TCPServer& dst, TCPServer&& tmp){ move(dst, (TCPServer&)tmp); }
		#else //move for C++98
		private: TCPServer(const TCPServer&); TCPServer& operator=(const TCPServer&); public: //Скрытие копирования.
		TCPServer& steal(const TCPServer& victim){ move(*this, (TCPServer&)victim); return *this; }
		friend void move(TCPServer& dst, const TCPServer& victim){ move(dst, (TCPServer&)victim); }
		//TCPServer& operator=(TCPServer other){ move(*this, other); return *this; } //Old UB rvalue copy imitator
		#endif
		friend void move(TCPServer& dst, TCPServer& tmp){ if(&dst==&tmp) return; move(dst.sock, tmp.sock); }
	};
	
	struct ReqCache { http::Req req; bool headwait; Buffer bf, wsbf; unsigned int bodylen; bool ws; ReqCache() : headwait(true), bodylen(0), ws(false){} };
	struct HTTPServer : TCPServer { HashMap<Socket*, ReqCache> rcache; 
		unsigned int max_headerlen; unsigned int max_bodylen; unsigned int max_sockets; bool wsmode;
		HTTPServer() : TCPServer(), onRequest(NULL){ _initSets(); }
		HTTPServer(int port, const CString& bindip="::") : TCPServer(port, bindip), onRequest(NULL){ _initSets(); }
		
		void (*onRequest)(http::Req& req, http::Res& res);
		void (*onUpgrade)(TCPSocket& socket, http::Req& req, Buffer& data);
		protected: void _initSets(){ if(http::ErrCodes.size()==0){ http::_initErrCodes(); }
			max_headerlen=8*1024; max_bodylen=4*1048576; max_sockets=1000; wsmode=false; }
		//void _onConnect(TCPSocket& socket){ if(onConnect!=NULL) onConnect(socket); }
		void _onData(TCPSocket& socket, const Buffer& data){ if(onData!=NULL) onData(socket, data); if(onRequest==NULL&&!wsmode) return;
			http::Req& req=rcache[&socket].req; Buffer& rbuff=rcache[&socket].bf; rbuff+=data;
			if(rcache[&socket].headwait){
				if(rbuff.size()>max_headerlen){ http::Res res_err(socket); res_err.SendErr(431); rcache.erase(&socket); 
					print("(#DEBUG) 431 Headers Too Large error responsed.\n"); return; }
				if(rbuff.indexOf("\r\n\r\n")==NPOS){ return; } req=http::ReqParse(rbuff); rbuff.clear(); //res.hver=req.hver;
				if(rcache[&socket].req.bad){ http::Res res_err(socket); res_err.SendErr(400); rcache.erase(&socket); 
					print("(#DEBUG) 400 Bad Req responsed.\n"); return; } rcache[&socket].headwait = false; }
			if(wsmode&&req.headers["connection"]=="Upgrade"&&req.headers["upgrade"]=="websocket") return; //WS redirect
			if(max_bodylen>0&&req.headers.has("content-length")){ if(rcache[&socket].bodylen==0){ rcache[&socket].bodylen=stoin(req.headers["content-length"]); }
			    //print("(#DEBUG) HTTPServ body part: body+rbuff size: "); print(req.body.size()+rbuff.size()); print(" | "); print(rcache[&socket].bodylen); print("\n"); 
				if(req.body.size()+rbuff.size()>max_bodylen){ http::Res res_err(socket); res_err.SendErr(413); rcache.erase(&socket); 
					print("(#DEBUG) 413 Payload Too Large error responsed.\n"); return; }
				if(req.body.size()+rbuff.size()<rcache[&socket].bodylen){ return; } req.body+=rbuff; rbuff.clear(); }
			http::Res res(socket); if(req.headers.has("connection")){ res.headers["connection"]=req.headers["connection"]; } 
			if(onRequest!=NULL) onRequest(req, res); rcache.erase(&socket); }
		void _onClose(TCPSocket& socket){ if(onClose!=NULL) onClose(socket); rcache.erase(&socket); }
	};
}
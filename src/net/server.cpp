#ifdef _WIN32
#else
#include <sys/epoll.h>
#include <unistd.h>
//#include <fcntl.h>
#endif
namespace ncpp{	// Buffer buf; while ((buf = clientSock.recv()).size() > 0){}
    //use epoll()/poll()/IOCP of Berkeley sockets? (epoll() for linux) https://habr.com/ru/companies/infopulse/articles/415259/
    //IOCP: https://habr.com/ru/companies/infopulse/articles/415403/ 
	//IOCP guide: https://stackoverflow.com/questions/35242143/unable-to-accept-a-new-connection-properly-using-iocp-invalid-socket-handle
	// -- https://www.codeproject.com/Articles/10330/A-simple-IOCP-Server-Client-Class
	//AcceptEx question: https://stackoverflow.com/questions/19956186/iocp-acceptex-not-creating-completion-upon-connect
	//IOCP example: https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/netds/winsock/iocp/serverex/IocpServerex.Cpp 
	//short life-time TCP, epoll: https://habr.com/ru/articles/416669/
	//epoll man: https://man7.org/linux/man-pages/man7/epoll.7.html
	//typedef std::shared_ptr<Socket> SocketP;
	//typedef std::shared_ptr<TCPSocket> TCPSocketP;
	
	struct AsyncIO { enum {FD_SOCK,FD_FILE} _type; };
	struct SocketPool : AsyncIO { HashSet<Socket*> sockets; int servfd; 
	#ifdef _WIN32
		std::vector<Socket*> acceptSocks; LPFN_ACCEPTEX PAcceptEx;
		LPFN_ACCEPTEX WIN_GetAcceptEx(int sockfd){ LPFN_ACCEPTEX AcceptExPtr = nullptr; GUID guidAcceptEx = WSAID_ACCEPTEX; DWORD bytes; 
			if(WSAIoctl(sockfd, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx),
				&AcceptExPtr, sizeof(AcceptExPtr), &bytes, NULL, NULL) != 0){ print("(!) WIN_GetAcceptEx error.\n"); }
			  if(AcceptExPtr == nullptr){ print("(!) AcceptEx pointer is null.\n"); } return AcceptExPtr; }
	    SocketPool() : servfd(-1), PAcceptEx(NULL), onConnect(NULL), onData(NULL), onError(NULL), onClose(NULL){ _type=FD_SOCK; } //_initIOCP();
		~SocketPool(){ CloseHandle(hIOCP); }
		enum IOType { IO_READ, IO_WRITE, IO_ACCEPT, IO_CLOSE, IO_NONE };
		struct IOEvent : OVERLAPPED { Buffer buff; };

		bool addServSocket(Socket* srvsock){ servfd=srvsock->sockfd; return addSocket(srvsock, true); }
		bool addSocket(Socket* socket, bool isserv=false){ if(!registerIOCP(socket)){ delete socket; return false; }; 
			sockets.insert(socket); return isserv?initServAccept(socket):initSockRecv(socket); }
		void removeSocket(int sockfd){} // IOCP не требует явного удаления сокета
		
		void waitAndProcess(int timeout=-1){ DWORD bytesCnt; ULONG_PTR completionKey; IOEvent* ioctx;
			while(true){
				if(GetQueuedCompletionStatus(hIOCP, &bytesCnt, &completionKey, (OVERLAPPED**)&ioctx, (DWORD)timeout)){
					Socket* socket=(Socket*)completionKey; switch((size_t)ioctx->hEvent){
						case IO_ACCEPT: print("New connection on sockfd: "); print(dtos(socket->sockfd)); print("\n"); break;
						case IO_READ: { print("Read event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); ioctx->buff.resize(bytesCnt); 
							if(bytesCnt>0){ print(" New Message: "); print(ioctx->buff.cout()); print("\n"); initSockRecv(socket); break; } }
						case IO_CLOSE: print("Close connection on sockfd: "); print(dtos(socket->sockfd)); print("\n"); removeSocket(socket); break;
						case IO_WRITE: print("Write event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); break;
						default: print("Other event on sockfd: "); print(dtos(socket->sockfd)); print("\n"); }
					//std::cout << "-- IOCP event on sockfd: " << socket->sockfd << " (servfd: " << servfd << ", completionKey: "<< completionKey <<")" << std::endl;
					delete ioctx; //processSocket(sock1, ioctx->buff); ioctx->buff.clear();
				}else{ print("GetQueuedCompletionStatus failed: "); print(dtos(WSAGetLastError())); print("\n"); } }
		}
		private: HANDLE hIOCP;
		void _initIOCP(){ hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if(hIOCP==NULL){ print("Failed to create IOCP: "); print(dtos(WSAGetLastError())); print("\n"); exit(EXIT_FAILURE); }
			print("Failed to create IOCP\n"); }
		bool registerIOCP(Socket* socket){ if(CreateIoCompletionPort((HANDLE)(size_t)socket->sockfd, hIOCP, (ULONG_PTR)socket, 0) == NULL){
				print("Failed IOCP addSocket: "); print(dtos(WSAGetLastError())); print("\n"); return false; }
			//std::cerr << "IOCP new addSocket: " << socket->sockfd <<  std::endl; 
			return true; }
		bool initSockRecv(Socket* socket){ WSABUF wsabuf; IOEvent* ioctx = new IOEvent(); ZeroMemory(ioctx, sizeof(IOEvent)); 
			ioctx->hEvent = (HANDLE)IO_READ; ioctx->buff.reserve(DEF_BUFF_SIZE); ioctx->buff.resize(DEF_BUFF_SIZE); wsabuf.buf = (char*)&ioctx->buff[0]; 
			wsabuf.len = DEF_BUFF_SIZE; DWORD flags = 0; int result = WSARecv(socket->sockfd, &wsabuf, 1, NULL, &flags, ioctx, NULL);
			
			int err=WSAGetLastError(); if(result==SOCKET_ERROR && err!=WSA_IO_PENDING && err!=ERROR_IO_PENDING){ delete ioctx;
				print("Failed addSocket -> WSARecv failed: "); print(dtos(err)); print("\n"); return false; } return true; }
		bool initServAccept(Socket* socket){ int newsock=Socket::create(); IOEvent* ioctx = new IOEvent(); ZeroMemory(ioctx, sizeof(IOEvent));
			ioctx->hEvent = (HANDLE)IO_ACCEPT; ioctx->buff.resize((sizeof(sockaddr_storage)+16)*2); PAcceptEx = WIN_GetAcceptEx(servfd); 
			registerIOCP(new TCPSocket(newsock)); bool result = PAcceptEx(socket->sockfd, newsock, &ioctx->buff[0], 0, sizeof(sockaddr_storage)+16, sizeof(sockaddr_storage)+16, NULL, ioctx); 
			//else{ result2=true; std::cerr << "Waiting sock..." << std::endl; Socket* clsock=(Socket*)((TCPSocket*)socket)->accept();
			//	addSocket(clsock); std::cout << "Connected (newsock: "<<clsock->sockfd<<"): destPort: " << clsock->destAddr.port << std::endl; }
			//std::cout << "(#) newsock " << newsock << "; sockfd: " << socket->sockfd << std::endl;
			
			//std::cout << sockets << "; " << socket->sockfd << "; " << newsock << std::endl;
			//std::cout << "PAcceptEx(" << socket->sockfd << ", " << newsock << ", " << (void*)&ioctx->buff[0] << ", " << 0 << ", " << sizeof(sockaddr_storage)+16 << ", " << sizeof(sockaddr_storage)+16 << ", NULL, " << ioctx << std::endl;
			int err=WSAGetLastError(); if(!result && err!=ERROR_IO_PENDING){ delete ioctx; print("Failed initServAccept -> AcceptEx failed: "); 
				print(dtos(err)); print(" (result = "); print(dtos(result)); print(")\n"); return false; } return true; }
	#else // Linux code
		SocketPool() : servfd(-1), onConnect(NULL), onData(NULL), onError(NULL), onClose(NULL){ _type=FD_SOCK; 
		#ifdef EPOLL_CLOEXEC
			epfd = epoll_create1(EPOLL_CLOEXEC); //epfd = epoll_create1(0);
		#else
			epfd = epoll_create(1); fcntl(epfd, F_SETFD, FD_CLOEXEC);
		#endif
			if(epfd == -1){ print("Failed to create epoll file descriptor\n"); exit(EXIT_FAILURE); } }
		~SocketPool(){ close(epfd); sockets.clear_ptr(); }
		
		inline bool addServSocket(Socket* srvsock){ servfd=srvsock->sockfd; return addSocket(srvsock); }
		bool addSocket(Socket* socket){ struct epoll_event event; event.events = EPOLLIN | EPOLLERR | EPOLLHUP; // | EPOLLOUT | EPOLLET
			socket->setNonBlocking(); event.data.ptr = socket; sockets.insert(socket); //event.data.fd = sockfd;
			if(epoll_ctl(epfd, EPOLL_CTL_ADD, socket->sockfd, &event) == -1){
				print("Failed to add socket to epoll: "); print(strerror(errno)); print("\n"); return false; } return true; }
		void removeSocket(int sockfd){ if(epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL) == -1){ 
			print("Failed to remove socket from epoll: "); print(strerror(errno)); print("\n"); } }
		
		void waitAndProcess(int timeout=-1){ std::vector<struct epoll_event> events(10);
			while(true){ int nfds = epoll_wait(epfd, events.data(), events.size(), timeout);
				if (nfds == -1){ print("epoll_wait failed: "); print(strerror(errno)); print("\n"); continue; }
				for(int i = 0; i < nfds; ++i){ TCPSocket& socket=*(TCPSocket*)events[i].data.ptr;
					if(events[i].events & EPOLLIN){ if(servfd==socket.sockfd){ TCPSocket* clsock = socket.accept();
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
	#endif
		public: bool addSocket(int sockfd){ return addSocket(new TCPSocket(sockfd)); }
		inline void removeSocket(Socket* socket){ removeSocket(socket->sockfd); sockets.erase_ptr(socket); }
		void _runSelect(int timeout=-1){ std::vector<Socket> sockets; Buffer buff; buff.reserve(DEF_BUFF_SIZE); //lsof -p <PID> - descryptors list. std::cout << "DEBUG: Select Run." << std::endl;
			TCPSocket servsock(servfd); struct timeval tv; tv.tv_sec = timeout; tv.tv_usec = 0;
			while(true){ fd_set read_fds; FD_ZERO(&read_fds); FD_SET(servfd, &read_fds); int max_sd = servfd;
				for (size_t i=0;i<sockets.size();i++){ FD_SET(sockets[i].sockfd, &read_fds); if(sockets[i].sockfd > max_sd) max_sd = sockets[i].sockfd; }
				int activity = ::select(max_sd+1, &read_fds, NULL, NULL, timeout>0?&tv:NULL);
				if (activity<0&&Socket::GetLastErr()!=EINTR){ print("SelectPool: select error.\n"); }
				if (FD_ISSET(servfd, &read_fds)){ TCPSocket socket = servsock._accept().own(false); if(socket.sockfd<0){ print("select: (!) Accept error: "); print(dtos(socket.GetErr())); print("\n"); break; }
					sockets.push_back((Socket)socket.own(false)); _onConnect(socket); }
				for(std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end();){ Socket& socket = *it; if(!FD_ISSET(socket.sockfd, &read_fds)){ ++it; continue; }
					buff.resize(DEF_BUFF_SIZE); int bytesRead = ::recv(socket.sockfd, (char*)&buff[0], DEF_BUFF_SIZE, 0); //std::cout << "select recv: " << bytesRead << " bytes." << std::endl;
					if(bytesRead<=0){ if(bytesRead<0){ socket.GetErr(); if(onError!=NULL) onError((TCPSocket&)socket); } _onClose((TCPSocket&)socket); 
						socket.destroy(); it = sockets.erase(it); continue; } buff.resize(bytesRead); _onData((TCPSocket&)socket, buff); ++it; } 
				//std::cout << "DEBUG: Select std::vector<Socket> size: " << sockets.size() << std::endl; 
			} }
		void (*onConnect)(TCPSocket& socket);
		void (*onData)(TCPSocket& socket, const Buffer& data);
		void (*onError)(TCPSocket& socket);
		void (*onClose)(TCPSocket& socket);
		private: virtual void _onData(TCPSocket& socket, const Buffer& data){ if(onData!=NULL) onData(socket, data); }
			virtual void _onConnect(TCPSocket& socket){ if(onConnect!=NULL) onConnect(socket); }
			virtual void _onClose(TCPSocket& socket){ if(onClose!=NULL) onClose(socket); }
	};
	
	struct TCPServer : SocketPool, TCPSocket { //threads
		TCPServer(){}
		TCPServer(int port, const std::string& bindip="::"){ init(port, bindip); }
		#if __cplusplus >= 201103L
		//TCPServer(TCPServer&& other){ _move(other); }
		TCPServer& operator=(TCPServer&& other){ _move(other); return *this; }
		#else
		//TCPServer(TCPServer& other){ _move(other); } //"передача" права на уничтожение дескриптора.
		TCPServer& operator=(TCPServer other){ _move(other); return *this; }
		//TCPServer(const TCPServer& other){ _move((Socket&)other); } // UB: non-const violation
		//TCPServer& operator=(const TCPServer& other){ _move((Socket&)other); return *this; } // UB: non-const violation
		#endif
		~TCPServer(){ if(autodestroy) close(); }
		
		void broadcast(const Buffer& buff){ for(HashSet<Socket*>::iterator it = sockets.begin(); it != sockets.end();++it){ ((TCPSocket*)*it)->send(buff); } }
		void run(){ servfd=sockfd; _runSelect();
			//addServSocket((Socket*)this); sockets.erase((Socket*)this); waitAndProcess(); 
		}
		void close(){}
		//bool _bind(){ return Socket::bind(); }
		//void bind(int port, const std::string& bindip="::"){ init(port, bindip); }
		
		void init(int port, const std::string& bindip="::"){
			if(!bind(port, bindip)){ throw Err("[Error] TCPServer: bind failed on port: "+dtos(port)); }
			if(!listen()){ throw Err("[Error] TCPServer: listen failed on port: "+dtos(port)); } }
	};
	
	struct ReqCache { bool bodywait; Buffer rbuff; };
	struct HTTPServer : TCPServer { HashMap<Socket*, Buffer> rcache; unsigned int max_headersize; unsigned int max_bodysize; unsigned int max_sockets;
		HTTPServer() : TCPServer(), onRequest(NULL){ _initSets(); }
		HTTPServer(int port, const std::string& bindip="::") : TCPServer(port, bindip), onRequest(NULL){ _initSets(); }
		
		void (*onRequest)(http::Req req, http::Res res);
		private: void _initSets(){ if(http::ErrCodes.size()==0){ http::_initErrCodes(); }
			max_headersize=8*1024; max_bodysize=4*1048576; max_sockets=1000; }
		void _onConnect(TCPSocket& socket){ if(onConnect!=NULL) onConnect(socket); }
		void _onData(TCPSocket& socket, const Buffer& data){ if(onData!=NULL) onData(socket, data); if(onRequest==NULL) return; rcache[&socket]+=data;
			if(rcache[&socket].size()>max_headersize){ http::Res res_err(socket); res_err.SendErr(431); return; }
			long hEnd = rcache[&socket].indexOf("\r\n\r\n"); if(hEnd<=0){ return; }
			http::Req req=http::ReqParse(rcache[&socket]); http::Res res(socket); 
			if(req.headers.has("connection")){ res.headers["connection"]=req.headers["connection"]; }
			onRequest(req, res); rcache[&socket].clear(); }
		void _onClose(TCPSocket& socket){ if(onClose!=NULL) onClose(socket); rcache.erase(&socket); }
	};
}
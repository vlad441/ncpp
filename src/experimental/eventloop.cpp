#ifdef _WIN32
#else
#include <sys/epoll.h>
#include <unistd.h>
//#include <fcntl.h>
#endif
//API: select, poll, epoll/IOCP
//#define USE_NSELECT
//#define USE_NPOLL
//#define USE_IO_URING
//#define NCPP_NET_MULTIPLEXOR
//EventLoop API:
// run(), waitOneEvent(), hasEvents(), nextEvent()
// Events: sockets, files, console input, GUI, etc.

namespace ncpp{	// Buffer buf; while ((buf = clientSock.recv()).size() > 0){}
	//Events:
	//1) sockets, files, console input, GUI, etc.
	//2) files read and write one part to all
	//3) Non blocking read console input?
	//4) GUI events?
	//5) Timers?
	
	//Node.js Event Loop:
	// - timers
	// - pending callbacks
	// - idle, prepare 
	// - poll (I/O) <───── incoming: connections, data, etc.
	// - check
	// - close callbacks
	// - timers
}
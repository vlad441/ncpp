//#include "malloc-debug.h" 
#include "../src/ncpp.cpp"
using namespace ncpp;

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

HTTPServer srv(3001); volatile unsigned long long rcnt=0; int port = 3001;
void ShowReqCnt(){ if(rcnt<=0) return; print("Req/sec: "); print(dtos(rcnt, '\''));
	print(" | Sockets: "); print(srv.sockets.size()); print("\n"); rcnt=0; }

void onReq(http::Req& req, http::Res& res){ ++rcnt; res.end("true"); } //print("New req: "); print(req.url); print("\n");
//void onReq(http::Req& req, http::Res& res){ ++rcnt; res.end(fs::readFile("http-test.cpp")); } //print("New req: "); print(req.url); print("\n");
//void onReq(http::Req& req, http::Res& res){ ++rcnt; res.end("true"); print(req.cout()); print("\n"); } //print("New req: "); print(req.url); print("\n");

/*void onReq(http::Req& req, http::Res& res){ ++rcnt; 
	if(req.url=="/_stats"){ String ss; ss << "HTTPServ rcache.size(): " << srv.rcache.size() << "<br>"; 
		ss << "HTTPServ HashSet sockets.size(): " << srv.sockets.size() << "\n"; res.end(ss); return; }
	res.end("true"); print("\n"); }*/

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();
	srv.onRequest=onReq; setInterval(ShowReqCnt, 1000); print("HTTP server runned on port: "); print(port); print("\n"); srv.run(); }

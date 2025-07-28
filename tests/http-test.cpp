#include "../src/ncpp.cpp"
using namespace ncpp;

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

unsigned long long rcnt=0;
void ShowReqCnt(){ if(rcnt<=0) return; print("Req/sec: "); print(rcnt); print("\n"); rcnt=0; }

void onReq(http::Req req, http::Res res){ ++rcnt; res.end("true"); } //print("New req: "); print(req.url); print("\n");

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version();
	HTTPServer srv(3001); srv.onRequest=onReq; setInterval(ShowReqCnt, 1000); srv.run(); }

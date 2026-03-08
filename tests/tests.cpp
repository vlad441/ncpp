#ifdef NCPP_LIB_USE
#include "../include/ncpp.h"
#else
#include "../src/ncpp.cpp"
#endif
//include "malloc-debug.h"

using namespace ncpp;
int tested=0, passed=0, warnings=0, errors=0; char color[2];
int warns=0, errs=0; String currtest, tresp;
Array<String> errsarr; Array<String> warnarr;
Console cons; //performance::start(); long long time1=performance::now();

//http::SaveStream("http://static.wikia.nocookie.net/test1666/images/c/c9/T8vlSlTOp3o.jpg/revision/latest?cb=20171017123525&path-prefix=ru", "./saved_stream");

#include "tests-header.h"

Console& rsetCons(){ cons.reset(); color[0]=0; return cons; }
void clog(const CString& str, bool endl=false){ cons << str << "\n"; }
void subm(){ if(currtest.empty()){ color[0]=0; performance::start(); return; } tested++; tresp="OK"; color[1]=2;
	if(errs>0){ tresp="--ERR--"; errsarr.push(currtest); color[1]=1; }
	else if(warns>0){ tresp="(WARN)"; warnarr.push(currtest); color[1]=3; passed++; }else{ passed++; }
	if(color[0]!=color[1]){ color[0]=color[1]; cons.setColor(color[0]); }
	warnings+=warns; warns=0; errors+=errs; errs=0; 
	cons << " " << currtest << " ... " << tresp << "\n"; }
void NextTest(const CString& str){ subm(); currtest=str; }

void TIME_DIFF(const CString& descr, long long oldtime){ cons.setColor(6) << descr; 
	cons.setColor(3) << dtos((GetTimestamp('u')-oldtime)/1000.0, 3); cons.setColor(5) << " msec\n"; cons.reset(); }

template <typename T1, typename T2> void TEST_EQ(const T1& v1, const T2& v2){ if(v1!=v2) errs++; }
template <typename T1, typename T2> void TEST_NE(const T1& v1, const T2& v2){ if(v1==v2) errs++; }

#include "units/module-base.cpp" //Base_module_test();
void Sys_module_test();
//#include "units/module-structs.cpp" //Structs_module_test();
#include "units/module-crypto.cpp" //Crypto_module_test();

int main(int argc, char* argv[]){ cpp_version(); gcc_version(); ncpp_version(); arch_current(); //setInterval(_dbg_ShowAllocs, 1000);
	// === BEGIN TEST ===
	Base_module_test();
	//Structs_module_test();
	Crypto_module_test();
	// === END TEST ===
	subm(); double time = performance::now();
	
	cons.setColor(3) << "\n==== Tests Complete ==== "; cons.reset() << "(Time: "; cons.setColor(3) << dtos(time, 3); cons.reset() << " msec)\n";
	cons.reset() << " Tested|(Passed/Fail): "; cons.setColor(3) << tested; cons.reset() << " | ("; cons.setColor(2) << passed;
	cons.reset() << "/"; cons.setColor(1) << (tested-passed);
	cons.reset() << "); Errors|Warnings: "; cons.setColor(1) << errors; cons.reset() << " | "; cons.setColor(3) << warnings; cons.reset() << "\n";
	if(errors>0){ cons << " Errors pull: "; cons.setColor(1) << errsarr; cons.reset() << "\n"; return 1; }
	else if(warnings>0){ cons << " Warnings pull: "; cons.setColor(3) << warnarr; cons.reset() << "\n"; return 2; } return 0; }

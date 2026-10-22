#define USE_EXPERIMENTAL
#define NOUSE_AUDIO
//#define NCPP_IMAGE_LNKLIBS
#include "../src/ncpp.cpp"
using namespace ncpp;
Console cons;
#include "tests-header.h"

int main(int argc, char* argv[]){ print(NCPP_COMPILE_INFO); _ncpp_check_ver(); os_version();
	const Image& img = Image::fromFile("../index"); print("img: "); print(img.w); print("; "); print(img.h); print("\n");
	//img.savePNG("../index2.png"); print("PNG saved.\n");
	char q[] = {10, 30, 50,90,95,100}; for(char i=0;i<(char)(sizeof(q)/sizeof(q[0]));++i) img.saveJPG("../index2-"+String(dtos(q[i]))+".jpg", q[i]);
	print("JPG saved.\n");
}

// g++ -s -O2 image_test.cpp -o image_test /usr/lib/x86_64-linux-gnu/libpng16.so.16 /usr/lib/x86_64-linux-gnu/libjpeg.so.62
// g++ -s -O2 image_test.cpp -o image_test -l:libpng16.so.16 -l:libjpeg.so.62
// g++ -s -O2 image_test.cpp -o image_test -lpng -ljpeg
// /usr/bin/ld: cannot find -lpng: No such file or directory
// /usr/bin/ld: cannot find -ljpeg: No such file or directory

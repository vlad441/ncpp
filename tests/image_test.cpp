#define USE_EXPERIMENTAL
//#define NCPP_IMAGE_LNKLIB
#include "../src/ncpp.cpp"
using namespace ncpp;
Console cons;
#include "tests-header.h"

int main(int argc, char* argv[]){ print(NCPP_COMPILE_INFO); _ncpp_check_ver(); os_version();
	const Image& img = Image::fromFile("../index"); print("img: "); print(img.w); print("; "); print(img.h); print("\n");
	img.savePNG("../index2.png"); print("PNG saved.\n");
	//img.saveJPG("../index2.jpg", 30); print("JPG saved.\n");
}

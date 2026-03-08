#include "../src/ncpp.cpp"
#include "../src/experimental/media/audio.cpp"
using namespace ncpp;
using namespace ncpp::Media;
#include "tests-header.h"

Console cons;

//#define FNAME "Again - fragm1"
//#define FNAME "OST Classroom of the Elite Soundtrack(ver2)"

void mp3_frames(){ Buffer mp3 = fs::readFile(FNAME ".mp3"); MP3Frame header;
    size_t pos=MP3Frame::skipID3v2(mp3.data()), frames=0, frameBytes=0, TotalPCMSize=0; //unsigned char buffer[4];
    while(pos<mp3.size()){ if(!header.parse(&mp3[pos])){ cons << "(!) mp3 frame read fail\n"; continue; }
		cons << "Frame: " << frames << " | " << header.bitrate << " Kbps, " << header.freq << " Hz, "<< header.ch << " ch";
		int frameSz = header.getFrameSize(); TotalPCMSize+=header.getPCMSize();
		cons << " | -- ver: " << header.version << ", Layer: " << header.layer << ", frameSize: " << header.getFrameSize() << "\n";
		pos+=frameSz; frameBytes+=frameSz; frames++; }
	cons << " == Frame Bytes Read: " << dtos(frameBytes, '\'') << "\n";
	cons << " == PCM Size: " << dtos(TotalPCMSize, '\'') << "\n";
}

int main(){
	//mp3_frames();
	PlayFile(FNAME ".mp3");
}
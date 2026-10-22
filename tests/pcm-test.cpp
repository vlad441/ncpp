#include "../src/ncpp.cpp"
using namespace ncpp;
using namespace ncpp::Media;
#include "tests-header.h"

Console cons; Audio ply;

//#define PATH ""

//#define FNAME "Again - fragm1"
//#define FNAME "OST Classroom of the Elite Soundtrack(ver2)"
//#define FNAME "OST Re:ZERO -Starting Life in Another World- Ending 1 | Styx Helix"

const char* file = PATH "Re ZERO - Memento - s2 - Ed 1.mp3";

void mp3_frames(const CString& fname, bool exlogs=1){ Buffer mp3 = fs::readFile(fname); MP3Frame header;
    size_t pos=MP3Frame::skipID3v2(mp3.data()), frames=0, frameBytes=0, TotalPCMSize=0; //unsigned char buffer[4];
    while(pos<mp3.size()){ if(!header.parse(&mp3[pos])){ cons << "(!) mp3 frame read fail\n"; continue; }
		if(exlogs) cons << "Frame: " << frames << " | " << header.bitrate << " Kbps, " << header.freq << " Hz, "<< header.ch << " ch";
		int frameSz = header.getFrameSize(); TotalPCMSize+=header.getPCMSize();
		if(exlogs) cons << " | -- ver: " << header.version << ", Layer: " << header.layer << ", frameSize: " << header.getFrameSize() << "\n";
		pos+=frameSz; frameBytes+=frameSz; frames++; }
	cons << " == Frame Bytes Read: " << dtos(frameBytes, '\'') << "\n";
	cons << " == PCM Size: " << dtos(TotalPCMSize, '\'') << "\n";
}

int main(){ mp3_frames(file, 0); 
    // --- Case 1 ---
	//ply.playFile(file);
	// --- Case 2 ---
	cons << "freq/vol: "; Array<String> arr = readline().split(" "); int freq = stoin(arr[0]); int volume=arr.size()>1?stoin(arr[1]):100;
	ply.reset(freq); ply.setLoop(); cons << "\n"; Buffer mp3 = fs::readFile(file); ply.pushMP3(mp3); ply.setVolume(volume);
	// --- Case 3 ---
	//Buffer pcm, mp3 = fs::readFile(file); MP3Frame frame; MP3Decode(pcm, mp3, frame); PlayPCM(pcm.data(), pcm.size());
	ply.run();
}
#define MINIMP3_IMPLEMENTATION
#define MINIMP3_ONLY_MP3
#include "minimp3.h"

namespace ncpp { namespace Media {
//BitReader rd(buffer + offset)?

struct MP3Frame { int bitrate; int freq; //unsigned char* fptr;
	unsigned char version; // 0=2.5, 1=reserved, 2=II, 3=I
    unsigned char layer;   // 1=L3, 2=L2, 3=L1
	bool padding; unsigned char ch; 
	#ifdef MINIMP3_H
	mp3dec_t _dec;
	#endif
	
	MP3Frame(){ //if(fptr!=NULL) skipID3v2();
		#ifdef MINIMP3_H
		mp3dec_init(&_dec);
		#endif
	}
	
	static unsigned int skipID3v2(const void* buffer){ const unsigned char* b = (const unsigned char*)buffer;
		if(b[0]=='I' && b[1]=='D' && b[2]=='3'){
			unsigned int size = (b[6] & 0x7F) << 21 | (b[7] & 0x7F) << 14 | (b[8] & 0x7F) << 7 | (b[9] & 0x7F); return size+10; }else{ return 0; } }
	//unsigned int skipID3v2(){ fptr+=skipID3v2(fptr); }

    bool parse(const void* buffer){ const unsigned char* buff = (const unsigned char*)buffer; unsigned int h = (buff[0] << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3];
		bool sync = (h >> 21) == 0x7FF; if(!sync) return false; // Проверка Sync Word
		version = (h >> 19) & 3; layer = (h >> 17) & 3;
		unsigned char btrIdx = (h >> 12) & 0xF; unsigned char freqIdx = (h >> 10) & 3; padding = (h >> 9) & 1;
		unsigned char mode = (h >> 6) & 3; ch = (mode==3)?1:2; // 3 = Mono, остальное — стерео варианты
		
		static const int frTable[4][4] = { // Таблица частот дискретизации [Версия][Индекс]
			{ 11025, 12000,  8000, 0 },   // MPEG-2.5
			{ 0,     0,      0,    0 },   // Reserved
			{ 22050, 24000, 16000, 0 },   // MPEG-2
			{ 44100, 48000, 32000, 0 } }; // MPEG-1
		static const int brTable[4][16] = { // Таблица битрейтов для Layer III [Версия][Индекс]
			{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}, // MPEG-2.5 (Layer III)
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Reserved
			{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0}, // MPEG-2 (Layer III)
			{0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0} }; // MPEG-1 (Layer III)
        bitrate = brTable[version][btrIdx]; freq = frTable[version][freqIdx]; return (bitrate>0 && freq>0); }
		
	//bool nextFrame(){ if(!parse(fptr)) return false; fptr+=getFrameSize(); return true; }
	
	int getFrameSize() const { int coeff = (version==3)?144:72; return (coeff * bitrate * 1000) / freq + (padding ? 1 : 0); }
	int getPCMSize() const { int samplesPerFrame = (version == 3) ? 1152 : 576; return samplesPerFrame*ch*2; }
	
	static size_t calcSize(const void* ptr, size_t len, int& mp3Size, int& frames, char type=0){ MP3Frame header; 
		size_t pos=MP3Frame::skipID3v2(ptr), PCMSize=0; mp3Size=0; frames=0;
		while(pos<len){ if(!header.parse((const char*)ptr+pos)){ break; } int frameSz = header.getFrameSize();
			pos+=frameSz; mp3Size+=frameSz; PCMSize+=header.getPCMSize(); frames++; } return PCMSize; }
	static size_t calcSize(const void* ptr, size_t len, char type=0){ int mp3Size=0, frames=0; return calcSize(ptr, len, mp3Size, frames, type); }
	
	bool decode(void* pcm, const void* fptr);
};

#ifdef MINIMP3_H //void pcm[1152*2*2]
bool MP3Frame::decode(void* pcm, const void* fptr){ if(!parse(fptr)) return false; mp3dec_frame_info_t info; //static float pow43[8192]; for(int i=0; i<8192; i++) pow43[i] = pow((float)i, 4.0f/3.0f); } // Для всех возможных значений Хаффмана
	int samples = mp3dec_decode_frame(&_dec, (const unsigned char*)fptr, getFrameSize(), (short*)pcm, &info); if(samples>0) return true; else return false; }
#endif

void MP3Decode(Buffer& pcm, const Buffer& mp3, MP3Frame& frame){ size_t pos=frame.skipID3v2(mp3.data()), decpos=0; 
	pcm.resize(frame.calcSize(mp3.data(), mp3.size()));
	while(pos<mp3.size()){ if(!frame.parse(&mp3[pos])) break; frame.decode(&pcm[decpos], &mp3[pos]); decpos+=frame.getPCMSize(); pos+=frame.getFrameSize(); } }
Buffer MP3Decode(const Buffer& mp3){ Buffer pcm; MP3Frame frame; MP3Decode(pcm, mp3, frame); return pcm; }

} }
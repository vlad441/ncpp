#ifdef _WIN32
#include <mmsystem.h> // -lwinmm
#else
#include <alsa/asoundlib.h> // -lasound //PulseAudio / PipeWire ?
#endif
//Linux packages: libasound2-dev
//MP3: https://github.com/lieff/minimp3/blob/master/minimp3.h
//FLAC: libFLAC?
// - https://github.com/mackron/dr_libs/blob/master/dr_flac.h?
// - https://github.com/jprjr/miniflac/blob/main/miniflac.h?
//Vorbis: libvorbis?
// - https://github.com/nothings/stb/blob/master/stb_vorbis.c?
// - https://github.com/edubart/minivorbis/blob/main/minivorbis.h?
//Opus?

namespace ncpp { namespace Media {
	
void PlayPCM(void* samples, int len, int freq=44100, char bits=16, char ch=2){ int smpSz = bits/8;
#ifdef _WIN32
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, (WORD)ch, (DWORD)freq, (DWORD)(freq*ch*smpSz), (WORD)(ch*smpSz), (WORD)bits, 0};
    HWAVEOUT hWaveOut; if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) return;
    WAVEHDR header = { (LPSTR)samples, (DWORD)len, 0, 0, 0, 0, 0, 0 };
	//header.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP | WHDR_PREPARED; //header.dwLoops = -1; // Бесконечно (0xFFFFFFFF)
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR)); waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));

    while(!(header.dwFlags & WHDR_DONE)) Sleep(50); // Ждем окончания. Sleep(10) достаточно, чтобы не грузить CPU
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR)); waveOutClose(hWaveOut);
#else // Linux (ALSA)
    snd_pcm_t *handle; if(snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) return; snd_pcm_format_t format;
	switch(bits){ case 8: format = SND_PCM_FORMAT_U8; break; case 24: format = SND_PCM_FORMAT_S24_LE; break;
		case 32: format = SND_PCM_FORMAT_S32_LE; break; default: format = SND_PCM_FORMAT_S16_LE; break; }
    snd_pcm_set_params(handle, format, SND_PCM_ACCESS_RW_INTERLEAVED, (unsigned int)ch, (unsigned int)freq, 1, 50000);
    snd_pcm_writei(handle, samples, (snd_pcm_uframes_t)len/(ch*smpSz));
	
	//while(loop_active){ snd_pcm_writei(handle, samples, frames); }
    snd_pcm_drain(handle); snd_pcm_close(handle);
#endif
}
void PlayPCM(const Buffer& pcm, int freq=44100, char bits=16, char ch=2){ PlayPCM(pcm.data(), pcm.size(), freq, bits, ch); }

bool skipWAV(void*& start, int& len){ Buffer buff; buff.stack(start, len);
	if(buff.startsWith("RIFF")&&buff.slice(8,12)=="WAVE"){ start=(char*)start+44; len-=44; return true; }else{ return false; } }
	
} }
	
#include "mp3.cpp"

namespace ncpp { namespace Media {
	
struct Audio { int freq; char bits, ch; volatile char volume; bool loop;
#ifdef _WIN32
    HWAVEOUT hWaveOut; WAVEFORMATEX wfx; Array<WAVEHDR*> wavePtrs;
	WAVEHDR* _allocWaveHeader(void* data, size_t len){
        WAVEHDR* h = (WAVEHDR*)malloc(sizeof(WAVEHDR)+len); ZeroMemory(h, sizeof(WAVEHDR));
        char* pData = (char*)h+sizeof(WAVEHDR); memcpy(pData, data, len); h->lpData = pData; h->dwBufferLength = len; 
		if(loop){ h->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP; h->dwLoops = 0xFFFFFFFF; } return h; }
	void _freeWaveBuff(WAVEHDR* h){ waveOutUnprepareHeader(hWaveOut, h, sizeof(WAVEHDR)); free(h); }
	
    void erase_headers(){
        for(size_t i=0; i<wavePtrs.size();){ WAVEHDR* h = wavePtrs[i]; 
			if(h->dwFlags & WHDR_DONE){ _freeWaveBuff(h); wavePtrs.erase(i, 1); }else{ ++i; } } }
	
	void _init(){ int smpSz = bits/8; //wfx = { WAVE_FORMAT_PCM, (WORD)ch, (DWORD)freq, (DWORD)(freq*ch*smpSz), (WORD)(ch*smpSz), (WORD)bits, 0 };
		wfx.wFormatTag = WAVE_FORMAT_PCM; wfx.nChannels = (WORD)ch; wfx.nSamplesPerSec = (DWORD)freq; wfx.nAvgBytesPerSec = (DWORD)(freq*ch*smpSz); 
		wfx.nBlockAlign = (WORD)(ch*smpSz); wfx.wBitsPerSample = (WORD)bits; wfx.cbSize = 0;
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL); }
	void _clear(){ waveOutReset(hWaveOut);
        for(size_t i=0; i<wavePtrs.size(); ++i){ WAVEHDR* h = wavePtrs[i]; _freeWaveBuff(h); } wavePtrs.clear(); waveOutClose(hWaveOut); hWaveOut=NULL; }
#else
    snd_pcm_t *handle; Buffer samples; volatile bool _tostop; 
	void _init(int msLen=50){
        if(snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)==0){ snd_pcm_format_t format;
            switch(bits){ case 8: format = SND_PCM_FORMAT_U8; break; case 24: format = SND_PCM_FORMAT_S24_LE; break;
                case 32: format = SND_PCM_FORMAT_S32_LE; break; default: format = SND_PCM_FORMAT_S16_LE; break; }
            snd_pcm_set_params(handle, format, SND_PCM_ACCESS_RW_INTERLEAVED, (unsigned int)ch, (unsigned int)freq, 1, msLen*1000);
        }else{ handle = NULL; } }
	void _clear(){ if(handle){ snd_pcm_drain(handle); snd_pcm_close(handle); handle = NULL; } }
#endif
	//Audio() : freq(freq1), bits(bits1), ch(ch1){ _init(); }
    Audio(int freq1=44100, char bits1=16, char ch1=2) : freq(freq1), bits(bits1), ch(ch1){ volume=100; _init(); }
    ~Audio(){ _clear(); }

    void pushPCM(void* data, size_t len){ if(!data || len <= 0) return;
	#ifdef _WIN32
        erase_headers(); WAVEHDR* header = _allocWaveHeader(data, len);
		waveOutPrepareHeader(hWaveOut, header, sizeof(WAVEHDR)); waveOutWrite(hWaveOut, header, sizeof(WAVEHDR)); wavePtrs.push(header);
	#else
        samples.push(data, len);
	#endif
    }
	void pushPCM(const Buffer& pcm){ pushPCM((void*)pcm.data(), pcm.size()); }
	
	#ifdef _WIN32
	void stop(){ waveOutReset(hWaveOut); erase_headers(); }
	void wait(){ while(!wavePtrs.empty()){ erase_headers(); Sleep(20); } }
	void run(){ wait(); }
	void playPCM(void* data, size_t len){ stop(); pushPCM(data, len); }
	void playPCM(const Buffer& pcm){ stop(); pushPCM(pcm.data(), pcm.size()); }
	
	void setVolume(char proc){ DWORD vol = (DWORD)((proc/100.0) * 0xFFFF);
		DWORD dualChannelVolume = (vol << 16) | vol; waveOutSetVolume(hWaveOut, dualChannelVolume); }
	#else
	void stop(){ if(handle){ snd_pcm_drop(handle); snd_pcm_prepare(handle); _tostop=true; } }
	void wait(){ if(handle) snd_pcm_drain(handle); }
	void run(){ _playStream(); }
	void playPCM(void* data, size_t len){ stop(); samples.clear(); pushPCM(data, len); }
	void playPCM(const Buffer& pcm){ stop(); samples.clear(); pushPCM((void*)pcm.data(), pcm.size()); }
	
	private:
	int _pushRawPCM(void* data, size_t len){ if(!handle) return -1; int smpSz = bits/8; int frameSz = ch*smpSz;
		snd_pcm_uframes_t frames = len/frameSz; snd_pcm_sframes_t written = snd_pcm_writei(handle, data, frames);
        if(written == -EPIPE){ snd_pcm_prepare(handle); written = snd_pcm_writei(handle, data, frames); } // as Underrun
		else if(written < 0){ snd_pcm_recover(handle, written, 0); } return written>=0?written*frameSz:written; }
	
	void _playStream(){ if(!handle) return; _tostop=false; size_t pos = 0; int smpSz = bits/8; int frameSz = ch*smpSz;
        snd_pcm_uframes_t buffer_size=0, period_size=0; snd_pcm_get_params(handle, &buffer_size, &period_size);
        size_t chunkSz = period_size*frameSz; if(chunkSz==0) chunkSz = 4096;

        while(pos<samples.size()&&!_tostop){
            size_t remaining = samples.size()-pos;
            int bytesToSend = (remaining < chunkSz)?remaining:chunkSz; //if(bits==16){ _applyVolume16(samples.data()+pos, bytesToSend, volume); }
			int wrbytes=_pushRawPCM(samples.data()+pos, bytesToSend); if(wrbytes>0) pos+=wrbytes; }
    }
	
	static void _applyVolume16(void* data, size_t len, unsigned char volume){ if(volume==100) return; float fVol=volume/100.0f; //if(fVol==1.0f) return;
		int16_t* samples_ptr = (int16_t*)data; size_t conut = len/sizeof(int16_t); //union { unsigned char b[2]; int16_t s; } val; //Strict Aliasing?
		for(size_t i=0; i<conut; ++i){ float scaled = samples_ptr[i]*fVol;
			if(scaled > 32767.0f){ scaled = 32767.0f; } else if(scaled < -32768.0f){ scaled = -32768.0f; } samples_ptr[i] = (int16_t)scaled; }
	}
	
	public:
	
	void setVolume(char proc){ volume=proc; }
	#endif

	//void playSync(){ pushPCM((void*)pcm.data(), pcm.size()); }
	void reset(int freq1=44100, char bits1=16, char ch1=2){ freq=freq1; bits=bits1; ch=ch1; _clear(); _init(); }
	void setLoop(bool loop1=true){ loop=loop1; }
	
	void pushMP3(const Buffer& mp3){ Buffer pcm; MP3Frame frame; MP3Decode(pcm, mp3, frame); pushPCM(pcm); }
	void playMP3(const Buffer& mp3){ Buffer pcm; MP3Frame frame; MP3Decode(pcm, mp3, frame); 
		if(freq!=frame.freq||ch!=frame.ch){ freq=frame.freq; ch=frame.ch; _clear(); _init(); } pushPCM(pcm); }
	
	bool playFile(const CString& fname){ Buffer pcm;
		if(fname.endsWith(".flac")) return false; 
		else if(fname.endsWith(".mp3")){ Buffer mp3 = fs::readFile(fname); MP3Frame frame; MP3Decode(pcm, mp3, frame); freq=frame.freq; ch=frame.ch; }
		else if(fname.endsWith(".ogg")) return false; 
		else{ pcm = fs::readFile(fname); } _clear(); _init(); pushPCM(pcm); return true; }
};

} }

namespace ncpp { namespace Media {
}}

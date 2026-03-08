#ifdef _WIN32
#include <mmsystem.h> // -lwinmm
#else
#include <alsa/asoundlib.h> // -lasound //PulseAudio / PipeWire ?
#endif
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
	
struct Audio { int freq; char bits, ch; Buffer samples; 
	Audio(int freq1=44100, char bits1=16, char ch1=2) : freq(freq1), bits(bits1), ch(ch1){} 
	void play(){ PlayPCM(samples, freq, bits, ch); } void wait(); };

/*void PushSamples(void* data, int len){
#ifdef _WIN32
    // На Windows нужно жонглировать минимум двумя WAVEHDR.
    // Если просто вызвать waveOutWrite еще раз с тем же заголовком, пока он играет — получишь ошибку.
    WAVEHDR* h = new WAVEHDR{ (LPSTR)data, (DWORD)len, 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, h, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, h, sizeof(WAVEHDR));
    // Тут нужна логика очистки: когда h->dwFlags & WHDR_DONE, сделать Unprepare и delete h.
#else
    // На Linux всё просто:
    snd_pcm_sframes_t frames = snd_pcm_writei(handle, data, len / (ch * smpSz));
    if(frames == -EPIPE) snd_pcm_prepare(handle); // Лечим Underrun
#endif
}*/

} }

#include "mp3.cpp"

namespace ncpp { namespace Media { 
	bool PlayFile(const CString& fname){ Buffer pcm; int freq=44100; char bits=16, ch=2; 
		if(fname.endsWith(".flac")) return false; 
		else if(fname.endsWith(".mp3")){ Buffer mp3 = fs::readFile(fname); MP3Frame frame; MP3Decode(pcm, mp3, frame); freq=frame.freq; ch=frame.ch; }
		else if(fname.endsWith(".ogg")) return false; 
		else{ pcm = fs::readFile(fname); } PlayPCM(pcm, freq, bits, ch); return true; }
}}
namespace ncpp{

/*struct EventEmitter { 
	void on(std::string event, void (*callback)());
    void (*OnData)(Buffer data);
};*/
	
struct Stream { bool destroyed;
	Stream() : destroyed(false), _pipestream(NULL){} ~Stream(){ _onDestroy(); }
	Buffer read(long size=-1){ Buffer rbuff; if(size>0){ rbuff=_buff.slice(0, size); _buff=_buff.slice(size); }else{ rbuff=_buff; _buff.clear(); } _onRead(rbuff); return rbuff; }
	void write(const Buffer& wrbuff){ _onWrite(wrbuff); if(_pipestream==NULL){ _buff+=wrbuff; } }
	size_t _sizebuff(){ return _buff.size(); }
	void pipe(Stream* stream2){ _pipestream=stream2; }
	void destroy(){ _onDestroy(); }
	void close(){ _onDestroy(); }
	protected: Buffer _buff; Stream* _pipestream; void (*OnData)(Buffer data);
		void _onWrite(const Buffer& wrbuff){  }
		void _onRead(const Buffer& rbuff){ if(_pipestream!=NULL){ _pipestream->write(wrbuff); } }
		void _onDestroy(){}
};

namespace fs{
bool _writeFile(std::string path, const Buffer& data, std::ios_base::openmode mode=std::ios::binary){ std::ofstream f(path.c_str(), mode); 
	if(!f.is_open()){ std::cerr << "writeFile: Open file error." << std::endl; return false; }
	if(data.size()<=0){ f.close(); return true; } f.write((const char*)&data[0], data.length); f.close(); return true; }
bool writeFile(std::string path, const Buffer& data){ return _writeFile(path,data); }
bool appendFile(std::string path, const Buffer& data){ return _writeFile(path,data,std::ios::app|std::ios::binary); }

Buffer readFile(std::string path){ std::ifstream f(path.c_str(), std::ios::binary);
	if(!f.is_open()){ std::cerr << "readFile: Open file error." << std::endl; return Buffer(); }
    f.seekg(0, std::ios::end); std::streamsize fileSize = f.tellg(); f.seekg(0, std::ios::beg); Buffer data(fileSize);
	if(fileSize<=0){ return data; } if(f.read((char*)&data[0], fileSize)){ return data; }
	else{ std::cerr << "readFile: Read file error." << std::endl; } f.close(); return Buffer(); }

struct FileStream : Stream { int _fd; long fileSize; //f.seekg(0, std::ios::end); fileSize = _file.tellg(); _file.seekg(0, std::ios::beg);
#ifdef _WIN32

#else
	void open(const std::string& path){ int flags=0; _fd = ::open(path.c_str(), flags);
        if (_fd == -1){ throw std::runtime_error("FileStream: Open file error"); }
        fileSize = lseek(_fd, 0, SEEK_END); lseek(_fd, 0, SEEK_SET); }
    void close(const std::string& path){ if(_fd != -1){ ::close(_fd); } _fd=-1; }
    
    ssize_t read(void* buffer, size_t size){ return ::read(_fd, buffer, size); }

    ssize_t write(const void* buffer, size_t size){ return ::write(_fd, buffer, size); }
    
    Buffer read(long size=-1){ Buffer rbuff(1); if(size<=0){ size= } rbuff.resize(size); _file.read((char*)&rbuff[0], size); }
		 else{ rbuff.resize(fileSize-_file.tellg()); _file.read((char*)&rbuff[0], fileSize-_file.tellg()); } filePos=_file.tellg(); _onRead(rbuff); return rbuff; }
    void write(const Buffer& wrbuff){ if(wrbuff.size()<=0){ return; } if(_file.is_open()){ _file.write((const char*)&wrbuff[0], wrbuff.size()); } }
    
#endif
    FileStream(const std::string& path, bool iswrite=false)
		: fpath(path), fmode(mode), _file(path.c_str(), mode){ _createStream(); _file.seekg(0, std::ios::end);  }
	FileStream(int _fd)	: _fd(fd){}
    ~FileStream(){ _onDestroy(); }
    template<typename T>
    static void _swap(T& a, T& b){ T tmp(a); a = b; b = tmp; }
    size_t tellg(const std::string& path){ return filePos; }
    //static FileStream createReadStream(const std::string& path){ return FileStream(path, std::ios::in | std::ios::binary); }
    //static FileStream createWriteStream(const std::string& path){ return FileStream(path, std::ios::out | std::ios::binary); }
    
    Buffer read(long size=-1){ Buffer rbuff(1); if(size>0){ rbuff.resize(size); _file.read((char*)&rbuff[0], size); }
		 else{ rbuff.resize(fileSize-_file.tellg()); _file.read((char*)&rbuff[0], fileSize-_file.tellg()); } filePos=_file.tellg(); _onRead(rbuff); return rbuff; }
    void write(const Buffer& wrbuff){ if(wrbuff.size()<=0){ return; } if(_file.is_open()){ _file.write((const char*)&wrbuff[0], wrbuff.size()); } }

protected:
    //void _onWrite(const Buffer& wrbuff){ if(_file.is_open()){ _file.write((const char*)&wrbuff[0], wrbuff.size()); } }
    //void _onRead(const Buffer& rbuff){ if(_file.is_open()){ _file.read(reinterpret_cast<char*>(rbuff.data()), rbuff.size()); } }
    
    void _onDestroy(){ close(); }
};
	
FileStream createReadStream(std::string path){ return FileStream(path, std::ios::in | std::ios::binary); }
FileStream createWriteStream(std::string path){ return FileStream(path, std::ios::out | std::ios::binary); }
} }

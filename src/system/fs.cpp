#ifndef _WIN32
#include <sys/stat.h> // posix stat()
#include <dirent.h> //opendir/readdir/closedir
#include <pwd.h>
#include <grp.h>
#include <stdio.h> //for ::rename() (Его нет в unistd.h? WTF?!)
#endif

#define DEF_SIZE 16384
namespace ncpp{

/*struct EventEmitter { 
	void on(String event, void (*callback)());
	void once(String event, void (*callback)());
	void emit(String event);
    void (*OnData)(Buffer data);
};*/
	
struct Stream { bool destroyed;
	Stream() : destroyed(false), _pipestream(NULL){} ~Stream(){ _onDestroy(); }
	size_t read(Buffer* rbuff, size_t count=0){ _onRead(*rbuff); return 0; }
	Buffer read(size_t count=0){ Buffer rbuff; read(&rbuff, count); return rbuff; }
	void write(const Buffer& wrbuff){ _onWrite(wrbuff); }
	void pipe(Stream& stream2){ _pipestream=&stream2; }
	void pipe(Stream* stream2){ _pipestream=stream2; }
	void pipestart(){ if(_pipestream==NULL){ return; } while(read().size()>0){} }
	void destroy(){ _onDestroy(); }
	void close(){ _onDestroy(); }
	Stream& operator<<(const Buffer& data){ if(!destroyed){ this->write(data); } return *this; }
	void (*OnData)(const Buffer& data);
	protected: Stream* _pipestream; //Buffer _buff;
		//TODO: Реализация pipe()
		void _onWrite(const Buffer& wrbuff){ if(OnData!=NULL){OnData(wrbuff);} 
			if(_pipestream!=NULL){ _pipestream->write(wrbuff); } }
		void _onRead(const Buffer& rbuff){ _onWrite(rbuff); }
		void _onDestroy(){}
};

struct FStream : Stream { enum { IO_READ, IO_WRITE, IO_APPEND }; bool autodestroy; //FStream ≈ std::fstream
    FStream() : autodestroy(true){}
	FStream(const CString& fpath, char mode) : autodestroy(true){ open(fpath, mode); }
	FStream(const FStream& other) : autodestroy(true){ _fd=other._fd; }
	~FStream(){ if(_fd>0&&autodestroy) close(); }
	FStream& own(bool en=true){ autodestroy=en; return *this; }
#ifdef _WIN32
    HANDLE _fd; size_t getfd(){ return (size_t)_fd; } 
	FStream(size_t fd) : autodestroy(true){ _fd=(HANDLE)fd; }
	bool open(const CString& fpath, char mode){ DWORD dwDesiredAccess = GENERIC_READ; DWORD dwCreationDisposition = OPEN_EXISTING; //IO_READ
		if(mode == IO_WRITE){ dwDesiredAccess = GENERIC_WRITE; dwCreationDisposition = CREATE_ALWAYS; }
		else if(mode == IO_APPEND){ dwDesiredAccess = GENERIC_WRITE; dwCreationDisposition = OPEN_ALWAYS; }
		_fd = CreateFileW(_toWStr(fpath).c_str(), dwDesiredAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
		if(_fd == INVALID_HANDLE_VALUE){ _fd=0; destroy(); return false; }
		else if(mode == IO_APPEND){ SetFilePointer(_fd, 0, NULL, FILE_END); } return true; }
	
	size_t pos(){ LARGE_INTEGER currPos; currPos.QuadPart = 0; LARGE_INTEGER offset; offset.QuadPart = 0;
		if(SetFilePointerEx(_fd, offset, &currPos, FILE_CURRENT)){ return (size_t)currPos.QuadPart; } return NPOS; }
	void setPos(size_t pos){ LARGE_INTEGER newPos; newPos.QuadPart = pos; SetFilePointerEx(_fd, newPos, NULL, FILE_BEGIN); }
	size_t size(){ LARGE_INTEGER fileSize; if(!GetFileSizeEx(_fd, &fileSize)) return NPOS; return fileSize.QuadPart; }
	void close(){ CloseHandle(_fd); _fd=0; }
	
	int read(char* ptr, size_t size){ DWORD bytesRead; if(ReadFile(_fd, ptr, (DWORD)size, &bytesRead, NULL)){ return (int)bytesRead; } return -1; }
	void write(const char* ptr, size_t size){ DWORD wrbytes; WriteFile(_fd, ptr, (DWORD)size, &wrbytes, NULL); }
#else
	int _fd; size_t getfd(){ return _fd; } 
	FStream(size_t fd) : autodestroy(true){ _fd=fd; }
	bool open(const CString& fpath, char mode){ int flags = 0; mode_t perms = 0744; // O_CREAT
		if(mode == IO_READ){ flags = O_RDONLY; } else if(mode == IO_WRITE){ flags = O_WRONLY | O_CREAT | O_TRUNC; }
		else if(mode == IO_APPEND){ flags = O_WRONLY | O_CREAT | O_APPEND; }
		_fd = ::open(fpath.c_str(), flags, perms); if(_fd==-1){ destroy(); return false; } return true; }
	
	size_t pos(){ return (size_t)lseek(_fd, 0, SEEK_CUR); }
	void setPos(size_t pos){ lseek(_fd, (off_t)pos, SEEK_SET); }
	size_t size(){ struct stat st; if(fstat(_fd, &st)==-1) return NPOS; return (size_t)st.st_size; }
	void close(){ ::close(_fd); _fd=-1; }
	
	int read(char* ptr, size_t size){ return ::read(_fd, ptr, size); }
	void write(const char* ptr, size_t size){ ::write(_fd, ptr, size); }
#endif	
    bool is_open(){ return _fd>0?true:false; }
	bool isOpen(){ return _fd>0?true:false; }
	size_t tellg(){ return pos(); }
	void seekg(size_t pos){ setPos(pos); }
	
	int read(Buffer* rbuff){ int rbytes = read((char*)rbuff->data(), rbuff->size()); rbuff->resize(rbytes); return rbytes; }
	Buffer read(){ size_t fsize=size(); if(fsize==NPOS){ return readEOF(); } Buffer rbuff(fsize-pos()); read(&rbuff); return rbuff; }
    Buffer read(int len){ Buffer rbuff(len); int rbytes=read(&rbuff); print("FStream::read() rbytes: "); print(rbytes); print("\n"); if(rbytes<len){ rbuff.resize(rbytes); } return rbuff; }
	template <typename V>
	V _readEOF(){ V data; char _buff[DEF_SIZE]; int rbytes=0;
		while((rbytes=read(_buff, sizeof(_buff)))>0){ data.push(_buff, rbytes); } return data; }
	Buffer readEOF(){ return _readEOF<Buffer>(); }
	
	bool readline(String& line, bool once=false){ line.clear(); char _b[512]; String buff; buff.stack(_b); int rbytes=0; bool ok=false;
		while((rbytes = read(_b, sizeof(_b)))>0){ ok=true; size_t nidx = buff.indexOf('\n');
			if(nidx==NPOS){ line.push(_b, rbytes); if(line.back()=='\r') line.pop(); continue; }
			int loffset=0; if(rbytes>1&&_b[nidx-1]=='\r') ++loffset;
			line.push(_b, nidx-loffset); if(!once) setPos(pos()-rbytes+nidx+1); return true; } return ok; }
	
	void write(const char* c){ write(c, strlen(c)); }
    void write(const Buffer& wrbuff){ write((const char*)wrbuff.data(), wrbuff.size()); }
	
	FStream& operator<<(const char* c){ write(c); return *this; }
	template <typename T, typename D>
	FStream& operator<<(const BaseString<T, D>& s){ write((const char*)s.data(), s.size()); return *this; }
};

namespace fs{
FStream createReadStream(const CString& path){ return FStream(path, FStream::IO_READ).own(false); }
FStream createWriteStream(const CString& path){ return FStream(path, FStream::IO_WRITE).own(false); }

bool _writeFile(const CString& path, const Buffer& data, char mode=FStream::IO_WRITE){ if(data.size()<=0){ return false; }
	FStream f(path, mode); if(!f.isOpen()){ print("(!) writeFile: Open file error.\n"); return false; } f.write(data); f.close(); return true; }
bool writeFile(const CString& path, const Buffer& data){ return _writeFile(path,data); }
bool appendFile(const CString& path, const Buffer& data){ return _writeFile(path,data,FStream::IO_APPEND); }

template <typename V>
V _readFile(const CString& path){ FStream f(path, FStream::IO_READ); if(!f.isOpen()){ print("(!) fs::_readFile(): Open file error.\n"); return V(); } 
	size_t fsize = f.size(); if(fsize==0){ return f._readEOF<V>(); }else if(fsize==NPOS){ print("(!) fs::_readFile(): fail get file size.\n"); }
	V data(fsize); f.read((char*)data.data(), data.size()); f.close(); return data; }
Buffer readFile(const CString& path){ return _readFile<Buffer>(path); }
	
Array<String> readLines(const CString& path){ Array<String> lines; FStream f(path, FStream::IO_READ);
	if(!f.isOpen()){ print("(!) fs::readLines(): Open file error.\n"); return lines; }
    char _b[DEF_SIZE]; int rbytes = 0; String line;
    while((rbytes = f.read(_b, sizeof(_b)))>0){ int idx=0;
        for(int i=0; i<rbytes; ++i){ 
            if(_b[i] == '\n'){ int loffset=0; if(i>0&&_b[i-1]=='\r') ++loffset;
				lines.push(String()).back().push(line).push(_b+idx, i-idx-loffset); idx=i+1; line.clear(); } }
		line.push(_b+idx, rbytes-idx); if(line.back()=='\r') line.pop();
    } lines.push(line); f.close(); return lines; }
	
String readFstLine(const CString& path){ FStream f(path, FStream::IO_READ); if(!f.isOpen()){ print("fs::readFirstLine(): Open file error.\n"); return ""; }  
	String line; f.readline(line, true); return line; }

StringMap ConfigRead(const CString& path, bool unescape=false, const CString& delim="="){ Array<String> lines=readLines(path);
	StringMap config; for(size_t i=0;i<lines.size();i++){ if(lines[i].size()<3||lines[i].startsWith("#")) continue; Array<String> line = lines[i].split(delim); 
		config[line[0]]=line.slice(1).join(delim); if(unescape&&config[line[0]][0]=='"'&&config[line[0]].back()=='"'){ config[line[0]]=config[line[0]].slice(1,-1); } } return config; }
bool ConfigWrite(const CString& path, StringMap config, String delim="="){ Buffer data; String endl="\n";
	for(StringMap::const_iterator it = config.begin(); it != config.end(); ++it){ data+=it->first+delim+it->second+endl; } return writeFile(path, data); }
//bool ConfigWriteEx(Object config){}?

DoubleMap stat(const CString& path){ DoubleMap stinfo;
	#ifdef _WIN32
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if(GetFileAttributesExW(_toWStr(path).c_str(), GetFileExInfoStandard, &fileInfo) == 0){ print("(!) stat: get file attributes fail.\n"); return stinfo; }
    LARGE_INTEGER fileSize; fileSize.LowPart = fileInfo.nFileSizeLow; fileSize.HighPart = fileInfo.nFileSizeHigh;
	stinfo["mode"] = (double)fileInfo.dwFileAttributes; stinfo["size"] = (double)fileSize.QuadPart; 
	stinfo["blocks"] = stinfo["size"]/512.0; stinfo["atime"] = (double)_FtToUnixTime(fileInfo.ftLastAccessTime);
	stinfo["mtime"] = (double)_FtToUnixTime(fileInfo.ftLastWriteTime); stinfo["ctime"] = (double)_FtToUnixTime(fileInfo.ftCreationTime);
	#else
	struct stat statbuf; if(::stat(path.c_str(), &statbuf)!=0){ print("(!) stat: get stat fail.\n"); return stinfo; }
	stinfo["dev"]=statbuf.st_dev; stinfo["mode"]=statbuf.st_mode; stinfo["size"]=statbuf.st_size; stinfo["blocks"]=statbuf.st_blocks;
	stinfo["ino"]=statbuf.st_ino; stinfo["nlink"]=statbuf.st_nlink; stinfo["uid"]=statbuf.st_uid; stinfo["gid"]=statbuf.st_gid;
	stinfo["atime"]=statbuf.st_atime; stinfo["mtime"]=statbuf.st_mtime; stinfo["ctime"]=statbuf.st_ctime;
	#endif 
	return stinfo; }
	
#ifdef _WIN32
bool isDir(const DoubleMap& stat){ return ((int)stat.at("mode") & FILE_ATTRIBUTE_DIRECTORY)!=0?true:false; }
bool copy(const CString& src, const CString& dst){ return CopyFileW(_toWStr(src).c_str(), _toWStr(dst).c_str(), false); }
bool rename(const CString& oldpath, const CString& newpath){ return MoveFileW(_toWStr(oldpath).c_str(), _toWStr(newpath).c_str()); }
bool unlink(const CString& path){ return DeleteFileW(_toWStr(path).c_str()); }
bool mkdir(const CString& path){ return CreateDirectoryW(_toWStr(path).c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS; }
bool rmdir(const CString& path){ return RemoveDirectoryW(_toWStr(path).c_str()); }
bool chmod(const CString& path, int mode){ return false; }
#include <aclapi.h>
bool chown(const CString& path, const CString& uowner, const CString& ugroup=""){
    _WString wpath = _toWStr(path); _WString wuser = _toWStr(uowner); Buffer sidbuff; 
	DWORD sidSize = 0; DWORD domainSize = 0; SID_NAME_USE sidType; _WString domainName;
    LookupAccountNameW(NULL, wuser.c_str(), NULL, &sidSize, NULL, &domainSize, &sidType);
    if(GetLastError() != ERROR_INSUFFICIENT_BUFFER){ return false; } sidbuff.resize(sidSize); domainName.resize(domainSize);
    if(!LookupAccountNameW(NULL, wuser.c_str(), &sidbuff[0], &sidSize, &domainName[0], &domainSize, &sidType)){ return false; }
    DWORD result = SetNamedSecurityInfoW((LPWSTR)wpath.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &sidbuff[0], NULL, NULL, NULL);
    if(result != ERROR_SUCCESS){ return false; } return true; }
#else
bool isDir(const DoubleMap& stat){ return S_ISDIR((int)stat.at("mode"))?true:false; }
bool copy(const CString& src, const CString& dst){ FStream fsrc(src, FStream::IO_READ); FStream fdst(dst, FStream::IO_WRITE); 
	if(!fsrc.isOpen()||!fdst.isOpen()){ return false; } char _buff[DEF_SIZE]; int rbytes=0;
	while((rbytes=fsrc.read((char*)_buff, sizeof(_buff)))>0){ fdst.write(_buff, rbytes); } return true; }
bool rename(const CString& oldpath, const CString& newpath){ return ::rename(oldpath.c_str(), newpath.c_str()) == 0; }
bool unlink(const CString& path){ return ::unlink(path.c_str()) == 0; }
bool mkdir(const CString& path){ return ::mkdir(path.c_str(), 0755) == 0 || errno == EEXIST; }
bool rmdir(const CString& path){ return ::rmdir(path.c_str()) == 0; }
bool chmod(const CString& path, int mode){ return ::chmod(path.c_str(), mode) == 0; }
bool chown(const CString& path, const CString& uowner, const CString& ugroup=""){
    struct passwd* pw = getpwnam(uowner.c_str()); if(!pw){ return false; } uid_t uid = pw->pw_uid; 
	gid_t gid; struct group* gr = getgrnam(ugroup.c_str()); if(gr){ gid = gr->gr_gid; }else{ gid = pw->pw_gid; }
	return ::chown(path.c_str(), uid, gid)!=0; }
#endif
bool exists(const CString& path){ return stat(path).has("size"); }
bool isDir(const CString& path){ return isDir(stat(path)); }
bool rm(const CString& path){ return unlink(path); }

Array<String> readDir(const CString& path){ Array<String> files;
#ifdef _WIN32
    WIN32_FIND_DATAW findFileData; HANDLE hF=FindFirstFileW(_toWStr(path+"\\*").c_str(), &findFileData);
    if(hF==INVALID_HANDLE_VALUE){ print("(!) Failed to open directory: "); print(path); print("\n"); return files; }
    do { files.push(_toUTF8(findFileData.cFileName)); }while(FindNextFileW(hF, &findFileData) != 0); FindClose(hF);
#else
    DIR* dir = opendir(path.c_str()); if(dir==NULL){ print("(!) Failed to open directory: "); print(path); print("\n"); return files; }
    struct dirent* entry; while((entry = readdir(dir)) != NULL){ String name=entry->d_name; files.push(name); } closedir(dir);
#endif
	return files; }
//struct FileInfo {};
//Array<String> readDirEx(const CString& path);
	
String dirname(const CString& path){ size_t pos = path.find_last_of("/\\");  
	if(pos==NPOS){ return "."; } return path.substr(0, pos); }
	
} }
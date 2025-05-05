#ifndef _WIN32
#include <sys/stat.h> // posix stat()
#include <dirent.h> //opendir/readdir/closedir
#include <pwd.h>
#include <grp.h>
#endif
namespace ncpp{

/*struct EventEmitter { 
	void on(std::string event, void (*callback)());
	void once(std::string event, void (*callback)());
	void emit(std::string event);
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

namespace fs{
bool _writeFile(const std::string& path, const Buffer& data, std::ios_base::openmode mode=std::ios::binary){ std::ofstream f(path.c_str(), mode); 
	if(!f.is_open()){ print("writeFile: Open file error."); print("\n"); return false; }
	if(data.size()<=0){ f.close(); return true; } f.write((const char*)&data[0], data.size()); f.close(); return true; }
bool writeFile(const std::string& path, const Buffer& data){ return _writeFile(path,data); }
bool appendFile(const std::string& path, const Buffer& data){ return _writeFile(path,data,std::ios::binary|std::ios::app); }

Buffer readFile(const std::string& path){ std::ifstream f(path.c_str(), std::ios::binary);
	if(!f.is_open()){ print("readFile: Open file error.\n"); return Buffer(); }
    f.seekg(0, std::ios::end); std::streampos fileSize = f.tellg(); 
    if(fileSize<=0){ print("readFile: Invalid file size: "); print(dtos(fileSize)); print("\n"); return Buffer(); } 
	f.seekg(0, std::ios::beg); Buffer data(fileSize); if(f.read((char*)&data[0], fileSize)){ return data; }
	else{ print("readFile: Read file error.\n"); } f.close(); return Buffer(); }
	
Array<String> readLines(const std::string& path){ std::ifstream f(path.c_str()); Array<String> lines;
	if(!f.is_open()){ print("readLines: Open file error.\n"); return lines; }
	std::string line; while(std::getline(f, line)){ lines.push(line); } f.close(); return lines; }
	
String readLine(const std::string& path){ std::ifstream f(path.c_str()); 
	if(!f.is_open()){ print("readLine: Open file error.\n"); return ""; } 
	std::string line; std::getline(f, line); f.close(); return line; }

StringMap ConfigRead(const std::string& path, bool unescape=false, std::string delim="="){ Array<String> lines=readLines(path);
	StringMap config; for(size_t i=0;i<lines.size();i++){ if(lines[i].size()<3||lines[i].startsWith("#")) continue; Array<String> line = lines[i].split(delim); 
		config[line[0]]=line.slice(1).join(delim); if(unescape&&config[line[0]][0]=='"'&&config[line[0]].back()=='"'){ config[line[0]]=config[line[0]].slice(1,-1); } } return config; }
bool ConfigWrite(const std::string& path, StringMap config, std::string delim="="){ Buffer data; std::string endl="\n";
	for(StringMap::const_iterator it = config.begin(); it != config.end(); ++it){ data+=it->first+delim+it->second+endl; } return writeFile(path, data); }
//bool ConfigWriteEx(Object config){}??

DoubleMap stat(const std::string& path){ DoubleMap stinfo;
	#ifdef _WIN32
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if(GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fileInfo) == 0){ print("stat: get file attributes fail.\n"); return stinfo; }
    LARGE_INTEGER fileSize; fileSize.LowPart = fileInfo.nFileSizeLow; fileSize.HighPart = fileInfo.nFileSizeHigh;
	stinfo["mode"] = (double)fileInfo.dwFileAttributes; stinfo["size"] = (double)fileSize.QuadPart; 
	stinfo["blocks"] = stinfo["size"]/512.0; stinfo["atime"] = (double)_FtToUnixTime(fileInfo.ftLastAccessTime);
	stinfo["mtime"] = (double)_FtToUnixTime(fileInfo.ftLastWriteTime); stinfo["ctime"] = (double)_FtToUnixTime(fileInfo.ftCreationTime);
	#else
	struct stat statbuf; if(::stat(path.c_str(), &statbuf)!=0){ print("stat: get stat fail.\n"); return stinfo; }
	stinfo["dev"]=statbuf.st_dev; stinfo["mode"]=statbuf.st_mode; stinfo["size"]=statbuf.st_size; stinfo["blocks"]=statbuf.st_blocks;
	stinfo["ino"]=statbuf.st_ino; stinfo["nlink"]=statbuf.st_nlink; stinfo["uid"]=statbuf.st_uid; stinfo["gid"]=statbuf.st_gid;
	stinfo["atime"]=statbuf.st_atime; stinfo["mtime"]=statbuf.st_mtime; stinfo["ctime"]=statbuf.st_ctime;
	#endif 
	return stinfo; }
	
#ifdef _WIN32
bool isDir(const DoubleMap& stat){ return ((int)stat.at("mode") & FILE_ATTRIBUTE_DIRECTORY)!=0?true:false; }
bool copy(const std::string& src, const std::string& dst){ return CopyFileW(_toWStr(src).c_str(), _toWStr(dst).c_str(), false); }
bool rename(const std::string& oldpath, const std::string& newpath){ return MoveFileW(_toWStr(oldpath).c_str(), _toWStr(newpath).c_str()); }
bool unlink(const std::string& path){ return DeleteFileW(_toWStr(path).c_str()); }
bool mkdir(const std::string& path){ return CreateDirectoryW(_toWStr(path).c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS; }
bool rmdir(const std::string& path){ return RemoveDirectoryW(_toWStr(path).c_str()); }
bool chmod(const std::string& path, int mode){ return false; }
#include <aclapi.h>
bool chown(const std::string& path, const std::string& uowner, const std::string& ugroup=""){
    std::wstring wpath = _toWStr(path); std::wstring wuser = _toWStr(uowner); Buffer sidbuff; 
	DWORD sidSize = 0; DWORD domainSize = 0; SID_NAME_USE sidType; std::wstring domainName;
    LookupAccountNameW(NULL, wuser.c_str(), NULL, &sidSize, NULL, &domainSize, &sidType);
    if(GetLastError() != ERROR_INSUFFICIENT_BUFFER){ return false; } sidbuff.resize(sidSize); domainName.resize(domainSize);
    if(!LookupAccountNameW(NULL, wuser.c_str(), &sidbuff[0], &sidSize, &domainName[0], &domainSize, &sidType)){ return false; }
    DWORD result = SetNamedSecurityInfoW((LPWSTR)wpath.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &sidbuff[0], NULL, NULL, NULL);
    if(result != ERROR_SUCCESS){ return false; } return true; }
#else
bool isDir(const DoubleMap& stat){ return S_ISDIR((int)stat.at("mode"))?true:false; }
bool copy(const std::string& src, const std::string& dst){ std::ifstream fsrc(src.c_str(), std::ios::binary);
    std::ofstream fdst(dst.c_str(), std::ios::binary); if(!fsrc.is_open() || !fdst.is_open()){ return false; } fdst << fsrc.rdbuf(); return true; }
bool rename(const std::string& oldpath, const std::string& newpath){ return ::rename(oldpath.c_str(), newpath.c_str()) == 0; }
bool unlink(const std::string& path){ return ::unlink(path.c_str()) == 0; }
bool mkdir(const std::string& path){ return ::mkdir(path.c_str(), 0755) == 0 || errno == EEXIST; }
bool rmdir(const std::string& path){ return ::rmdir(path.c_str()) == 0; }
bool chmod(const std::string& path, int mode){ return ::chmod(path.c_str(), mode) == 0; }
bool chown(const std::string& path, const std::string& uowner, const std::string& ugroup=""){
    struct passwd* pw = getpwnam(uowner.c_str()); if(!pw){ return false; } uid_t uid = pw->pw_uid; 
	gid_t gid; struct group* gr = getgrnam(ugroup.c_str()); if(gr){ gid = gr->gr_gid; }else{ gid = pw->pw_gid; }
	return ::chown(path.c_str(), uid, gid)!=0; }
#endif
bool exists(const std::string& path){ return stat(path).has("size"); }
bool isDir(const std::string& path){ return isDir(stat(path)); }
bool rm(const std::string& path){ return unlink(path); }

Array<String> readDir(const std::string& path){ Array<String> files;
#ifdef _WIN32
    WIN32_FIND_DATAW findFileData; HANDLE hF=FindFirstFileW(_toWStr(path+"\\*").c_str(), &findFileData);
    if(hF==INVALID_HANDLE_VALUE){ print("Failed to open directory: "); print(path); print("\n"); return files; }
    do { std::string name=_toUTF8(findFileData.cFileName); files.push(name); }while(FindNextFileW(hF, &findFileData) != 0); FindClose(hF);
#else
    DIR* dir = opendir(path.c_str()); if(dir==NULL){ print("Failed to open directory: "); print(path); print("\n"); return files; }
    struct dirent* entry; while((entry = readdir(dir)) != NULL){ std::string name=entry->d_name; files.push(name); } closedir(dir);
#endif
	return files; }
	
std::string dirname(const std::string& path){ size_t pos = path.find_last_of("/\\");  
	if(pos==std::string::npos){ return "."; } return path.substr(0, pos); }

struct FStream : Stream {
    FStream(const std::string& path, std::ios_base::openmode mode)
		: _file(path.c_str(), mode), fpath(path), fmode(mode)
		{ _createStream(); _file.seekg(0, std::ios::end); fileSize = _file.tellg(); _file.seekg(0, std::ios::beg); }
	FStream(const FStream& other) : _file(other.fpath.c_str(), other.fmode), fpath(other.fpath), fmode(other.fmode), filePos(other.filePos), fileSize(other.fileSize)
	{ _createStream(); _file.seekg(filePos); //print("DEBUG: FStream: Destroyment and Fenix Constructor Called.\n"); 
		} // Конструктор уничтожения и воссоздания заново(из-за сраного std::fstream).
    ~FStream(){ if(_file.is_open()){ _file.close(); } }
    	
    int read(unsigned char* ptr, size_t size){ _file.read((char*)ptr, size); return _file.gcount(); }
    int read(Buffer* rbuff){ _file.read((char*)&(*rbuff)[0], rbuff->size()); if(_file.gcount()<(int)rbuff->size()){ rbuff->resize(_file.gcount()); } return _file.gcount(); }
    Buffer read(int size=-1){ filePos=_file.tellg(); 
		if(size>0){ Buffer rbuff(size); _file.read((char*)&rbuff[0], rbuff.size()); if(_file.gcount()<size){ rbuff.resize(_file.gcount()); } return rbuff; }
		else{ Buffer rbuff(fileSize-filePos); _file.read((char*)&rbuff[0], fileSize-filePos); return rbuff; } }
	void write(const unsigned char* ptr, size_t size){ if(_file.is_open()){ _file.write((const char*)ptr, size); } }
    void write(const Buffer& wrbuff){ if(wrbuff.size()<=0){ return; } if(_file.is_open()){ _file.write((const char*)&wrbuff[0], wrbuff.size()); } }
	
	template<typename T>
    static void _swap(T& a, T& b){ T tmp(a); a = b; b = tmp; }
    static FStream createReadStream(const std::string& path){ return FStream(path, std::ios::in | std::ios::binary); }
    static FStream createWriteStream(const std::string& path){ return FStream(path, std::ios::out | std::ios::binary); }

protected:
    //void _onWrite(const Buffer& wrbuff){ if(_file.is_open()){ _file.write((const char*)&wrbuff[0], wrbuff.size()); } }
    //void _onRead(const Buffer& rbuff){ if(_file.is_open()){ _file.read(reinterpret_cast<char*>(rbuff.data()), rbuff.size()); } }
    void _createStream(){ if(!_file.is_open()){ print("FStream: Open file error: "); print(fpath); print("\n"); destroyed=true; return; } }
    
    void _onDestroy(){ if(_file.is_open()){ _file.close(); } }

private:
    std::fstream _file; std::string fpath; std::ios_base::openmode fmode; std::streampos filePos; std::streamsize fileSize;
    //Запрещаем копирование (из-за сраного std::fstream, в котором оно приватное)
    //FStream(const FStream&);
    //FStream& operator=(const FStream&);
};
	
FStream createReadStream(std::string path){ return FStream(path, std::ios::in | std::ios::binary); }
FStream createWriteStream(std::string path){ return FStream(path, std::ios::out | std::ios::binary); }
} }

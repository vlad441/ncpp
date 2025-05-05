namespace ncpp{
	String excmd(const std::string& cmd, Stream* stream=NULL){ std::stringstream ss; char buf[1024]; std::string ecmd;
	#ifdef _WIN32
		ecmd="cmd /c "+cmd; PROCESS_INFORMATION pi; STARTUPINFOW si; ZeroMemory(&si, sizeof(STARTUPINFO)); SECURITY_ATTRIBUTES sa; 
		HANDLE newstdout,hRead; sa.lpSecurityDescriptor = NULL; sa.nLength = sizeof(SECURITY_ATTRIBUTES); sa.bInheritHandle = true;
		if(!CreatePipe(&hRead,&newstdout,&sa,0)){ return "(CreatePipe Error)"; } GetStartupInfoW(&si); 
		si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW; si.hStdOutput = newstdout; si.hStdError = newstdout; si.wShowWindow = SW_HIDE;
		if(!CreateProcessW(NULL,(LPWSTR)_toWStr(ecmd).c_str(),NULL,NULL,true,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))
		{ CloseHandle(newstdout); CloseHandle(hRead); return "(CreateProcess Error)"; } unsigned long exit=0, bread, avail; String str;
	
		while(true){ PeekNamedPipe(hRead,buf,sizeof(buf),&bread,&avail,NULL); if(bread>0){ do{ ReadFile(hRead,buf,sizeof(buf),&bread,NULL);
				if(stream==NULL){ str+=_OEMtoUTF8(&buf[0], bread); }else{ (*stream)<<_OEMtoUTF8(&buf[0], bread); } //ZeroMemory(buf, sizeof(buf));
			}while(bread >= sizeof(buf)); } GetExitCodeProcess(pi.hProcess,&exit); if(exit!=STILL_ACTIVE){ break; } Sleep(200); }
		CloseHandle(pi.hThread); CloseHandle(pi.hProcess); CloseHandle(newstdout); CloseHandle(hRead); return str;
	#else
		ecmd="/bin/sh -c \""+cmd+" 2>&1\""; FILE* pipe = popen(ecmd.c_str(), "r"); if(!pipe){ throw std::runtime_error("popen() failed!"); }
		while(fgets(buf, sizeof(buf), pipe)!=NULL){ if(stream==NULL){ ss << buf; }else{ (*stream) << buf; } } pclose(pipe);
	#endif
		return ss.str(); }
		
	namespace system {
		#ifdef _WIN32
		long long getUptime(char type='s'){ return type=='m'?GetTickCount():GetTickCount()/1000.0; } //GetTickCount64()
		#include <psapi.h>
		std::string ReadRegStr(HKEY hKeyRoot, const std::string& path, const std::string& valueName){
			HKEY hKey; LONG lRes = RegOpenKeyExA(hKeyRoot, path.c_str(), 0, KEY_READ, &hKey); if(lRes != ERROR_SUCCESS){ return ""; }
			char wbuff[512]; DWORD bSize = sizeof(wbuff); DWORD type = 0;
			lRes = RegQueryValueExW(hKey, _toWStr(valueName).c_str(), NULL, &type, reinterpret_cast<LPBYTE>(wbuff), &bSize);
			if (lRes != ERROR_SUCCESS){ RegCloseKey(hKey); return ""; } RegCloseKey(hKey);

			if(type==REG_DWORD){ return dtos(Buffer(&wbuff[0], 4).readUInt32LE()); }
			if(type!=REG_SZ&&type!=REG_EXPAND_SZ){ return ""; } return _toUTF8((wchar_t*)wbuff, bSize-1); } // Убираем нулевой терминатор
		StringMap os_info(){ StringMap sinfo; OSVERSIONINFO osvi; 
			ZeroMemory(&osvi, sizeof(OSVERSIONINFO)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			sinfo["OS"]="Windows "; if(GetVersionEx(&osvi)){ sinfo["NT"]=dtos(osvi.dwMajorVersion)+"."+dtos(osvi.dwMinorVersion); 
				float nt=stodn(sinfo["NT"]); if(nt>=6.2f){
					std::string OSName = ReadRegStr(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
					std::string OSVer = ReadRegStr(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentMajorVersionNumber");
					if(OSName.empty()){ sinfo["OS"]+=">=8"; }else{ sinfo["OS"]=OSName; } if(!OSVer.empty()){ sinfo["NT"]=OSVer; } }
				else if(nt==6.1f){ sinfo["OS"]+="7"; } else if(nt==6){ sinfo["OS"]+="Vista"; } else if(nt==5.2f){ sinfo["OS"]+="XP 64-bit Edition"; } 
				else if(nt==5.1f){ sinfo["OS"]+="XP"; } else if(nt==5){ sinfo["OS"]+="2000"; } else { sinfo["OS"]+="(?)"; }
			}else{ sinfo["OS"]+="(-)"; } char buf[256]; DWORD size=256; GetComputerNameA(&buf[0],&size); 
			sinfo["Hostname"]=std::string(buf); return sinfo; }
		#else
		long long getUptime(char type='s'){ String line = fs::readLine("/proc/uptime"); line=line.split(" ")[0]; return type=='m'?stodn(line)*1000:stodn(line); }
		#include <sys/utsname.h> //for uname()
		StringMap os_info(){ StringMap sinfo; struct utsname buffer;
			if(uname(&buffer)==0){ sinfo["OS"]=buffer.sysname; sinfo["Kernel"]=buffer.release; sinfo["Arch"]=buffer.machine; 
				sinfo["Hostname"]=buffer.nodename; }else{ sinfo["OS"]="Linux"; sinfo["Kernel"]="-"; }
			StringMap conf = fs::ConfigRead("/etc/os-release", true); if(conf.has("NAME")){ sinfo["OS"]=conf["NAME"]+" "+conf["VERSION_ID"]; return sinfo; }
			String str = fs::readFile("/etc/debian_version").toString(); if(str!=""){ sinfo["OS"]="Debian "+str; sinfo["Package Manager"]="apt"; return sinfo; }
			str = fs::readFile("/etc/redhat-release").toString(); if(str!=""){ sinfo["OS"]=str; sinfo["Package Manager"]="yum"; return sinfo; } return sinfo; }
		#endif
		
		String getUptimeStr(){ String str; size_t uptime=getUptime('s'); str=dtos(uptime/86400)+" Days, "; 
			uptime-=uptime/86400*86400; str+=dtos(uptime/3600)+" Hours, "; uptime-=uptime/3600*3600; str+=dtos(uptime/60)+" Mins"; return str; }
		
		namespace CPU {
			StringMap info(){ StringMap cpuinfo;
			#ifdef _WIN32
				cpuinfo["model"] = ReadRegStr(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");
				DWORD bufferSize = 0; GetLogicalProcessorInformation(nullptr, &bufferSize); // Запрос размера буфера
				std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(bufferSize/sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
				if(!GetLogicalProcessorInformation(&buffer[0], &bufferSize)){ return cpuinfo; } int sockets=0, cores=0, threads=0;
				for (std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION>::iterator it = buffer.begin(); it != buffer.end(); ++it){
					SYSTEM_LOGICAL_PROCESSOR_INFORMATION& info = *it;
					if (info.Relationship == RelationProcessorCore){ cores++; DWORD mask = info.ProcessorMask;
						int threadCount = 0; while(mask){ threadCount += mask & 1; mask >>= 1; } threads += threadCount;
					}else if (info.Relationship == RelationProcessorPackage){ sockets++; } }
				cpuinfo["sockets"]=dtos(sockets); cpuinfo["cores"]=dtos(cores); cpuinfo["threads"]=dtos(threads);
			#else
				Array<String> lines = fs::readLines("/proc/cpuinfo"); StringMap cpu0; size_t indx=0; int threads=1;
				for(;indx<lines.size();indx++){ if(lines[indx]==""){ break; } Array<String> splt=lines[indx].split(":"); cpu0[splt[0].trim()]=splt[1].trim(); }
				HashSet<std::string> ids; ids.insert(cpu0["physical id"]); for(;indx<lines.size();indx++){ 
					if(lines[indx].startsWith("processor")){ threads+=1; }else if(lines[indx].startsWith("physical id")){ 
						Array<String> splt=lines[indx].split(":"); ids.insert(splt[1].trim()); } }
				cpuinfo["model"]=cpu0["model name"]; cpuinfo["sockets"]=dtos(ids.size()); cpuinfo["cores_per_socket"]=cpu0["cpu cores"];
				cpuinfo["cores"]=dtos(stoin(cpu0["cpu cores"])*ids.size()); cpuinfo["threads"]=dtos(threads);
			#endif
				return cpuinfo; }
				
			struct CPUTime { long long user; long long nice; long long system; long long idle; long long iowait; long long irq; long long softirq;
				CPUTime() : user(0), nice(0), system(0), idle(0), iowait(0), irq(0), softirq(0){}
			}; Array<CPUTime> PrevLoadStats; CPUTime PrevAvgLoad, PrevUsage;
			#ifdef _WIN32
			String model(){ return ReadRegStr(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString"); return ""; }
			CPUTime getTotalTime(){ CPUTime avgtime; FILETIME idleTime, kernelTime, userTime;
				if(!GetSystemTimes(&idleTime, &kernelTime, &userTime)){ return avgtime; } avgtime.idle = _FtToUnixTime(idleTime, 'm'); 
				avgtime.system = _FtToUnixTime(kernelTime, 'm')-avgtime.idle; avgtime.user = _FtToUnixTime(userTime, 'm'); return avgtime; }
			//TODO: getCoresTime for Windows
			Array<CPUTime> getCoresTime(){ return Array<CPUTime>(); }
			CPUTime getUsageTime(int pid=-1){ CPUTime stats; HANDLE hProc; if(pid<0){ hProc = GetCurrentProcess(); }
				else{ hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid); if(!hProc){ print("Failed to open process.\n"); return stats; } }
				FILETIME creationTime, exitTime, kernelTime, userTime; GetProcessTimes(hProc, &creationTime, &exitTime, &kernelTime, &userTime); CloseHandle(hProc);
				stats.system=_FtToUnixTime(kernelTime, 'm'); stats.user=_FtToUnixTime(userTime, 'm'); 
				stats.idle=GetTimestamp('m')-_FtToUnixTime(creationTime, 'm')-stats.system-stats.user; return stats; }
			#else
			String model(){ Array<String> lines = fs::readLines("/proc/cpuinfo"); for(size_t indx=0;indx<lines.size();indx++){ 
					if(lines[indx].startsWith("model name")){ Array<String> splt=lines[indx].split(":"); return splt[1].trim(); } } return ""; }	
			CPUTime _parseCPUStats(const std::string& line){ std::istringstream iss(line); std::string cpuLabel; CPUTime stats; 
				iss >> cpuLabel >> stats.user >> stats.nice >> stats.system >> stats.idle >> stats.iowait >> stats.irq >> stats.softirq; return stats; }
			CPUTime getTotalTime(){ CPUTime avgtime; String line = fs::readLine("/proc/stat"); if(!line.startsWith("cpu ")){ return avgtime; } return _parseCPUStats(line); }
			Array<CPUTime> getCoresTime(){ Array<CPUTime> CoresTime; Array<String> lines = fs::readLines("/proc/stat"); for(size_t i=1; i<lines.size();i++){ 
				if(!lines[i].startsWith("cpu")){ continue; } CoresTime.push(_parseCPUStats(lines[i])); } return CoresTime; }
			CPUTime getUsageTime(int pid = -1){ CPUTime stats; long ticksPerSec = sysconf(_SC_CLK_TCK); if(ticksPerSec == 0){ return stats; }
				std::string statPath = (pid<0) ? "/proc/self/stat" : "/proc/" + dtos(pid) + "/stat";
				std::ifstream statFile(statPath.c_str()); 
				if(!statFile.is_open()){ print("getUsageTime(): Failed to open "); print(statPath); print("\n"); return stats; }
				std::string line; std::getline(statFile, line); std::istringstream iss(line); long starttime = 0;
				for(int i = 0; i < 13; ++i){ iss >> line; } iss >> stats.user >> stats.system;
				for(int i = 0; i < 4; ++i){ iss >> line; } iss >> starttime;
				stats.user = (stats.user*1000) / ticksPerSec; stats.system = (stats.system*1000) / ticksPerSec;
				starttime = (starttime*1000) / ticksPerSec; stats.idle = (getUptime('m')-starttime)-stats.user-stats.system; return stats; }
			#endif
			unsigned long long _totalCPUTime(const CPUTime& stats){ return stats.user
				+stats.nice+stats.system+stats.idle+stats.iowait+stats.irq+stats.softirq; }
				
			double calcLoad(CPUTime& prevTime, const CPUTime& currTime){ long long totalDiff = _totalCPUTime(currTime)-_totalCPUTime(prevTime);
				long long idleDiff = currTime.idle-prevTime.idle; prevTime=currTime; return 100.0*(totalDiff-idleDiff)/totalDiff; }
			
			Array<double> calcLoad(Array<CPUTime>& prevCoresTime, const Array<CPUTime>& CoresTime){ Array<double> cpuload(CoresTime.size());
				for(size_t i=0;i<CoresTime.size()&&i<prevCoresTime.size();i++){ 
					long long totalDiff = _totalCPUTime(CoresTime[i])-_totalCPUTime(prevCoresTime[i]);
					long long idleDiff = CoresTime[i].idle-prevCoresTime[i].idle; cpuload[i] = 100.0*(totalDiff-idleDiff)/totalDiff;
				} prevCoresTime=CoresTime; return cpuload; }
			
			Array<double> getLoad(){ return calcLoad(PrevLoadStats, getCoresTime()); }
			double getAvgLoad(){ if(_totalCPUTime(PrevAvgLoad)<=0){ PrevAvgLoad=getTotalTime(); return 0; }else{ return calcLoad(PrevAvgLoad, getTotalTime()); } }
			double usage(int pid=-1){ return calcLoad(PrevUsage, getUsageTime(pid)); }
		}
		namespace RAM {
			DoubleMap info(){ DoubleMap raminfo;
			#ifdef _WIN32
				MEMORYSTATUSEX statex; statex.dwLength = sizeof(statex);
				if(!GlobalMemoryStatusEx(&statex)){ print("Failed to get memory status\n"); return DoubleMap(); }
				raminfo["used"] = (double)(statex.ullTotalPhys - statex.ullAvailPhys);
				raminfo["available"] = (double)statex.ullAvailPhys; raminfo["total"] = (double)statex.ullTotalPhys;
				raminfo["swap_total"] = (double)statex.ullTotalPageFile - (double)statex.ullTotalPhys;
			#else
				Array<String> lines = fs::readLines("/proc/meminfo"); StringMap tinfo; Array<String> args;
				for(size_t i=0;i<lines.size();++i){ args = lines[i].split(":"); tinfo[args[0]]=args[1].trim(); }
				double total=stodn(tinfo["MemTotal"].split(" ")[0])*1024, available=stodn(tinfo["MemAvailable"].split(" ")[0])*1024;
				raminfo["total"]=total; raminfo["used"] = (total-available); raminfo["available"]=available;
				raminfo["swap_total"] = stodn(tinfo["SwapTotal"].split(" ")[0])*1024;
				raminfo["swap_used"] = stodn(tinfo["SwapTotal"].split(" ")[0])*1024-stodn(tinfo["SwapFree"].split(" ")[0])*1024;
			#endif
				return raminfo; }
							
			DoubleMap usage(int pid=-1){ DoubleMap uinfo;
			#ifdef _WIN32
				PROCESS_MEMORY_COUNTERS pmc; if(pid>0){ HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
					if(hProc==NULL||!GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))){ CloseHandle(hProc); return uinfo; }
					CloseHandle(hProc); }else{ if(!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))){ return uinfo; } }
				uinfo["rss"] = pmc.WorkingSetSize; uinfo["swap"] = pmc.PagefileUsage; uinfo["virt"] = uinfo["rss"]+uinfo["swap"];
			#else
				String line; if(pid>0){ line = fs::readLine("/proc/"+dtos(pid)+"/statm"); }else{ line = fs::readLine("/proc/self/statm"); }
				std::istringstream iss(line); size_t total, rss; iss >> total >> rss; size_t pgsize = sysconf(_SC_PAGESIZE);
				uinfo["virt"] = total*pgsize; uinfo["rss"] = rss*pgsize;
			#endif
				return uinfo; }
			size_t usage_rss(int pid=-1){ return usage(pid)["rss"]; }
			String strUsage(const DoubleMap& rinf=info()){ return dtos(rinf.at("used")/1073741824.0, 2)+" GB/"+dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
			String strTotal(const DoubleMap& rinf=info()){ return dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
		}
		namespace Disk { struct DiskInfo { std::string model; unsigned long long used, size; std::string label; };
			#ifdef _WIN32
			String _norm_spaces(const std::string& input){ std::string output; bool inSpace = false; 
				for(size_t i = 0; i < input.length(); ++i){
					if(input[i] == ' ' || input[i] == '\t'){ if(!inSpace) { output += ' '; inSpace = true; } } 
					else { output += input[i]; inSpace = false; } } return output; }

			Array<DiskInfo> drives(){ Array<String> out = _norm_spaces(excmd("wmic diskdrive get Caption,Size")).split("\r\n");
				Array<DiskInfo> list; for(size_t i=1;i<out.size(); i++){ out[i]=out[i].trim(); if(out[i].empty()) continue; 
					Array<String> tmp = out[i].split(" "); DiskInfo dinfo; dinfo.model=tmp[0]; dinfo.used=0;
					dinfo.size=(tmp.size()>1)?stolln(tmp[1]):0; list.push(dinfo); } return list; }
				
			Array<DiskInfo> logical(){ Array<String> out = _norm_spaces(excmd("wmic logicaldisk get DeviceID,FreeSpace,Size,VolumeName")).split("\n");
				Array<DiskInfo> list; for(size_t i=1;i<out.size(); i++){ out[i]=out[i].trim(); if(out[i].empty()) continue; 
					Array<String> tmp = out[i].split(" "); DiskInfo dinfo; dinfo.model=tmp[0]; dinfo.used=0; dinfo.label=(tmp.size()>3)?tmp[3]:"";
					dinfo.size=(tmp.size()>2)?stolln(tmp[2]):0; dinfo.used=(tmp.size()>2)?dinfo.size-stolln(tmp[1]):0; list.push(dinfo); } return list; }
			#else
			#include <sys/statvfs.h>
			#include <mntent.h>
			Array<DiskInfo> drives(){ Array<String> dir = fs::readDir("/sys/block"); Array<DiskInfo> list;
				for(size_t i=0;i<dir.size(); i++){ if(dir[i]=="."||dir[i]== ".." || dir[i].find("loop") == 0 || dir[i].find("ram") == 0) continue;
					DiskInfo dinfo; dinfo.model=fs::readLine("/sys/block/"+dir[i]+"/device/model").trim(); dinfo.label=dir[i]; dinfo.used=0;
					dinfo.size=stolln(fs::readLine("/sys/block/"+dir[i]+"/size").trim())*512; list.push(dinfo); } return list; }
						
			Array<DiskInfo> logical(){ Array<DiskInfo> list;
				FILE* mounts = setmntent("/proc/mounts", "r"); if(!mounts) return list; struct mntent* ent;
				while((ent = getmntent(mounts)) != NULL){ const char* device = ent->mnt_fsname;	
					const char* mountpoint = ent->mnt_dir; if(strncmp(device, "/dev/", 5) != 0) continue; // Пропускаем виртуальные и tmpfs
					struct statvfs stats; if(statvfs(mountpoint, &stats) != 0) continue; DiskInfo info; 
					info.model = device; info.label = mountpoint; info.size = stats.f_blocks * stats.f_frsize; 
					info.used = info.size-(stats.f_bfree * stats.f_frsize); list.push(info); } endmntent(mounts); return list; }
			// struct statvfs.f_files; // общее число дескрипторов файлов (inodes)
			#endif
			Array<String> models(){ Array<DiskInfo> dinfo = drives(); Array<String> list;
				for(size_t i=0;i<dinfo.size(); i++){ list.push(dinfo[i].model); } return list; }
			
			DoubleMap info(){ DoubleMap dinfo; Array<DiskInfo> d1=drives(); dinfo["drives"]=d1.size(); double tsize=0; 
				for(size_t i=0;i<d1.size(); i++){ tsize+=d1[i].size; } dinfo["total"]=tsize; Array<DiskInfo> d2=logical();
				tsize=0; for(size_t i=0;i<d2.size(); i++){ tsize+=d2[i].used; } dinfo["used"]=tsize; return dinfo; }
			String strUsage(const DoubleMap& rinf=info()){ return dtos(rinf.at("used")/1073741824.0, 2)+" GB/"+dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
			String strTotal(const DoubleMap& rinf=info()){ return dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
		}
		namespace GPU {
			#ifdef _WIN32
			Array<String> list(){ HashSet<String> GPUs;	DISPLAY_DEVICE dd; ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); int deviceIndex = 0;
				while(EnumDisplayDevices(NULL, deviceIndex, &dd, 0)){ GPUs.insert(dd.DeviceString); deviceIndex++;
				ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); } return GPUs.keys(); }
			String model(){ DISPLAY_DEVICE dd; ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); EnumDisplayDevices(NULL, 0, &dd, 0); return dd.DeviceString; }
			#else
			Array<String> list(){ HashSet<String> GPUs; Array<String> out = excmd("lspci | grep -E 'VGA|3D|Display'").split("\n");
				for(size_t i=0;i<out.size();i++){ Array<String> arr=out[i].split(": "); if(arr.size()>1){ GPUs.insert(arr[1].trim()); } } return GPUs.keys(); }
			String model(){ Array<String> lst=list(); return lst.size()>0?lst[0]:""; }
			#endif		
		}
		
		StringMap info(){ StringMap sysinfo=os_info(); sysinfo["Memory"]=RAM::strTotal(); sysinfo["CPU"]=CPU::model(); 
			sysinfo["GPU"]=GPU::model(); sysinfo["Uptime"]=getUptimeStr(); //neofetch
			return sysinfo; }
	}
	
	#ifdef _WIN32
	#include <tlhelp32.h>
	#endif
	struct ProcessInfo { int pid; int threads; int parent; int priority; std::string name;
		ProcessInfo() : pid(-1), threads(0), parent(0), priority(0), name(""), state('-'), nice(0), virt(0), rss(0){}
		char state; int nice; size_t virt; size_t rss; };
	struct ProcessOpts { char detach; bool getpid; bool StdOut; bool StdIn;
		ProcessOpts(char detach1=1, bool getpid1=true, bool stdout1=false, bool stdin1=false) : 
			detach(detach1), getpid(getpid1), StdOut(stdout1), StdIn(stdin1){} };
	
	struct Process { int pid;
		Process() : pid(0){} Process(int numb) : pid(numb){} ~Process(){}
		static Process start(const std::string& execpath, const ProcessOpts& opts){ Process proc(_run(execpath, opts)); return proc; }
		static Process start(const std::string& execpath, char detach=1){ Process proc(_run(execpath, ProcessOpts(detach))); return proc; }
		static bool exec(const std::string& execcmd, char detach=0){ return _run(execcmd, ProcessOpts(detach, false))>0?true:false; }
		
		static bool signal(int pid, int id){ if(pid<=0){ return false; }
		#ifdef _WIN32
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid); if(hProcess == NULL){ return false; }
			if (!TerminateProcess(hProcess, 0)){ CloseHandle(hProcess); return false; } CloseHandle(hProcess);
		#else
			if(::kill(pid, id) == -1){ return false; }
		#endif
			return true; }
		static bool kill(int pid){ return signal(pid, 9); } static bool term(int pid){ return signal(pid, 15); }
		bool signal(int id){ if(signal(pid, id)){pid=-1; return true; }else{ return false; }; }
		bool kill(){ return signal(9); } bool term(){ return signal(15); }

		#ifdef _WIN32
		inline static int getpid(){	return (int)GetCurrentProcessId(); }
		bool isrun() const { if(pid<=0){ return false; }
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
			if (hProcess == NULL){ return false; } DWORD exitCode;
			if (!GetExitCodeProcess(hProcess, &exitCode)){ CloseHandle(hProcess); return false; }
			CloseHandle(hProcess); return exitCode == STILL_ACTIVE; }
		static std::string GetExePath(int pid=-1){ wchar_t result[MAX_PATH]; DWORD len = GetModuleFileNameW(nullptr, result, MAX_PATH);
			if(len>0){ return _toUTF8(result, len); } return ""; }
		static ProcessInfo GetProcInfo(int pid){ ProcessInfo pinfo; pinfo.pid=-1; HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
			if(hSnap == NULL){ return pinfo; } PROCESSENTRY32 proc; proc.dwSize = sizeof(PROCESSENTRY32);
			if(Process32First(hSnap, &proc)){ do{ if((int)proc.th32ProcessID!=pid){ continue; } pinfo.name=proc.szExeFile; 
				pinfo.pid=proc.th32ProcessID; pinfo.threads=proc.cntThreads; pinfo.parent=proc.th32ParentProcessID;
				pinfo.priority=proc.pcPriClassBase; CloseHandle(hSnap); DoubleMap rinfo = system::RAM::usage(pinfo.pid); 
				pinfo.rss=rinfo["rss"]; pinfo.virt=rinfo["virt"]; return pinfo; }while(Process32Next(hSnap, &proc)); } return pinfo; }
		static Array<ProcessInfo> GetList(){ Array<ProcessInfo> plist; HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
			if(hSnap == NULL){ return plist; } PROCESSENTRY32 proc; proc.dwSize = sizeof(PROCESSENTRY32);
			if(Process32First(hSnap, &proc)){ do{ ProcessInfo pinfo; pinfo.name=proc.szExeFile; 
				pinfo.pid=proc.th32ProcessID; pinfo.threads=proc.cntThreads; pinfo.parent=proc.th32ParentProcessID;
				pinfo.priority=proc.pcPriClassBase; DoubleMap rinfo = system::RAM::usage(pinfo.pid); 
				pinfo.rss=rinfo["rss"]; pinfo.virt=rinfo["virt"]; plist.push(pinfo); }while(Process32Next(hSnap, &proc)); } CloseHandle(hSnap); return plist; }
		struct { HANDLE hRead;
			int read(Buffer* buff){ if(buff==NULL) return -1; if(buff->size()<buff->capacity()){ buff->resize(buff->capacity()); } DWORD rbytes=0; 
				if(ReadFile(hRead, &(*buff)[0], buff->size(), &rbytes, NULL)&&rbytes>0){ buff->resize(rbytes); } return rbytes; }
			void clear(){ CloseHandle(hRead); }
		} StdOut;
		#else
		inline static int getpid(){	return ::getpid(); }
		bool isrun() const { if(pid<=0){ return false; } if(::kill(pid, 0) == -1){ return false; } return true; }
		static std::string GetExePath(int pid=-1){ char result[PATH_MAX]; std::string path = (pid<0)?"/proc/self/exe":"/proc/" +dtos(pid)+"/exe";
			ssize_t count = readlink(path.c_str(), result, PATH_MAX); if(count!=-1){ return std::string(result, count); } return ""; }
		static ProcessInfo GetProcInfo(int pid){ ProcessInfo pinfo; pinfo.pid = -1; size_t pgsize = sysconf(_SC_PAGESIZE);
			std::string procPath = "/proc/"+dtos(pid)+"/stat"; std::ifstream stfile(procPath.c_str()); if(!stfile.is_open()){ return pinfo; }
			String line; std::getline(stfile, line); stfile.close(); std::istringstream iss(line);
			iss >> pinfo.pid >> pinfo.name >> pinfo.state; pinfo.name = String(pinfo.name).slice(1,-1);
			int dummy; iss >> pinfo.parent >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy 
				>> dummy >> dummy >> dummy >> dummy >> dummy >> pinfo.priority >> pinfo.nice >> pinfo.threads
				>> dummy >> dummy >> pinfo.virt >> pinfo.rss; pinfo.rss = pinfo.rss*pgsize; return pinfo; }
		static Array<ProcessInfo> GetList(){ Array<ProcessInfo> plist;
			DIR* procDir = opendir("/proc"); if(!procDir){ return plist; } struct dirent* entry;
			while ((entry = readdir(procDir)) != nullptr){ if(entry->d_type != DT_DIR){ continue; } int pid = stoin(entry->d_name);
				if(pid > 0){ ProcessInfo pinfo = GetProcInfo(pid); if(pinfo.pid != -1){ plist.push(pinfo); } } } closedir(procDir); return plist; }
		#endif
		
		private:
			static int _run(const std::string& execpath, const ProcessOpts& opts=ProcessOpts(), Process* proc=NULL){ int pid=-1;
		#ifdef _WIN32
			std::string ecmd="cmd /c "+execpath; STARTUPINFOW si; PROCESS_INFORMATION pi; ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
			DWORD procFlag=CREATE_NO_WINDOW; //if(opts.detach<2){ si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = SW_HIDE; } - Hide GUI Window
			if(opts.detach==1){ procFlag=CREATE_NO_WINDOW|DETACHED_PROCESS; }else if(opts.detach>=2){ procFlag=CREATE_NEW_CONSOLE; }
			if(opts.getpid){ ecmd=execpath; } if(!CreateProcessW(NULL, &_toWStr(ecmd)[0], NULL, NULL, FALSE, procFlag, NULL, NULL, &si, &pi)){ return pid; }
			pid = pi.dwProcessId; CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
		#else
			if(opts.getpid){ posix_spawnattr_t attr; posix_spawnattr_init(&attr); if(opts.detach >= 1){
				#ifdef POSIX_SPAWN_SETSID
				posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
				#else
				pid_t p = fork(); if(p==0){ setsid(); }else if(p > 0){ return p; }else{ return -1; }
				#endif
				} Array<String> args = String(execpath).split(" ");
				std::vector<char*> argv; for(size_t i=0;i<args.size();i++){ argv.push_back((char*)args[i].c_str()); } 
				argv.push_back(nullptr); int status = posix_spawn(&pid, argv[0], NULL, &attr, &argv[0], environ);
				posix_spawnattr_destroy(&attr); if (status != 0){ print("posix_spawn failed: "); print(strerror(status)); print("\n"); } }
			else{ pid_t p = fork(); if(opts.detach >= 1){ setsid(); } if(p == -1){ return -1; } 
				else if(p == 0){ execl("/bin/sh", "sh", "-c", execpath.c_str(), (char *)0); exit(EXIT_FAILURE); } else { pid = p; } }
		#endif
			return pid; }
	};

	namespace Mouse {}
	namespace Keyboard {}
	
	namespace performance { static long long _start_usec=0;
		void start(){ _start_usec=GetTimestamp('u'); }
		double now(){ return (GetTimestamp('u')-_start_usec)/1000.0; } }
}

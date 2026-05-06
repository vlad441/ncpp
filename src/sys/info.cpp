#ifdef _WIN32
#include <psapi.h> //for ReadRegStr
#else
#include <sys/utsname.h> //for uname(&uinfo)
#endif
namespace ncpp{
	String execShell(const CString& cmd, Stream* stream=NULL);
namespace system {
	#ifdef _WIN32
	#if _WIN32_WINNT >= 0x0600 //Windows Vista
	long long getUptime(char type='s'){ return type=='m'?GetTickCount64():GetTickCount64()/1000; }
	#else
	long long getUptime(char type='s'){ return type=='m'?GetTickCount():GetTickCount()/1000; }
	#endif
	
	String ReadRegStr(HKEY hKeyRoot, const CString& path, const CString& valueName){
		HKEY hKey; LONG lRes = RegOpenKeyExA(hKeyRoot, path.c_str(), 0, KEY_READ, &hKey); if(lRes != ERROR_SUCCESS){ return ""; }
		char wbuff[512]; DWORD bSize = sizeof(wbuff); DWORD type = 0;
		lRes = RegQueryValueExW(hKey, _toWStr(valueName).c_str(), NULL, &type, (LPBYTE)wbuff, &bSize);
		if(lRes != ERROR_SUCCESS){ RegCloseKey(hKey); return ""; } RegCloseKey(hKey);

		if(type==REG_DWORD){ return dtos(Buffer(&wbuff[0], 4).readUInt32LE()); }
		if(type!=REG_SZ&&type!=REG_EXPAND_SZ){ return ""; } return _toUTF8((wchar_t*)wbuff, bSize/2); }
	StringMap os_info(){ StringMap sinfo; OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		sinfo["OS"]="Windows "; if(GetVersionEx(&osvi)){ sinfo["NT"]=dtos(osvi.dwMajorVersion)+"."+dtos(osvi.dwMinorVersion); 
			int nt=osvi.dwMajorVersion*10+osvi.dwMinorVersion; if(nt>=62){
				String OSName = ReadRegStr(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
				String OSVer = ReadRegStr(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentMajorVersionNumber");
				if(OSName.empty()){ sinfo["OS"]+=">=8"; }else{ sinfo["OS"]=OSName; } if(!OSVer.empty()){ sinfo["NT"]=OSVer; } }
			else if(nt==61){ sinfo["OS"]+="7"; } else if(nt==60){ sinfo["OS"]+="Vista"; } else if(nt==52){ sinfo["OS"]+="XP 64-bit Edition"; } 
			else if(nt==51){ sinfo["OS"]+="XP"; } else if(nt==50){ sinfo["OS"]+="2000"; } else { sinfo["OS"]+="(?)"; }
		}else{ sinfo["OS"]+="(-)"; } char buf[256]; DWORD size=256; GetComputerNameA(&buf[0],&size); 
		sinfo["Hostname"]=String(buf, size); return sinfo; }
	String GetKernelVer(){ return "(Windows)"; }
	void GetKernelVer(int v[3]){ memset(v, 0, sizeof(v[0])*3); }
	#else
	long long getUptime(char type='s'){ String line = fs::readFstLine("/proc/uptime"); line=line.split(" ")[0]; return type=='m'?stodn(line)*1000:stodn(line); }
	StringMap os_info(){ StringMap sinfo; struct utsname uinfo;
		if(uname(&uinfo)==0){ sinfo["OS"]=uinfo.sysname; sinfo["Kernel"]=uinfo.release; sinfo["Arch"]=uinfo.machine; 
			sinfo["Hostname"]=uinfo.nodename; }else{ sinfo["OS"]="Linux"; sinfo["Kernel"]="-"; }
		StringMap conf = fs::ConfigRead("/etc/os-release", true); if(conf.has("NAME")){ sinfo["OS"]=conf["NAME"]+" "+conf["VERSION_ID"]; return sinfo; }
		String str = fs::_readFile<String>("/etc/debian_version"); if(str!=""){ sinfo["OS"]="Debian "+str; sinfo["Package Manager"]="apt"; return sinfo; }
		str = fs::_readFile<String>("/etc/redhat-release"); if(str!=""){ sinfo["OS"]=str; sinfo["Package Manager"]="yum"; return sinfo; } return sinfo; }
	String GetKernelVer(){ struct utsname uinfo; uname(&uinfo); return uinfo.release; }
	void GetKernelVer(int v[3]){ Array<String> ver=GetKernelVer().split(".", 1, 3); for(int i=0;i<3;i++) v[i]=stoin(ver[i]); }
	#endif
	
	String getUptimeStr(){ String ss; size_t uptime=getUptime('s'); ss << dtos(uptime/86400) << " days, ";
		uptime %= 86400; int tmp = uptime/3600; ss<<(tmp<10?"0":"")<<tmp<<":"; uptime-=tmp*3600; tmp=uptime/60; ss<<(tmp<10?"0":"")<<tmp; 
		uptime-=tmp*60; ss<<":"<<(uptime<10?"0":"")<<uptime; return ss; }
	
	namespace CPU {
		#ifdef _WIN32
		String model(){ return ReadRegStr(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString"); }
		StringMap info(){ StringMap cpuinfo; cpuinfo["model"] = model(); DWORD bufferSize = 0;
			GetLogicalProcessorInformation(nullptr, &bufferSize); // Запрос размера буфера
			Array<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(bufferSize/sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
			if(!GetLogicalProcessorInformation(&buffer[0], &bufferSize)){ return cpuinfo; } int sockets=0, cores=0, threads=0;
			for (Array<SYSTEM_LOGICAL_PROCESSOR_INFORMATION>::iterator it = buffer.begin(); it != buffer.end(); ++it){
				SYSTEM_LOGICAL_PROCESSOR_INFORMATION& info = *it;
				if (info.Relationship == RelationProcessorCore){ cores++; DWORD mask = info.ProcessorMask;
					int threadCount = 0; while(mask){ threadCount += mask & 1; mask >>= 1; } threads += threadCount;
				}else if (info.Relationship == RelationProcessorPackage){ sockets++; } }
			cpuinfo["sockets"]=dtos(sockets); cpuinfo["cores"]=dtos(cores); cpuinfo["threads"]=dtos(threads); return cpuinfo; }
		#else
		String model(){ Array<String> lines = fs::readLines("/proc/cpuinfo"); 
			for(size_t indx=0;indx<lines.size();indx++){ 
				if(lines[indx].startsWith("model name")){ Array<String> splt=lines[indx].split(":"); return splt[1].trim(); } } return ""; }
		StringMap info(){ StringMap cpuinfo; Array<String> lines = fs::readLines("/proc/cpuinfo"); StringMap cpu0; size_t indx=0; int threads=1;
			for(;indx<lines.size();indx++){ if(lines[indx]==""){ break; } Array<String> splt=lines[indx].split(":"); cpu0[splt[0].trim()]=splt[1].trim(); }
			HashSet<String> ids; ids.insert(cpu0["physical id"]); for(;indx<lines.size();indx++){ 
				if(lines[indx].startsWith("processor")){ threads+=1; }else if(lines[indx].startsWith("physical id")){ 
					Array<String> splt=lines[indx].split(":"); ids.insert(splt[1].trim()); } }
			cpuinfo["model"]=cpu0["model name"]; cpuinfo["sockets"]=dtos(ids.size()); cpuinfo["cores_per_socket"]=cpu0["cpu cores"];
			cpuinfo["cores"]=dtos(stoin(cpu0["cpu cores"])*ids.size()); cpuinfo["threads"]=dtos(threads); return cpuinfo; }
		#endif
			
		struct CPUTime { long long user; long long nice; long long system; long long idle; long long iowait; long long irq; long long softirq;
			CPUTime() : user(0), nice(0), system(0), idle(0), iowait(0), irq(0), softirq(0){}
		}; Array<CPUTime> PrevLoadStats; CPUTime PrevAvgLoad, PrevUsage;
		#ifdef _WIN32
		CPUTime getTotalTime(){ CPUTime avgtime; FILETIME idleTime, kernelTime, userTime;
			if(!GetSystemTimes(&idleTime, &kernelTime, &userTime)){ return avgtime; } avgtime.idle = _FtToUnixTime(idleTime, 'm'); 
			avgtime.system = _FtToUnixTime(kernelTime, 'm')-avgtime.idle; avgtime.user = _FtToUnixTime(userTime, 'm'); return avgtime; }
		//TODO: getCoresTime for Windows
		Array<CPUTime> getCoresTime(){ return Array<CPUTime>(); }
		CPUTime getUsageTime(int pid=-1){ CPUTime stats; HANDLE hProc; if(pid<0){ hProc = GetCurrentProcess(); }
			else{ hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid); if(!hProc){ Except("getUsageTime: Failed to open process.\n"); return stats; } }
			FILETIME creationTime, exitTime, kernelTime, userTime; GetProcessTimes(hProc, &creationTime, &exitTime, &kernelTime, &userTime); CloseHandle(hProc);
			stats.system=_FtToUnixTime(kernelTime, 'm'); stats.user=_FtToUnixTime(userTime, 'm'); 
			stats.idle=GetTimestamp('m')-_FtToUnixTime(creationTime, 'm')-stats.system-stats.user; return stats; }
		#else	
		CPUTime _parseCPUStats(const CString& line){ CPUTime stats; Array<String> splited = line._splitTokens<String>(8);
			stats.user = stolln(splited[1]); stats.nice = stolln(splited[2]); stats.system = stolln(splited[3]); stats.idle = stolln(splited[4]); 
			stats.iowait = stolln(splited[5]); stats.irq = stolln(splited[6]); stats.softirq = stolln(splited[7]); return stats; }
		CPUTime getTotalTime(){ CPUTime avgtime; String line = fs::readFstLine("/proc/stat"); if(!line.startsWith("cpu ")){ return avgtime; } return _parseCPUStats(line); }
		Array<CPUTime> getCoresTime(){ Array<CPUTime> CoresTime; Array<String> lines = fs::readLines("/proc/stat"); for(size_t i=1; i<lines.size();i++){ 
			if(!lines[i].startsWith("cpu")){ continue; } CoresTime.push(_parseCPUStats(lines[i])); } return CoresTime; }
		CPUTime getUsageTime(int pid = -1){ CPUTime stats; long ticksPerSec = sysconf(_SC_CLK_TCK); if(ticksPerSec == 0){ return stats; }
			String statPath = (pid<0) ? "/proc/self/stat" : "/proc/" + dtos(pid) + "/stat";
			String line=fs::_readFile<String>(statPath); Array<String> splited = line.splitTokens();
			stats.user = stolln(splited[13]); stats.system = stolln(splited[14]); long starttime = stolln(splited[21]);
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
		ULLMap info(){ ULLMap raminfo;
		#ifdef _WIN32
			MEMORYSTATUSEX statex; statex.dwLength = sizeof(statex);
			if(!GlobalMemoryStatusEx(&statex)){ Except("Failed to get memory status\n"); return ULLMap(); }
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
						
		ULLMap usage(int pid=-1){ ULLMap uinfo;
		#ifdef _WIN32
			PROCESS_MEMORY_COUNTERS pmc; if(pid>0){ HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
				if(hProc==NULL||!GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))){ CloseHandle(hProc); return uinfo; }
				CloseHandle(hProc); }else{ if(!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))){ return uinfo; } }
			uinfo["rss"] = pmc.WorkingSetSize; uinfo["swap"] = pmc.PagefileUsage; uinfo["virt"] = uinfo["rss"]+uinfo["swap"];
		#else
			String line; if(pid>0){ line = fs::readFstLine("/proc/"+dtos(pid)+"/statm"); }else{ line = fs::readFstLine("/proc/self/statm"); }
			Array<String> splited = line.split(' '); size_t pgsize = sysconf(_SC_PAGESIZE);
			uinfo["virt"] = (size_t)stolln(splited[0])*pgsize; uinfo["rss"] = (size_t)stolln(splited[1])*pgsize;
		#endif
			return uinfo; }
		size_t usage_rss(int pid=-1){ return usage(pid)["rss"]; }
		String strUsage(const ULLMap& rinf=info()){ return dtos(rinf.at("used")/1073741824.0, 2)+" GB/"+dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
		String strTotal(const ULLMap& rinf=info()){ return dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
	}
	namespace Disk { struct DiskInfo { String model; unsigned long long used, size; String label; };
		#ifdef _WIN32
		String _norm_spaces(const CString& input){ String output; bool inSpace = false; 
			for(size_t i = 0; i < input.length(); ++i){
				if(input[i] == ' ' || input[i] == '\t'){ if(!inSpace) { output += ' '; inSpace = true; } } 
				else { output += input[i]; inSpace = false; } } return output; }

		Array<DiskInfo> drives(){ Array<String> out = _norm_spaces(execShell("wmic diskdrive get Caption,Size")).split("\r\n");
			Array<DiskInfo> list; for(size_t i=1;i<out.size(); i++){ out[i]=out[i].trim(); if(out[i].empty()) continue; 
				Array<String> tmp = out[i].split(" "); DiskInfo dinfo; dinfo.model=tmp[0]; dinfo.used=0;
				dinfo.size=(tmp.size()>1)?stolln(tmp[1]):0; list.push(dinfo); } return list; }
			
		Array<DiskInfo> logical(){ Array<String> out = _norm_spaces(execShell("wmic logicaldisk get DeviceID,FreeSpace,Size,VolumeName")).split("\n");
			Array<DiskInfo> list; for(size_t i=1;i<out.size(); i++){ out[i]=out[i].trim(); if(out[i].empty()) continue; 
				Array<String> tmp = out[i].split(" "); DiskInfo dinfo; dinfo.model=tmp[0]; dinfo.used=0; dinfo.label=(tmp.size()>3)?tmp[3]:"";
				dinfo.size=(tmp.size()>2)?stolln(tmp[2]):0; dinfo.used=(tmp.size()>2)?dinfo.size-stolln(tmp[1]):0; list.push(dinfo); } return list; }
		#else
		#include <sys/statvfs.h>
		#include <mntent.h>
		Array<DiskInfo> drives(){ Array<String> dir = fs::readDir("/sys/block"); Array<DiskInfo> list;
			for(size_t i=0;i<dir.size(); i++){ if(dir[i]=="."||dir[i]== ".." || dir[i].find("loop") == 0 || dir[i].find("ram") == 0) continue;
				DiskInfo dinfo; dinfo.model=fs::readFstLine("/sys/block/"+dir[i]+"/device/model").trim(); dinfo.label=dir[i]; dinfo.used=0;
				dinfo.size=stolln(fs::readFstLine("/sys/block/"+dir[i]+"/size").trim())*512; list.push(dinfo); } return list; }
					
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
		
		ULLMap info(){ ULLMap dinfo; Array<DiskInfo> d1=drives(); dinfo["drives"]=d1.size(); double tsize=0; 
			for(size_t i=0;i<d1.size(); i++){ tsize+=d1[i].size; } dinfo["total"]=tsize; Array<DiskInfo> d2=logical();
			tsize=0; for(size_t i=0;i<d2.size(); i++){ tsize+=d2[i].used; } dinfo["used"]=tsize; return dinfo; }
		String strUsage(const ULLMap& rinf=info()){ return dtos(rinf.at("used")/1073741824.0, 2)+" GB/"+dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
		String strTotal(const ULLMap& rinf=info()){ return dtos(rinf.at("total")/1073741824.0, 2)+" GB"; }
	}
	namespace GPU {
		#ifdef _WIN32
		Array<String> list(){ HashSet<String> GPUs;	DISPLAY_DEVICE dd; ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); int deviceIndex = 0;
			while(EnumDisplayDevices(NULL, deviceIndex, &dd, 0)){ GPUs.insert(dd.DeviceString); deviceIndex++;
			ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); } return GPUs.keys(); }
		String model(){ DISPLAY_DEVICE dd; ZeroMemory(&dd, sizeof(dd)); dd.cb = sizeof(dd); EnumDisplayDevices(NULL, 0, &dd, 0); return dd.DeviceString; }
		#else
		Array<String> list(){ HashSet<String> GPUs; Array<String> out = execShell("lspci | grep -E 'VGA|3D|Display'").split("\n");
			for(size_t i=0;i<out.size();i++){ Array<String> arr=out[i].split(": "); if(arr.size()>1){ GPUs.insert(arr[1].trim()); } } return GPUs.keys(); }
		String model(){ Array<String> lst=list(); return lst.size()>0?lst[0]:""; }
		#endif		
	}
	
	StringMap info(){ StringMap sysinfo=os_info(); sysinfo["Memory"]=RAM::strTotal(); sysinfo["CPU"]=CPU::model(); 
		sysinfo["GPU"]=GPU::model(); sysinfo["Uptime"]=getUptimeStr(); //neofetch
		return sysinfo; }
} 
namespace sys = system; }

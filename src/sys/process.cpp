namespace ncpp{
	//String execShell(const CString& cmd, Stream* stream=NULL);
	String execShell(const CString& cmd, Stream* stream){ String ss; char buf[1024]; //int execpid=-1;
	#ifdef _WIN32
		String ecmd="cmd /c "+cmd; PROCESS_INFORMATION pi; STARTUPINFOW si; ZeroMemory(&si, sizeof(STARTUPINFO)); SECURITY_ATTRIBUTES sa; 
		HANDLE newstdout,hRead; sa.lpSecurityDescriptor = NULL; sa.nLength = sizeof(SECURITY_ATTRIBUTES); sa.bInheritHandle = true;
		if(!CreatePipe(&hRead,&newstdout,&sa,0)){ return "(CreatePipe Error)"; } GetStartupInfoW(&si); 
		si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW; si.hStdOutput = newstdout; si.hStdError = newstdout; si.wShowWindow = SW_HIDE;
		if(!CreateProcessW(NULL, (LPWSTR)_toWStr(ecmd).c_str(), NULL,NULL,true,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))
		{ CloseHandle(newstdout); CloseHandle(hRead); return "(CreateProcess Error)"; } unsigned long bread; String str;
			
		while(ReadFile(hRead, buf, sizeof(buf), &bread, NULL) && bread>0){
			String chunk = _OEMtoUTF8(buf, bread); if(stream==NULL){ str+=chunk; }else{ (*stream)<<chunk; } }
			
		CloseHandle(pi.hThread); CloseHandle(pi.hProcess); CloseHandle(newstdout); CloseHandle(hRead); return str;
	#else
		String ecmd="/bin/sh -c \""+cmd+" 2>&1\""; FILE* pipe = popen(ecmd.c_str(), "r"); if(!pipe){ Except("popen() failed!"); return ""; }
		while(fgets(buf, sizeof(buf), pipe)!=NULL){ if(stream==NULL){ ss << buf; }else{ (*stream) << buf; } } pclose(pipe);
	#endif
		return ss; }
	
	#ifdef _WIN32
	#include <tlhelp32.h>
	#endif
	struct ProcessInfo { int pid; int threads; int parent; int priority; String name;
		ProcessInfo() : pid(-1), threads(0), parent(0), priority(0), name(""), state('-'), nice(0), virt(0), rss(0){}
		char state; int nice; size_t virt; size_t rss; };
	struct ProcessOpts { char detach; bool getpid; bool StdOut; bool StdIn;
		ProcessOpts(char detach1=1, bool getpid1=true, bool stdout1=false, bool stdin1=false) : 
			detach(detach1), getpid(getpid1), StdOut(stdout1), StdIn(stdin1){} };
	
	struct Process { int pid;
		Process() : pid(0){} Process(int numb) : pid(numb){} ~Process(){}
		static Process start(const CString& execpath, const ProcessOpts& opts){ Process proc(_run(execpath, opts)); return proc; }
		static Process start(const CString& execpath, char detach=1){ Process proc(_run(execpath, ProcessOpts(detach))); return proc; }
		static bool exec(const CString& execcmd, char detach=0){ return _run(execcmd, ProcessOpts(detach, false))>0?true:false; }
		
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
		static String GetExePath(int pid=-1){ wchar_t result[MAX_PATH]; DWORD len = GetModuleFileNameW(nullptr, result, MAX_PATH);
			if(len>0){ return _toUTF8(result, len); } return ""; }
		static ProcessInfo GetProcInfo(int pid){ ProcessInfo pinfo; pinfo.pid=-1; HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
			if(hSnap == NULL){ return pinfo; } PROCESSENTRY32 proc; proc.dwSize = sizeof(PROCESSENTRY32);
			if(Process32First(hSnap, &proc)){ do{ if((int)proc.th32ProcessID!=pid){ continue; } pinfo.name=proc.szExeFile; 
				pinfo.pid=proc.th32ProcessID; pinfo.threads=proc.cntThreads; pinfo.parent=proc.th32ParentProcessID;
				pinfo.priority=proc.pcPriClassBase; CloseHandle(hSnap); ULLMap rinfo = system::RAM::usage(pinfo.pid); 
				pinfo.rss=rinfo["rss"]; pinfo.virt=rinfo["virt"]; return pinfo; }while(Process32Next(hSnap, &proc)); } return pinfo; }
		static Array<ProcessInfo> GetList(){ Array<ProcessInfo> plist; HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
			if(hSnap == NULL){ return plist; } PROCESSENTRY32 proc; proc.dwSize = sizeof(PROCESSENTRY32);
			if(Process32First(hSnap, &proc)){ do{ ProcessInfo pinfo; pinfo.name=proc.szExeFile; 
				pinfo.pid=proc.th32ProcessID; pinfo.threads=proc.cntThreads; pinfo.parent=proc.th32ParentProcessID;
				pinfo.priority=proc.pcPriClassBase; ULLMap rinfo = system::RAM::usage(pinfo.pid); 
				pinfo.rss=rinfo["rss"]; pinfo.virt=rinfo["virt"]; plist.push(pinfo); }while(Process32Next(hSnap, &proc)); } CloseHandle(hSnap); return plist; }
		struct { HANDLE hRead;
			int read(Buffer* buff){ if(buff==NULL) return -1; if(buff->size()<buff->capacity()){ buff->resize(buff->capacity()); } DWORD rbytes=0; 
				if(ReadFile(hRead, &(*buff)[0], buff->size(), &rbytes, NULL)&&rbytes>0){ buff->resize(rbytes); } return rbytes; }
			void clear(){ CloseHandle(hRead); }
		} StdOut;
		#else
		inline static int getpid(){	return ::getpid(); }
		bool isrun() const { if(pid<=0){ return false; } if(::kill(pid, 0) == -1){ return false; } return true; }
		static String GetExePath(int pid=-1){ char result[PATH_MAX]; String path = (pid<0)?"/proc/self/exe":"/proc/" +dtos(pid)+"/exe";
			ssize_t count = readlink(path.c_str(), result, PATH_MAX); if(count!=-1){ return String(result, count); } return ""; }
		static ProcessInfo GetProcInfo(int pid){ ProcessInfo pinfo; pinfo.pid = -1; size_t pgsize = sysconf(_SC_PAGESIZE);
			String line=fs::_readFile<String>("/proc/"+dtos(pid)+"/stat"); Array<String> spltd = line.splitTokens(24);
			if(spltd.size()<24){ Except("(!) GetProcInfo err: read proc data: "+dtos(spltd.size())+"/24\n"); }
			pinfo.pid = stoin(spltd[0]); pinfo.name = spltd[1].slice(1,-1); pinfo.state = *spltd[2].c_str();
			pinfo.parent = stoin(spltd[3]); pinfo.priority = stoin(spltd[17]); pinfo.nice = stoin(spltd[18]);
			pinfo.threads = stoin(spltd[19]); pinfo.virt = stolln(spltd[22]); pinfo.rss = stolln(spltd[23]); pinfo.rss = pinfo.rss*pgsize; return pinfo; }
		static Array<ProcessInfo> GetList(){ Array<ProcessInfo> plist;
			DIR* procDir = opendir("/proc"); if(!procDir){ return plist; } struct dirent* entry;
			while ((entry = readdir(procDir)) != NULL){ if(entry->d_type != DT_DIR){ continue; } int pid = stoin(entry->d_name);
				if(pid > 0){ ProcessInfo pinfo = GetProcInfo(pid); if(pinfo.pid != -1){ plist.push(pinfo); } } } closedir(procDir); return plist; }
		#endif
		
		private:
		#ifndef _WIN32
			static Array<String> _ParseArgs(const CString& argstr){ Array<String> args; String str; bool in_quotes = false;
				for(size_t i=0; i<argstr.size(); ++i){ char c = argstr[i];
					if(c == '"'){ in_quotes = !in_quotes; continue; }
					if(c == ' '&&!in_quotes){ if(!str.empty()){ args.push(str); str.clear(); } }else{ str += c; } }
				if(!str.empty()){ args.push(str); } return args; }
		#endif
			static int _run(const CString& execpath, const ProcessOpts& opts=ProcessOpts(), Process* proc=NULL){ int pid=-1;
			#ifdef _WIN32
				String ecmd="cmd /c "+execpath; STARTUPINFOW si; PROCESS_INFORMATION pi; ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
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
					} Array<String> args = _ParseArgs(execpath); Array<char*> argv; for(size_t i=0;i<args.size();i++){ argv.push((char*)args[i].c_str()); }
					argv.push(NULL); int status = posix_spawn(&pid, argv[0], NULL, &attr, argv.data(), environ);
					posix_spawnattr_destroy(&attr); if (status != 0){ Except("(!) posix_spawn failed: "+String(strerror(status))+"\n"); 
						print("(#DEBUG) _run path: "); print(execpath); print("\n");
						print("(#DEBUG) _run array:"); print(args); print("\n");
					} }
				else{ pid_t p = fork(); if(opts.detach >= 1){ setsid(); } if(p == -1){ return -1; } 
					else if(p == 0){ execl("/bin/sh", "sh", "-c", execpath.c_str(), (char *)0); exit(EXIT_FAILURE); } else { pid = p; } }
			#endif
				return pid; }
	};
}

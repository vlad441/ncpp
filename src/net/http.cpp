namespace ncpp{ namespace http{ HashMap<int, String> ErrCodes;
	void _initErrCodes(){ ErrCodes[200]="OK"; ErrCodes[301]="Moved Permanently"; ErrCodes[302]="Moved Temporarily"; 
		ErrCodes[403]="Forbidden"; ErrCodes[404]="Not Found"; ErrCodes[413]="Payload Too Large"; ErrCodes[431]="Request Header Fields Too Large"; 
		ErrCodes[500]="Internal Server Error"; ErrCodes[501]="Not Implemented"; ErrCodes[504]="Gateway Timeout"; }
	struct Res; Buffer RespComposer(Res& opts);
	struct HttpMsg { StringMap headers; Buffer body; };
	struct Req : HttpMsg { float hver; String method; String url; bool nobody; Req() : hver(1.1), url("/"), nobody(false){}
		String cout() const { String ss; ss << "Request: " << this->url << "\nMethod: " << method
			<< "\n  === Headers ===  " << this->headers.cout() << "\n  === Body ===  \n" << this->body << "\n\n"; return ss; }
	};
	struct Res : HttpMsg { float hver; int status; TCPSocket* socket; bool ttfb; Res() : hver(1.1), status(0), socket(NULL), ttfb(false){}
		Res(TCPSocket& sock) : hver(1.1), status(0), socket(&sock), ttfb(false){}
		void write(const char* ptr, size_t len){ if(socket==NULL) return; if(!ttfb){ socket->send(RespComposer(*this)); ttfb=true; } socket->send(ptr, len); }
		void write(const char* ptr){ write(ptr, strlen(ptr)); }
		void write(const Buffer& data){ write((const char*)data.data(), data.size()); }
		void end(const Buffer& data=Buffer()){ if(socket==NULL) return; Buffer resp; 
			if(!ttfb){ this->body+=data; resp=RespComposer(*this); }else{ resp=data; } socket->send(resp);
			if(hver<=1.0||headers["connection"]=="close") socket->destroy(); }
		bool ok(){ return status < 300 && status >= 200; }
		bool hasLength(){ return headers.has("content-length")||headers.has("transfer-encoding"); }
		void Redirect(const CString& url){ status=301; headers["location"]=url; end(); }
		void SendErr(int status1){ status=status1; end(); }
		String cout() const { String ss("Response Headers:"); ss << this->headers.cout() << "\n  === Body ===  \n" << this->body << "\n\n"; return ss; }
	};
	
	IPAddr splitIpPort(const CString& ipstr){ IPAddr result; result.ip=""; result.port=0; size_t closingIndx = ipstr.find("]");
		if (ipstr[0] == '[' && closingIndx != NPOS) { // IPv6 с портом
			result.ip = ipstr.substr(1, closingIndx - 1); size_t colonPos = ipstr.find(":", closingIndx);
			if(colonPos != NPOS){ result.port = stoin(ipstr.substr(colonPos + 1)); }
		} else { size_t colonIndx = ipstr.find(":"); if (colonIndx != NPOS){
				result.ip = ipstr.substr(0, colonIndx); result.port = stoin(ipstr.substr(colonIndx + 1)); } 
			else { result.ip = ipstr; } } return result; }
			
	HttpMsg HttpParse(const Buffer& rawreq, size_t pos=NPOS){ HttpMsg parsed; if(pos==NPOS){ pos = rawreq.indexOf("\r\n")+2; }
		size_t headersEnd = rawreq.indexOf("\r\n\r\n"); if(headersEnd==NPOS){ return parsed; } size_t lineEnd; String currKey=""; 
		while(pos<=headersEnd){ lineEnd = rawreq.indexOf("\r\n", pos); if(lineEnd == pos){ pos += 2; break; } if(lineEnd==NPOS){ break; }
			String hline = rawreq.slice(pos, lineEnd).toString(); size_t dPos = hline.indexOf(": ");
			if(dPos > 0){ if(hline.startsWith("\t") || hline.startsWith(" ")){ parsed.headers[currKey]+="\r\n "+hline.trim(); }else{  
				currKey = hline.slice(0, dPos).toLowerCase(); parsed.headers[currKey] = hline.slice(dPos+2); } } pos = lineEnd + 2;
		} parsed.body = rawreq.slice(pos+2); return parsed; }
			
	Req ReqParse(const Buffer& rawreq){ Req parsed; size_t lineEnd = rawreq.indexOf("\r\n"); if(lineEnd==NPOS) return parsed;
		Array<String> statusLn = String((char*)rawreq.data(), lineEnd).splitTokens(3);
		parsed.method = statusLn[0]; parsed.url = statusLn[1]; parsed.hver=stofn(statusLn[2].split('/')[1]);
		HttpMsg msg = HttpParse(rawreq, lineEnd+2); parsed.headers=msg.headers; parsed.body=msg.body; return parsed; }
			
	Res RespParse(const Buffer& rawreq){ Res parsed; size_t lineEnd = rawreq.indexOf("\r\n"); if(lineEnd==NPOS) return parsed; 
		Array<String> statusLn = String((char*)rawreq.data(), lineEnd).splitTokens(3);
		parsed.hver=stofn(statusLn[0].split('/')[1]); parsed.status = stoin(statusLn[1]);
		HttpMsg msg = HttpParse(rawreq, lineEnd+2); parsed.headers=msg.headers; parsed.body=msg.body; return parsed; }
	
	Buffer QueryComposer(const CString& host, const Req& opts){ Buffer query; String headers; headers << (opts.method.empty() ? "GET" : opts.method)
			<<" "<< opts.url <<" HTTP/"<<dtos(opts.hver, 1)<<"\r\n"; if(!host.empty()){ headers << "Host: " << host << "\r\n"; }
		for(StringMap::const_iterator it = opts.headers.begin(); it != opts.headers.end(); ++it){ headers << it->first + ": " + it->second + "\r\n"; }
		if(!opts.headers.has("accept")){ headers<<"Accept: */*\r\n"; } if(!opts.headers.has("accept-encoding")){ headers<<"Accept-Encoding: identity\r\n"; }
		if(!opts.headers.has("user-agent")){ headers<<"User-Agent: Mozilla/5.0\r\n"; } if(!opts.headers.has("connection")){ headers<<"Connection: close\r\n"; } headers << "\r\n";
		query=headers; if(!opts.body.empty()){ query.concat(opts.body); } return query; }
		
	Buffer RespComposer(Res& opts){ if(opts.status<=0) opts.status=200; String headers; headers <<"HTTP/"<<dtos(opts.hver, 1)<< " " << opts.status
			<< " " << (ErrCodes.has(opts.status)?ErrCodes[opts.status]:"Code") << "\r\n";
		for(StringMap::const_iterator it = opts.headers.begin(); it != opts.headers.end(); ++it){ headers << it->first + ": " + it->second + "\r\n"; }
		if(!opts.headers.has("content-type")){ headers<<"Content-Type: text/html\r\n"; }
		if(!opts.hasLength()){ headers<<"Content-Length: "<<opts.body.size()<<"\r\n"; }
		if(!opts.headers.has("connection")){ headers<<"Connection: close\r\n"; } headers<<"\r\n"; Buffer resp=headers; 
		if(opts.body.size()>0) resp+=opts.body; return resp; }
		
	Buffer chunkedStreamParser(Buffer& rawbuff, unsigned int& chunkSize, bool& is_end){ Buffer chunk;
		while(!rawbuff.empty()){
			if(chunkSize == 0){ size_t chunkEndIndx = rawbuff.indexOf("\r\n"); if(chunkEndIndx==NPOS){ return chunk; }
				chunkSize = Buffer::htoi(rawbuff.slice(0, chunkEndIndx).toString().c_str()); 
				if(chunkSize == 0){ is_end=true; return chunk; } rawbuff = rawbuff.slice(chunkEndIndx+2); }
			if(rawbuff.size() < chunkSize+2){ return chunk; } // Если данных меньше, чем размер чанка, выходим и ждем дополнительные данные
			chunk+=rawbuff.slice(0, chunkSize); rawbuff = rawbuff.slice(chunkSize+2); chunkSize=0;
		} return chunk; }
		
	template <typename S>	
	Res request(S& socket, const CString& host, int port, Req& params){
		if(!socket.connected&&!socket.connect(host, port)){ Res response; response.status = 0; return response; }
		Buffer rawreq = QueryComposer(host, params);
		
		socket.send(rawreq); Buffer rawresp, chunk; while(true){ chunk=socket.recv(); rawresp+=chunk;
			if(rawresp.indexOf("\r\n\r\n")>=0||chunk.size()<=0){ break; } }
		Res resp = RespParse(rawresp); size_t datasize[2]; datasize[0]=resp.body.size(); datasize[1]=0; bool chunked=false; if(params.nobody){ return resp; }
		else if(resp.headers.has("content-length")){ datasize[1]=(size_t)stodn(resp.headers["content-length"]); }
		else if(resp.headers["transfer-encoding"]=="chunked"){ chunked=true; }
		struct { Buffer buff; unsigned int chunkSize; bool is_end;}ch_data; 
		if(chunked){ ch_data.chunkSize=0; ch_data.is_end=false; ch_data.buff=resp.body; datasize[0]=0; resp.body.clear(); }
		while(datasize[0]<datasize[1]||chunked){ chunk=socket.recv(); if(!chunked){ resp.body+=chunk; }
			else{ ch_data.buff+=chunk; resp.body+=chunkedStreamParser(ch_data.buff, ch_data.chunkSize, ch_data.is_end); if(ch_data.is_end){ break; } }
			datasize[0]+=chunk.size(); if(chunk.size()<=0){ break; } } return resp; }
	template <typename S>
	Res request(S& socket, const CString& host, int port=80){ Req params; return request(socket, host, port, params); }
	
	Res fetchd(String url, Req& params){ String host, path = "/"; int port = 80;
		bool useTLS = false; if(url.find("https://") == 0){ useTLS = true; port = 443; } if(useTLS){ print("(!) fetchd: https protocol unsupported"); return Res(); }
		size_t uindx = url.find("://"); url = (uindx != NPOS) ? url.slice(uindx + 3) : url; uindx = url.find("/");
		if(uindx != NPOS){ params.url = url.slice(uindx); url = url.slice(0, uindx); }else{ params.url = "/"; }

		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; } TCPSocket socket; return request(socket, addr.ip, addr.port, params); }
	Res fetchd(const CString& url){ Req params; return fetchd(url, params); }
	
	bool SaveStream(String url, const CString& fpath, String* err=NULL){ String host, path = "/"; int port = 80;
		if(url.find("https://") == 0){ if(err){ *err="https protocol unsupported"; } return false; }
		size_t uindx = url.find("://"); url = (uindx != NPOS) ? url.slice(uindx + 3) : url; uindx = url.find("/"); Req params; params.nobody=true;
		if(uindx != NPOS){ params.url = url.slice(uindx); url = url.slice(0, uindx); }else{ params.url = "/"; }
		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; } TCPSocket socket; 
		Res resp = request(socket, addr.ip, addr.port, params); if(!resp.ok()){ if(err){ *err="statusCode: "+dtos(resp.status); } return false; } Buffer chunk(65536); bool chunked=false;
		
		FStream f(fpath, FStream::IO_WRITE); if(f.destroyed){ if(err){ *err="open file err: "+fpath; } return false; } size_t datasize[2]; datasize[0]=resp.body.size(); datasize[1]=0;
		if(resp.headers.has("content-length")){ datasize[1]=(size_t)stodn(resp.headers["content-length"]); }
		else if(resp.headers["transfer-encoding"]=="chunked"){ chunked=true; }
		struct { Buffer buff; unsigned int chunkSize; bool is_end;}ch_data; 
		if(chunked){ ch_data.chunkSize=0; ch_data.is_end=false; ch_data.buff=resp.body; datasize[0]=0; resp.body.clear(); }else{ f.write(resp.body); }
		while((datasize[0]<datasize[1]||chunked)&&socket.recv(&chunk)>0){ if(!chunked){ f.write(chunk); }
			else{ ch_data.buff+=chunk; f.write(chunkedStreamParser(ch_data.buff, ch_data.chunkSize, ch_data.is_end)); if(ch_data.is_end){ break; } }
			datasize[0]+=chunk.size(); } return true; } //std::cout << "(SaveStream: BODY chunk size): " << datasize[0] << "/" << datasize[1] << " | chunked: " << chunked << std::endl;
			
	String urlDecoder(const CString& str){ String decoded; //decoded.reserve(str.size());
		for (size_t i = 0; i < str.size(); ++i){
			if(str[i] == '%' && i+2 < str.size()){ decoded.push((char)Buffer::htoc(&str[i+1])); i+=2; }
			else if(str[i] == '+'){ decoded.push(' '); }
			else{ decoded.push(str[i]); } } return decoded; }
			
	String GetMIMEType(const CString& ext){
		if(ext=="html"||ext=="css"||ext=="xml"||ext=="csv"||ext=="htm") return "text/"+ext; if(ext=="txt") return "text/plain"; //text
		if(ext=="js"||ext=="mjs") return "text/javascript"; if(ext=="svg") return "image/svg+xml";
		if(ext=="png"||ext=="jpg"||ext=="jpeg"||ext=="gif"||ext=="webp"||ext=="bmp"||ext=="avif"||ext=="tiff") return "image/"+ext; //image
		if(ext=="mp3"||ext=="aac"||ext=="opus"||ext=="wav") return "audio/"+ext; //audio 
		if(ext=="ogg"||ext=="oga") return "audio/ogg"; if(ext=="weba") return "audio/webm"; if(ext=="mid"||ext=="midi") return "audio/midi";
		if(ext=="mp4"||ext=="mpeg"||ext=="webm"||ext=="3gp") return "video/"+ext; if(ext=="avi") return "video/x-msvideo"; if(ext=="ts") return "video/mp2t"; //video
		if(ext=="json"||ext=="zip"||ext=="pdf"||ext=="rtf"||ext=="xml"||ext=="ogx") return "application/"+ext; //application
		if(ext=="gz") return "application/gzip"; if(ext=="tar") return "application/x-tar"; if(ext=="rar") return "application/vnd.rar";
		return "application/octet-stream"; }
			
	void SendFile(const CString& fpath, Res& res){ String fext = fpath.slice(fpath.lastIndexOf('.')+1); 
		res.headers["content-type"]=GetMIMEType(fext)+"; charset=utf-8";
		if(res.headers["content-type"]=="application/octet-stream"){ res.headers["Content-Disposition"]="attachment"; } 
		Buffer file=fs::readFile(fpath); if(file.size()<=0){ res.status=404; res.end("404 File Not Found"); return; } res.end(file); }
	//SendFileStream(const CString& fpath, Res res);
			
	void AutoIndex(const CString& dirpath, Res& res){ String path = urlDecoder(dirpath);
		//print("(#DEBUG) http::AutoIndex | path = "); print(path); print("\n");
		if(!fs::exists(path)){ res.status=422; res.end("422 Incorrect Path"); return; }
		if(!fs::isDir(path)){ SendFile(path, res); return; }
		
		Array<String> files = fs::readDir(path); String fileList = ""; 
		for(size_t i=0; i<files.size(); i++){ fileList << "<li><a href=\"/" << path << "/" << files[i] << "\">" << files[i] << "</a></li>"; }
		
		String htmlContent = "<html><head><title>AutoIndex of "+path+"</title></head>\n<body>"\
		"<h3>AutoIndex of "+path+"</h3><hr><pre><ul>"+fileList+"</ul><hr></pre>\nPowered by <a href=\"http://ncpp.art\">ncpp</a></body></html>"; res.end(htmlContent);
	 }
} }

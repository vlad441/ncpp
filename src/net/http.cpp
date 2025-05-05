namespace ncpp{ namespace http{ HashMap<int, std::string> ErrCodes;
	void _initErrCodes(){ ErrCodes[200]="OK"; ErrCodes[301]="Moved Permanently"; ErrCodes[302]="Moved Temporarily"; 
		ErrCodes[403]="Forbidden"; ErrCodes[404]="Not Found"; ErrCodes[413]="Payload Too Large"; ErrCodes[431]="Request Header Fields Too Large"; 
		ErrCodes[500]="Internal Server Error"; ErrCodes[501]="Not Implemented"; ErrCodes[504]="Gateway Timeout"; }
	struct Res; Buffer RespComposer(Res& opts);
	struct HttpMsg { StringMap headers; Buffer body; };
	struct Req : HttpMsg { float hver; std::string method; std::string url; bool nobody; Req() : hver(1.1), nobody(false){} 
		std::string cout() const { std::stringstream ss; ss << "Request: " << this->url << "\nMethod: " << method
			<< "\n  === Headers ===  " << this->headers.cout() << "\n  === Body ===  \n" << this->body << "\n\n"; return ss.str(); }
	};
	struct Res : HttpMsg { float hver; int status; TCPSocket* socket; Res() : hver(1.1), status(0), socket(NULL){}
		Res(TCPSocket& sock) : hver(1.1), status(0), socket(&sock){}
		void write(const Buffer& data){ if(socket==NULL) return; 
			if(status<=0){ socket->send(RespComposer(*this)+data); }else{ socket->send(data); } }
		void end(const Buffer& data=Buffer()){ if(socket==NULL) return; Buffer resp; 
			if(status<=0){ this->body+=data; resp=RespComposer(*this); }else{ resp=data; }
			if(!hasLength()&&hver>=1.1){ headers["content-length"]=dtos(this->body.size()); } socket->send(resp); 
			if(hver<=1.0||headers["connection"]=="close"){ socket->destroy(); } }
		bool ok(){ return status < 300 && status >= 200; }
		bool hasLength(){ return headers.has("content-length")||headers.has("transfer-encoding"); }
		void Redirect(const std::string& url){ status=301; headers["location"]=url; end(); }
		void SendErr(int status1){ status=status1; end(); }
		std::string cout() const { std::stringstream ss; ss << "Response Headers:"
			<< this->headers.cout() << "\n  === Body ===  \n" << this->body << "\n\n"; return ss.str(); }
	};
	
	IPAddr splitIpPort(const std::string& ipstr){ IPAddr result; result.ip=""; result.port=0; size_t closingIndx = ipstr.find("]");
		if (ipstr[0] == '[' && closingIndx != std::string::npos) { // IPv6 с портом
			result.ip = ipstr.substr(1, closingIndx - 1); size_t colonPos = ipstr.find(":", closingIndx);
			if(colonPos != std::string::npos){ result.port = stoin(ipstr.substr(colonPos + 1)); }
		} else { size_t colonIndx = ipstr.find(":"); if (colonIndx != std::string::npos){
				result.ip = ipstr.substr(0, colonIndx); result.port = stoin(ipstr.substr(colonIndx + 1)); } 
			else { result.ip = ipstr; } } return result; }
			
	HttpMsg HttpParse(const Buffer& rawreq, long pos=-1){ HttpMsg parsed; if(pos<0){ pos = rawreq.indexOf("\r\n")+2; } 
		long headersEnd = rawreq.indexOf("\r\n\r\n"); if(headersEnd<=0){ return parsed; } long lineEnd; String currKey=""; 
		while(pos<=headersEnd){ lineEnd = rawreq.indexOf("\r\n", pos); if(lineEnd == pos){ pos += 2; break; } if(lineEnd < 0){ break; }
			String hline = rawreq.slice(pos, lineEnd).toString(); long dPos = hline.indexOf(": ");
			if(dPos > 0){ if(hline.startsWith("\t") || hline.startsWith(" ")){ parsed.headers[currKey]+="\r\n "+hline.trim(); }else{  
				currKey = hline.slice(0, dPos).toLowerCase(); parsed.headers[currKey] = hline.slice(dPos+2); } } pos = lineEnd + 2;
		} parsed.body = rawreq.slice(pos+2); return parsed; }
			
	Req ReqParse(const Buffer& rawreq){ Req parsed; long lineEnd = rawreq.indexOf("\r\n"); if(lineEnd<0) return parsed; 
		std::istringstream statusStream(rawreq.slice(0, lineEnd).toString()); std::string hver_str;
		statusStream >> parsed.method; statusStream >> parsed.url; statusStream >> hver_str; parsed.hver=stofn(String(hver_str).split("/")[1]);
		HttpMsg msg = HttpParse(rawreq, lineEnd+2); parsed.headers=msg.headers; parsed.body=msg.body; return parsed; }
			
	Res RespParse(const Buffer& rawreq){ Res parsed; long lineEnd = rawreq.indexOf("\r\n"); if(lineEnd<0) return parsed; 
		std::istringstream statusStream(rawreq.slice(0, lineEnd).toString()); std::string hver_str;
		statusStream >> hver_str; parsed.hver=stofn(String(hver_str).split("/")[1]); statusStream >> parsed.status;
		HttpMsg msg = HttpParse(rawreq, lineEnd+2); parsed.headers=msg.headers; parsed.body=msg.body; return parsed; }
	
	Buffer QueryComposer(const std::string& host, const Req& opts){ Buffer query; std::stringstream headers; headers << (opts.method.empty() ? "GET" : opts.method)
			<<" "<<(opts.url.empty() ? "/" : opts.url)<<" HTTP/"<<opts.hver<<"\r\n"<<(!host.empty()?"Host: "+host+"\r\n":"");
		for(StringMap::const_iterator it = opts.headers.begin(); it != opts.headers.end(); ++it){ headers << it->first + ": " + it->second + "\r\n"; }
		if(!opts.headers.has("accept")){ headers<<"Accept: */*\r\n"; } if(!opts.headers.has("accept-encoding")){ headers<<"Accept-Encoding: identity\r\n"; }
		if(!opts.headers.has("user-agent")){ headers<<"User-Agent: Mozilla/5.0\r\n"; } if(!opts.headers.has("connection")){ headers<<"Connection: close\r\n"; } headers << "\r\n";
		query=headers.str(); if(!opts.body.empty()){ query.concat(opts.body); } return query; }
		
	Buffer RespComposer(Res& opts){ if(opts.status<=0) opts.status=200; std::stringstream headers; headers << "HTTP/" << opts.hver << " " << opts.status 
			<< " " << (ErrCodes.has(opts.status)?ErrCodes[opts.status]:"Code") << "\r\n";
		for(StringMap::const_iterator it = opts.headers.begin(); it != opts.headers.end(); ++it){ headers << it->first + ": " + it->second + "\r\n"; }
		if(!opts.headers.has("content-type")){ headers<<"Content-Type: text/html\r\n"; } if(!opts.headers.has("connection")){ headers<<"Connection: close\r\n"; } 
		if(!opts.hasLength()){ headers<<"Content-Length: "<<opts.body.size()<<"\r\n"; } headers<<"\r\n"; Buffer resp=headers.str(); if(opts.body.size()>0) resp.concat(opts.body); return resp; }
		
	Buffer chunkedStreamParser(Buffer& rawbuff, unsigned int& chunkSize, bool& is_end){ Buffer chunk;
		while(!rawbuff.empty()){
			if(chunkSize == 0){ long chunkEndIndx = rawbuff.indexOf("\r\n"); if(chunkEndIndx<0){ return chunk; }
				chunkSize = Buffer::htoi(rawbuff.slice(0, chunkEndIndx).toString()); 
				if(chunkSize == 0){ is_end=true; return chunk; } rawbuff = rawbuff.slice(chunkEndIndx+2); }
			if(rawbuff.size() < chunkSize+2){ return chunk; } // Если данных меньше, чем размер чанка, выходим и ждем дополнительные данные
			chunk+=rawbuff.slice(0, chunkSize); rawbuff = rawbuff.slice(chunkSize+2); chunkSize=0;
		} return chunk; }
		
	template <typename S>	
	Res request(S& socket, const std::string& host, int port, Req& params){
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
	Res request(S& socket, const std::string& host, int port=80){ Req params; return request(socket, host, port, params); }
	
	Res fetchd(std::string url, Req& params){ std::string host, path = "/"; int port = 80;
		bool useTLS = false; if(url.find("https://") == 0){ useTLS = true; port = 443; } if(useTLS){ throw std::runtime_error("fetchd: https protocol unsupported"); }
		size_t uindx = url.find("://"); url = (uindx != std::string::npos) ? url.substr(uindx + 3) : url; uindx = url.find("/");
		if(uindx != std::string::npos){ params.url = url.substr(uindx); url = url.substr(0, uindx); }else{ params.url = "/"; }

		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; } TCPSocket socket; return request(socket, addr.ip, addr.port, params); }
	Res fetchd(std::string url){ Req params; return fetchd(url, params); }
	
	bool SaveStream(std::string url, std::string fpath, String* err=NULL){ std::string host, path = "/"; int port = 80;
		if(url.find("https://") == 0){ if(err){ *err="https protocol unsupported"; } return false; }
		size_t uindx = url.find("://"); url = (uindx != std::string::npos) ? url.substr(uindx + 3) : url; uindx = url.find("/"); Req params; params.nobody=true;
		if(uindx != std::string::npos){ params.url = url.substr(uindx); url = url.substr(0, uindx); }else{ params.url = "/"; }
		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; } TCPSocket socket; 
		Res resp = request(socket, addr.ip, addr.port, params); if(!resp.ok()){ if(err){ *err="statusCode: "+dtos(resp.status); } return false; } Buffer chunk(65536); bool chunked=false;
		
		fs::FStream fstream = fs::createWriteStream(fpath); if(fstream.destroyed){ if(err){ *err="open file err: "+fpath; } return false; } size_t datasize[2]; datasize[0]=resp.body.size(); datasize[1]=0;
		if(resp.headers.has("content-length")){ datasize[1]=(size_t)stodn(resp.headers["content-length"]); }
		else if(resp.headers["transfer-encoding"]=="chunked"){ chunked=true; }
		struct { Buffer buff; unsigned int chunkSize; bool is_end;}ch_data; 
		if(chunked){ ch_data.chunkSize=0; ch_data.is_end=false; ch_data.buff=resp.body; datasize[0]=0; resp.body.clear(); }else{ fstream.write(resp.body); }
		while((datasize[0]<datasize[1]||chunked)&&socket.recv(&chunk)>0){ if(!chunked){ fstream.write(chunk); }
			else{ ch_data.buff+=chunk; fstream.write(chunkedStreamParser(ch_data.buff, ch_data.chunkSize, ch_data.is_end)); if(ch_data.is_end){ break; } }
			datasize[0]+=chunk.size(); } return true; } //std::cout << "(SaveStream: BODY chunk size): " << datasize[0] << "/" << datasize[1] << " | chunked: " << chunked << std::endl;
} }

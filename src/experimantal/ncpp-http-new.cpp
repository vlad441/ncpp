namespace ncpp{ namespace http{
	struct HttpReq { std::string hver; std::string method; std::string url; StringMap headers; Buffer body; bool nobody; HttpReq() : nobody(false){} };
	struct HttpRes { std::string hver; int status; std::string method; std::string url; StringMap headers; Buffer body; bool ok; };
	
	IPAddr splitIpPort(const std::string& ipstr){ IPAddr result; result.ip=""; result.port=0; size_t closingIndx = ipstr.find("]");
		if (ipstr[0] == '[' && closingIndx != std::string::npos) { // IPv6 с портом
			result.ip = ipstr.substr(1, closingIndx - 1); size_t colonPos = ipstr.find(":", closingIndx);
			if(colonPos != std::string::npos){ result.port = stoi(ipstr.substr(colonPos + 1)); }
		} else { size_t colonIndx = ipstr.find(":"); if (colonIndx != std::string::npos){
				result.ip = ipstr.substr(0, colonIndx); result.port = stoi(ipstr.substr(colonIndx + 1)); } 
			else { result.ip = ipstr; } } return result; }

	HttpRes HttpParse(Buffer rawreq){ HttpRes parsed; long pos = 0;
		long lineEnd = rawreq.indexOf("\r\n"); long headersEnd = rawreq.indexOf("\r\n\r\n"); // Парсинг строки состояния
		if (lineEnd != -1){ std::istringstream statusStream(rawreq.slice(0, lineEnd).toString());
			statusStream >> parsed.method; if(String(parsed.method).startsWith("HTTP/")){ parsed.hver=parsed.method; statusStream >> parsed.status; }
			else{ statusStream >> parsed.url; statusStream >> parsed.hver; } parsed.hver=String(parsed.hver).split("/")[1]; pos = lineEnd + 2; }
		String currKey=""; // Парсинг заголовков
		while(pos<=headersEnd){ lineEnd = rawreq.indexOf("\r\n", pos); if(lineEnd == pos){ pos += 2; break; } if(lineEnd < 0){ break; }
			String hline = rawreq.slice(pos, lineEnd).toString(); long dPos = hline.indexOf(": ");
			if(dPos > 0){ if(hline.startsWith("\t") || hline.startsWith(" ")){ parsed.headers[currKey]+="\r\n "+hline.trim(); }else{  
				currKey = hline.slice(0, dPos).toLowerCase(); parsed.headers[currKey] = hline.slice(dPos+2); } } pos = lineEnd + 2;
		} parsed.body = rawreq.slice(pos+2); return parsed; }
	
	Buffer QueryComposer(const std::string& host, HttpReq& opts){ Buffer query; std::string headers;
		headers += (opts.method.empty() ? "GET" : opts.method) + " "+(opts.url.empty() ? "/" : opts.url)+" HTTP/"+(opts.hver.empty() ? "1.1" : opts.hver)+"\r\n";
		headers += "Host: " + host + "\r\n";

		for(StringMap::const_iterator it = opts.headers.begin(); it != opts.headers.end(); ++it){ headers += it->first + ": " + it->second + "\r\n"; }
		
		if(!opts.headers.has("accept")){ headers+="Accept: */*\r\n"; } if(!opts.headers.has("accept-encoding")){ headers+="Accept-Encoding: identity\r\n"; }
		if(!opts.headers.has("user-agent")){ headers+="User-Agent: Mozilla/5.0\r\n"; } if(!opts.headers.has("connection")){ headers+="Connection: close\r\n"; } headers += "\r\n";
		std::cout << "(debug: QueryComposer): headers str: " << headers << std::endl;
		query=headers; if (!opts.body.empty()){ query.concat(opts.body); } return query; }
		
	Buffer chunkedStreamParser(Buffer& rawbuff, unsigned int& chunkSize, bool& is_end){ Buffer chunk;
		std::cout << "(debug: chunkedStreamParser chunkSize): " << rawbuff.size() << "/" << chunkSize << std::endl;
		std::cout << "(debug: chunkedStreamParser rawbuff(text)): " << rawbuff.slice(0,30).toString() << " ... " << std::endl;
		while(!rawbuff.empty()){
			if (chunkSize == 0){ long chunkEndIndx = rawbuff.indexOf("\r\n"); if(chunkEndIndx<0){ return chunk; }
				chunkSize = htoi(rawbuff.slice(0, chunkEndIndx).toString()); 
				if(chunkSize == 0){ std::cout << "(debug: chunkedStreamParser): ISEND" << std::endl; is_end=true; return chunk; } rawbuff = rawbuff.slice(chunkEndIndx+2); }
			if(rawbuff.size() < chunkSize+2){ return chunk; } // Если данных меньше, чем размер чанка, выходим и ждем дополнительные данные
			chunk+=rawbuff.slice(0, chunkSize); rawbuff = rawbuff.slice(chunkSize+2); chunkSize=0;
		} std::cout << "(debug: chunkedStreamParser RETURN): " << chunk.size() << " bytes" << std::endl; return chunk; }
	template <typename TSocket>	
	HttpRes request(TSocket& socket, const std::string& host, int port, HttpReq& params){
		if(!socket.connected&&!socket.connect(host, port)){ HttpRes response; response.status = 0; return response; }
		std::cout << "(debug: socket connected)" << std::endl;
		
		Buffer rawreq = QueryComposer(host, params);
		std::cout << "(debug: QueryComposer): first line: " << rawreq.split("\r\n")[0].toString() << std::endl;
		socket.send(rawreq); Buffer rawresp, chunk; while(true){ chunk=socket.recv(); rawresp+=chunk; std::cout << "(debug: HEADER chunk size): " << rawresp.size() << std::endl;
			if(rawresp.indexOf("\r\n\r\n")>=0||chunk.size()<=0){ break; } }
		HttpRes resp = HttpParse(rawresp); size_t datasize[2]; datasize[0]=resp.body.size(); datasize[1]=0; bool chunked=false; if(params.nobody){ return resp; }
		else if(resp.headers.has("content-length")){ datasize[1]=(size_t)stod(resp.headers["content-length"]); }
		else if(resp.headers["transfer-encoding"]=="chunked"){ chunked=true; } struct { Buffer buff; unsigned int chunkSize; bool is_end;}ch_data; 
		if(chunked){ ch_data.chunkSize=0; ch_data.is_end=false; ch_data.buff=resp.body; datasize[0]=0; resp.body.clear(); }
		while(datasize[0]<datasize[1]||chunked){ chunk=socket.recv(); if(!chunked){ resp.body+=chunk; }
			else{ ch_data.buff+=chunk; resp.body+=chunkedStreamParser(ch_data.buff, ch_data.chunkSize, ch_data.is_end); if(ch_data.is_end){ break; } }
			datasize[0]+=chunk.size(); std::cout << "(debug: BODY chunk size): " << datasize[0] << "/" << datasize[1] << " | chunked: " << chunked << std::endl;
			if(chunk.size()<=0){ break; } }
		
		std::cout << "(debug: TOTAL datasize): " << datasize[0] << "/" << datasize[1] << std::endl; 
		Sleep(1000); std::cout << "(debug: recvtest): " << socket.recv() << " | " << socket.connected << std::endl; 
		return resp; }
	template <typename TSocket>
	HttpRes request(TSocket& socket, const std::string& host, int port=80){ HttpReq params; return request(socket, host, port, params); }
	
	HttpRes fetchd(std::string url, HttpReq& params){ std::string host, path = "/"; int port = 80;
		bool useTLS = false; if(url.find("https://") == 0){ useTLS = true; port = 443; } if(useTLS){ throw std::runtime_error("fetchd: https protocol unsupported"); }
		// Парсинг URL для выделения хоста и пути
		size_t uindx = url.find("://"); url = (uindx != std::string::npos) ? url.substr(uindx + 3) : url; uindx = url.find("/");
		if(uindx != std::string::npos){ params.url = url.substr(uindx); url = url.substr(0, uindx); }else{ params.url = "/"; }

		// Разделяем хост и порт, если порт указан в хосте
		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; } 
		TCPSocket socket; return request(socket, addr.ip, addr.port, params); }
	HttpRes fetchd(std::string url){ HttpReq params; return fetchd(url, params); }
	
	void SaveStream(std::string url, std::string fpath){ std::string host, path = "/"; int port = 80;
		bool useTLS = false; if(url.find("https://") == 0){ useTLS = true; port = 443; } if(useTLS){ throw std::runtime_error("fetchd: https protocol unsupported"); }
		size_t uindx = url.find("://"); url = (uindx != std::string::npos) ? url.substr(uindx + 3) : url; uindx = url.find("/"); HttpReq params; params.nobody=true;
		if(uindx != std::string::npos){ params.url = url.substr(uindx); url = url.substr(0, uindx); }else{ params.url = "/"; }
		IPAddr addr = splitIpPort(url); if(addr.port == 0){ addr.port=port; }  
		TCPSocket socket; HttpRes resp = request(socket, addr.ip, addr.port, params); Buffer chunk; bool chunked=false;
		
		fs::FileStream fstream = fs::createWriteStream(fpath); size_t datasize[2]; datasize[0]=resp.body.size(); datasize[1]=0;
		if(resp.headers.has("content-length")){ datasize[1]=(size_t)stod(resp.headers["content-length"]); }
		else if(resp.headers["transfer-encoding"]=="chunked"){ chunked=true; } struct { Buffer buff; unsigned int chunkSize; bool is_end;}ch_data; 
		if(chunked){ ch_data.chunkSize=0; ch_data.is_end=false; ch_data.buff=resp.body; datasize[0]=0; resp.body.clear(); }else{ fstream.write(resp.body); }
		while((datasize[0]<datasize[1]||chunked)&&(chunk = socket.recv()).size()>0){ if(!chunked){ fstream.write(chunk); }
			else{ ch_data.buff+=chunk; fstream.write(chunkedStreamParser(ch_data.buff, ch_data.chunkSize, ch_data.is_end)); if(ch_data.is_end){ break; } }
			datasize[0]+=chunk.size(); std::cout << "(debug: BODY chunk size): " << datasize[0] << "/" << datasize[1] << " | chunked: " << chunked << std::endl; } }

} }

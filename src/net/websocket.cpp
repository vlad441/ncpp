namespace ncpp{
	struct WebSocket : TCPSocket { enum State { WS_HANDSHAKE, WS_OPEN, WS_CLOSE }; char state; bool client; TCPSocket* _kostyl;
		WebSocket() : client(true){}
		WebSocket(const CString& ip, int port, bool toconn=false) : TCPSocket(ip, port), state(0), client(true), _kostyl(NULL){ if(toconn) connect(); }
		WebSocket(const IPAddr& addr, bool toconn=false) : TCPSocket(addr.ip, addr.port), state(0), client(true), _kostyl(NULL){ if(toconn) connect(); }
		WebSocket(const Socket& sock) : TCPSocket(sock), state(0), client(true), _kostyl(NULL){}
		
		struct WSFrameInfo { char opcode; unsigned int headerSize, bodyLen; bool masked; bool fin; };
		
		bool connect(const CString& ip, int port){ if(!TCPSocket::connect(ip, port)) return false; 
			http::Req req; req.headers["connection"]="Upgrade"; req.headers["Upgrade"]="websocket"; 
			req.headers["Sec-Websocket-Version"]="13"; req.headers["Sec-WebSocket-Key"]="YWxha2F6YW0tMTYtYnl0ZQ=="; // Permanent 16 bytes key: "alakazam-16-byte";
			//req.headers["Sec-Websocket-Key"]=Buffer::randBytes(16).toString("base64"); // Random-Key
			http::Res res = http::request((TCPSocket&)*this, ip, port, req);
			print("(#DEBUG) ws.connect() res: "); print(res.cout()); print("-- res.body:"); print(res.body.toString()); print("\n");
			if(res.status==101&&res.headers.has("sec-websocket-accept")){ return true; }else{ destroy(); return false; } }
		bool connect(const IPAddr& a){ return connect(a.ip, a.port); };
		bool connect(){ return connect(destAddr.ip, destAddr.port); };
		
		int send(const char* cptr, char opcode=1){ return TCPSocket::send(WSEncodeFrame(cptr, opcode, client, true)); }
		int send(const Buffer& buff, char opcode=2){ return TCPSocket::send(WSEncodeFrame(buff, opcode, client, true)); }
		int send(const char* cptr, int len, char opcode){ return TCPSocket::send(WSEncodeFrame(Buffer(cptr, len), opcode, client, true)); }
		void ping(){ TCPSocket::send(WSEncodeFrame("", 9, client, true)); }
		void pong(){ TCPSocket::send(WSEncodeFrame("", 10, client, true)); }
		void close(int code=1000, const CString& reason=""){ Buffer resp(2); resp.writeUInt16BE(code); resp+=reason; 
			TCPSocket::send(WSEncodeFrame(resp, 8, client, true)); destroy(); }
		void destroy(){ if(_kostyl!=NULL){ _kostyl->destroy(); }else{ TCPSocket::destroy(); } }
		
		Buffer recv(WSFrameInfo* winfo=NULL){ Buffer frame(14); size_t rbytes=0, bSize=0;
			while(bSize==0&&rbytes<10&&sockfd!=-1){ rbytes+=TCPSocket::recv((char*)frame.data()+rbytes, 10-rbytes); bSize = WSGetFrameLen(frame); }
			if(bSize>0&&sockfd!=-1){ frame.resize(bSize); }else{ return Buffer(); }
			while(rbytes<bSize&&sockfd!=-1){ rbytes+=TCPSocket::recv((char*)&frame[rbytes], bSize-rbytes); } return WSDecodeFrame(frame, winfo); }
			
		void (*onMessage)(const WebSocket& ws, const Buffer& data);
		void (*onPing)(const WebSocket& ws);
		
		static Buffer WSEncodeFrame(const Buffer& data, char opcode=1, bool mask=false, bool is_final=true){
			//print("(#DEBUG) WSEncodeFrame mask: "); print(mask); print("\n");
			Buffer header; header.reserve(14); size_t len = data.size(); uint8_t mask_bit = mask?0x80U:0x00;
			header.push((is_final?0x80U:0x00) | (opcode & 0x0F));

			if(len <= 125){ header.push(mask_bit | (uint8_t)len); }
			else if(len <= 65535){ header.push(mask_bit | 126); header.writeUInt16BE(len, header.size()); }
			else{ header.push(mask_bit | 127); header.writeUInt64BE(len, header.size()); } if(len==0) return header;
			if(mask){ unsigned int maskKey = 0xFFFFFFFF; header.writeUInt32BE(maskKey, header.size()); Buffer masked; masked.resize(len);
				uint8_t mask_key_bytes[4] = { (uint8_t)((maskKey >> 24) & 0xFF), (uint8_t)((maskKey >> 16) & 0xFF), 
					(uint8_t)((maskKey >> 8) & 0xFF), (uint8_t)(maskKey & 0xFF) };
				for(size_t i = 0; i < len; ++i){ masked[i] = data[i] ^ mask_key_bytes[i % 4]; } header += masked; }else{ header+=data; }
			return header; }

		//Header Len: 2-14 bytes (2 base + 8 len + 4 mask)
		static Buffer WSDecodeFrame(const Buffer& data, WSFrameInfo* winfo=NULL){ unsigned char offset = 0; if(data.size() < 2){ return Buffer(); } 
			bool masked = (data[1] & 0x80) != 0; char len_7bit = data[1] & 0x7F; unsigned long long bodyLen = 0;

			if(len_7bit <= 125){ bodyLen = len_7bit; offset = 2; }
			else if(len_7bit == 126){ offset = 4; if(data.size() < offset){ return Buffer(); } bodyLen = data.readUInt16BE(2); }
			else{ offset = 10; if(data.size() < offset){ return Buffer(); } bodyLen = data.readUInt64BE(2); }
			char body_start = offset+(masked?4:0);
			
			if(winfo){ winfo->opcode=data[0] & 0x0F; winfo->fin=data[0] & 0x80; winfo->masked=masked; 
				winfo->headerSize=body_start; winfo->bodyLen=data.size()-body_start; }
			if(masked){ Buffer decoded; decoded.resize(bodyLen); uint8_t mask_key[4]; for(int i = 0; i < 4; ++i){ mask_key[i] = data[offset + i]; }
				for(unsigned int i = 0; i < bodyLen; ++i){ decoded[i] = data[body_start+i] ^ mask_key[i % 4]; } return decoded; }
			else{ return data.slice(body_start); } }
			
		static size_t WSGetFrameLen(const Buffer& data){ unsigned char offset = 0; if(data.size() < 2){ return 0; } 
			bool masked = (data[1] & 0x80) != 0; char len_7bit = data[1] & 0x7F; unsigned long long bodyLen = 0;

			if(len_7bit <= 125){ bodyLen = len_7bit; offset = 2; }
			else if(len_7bit == 126){ offset = 4; if(data.size() < offset){ return 0; } bodyLen = data.readUInt16BE(2); }
			else{ offset = 10; if(data.size() < offset){ return 0; } bodyLen = data.readUInt64BE(2); }
			return offset+(masked?4:0)+bodyLen; }
		
		//return bool: true = one frame ready, false = no frames
		static bool _splitFrame(Buffer& raw, Buffer& msg, WebSocket* ws=NULL){
			while(true){ size_t bSize = WSGetFrameLen(raw); if(bSize==0||raw.size() < bSize) return false;
				WSFrameInfo winfo; winfo.opcode=0; winfo.fin=false; Buffer frame=WSDecodeFrame(raw.slice(0, bSize), &winfo); raw = raw.slice(bSize);
				switch(winfo.opcode){ case 8: { if(ws) ws->destroy(); continue; } //close
					case 9: { if(ws) ws->pong(); continue; } //ping
					case 10: continue; //pong
					case 0: case 1: case 2: default: { msg+=frame; if(winfo.fin){ return true; } continue; } } } }
		//private: Buffer _rbuff, _msg;
	};

	struct WServer : HTTPServer {
		WServer() : HTTPServer(){ wsmode=true; }
		WServer(int port, const CString& bindip="::") : HTTPServer(port, bindip){ wsmode=true; }
		
		void (*onConnect)(WebSocket& ws, http::Req& req);
		void (*onMessage)(WebSocket& ws, const Buffer& data, http::Req& req);
		private: virtual void _onData(TCPSocket& socket, const Buffer& data){ http::Req& req=rcache[&socket].req; 
			if(rcache[&socket].ws){ if(onData!=NULL) onData(socket, data); rcache[&socket].bf+=data; _onMessage(socket, req); return; }
			HTTPServer::_onData(socket, data); if(req.headers["connection"]!="Upgrade"||req.headers["upgrade"]!="websocket") return;
			print("(#DEBUG) WS handshake captured\n"); print(req.cout()); print("\n");
			
			http::Res res(socket); res.status = 101; //Switching Protocols
			res.headers["Upgrade"] = "websocket"; res.headers["Connection"] = "Upgrade";
			res.headers["Sec-WebSocket-Accept"] = crypto::SHA1(Buffer::from(req.headers["sec-websocket-key"]+"258EAFA5-E914-47DA-95CA-C5AB0DC85B11")).digest().toString("base64");
			rcache[&socket].ws=true; res.end(""); if(onConnect!=NULL){ WebSocket ws=_WSfromTCP(socket); onConnect(ws, req); } }
		
		void _onMessage(TCPSocket& socket, http::Req& req){ if(onMessage==NULL) return; Buffer& wsbf=rcache[&socket].wsbf;
			while(WebSocket::_splitFrame(rcache[&socket].bf, wsbf)){ WebSocket ws=_WSfromTCP(socket); onMessage(ws, wsbf, req); wsbf.clear(); }
		}
		virtual void _onClose(TCPSocket& socket){ if(onClose!=NULL) onClose(socket); }
		static WebSocket _WSfromTCP(TCPSocket& socket){ WebSocket ws(socket); ws.own(false); ws.client=false; ws._kostyl=&socket; return ws; }
	};
}
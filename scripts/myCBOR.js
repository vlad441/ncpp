//myCBOR_v2.js
function createState(initSz=256){
	return { bf: Buffer.allocUnsafe(initSz), offset: 0, tmpbf: Buffer.allocUnsafe(512),
	
		reserve(sz=0){ let _msize = this.bf.length; if(sz <= _msize){ return this; }
			_msize = (sz < _msize*2)?_msize*2:sz; const _nptr = Buffer.allocUnsafe(_msize);
			this.bf.copy(_nptr, 0, 0, this._len); this.bf = _nptr; return this; },
		
		push8(val=0){ this.reserve(this._len+1); this.bf[this._len++] = val; return this; },
		pushStr(str = ""){ let len = Buffer.byteLength(str, 'utf8'); this.reserve(this._len + len); 
			this.bf.write(str, this._len, len, 'utf8'); this._len += len; return this; },
		pushBuff(bf){ if(!Buffer.isBuffer(bf)){ throw new TypeError("Argument must be a Buffer"); }
			const len = bf.length; this.reserve(this._len + len); bf.copy(this.bf, this._len, 0, len); this._len += len; return this; },
	} }
function ensureSz(st, needed){ if(st.offset+needed <= st.bf.length) return; st.reserve(st.offset+needed); }
//========= CBOR sizeof calc =========
function calcCBORLenSz(len=0){ if(len <= 23) return 1; if(len <= 0xFF) return 2; if(len <= 0xFFFF) return 3; if(len <= 0xFFFFFFFF) return 5; return 9; }
function calcCBORSzOf(obj){ if(obj === null || obj === undefined || typeof obj === 'boolean') return 1;
    if(typeof(obj) === 'number'){ if(Number.isInteger(obj)){ return calcCBORLenSz(Math.abs(obj)); } return 9; } // double //const val = obj<0 ? -1-obj : obj;?
    if(typeof(obj) === 'string'){ const len = Buffer.byteLength(obj, 'utf8'); return calcCBORLenSz(len) + len; }
    if(Buffer.isBuffer(obj)){ return calcCBORLenSz(obj.length) + obj.length; }
    if(Array.isArray(obj)){ let size = calcCBORLenSz(obj.length); for(const item of obj) size += calcCBORSzOf(item); return size; }
    if(typeof(obj) === 'object'){ const keys = Object.keys(obj); let size = calcCBORLenSz(keys.length);
        for(const key of keys){ size += calcCBORSzOf(key); size += calcCBORSzOf(obj[key]); } return size; } return 0; }
//========= CBOR serializer/parser =========
let myCBOR={}; myCBOR.calcSizeOf=calcCBORSzOf;
let njs = { buff: {
	readInt64BE(buff, offset=0){ const upper = buff.readInt32BE(offset);
		const lower = buff.readUInt32BE(offset + 4); return upper * 0x100000000 + lower; },
	writeInt64BE(buff, value, offset=0){ const upper = Math.floor(value / 0x100000000); const lower = value % 0x100000000;
		buff.writeInt32BE(upper, offset); buff.writeUInt32BE(lower, offset + 4); return buff; }, } };
		
myCBOR.writeLen = function(buf, len, otp=0x00, offset=0){
    if(len <= 23){ buf[offset++] = otp+len; } 
    else if(len <= 0xFF){ buf[offset++] = otp+24; buf[offset++] = len; }
    else if(len <= 0xFFFF){ buf[offset++] = otp+25; buf.writeUInt16BE(len, offset); offset+=2; }
    else if(len <= 0xFFFFFFFF){ buf[offset++] = otp+26; buf.writeUInt32BE(len, offset); offset+=4; }
    else{ buf[offset++] = otp+27;
        if(len <= Number.MAX_SAFE_INTEGER){ njs.buff.writeInt64BE(buf, len, offset); offset+=8; }
        else if(typeof BigInt !== 'undefined'){ buf.writeBigUInt64BE(BigInt(len), offset); offset+=8; }else{ offset--; } } return offset; }
		
myCBOR._calcLenSz=function(len=0){ if(len <= 23) return 1; if(len <= 0xFF) return 2; if(len <= 0xFFFF) return 3; if(len <= 0xFFFFFFFF) return 5; return 9; }
		
//myCBOR._wrLen = function(st, len, otp=0x00){ ensureSz(st, 9); st.offset=myCBOR.writeLen(st.bf,len,otp,st.offset); } // Minus ~10 ms
myCBOR._wrLen = function(st, len, otp=0x00){ ensureSz(st, 9); let buf = st.bf, offset = st.offset;
    if(len <= 23){ buf[offset++] = otp+len; }
    else if(len <= 0xFF){ buf[offset++] = otp+24; buf[offset++] = len; }
    else if(len <= 0xFFFF){ buf[offset++] = otp+25; buf.writeUInt16BE(len, offset); offset+=2; }
    else if(len <= 0xFFFFFFFF){ buf[offset++] = otp+26; buf.writeUInt32BE(len, offset); offset+=4; }
    else{ buf[offset++] = otp+27;
        if(len <= Number.MAX_SAFE_INTEGER){ njs.buff.writeInt64BE(buf, len, offset); offset+=8; }
        else if(typeof BigInt !== 'undefined'){ buf.writeBigUInt64BE(BigInt(len), offset); offset+=8; }else{ offset--; } } st.offset = offset; }

myCBOR._serializeVal = function(st, obj){
    if(obj === null){ ensureSz(st, 1); st.bf[st.offset++] = 0xF6; return; }
    if(obj === undefined){ ensureSz(st, 1); st.bf[st.offset++] = 0xF7; return; } const type = typeof(obj);
    if(type === 'boolean'){ ensureSz(st, 1); st.bf[st.offset++] = obj?0xF5:0xF4; return; }
    if(type === 'number'){ 
        if(Number.isInteger(obj)){ if(obj>=0){ this._wrLen(st, obj, 0x00); }else{ this._wrLen(st, -1-obj, 0x20); } }
        else{ ensureSz(st, 9); st.bf[st.offset++] = 0xFB; st.bf.writeDoubleBE(obj, st.offset); st.offset += 8; } return; }
    if(type === 'string'){ let len=obj.length;
		//if(len <= 100 && !/[^\x00-\x7F]/.test(obj)){} //ASCII optimization?
		if(len <= 100){ let bLen=0; //UTF8 optimization
			for(let i=0; i<len; i++){ const c = obj.charCodeAt(i); bLen+=(c<0x80) ? 1 : (c<0x800) ? 2 : (c<0x10000)?3:4; } len=bLen; }
		else{ len = Buffer.byteLength(obj, 'utf8'); }
		this._wrLen(st, len, 0x60); ensureSz(st, len); st.bf.write(obj, st.offset, 'utf8'); st.offset += len; return; }
    if(Buffer.isBuffer(obj)){ this._wrLen(st, obj.length, 0x40);
        ensureSz(st, obj.length); obj.copy(st.bf, st.offset, 0, obj.length); st.offset += obj.length; return; }
    if(Array.isArray(obj)){ this._wrLen(st, obj.length, 0x80); for(const item of obj) this._serializeVal(st, item); return; }
    if(type === 'object'){ const keys = Object.keys(obj); this._wrLen(st, keys.length, 0xA0);
        for(const key of keys){ this._serializeVal(st, key); this._serializeVal(st, obj[key]); } return; }
}

myCBOR.serialize = function(obj){ const st = createState(); this._serializeVal(st, obj); return st.bf.subarray(0, st.offset); };

myCBOR.parseLen=function(buff, state={offset: 0}){ let otp = buff[state.offset] & 0x1F; //let otp = buff[state.offset] % 32; //<- маска заместо остатка
	if(otp <= 23){ state.offset++; return otp; } let rst = state.offset + 1;
	if(otp === 0x18){ if(buff.length <= state.offset + 1){ state.offset++; return 0; } state.offset += 2; return buff[rst]; } // 1 byte
	else if(otp === 0x19){ if(buff.length <= state.offset + 2){ state.offset++; return 0; } state.offset += 3; return buff.readUInt16BE(rst); } // 2 bytes
	else if(otp === 0x1A){ if(buff.length <= state.offset + 4){ state.offset++; return 0; } state.offset += 5; return buff.readUInt32BE(rst); } // 4 bytes
	else if(otp === 0x1B){ if(buff.length <= state.offset + 8){ state.offset++; return 0; } state.offset += 9; let val = njs.buff.readInt64BE(buff, rst);
		if(!Number.isSafeInteger(val) && typeof BigInt !== 'undefined'){ return buff.readBigInt64BE(rst); } return val; } // 8 bytes
	else{ return 0; } }

myCBOR.parseValue=function(cbor, state={offset: 0}){
	let type = cbor[state.offset]; let len;

	if(type <= 0x1F){ return this.parseLen(cbor, state); } // Unsigned Integer
	else if(type >= 0x20 && type <= 0x3F){ return -1-this.parseLen(cbor, state); } // Negative Integer
	else if(type >= 0x40 && type <= 0x5F){ len = this.parseLen(cbor, state); let rst = state.offset;
		state.offset += len; return cbor.slice(rst, rst+len); } // Byte String (Buffer)
	else if(type >= 0x60 && type <= 0x7F){ len = this.parseLen(cbor, state); let rst = state.offset;
		state.offset += len; return cbor.slice(rst, rst+len).toString('utf8'); } // Text String
	else if ((type >= 0x80 && type <= 0x9F) || (type >= 0xA0 && type <= 0xBF)){ return this.parse(cbor, state); } //Array or Map
	else { state.offset++; // Simple Value / Float
		switch(type){
			case 0xF4: return false;
			case 0xF5: return true;
			case 0xF6: return null;
			//case 0xF9: { let rst = state.offset; state.offset += 2; return cbor.readFloatBE(rst); } // Half-precision float (read as 2 bytes)
			case 0xFA: { let rst = state.offset; state.offset += 4; return cbor.readFloatBE(rst); } // Float 32
			case 0xFB: { let rst = state.offset; state.offset += 8; return cbor.readDoubleBE(rst); } // Double 64
			case 0xF7: default: return undefined;
		}
	}
}

myCBOR.parse=function(cbor, state={offset: 0}){ let obj = {}; let arr = [];
	let indx = 0; let len = 0; let ismap = false; let type = cbor[state.offset]; let isIndef = (type & 0x1F)===0x1F;
	// Определяем Array или Map
	if(type >= 0x80 && type <= 0xBF){ len = this.parseLen(cbor, state); }
	if(type >= 0xA0 && type <= 0xBF){ ismap = true; }
	// Цикл обработки элементов
	while((isIndef?true:indx < len) && state.offset < cbor.length){
		if(ismap){ let key = this.parseValue(cbor, state); obj[key] = this.parseValue(cbor, state); }
		else{ arr.push(this.parseValue(cbor, state)); } indx++; } return ismap?obj:arr; }

module.exports=myCBOR;
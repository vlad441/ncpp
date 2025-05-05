namespace ncpp{ struct ObjectValue; struct Object; typedef ObjectValue Variant; typedef Array<Variant> VArray; 
namespace JSON { Object parse(String jsonstr); }
//#if __cplusplus >= 201103L // C++11
	//struct Object : std::unordered_map<std::string, ObjectValue> {
		//typedef std::unordered_map<std::string, ObjectValue> MapType;
//#else // or C++98
	struct Object : std::map<std::string, ObjectValue> {
		typedef std::map<std::string, ObjectValue> MapType;
//#endif
		typedef MapType::iterator iterator;
		typedef MapType::const_iterator const_iterator;
		Object(){} ~Object(){ clear(); }
		Object(String json){ *this=JSON::parse(json); }
		ObjectValue& operator[](const std::string& key);
		const ObjectValue& operator[](const std::string& key) const;
		Array<String> keys() const;
		bool has(std::string key) const;
		std::string cout(int depth=2, int bias=2) const;
		String toString(int depth=2) const { return cout(depth); }
		std::string type(){ return "object"; }
		//Object& assign(Array<Object> key){}
		static const ObjectValue& getNestedValue(const Object* obj, const Array<String>& path);
		static const ObjectValue& getNestedValue(const Object& obj, const String& path);
		static ObjectValue& getNestedValue(Object* obj, Array<String> path){ return (ObjectValue&)getNestedValue((const Object*)obj, path); }
		static ObjectValue& getNestedValue(Object& obj, String path){ return (ObjectValue&)getNestedValue((const Object&)obj, path); }
		static void setNestedValue(Object* obj, const Array<String>& path, const ObjectValue& value);
		static void setNestedValue(Object& obj, const String& path, const ObjectValue& value);
		struct ConstIter;
	};
	
	struct ObjectValue { enum Type { DOUBLE, INT32, INT64, BOOLEAN, STRING, BUFFER, OBJECT, ARRAY, PTR, NIL, UNDEFINED }; Type _type;
		union { double d; int i; long long ll; bool b; void* ptr; } value; 
		struct { String s; Buffer bf; Object o; } edata;
		ObjectValue() : _type(NIL){} ~ObjectValue(){ clear(); }
		ObjectValue(double d){ set(d); }
		#if __cplusplus >= 201103L
		explicit ObjectValue(long long v){ set(v); }
		explicit ObjectValue(bool b){ set(b); }
		#endif
		ObjectValue(const std::string& s){ set(s); }
		ObjectValue(const Buffer& bf){ set(bf); }
		ObjectValue(const char* c){ set((const std::string&)c); }
		ObjectValue(const Object& o){ set(o); }
		ObjectValue(const StringMap& map){ set(map); }
		ObjectValue(const DoubleMap& map){ set(map); }
		template<typename T>
		ObjectValue(const Array<T>& arr){ set(arr); }

		//void clear(){ if(_type == STRING){ delete edata.s; } else if(_type == OBJECT){ delete value.o; } _type = NIL; }
		void clear(){ _type = NIL; }
		
		ObjectValue& set(double d){ clear(); _type = DOUBLE; value.d = d; return *this; } 
		ObjectValue& set(int i){ clear(); _type = INT32; value.i = i; return *this; }
		ObjectValue& set(long long ll){ clear(); _type = INT64; value.ll = ll; return *this; }
		ObjectValue& set(bool b){ clear(); _type = BOOLEAN; value.b = b; return *this; }
		ObjectValue& set(const std::string& s){ clear(); _type = STRING; edata.s = s; return *this; }
		ObjectValue& set(const Buffer& bf){ clear(); _type = BUFFER; edata.bf = bf; return *this; }
		ObjectValue& set(const char* c){ set((const std::string&)c); return *this; }
		ObjectValue& set(const Object& o){ clear(); _type = OBJECT; edata.o = o; return *this; }
		ObjectValue& set(const StringMap& map){ clear(); _type = OBJECT; edata.o=Object(); 
			for(StringMap::const_iterator it = map.begin(); it != map.end(); ++it){ edata.o[it->first]=it->second; } return *this; }
		ObjectValue& set(const DoubleMap& map){ clear(); _type = OBJECT; edata.o=Object(); 
			for(DoubleMap::const_iterator it = map.begin(); it != map.end(); ++it){ edata.o[it->first]=it->second; } return *this; }
		template<typename T>
		ObjectValue& set(const Array<T>& arr){ clear(); _type = ARRAY; edata.o=Object(); for(size_t i=0;i<arr.size();i++){ edata.o[dtos(i)]=arr[i]; } return *this; }
		ObjectValue& setNull(){ clear(); _type = NIL; return *this; }
		ObjectValue& setUndefined(){ clear(); _type = UNDEFINED; return *this; }

		bool isInt32() const { return _type == INT32; } bool isInt64() const { return _type == INT64; }
		bool isDouble() const { return _type == DOUBLE; } bool isNumber() const { return isDouble()||isInt32()||isInt64(); }
		bool isBool() const { return _type == BOOLEAN; }
		bool isString() const { return _type == STRING; }
		bool isBuff() const { return _type == BUFFER; }
		bool isObj() const { return _type == OBJECT; }
		bool isArray() const { return _type == ARRAY; }
		bool isNull() const { return _type == NIL; }
		bool isUndefined() const { return _type == UNDEFINED; }

		double& asDouble(){ if(_type!=DOUBLE){ throw Err("asDouble(): is not a Double type"); } return value.d; }
		bool& asBool(){ if(!isBool()){ throw Err("asBool(): is not a Bool type"); } return value.b; }
		String& asString(){ if(!isString()){ throw Err("asString(): is not a String type"); } return edata.s; }
		Buffer& asBuff(){ if(!isBuff()){ throw Err("asBuff(): is not a Buffer type"); } return edata.bf; }
		Object& asObj(){ if(!isObj()){ throw Err("asObj(): is not a Object type"); } return edata.o; }
		
		const double& asDouble() const { if(_type!=DOUBLE){ throw Err("asDouble(): is not a Double type"); } return value.d; }
		const bool& asBool() const { if(!isBool()){ throw Err("asBool(): is not a Bool type"); } return value.b; }
		const String& asString() const { if(!isString()){ throw Err("asString(): is not a String type"); } return edata.s; }
		const Buffer& asBuff() const { if(!isBuff()){ throw Err("asBuff(): is not a Buffer type"); } return edata.bf; }
		const Object& asObj() const { if(!isObj()&&!isArray()){ throw Err("asObj(): is not a Object type"); } return edata.o; }
		//const VArray& asArray() const { if(!isArray()){ throw Err("asArray(): is not a Array type"); } return edata.o; }
		
		operator double() const { return toNumber(); }
		#if __cplusplus >= 201103L
		explicit operator int() const { return toInt32(); }
		explicit operator long long() const { return toInt64(); }
		explicit operator bool() const { return toBool(); }
		#endif
		operator String() const { return toString(); }
		operator Buffer() const { return toBuff(); }
		operator Object() const { return isObj()?edata.o:Object(); }
		template<typename T>
		operator Array<T>() const { Array<T> arr; for(Object::const_iterator it = edata.o.begin(); it != edata.o.end(); ++it){ arr.push(it->second); } return arr; }
		
		ObjectValue& operator=(double d){ set(d); return *this; }
		ObjectValue& operator=(int i){ set(i); return *this; }
		ObjectValue& operator=(long long ll){ set(ll); return *this; }
		ObjectValue& operator=(bool b){ set(b); return *this; }
		ObjectValue& operator=(const std::string& s){ set(s); return *this; }
		ObjectValue& operator=(const char* c){ set((const std::string&)c); return *this; }
		ObjectValue& operator=(const Buffer& bf){ set(bf); return *this; }
		ObjectValue& operator=(const Object& o){ set(o); return *this; }
		ObjectValue& operator=(const StringMap& map){ set(map); return *this; }
		ObjectValue& operator=(const DoubleMap& map){ set(map); return *this; }
		template<typename T>
		ObjectValue& operator=(const Array<T>& arr){ set(arr); return *this; }
		ObjectValue& operator[](const std::string& key){ return edata.o[key]; }
		ObjectValue& operator[](const char* key){ return edata.o[std::string(key)]; }
		//ObjectValue& operator[](size_t indx){ if(!isArray()){ throw Err("ObjectValue& operator[](size_t indx): is not a Array type"); } return edata.o[dtos(indx)]; }
		
		bool operator==(const std::string& s) const { return isString()&&s==edata.s; }
		bool operator==(const char* c) const { return isString()&&std::string(c)==edata.s; }
		bool operator==(bool b) const { return isBool()&&b==value.b; }
		bool operator==(const Buffer& bf) const { return isBuff()&&bf==edata.bf; }
		bool operator!=(const std::string& s) const { return isString()&&s!=edata.s; }
		bool operator!=(const char* c) const { return isString()&&std::string(c)!=edata.s; }
		bool operator!=(const Buffer& bf) const { return isBuff()&&bf!=edata.bf; }
		bool operator!=(bool b) const { return isBool()&&b!=value.b; }
		
		ObjectValue& operator+=(double numb){ if(isDouble()){ value.d+=numb; }else if(isInt32()){ value.i+=numb; }
			else if(isInt64()){ value.ll+=numb; }else{ set(this->toNumber()+numb); }  return *this; }
		ObjectValue& operator+=(const String& str){ if(isString()){ edata.s+=str; }
			else if(isBuff()){ edata.bf+=str; }else{ set(this->toString()+str); }  return *this; }
		ObjectValue operator+(double numb) const { ObjectValue r = *this; r += numb; return r; }
		ObjectValue operator+(const String& str) const { ObjectValue r = *this; r += str; return r; }
		ObjectValue& operator++(){ *this+=1; return *this; } // frefix: ++a
		ObjectValue operator++(int){ ObjectValue tmp(*this); *this+=1; return tmp; } // postfix: a++
		

		ObjectValue& operator-=(double numb){ if(isDouble()){ value.d-=numb; }else if(isInt32()){ value.i-=numb; }
			else if(isInt64()){ value.ll-=numb; }else{ set(this->toNumber()-numb); } return *this; }
		ObjectValue operator-(double numb) const { ObjectValue r = *this; r -= numb; return r; }
		ObjectValue& operator--(){ *this-=1; return *this; }
		ObjectValue operator--(int){ ObjectValue tmp(*this); *this-=1; return tmp; }
		
		static bool compare(const ObjectValue& val1, const ObjectValue& val2, bool strong=false){
			if(strong){ if(val1._type != val2._type) return false; }

			if(val1.isNumber() && val2.isNumber()){ return val1.toNumber() == val2.toNumber(); } 
			else if(val1.isString() && val2.isString()){ return val1.asString() == val2.asString(); } 
			else if(val1.isBool() && val2.isBool()){ return val1.asBool() == val2.asBool(); } 
			else if(val1.isBuff() && val2.isBuff()){ return val1.asBuff() == val2.asBuff(); } 
			else if(val1.isObj() && val2.isObj()){ return val1.asObj().toString() == val2.asObj().toString(); }
			else if(val1.isNull() && val2.isNull()) return true;
			else if(val1.isNull() || val2.isNull()) return false;
			else if(val1.isUndefined() && val2.isUndefined()) return true;  // undefined === undefined
			else if(val1.isUndefined() || val2.isUndefined()) return false; // undefined !== любому другому
			//else if(val1.isArray() && val2.isArray()){ return val1.asObj().toString() == val2.asObj().toString(); }
			if(!strong){ return val1.toString() == val2.toString(); } return false; } 
		
		std::string type() const noexcept { switch((*this)._type){ case DOUBLE: return "double"; case INT32: return "int32"; case INT64: return "int64"; 
			case BOOLEAN: return "boolean"; case STRING: return "string"; case BUFFER: return "buffer"; case ARRAY: return "array";
			case OBJECT: return "object"; case NIL: return "null"; case UNDEFINED: return "undefined"; default: return "(unknown)"; } }
		
		String toString() const noexcept { switch((*this)._type){ case DOUBLE: return dtos((*this).value.d);
            case INT32: return dtos((*this).value.i); case INT64: return dtos((*this).value.ll);
            case BOOLEAN: return ((*this).value.b?"true":"false"); case STRING: return (*this).edata.s;
            case BUFFER: return (*this).edata.bf.cout(); case ARRAY: return "[...]"; case OBJECT: return "{...}";
            case NIL: return "<null>"; case UNDEFINED: return "<undefined>"; default: return "<?>"; } }
			
		double toNumber() const noexcept { switch((*this)._type){ case DOUBLE: return (*this).value.d;
            case INT32: return (*this).value.i; case INT64: return (*this).value.ll;
            case BOOLEAN: return ((*this).value.b ? 1 : 0); case STRING: return stodn((*this).edata.s);
            case BUFFER: return (*this).edata.bf.toInt(); default: return 0; } }
        //double toDouble() const noexcept { return toNumber(); } 
        int toInt32() const noexcept { return _type==INT32?(*this).value.i:toNumber(); } 
		long long toInt64() const noexcept { return _type==INT64?(*this).value.ll:toNumber(); }
            
        Buffer toBuff() const noexcept { switch((*this)._type){ case BUFFER: return (*this).edata.bf;
			case DOUBLE: { Buffer buff(8); buff.writeDoubleLE((*this).value.d); return buff; }
			case INT32: { Buffer buff(4); buff.writeInt32LE((*this).value.i); return buff; }
			case INT64: { Buffer buff(8); buff.writeInt64LE((*this).value.ll); return buff; }
            case STRING: return Buffer::from((*this).edata.s); default: return Buffer(); } }
		
		bool toBool() const noexcept { switch((*this)._type){ case DOUBLE: case INT32: case INT64: return toNumber()>0;
            case BOOLEAN: return (*this).value.b; case STRING: return (*this).edata.s!=""&&(*this).edata.s!="0";
            case BUFFER: return (*this).edata.bf.size()>0; case OBJECT: return true; default: return false; } }
	};
	
	Array<String> Object::keys() const { Array<String> keys;
		for(Object::const_iterator it = this->begin(); it != this->end(); ++it){ keys.push(it->first); } return keys; }
	bool Object::has(std::string key) const { return this->find(key) != this->end(); }
	
	ObjectValue& Object::operator[](const std::string& key){ return MapType::operator[](key); }
	const ObjectValue& Object::operator[](const std::string& key) const {
		Object::const_iterator it = find(key); if (it != end()){ return it->second; }
		static ObjectValue emptyValue; return emptyValue; }
		
	const ObjectValue& Object::getNestedValue(const Object* obj, const Array<String>& path){ size_t i=0;
		for(;i<path.size()-1;i++){ if(!(*obj)[path[i]].isObj()){ return (*obj)[path[i]]; } obj = &(*obj)[path[i]].asObj(); } return (*obj)[path[i]]; }
	const ObjectValue& Object::getNestedValue(const Object& obj, const String& path){ if(!path.includes(".")){ return obj[path]; }
		else{ return getNestedValue(&obj, path.split(".")); } }
	void Object::setNestedValue(Object* obj, const Array<String>& path, const ObjectValue& value){
		for(size_t i=0;i<path.size()-1;i++){ if(!(*obj).has(path[i])){ (*obj)[path[i]]=Object(); } obj = &(*obj)[path[i]].asObj(); } (*obj)[path[path.size()-1]] = value; }
	void Object::setNestedValue(Object& obj, const String& path, const ObjectValue& value){ setNestedValue(&obj, path.split("."), value); };
	
	struct Object::ConstIter { bool isfirst; ConstIter(const Object& obj) : isfirst(true), it(obj.begin()), _end(obj.end()){}
		bool next(){ if(it == _end) return false; ++it; return it != _end; }
		bool iterate(){ if(it == _end) return false; if(isfirst){ isfirst=false; return true; } ++it; return it != _end; }
		bool end(){ if(it == _end){ return true; } return false; }
		const std::string& key() const { return it->first; }
		const ObjectValue& value() const { return it->second; } 
		private: Object::const_iterator it, _end; };
	//for(Object::Iter it(filter); !it.end(); it.next()){	
	//for(Object::Iter it(filter); it.iterate();){
		
	std::ostream& operator<<(std::ostream& os, const ObjectValue& oval){ os << oval.toString(); return os; }
            
    std::string Object::cout(int depth, int bias) const { std::stringstream ss; ss << "{";
			for(Object::const_iterator it = this->begin(); it != this->end(); ++it){ ss << "\n"+Buffer(bias, 0x20).toString()+"\"" << it->first << "\": ";
				if(depth>1&&it->second.isObj()){ ss << it->second.asObj().cout(depth-1, bias+2); }
				else{ ss << it->second; } ss << ", "; } ss << "}"; return ss.str(); }
	std::ostream& operator<<(std::ostream& os, const Object& obj){ os << obj.cout(); return os; }
	
namespace JSON{ //TODO: экранирование \\\" и прочего
	String escapeString(String str){ str = str.replaceAll("\\", "\\\\").replaceAll("\"", "\\\"")
		.replaceAll("\n", "\\n").replaceAll("\t", "\\t").replaceAll("\r", "\\r")
		.replaceAll("\b", "\\b").replaceAll("\f", "\\f"); return str; }
	String unescapeString(String str){ str = str.replaceAll("\\\\", "\\").replaceAll("\\\"", "\"")
		.replaceAll("\\n", "\n").replaceAll("\\t", "\t").replaceAll("\\r", "\r") 
		.replaceAll("\\b", "\b").replaceAll("\\f", "\f"); return str; }
	
	String stringify(const Object& obj){ std::stringstream ss; ss << "{";
		for(Object::const_iterator it = obj.begin(); it != obj.end(); ++it){
			if (it != obj.begin()){ ss << ","; } ss << "\"" << it->first << "\":"; 
			if(it->second.isString()||it->second.isBuff()){ ss << "\""; ss << escapeString(it->second.toString()); ss << "\""; }
			else if(it->second.isObj()){ ss << stringify(it->second.asObj()); }
			else{ ss << it->second.toString(); } } ss << "}"; return ss.str(); }
		
	String stringify(const StringMap& map){ std::stringstream ss; ss << "{";
		for(StringMap::const_iterator it = map.begin(); it != map.end(); ++it){ if(it != map.begin()){ ss << ","; } 
		    ss << "\"" << it->first << "\":\"" << escapeString(it->second) << "\""; } ss << "}"; return ss.str(); }
	
	size_t findNextUnescaped(const ncpp::String& str, char ch, size_t start = 0){ bool inEscape = false;
		for(size_t i = start; i < str.size(); ++i){ if(str[i] == '\\' && !inEscape){ inEscape = true; } 
			else if(str[i] == ch && !inEscape){ return i; } else{ inEscape = false; } } return std::string::npos; }
	
	Object parse(String jsonstr){ ncpp::Object obj; jsonstr=jsonstr.trim();
		if (jsonstr[0] != '{' || jsonstr[jsonstr.size()-1] != '}'){ throw std::runtime_error("JSON::parse: Invalid JSON format"); } jsonstr = jsonstr.slice(1, -1); size_t pos = 0;
		
		while (pos < jsonstr.size()){
			size_t keyStart = jsonstr.indexOf('"', pos); if(keyStart == std::string::npos) break;
			size_t keyEnd = jsonstr.indexOf('"', keyStart + 1); if(keyEnd == std::string::npos) break;
			String key = jsonstr.slice(keyStart + 1, keyEnd);
			size_t colonPos = jsonstr.indexOf(':', keyEnd + 1); if(colonPos == std::string::npos) break;
			size_t valueStart = colonPos + 1;
			
			while (valueStart < jsonstr.size() && (jsonstr[valueStart] == ' ' || jsonstr[valueStart] == '\n' || jsonstr[valueStart] == '\t')){ valueStart++; }
			char FirstChar = jsonstr[valueStart]; size_t valueEnd = valueStart;

			if(FirstChar == '"'){ valueEnd = findNextUnescaped(jsonstr, '"', valueStart+1); if (valueEnd != std::string::npos){ valueEnd++; }else{ throw std::runtime_error("JSON::parse: String literal: Invalid format"); } } // Строка
			else if(FirstChar == '{') { // Объект
				int braceCount = 1; valueEnd = valueStart+1;
				while (braceCount > 0 && valueEnd < jsonstr.size()){
					if(jsonstr[valueEnd] == '{') braceCount++;
					if(jsonstr[valueEnd] == '}'){ braceCount--; } valueEnd++; } 
			} else if (isdigit(FirstChar) || FirstChar == '-' || FirstChar == '+'){ // Число
				while (valueEnd < jsonstr.size() && (isdigit(jsonstr[valueEnd]) || jsonstr[valueEnd] == '.' || jsonstr[valueEnd] == 'e' || jsonstr[valueEnd] == 'E' || jsonstr[valueEnd] == '-' || jsonstr[valueEnd] == '+')){
					valueEnd++; }
			} else { while(valueEnd < jsonstr.size() && jsonstr[valueEnd] != ',' && jsonstr[valueEnd] != '}'){ valueEnd++; } } // boolean, null or undefined

			ncpp::String value = jsonstr.slice(valueStart, valueEnd);
			if(value[0] == '"'){ obj[key] = unescapeString(value.slice(1,-1)); } 
			else if(value[0] == '{') { obj[key] = parse(value); } 
			else if(value == "true" || value == "false"){ obj[key] = (value == "true"); } 
			else if(value == "null"){ obj[key]._type = ncpp::ObjectValue::NIL; } 
			else if(value == "undefined"){ obj[key]._type = ncpp::ObjectValue::UNDEFINED; } 
			else { obj[key] = stodn(value); } // Число

			pos = valueEnd+1; if (pos < jsonstr.size() && jsonstr[pos] == ',') pos++;
		} return obj; }
}

namespace BSON{
	void insertCStr(Buffer& buff, const std::string& str, size_t offset=0){ buff.write(str, offset); buff[offset+str.size()] = 0x00; }
	void insertSz(Buffer& buff, size_t value, size_t offset=0, char sz=4){ if(sz==4){ buff.writeUInt32LE(value, offset); }else{ buff.writeUInt64LE(value, offset); } }
	
	Buffer serialize(const Object& obj, bool ex=false){ const char intsz=ex?8:4; Buffer buff(intsz), bf;
		for(Object::const_iterator it = obj.begin(); it != obj.end(); ++it){ const std::string& key = it->first; const ObjectValue& value = it->second;
			if(value.isNumber()){ bf.resize(key.size()+10); insertCStr(bf, key, 1); 
				if(value.isInt32()){ bf.resize(key.size()+6); bf[0]=0x10; bf.writeInt32LE(value.toNumber(), key.size()+2); } // Int32
				else if(value.isInt64()){ bf[0]=0x12; bf.writeInt64LE(value.toNumber(), key.size()+2); } // Int64
				else{ bf[0]=0x01; bf.writeDoubleLE(value.toNumber(), key.size()+2); } // Double
			}else if(value.isString()){ std::string str=value.toString(); bf.resize(key.size()+7+str.size()); bf[0]=0x02; insertCStr(bf, key, 1); 
				insertSz(bf, str.size()+1, key.size()+2); insertCStr(bf, str, key.size()+6); } // String
			else if(value.isNull()){ bf.resize(key.size()+2); bf[0]=0x0A; insertCStr(bf, key, 1); } // Null
			else if(value.isObj()){ bf.resize(key.size()+2); bf[0]=0x03; insertCStr(bf, key, 1); bf+=serialize(value.asObj(), ex); } // Object (Embedded document)
			else if(value.isBuff()){ const Buffer& tbf=value.asBuff(); bf.resize(key.size()+3+intsz); bf[0]=0x05; insertCStr(bf, key, 1);
				insertSz(bf, tbf.size(), key.size()+2, intsz); bf.back()=0; bf+=tbf; } //bf.back()=subtype // Binary data
			else if(value._type==ObjectValue::ARRAY){ bf.resize(key.size()+2); bf[0]=0x04; insertCStr(bf, key, 1); // Array
				  const Object& obj = value.asObj(); Object arr; Array<String> keys = obj.keys();
				  for(size_t i=0;i<keys.size();i++){ arr[dtos(i)] = obj[keys[i]]; } bf+=serialize(obj, ex); }
			else if(value.isBool()){ bf.resize(key.size()+3); bf[0]=0x08; insertCStr(bf, key, 1); bf.back()=value.asBool()?0x01:0x00; } // Boolean
			else if(value.isUndefined()){ bf.resize(key.size()+2); bf[0]=0x06; insertCStr(bf, key, 1); } // Undefined
			else{ throw ncpp::Err("Unsupported BSON type for key: "+key); } buff+=bf; }
		buff+=Buffer(1,0); insertSz(buff, buff.size(), 0, intsz); return buff; }
		
	Buffer serialize(const StringMap& map, bool ex=false){ const char intsz=ex?8:4; Buffer buff(intsz), bf;
		for(StringMap::const_iterator it = map.begin(); it != map.end(); ++it){ const std::string& key = it->first;
			std::string str=it->second; bf.resize(key.size()+7+str.size()); bf[0]=0x02; insertCStr(bf, key, 1); 
			insertSz(bf, str.size()+1, key.size()+2); insertCStr(bf, str, key.size()+6); buff+=bf; }
		buff+=Buffer(1,0); insertSz(buff, buff.size(), 0, intsz); return buff; }
	
	std::string readCStr(const Buffer& buff, size_t& offset){ size_t start = offset; while(buff[offset]!=0){ offset++; }
		return buff.slice(start, offset++).toString(); }
	size_t readSz(const Buffer& buff, size_t offset, char sz=4){ 
		if(sz==4){ return buff.readUInt32LE(offset); }else{ return buff.readUInt64LE(offset); } offset+=sz; }
	
	Object parse(const Buffer& bson, bool ex=false, size_t offset=0){ const char intsz=ex?8:4; 
		if(bson.size()<5){ throw ncpp::Err("BSON::parse: This document is too small (<5 bytes)"); }
		size_t len = readSz(bson, offset, intsz); if(offset==0&&len!=bson.size()){ throw ncpp::Err("BSON::parse: Incorrect document: Invalid length - "+dtos(len)+"/"+dtos(bson.size())); }
		Object doc; const size_t end = offset+len; offset+=intsz; if(end > bson.size()){ throw ncpp::Err("BSON::parse: Buffer out of bounds."); }
        while(offset < end){ if(bson[offset]==0){ break; } char type = bson[offset++]; std::string key = readCStr(bson, offset); size_t tlen;
            switch(type){
                case 0x01: doc[key] = bson.readDoubleLE(offset); offset+=8; break; // Double
				case 0x02: { tlen = bson.readUInt32LE(offset); offset+=4; doc[key] = bson.slice(offset, offset+tlen-1).toString(); offset+=tlen; break; } // String
				case 0x03: doc[key] = parse(bson, ex, offset); offset+=readSz(bson, offset, intsz); break; // Object (Embedded document type)
				case 0x04: doc[key] = parse(bson, ex, offset); offset+=readSz(bson, offset, intsz); break; // Array
				case 0x05: { tlen = readSz(bson, offset, intsz); offset+=intsz+1; //char subtype=bson[offset++];
					doc[key] = bson.slice(offset, offset+tlen); offset+=tlen; break; } // Binary data
				case 0x06: doc[key].setUndefined(); break; // Undefined
				case 0x07: doc[key] = bson.slice(offset, offset+12).toString("hex"); offset+=12; break; // ObjectID
				case 0x08: if(bson[offset++]==0){ doc[key] = false; }else{ doc[key] = true; } break; // Boolean
                case 0x10: doc[key] = bson.readInt32LE(offset); offset+=4; break; // int32
				case 0x09: // DateTime (Unix Timestamp)
                case 0x12: doc[key] = bson.readInt64LE(offset); offset+=8; break; // int64
				case 0x11: doc[key]=Object(); doc[key]["t"]=bson.readInt32LE(offset+4); doc[key]["i"]=bson.readInt32LE(offset); offset+=8; break; // Timestamp (MongoDB 2x32bit numbers Timestamp)
				case 0x0A: doc[key].setNull(); break; // Null value
                default: throw ncpp::Err("BSON::parse: Unknown or unsupported BSON type: "+dtos(type));
            } } if(bson[offset++]!=0){ throw ncpp::Err("BSON::parse: Expected null terminator at end of document"); } return doc; }
}

namespace CBOR{ Buffer serialize(const Object& obj); Buffer serialize(const StringMap& map);
	void writeLen(Buffer& buff, unsigned long long len, unsigned char otp=0x00, size_t offset=0){
		//size_t requiredSize = offset + (len <= 23 ? 1 : (len <= 255 ? 2 : (len <= 65535 ? 3 : (len <= 4294967295 ? 5 : 9)))); 
		//if(buff.size()<requiredSize){ buff.resize(requiredSize); }
		
		if(len<=23){ if(buff.size()<offset+1){ buff.resize(offset+1); } buff[offset++]=otp+len; return; } otp+=23;
		if(len<=255){ if(buff.size()<offset+2){ buff.resize(offset+2); } buff[offset++]=otp+1; buff[offset++]=len; return; }
		else if(len<=65535){ if(buff.size()<offset+3){ buff.resize(offset+3); } buff[offset++]=otp+2; buff.writeUInt16BE(len, offset); offset+=2; return; }
		else if(len<=4294967295U){ if(buff.size()<offset+5){ buff.resize(offset+5); } buff[offset++]=otp+3; buff.writeUInt32BE(len, offset); offset+=4; return; }
		else{ if(buff.size()<offset+9){ buff.resize(offset+9); } buff[offset++]=otp+23+4; buff.writeUInt64BE(len, offset); return; } }
	
	Buffer encode(unsigned long long ll){ Buffer buff; writeLen(buff, ll); return buff; }
	Buffer encode(long long val){ if(val>=0){ return encode((unsigned long long)val); }
		else{ Buffer buff; writeLen(buff, val, 0x20); return buff; } }
	Buffer encode(int i){ return encode((long long)i); }
	Buffer encode(double d){ Buffer buff(9); buff[0]=0xFB; buff.writeDoubleBE(d, 1); return buff; }
	Buffer encode(const Buffer& bf){ Buffer buff; writeLen(buff, bf.size(), 0x40); buff+=bf; return buff; }
	Buffer encode(const String& str){ Buffer buff; writeLen(buff, str.size(), 0x60); buff+=Buffer(str); return buff; }
	Buffer encode(const std::string& str){ return encode(String(str)); }
	Buffer encode(const char* cstr){ return encode(String(cstr)); }
	Buffer encode(bool b){ return b?Buffer(1,0xF5):Buffer(1,0xF4); }
	Buffer encode(const Object& obj){ return serialize(obj); }
	Buffer encode(const StringMap& map){ return serialize(map); }
	Buffer encode(Variant val){ switch(val._type){ case Variant::DOUBLE: return encode((double)val.value.d);
		case Variant::INT32: return encode((long long)val.value.i); case Variant::INT64: return encode((long long)val.value.ll);
		case Variant::BOOLEAN: return encode(val.value.b); case Variant::STRING: return encode(val.toString());
		case Variant::BUFFER: return encode(val.toBuff()); case Variant::ARRAY: case Variant::OBJECT: return serialize(val.asObj()); 
		case Variant::NIL: return Buffer(1,0xF6); case Variant::UNDEFINED: default: return Buffer(1,0xF7); } }
	
	Buffer serialize(const Object& obj){ Buffer buff; writeLen(buff, obj.size(), 0xA0);
		for(Object::const_iterator it = obj.begin(); it != obj.end(); ++it){ const std::string& key = it->first; 
			buff+=encode(key)+encode(obj[key]); } return buff; }
		
	Buffer serialize(const StringMap& map){ Buffer buff; writeLen(buff, map.size(), 0xA0);
		for(StringMap::const_iterator it = map.begin(); it != map.end(); ++it){ const std::string& key = it->first; 
			buff+=encode(key)+encode(map.at(key)); } return buff; }
	
	size_t parseLen(const Buffer& buff, size_t& offset){ unsigned char otp=buff[offset]%32;
		if(otp<=23){ offset++; return otp; } size_t rst=offset+1; // Simple, <=0x17
		if(otp==0x18){ if(buff.size()<=offset+1){ offset++; return 0; } offset+=2; return buff[rst]; } //1 byte
		else if(otp==0x19){ if(buff.size()<=offset+2){ offset++; return 0; } offset+=3; return buff.readUInt16BE(rst); } //2 byte
		else if(otp==0x1A){ if(buff.size()<=offset+4){ offset++; return 0; } offset+=5; return buff.readUInt32BE(rst); } //4 byte 
		else if(otp==0x1B){ if(buff.size()<=offset+8){ offset++; return 0; } offset+=9; return buff.readUInt64BE(rst); }else{ return 0; } } //8 bytes 
	
	Object parse(const Buffer& cbor, size_t& offset);
	Variant parseValue(const Buffer& cbor, size_t& offset){ unsigned char type=cbor[offset]; size_t len; char ptype[2]; Array<char> bits = cbor.BitsRead(offset);
		Buffer::BitsWrite(&ptype[0], bits.slice(0,3)); Buffer::BitsWrite(&ptype[1], bits.slice(3));
		//std::cout << "CBOR::parseValue: type = " << std::hex << (int)type << std::dec << " (ptype: " << (int)ptype[0] << ", " << (int)ptype[1] << ")" << std::endl;
		
		if(type<=0x1F){ return parseLen(cbor, offset); } //Unsigned Integer
		else if(type>=0x20&&type<=0x3F){ return parseLen(cbor, offset)*-1; } //Negative Integer
		else if(type>=0x40&&type<=0x5F){ len=parseLen(cbor, offset); size_t rst=offset; offset+=len; return cbor.slice(rst, rst+len); }  //Byte String
		else if(type>=0x60&&type<=0x7F){ len=parseLen(cbor, offset); size_t rst=offset; offset+=len; return cbor.slice(rst, rst+len).toString(); } //Text String
		else if(type>=0x80&&type<=0x9F){ return parse(cbor, offset); //Array
		}else if(type>=0xA0&&type<=0xBF){ return parse(cbor, offset); //Map
		}else if(type>=0xC0&&type<=0xDF){ //Semantic Tag
		}else{ offset++; switch(type){ case 0xF4: return ObjectValue().set(false); case 0xF5: return ObjectValue().set(true); //type>=0xE0, Simple Value/Float
			case 0xF6: { return ObjectValue().setNull(); } case 0xF9: { size_t rst=offset; offset+=2; return cbor.readT_LE<float>(rst, 2); }
			case 0xFA: { size_t rst=offset; offset+=4; return cbor.readT_LE<float>(rst, 4); } case 0xFB: { size_t rst=offset; offset+=8; return cbor.readDoubleBE(rst); }
			case 0xF7: default: { return ObjectValue().setUndefined(); }
		} } return ObjectValue().setUndefined(); }
	Variant parseValue(const Buffer& cbor){ size_t offset=0; return parseValue(cbor, offset); }
	
	Object parse(const Buffer& cbor, size_t& offset){ Object obj; size_t indx=0, len=0; bool ismap=false; unsigned char type = cbor[offset];
		if(type>=0x80&&type<=0xBF){ len=parseLen(cbor,offset); } //Array or Map
		if(type>=0xA0&&type<=0xBF){ ismap=true; } //Map
		
		while((len<=0||indx<len)&&offset<cbor.size()&&type!=0xFF){ String key=ismap?"":dtos(indx);
			if(ismap){ key=parseValue(cbor, offset).toString(); }
			obj[key]=parseValue(cbor,offset); indx++; type = cbor[offset]; } return obj; }
	Object parse(const Buffer& cbor){ size_t offset=0; return parse(cbor, offset); }
}}

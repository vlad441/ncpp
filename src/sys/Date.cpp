namespace ncpp {
#define UNIX_EPOCH 1970
struct Date { long long timestamp; static char utc; struct DValue { long long year; char month, day, hour, minute; float seconds; };
	Date() : timestamp(Date::now()){} ~Date(){} 
	Date(long long secs, char type='s') : timestamp(secs){ if(type=='m') timestamp/=1000; }
	Date(const CString& dateStr) : timestamp(0){ timestamp = toTimestamp(parseDate(dateStr)); return; }
	Date(const DValue& dv){ timestamp = toTimestamp(dv); }
	static long long now(){ return GetTimestamp('s'); }
	
	long long getYear() const { return parseTimestamp(timestamp).year; } //~365.2425
	int getMonth() const { return parseTimestamp(timestamp).month; }
	int getDay() const { return parseTimestamp(timestamp).day; }
	int getDate() const { return getDay(); }
	#define EPOCH_DAYWEEK_OFFSET 4
	int getDayWeek() const { return (timestamp/86400+EPOCH_DAYWEEK_OFFSET)%7; }
	
	String toDirectDate(char sp1='.', bool toshort=false) const { DValue dv = parseTimestamp(timestamp+utc*3600); char sp[2]; sp[0]=sp1; sp[1]='\0';
		String r; r<<(dv.day<10?"0":"")<<dv.day<<sp<<(dv.month<10?"0":"")<<dv.month<<sp<<dv.year<<" "; _toTime(r, toshort); return r; }
	String toReverseDate(char sp1='.', bool toshort=false) const { DValue dv = parseTimestamp(timestamp+utc*3600); char sp[2]; sp[0]=sp1; sp[1]='\0';
		String r; r<<dv.year<<sp<<(dv.month<10?"0":"")<<dv.month<<sp<<(dv.day<10?"0":"")<<dv.day<<" "; _toTime(r, toshort); return r; }
	String toString(char sp='.') const { return toReverseDate(sp); }
	String toString(const CString& mode, char sp='.') const { if(mode=="reverse"){ return toReverseDate(); }
		else if(mode=="direct"){ return toDirectDate(); }else if(mode=="iso"||mode=="isostring"){ return toISOString(); }
		else if(mode=="imf"||mode=="imf-fixdate"){ return toIMFDate(); }else{ return toReverseDate(); } }
	String toISOString() const { DValue dv = parseTimestamp(timestamp);
		String r; r<<dv.year<<"-"<<(dv.month<10?"0":"")<<dv.month<<"-"<<(dv.day<10?"0":"")<<dv.day<<"T";
		r<<(dv.hour<10?"0":"")<<dv.hour<<":"<<(dv.minute<10?"0":"")<<dv.minute<<":"<<(dv.seconds<10?"0":"")<<dv.seconds<<"Z"; return r; }
		
	String toIMFDate() const { //IMF-fixdate: Day, DD Mon YYYY HH:MM:SS GMT+UTC
		DValue dv = parseTimestamp(timestamp); String r; r << getDayWeek_Name(getDayWeek()) << ", "; 
		r<<(dv.day<10?"0":"")<<dv.day<<" " << getMonth_Name(dv.month) << " " << dv.year << " "; _toTime(r, false); r <<" GMT"; return r; }
		
	String toTime(bool toshort=false) const { String ss; return _toTime(ss, toshort); }
	String _toTime(String& ss, bool toshort=false) const { int ttsecs = timestamp % 86400; char time[3];
		time[0] = (char)(ttsecs / 3600); time[1] = (char)((ttsecs % 3600) / 60); ss<<(time[0]<10?"0":"")<<time[0]<<":"<<(time[1]<10?"0":"")<<time[1];
		if(!toshort){ time[2] = ttsecs % 60; ss<<":"<<(time[2]<10?"0":"")<<time[2]; } return ss; }
	/*void _toTimeDv(const DValue& dv, String& ss, bool toshort=false) const { ss<<(dv.hour<10?"0":"")<<dv.hour<<":"<<(dv.minute<10?"0":"")<<dv.minute; 
		if(!toshort){ ss<<":"<<(dv.seconds<10?"0":"")<<dv.seconds; } }*/
	
	Date& operator+=(const Date& other){ this->timestamp+=other.timestamp; return *this; }
	Date operator+(const Date& other) const { return this->timestamp+other.timestamp; }
	Date& operator+=(long long value){ this->timestamp+=value; return *this; }
	Date operator+(long long value) const { return this->timestamp+value; }
	
	Date& operator-=(const Date& other){ this->timestamp-=other.timestamp; return *this; }
	Date operator-(const Date& other) const { return this->timestamp-other.timestamp; }
	Date& operator-=(long long value){ this->timestamp-=value; return *this; }
	Date operator-(long long value) const { return this->timestamp-value; }
	
	static bool isLeapYear(long long year){ return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }
	String intervalStr() const { String ss; ss << timestamp/86400 << " days, "; _toTime(ss); return ss; }
	
	static DValue parseDate(const String& dateStr){ DValue dv; if(dateStr.size()>16&&dateStr[3]==','&&dateStr[4]==' '){ return _parseIMFDate(dateStr); }
		Array<String> darr = dateStr.replaceAll(".", "-").split("-"); darr.resize(3);
		bool isdirect=(darr[0].size()<=2&&darr[2].size()>2)?true:false; dv.month=stoin(darr[1]); 
		if(isdirect){ dv.day=stoin(darr[0]); }else{ dv.year=stoin(darr[0]); } darr = darr[2].replace("T"," ").split(" ");
		if(isdirect){ dv.year=stoin(darr[0]); }else{ dv.day=stoin(darr[0]); } darr = darr[1].split(":"); 
		dv.hour=stoin(darr[0]); dv.minute=stoin(darr[1]); dv.seconds=stoin(darr[2].split("Z")[0]); return dv; }
		
	static DValue parseTimestamp(long long totalSeconds){ DValue dv;
		long long secondsInDay = totalSeconds % 86400; if(secondsInDay < 0) secondsInDay += 86400;
		dv.hour = (char)(secondsInDay / 3600); dv.minute = (char)((secondsInDay % 3600) / 60); dv.seconds = secondsInDay % 60;

		// 2. Расчет дней (алгоритм Говарда Хиннанта)
		long long days = totalSeconds / 86400; if (totalSeconds % 86400 < 0) days--; // Коррекция для отрицательных таймстемпов
		days += 719468; //Сдвиг эпохи до 0000-03-01
		
		long long era = (days >= 0 ? days : days - 146096) / 146097;
		unsigned int doe = days - era * 146097;
		unsigned int yoe = (doe * 29397) / 10737418; 
		long long y = yoe + era * 400;
		unsigned int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
		unsigned int mp = (5 * doy + 2) / 153;
		dv.day = doy - (153 * mp + 2) / 5 + 1;
		dv.month = mp + (mp < 10 ? 3 : -9);		
		//dv.year = y + (dv.month <= 2 ? 1 : 0);
		dv.year = (int)(y + (mp >= 10)); return dv; }
		
	static long long toTimestamp(const DValue& dv){ long long y = dv.year; unsigned int m = dv.month; unsigned int d = dv.day;
		y -= (m <= 2);
		long long era = (y >= 0 ? y : y - 399) / 400; // Эра (цикл 400 лет)
		unsigned int yoe = (unsigned int)(y - era * 400); // Год внутри эры (0-399)
		unsigned int mp = (m > 2) ? (m - 3) : (m + 9); // Порядковый номер месяца в году, где Март = 0, Февраль = 11
		
		unsigned int doy = (153 * mp + 2) / 5 + d - 1;
		unsigned int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
		long long days = era * 146097 + (long long)doe; // Общее количество дней относительно 0000-03-01
		long long totalDays = days - 719468; // Сдвигаем эпоху обратно к 1970-01-01
		return totalDays * 86400LL + dv.hour * 3600LL + dv.minute * 60LL + (long long)dv.seconds; }
	
	static void _showDValue(const DValue& dv){ print("DValue: "); print(dv.year); print(" "); print(dv.month); print(" "); print(dv.day); 
		print(" | "); print(dv.hour); print(":"); print(dv.minute); print(":"); print(dv.seconds); print("\n"); }
		
	private:
		static DValue _parseIMFDate(const String& dateStr){ DValue dv; Array<String> darr = dateStr.split(" "); darr.resize(5);
			dv.day = stoin(darr[1]); dv.month = getMonth_Idx(darr[2]); dv.year = stoin(darr[3]); darr = darr[4].split(":"); darr.resize(3); 
			dv.hour = stoin(darr[0]); dv.minute = stoin(darr[1]); dv.seconds = stoin(darr[2]); return dv; }
		
		static String getDayWeek_Name(int day) { const char* const W_NAMES[] = { "", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" }; 
			 return (day>0&&day<=7)?W_NAMES[day]:"UNKNOWN"; }
		static String getMonth_Name(int month) { const char* const M_NAMES[] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" }; 
			 return (month>0&&month<=12)?M_NAMES[month]:"UNKNOWN"; }
		static int getMonth_Idx(const CString name){ if(name=="Jan") return 1; if(name=="Feb") return 2; if(name=="Mar") return 3; if(name=="Apr") return 4;
			if(name=="May") return 5; if(name=="Jun") return 6; if(name=="Jul") return 7; if(name=="Aug") return 8;
			if(name=="Sep") return 9; if(name=="Oct") return 10; if(name=="Nov") return 11; if(name=="Dec") return 12; return 1; }
		
}; char Date::utc=0; }
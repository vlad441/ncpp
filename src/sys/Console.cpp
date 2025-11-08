namespace ncpp{ struct Console {
	void log(const char* c){ String ss(c); ss+='\n'; print(ss); }
	void log(const CString& cs){ String ss(cs); ss+='\n'; print(ss); }
	String input(){ return readline(); }
	
	Console& operator+=(const char* c){ print(c); return *this; }
	template <typename T, typename D>
	Console& operator+=(const BaseString<T, D>& s){ print(s); return *this; }
	
	template <typename T> Console& operator<<(T val){ print(val); return *this; }
	
	Console& operator>>(String& s){ s=readline(); return *this; }
	Console& operator>>(int& i){ i=stoin(readline()); return *this; }
	Console& operator>>(long long& ll){ ll=stolln(readline()); return *this; }
	Console& operator>>(double& d){ d=stodn(readline()); return *this; }
	Console& operator>>(float& f){ f=stofn(readline()); return *this; }
	
	#ifdef _WIN32
	Console(){ hConsole = GetStdHandle(STD_OUTPUT_HANDLE); CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo); _base_attr = consoleInfo.wAttributes; }
	~Console(){ reset(); }
	
	Console& setColor(char color){ if(color > 7 || color < 0) return *this;
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo; GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		SetConsoleTextAttribute(hConsole, (consoleInfo.wAttributes & 0xFFF0) | win_colors[(int)color]); return *this; }
	Console& setBGColor(char color){ if(color > 7 || color < 0) return *this;
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo; GetConsoleScreenBufferInfo(hConsole, &consoleInfo); // Получаем текущие атрибуты
		SetConsoleTextAttribute(hConsole, (consoleInfo.wAttributes & 0xFF0F) | (win_colors[(int)color] << 4)); return *this; }
	Console& setFont(char font){ CONSOLE_SCREEN_BUFFER_INFO consoleInfo; GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		WORD new_attributes = consoleInfo.wAttributes; //коды 2, 3, 4 не поддерживаются в консоли Windows
		switch(font){ case 0: { reset(); break; } case 1: new_attributes |= FOREGROUND_INTENSITY; break; // Включаем яркость ("жирный")
			default: break; } SetConsoleTextAttribute(hConsole, new_attributes); return *this; }
	Console& reset(){ SetConsoleTextAttribute(hConsole, _base_attr); return *this; }
	Console& clear(){ ::system("cls"); return *this; }
	Console& clearLn(){ *this << "\r"; CONSOLE_SCREEN_BUFFER_INFO csbi; DWORD charsWritten; COORD current_pos;
		if(!GetConsoleScreenBufferInfo(hConsole, &csbi)){ return *this; }
		current_pos = csbi.dwCursorPosition; DWORD cellsToClear = csbi.dwSize.X - current_pos.X;
		if(!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', cellsToClear, current_pos, &charsWritten)){ return *this; }
		if(!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellsToClear, current_pos, &charsWritten)){ return *this; }
		SetConsoleCursorPosition(hConsole, current_pos); return *this; }
	//Расширенная цветовая палитра недоступна в Windows cmd.exe
	Console& setColorEx(unsigned char color){ return *this; } Console& setColorEx(unsigned char r, unsigned char g, unsigned char b){ return *this; }
	
	private: HANDLE hConsole; WORD _base_attr; static const WORD win_colors[8];
	#else
	Console& setColor(char color){ String ss("\033[3"); ss.push('0'+color); ss << "m"; print(ss); return *this; }
	Console& setColorEx(unsigned char color){ String ss("\033[38;5;"); ss << dtos(color) << "m"; print(ss); return *this; }
	//Console& setColorEx(unsigned int hex);
	Console& setColorEx(unsigned char r, unsigned char g, unsigned char b){ String ss("\033[38;2;"); 
		ss << dtos(r) << ";" << dtos(g) << ";" << dtos(b) << "m"; print(ss); return *this; }
	Console& setBGColor(char color){ String ss("\033[4"); ss.push('0'+color); ss << "m"; print(ss); return *this; }
	Console& setFont(char font){ String ss("\033["); ss.push('0'+font); ss << "m"; print(ss); return *this; }
	Console& reset(){ print("\033[0m"); return *this; }
	Console& clear(){ print("\033[2J\033[H"); fflush(stdout); return *this; }
	Console& clearLn(){ print("\r\033[K"); fflush(stdout); return *this; }
	#endif
}; 
#ifdef _WIN32
const WORD Console::win_colors[8] = { 0, FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE, FOREGROUND_GREEN | FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
#endif

}

// Цвета:
// 0	Черный
// 1	Красный
// 2	Зеленый
// 3	Желтый
// 4	Синий
// 5	Пурпурный
// 6	Голубой
// 7	Белый

// Шрифт:
// 0	Сброс всех атрибутов
// 1	Жирный/Яркий
// 2	Тусклый (не всегда поддерживается)
// 3	Курсив (не всегда поддерживается)
// 4	Подчеркнутый (не всегда поддерживается)

#ifdef NCPP_COUT_COLORED
namespace ncpp{
//template <> Console& Console::operator<<(const Object& val);
//template <> Console& Console::operator<<(const Array<T> val);
template <> Console& Console::operator<<(long long ll){ this->setColor(3); print(ll); this->reset(); return *this; }
//template <> Console& Console::operator<<(int ll){ return Console::operator<<((long long)ll); }
}
#endif
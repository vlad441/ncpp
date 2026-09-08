	#if __cplusplus >= 201103L //move-semantics for C++11
	#ifdef _MOVE_T_NOCOPY
	_MOVE_T(const _MOVE_T&) = delete; _MOVE_T& operator=(const _MOVE_T&) = delete; //Запрет копирования.
	#endif
	
	_MOVE_T(_MOVE_T&& tmp) noexcept { move(*this, tmp); }
	_MOVE_T& operator=(_MOVE_T&& tmp) noexcept { move(*this, tmp); return *this; }
	_MOVE_T& steal(_MOVE_T& tmp){ move(*this, tmp); return *this; }
	_MOVE_T& steal(_MOVE_T&& tmp){ move(*this, tmp); return *this; }
	friend void move(_MOVE_T& dst, _MOVE_T&& tmp){ move(dst, (_MOVE_T&)tmp); }
	#else //move for C++98
	#ifdef _MOVE_T_NOCOPY
	private: _MOVE_T(const _MOVE_T&); _MOVE_T& operator=(const _MOVE_T&); public: //Скрытие копирования.
	#endif
	
	_MOVE_T& steal(const _MOVE_T& victim){ move(*this, (_MOVE_T&)victim); return *this; }
	friend void move(_MOVE_T& dst, const _MOVE_T& victim){ move(dst, (_MOVE_T&)victim); }
	#endif
	//friend void move(_MOVE_T& dst, _MOVE_T& tmp);
#undef _MOVE_T
#ifdef _MOVE_T_NOCOPY
#undef _MOVE_T_NOCOPY
#endif
#ifndef NCPP_INT128
#define NCPP_INT128
template <typename T>
struct __my_int128 { uint64_t low; T high; typedef __my_int128<T> _int128_t;
	__my_int128(){}
	__my_int128(int i) : low(i), high(0){}
	__my_int128(unsigned long long ll) : low(ll), high(0){}
	__my_int128& operator=(unsigned long long ll){ low=ll; high=0; return *this; }
	
	_int128_t operator+(const _int128_t& rhs) const { _int128_t result;
		result.low = low + rhs.low; uint64_t carry = (result.low < low);
		result.high = high+rhs.high+carry; return result; }
	_int128_t operator+(int val) const { return operator+(_int128_t(val)); }
	_int128_t& operator+=(const _int128_t& rhs){ uint64_t old_low = low; 
		low += rhs.low; uint64_t carry = (this->low < old_low);
		high += rhs.high+carry; return *this; }
	_int128_t& operator+=(int val) { return operator+=(_int128_t(val)); }
	friend inline _int128_t operator+(uint64_t val, const _int128_t& rhs){ return rhs + val; }
	_int128_t& operator++(){ *this+=1; return *this; } // prefix: ++a
	_int128_t operator++(int){ _int128_t tmp(*this); *this+=1; return tmp; } // postfix: a++
		
	_int128_t operator-(const _int128_t& rhs) const { _int128_t result;
		result.low = low - rhs.low; uint64_t borrow = (result.low > low);
		result.high = high-rhs.high-borrow; return result; }
	_int128_t operator-(int val) const { return operator-(_int128_t(val)); }
	_int128_t& operator-=(const _int128_t& rhs){ uint64_t old_low = low; 
		low -= rhs.low; uint64_t borrow = (low > old_low);
		high -= rhs.high-borrow; return *this; }
	_int128_t& operator-=(int val) { return operator-=(_int128_t(val)); }
	_int128_t& operator--(){ *this-=1; return *this; } // prefix: --a
	_int128_t operator--(int){ _int128_t tmp(*this); *this-=1; return tmp; } // postfix: a--
	
	_int128_t operator*(const _int128_t& rhs) const { return mul_128x128_128(*this, rhs); }
	_int128_t operator*(uint64_t val) const { return mul_128x64_128(*this, val); }
	_int128_t operator*(unsigned int val) const { _int128_t m=*this; return m*=val; }
	_int128_t operator*(int val) const { return operator*((unsigned int)val); }
	friend inline _int128_t operator*(uint64_t val, const _int128_t& rhs){ return rhs * val; }
	_int128_t& operator*=(const _int128_t& rhs){ *this = *this * rhs; return *this; }
	_int128_t& operator*=(uint64_t val){ *this = *this * val; return *this; }
	_int128_t& operator*=(unsigned int val){ mul_128x32(*this, val); return *this; }
	_int128_t& operator*=(int val){ return operator*=((unsigned int)val); }
	
	_int128_t operator/(const _int128_t& rhs) const;
	_int128_t operator/(unsigned int val) const { _int128_t q=*this; divmod_128x32(q, val); return q; }
	_int128_t operator/(int val) const { return operator/((unsigned int)val); }
	_int128_t& operator/=(const _int128_t& rhs);
	_int128_t& operator/=(unsigned int val){ divmod_128x32(*this, val); return *this; }
	_int128_t& operator/=(int val){ return operator/=((unsigned int)val); }
	
	_int128_t operator%(const _int128_t& rhs) const;
	unsigned int operator%(unsigned int val) const { _int128_t q=*this; return divmod_128x32(q, val); }
	unsigned int operator%(int val) const { return operator%((unsigned int)val); }
	_int128_t& operator%=(const _int128_t& rhs);
	_int128_t& operator%=(unsigned int val){ *this=divmod_128x32(*this, val); return *this; }
	_int128_t& operator%=(int val){ return operator%=((unsigned int)val); }
	
	bool operator==(const _int128_t& rhs) const { return (high == rhs.high) && (low == rhs.low); }
	bool operator==(int val) const { return (high == 0) && (low == (uint64_t)val); }
	
	bool operator!=(const _int128_t& rhs) const { return (high != rhs.high) || (low != rhs.low); }
	bool operator!=(int val) const { return (high != 0) || (low != (uint64_t)val); }
	
	bool operator<(const _int128_t& rhs) const { if(high != rhs.high){ return high < rhs.high; } return low < rhs.low; }
	bool operator<(int val) const { return high>0?false:low<(uint64_t)val; }
	
	bool operator>(const _int128_t& rhs) const { if(high != rhs.high){ return high > rhs.high; } return low > rhs.low; }
	bool operator>(int val) const { return high>0?true:low>(uint64_t)val; }
	
	bool operator<=(const _int128_t& rhs) const { if(high != rhs.high){ return high < rhs.high; } return low <= rhs.low; }
	bool operator<=(int val) const { return high>0?false:low<=(uint64_t)val; }
	
	bool operator>=(const _int128_t& rhs) const { if(high != rhs.high){ return high > rhs.high; } return low >= rhs.low; }
	bool operator>=(int val) const { return high>0?true:low>=(uint64_t)val; }
	
	
	_int128_t& operator<<=(unsigned int bits){ return _shift_left(bits); }
	_int128_t operator<<(unsigned int bits) const { _int128_t result = *this; return (result <<= bits); }
	_int128_t& operator>>=(unsigned int bits){ return _shift_right(bits); }
	_int128_t operator>>(unsigned int bits) const { _int128_t result = *this; return (result >>= bits); }
		
	_int128_t operator~() const { _int128_t result; result.low = ~low; result.high = ~high; return result; }
	_int128_t operator-() const { return (~*this)+1; }
	
	#if __cplusplus >= 201103L
	explicit operator unsigned long long() const { return low; }
	explicit operator long long() const { return low; }
	#else
	//operator unsigned long long() const { return low; }
	operator long long() const { return low; }
	#endif
	
	//explicit operator __my_int128<unsigned long long>() const {}
	//explicit operator __my_int128<long long>() const {}
	
	static void mul_128x32(_int128_t& a, unsigned int value){ uint64_t carry = 0;
		unsigned int A[4]; A[0] = a.low; A[1] = (a.low >> 32); A[2] = a.high; A[3] = (a.high >> 32); // a = a0 -> a3
		for(size_t i = 0; i < 4-1; i++){ uint64_t product = (uint64_t)A[i] * value + carry; A[i] = product & UINT_MAX; carry = product >> 32; }
		a.low = ((uint64_t)A[1] << 32) | (uint64_t)A[0]; a.high = ((uint64_t)A[3] << 32) | (uint64_t)A[2]; }
	
	static unsigned int divmod_128x32(_int128_t& a, unsigned int value){
		if(value == 0){ print("(!) uint128_t: Matan rule 0: Never divide by zero :)"); return 0; } uint64_t r = 0;
		unsigned int A[4]; A[0] = a.low; A[1] = (a.low >> 32); A[2] = a.high; A[3] = (a.high >> 32); // a = a0 -> a3
		for(int i = 4-1; i >= 0; --i){ uint64_t current = (r << 32) | A[i]; A[i] = current / value; r = current % value; }
		a.low = ((uint64_t)A[1] << 32) | (uint64_t)A[0]; a.high = ((uint64_t)A[3] << 32) | (uint64_t)A[2]; return r; }
	
	static _int128_t mul_64x64_128(uint64_t a, uint64_t b){
		const uint64_t MASK = 0xFFFFFFFFULL; // 1. Разбиение на 32-битные части (нижние/верхние)
		
		uint32_t a0 = (uint32_t)a; uint32_t a1 = (uint32_t)(a >> 32); // a = a1:a0 
		uint32_t b0 = (uint32_t)b; uint32_t b1 = (uint32_t)(b >> 32); // b = b1:b0

		// 2. Четыре произведения 32x32 = 64
		uint64_t p00 = (uint64_t)a0 * b0; // P(0-63) - Базовая часть low
		uint64_t p01 = (uint64_t)a0 * b1; // P(32-95) - Средняя
		uint64_t p10 = (uint64_t)a1 * b0; // P(32-95) - Средняя
		uint64_t p11 = (uint64_t)a1 * b1; // P(64-127) - Базовая часть high

		// 3. Сложение и обработка переносов (самое уродливое)
		uint64_t L_high = (p00 >> 32) + (p01 & MASK) + (p10 & MASK); uint64_t carry = L_high >> 32;

		_int128_t result; result.low = (p00 & MASK) | (L_high << 32); // Младшие 64 бита (P00_low + L_high_low)
		result.high = p11 + (p01 >> 32) + (p10 >> 32) + carry; // Старшие 64 бита (P11 + Верхние 32 бита P01 и P10 + carry) 
		return result; }
		
	static _int128_t mul_128x64_128(const _int128_t& a, uint64_t value){ _int128_t result;
		_int128_t P0 = mul_64x64_128(a.low, value); result.low = P0.low; result.high = P0.high;
		_int128_t P1 = mul_64x64_128(a.high, value); result.high += P1.low; return result; }
	
	static _int128_t mul_128x128_128(const _int128_t& a, const _int128_t& b){ _int128_t result;
		_int128_t P0 = mul_64x64_128(a.low, b.low); result.low = P0.low; result.high = P0.high;
		_int128_t P1 = mul_64x64_128(a.low, b.high); _int128_t P2 = mul_64x64_128(a.high, b.low);
		// 5. Суммируем P0_high, P1_low и P2_low, отслеживая перенос (carry).
		uint64_t old_high = result.high; // Сложение P0_high + P1_low:
		result.high += P1.low; uint64_t carry = (result.high < old_high); // Перенос (если был, он выходит за 128 бит и отбрасывается)
		// Сложение (P0_high + P1_low) + P2_low:
		old_high = result.high; result.high += P2.low; //carry += (result.high < old_high); // Финальный 'carry' (который выходит за 128 бит) игнорируется.
		return result; }
	
	static _int128_t divmod_128x128_128(const _int128_t& a, const _int128_t& b, _int128_t& r);
	static _int128_t divmod_128x128_128(const _int128_t& a, const _int128_t& b){ _int128_t r; return divmod_128x128_128(a, b, r); }
	
	_int128_t& _shift_left(int bits){ if(bits==0||*this==0) return *this; for(int i = 0; i < bits; ++i){ *this*=2; } return *this; }
	_int128_t& _shift_right(int bits){ if(bits==0||*this==0) return *this; for(int i = 0; i < bits; ++i){ *this/=2; } return *this; }
	
	Buffer _toBuff() const { Buffer bf(this, sizeof(_int128_t)); bf.reverse(); return bf; }
	void _print(char sep=0) const { char num[52]; dtos(num, *this, sep); print(num); } //max "340'282'366'920'938'463'463'374'607'431'768'211'455\0" - 52 ch
	void _print2() const { print(_toBuff().toIntString()); }
};
typedef __my_int128<unsigned long long> uint128_t;
typedef __my_int128<long long> int128_t;
#endif
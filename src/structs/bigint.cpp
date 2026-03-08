namespace ncpp{
#if __SIZEOF_INT128__
    typedef __int128 int128_t;
	typedef unsigned __int128 uint128_t;
    //#define CARRY_MAX (CARRY_T)-1 // VS ~((CARRY_T)0)
#else
	#include "../experimental/__int128.cpp"
#endif
	//TODO: умножение Карацубы, modular exponential Монтгомери
	struct BigInt { //(https://gmplib.org/manual/Algorithms)
	#if __SIZEOF_INT128__
		typedef unsigned long long ELEM_T; typedef uint128_t CARRY_T; typedef int128_t DIFF_T;
		static const ELEM_T ELEM_MAX = 0xFFFFFFFFFFFFFFFF; //ULLONG_MAX;
		static const CARRY_T ELEM_BASE = (CARRY_T)ELEM_MAX+1ULL;
	#else
		typedef unsigned int ELEM_T; typedef unsigned long long CARRY_T; typedef long long DIFF_T;
		static const ELEM_T ELEM_MAX = 0xFFFFFFFF; //UINT_MAX;
		static const unsigned long long ELEM_BASE = UINT_MAX+1ULL;
	#endif
		static const unsigned char ELEM_SZ = sizeof(ELEM_T);
		Array<ELEM_T> parts; bool sign;
		BigInt() : sign(true){}
		~BigInt(){} // Деструктор
        BigInt(const CString& value, const CString& type="dec") : sign(true){ if(type=="hex"){ _fromHex(value); }else{ _fromIntString(value); } }
        BigInt(const char* cstr) : sign(true){ _fromIntString(cstr); }
		BigInt(const String& s) : sign(true){ _fromIntString(s); }
        BigInt(unsigned int i) : sign(true){ set(i); }
		BigInt(unsigned long long ll) : sign(true){ set(ll); }
		BigInt(int i){ set(i); }
		BigInt(long long ll){ set(ll); }
		BigInt(const Buffer& bf) : sign(true){ set(bf); }
		
		BigInt& _fromIntString(const CString& cs){ sign=(cs.size()>0&&cs[0]=='-')?false:true; set(Buffer::fromInt(cs)); return *this; }
		BigInt& _fromHex(const CString& cs){ sign=(cs.size()>0&&cs[0]=='-')?false:true; set(Buffer::fromHex(cs)); return *this; }
		BigInt& _fromBuffLE(const Buffer& bf){ parts.resize(bf.size()/ELEM_SZ+1); if(bf.size()%ELEM_SZ==0) parts.resize(parts.size()-1); 
			memcpy((char*)parts.data(), bf.data(), bf.size()); return *this; }
		BigInt& _fromBuffBE(Buffer bf){ bf.reverse(); return _fromBuffLE(bf); }
			
		static BigInt fromInt(unsigned long long ll){ BigInt bint; bint.set(ll); return bint; }
		static BigInt fromInt(const CString& cs){ return BigInt()._fromIntString(cs); }
		static BigInt fromHex(const CString& cs){ return BigInt()._fromHex(cs); }
		static BigInt fromBuffLE(const Buffer& bf){ return BigInt()._fromBuffLE(bf); }
		static BigInt fromBuffBE(const Buffer& bf){ return BigInt()._fromBuffBE(bf); }
		
		void set(unsigned int i){ parts.clear(); parts.push(i); }
		void set(unsigned long long ll){ parts.clear(); if(ELEM_SZ>=8){ parts.push(ll); return; } parts.push((ll & 0xFFFFFFFFULL)).push((ll >> 32)); }
		void set(int i){ parts.clear(); if(i>=0){ parts.push(i); sign=true; }else{ parts.push(-i); sign=false; } }
		void set(long long ll){ if(ll>=0){ set((unsigned long long)ll); sign=true; }else{ set((unsigned long long)(-ll)); sign=false; } }
		void set(const CString& cs){ _fromIntString(cs); }
		void set(const Buffer& bf){ _fromBuffBE(bf); }
		BigInt& operator=(unsigned int i){ set(i); return *this; }
		BigInt& operator=(unsigned long long ll){ set(ll); return *this; }
		BigInt& operator=(int i){ set(i); return *this; }
		BigInt& operator=(long long ll){ set(ll); return *this; }
		BigInt& operator=(const char* cstr){ set(CString(cstr)); return *this; }
		BigInt& operator=(const String& s){ set(CString(s)); return *this; }
		BigInt& operator=(const CString& cs){ set(cs); return *this; }
		BigInt& operator=(const Buffer& bf){ set(bf); return *this; }
		
		size_t size() const { return parts.size()*ELEM_SZ; }
		size_t bits() const { return size()*8; }
		bool IsEven() const { return (parts.front() & 1) == 0; }
		bool IsZero() const { return parts.empty()||(parts.size()==1&&parts.front()==0); }
		BigInt& negate(){ sign?sign=false:sign=true; return *this; }
		BigInt& abs(){ if(!sign) sign=true; return *this; }
		// Вывод
		Buffer toBuffLE() const { Buffer bf(parts.size()*ELEM_SZ); memcpy(bf.data(), (char*)parts.data(), bf.size());
			Buffer::removeLastZeros(bf); return bf; }
		Buffer toBuffBE() const { Buffer bf(parts.size()*ELEM_SZ); memcpy(bf.data(), (char*)parts.data(), bf.size());
			Buffer::removeLastZeros(bf); bf.reverse(); return bf; }
		Buffer toBuff() const { return toBuffBE(); }
		String toIntString() const { return (sign?"":"-")+toBuff().toIntString(); }
		String toHex() const { return (sign?"":"-")+toBuff().toHex(); }
		String toString() const { return toIntString(); }
		String toString(const CString& type) const { if(type=="hex"){ return toHex(); }
			else if(type=="int"||type=="number"||type=="dec"){ return toIntString(); }
			else if(type=="raw"){ return parts.cout(); }else if(type=="buff"){ return toBuff().cout(); }else{ return toIntString(); } }
			
		operator Buffer(){ return this->toBuff(); }
			
		static BigInt random(size_t bits){ return BigInt(Buffer::randBytes(bits/8)); } // Fast and Unsafe
		static BigInt safeRandom(size_t bits); // Slower and Safe
		
		//=== simple matan ===
		static void increment(BigInt& bint, unsigned int value=1, bool insert = true){ Array<ELEM_T>& arr = bint.parts;
			if(value==1&&!insert){ for(size_t j = 0; j < arr.size(); j++){ if(++arr[j] != 0) return; } return; } CARRY_T carry = value;
			for(size_t i = 0; i < arr.size(); i++){ CARRY_T sum = (CARRY_T)arr[i] + carry; arr[i] = sum & ELEM_MAX; carry = sum >> (ELEM_SZ*8); }
			if(insert && carry != 0){ while(carry != 0){ arr.push(carry & ELEM_MAX); carry >>= (ELEM_SZ*8); } } }

		static void decrement(BigInt& bint, unsigned int value=1, bool erase = true){ Array<ELEM_T>& arr = bint.parts; CARRY_T borrow = value;
			if(value==1&&!erase){ for(size_t j = 0; j < arr.size(); j++){ if(--arr[j] != ELEM_MAX) return; } return; }
			for(size_t i = 0; i < arr.size(); i++){ DIFF_T diff = arr[i] - borrow; arr[i] = (diff + ELEM_BASE) & ELEM_MAX;
				borrow = (diff < 0) ? 1 : 0; if(borrow == 0) break; //value >>= (ELEM_SZ*8); borrow += (value & ELEM_MAX);
			} if(erase){ if(borrow > 0){ arr.fill(0); } removeLeadingZeros(bint); } }
		
		static void multiply_single(BigInt& bint, unsigned int value, bool insert = true){ CARRY_T carry = 0; Array<ELEM_T>& arr = bint.parts;
			for(size_t i = 0; i < arr.size(); i++){ CARRY_T product = (CARRY_T)arr[i] * value + carry; arr[i] = product & ELEM_MAX; carry = product >> (ELEM_SZ*8); }
			if(insert && carry != 0){ while(carry != 0){ arr.push(carry & ELEM_MAX); carry >>= (ELEM_SZ*8); } } }

		static unsigned int divide_single(BigInt& bint, unsigned int value, bool erase = true){
			if(value == 0){ print("(!) Matan rule 0: Never divide by zero :)"); return 0; } CARRY_T r = 0; Array<ELEM_T>& arr = bint.parts;
			for(int i = arr.size() - 1; i >= 0; --i){ CARRY_T current = (r << (ELEM_SZ*8)) | arr[i]; 
				arr[i] = current / value; r = current % value; } if(erase){ removeLeadingZeros(bint); } return r; }

		static char compare(const BigInt& a, const BigInt& b){
			size_t a_len = a.parts.size(); while(a_len > 0 && a.parts[a_len-1] == 0){ --a_len; }
			size_t b_len = b.parts.size(); while(b_len > 0 && b.parts[b_len-1] == 0){ --b_len; }
			if(a_len > b_len){ return 1; } if(a_len < b_len){ return -1; }
			for(int i = a.parts.size() - 1; i >= 0; --i){
				if(a.parts[i] > b.parts[i]){ return 1; }
				if(a.parts[i] < b.parts[i]){ return -1; }
			} return 0; }
		// --- --- ---
		static void _add(BigInt& a, const BigInt& b, bool resize = true){ Array<ELEM_T>& arr_a = a.parts; const Array<ELEM_T>& arr_b = b.parts;
			const size_t ARR_SZ = max(arr_a.size(), arr_b.size()); if(arr_a.size() < ARR_SZ) arr_a.resize(ARR_SZ, 0); CARRY_T carry = 0;
			for(size_t i = 0; i < arr_b.size(); ++i){ CARRY_T sum = (CARRY_T)arr_a[i] + arr_b[i] + carry;
				arr_a[i] = (ELEM_T)(sum & ELEM_MAX); carry = sum >> (ELEM_SZ*8); }
			if(resize && carry != 0){ while(carry != 0){ arr_a.push(carry & ELEM_MAX); carry >>= (ELEM_SZ*8); } } }
		static BigInt add(const BigInt& a, const BigInt& b, bool resize = true){ BigInt a1=a; _add(a1, b, resize); return a1; }
		
		static void _subtract(BigInt& a, const BigInt& b, bool resize = true){ Array<ELEM_T>& arr_a = a.parts; const Array<ELEM_T>& arr_b = b.parts;
			const size_t ARR_SZ = max(arr_a.size(), arr_b.size()); if(arr_a.size() < ARR_SZ) arr_a.resize(ARR_SZ, 0); DIFF_T borrow = 0; // 0 или 1
			for(size_t i = 0; i < arr_b.size(); ++i){ DIFF_T diff = (DIFF_T)arr_a[i] - arr_b[i] - borrow;
				arr_a[i] = (ELEM_T)((diff + ELEM_BASE) & ELEM_MAX); borrow = (diff < 0) ? 1 : 0; }
			for(size_t i = arr_b.size(); i < arr_a.size() && borrow; ++i){ DIFF_T diff = (DIFF_T)arr_a[i] - borrow;
				arr_a[i] = (ELEM_T)((diff + ELEM_BASE) & ELEM_MAX);	borrow = (diff < 0) ? 1 : 0; }
			if(resize) removeLeadingZeros(a); }
		static BigInt subtract(const BigInt& a, const BigInt& b, bool resize = true){ BigInt a1=a; _subtract(a1, b, resize); return a1; }
		
		static BigInt multiply(const BigInt& a, const BigInt& b, bool resize = true){ return multiply_native(a, b, resize); }
		
		static BigInt divide(const BigInt& a, const BigInt& b, bool resize = true){ return divmod(a, b).first; }
		static BigInt mod(const BigInt& a, const BigInt& b, bool resize = true){ return divmod(a, b).second; }
		static Pair<BigInt,BigInt> divmod(const BigInt& a, const BigInt& b, bool resize = true){
			if(b.IsZero()){ return Pair<BigInt,BigInt>(0, 0); } char cmp_ab = compare(a, b);
			if(cmp_ab < 0){ return Pair<BigInt,BigInt>(0, a); }
			if(cmp_ab == 0){ return Pair<BigInt,BigInt>(1, 0); }
			//if(b.parts.size()==1){ BigInt q=a; ELEM_T r = divide_single(q, b.parts[0]); return Pair<BigInt,BigInt>(q, r); }
			return divmod_binary(a, b, resize); }
		
		// "Нативный" (школьный) алгоритм: O(N^2)
		static BigInt multiply_native(const BigInt& a, const BigInt& b, bool resize = true){ const Array<ELEM_T>& arr_a = a.parts; const Array<ELEM_T>& arr_b = b.parts;
			const size_t ARR_SZ = arr_a.size()+arr_b.size(); BigInt resp; Array<ELEM_T>& result = resp.parts; result.resize(ARR_SZ, 0);
			// Основной цикл (школьный алгоритм): O(N^2)
			for(size_t i = 0; i < arr_a.size(); ++i){ CARRY_T carry = 0;
				for(size_t j = 0; j < arr_b.size(); ++j){ CARRY_T product = (CARRY_T)arr_a[i] * arr_b[j] + (CARRY_T)result[i + j] + carry;                        
					result[i + j] = (ELEM_T)(product & ELEM_MAX); carry = product >> (ELEM_SZ*8); }
				if(carry != 0){ result[i + arr_b.size()] = (ELEM_T)carry; } }
			if(resize){ removeLeadingZeros(resp); } return resp; }
			
		//Binary Division (Сдвиги и Вычитания): O(N^2)
		static Pair<BigInt,BigInt> divmod_binary(const BigInt& a, const BigInt& b, bool resize = true){
			//print("(#DEBUG) divmod_binary: "); print(a.toString()); print("/"); print(b.toString()); print(" ==\n");
			BigInt remainder = a; BigInt quotient(0); BigInt temp_b = b; unsigned int shifts = 0;
			while(compare(remainder, temp_b) >= 0){ temp_b <<= 1; shifts++; } temp_b >>= 1; shifts--;

			for(unsigned int i = 0; i <= shifts; ++i){
				if(compare(remainder, temp_b) >= 0){
					_subtract(remainder, temp_b, true); 
					BigInt bit_val(1); bit_val <<= (shifts - i);
					_add(quotient, bit_val, true); 
				} temp_b >>= 1; }
				
			if(resize){ removeLeadingZeros(quotient); removeLeadingZeros(remainder); } 
			return Pair<BigInt,BigInt>(quotient, remainder); }
			
		//=== advanced matan ===
		static BigInt multiply_karatsuba(const BigInt& a, const BigInt& b, bool resize = true);
		
		//Knuth’s Algorithm D (https://skanthak.hier-im-netz.de/division.html) - Не реализован :(
		static Pair<BigInt,BigInt> divmod_knuth(const BigInt& a, const BigInt& b, bool resize = true);
		
		BigInt pow(BigInt base, BigInt exp){ BigInt result(1);
			while(exp > 0){ if(exp % 2 == 1){ result = (result * base); }
				base = (base * base); exp/=2; } return result; }
		static BigInt powMod(BigInt base, BigInt exp, const BigInt& mod){ BigInt result(1);
			//print("(#DEBUG) BigInt::powMod() - "); print("(mod bits: "); print(mod.size()*8); print("): ");  print(base.toString()); print(" ^ "); print(exp.toString()); print(" % "); print(mod.toString()); print("\n");
			while(exp > 0){ if(exp % 2 == 1){ result = (result * base) % mod; }
				base = (base * base) % mod; exp /= 2; } return result; }
		// Перегрузки
		BigInt& operator+=(const BigInt& other){ if(sign == other.sign){ _add(*this, other, true); }else{ 
				if(compare(*this, other) >= 0){ _subtract(*this, other, true); }
				else{ *this=subtract(other, *this, true); sign = other.sign; } } return *this; }
		BigInt& operator+=(unsigned int value){ if(sign){ increment(*this, value, true); }
			else{ decrement(*this, value, true); if(IsZero()) sign = true; } return *this; }
		BigInt operator+(const BigInt& other) const { BigInt result = *this; result += other; return result; }
		BigInt operator+(unsigned int value) const { BigInt result = *this; result += value; return result; }
		BigInt& operator++(){ *this+=1; return *this; } // prefix: ++a
		BigInt operator++(int){ BigInt tmp(*this); *this+=1; return tmp; } // postfix: a++
		

		BigInt& operator-=(const BigInt& other){ if(sign != other.sign){ _add(*this, other, true); sign=!other.sign; }else{
				if(compare(*this, other) >= 0){ _subtract(*this, other, true); }
				else{ *this=subtract(other, *this, true); sign = !other.sign; } } return *this; }
		BigInt& operator-=(unsigned int value){ if(sign){ decrement(*this, value, true); if(IsZero()) sign = true; 
			}else{ increment(*this, value, true); } return *this; }
		BigInt operator-(const BigInt& other) const { BigInt result = *this; result -= other; return result; }
		BigInt operator-(unsigned int value) const { BigInt result = *this; result -= value; return result; }
		BigInt& operator--(){ *this-=1; return *this; } // prefix: --a
		BigInt operator--(int){ BigInt tmp(*this); *this-=1; return tmp; } // postfix: a--

		BigInt& operator*=(const BigInt& other){ bool oldsign=sign; *this = multiply(*this, other, true); sign = (oldsign == other.sign); return *this; }
		BigInt& operator*=(unsigned int value){ multiply_single(*this, value); return *this; }
		BigInt operator*(const BigInt& other) const { BigInt result = *this; result *= other; return result; }
		BigInt operator*(unsigned int value) const { BigInt result = *this; result *= value; return result; }

		BigInt& operator/=(const BigInt& other){ bool oldsign=sign; *this = divide(*this, other, true); sign = (oldsign == other.sign); return *this; }
		BigInt& operator/=(unsigned int value){ divide_single(*this, value); return *this; }
		BigInt operator/(const BigInt& other) const { BigInt result = *this; result /= other; return result; }
		BigInt operator/(unsigned int value) const { BigInt result = *this; result /= value; return result; }
		
		BigInt operator%(const BigInt& other) const { return mod(*this, other, true); }
		unsigned int operator%(unsigned int value) const { BigInt rem = *this; return divide_single(rem, value); }
		BigInt& operator%=(const BigInt& other){ *this = *this % other; return *this; }
		BigInt& operator%=(unsigned int value){ *this = *this % value; return *this; } //*this=divide_single(*this, value);

		bool operator<(const BigInt& other) const { if(sign != other.sign){ return !sign; }
			return sign ? (compare(*this, other) < 0) : (compare(*this, other) > 0); }
		bool operator<=(const BigInt& other) const { return (*this < other) || (*this == other); }
		bool operator>(const BigInt& other) const { return !(*this <= other); }
		bool operator>=(const BigInt& other) const { return !(*this < other); }
		
		bool operator==(const BigInt& other) const { return sign == other.sign && compare(*this, other) == 0; }
		bool operator!=(const BigInt& other) const { return !(*this == other); }
		
		BigInt& operator<<=(unsigned int bits){ return _shift_left(bits); }
		BigInt operator<<(unsigned int bits) const { BigInt result = *this; return (result <<= bits); }
		BigInt& operator>>=(unsigned int bits){ return _shift_right(bits); }
		BigInt operator>>(unsigned int bits) const { BigInt result = *this; return (result >>= bits); }
		
		BigInt operator-() const { BigInt result = *this; result.sign=!result.sign; return result; }
		
		static void removeLeadingZeros(BigInt& bint){ while(bint.parts.size() > 1 && bint.parts.back() == 0) bint.parts.pop(); }
		private:
			BigInt& _shift_left(unsigned int bits){ if(IsZero() || bits == 0) return *this; // O(N)
				const unsigned int ELEM_BITS = ELEM_SZ*8;
				unsigned int elem_shift = bits / ELEM_BITS; // Сдвиг на целое кол-во элементов
				unsigned int bit_shift = bits % ELEM_BITS;  // Сдвиг внутри элемента (0 до ELEM_BITS-1)

				if (elem_shift > 0){ parts.resize(parts.size()+elem_shift, 0);
					for(int i = parts.size() - 1; i >= (int)elem_shift; --i){ parts[i] = parts[i - elem_shift]; }
					for(unsigned int i = 0; i < elem_shift; ++i){ parts[i] = 0; } }

				if (bit_shift > 0){ ELEM_T carry = 0; //ELEM_T high_mask = ELEM_MAX << (ELEM_BITS - bit_shift);
					for(size_t i = elem_shift; i < parts.size(); ++i){
						ELEM_T new_carry = parts[i] >> (ELEM_BITS - bit_shift); // Биты, которые уходят "вверх"
						parts[i] = (parts[i] << bit_shift) | carry; carry = new_carry; }
					if(carry != 0){ parts.push(carry); } } return *this; }

			BigInt& _shift_right(unsigned int bits){ if(IsZero() || bits == 0) return *this; // O(N)
				const unsigned int ELEM_BITS = ELEM_SZ*8;
				unsigned int elem_shift = bits / ELEM_BITS; // Сдвиг на целое кол-во элементов
				unsigned int bit_shift = bits % ELEM_BITS;  // Сдвиг внутри элемента
				if(elem_shift >= parts.size()){ parts.clear(); return *this; }

				if(elem_shift > 0){
					for(size_t i = 0; i < parts.size() - elem_shift; ++i){ parts[i] = parts[i + elem_shift]; }
					parts.resize(parts.size()-elem_shift); }

				if (bit_shift > 0){ ELEM_T borrow = 0; unsigned int reverse_shift = ELEM_BITS - bit_shift;
					for(int i = parts.size() - 1; i >= 0; --i){
						ELEM_T current_carry = parts[i] << reverse_shift; // Биты, которые уходят "вниз"
						parts[i] = (parts[i] >> bit_shift) | borrow; borrow = current_carry; } }
				
				removeLeadingZeros(*this); return *this; }
	};
}

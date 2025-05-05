namespace ncpp{
	struct Buffer::Math{ // (https://gmplib.org/manual/Algorithms)
		// === base mathan ===
		static void increment(Buffer& buffer, unsigned int value=1, bool insert = false){
			if(value==1){ for(int j = buffer.size() - 1; j >= 0; --j){ if(++buffer[j] != 0) return; } } int carry = value;
			for(int i = buffer.size() - 1; i >= 0; --i){ unsigned int sum = buffer[i] + carry; buffer[i] = sum & 0xFF; carry = sum >> 8; }
			if(insert && carry != 0){ while(carry != 0){ buffer.insert(buffer.begin(), carry & 0xFF); carry >>= 8; } } }

		static void decrement(Buffer& buffer, unsigned int value=1, bool erase = false){ unsigned int borrow = value;
		    if(value==1){ for(int j = buffer.size() - 1; j >= 0; --j){ if(buffer[j]-- != 0) return; } }
			for(int i = buffer.size() - 1; i >= 0; --i){ int diff = buffer[i] - (borrow & 0xFF); buffer[i] = (diff + 256) & 0xFF;
				borrow = (diff < 0) ? 1 : 0; value >>= 8; borrow += (value & 0xFF); }
			if(erase){ if(borrow > 0){ buffer.fill(0); }
				while (buffer.size() > 1 && buffer.front() == 0){ buffer.erase(buffer.begin()); } } }

		static void multiply_single(Buffer& buffer, unsigned int value, bool insert = true){ unsigned int carry = 0;
			for(int i = buffer.size() - 1; i >= 0; --i){ unsigned int product = buffer[i] * value + carry; buffer[i] = product & 0xFF; carry = product >> 8; }
			if (insert && carry != 0){ while(carry != 0){ buffer.insert(buffer.begin(), carry & 0xFF); carry >>= 8; } } }

		static unsigned int divide_single(Buffer& buffer, unsigned int value, bool erase = true){
			if(value == 0){ throw std::invalid_argument("Matan rule 0: Never divide by zero :)"); } unsigned int remainder = 0;
			for(size_t i = 0; i < buffer.size(); ++i){ unsigned int current = (remainder << 8) | buffer[i]; buffer[i] = current / value; remainder = current % value; }
			if(erase){ removeLeadingZeros(buffer); } return remainder; }
			
		static Buffer add(const Buffer& a, const Buffer& b, bool resize = true){ Buffer result(std::max(a.size(),b.size())); unsigned int carry = 0;
			for(size_t i = 0; i < result.size(); ++i){ unsigned int sum = carry;
				if(i < a.size()){ sum += a[a.size() - 1 - i]; } if (i < b.size()){ sum += b[b.size() - 1 - i]; } result[result.size() - 1 - i] = sum & 0xFF; carry = sum >> 8; }
			if(resize){ while(carry != 0){ result.insert(result.begin(), carry & 0xFF); carry >>= 8; } } return result; }

		static Buffer subtract(const Buffer& a, const Buffer& b, bool resize = true){ Buffer result = a; unsigned int borrow = 0;
			for(int i = result.size() - 1, j = b.size() - 1; i >= 0; --i, --j) {
				int diff = result[i] - (j >= 0 ? b[j] : 0) - borrow; result[i] = (diff + 256) & 0xFF; borrow = (diff < 0) ? 1 : 0; }
			if(resize){ if(borrow > 0){ result.fill(0); } removeLeadingZeros(result); } return result; }
		
		static Buffer multiply(const Buffer& a, const Buffer& b, bool resize = true){ Buffer result(a.size() + b.size());
			//if(std::max(a.size(), b.size())>16){ return karatsuba_multiply(a, b); }
			for(int i = a.size() - 1; i >= 0; --i){ unsigned int carry = 0;
				for(int j = b.size() - 1; j >= 0; --j){ unsigned int product = a[i] * b[j] + result[i + j + 1] + carry;
					result[i + j + 1] = product & 0xFF; carry = product >> 8; } result[i] += carry; }
			if(resize){ removeLeadingZeros(result); }else{ while(result.size() > a.size()){ result.erase(result.begin()); } } return result; }
		
		static Buffer divide(const Buffer& a, const Buffer& b, bool resize = true){ return binary_divmod(a, b).first; }
		//static Buffer divide(const Buffer& a, const Buffer& b, bool resize = true){ return divmod(a, b).first; }
		
		static Buffer mod(const Buffer& a, const Buffer& b, bool resize = true){ return binary_divmod(a, b).second; }
		//static Buffer mod(const Buffer& a, const Buffer& b, bool resize = true){ return divmod(a, b).second; }
			
		static Buffer mod_native(const Buffer& a, const Buffer& b, bool resize = true){ //std::cout << " === Buffer::mod: INPUT: " << a.toIntString() << ", " << b.toIntString() << std::endl;
			Buffer quotient = divide(a, b, resize); 
			//std::cout << " === Buffer::mod: quotient = " << quotient.toIntString() << std::endl;
			Buffer product = multiply(quotient, b, resize); 
			//std::cout << " === Buffer::mod: product = " << product.toIntString() << std::endl; 
			Buffer remainder = subtract(a, product, resize); 
			//std::cout << " === Buffer::mod: remainder (return) = " << remainder.toIntString() << std::endl; 
			return remainder; }
			
		static int compare(const Buffer& a, const Buffer& b){ size_t a_size = a.size(), b_size = b.size();
			size_t a_start = 0;	while(a_start < a_size && a[a_start] == 0){ ++a_start; }
			size_t b_start = 0; while(b_start < b_size && b[b_start] == 0){ ++b_start; }
			size_t a_len = a_size-a_start; size_t b_len = b_size-b_start;
			if (a_len > b_len){ return 1; } if(a_len < b_len){ return -1; }
			for (size_t i = 0; i < a_len; i++){
				if(a[a_start+i] > b[b_start+i]){ return 1; }
				if(a[a_start+i] < b[b_start+i]){ return -1; }
			} return 0; }
		
		// Бинарный алгоритм деления через удвоение
		static std::pair<Buffer, Buffer> binary_divmod(const Buffer& a, const Buffer& b){ Buffer q, r = a;
			if(b.size()<=4){ q=a; r=Buffer::from(divide_single(q, b.toInt())); return std::make_pair(q, r); }
			while (compare(r, b) >= 0){ Buffer temp = b, cmp_tmp = b, multiple(1, 1); multiply_single(cmp_tmp, 2);
				while(compare(r, cmp_tmp) >= 0){
					multiply_single(temp, 2); multiply_single(multiple, 2);
					cmp_tmp=temp; multiply_single(cmp_tmp, 2); }
				r = subtract(r, temp); q = add(q, multiple);
			} return std::make_pair(q, r); }	
		// === advanced mathan ===
		static Buffer pow(Buffer base, Buffer exp){ Buffer result(1, 1); //std::cout << "Buffer::pow compare(exp,Buffer((size_t)0) " << compare(exp,Buffer((size_t)0)) << ", exp=" << exp << std::endl;
			while(compare(exp,Buffer((size_t)0))==1){
				if((exp.back()&1)!=0){ result = multiply(result, base); }
				base = multiply(base, base); divide_single(exp, 2); } return result; }
		//TODO: Knuth’s Algorithm D (https://skanthak.hier-im-netz.de/division.html)
		//#include "experimantal/ncpp-divide_knuth.cpp"
		//TODO: Алгоритм Карацубы (>=128-256 bytes)
		/*static Buffer karatsuba_multiply(const Buffer& a, const Buffer& b){
			if(a.size() <= 4 || b.size() <= 4){ return multiply(a, b); }
			
			size_t n = std::max(a.size(), b.size()); size_t n2 = n / 2;
			// Split the buffers into high and low parts using available methods
			Buffer pow10n2 = Math::pow(Buffer(1, 10), Buffer::from(n2));
			std::cout << "karatsuba pow10n2 = " << pow10n2 << "(n=" << n << ", n2=" << n2 << std::endl;
			Buffer x = divide(a, pow10n2);
			Buffer y = mod(a, pow10n2);
			Buffer z = divide(b, pow10n2);
			Buffer w = mod(b, pow10n2);

			// Recursively compute p, q, and r using Karatsuba's method
			Buffer p = karatsuba_multiply(x, z);           // p = x * z
			Buffer q = karatsuba_multiply(add(x, y), add(z, w)); // q = (x + y) * (z + w)
			Buffer r = karatsuba_multiply(y, w);           // r = y * w
			std::cout << "karatsuba FINALITY" << std::endl;
			// Combine the results using the Karatsuba formula
			return add(add(multiply(Math::pow(Buffer(1, 10), Buffer::from(2*n2)), p), multiply(pow10n2, subtract(subtract(q, p), r))), r);
			//return (long long)pow(10, 2 * n2) * p + (long long)pow(10, n2) * (q - p - r) + r;
		}*/
		static Buffer karatsuba_multiply(const Buffer& a, const Buffer& b){
			if(a.empty() || b.empty()){ return Buffer(); }
			if(a.size() <= 16 || b.size() <= 16){ return multiply(a, b); } // Если числа маленькие, используем обычное умножение
			size_t half = std::max(a.size(), b.size()) / 2; // Находим половину длины большего числа

			// Разделяем числа на две части
			Buffer a_high(a.begin(), a.begin() + std::min(half, a.size()));
			Buffer a_low(a.begin() + std::min(half, a.size()), a.end());
			Buffer b_high(b.begin(), b.begin() + std::min(half, b.size()));
			Buffer b_low(b.begin() + std::min(half, b.size()), b.end());

			// Рекурсивно вычисляем три произведения
			Buffer z0 = karatsuba_multiply(a_low, b_low);
			Buffer z1 = karatsuba_multiply(add(a_low, a_high), add(b_low, b_high));
			Buffer z2 = karatsuba_multiply(a_high, b_high);

			
			Buffer temp = subtract(subtract(z1, z0), z2); // Вычисляем промежуточные результаты
			z2.insert(z2.end(), 2 * half, 0); // Сдвигаем z2 на 2*half байт влево
			temp.insert(temp.end(), half, 0); // Сдвигаем temp на half байт влево
			Buffer result = add(z2, add(temp, z0)); // Суммируем результаты

			
			removeLeadingZeros(result); return result; }
		private:
			static void removeLeadingZeros(Buffer& buff){ if(buff.size() <= 1) return; size_t firstNonZero = 0;
				while(firstNonZero < buff.size()-1 && buff[firstNonZero] == 0) ++firstNonZero;
				if(firstNonZero > 0){ buff.erase(buff.begin(), buff.begin() + firstNonZero); } }
	};
	
	struct BigInt : Buffer { bool positive;
		BigInt() : Buffer(), positive(true){}
		~BigInt(){} // Деструктор
        BigInt(std::string value, std::string type="dec") : positive(true){ if(type=="hex"){ fromHex(value); }else{ fromIntString(value); } }
        explicit BigInt(const char* cstr) : positive(true){ fromIntString(std::string(cstr)); }
        BigInt(int value){ fromInt64(value); }
        BigInt(unsigned int value){ fromInt64(value); }
		BigInt(long long value){ fromInt64(value); }
		BigInt(const Buffer& other) : Buffer(other), positive(true){}
		
		void fromIntString(const std::string& str){ String str1=str; if(str[0]=='-'){ positive = false; str1=str1.slice(1); }else{ positive = true; } Buffer::_fromIntString(str1); }
		void fromHex(const std::string& str){ String str1=str; if(str[0]=='-'){ positive = false; str1=str1.slice(1); }else{ positive = true; } Buffer::fromHex(str1); }
		void fromInt64(long long value){ if(value>=0){ *this=Buffer::from(value); positive=true; }else{ *this=Buffer::from(::abs(value)); positive=false; } }
		std::string toIntString() const { if(positive){ return Buffer::toIntString(); }else{ return "-"+Buffer::toIntString(); } }
		std::string toHexString() const { if(positive){ return Buffer::toHexString(); }else{ return "-"+Buffer::toHexString(); } }
		bool IsEven(){ return (this->back() & 1) == 0; }
		void negate(){ positive?positive=false:positive=true; }
		void trim(){ Buffer::removeLeadingZeros(*this); }
		//=== advanced mathan ===
		BigInt pow(BigInt base, BigInt exp){ BigInt result(1);
			while(exp > 0){ if(exp % 2 == 1){ result = (result * base); }
				base = (base * base); exp/=2; } return result; }
		static BigInt powMod(BigInt base, BigInt exp, const BigInt& mod){ BigInt result(1);
			while(exp > 0){ if(exp % 2 == 1){ result = (result * base) % mod; }
				base = (base * base) % mod; exp /= 2; } return result; }
		// Вывод
		String toString() const { return toIntString(); }
		String toString(std::string type) const { if(type=="hex"){ return toHexString(); }
			else if(type=="int"||type=="number"||type=="dec"){ return toIntString(); }
			else if(type=="raw"||type=="buff"){ return cout(); }else{ return toIntString(); } }
		// Перегрузки
		BigInt& operator+=(const BigInt& other){ if(positive == other.positive){ *this = Buffer::Math::add(*this, other, true); } else {
				if(Buffer::Math::compare(*this, other) >= 0){ *this = Buffer::Math::subtract(*this, other, true); } 
				else{ *this = Buffer::Math::subtract(other, *this, true); positive = other.positive; } } return *this; }
		BigInt& operator+=(unsigned int value){ if(positive){ Buffer::Math::increment(*this, value, true); } 
			else{ Buffer::Math::decrement(*this, value, true); if(this->empty()||(this->size() == 1 && this->at(0) == 0)){ positive = true; } } return *this; }
		BigInt operator+(const BigInt& other) const { BigInt result = *this; result += other; return result; }
		BigInt operator+(unsigned int value) const { BigInt result = *this; result += value; return result; }
		BigInt& operator++(){ *this+=1; return *this; } // frefix: ++a
		BigInt operator++(int){ BigInt tmp(*this); *this+=1; return tmp; } // postfix: a++
		

		BigInt& operator-=(const BigInt& other){ if(positive != other.positive){ *this = Buffer::Math::add(*this, other, true); positive=!other.positive; }else{
            if(Buffer::Math::compare(*this, other) >= 0){ *this = Buffer::Math::subtract(*this, other, true); }
            else{ *this = Buffer::Math::subtract(other, *this, true); positive = !other.positive; } } return *this; }
		BigInt& operator-=(unsigned int value){ if(positive){ Buffer::Math::decrement(*this, value, true); }
			else{ Buffer::Math::increment(*this, value, true); } return *this; }
		BigInt operator-(const BigInt& other) const { BigInt result = *this; result -= other; return result; }
		BigInt operator-(unsigned int value) const { BigInt result = *this; result -= value; return result; }
		BigInt& operator--(){ *this-=1; return *this; }
		BigInt operator--(int){ BigInt tmp(*this); *this-=1; return tmp; }

		BigInt& operator*=(const BigInt& other){ bool oldpos=positive; *this = Buffer::Math::multiply(*this, other, true); 
			positive = (oldpos == other.positive); return *this; }
		BigInt& operator*=(unsigned int value){ *this *= BigInt(value); return *this; }
		BigInt operator*(const BigInt& other) const { BigInt result = *this; result *= other; return result; }
		BigInt operator*(unsigned int value) const { BigInt result = *this; result *= BigInt(value); return result; }

		BigInt& operator/=(const BigInt& other){ bool oldpos=positive; *this = Buffer::Math::divide(*this, other, true); 
			positive = (oldpos == other.positive); return *this; }
		BigInt& operator/=(unsigned int value){ *this /= BigInt(value); return *this; }
		BigInt operator/(const BigInt& other) const { BigInt result = *this; result /= other; return result; }
		BigInt operator/(unsigned int value) const { BigInt result = *this; result /= BigInt(value); return result; }
		
		BigInt operator%(const BigInt& other) const { BigInt result = Buffer::Math::mod(*this, other, true); return result; }
		BigInt operator%(unsigned int value) const { return *this % BigInt(value); }
		BigInt& operator%=(const BigInt& other){ *this = *this % other; return *this; }
		BigInt& operator%=(unsigned int value){ *this = *this % BigInt(value); return *this; }

		bool operator<(const BigInt& other) const { if(positive != other.positive){ return !positive; }
			return positive ? (Buffer::Math::compare(*this, other) < 0) : (Buffer::Math::compare(*this, other) > 0);}
		bool operator<=(const BigInt& other) const { return (*this < other) || (*this == other); }
		bool operator>(const BigInt& other) const { return !(*this <= other); }
		bool operator>=(const BigInt& other) const { return !(*this < other); }
		
		bool operator==(const BigInt& other) const { return positive == other.positive && Buffer::Math::compare(*this, other) == 0; }
		bool operator!=(const BigInt& other) const { return !(*this == other); }
	};
	// Перегрузки операторов для BigInt
	std::ostream& operator<<(std::ostream& os, const BigInt& bigint){ os << bigint.toIntString(); return os; }
}

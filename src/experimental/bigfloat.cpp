namespace ncpp{
	struct BigFloat { static const unsigned char ELEM_SZ = sizeof(unsigned long long);
		bool sign; long long exponent; Array<unsigned long long> mantissa;
		BigFloat() : sign(true){}
		~BigFloat(){} // Деструктор
	};
}
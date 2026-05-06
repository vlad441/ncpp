// ====== Base module ======
void Array_test(){
	NextTest("Array()"); Array<int> arr; TEST_EQ(arr.size(), 0U);
	
	// Переиспользуем arr
	NextTest("Array(size_t n, const T& val)"); arr=Array<int>(1, 0); 
	TEST_EQ(arr.size(), 1U); TEST_EQ(arr[0], 0);
	arr=Array<int>(2, 5); 
	TEST_EQ(arr.size(), 2U); TEST_EQ(arr[0], 5); TEST_EQ(arr[1], 5);
	
	int iarr[] = {10, 20, 30};
	NextTest("Array(const T* begin, const T* end)"); Array<int> copy(iarr, iarr+3); 
	TEST_EQ(copy.size(), 3U); TEST_EQ(copy[0], 10); TEST_EQ(copy.back(), 30);
	
	NextTest("Array(const Array<T>& arr)"); Array<int> copy2 = copy;
	TEST_EQ(copy2.size(), copy.size()); TEST_EQ(copy2[1], copy[1]);
	
	NextTest("Array(const T (&arr)[N])"); arr = iarr;
	TEST_EQ(arr.size(), 3U); TEST_EQ(arr[0], 10);
	
	// Методы resize/reserve
	NextTest("Array.resize(size_t len)"); 
	int r_init[] = {1, 2, 3}; arr = r_init; // Исправлено: замена {1, 2, 3}
	arr.resize(5, 9); TEST_EQ(arr.size(), 5U); TEST_EQ(arr[4], 9);
	int r_init2[] = {1, 2, 3}; arr = r_init2; 
	arr.resize(2); TEST_EQ(arr.size(), 2U); TEST_EQ(arr[1], 2);
	
	NextTest("Array.reserve(size_t len)"); 
	int res_init[] = {1, 2}; arr = res_init; // Исправлено: замена {1, 2}
	size_t cap = arr.capacity();
	arr.reserve(cap * 2); TEST_EQ(arr.size(), 2U); TEST_NE(arr.capacity(), cap);
	arr.reserve(cap / 2); TEST_EQ(arr.capacity(), cap * 2); // Емкость не должна уменьшаться
	
	// Методы push/pop
	NextTest("Array.push_back(const T& val) / pop_back()"); arr.clear();
	arr.push_back(100); TEST_EQ(arr.size(), 1U); TEST_EQ(arr[0], 100);
	arr.pop_back(); TEST_EQ(arr.size(), 0U);
	
	NextTest("Array.push(const T& val) / pop()"); arr.push(1); arr.push(2);
	TEST_EQ(arr.size(), 2U); TEST_EQ(arr.pop(), 2); TEST_EQ(arr.size(), 1U);
	
	NextTest("Array.push(const T* ptr, size_t len)"); 
	int p_init[] = {1}; arr = p_init; // Исправлено: замена {1}
	int p_arr[] = {2, 3}; arr.push(p_arr, 2);
	TEST_EQ(arr.size(), 3U); TEST_EQ(arr[1], 2); TEST_EQ(arr[2], 3);
	
	NextTest("Array.push(const T (&arr)[N])"); 
	int p_init2[] = {1}; arr = p_init2; // Исправлено: замена {1}
	int p_arr2[] = {2, 3}; arr.push(p_arr2); TEST_EQ(arr.size(), 3U); TEST_EQ(arr[1], 2); TEST_EQ(arr[2], 3);
	
	NextTest("Array.push_front(const T& v) / pop_front()"); 
	int pf_init[] = {2, 3}; arr = pf_init; // Исправлено: замена {2, 3}
	arr.push_front(1); TEST_EQ(arr[0], 1); TEST_EQ(arr.size(), 3U);
	arr.pop_front(); TEST_EQ(arr[0], 2); TEST_EQ(arr.size(), 2U);
	
	// Методы insert
	NextTest("Array.insert(Iter ipos, const T& v)"); 
	int i_init[] = {1, 3}; arr = i_init; // Исправлено: замена {1, 3}
	arr.insert(arr.begin()+1, 2); TEST_EQ(arr.size(), 3U); TEST_EQ(arr[1], 2);
	
	NextTest("Array.insert(Iter ipos, const T* dptr, size_t len)"); 
	int i_init2[] = {1, 5}; arr = i_init2; // Исправлено: замена {1, 5}
	int ins_arr[] = {2, 3, 4}; arr.insert(arr.begin()+1, ins_arr, 3);
	TEST_EQ(arr.size(), 5U); TEST_EQ(arr[0], 1); TEST_EQ(arr[4], 5); TEST_EQ(arr[2], 3);
	
	// Методы erase
	NextTest("Array.erase(Iter first, Iter last)"); 
	int e_init[] = {1, 2, 3, 4, 5}; arr = e_init; // Исправлено: замена {1, 2, 3, 4, 5}
	arr.erase(arr.begin()+1, arr.begin()+4); TEST_EQ(arr.size(), 2U); TEST_EQ(arr[0], 1); TEST_EQ(arr[1], 5);
	
	NextTest("Array.erase(Iter ipos)"); 
	int e_init2[] = {1, 2, 3}; arr = e_init2; // Исправлено: замена {1, 2, 3}
	arr.erase(arr.begin()+1); TEST_EQ(arr.size(), 2U); TEST_EQ(arr[0], 1); TEST_EQ(arr[1], 3);
	
	// Прочие методы
	NextTest("Array.clear()"); 
	int c_init[] = {1, 2, 3}; arr = c_init; // Исправлено: замена {1, 2, 3}
	arr.clear();
	TEST_EQ(arr.size(), 0U); TEST_EQ(arr.empty(), true);
	
	NextTest("Array.shrink_to_fit()"); 
	int s_init[] = {1}; arr = s_init; // Исправлено: замена {1}
	arr.reserve(10); size_t cap1 = arr.capacity();
	arr.shrink_to_fit(); TEST_NE(arr.capacity(), cap1); TEST_EQ(arr.size(), 1U);
	
	NextTest("Array.fill(const T& val)"); arr.resize(3); arr.fill(99);
	TEST_EQ(arr[0], 99); TEST_EQ(arr[2], 99);
	
	NextTest("Array.reverse()"); 
	int r_init3[] = {1, 2, 3}; arr = r_init3; // Исправлено: замена {1, 2, 3}
	arr.reverse();
	TEST_EQ(arr[0], 3); TEST_EQ(arr[2], 1);
	
	NextTest("Array.front() / back()"); 
	int fb_init[] = {5, 8, 1}; arr = fb_init; // Исправлено: замена {5, 8, 1}
	TEST_EQ(arr.front(), 5); TEST_EQ(arr.back(), 1);
	
	NextTest("Array.indexOf(const T& value, size_t start)"); 
	int io_init[] = {1, 2, 3, 2, 5}; arr = io_init; // Исправлено: замена {1, 2, 3, 2, 5}
	TEST_EQ(arr.indexOf(2), 1U); TEST_EQ(arr.indexOf(2, 2), 3U); TEST_EQ(arr.indexOf(99), (size_t)-1);
	
	NextTest("Array.slice(int start, int end)"); 
	int sl_init[] = {1, 2, 3, 4, 5}; arr = sl_init; // Исправлено: замена {1, 2, 3, 4, 5}
	Array<int> s1 = arr.slice(1, 4); TEST_EQ(s1.size(), 3U); TEST_EQ(s1[0], 2); TEST_EQ(s1[2], 4);
	Array<int> s2 = arr.slice(2); TEST_EQ(s2.size(), 3U); TEST_EQ(s2[0], 3);
	Array<int> s3 = arr.slice(-3, -1); TEST_EQ(s3.size(), 2U); TEST_EQ(s3[0], 3); TEST_EQ(s3[1], 4);
	
	// Операторы
	NextTest("Array.operator=(const Array& arr)"); 
	int a1_init[] = {1, 2}; Array<int> a1(a1_init); // Исправлено: Array<int> a1 = {1, 2}
	Array<int> a2; a2 = a1;
	TEST_EQ(a2.size(), 2U); TEST_EQ(a2[0], 1);
	
	NextTest("Array.operator=(const T (&arr)[N])"); int i_arr_assign[] = {10, 20}; arr = i_arr_assign;
	TEST_EQ(arr.size(), 2U); TEST_EQ(arr[1], 20);
	
	NextTest("Array.operator[](size_t pos) / at()"); 
	int op_init[] = {10, 20}; arr = op_init; // Исправлено: замена {10, 20}
	TEST_EQ(arr[1], 20); arr[0] = 5; TEST_EQ(arr.at(0), 5); TEST_EQ_W(arr.at(100), int()); // Проверка at() на выход за границы
	
	// Конкатенация и соединение
	NextTest("Array.concat(const Array<T>& arr2)"); 
	int c1_init[] = {1, 2}; arr = c1_init; // Исправлено: замена {1, 2}
	int b_init[] = {3, 4}; Array<int> b(b_init); // Исправлено: Array<int> b = {3, 4}
	arr.concat(b); TEST_EQ(arr.size(), 4U); TEST_EQ(arr[3], 4);
	
	NextTest("Array::concat(const Array<T>& arr1, const Array<T>& arr2)");
	int c3_init[] = {1, 2}; int c4_init[] = {3, 4};
	Array<int> c = Array<int>::concat(Array<int>(c3_init), Array<int>(c4_init)); // Исправлено: замена {1, 2}, {3, 4}
	TEST_EQ(c.size(), 4U); TEST_EQ(c[3], 4);
	
	// Применим join к Array<int> (хотя обычно это для строк/буферов)
	NextTest("Array.join(const T& delim)"); 
	// Исправлено: Array<String> str_arr = {"A", "B", "C"}
	Array<String> str_arr;
	str_arr.push_back("A"); str_arr.push_back("B"); str_arr.push_back("C"); // Использование push_back для совместимости
	String joined = str_arr.join("-");
	TEST_EQ(joined, "A-B-C");
}

void String_test(){
	NextTest("String()"); String str; TEST_EQ(str.size(), 0U);
	
	//String(const char* ptr, size_t len){ _init(len); _set(ptr, len); }
	//String(const char* cptr){ _init(strlen(cptr)); _set(cptr, _len); }
}

void Buffer_test(){ const char* ctest="Test str"; Buffer copy; unsigned int tmpval[2]; unsigned char uarr[]={5,8,11};
	
	// Конструкторы
	NextTest("Buffer()"); Buffer buff; if(buff.size()!=0){ errs++; }
	
	
	NextTest("Buffer(const void* ptr, size_t len)"); buff = Buffer(ctest, 4);
	if(buff.size()!=4||strncmp((char*)&buff[0], ctest, 4)!=0){ errs++; }
	
	NextTest("Buffer(const char* cptr)"); buff = Buffer("Hi, ncpp");
	if(strncmp((char*)&buff[0], "Hi, ncpp", 8)!=0||buff!=Buffer("Hi, ncpp")){ errs++; }
	
	NextTest("Buffer(size_t size)"); buff = Buffer(6); if(buff.size()!=6){ errs++; } //Этот конструктор не стирает выделенную память
	NextTest("Buffer(size_t len, unsigned char v)"); buff = Buffer(5, 12); if(buff.size()!=5||buff[0]!=12||buff[4]!=12){ errs++; }
	
	NextTest("Buffer(const Buffer& b)"); buff = Buffer(ctest); copy = Buffer(buff);
	if(copy!=buff||strncmp((char*)&copy[0], (char*)&buff[0], 8)!=0||copy.size()!=8||buff.size()!=8){ errs++; }
	buff = ""; if(buff.size()!=0){ errs++; } // Тест на баг конструктора копирования.
	
	NextTest("Buffer(const void* begin, const void* end)"); buff = Buffer(ctest+2,ctest+7);
	if(buff.size()!=5||buff!="st st"||strncmp((char*)&buff[0], "st st", 5)!=0){ errs++; }
	
	NextTest("Buffer(const unsigned char (&arr)[N])"); buff = Buffer(uarr);
	if(buff.size()!=3||buff[0]!=5||buff[1]!=8||buff[2]!=11){ errs++; }
	
	// Методы from
	NextTest("Buffer::from(const CString& str, const CString& type='str/hex/base64')");
	buff=Buffer::from(ctest, "str"); 
	TEST_EQ(buff.size(), 8U); TEST_EQ(buff.toString(), ctest); TEST_EQ(strncmp((char*)&buff[0], ctest, 8), 0);
	buff=Buffer::from("5465737420737472", "hex"); 
	TEST_EQ(buff.size(), 8U); TEST_EQ(buff.toString(), ctest); TEST_EQ(strncmp((char*)&buff[0], ctest, 8), 0);
	buff=Buffer::from("VGVzdCBzdHI=", "base64"); TEST_EQ(buff.size(), 8U); TEST_EQ(buff.toString(), ctest);
	
	NextTest("Buffer::fromHex(const char* hexstr, size_t len)");
	buff=Buffer::fromHex("5465737420737472", 16); if(buff.size()!=8||buff!=ctest||strncmp((char*)&buff[0], ctest, 8)!=0){ errs++; }
	
	NextTest("Buffer::fromHex(const CString& hexstr)");
	buff=Buffer::fromHex("5465737420737472"); if(buff.size()!=8||buff!=ctest||strncmp((char*)&buff[0], ctest, 8)!=0){ errs++; }
	
	NextTest("Buffer::fromInt(unsigned long long numb)");
	buff=Buffer::fromInt(40000); if(buff[buff.size()-2]!=0x9c||buff[buff.size()-1]!=0x40){ errs++; } if(buff.size()!=2) warns++;
	//NextTest("Buffer::fromInt(const CString& cs)");

	// -----------------------------------------

	// STL-подобные методы
	NextTest("Buffer.assign(const void* begin, const void* end)");
	buff.assign(ctest+1, ctest+6); TEST_EQ(buff.size(), 5U); TEST_EQ(buff.toString(), "est s");
	
	NextTest("Buffer.reserve(size_t sz)"); buff="short"; tmpval[0]=buff.size(); tmpval[1]=buff.capacity();
	buff.reserve(20); TEST_EQ(tmpval[0], buff.size()); TEST_NE(tmpval[1], buff.capacity()); // Должен выделить больше памяти
	
	NextTest("Buffer.resize(size_t len)"); buff=ctest; tmpval[1]=buff.capacity();
	buff.resize(3); TEST_EQ(buff.size(), 3U); TEST_EQ(buff.toString(), "Tes"); 
	buff.resize(6); TEST_EQ(buff.size(), 6U); TEST_EQ(buff.toString(), "Test s"); 
	TEST_EQ(tmpval[1], buff.capacity()); // Емкость не должна уменьшаться
	
	// Методы insert
	NextTest("Buffer.insert(size_t pos, const void* dptr, size_t len)"); buff="AC";
	buff.insert(1, "B", 1); TEST_EQ(buff.toString(), "ABC");
	
	NextTest("Buffer.insert(size_t pos, size_t n, unsigned char v)"); buff="BDE";
	buff.insert(0, 1, 'A'); buff.insert(buff.size(), 1, 'F'); buff.insert(2, 2, 'C'); TEST_EQ(buff.toString(), "ABCCDEF");
	
	NextTest("Buffer.insert(size_t pos, const char* cstr)"); buff="BC"; buff.insert(1, 2, 'A'); TEST_EQ(buff.toString(), "BAAC");
	NextTest("Buffer.insert(size_t pos, unsigned char v)"); buff="AD"; buff.insert(1, 'B'); TEST_EQ(buff.toString(), "ABD");
	NextTest("Buffer.insert(size_t pos, const Buffer& other)"); buff="AE"; Buffer ins="BCD"; buff.insert(1, ins); TEST_EQ(buff.toString(), "ABCDE");
	
	// Методы erase
	NextTest("Buffer.erase(Iter first, Iter last)"); buff="ABCDEF";
	buff.erase(buff.begin()+2, buff.begin()+4); TEST_EQ(buff.toString(), "ABEF");
	
	NextTest("Buffer.erase(Iter ipos)"); buff="ABC";
	buff.erase(buff.begin()+1); TEST_EQ(buff.toString(), "AC");
	
	NextTest("Buffer.push_back(unsigned char v) / pop_back()"); buff="A";
	buff.push_back('B'); TEST_EQ(buff.toString(), "AB");
	buff.pop_back(); TEST_EQ(buff.toString(), "A");
	
	NextTest("Buffer.clear()"); buff="123"; buff.clear(); TEST_EQ(buff.size(), 0U);
	
	NextTest("Buffer.shrink_to_fit()"); buff="1234567890"; buff.reserve(100); 
	size_t cap1=buff.capacity(); buff.shrink_to_fit(); TEST_NE(cap1, buff.capacity()); // Емкость должна уменьшиться
	
	NextTest("Buffer.shrink()"); // То же самое что shrink_to_fit
	
	NextTest("Buffer.push_front(unsigned char v) / pop_front()"); buff="B";
	buff.push_front('A'); TEST_EQ(buff.toString(), "AB");
	buff.pop_front(); TEST_EQ(buff.toString(), "B");
	
	// push/slice/split/fill/reverse
	
	NextTest("Buffer.push(const unsigned char (&arr)[N])"); buff="A"; unsigned char p[]={'B', 'C'};
	buff.push(p); TEST_EQ(buff.toString(), "ABC");
	
	NextTest("Buffer.push(const void* ptr, size_t len)"); buff="ABC"; buff.push("DEF", 3); TEST_EQ(buff.toString(), "ABCDEF");
	
	NextTest("Buffer.push(const Buffer& buff)"); buff="A"; Buffer pbuff="BC"; buff.push(pbuff); TEST_EQ(buff.toString(), "ABC");
	
	NextTest("Buffer.push(unsigned char v) / pop()"); buff="12";
	buff.push('3'); TEST_EQ(buff.toString(), "123"); TEST_EQ(buff.pop(), '3'); TEST_EQ(buff.toString(), "12");
	
	NextTest("Buffer.slice(int start, int end)"); buff="ABCDEF";
	TEST_EQ(buff.slice(1, 4).toString(), "BCD");
	TEST_EQ(buff.slice(3).toString(), "DEF");
	TEST_EQ(buff.slice(-3, -1).toString(), "DE");
	
	NextTest("Array<Buffer> split(const char* delim)"); buff="A,B,C";
	Array<Buffer> parts = buff.split(",");
	TEST_EQ(parts.size(), 3U); TEST_EQ(parts[1].toString(), "B");
	
	NextTest("Buffer.fill(unsigned char v)"); buff="ABC"; buff.fill('Z');
	TEST_EQ(buff.toString(), "ZZZ");
	
	NextTest("Buffer.reverse()"); buff="ABC"; buff.reverse();
	TEST_EQ(buff.toString(), "CBA");
	
	// ---------------------------------------------

	// Чтение/запись данных (read/write/readAny/writeAny)
	int val32=0x12345678; char valch='X';
	NextTest("Buffer.write(const void* src, size_t size, size_t offset)"); buff.clear();
	buff.write(&valch, 1, 0); buff.write(&val32, 4, 1);
	TEST_EQ(buff.size(), 5U); TEST_EQ(buff[0], 'X');
	
	NextTest("Buffer.read(void* dst, size_t size, size_t offset)"); int rval=0;
	buff.read(&rval, 4, 1); TEST_EQ(rval, val32);
	
	NextTest("Buffer.write(const char* cstr, size_t offset)"); buff.clear(); buff.write("abc");
	TEST_EQ(buff.toString(), "abc"); buff.write("xyz", 1); TEST_EQ(buff.toString(), "axyz");
	
	NextTest("Buffer.write(const Buffer& buff, size_t offset)"); buff.clear(); Buffer wbuff="123";
	buff.write(wbuff); TEST_EQ(buff.toString(), "123");
	
	NextTest("Buffer.readAny<T>(size_t offset)"); buff.clear(); buff.write(&val32, sizeof(int), 0); TEST_EQ(buff.readAny<int>(), val32);

	NextTest("Buffer.writeAny<T>(const T& data, size_t offset)"); int wval=99;
	buff.clear(); buff.writeAny(wval); TEST_EQ(buff.size(), sizeof(int));
	TEST_EQ(buff.readAny<int>(), 99);
	
	// Чтение/запись чисел (Endianness)
	uint32_t num32 = 0xAABBCCDD;
	NextTest("read/writeUInt32BE/LE"); buff.clear();
	buff.writeUInt32LE(num32); TEST_EQ(buff.readUInt32LE(), num32);
	TEST_EQ(buff[0], 0xDD); TEST_EQ(buff[3], 0xAA);
	buff.writeUInt32BE(num32); TEST_EQ(buff.readUInt32BE(), num32);
	TEST_EQ(buff[0], 0xAA); TEST_EQ(buff[3], 0xDD);
	
	NextTest("read/writeInt64BE/LE"); buff.clear();
	long long num64 = 0x1122334455667788LL;
	buff.writeInt64BE(num64); TEST_EQ(buff.readInt64BE(), num64);
	TEST_EQ(buff[0], 0x11); TEST_EQ(buff[7], 0x88);

	// Битовые операции
	unsigned char byte_val = 0xA5; // 10100101
	NextTest("Buffer::BitsRead(const void* ptr)");
	Buffer bits = Buffer::BitsRead(&byte_val);
	TEST_EQ(bits.size(), 8U); TEST_EQ(bits[0], 1); TEST_EQ(bits[1], 0); TEST_EQ(bits[7], 1);
	
	NextTest("Buffer::BitsWrite(void* ptr, const Buffer& bits)");
	unsigned char new_byte = 0; bits = Buffer::IntToBits(0x5A, 8); // 01011010
	Buffer::BitsWrite(&new_byte, bits); TEST_EQ(new_byte, 0x5A);
	
	NextTest("Buffer::BitsToInt(const Buffer& bits)");
	TEST_EQ(Buffer::BitsToInt(bits), 0x5A);
	
	NextTest("Buffer::IntToBits(int numb, size_t bitlen)");
	Buffer bits2 = Buffer::IntToBits(5, 4); // 0101
	print("-- bits2: "); print(bits2); print("\n");
	TEST_EQ(bits2.size(), 4U); TEST_EQ(bits2[0], 0); TEST_EQ(bits2[3], 1);
	
// --------------------------------------------------------------------------------

	// Преобразования
	NextTest("Buffer.toString()"); buff="abc"; TEST_EQ(buff.toString(), "abc");
	
	NextTest("Buffer.toString(type='hex/base64/int')");
	buff=ctest; // "Test str" -> 5465737420737472
	TEST_EQ(buff.toString("hex"), "5465737420737472");
	TEST_EQ(buff.toString("base64"), "VGVzdCBzdHI=");
	
	Buffer buff_num(2); buff_num.writeUInt16BE(12345);
	TEST_EQ(buff_num.toIntString(), "12345");
	
	NextTest("Buffer.toHex()"); buff="A"; TEST_EQ(buff.toHex(), "41");
	
	NextTest("Buffer.toIntString()"); 
	Buffer buff_big = Buffer::from("1000", "hex"); // 4096 в десятичном
	TEST_EQ(buff_big.toIntString(), "4096");
	
	NextTest("Buffer.cout()"); buff="wasso, hi"; TEST_EQ(buff.cout(), "<Buffer 77 61 73 73 6f 2c 20 68 69>");
	TEST_EQ(buff.cout(5), "<Buffer 77 61 73 73 6f ... 4 bytes more>");
	
// --------------------------------------------------------------------------------

	// Операторы
	NextTest("Buffer.operator=(const char*)"); buff="new val";
	TEST_EQ(buff.toString(), "new val");
	
	NextTest("Buffer.operator=(const Buffer&)"); Buffer b2="old val"; buff=b2;
	TEST_EQ(buff.toString(), "old val"); TEST_EQ(buff.size(), 7U);
	
	NextTest("Buffer.operator=(const unsigned char (&arr)[N])"); buff=uarr;
	TEST_EQ(buff.size(), 3U); TEST_EQ(buff[0], 5);
	
	NextTest("Buffer.operator+=(const char*)"); buff="A"; buff+="BC";
	TEST_EQ(buff.toString(), "ABC");
	
	NextTest("Buffer.operator+=(const Buffer&)"); buff="A"; Buffer b_add="BC"; buff+=b_add;
	TEST_EQ(buff.toString(), "ABC");
	
	NextTest("Buffer.operator+(const char*)"); buff="A"; Buffer res = buff+"BC";
	TEST_EQ(res.toString(), "ABC");
	
	NextTest("Buffer.operator+(const Buffer&)"); buff="A"; res = buff+b_add;
	TEST_EQ(res.toString(), "ABC");
	
	NextTest("Buffer.operator+(unsigned char)"); buff="A"; res = buff+'B';
	TEST_EQ(res.toString(), "AB");

	NextTest("Buffer.operator[] / at()"); buff="ABC";
	TEST_EQ(buff[1], 'B'); buff[1]='Z'; TEST_EQ(buff.at(1), 'Z');
	TEST_EQ(buff.at(100), 0); // Проверка at() на выход за границы
	
	NextTest("Buffer.operator==(const Buffer&)");
	TEST_EQ(Buffer("A") == Buffer("A"), true);
	TEST_EQ(Buffer("A") == Buffer("B"), false);
	
	NextTest("Buffer.toInt() / toInt64()"); 
	Buffer buff_i4(4); buff_i4.writeUInt32BE(0x11223344); TEST_EQ(buff_i4.toInt(), 0x11223344U);
	Buffer buff_i8(8); buff_i8.writeUInt64BE(0x1122334455667788ULL); TEST_EQ(buff_i8.toInt64(), 0x1122334455667788ULL);
	
	NextTest("Buffer::htoc() / Buffer::ctoh()"); char hex[3]; hex[2]=0;
	TEST_EQ(Buffer::htoc("41"), 'A'); Buffer::ctoh(hex, 0x42); TEST_EQ(String(hex), "42");
	
	NextTest("Buffer::lltoh(unsigned long long value)");
	TEST_EQ(Buffer::lltoh(0x4142ULL), "4142");
	
	NextTest("Buffer::htoll(const String& str)");
	TEST_EQ(Buffer::htoll("4142"), 0x4142ULL);

// --------------------------------------------------------------------------------

	// Static-методы управления числами (BE)
	NextTest("Buffer::increment(Buffer&, value=1, insert=false)"); buff=Buffer(2, 0xff);
	Buffer::increment(buff); TEST_EQ(buff.toHex(), "0000");
	Buffer::increment(buff); TEST_EQ(buff.toHex(), "0001");
	Buffer::increment(buff, 2); TEST_EQ(buff.toHex(), "0003");
	buff=Buffer(1, 0x01); Buffer::increment(buff, 0x100, true); //256
	TEST_EQ(buff.toHex(), "0101"); // 0001 + 0100 = 0101 (BE)
	
	NextTest("Buffer::decrement(Buffer&, value=1, erase=false)"); buff=Buffer(2, 0);
	Buffer::decrement(buff); TEST_EQ(buff.toHex(), "ffff");
	Buffer::decrement(buff); TEST_EQ(buff.toHex(), "fffe");
	Buffer::decrement(buff, 2); TEST_EQ(buff.toHex(), "fffc");
	
	NextTest("Buffer::removeLeadingZeros(Buffer& buff)");
	Buffer z_buff=Buffer::fromHex("0000ff01"); Buffer::removeLeadingZeros(z_buff);
	TEST_EQ(z_buff.toHex(), "ff01");
	Buffer z_buff2=Buffer::fromHex("00"); Buffer::removeLeadingZeros(z_buff2);
	TEST_EQ(z_buff2.toHex(), "00"); // Один ноль должен остаться

// --------------------------------------------------------------------------------
	
	// Base64
	NextTest("Buffer::base64_encode(ptr, len)");
	TEST_EQ(Buffer::base64_encode((unsigned char*)"\x01\x02\x03", 3), "AQID");
	TEST_EQ(Buffer::base64_encode((unsigned char*)"\x01\x02", 2), "AQI=");
	TEST_EQ(Buffer::base64_encode((unsigned char*)"\x01", 1), "AQ==");

	NextTest("Buffer::base64_decode(encoded_string)");
	TEST_EQ(Buffer::base64_decode("AQID").toHex(), "010203");
	TEST_EQ(Buffer::base64_decode("AQI=").toHex(), "0102");
	TEST_EQ(Buffer::base64_decode("AQ==").toHex(), "01");

// --------------------------------------------------------------------------------

	// Дополнительные статические методы
	NextTest("Buffer::concat(const Array<Buffer>& buffs)");
	Array<Buffer> arr_buffs; arr_buffs.push(Buffer("A")).push(Buffer("B"));
	TEST_EQ(Buffer::concat(arr_buffs).toString(), "AB");
	
	NextTest("Buffer::concat(const Buffer& buff1, const Buffer& buff2)");
	TEST_EQ(Buffer::concat(Buffer("A"), Buffer("B")).toString(), "AB");
}

void Date_test(){
	NextTest("Date()"); Date date; TEST_EQ(date.timestamp, Date::now());
	NextTest("Date(long long msecs, char type='s')"); date=Date(2000); TEST_EQ(date.timestamp, 2000LL); 
	date=Date(2000, 's'); TEST_EQ(date.timestamp, 2000LL); date=Date(2000, 'm'); TEST_EQ(date.timestamp, 2LL);
	NextTest("Date(const CString& dateStr)"); date=Date("2026.02.02 01:09:05"); TEST_EQ(date.timestamp, 1769994545LL);
	NextTest("Date(IMFDate)"); date=Date("Tue, 05 May 2026 18:29:45 GMT"); TEST_EQ(date.timestamp, 1778005785LL);
	//NextTest("Date(const DValue& dv)");
	NextTest("Date::now()"); TEST_EQ(Date::now(), GetTimestamp('s'));
	
}

void Module_Base_test(){
	Array_test();
	String_test();
	///Dtos_test();
	Buffer_test();
	///HashMap_test();
	Date_test();
}
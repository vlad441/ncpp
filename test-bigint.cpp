#include <iostream>
#include "src/ncpp.cpp"

using namespace ncpp;

/*void matan_test(){ // 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
    std::cout << std::endl;
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); 
	//Buffer::Math::divide_knuth(r1, 65537, true).first; // === Crush Test
	Buffer::Math::divide_single(r1, 65537, true);
	std::cout << "Buffer::Math::divide_single (etalon): " << r1 << " (R: 2)" << std::endl; // эталонное значение
	std::cout << "-------Buffer::Math::divide: " << Buffer::Math::divide_knuth(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537")).first.toIntString() << std::endl;
	std::cout << "---Buffer::Math::divide: 180/7 = " << Buffer::Math::divide_knuth(BigInt("180"), BigInt("7")).first.toIntString() << " (Correct: ~25.71 - R: 5)" << std::endl;
	std::cout << "---Buffer::Math::divide: 500/80 = " << Buffer::Math::divide_knuth(BigInt("500"), BigInt("80")).first.toIntString() << " (Correct: 6,25 - R: 20)" << std::endl;
	std::cout << "---Buffer::Math::divide: 23242/123 = " << Buffer::Math::divide_knuth(BigInt("23242"), BigInt("123")).first.toIntString() << " (Correct: ~188,95 - R: 118)" << std::endl;
	std::cout << "---Buffer::Math::divide: 1892642/1623 = " << Buffer::Math::divide_knuth(BigInt("1892642"), BigInt("1623")).first.toIntString() << " (Correct: ~1166,13 - R: 224)" << std::endl;
	std::cout << "---Buffer::Math::divide: 162305008000/12305008000 = " << Buffer::Math::divide_knuth(BigInt("162305008000"), BigInt("12305008000")).first.toIntString() << " (Correct: ~13.19 - R: 2339904000)" << std::endl;
}*/

void matan_test(){ // 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
    std::cout << std::endl;
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); Buffer::Math::divide_single(r1, 65537, true);
	std::cout << "Buffer::Math::divide_single: " << r1 << std::endl;
	std::cout << "-------Buffer::Math::divide: " << Buffer::Math::divide_and_mod(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537")).first.toIntString() << std::endl;
	std::cout << "---Buffer::Math::divide: 180/7 = " << Buffer::Math::divide_and_mod(BigInt("180"), BigInt("7")).first.toIntString() << " (Correct: ~25.71 - R: 5)" << std::endl;
	std::cout << "---Buffer::Math::divide: 500/80 = " << Buffer::Math::divide_and_mod(BigInt("500"), BigInt("80")).first.toIntString() << " (Correct: 6,25 - R: 20)" << std::endl;
	std::cout << "---Buffer::Math::divide: 23242/123 = " << Buffer::Math::divide_and_mod(BigInt("23242"), BigInt("123")).first.toIntString() << " (Correct: ~188,95 - R: 118)" << std::endl;
	std::cout << "---Buffer::Math::divide: 1892642/1623 = " << Buffer::Math::divide_and_mod(BigInt("1892642"), BigInt("1623")).first.toIntString() << " (Correct: ~1166,13 - R: 224)" << std::endl;
	std::cout << "---Buffer::Math::divide: 162305008000/12305008000 = " << Buffer::Math::divide_and_mod(BigInt("162305008000"), BigInt("12305008000")).first.toIntString() << " (Correct: ~13.19 - R: 2339904000)" << " | R: " << Buffer::Math::divide_and_mod(BigInt("162305008000"), BigInt("12305008000")).second.toIntString() << std::endl;
}

void cpp_version(){ print("[");
#if __cplusplus >= 202302L
	print(">=C++23");
#elif __cplusplus >= 202002L
	print("C++20");
#elif __cplusplus >= 201703L
	print("C++17");
#elif __cplusplus >= 201402L
	print("C++14");
#elif __cplusplus >= 201103L
	print("C++11");
#else
	print("c++98");
#endif
	print("] UTF-8 кирилица にも含み\n");
};

void gcc_version(){ print("[Compiller] ");
#if defined(__clang__)
	print("Clang "); print(__VERSION__); print("\n"); //std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << "." <<  __clang_patchlevel__ << std::endl;
#elif defined(__GNUC__)
	print("GCC "); print(__VERSION__); print("\n"); //std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__ << std::endl;
#elif defined(_MSC_VER)
	print("MSVC? WTF?\n");
#else
	print("(Unknown)\n");
#endif
};

int main(){ cpp_version(); gcc_version(); matan_test(); }

#include "../base/print.cpp"
#include "base.c"
#include "../base/dtos.cpp"

void dtosE(double d){ union { double d; unsigned long long u; } du; du.d=d; unsigned long long u = du.u;

    char sign = (u >> 63) & 1;
    int exp_field = (u >> 52) & 0x7FF; // 11 бит экспоненты
    unsigned long long mantissa = u & 0xFFFFFFFFFFFFF; // 52 бита мантиссы

    if(exp_field == 0x7FF){ // Обработка (NaN и Infinity)
		if(mantissa==0){ printf("%sInf\n", sign?"-":""); }
		else{ printf("NaN\n"); } return; }

    int unbiased_exp; int leading_digit;

    // 2. Определение нормализованного или денормализованного режима
    if(exp_field == 0){ if(mantissa == 0){ printf("%s0.0 * 2^0\n", sign ? "-" : ""); return; }
        // Денормализованное число: порядок фиксирован, а впереди неявный ноль
        unbiased_exp = 1-1023; leading_digit = 0; // -1022
    }else{ // Нормализованное число: реальный порядок = поле - смещение (1023)
        unbiased_exp = exp_field-1023; leading_digit = 1; 
	}
	
    // Собираем мантиссу целиком: неявная цифра + дробная часть
    double dec_mantissa = leading_digit + ((double)mantissa/4503599627370496.0);

    // Вывод в десятичном виде
    printf("%s%.15f * 2^%d\n", sign?"-":"", dec_mantissa, unbiased_exp);
}

void dtosE_v2(double d){ if (d == 0.0){ printf("0.0 * 10^0\n"); return; }
    char sign = (d < 0)?1:0; if (sign) d = -d; int exp10 = 0;

    // Быстрый поиск десятичного порядка бинарным делением границ
    if(d >= 1e16){ d /= 1e16; exp10 += 16; }
    if(d >= 1e8){ d /= 1e8; exp10 += 8;  }
    if(d >= 1e4){ d /= 1e4; exp10 += 4;  }
    if(d >= 1e2){ d /= 1e2; exp10 += 2;  }
    if(d >= 1e1){ d /= 1e1; exp10 += 1;  }

    if(d < 1e-15){ d *= 1e16; exp10 -= 16; }
    if(d < 1e-7){ d *= 1e8; exp10 -= 8;  }
    if(d < 1e-3){ d *= 1e4; exp10 -= 4;  }
    if(d < 1e-1){ d *= 1e2; exp10 -= 2;  }
    if(d < 1.0){ d *= 1e1; exp10 -= 1;  }

    // Финальная микроподгонка (если из-за точности double вышли за границы [1, 10))
    if(d >= 10.0){ d /= 10.0; exp10+=1; }
    else if(d < 1.0 && d > 0.0){ d *= 10.0; exp10-=1; }

    // На выходе 'd' >= 1.0 && <= 9.999...
    printf("Double: %s%.15f * 10^%d\n", sign?"-":"", d, exp10);
}

//Double.MAX_VALUE = ≈1.7976931348623157×10^308
//Double.MIN_VALUE = ≈2.2250738585072014×10^−308
//Double: Denorm ABS_MIN_VALUE = ≈4,9406564584124654×10^−324
void dtosE_v3(double d){ if (d == 0.0){ printf("0.0 * 10^0\n"); return; } char sign = (d < 0)?1:0; if (sign) d = -d;
	double exp10_0 = log10(d); //Debug
    int exp10 = floor(log10(d)); // Вычисляем порядок через логарифм (покрывает весь диапазон double от 1e-308 до 1e308)
    double norm = d*pow(10.0, -exp10); // Нормализуем число в диапазон [1, 10) с помощью pow или деления
   
    //if(norm >= 10.0){ norm /= 10.0; exp10 += 1; } else if(norm < 1.0){ norm *= 10.0; exp10 -= 1; }

    printf("Double: %s%.15f * 10^%d\n", sign ? "-" : "", norm, exp10);
	printf("exp10_0: %.15f\n", exp10_0);
}

//int main(){ print("Hello, C!"); }
// int main(){ char str[27]; itos(str, 6767, 0); print(str); 
	// size_t len = strlen(str); itos(str, len, 0); print(" | len: "); print(str); }
int main(){ dtosE_v3(1.616255e-35); } //dtosE_v3(1.616255e-35);
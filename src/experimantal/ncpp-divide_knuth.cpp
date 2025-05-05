//26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
static std::pair<Buffer, Buffer> divide_knuth(const Buffer& a, const Buffer& b, bool resize = true, char Bsize = 1){
	size_t n = b.size(); size_t m = a.size() - n;
	Buffer u = a; Buffer v = b; Buffer remainder(n), q(m + 1);

	unsigned int B = 1 << (Bsize * 8);
	unsigned int d = (B - 1) / v[0];
	multiply_single(u, d);
	multiply_single(v, d);

	for (size_t j = 0; j <= m; ++j){
		unsigned int Qhat = (u[j] * B + u[j + 1]) / v[0];
		unsigned int Rhat = (u[j] * B + u[j + 1]) % v[0];

		while (Qhat == B || (n > 1 && Qhat * v[1] > Rhat * B + u[j + 2])) {
			Qhat--;
			Rhat += v[0];
			if (Rhat >= B) break;
		}

		Buffer VQhat = v;
		multiply_single(VQhat, Qhat);

		bool is_borrow = false;
		for (size_t k = 0; k < VQhat.size(); k++) {
			int diff = u[j + k] - VQhat[k];
			if (diff < 0) {
				u[j + k] = diff + B;
				if (k + 1 < VQhat.size()) {
					u[j + k + 1]--;
				}
				is_borrow = true;
			} else {
				u[j + k] = diff;
			}
		}

		q[j] = Qhat;
		if(is_borrow){ q[j]--; 
			Buffer V_temp = v; 
			u = add(u, V_temp);
		}
	}

	remainder.assign(u.begin(), u.begin() + n);
	divide_single(remainder, d);

	return std::make_pair(q, remainder);
}

static std::pair<Buffer, Buffer> divide_knuth_Progeritsfera(const Buffer& a, const Buffer& b, bool resize=false) {
    // Проверка на деление на ноль
    if (b.empty() || (b.size() == 1 && b[0] == 0)){ throw std::runtime_error("Division by zero"); }

    // Если делимое меньше делителя, возвращаем 0 и само делимое
    if (compare(a, b) < 0){ return std::make_pair(Buffer(1, 0), a); }

    // Определяем размер машинного слова
    size_t word_size = sizeof(unsigned long long);
    size_t n = (b.size() + word_size - 1) / word_size;
    size_t m = (a.size() + word_size - 1) / word_size - n;

    // Преобразуем Buffer в вектор машинных слов
    std::vector<unsigned long long> u(m + n + 1, 0), v(n, 0);
    for (size_t i = 0; i < a.size(); ++i) {
        u[i / word_size] |= static_cast<unsigned long long>(a[a.size() - 1 - i]) << (8 * (i % word_size));
    }
    for (size_t i = 0; i < b.size(); ++i) {
        v[i / word_size] |= static_cast<unsigned long long>(b[b.size() - 1 - i]) << (8 * (i % word_size));
    }

    // Нормализация
    int shift = 0;
    unsigned long long high_bit = v[n - 1];
    while (high_bit != 0) {
        high_bit >>= 1;
        shift++;
    }

    for (size_t i = n - 1; i > 0; --i) {
        v[i] = (v[i] << shift) | (v[i - 1] >> (64 - shift));
    }
    v[0] <<= shift;

    for (size_t i = m + n; i > 0; --i) {
        u[i] = (u[i] << shift) | (u[i - 1] >> (64 - shift));
    }
    u[0] <<= shift;

    // Основной цикл деления
    std::vector<unsigned long long> q(m + 1, 0);
    for (int j = m; j >= 0; --j) {
        unsigned long long qhat = (u[j + n] * (1ULL << 32) + u[j + n - 1]) / v[n - 1];
        unsigned long long rhat = (u[j + n] * (1ULL << 32) + u[j + n - 1]) - qhat * v[n - 1];

        while (qhat * v[n - 2] > (rhat * (1ULL << 32) + u[j + n - 2])) {
            qhat--;
            rhat += v[n - 1];
            if (rhat >= (1ULL << 32)) break;
        }

        // Умножение и вычитание
        long long k = 0, t;
        for (size_t i = 0; i < n; ++i) {
            unsigned long long p = qhat * v[i];
            t = u[i + j] - k - (p & 0xFFFFFFFF);
            u[i + j] = t;
            k = (p >> 32) - (t >> 32);
        }
        t = u[j + n] - k;
        u[j + n] = t;

        q[j] = qhat;
        if (t < 0) {
            q[j]--;
            k = 0;
            for (size_t i = 0; i < n; ++i) {
                t = u[i + j] + v[i] + k;
                u[i + j] = t;
                k = t >> 32;
            }
            u[j + n] += k;
        }
    }

    // Формирование результата
    Buffer quotient, remainder;
    for (size_t i = 0; i <= m; ++i) {
        for (size_t j = 0; j < word_size && quotient.size() < a.size(); ++j) {
            quotient.insert(quotient.begin(), (q[i] >> (8 * j)) & 0xFF);
        }
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < word_size && remainder.size() < b.size(); ++j) {
            remainder.insert(remainder.begin(), (u[i] >> (8 * (word_size - 1 - j))) & 0xFF);
        }
    }

    // Денормализация остатка
    for (size_t i = 0; i < remainder.size() - 1; ++i) {
        remainder[i] = (remainder[i] >> shift) | (remainder[i + 1] << (8 - shift));
    }
    remainder.back() >>= shift;

    // Удаление ведущих нулей
    while (!quotient.empty() && quotient.front() == 0) quotient.erase(quotient.begin());
    while (!remainder.empty() && remainder.front() == 0) remainder.erase(remainder.begin());

    if (resize) {
        if (quotient.empty()) quotient.push_back(0);
        if (remainder.empty()) remainder.push_back(0);
    }

    return std::make_pair(quotient, remainder);
}

static std::pair<Buffer, Buffer> divmod(const Buffer& a1, const Buffer& b1){
    if (b1.empty()){ throw std::invalid_argument("Division by zero"); }

    // Нормализация
    unsigned int norm = 256 / (b1.back() + 1);
    Buffer a=a1; multiply_single(a, norm);
    Buffer b=b1; multiply_single(b, norm);
    Buffer q(a.size()), r;

    for (size_t i = 0; i < a.size(); ++i){
        multiply_single(r, 256); increment(r, a[i]); 

        unsigned int s1 = b.size() < r.size() ? r[r.size() - b.size()] : 0;
        unsigned int s2 = b.size() - 1 < r.size() ? r[r.size() - b.size() + 1] : 0;
        unsigned int d = ((s1 * 256 + s2) / b.back());
		Buffer tmp1=b; multiply_single(tmp1, d);
        r = subtract(r, tmp1); while(compare(r,b) < 0){ r = add(r,b); --d; } q[i] = d;
    }
	removeLeadingZeros(q); removeLeadingZeros(r); divide_single(r, norm); return std::make_pair(q, r);
}
[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::crypto::randomBytes()](#ncppcryptorandombytes)
- [ncpp::crypto::AES::ECB_encrypt](#ncppcryptoaesecb_encrypt)
- [ncpp::crypto::AES::ECB_decrypt](#ncppcryptoaesecb_decrypt)
- [ncpp::crypto::AES::CBC_encrypt](#ncppcryptoaescbc_encrypt)
- [ncpp::crypto::AES::CBC_decrypt](#ncppcryptoaescbc_decrypt)
- [ncpp::crypto::AES::GCM_encrypt](#ncppcryptoaesgcm_encrypt)
- [ncpp::crypto::AES::GCM_decrypt](#ncppcryptoaesgcm_decrypt)
- [ncpp::crypto::MD5](#ncppcryptomd5)
	- [MD5::hash()](#md5hash)
	- [MD5.reset()](#md5reset)
	- [MD5.update()](#md5update)
	- [MD5.digest()](#md5digest)
- [ncpp::crypto::SHA1](#ncppcryptosha1)
- [ncpp::crypto::SHA256](#ncppcryptosha256)
- [ncpp::crypto::SHA512](#ncppcryptosha512)
- [ncpp::crypto::SHA3](#ncppcryptosha3)
- [ncpp::crypto::checksum()](#ncppcryptochecksum)

## ncpp::crypto::randomBytes()
```cpp
Buffer randomBytes(size_t length)
```
Использует API операционной системы для обеспечения случайности которая может подходить для криптографических целей.
Шифрует по алгоритму `AES-ECB`.
	
## ncpp::crypto::AES::ECB_encrypt
```cpp
Buffer ECB_encrypt(const Buffer& key, Buffer value)
```
Шифрует по алгоритму `AES-ECB`.

## ncpp::crypto::AES::ECB_decrypt
```cpp
Buffer ECB_decrypt(const Buffer& key, Buffer value)
```
Расшифровывает по алгоритму `AES-ECB`.

## ncpp::crypto::AES::CBC_encrypt
```cpp
Buffer CBC_encrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(16))
```
Шифрует по алгоритму `AES-CBC`.

## ncpp::crypto::AES::CBC_decrypt
```cpp
Buffer CBC_decrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(16))
```
Расшифровывает по алгоритму `AES-CBC`.

## ncpp::crypto::AES::GCM_encrypt
```cpp
Buffer GCM_encrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(12), Buffer aad=Buffer())
```
Шифрует по алгоритму `AES-GCM`.

## ncpp::crypto::AES::GCM_decrypt
```cpp
Buffer GCM_decrypt(const Buffer& key, Buffer value, Buffer iv=Buffer(12), Buffer aad=Buffer())
```
Расшифровывает по алгоритму `AES-GCM`.
	
## ncpp::crypto::MD5
```cpp
MD5();
MD5(const Buffer& buff);
```
Структура хеша MD5.

### MD5::hash()
```cpp
static Buffer hash(const Buffer& input){ return MD5().finalize(input); }
```
Выполняет хеширование без сохранения состояния.

### MD5.reset()
```cpp
void reset();
```
Сбрасывает состояние хеширования.

### MD5.update()
```cpp
MD5& update(const Buffer& input);
```
Обновляет состояние хеша.

### MD5.digest()
```cpp
Buffer digest();
```
Подсчитывает окончательный хеш и возвращает его.

## ncpp::crypto::SHA1
Эта структура имеет конструктор и методы аналогичные `MD5`.

## ncpp::crypto::SHA256
Эта структура представлеят семейство `SHA2` 256 битную версию. Имеет конструктор и методы аналогичные `MD5`.

## ncpp::crypto::SHA512
```cpp
SHA512(int sha_size=512);
SHA512(const Buffer& buff);
```
Эта структура представлеят семейство `SHA2` 512 битную версию. Может работать и в 384 битном режиме. Имеет методы аналогичные `MD5`.

## ncpp::crypto::SHA3
```cpp
SHA3(int bitlen = 256);
SHA3(const Buffer& input, int bitlen = 256);
```
Эта структура представлеят семейство `SHA3`, поддерживает произвольную битность. Имеет методы аналогичные `MD5`.

## ncpp::crypto::checksum()
```cpp
template <typename T>
Buffer checksum(T& algo, const String& path)
```
Использует созданный экземпляр `algo` для вычисления контрольной суммы файла.

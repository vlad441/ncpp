// Diffie-Hellman implementation
namespace ncpp { namespace crypto { namespace DH {

// == Параметры по умолчанию (RFC 3526 Group 14: 2048-bit MODP ==)
const BigInt DEFAULT_P = Buffer("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E08"\
    "8A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD"\
    "3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6"\
    "F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381FFFFFFFFFFFFFFFF", "hex");
const BigInt DEFAULT_G = 2;

typedef std::pair<BigInt, BigInt> KeyPair;

// Генерация пары ключей
KeyPair generateKeyPair(const BigInt& p = DEFAULT_P, const BigInt& g = DEFAULT_G, int privBits = 256){
    BigInt priv = BigInt(crypto::randomBytes(privBits / 8));
    BigInt pub = BigInt::powMod(g, priv, p);
    return std::make_pair(priv, pub); }

// Вычисление общего секрета
BigInt computeSecret(const BigInt& otherPublicKey, const BigInt& privateKey, const BigInt& p = DEFAULT_P){
    return BigInt::powMod(otherPublicKey, privateKey, p); }
}}}

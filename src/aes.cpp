#include "aes.h"
#include <cstring>   
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
static CryptoPP::SecByteBlock prepareKey(const std::string& key) {
    CryptoPP::SecByteBlock k(CryptoPP::AES::MAX_KEYLENGTH); // always 32 bytes
    size_t len = std::min(key.size(), (size_t)CryptoPP::AES::MAX_KEYLENGTH);
    std::memcpy(k.data(), key.data(), len);
    if (len < CryptoPP::AES::MAX_KEYLENGTH)
        std::memset(k.data() + len, 0x00, CryptoPP::AES::MAX_KEYLENGTH - len);
    return k;
}
// Returns hex(IV) + ":" + hex(ciphertext)
std::string aesEncrypt(const std::string& plaintext, const std::string& key) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::SecByteBlock k = prepareKey(key);

    // Generate random IV
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    std::string ciphertext, ivHex, cipherHex;

    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(k, k.size(), iv);

    CryptoPP::StringSource s(plaintext, true,
        new CryptoPP::StreamTransformationFilter(enc,
            new CryptoPP::StringSink(ciphertext)));

    // Encode IV and ciphertext to hex
    CryptoPP::StringSource iv1(iv, sizeof(iv), true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(ivHex)));
    CryptoPP::StringSource c1(ciphertext, true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(cipherHex)));

    return ivHex + ":" + cipherHex;
}

// Expects input as hex(IV) + ":" + hex(ciphertext)
std::string aesDecrypt(const std::string& input, const std::string& key) {
    CryptoPP::SecByteBlock k = prepareKey(key);

    // Split IV and ciphertext
    size_t sep = input.find(':');
    std::string ivHex = input.substr(0, sep);
    std::string cipherHex = input.substr(sep + 1);

    std::string ivStr, cipherStr, recovered;

    CryptoPP::StringSource iv1(ivHex, true,
        new CryptoPP::HexDecoder(new CryptoPP::StringSink(ivStr)));
    CryptoPP::StringSource c1(cipherHex, true,
        new CryptoPP::HexDecoder(new CryptoPP::StringSink(cipherStr)));

    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(k, k.size(),
        (const CryptoPP::byte*)ivStr.data());

    CryptoPP::StringSource s(cipherStr, true,
        new CryptoPP::StreamTransformationFilter(dec,
            new CryptoPP::StringSink(recovered)));

    return recovered;
}
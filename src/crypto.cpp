#include "crypto.h"
#include "aes.h"
#include "hmac.h"
#include <cryptopp/hex.h>        // ← add this
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cstring>
#include <iostream>

static CryptoPP::SecByteBlock prepareKey(const std::string& password) {
    CryptoPP::SecByteBlock k(CryptoPP::AES::MAX_KEYLENGTH);
    size_t len = std::min(password.size(), (size_t)CryptoPP::AES::MAX_KEYLENGTH);
    std::memcpy(k.data(), password.data(), len);
    if (len < CryptoPP::AES::MAX_KEYLENGTH)
        std::memset(k.data() + len, 0x00, CryptoPP::AES::MAX_KEYLENGTH - len);
    return k;
}

std::string encryptNote(const std::string& content, const std::string& password) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::SecByteBlock k = prepareKey(password);

    // Generate random IV
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    // Encrypt content
    std::string ciphertext;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(k, k.size(), iv);
    CryptoPP::StringSource s(content, true,
        new CryptoPP::StreamTransformationFilter(enc,
            new CryptoPP::StringSink(ciphertext)));

    // Generate HMAC over ciphertext
    std::string hmacHex = generateHMAC(ciphertext, password);

    // Convert HMAC hex back to raw bytes (32 bytes)
    std::string hmacRaw;
    CryptoPP::StringSource h(hmacHex, true,
        new CryptoPP::HexDecoder(new CryptoPP::StringSink(hmacRaw)));

    // Final format: IV (16) + HMAC (32) + ciphertext
    std::string result;
    result.append((char*)iv, sizeof(iv));
    result.append(hmacRaw);
    result.append(ciphertext);
    return result;
}

std::string decryptNote(const std::string& data, const std::string& password) {
    if (data.size() < 48) {
        std::cerr << "  ❌ Corrupted note file!\n";
        return "";
    }

    CryptoPP::SecByteBlock k = prepareKey(password);

    // Extract IV (first 16 bytes)
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
    std::memcpy(iv, data.data(), sizeof(iv));

    // Extract HMAC (next 32 bytes)
    std::string storedHmacRaw = data.substr(16, 32);

    // Extract ciphertext (rest)
    std::string ciphertext = data.substr(48);

    // Verify HMAC
    std::string computedHmacHex = generateHMAC(ciphertext, password);
    std::string computedHmacRaw;
    CryptoPP::StringSource h(computedHmacHex, true,
        new CryptoPP::HexDecoder(new CryptoPP::StringSink(computedHmacRaw)));

    if (storedHmacRaw != computedHmacRaw) {
        std::cerr << "  ❌ HMAC verification failed! Note may be tampered or wrong password.\n";
        return "";
    }

    // Decrypt
    try {
        std::string recovered;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(k, k.size(), iv);
        CryptoPP::StringSource s(ciphertext, true,
            new CryptoPP::StreamTransformationFilter(dec,
                new CryptoPP::StringSink(recovered)));
        return recovered;
    } catch (...) {
        std::cerr << "  ❌ Decryption failed! Wrong password.\n";
        return "";
    }
}
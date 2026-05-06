#include "hmac.h"
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string generateHMAC(const std::string& message, const std::string& key) {
    std::string digest;
    CryptoPP::HMAC<CryptoPP::SHA256> hmac(
        (const CryptoPP::byte*)key.data(), key.size());

    CryptoPP::StringSource s(message, true,
        new CryptoPP::HashFilter(hmac,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest))));
    return digest;
}

bool verifyHMAC(const std::string& message, const std::string& key,
                const std::string& expectedHMAC) {
    std::string computed = generateHMAC(message, key);
    return computed == expectedHMAC;
}
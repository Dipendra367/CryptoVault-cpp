#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "hashing.h"
#include <cryptopp/sha.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string sha256Hash(const std::string& input) {
    std::string digest;
    CryptoPP::SHA256 hash;
    CryptoPP::StringSource s(input, true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest))));
    return digest;
}

std::string md5Hash(const std::string& input) {
    std::string digest;
    CryptoPP::Weak::MD5 hash;
    CryptoPP::StringSource s(input, true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest))));
    return digest;
}
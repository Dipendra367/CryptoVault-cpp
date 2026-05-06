#include "fileencrypt.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <fstream>
#include <cstring>
#include <iostream>

static CryptoPP::SecByteBlock prepareKey(const std::string& key) {
    CryptoPP::SecByteBlock k(CryptoPP::AES::MAX_KEYLENGTH);
    size_t len = std::min(key.size(), (size_t)CryptoPP::AES::MAX_KEYLENGTH);
    std::memcpy(k.data(), key.data(), len);
    if (len < CryptoPP::AES::MAX_KEYLENGTH)
        std::memset(k.data() + len, 0x00, CryptoPP::AES::MAX_KEYLENGTH - len);
    return k;
}

bool encryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& key) {
    try {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::SecByteBlock k = prepareKey(key);

        // Generate random IV
        CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
        rng.GenerateBlock(iv, sizeof(iv));

        // Read input file
        std::ifstream inFile(inputPath, std::ios::binary);
        if (!inFile) { std::cerr << "Cannot open input file!\n"; return false; }
        std::string plaintext((std::istreambuf_iterator<char>(inFile)),
                               std::istreambuf_iterator<char>());
        inFile.close();

        // Encrypt
        std::string ciphertext;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc;
        enc.SetKeyWithIV(k, k.size(), iv);
        CryptoPP::StringSource s(plaintext, true,
            new CryptoPP::StreamTransformationFilter(enc,
                new CryptoPP::StringSink(ciphertext)));

        // Write IV + ciphertext to output file (raw binary)
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile) { std::cerr << "Cannot open output file!\n"; return false; }
        outFile.write((char*)iv, sizeof(iv));         // first 16 bytes = IV
        outFile.write(ciphertext.data(), ciphertext.size());
        outFile.close();

        std::cout << "File encrypted -> " << outputPath << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Encrypt error: " << e.what() << "\n";
        return false;
    }
}

bool decryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& key) {
    try {
        CryptoPP::SecByteBlock k = prepareKey(key);

        // Read encrypted file
        std::ifstream inFile(inputPath, std::ios::binary);
        if (!inFile) { std::cerr << "Cannot open input file!\n"; return false; }

        // First 16 bytes = IV
        CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];
        inFile.read((char*)iv, sizeof(iv));

        // Rest = ciphertext
        std::string ciphertext((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
        inFile.close();

        // Decrypt
        std::string recovered;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(k, k.size(), iv);
        CryptoPP::StringSource s(ciphertext, true,
            new CryptoPP::StreamTransformationFilter(dec,
                new CryptoPP::StringSink(recovered)));

        // Write decrypted output
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile) { std::cerr << "Cannot open output file!\n"; return false; }
        outFile.write(recovered.data(), recovered.size());
        outFile.close();

        std::cout << "File decrypted -> " << outputPath << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Decrypt error: " << e.what() << "\n";
        return false;
    }
}
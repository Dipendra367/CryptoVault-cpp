#include "signature.h"
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/pssr.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/queue.h>
#include <iostream>

void generateRSAKeys(std::string& privateKey, std::string& publicKey) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey rsaPrivate;
    rsaPrivate.GenerateRandomWithKeySize(rng, 2048);

    // Serialize private key
    CryptoPP::ByteQueue privQueue;
    rsaPrivate.DEREncode(privQueue);
    CryptoPP::StringSink privSink(privateKey);
    privQueue.CopyTo(privSink);
    privSink.MessageEnd();

    // Serialize public key
    CryptoPP::RSA::PublicKey rsaPublic(rsaPrivate);
    CryptoPP::ByteQueue pubQueue;
    rsaPublic.DEREncode(pubQueue);
    CryptoPP::StringSink pubSink(publicKey);
    pubQueue.CopyTo(pubSink);
    pubSink.MessageEnd();

    std::cout << "RSA-2048 keys generated!\n";
    std::cout << "Private key size : " << privateKey.size() << " bytes\n";
    std::cout << "Public key size  : " << publicKey.size() << " bytes\n";
}

std::string signMessage(const std::string& message, const std::string& privKeyStr) {
    CryptoPP::AutoSeededRandomPool rng;

    // Load private key
    CryptoPP::RSA::PrivateKey privateKey;
    CryptoPP::ByteQueue privQueue;
    privQueue.Put((const CryptoPP::byte*)privKeyStr.data(), privKeyStr.size());
    privateKey.BERDecode(privQueue);

    // Sign
    std::string signature;
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer(privateKey);
    CryptoPP::StringSource s(message, true,
        new CryptoPP::SignerFilter(rng, signer,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(signature))));
    return signature;
}

bool verifySignature(const std::string& message, const std::string& sigHex,
                     const std::string& pubKeyStr) {
    // Load public key
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::ByteQueue pubQueue;
    pubQueue.Put((const CryptoPP::byte*)pubKeyStr.data(), pubKeyStr.size());
    publicKey.BERDecode(pubQueue);

    // Decode hex signature
    std::string sigDecoded;
    CryptoPP::StringSource dec(sigHex, true,
        new CryptoPP::HexDecoder(new CryptoPP::StringSink(sigDecoded)));

    // Verify
    bool result = false;
    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Verifier verifier(publicKey);
    CryptoPP::StringSource s(sigDecoded + message, true,
        new CryptoPP::SignatureVerificationFilter(verifier,
            new CryptoPP::ArraySink((CryptoPP::byte*)&result, sizeof(result)),
            CryptoPP::SignatureVerificationFilter::PUT_RESULT |
            CryptoPP::SignatureVerificationFilter::SIGNATURE_AT_BEGIN));
    return result;
}
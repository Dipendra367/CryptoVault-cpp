#pragma once
#include <string>

void generateRSAKeys(std::string& privateKey, std::string& publicKey);
std::string signMessage(const std::string& message, const std::string& privateKey);
bool verifySignature(const std::string& message, const std::string& signature, const std::string& publicKey);
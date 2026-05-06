#pragma once
#include <string>

std::string aesEncrypt(const std::string& plaintext, const std::string& key);
std::string aesDecrypt(const std::string& ciphertext, const std::string& key);
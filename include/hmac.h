#pragma once
#include <string>

std::string generateHMAC(const std::string& message, const std::string& key);
bool verifyHMAC(const std::string& message, const std::string& key, const std::string& expectedHMAC);
#pragma once
#include <string>

std::string encryptNote(const std::string& content, const std::string& password);
std::string decryptNote(const std::string& data, const std::string& password);
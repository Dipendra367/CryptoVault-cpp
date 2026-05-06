#pragma once
#include <string>

bool encryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& key);

bool decryptFile(const std::string& inputPath,
                 const std::string& outputPath,
                 const std::string& key);
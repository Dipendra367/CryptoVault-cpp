#include "crypto.h"    // ← add this
#include "notes.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;
const std::string NOTES_DIR = "notes/";

// Convert title to filename: "my note" → "my-note.note"
static std::string titleToFilename(const std::string& title) {
    std::string filename = title;
    std::replace(filename.begin(), filename.end(), ' ', '-');
    return NOTES_DIR + filename + ".note";
}

bool createNote(const std::string& title,
                const std::string& content,
                const std::string& password) {
    // Create notes directory if it doesn't exist
    fs::create_directories(NOTES_DIR);

    std::string filepath = titleToFilename(title);

    // Check if note already exists
    if (fs::exists(filepath)) {
        std::cerr << "  ❌ Note '" << title << "' already exists!\n";
        return false;
    }

    // Encrypt content
    std::string encrypted = encryptNote(content, password);

    // Save to file
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "  ❌ Could not create note file!\n";
        return false;
    }
    file.write(encrypted.data(), encrypted.size());
    file.close();

    std::cout << "  ✅ Note '" << title << "' saved securely!\n";
    return true;
}

std::vector<std::string> listNotes() {
    std::vector<std::string> notes;
    if (!fs::exists(NOTES_DIR)) return notes;

    for (const auto& entry : fs::directory_iterator(NOTES_DIR)) {
        if (entry.path().extension() == ".note") {
            std::string name = entry.path().stem().string();
            std::replace(name.begin(), name.end(), '-', ' ');
            notes.push_back(name);
        }
    }
    return notes;
}

std::string readNote(const std::string& title,
                     const std::string& password) {
    std::string filepath = titleToFilename(title);

    if (!fs::exists(filepath)) {
        std::cerr << "  ❌ Note '" << title << "' not found!\n";
        return "";
    }

    // Read encrypted data
    std::ifstream file(filepath, std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    file.close();

    // Decrypt and return
    return decryptNote(data, password);
}

bool deleteNote(const std::string& title) {
    std::string filepath = titleToFilename(title);

    if (!fs::exists(filepath)) {
        std::cerr << "  ❌ Note '" << title << "' not found!\n";
        return false;
    }

    fs::remove(filepath);
    std::cout << "  ✅ Note '" << title << "' deleted!\n";
    return true;
}
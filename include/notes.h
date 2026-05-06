#pragma once
#include <string>
#include <vector>

// Save encrypted note to disk
bool createNote(const std::string& title,
                const std::string& content,
                const std::string& password);

// List all saved notes
std::vector<std::string> listNotes();

// Read and decrypt a note
std::string readNote(const std::string& title,
                     const std::string& password);

// Delete a note
bool deleteNote(const std::string& title);
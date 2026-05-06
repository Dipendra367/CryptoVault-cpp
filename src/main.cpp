#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "notes.h"

void printBanner() {
    std::cout << "\n";
    std::cout << "  ███████╗███████╗ ██████╗██╗   ██╗██████╗ ███████╗\n";
    std::cout << "  ██╔════╝██╔════╝██╔════╝██║   ██║██╔══██╗██╔════╝\n";
    std::cout << "  ███████╗█████╗  ██║     ██║   ██║██████╔╝█████╗  \n";
    std::cout << "  ╚════██║██╔══╝  ██║     ██║   ██║██╔══██╗██╔══╝  \n";
    std::cout << "  ███████║███████╗╚██████╗╚██████╔╝██║  ██║███████╗\n";
    std::cout << "  ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝\n";
    std::cout << "  ███╗   ██╗ ██████╗ ████████╗███████╗███████╗\n";
    std::cout << "  ████╗  ██║██╔═══██╗╚══██╔══╝██╔════╝██╔════╝\n";
    std::cout << "  ██╔██╗ ██║██║   ██║   ██║   █████╗  ███████╗\n";
    std::cout << "  ██║╚██╗██║██║   ██║   ██║   ██╔══╝  ╚════██║\n";
    std::cout << "  ██║ ╚████║╚██████╔╝   ██║   ███████╗███████║\n";
    std::cout << "  ╚═╝  ╚═══╝ ╚═════╝    ╚═╝   ╚══════╝╚══════╝\n";
    std::cout << "\n    🔐 Your personal encrypted notes vault\n";
    std::cout << "  ==========================================\n";
}

void printMenu() {
    std::cout << "\n  ╔══════════════════════════════════╗\n";
    std::cout << "  ║        SECURE NOTES MENU         ║\n";
    std::cout << "  ╠══════════════════════════════════╣\n";
    std::cout << "  ║  [1]  Create a new note          ║\n";
    std::cout << "  ║  [2]  List all notes             ║\n";
    std::cout << "  ║  [3]  Read a note                ║\n";
    std::cout << "  ║  [4]  Delete a note              ║\n";
    std::cout << "  ║  [0]  Exit                       ║\n";
    std::cout << "  ╚══════════════════════════════════╝\n";
    std::cout << "  Choice: ";
}

int main() {
    printBanner();

    int choice;
    std::string title, content, password, line;

    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n";

        switch (choice) {

            case 1: {
                std::cout << "  Enter note title   : ";
                std::getline(std::cin, title);
                std::cout << "  Enter note content : ";
                std::getline(std::cin, content);
                std::cout << "  Enter password     : ";
                std::getline(std::cin, password);
                createNote(title, content, password);
                break;
            }

            case 2: {
                std::vector<std::string> notes = listNotes();
                if (notes.empty()) {
                    std::cout << "  📭 No notes found!\n";
                } else {
                    std::cout << "  📋 Your notes:\n";
                    for (int i = 0; i < (int)notes.size(); i++)
                        std::cout << "     [" << i+1 << "] " << notes[i] << "\n";
                }
                break;
            }

            case 3: {
                std::vector<std::string> notes = listNotes();
                if (notes.empty()) {
                    std::cout << "  📭 No notes found!\n";
                    break;
                }
                std::cout << "  📋 Your notes:\n";
                for (int i = 0; i < (int)notes.size(); i++)
                    std::cout << "     [" << i+1 << "] " << notes[i] << "\n";

                std::cout << "  Enter note title   : ";
                std::getline(std::cin, title);
                std::cout << "  Enter password     : ";
                std::getline(std::cin, password);

                std::string result = readNote(title, password);
                if (!result.empty()) {
                    std::cout << "\n  ╔══════════════════════════════════╗\n";
                    std::cout << "  ║  📝 " << title << "\n";
                    std::cout << "  ╠══════════════════════════════════╣\n";
                    std::cout << "  ║  " << result << "\n";
                    std::cout << "  ╚══════════════════════════════════╝\n";
                }
                break;
            }

            case 4: {
                std::vector<std::string> notes = listNotes();
                if (notes.empty()) {
                    std::cout << "  📭 No notes found!\n";
                    break;
                }
                std::cout << "  📋 Your notes:\n";
                for (int i = 0; i < (int)notes.size(); i++)
                    std::cout << "     [" << i+1 << "] " << notes[i] << "\n";

                std::cout << "  Enter note title to delete : ";
                std::getline(std::cin, title);
                deleteNote(title);
                break;
            }

            case 0:
                std::cout << "  Goodbye! Your notes are safe. 🔐\n\n";
                break;

            default:
                std::cout << "  ❌ Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
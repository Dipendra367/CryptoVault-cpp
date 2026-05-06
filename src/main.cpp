#include <iostream>
#include <string>
#include <limits>
#include "hashing.h"
#include "aes.h"
#include "fileencrypt.h"
#include "hmac.h"
#include "signature.h"

// Stored RSA keys during session
std::string sessionPrivKey, sessionPubKey, lastSignature, lastSignedMsg;

void printBanner() {
    std::cout << "\n";
    std::cout << "  ██████╗██████╗ ██╗   ██╗██████╗ ████████╗ ██████╗\n";
    std::cout << " ██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝██╔═══██╗\n";
    std::cout << " ██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   ██║   ██║\n";
    std::cout << " ██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   ██║   ██║\n";
    std::cout << " ╚██████╗██║  ██║   ██║   ██║        ██║   ╚██████╔╝\n";
    std::cout << "  ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝    ╚═════╝\n";
    std::cout << "          ██╗   ██╗ █████╗ ██╗   ██╗██╗  ████████╗\n";
    std::cout << "          ██║   ██║██╔══██╗██║   ██║██║  ╚══██╔══╝\n";
    std::cout << "          ██║   ██║███████║██║   ██║██║     ██║\n";
    std::cout << "          ╚██╗ ██╔╝██╔══██║██║   ██║██║     ██║\n";
    std::cout << "           ╚████╔╝ ██║  ██║╚██████╔╝███████╗██║\n";
    std::cout << "            ╚═══╝  ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝\n";
    std::cout << "\n       C++ Cryptography Toolkit  |  by Dipendra\n";
    std::cout << "  ================================================\n";
}

void printMenu() {
    std::cout << "\n  ╔══════════════════════════════════╗\n";
    std::cout << "  ║         CRYPTOVAULT MENU         ║\n";
    std::cout << "  ╠══════════════════════════════════╣\n";
    std::cout << "  ║  [1]  SHA-256 Hash               ║\n";
    std::cout << "  ║  [2]  MD5 Hash                   ║\n";
    std::cout << "  ║  [3]  AES-256 Encrypt            ║\n";
    std::cout << "  ║  [4]  AES-256 Decrypt            ║\n";
    std::cout << "  ║  [5]  Encrypt a File             ║\n";
    std::cout << "  ║  [6]  Decrypt a File             ║\n";
    std::cout << "  ║  [7]  Generate HMAC              ║\n";
    std::cout << "  ║  [8]  Verify HMAC                ║\n";
    std::cout << "  ║  [9]  Generate RSA Keys          ║\n";
    std::cout << "  ║  [10] Sign a Message             ║\n";
    std::cout << "  ║  [11] Verify Signature           ║\n";
    std::cout << "  ║  [0]  Exit                       ║\n";
    std::cout << "  ╚══════════════════════════════════╝\n";
    std::cout << "  Choice: ";
}

int main() {
    printBanner();

    int choice;
    std::string input, key, input2;

    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "\n";

        switch (choice) {

            case 1:
                std::cout << "  Enter message: ";
                std::getline(std::cin, input);
                std::cout << "  SHA-256 : " << sha256Hash(input) << "\n";
                break;

            case 2:
                std::cout << "  Enter message: ";
                std::getline(std::cin, input);
                std::cout << "  MD5     : " << md5Hash(input) << "\n";
                break;

            case 3:
                std::cout << "  Enter plaintext : ";
                std::getline(std::cin, input);
                std::cout << "  Enter key       : ";
                std::getline(std::cin, key);
                std::cout << "  Encrypted : " << aesEncrypt(input, key) << "\n";
                break;

            case 4:
                std::cout << "  Enter ciphertext (IV:HEX format) : ";
                std::getline(std::cin, input);
                std::cout << "  Enter key                        : ";
                std::getline(std::cin, key);
                try {
                    std::cout << "  Decrypted : " << aesDecrypt(input, key) << "\n";
                } catch (...) {
                    std::cout << "  ❌ Decryption failed! Wrong key or corrupted data.\n";
                }
                break;

            case 5:
                std::cout << "  Enter input file path  : ";
                std::getline(std::cin, input);
                std::cout << "  Enter output file path : ";
                std::getline(std::cin, input2);
                std::cout << "  Enter key              : ";
                std::getline(std::cin, key);
                encryptFile(input, input2, key);
                break;

            case 6:
                std::cout << "  Enter encrypted file path : ";
                std::getline(std::cin, input);
                std::cout << "  Enter output file path    : ";
                std::getline(std::cin, input2);
                std::cout << "  Enter key                 : ";
                std::getline(std::cin, key);
                decryptFile(input, input2, key);
                break;

            case 7:
                std::cout << "  Enter message : ";
                std::getline(std::cin, input);
                std::cout << "  Enter key     : ";
                std::getline(std::cin, key);
                std::cout << "  HMAC-SHA256 : " << generateHMAC(input, key) << "\n";
                break;

            case 8:
                std::cout << "  Enter message       : ";
                std::getline(std::cin, input);
                std::cout << "  Enter key           : ";
                std::getline(std::cin, key);
                std::cout << "  Enter expected HMAC : ";
                std::getline(std::cin, input2);
                std::cout << (verifyHMAC(input, key, input2) ? "  ✅ HMAC Valid!" : "  ❌ HMAC Invalid!") << "\n";
                break;

            case 9:
                std::cout << "  Generating RSA-2048 keys (please wait...):\n";
                generateRSAKeys(sessionPrivKey, sessionPubKey);
                std::cout << "  ✅ Keys stored in session memory!\n";
                break;

            case 10:
                if (sessionPrivKey.empty()) {
                    std::cout << "  ❌ No RSA keys found! Generate keys first (option 9).\n";
                    break;
                }
                std::cout << "  Enter message to sign : ";
                std::getline(std::cin, input);
                lastSignedMsg = input;
                lastSignature = signMessage(input, sessionPrivKey);
                std::cout << "  ✅ Message signed!\n";
                std::cout << "  Signature (first 64 chars): "
                          << lastSignature.substr(0, 64) << "...\n";
                break;

            case 11:
                if (sessionPubKey.empty()) {
                    std::cout << "  ❌ No RSA keys found! Generate keys first (option 9).\n";
                    break;
                }
                if (lastSignature.empty()) {
                    std::cout << "  ❌ No signature found! Sign a message first (option 10).\n";
                    break;
                }
                std::cout << "  Enter message to verify : ";
                std::getline(std::cin, input);
                std::cout << (verifySignature(input, lastSignature, sessionPubKey)
                    ? "  ✅ Signature Valid! Message is authentic."
                    : "  ❌ Signature Invalid! Message may be tampered.") << "\n";
                break;

            case 0:
                std::cout << "  Goodbye! Stay secure. 🔐\n\n";
                break;

            default:
                std::cout << "  ❌ Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
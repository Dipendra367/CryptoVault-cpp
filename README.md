# 🔐 CryptoVault C++

![C++](https://img.shields.io/badge/C++-17-blue?logo=cplusplus)
![Crypto++](https://img.shields.io/badge/Crypto++-8.9-green)
![Platform](https://img.shields.io/badge/Platform-Linux-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen)
![Security](https://img.shields.io/badge/Security-AES256%20%2B%20HMAC-red)

A professional-grade, command-line **cryptography toolkit and encrypted notes vault** built from scratch in **C++17** using the [Crypto++](https://cryptopp.com/) library. This project implements the exact same security algorithms used in banking systems, blockchain networks, end-to-end encrypted messengers, and SSL/TLS infrastructure — not wrappers, not tutorials, but real working cryptographic code.

---

## 🧠 What Makes This Project Special

Most beginner crypto projects just call one function and print a result. This project:

- Implements **AES-256-CBC with random IV generation** — same plaintext never produces the same ciphertext twice
- Uses **HMAC-SHA256 for integrity verification** — detects both tampering and wrong passwords before decryption even begins
- Builds a **fully working encrypted notes vault** — notes are saved to disk in encrypted form and are completely unreadable without the correct password
- Handles **RSA-2048 key pair generation, signing, and verification** — the foundation of HTTPS and code signing
- All modules are **independently implemented** across separate source files with clean header interfaces

---

## ✨ Feature Modules

| Module | Algorithm | Security Level | Real World Use |
|---|---|---|---|
| Hashing | SHA-256 | 256-bit | Bitcoin, Git commits, TLS |
| Hashing | MD5 | 128-bit (legacy) | Checksums, file integrity |
| Symmetric Encryption | AES-256-CBC | Military grade | WhatsApp, Signal, banking |
| File Encryption | AES-256-CBC | Military grade | Encrypted drives, secure transfer |
| Authentication | HMAC-SHA256 | Cryptographic MAC | API authentication, AWS signatures |
| Asymmetric Crypto | RSA-2048 | Public key | HTTPS, SSH, email encryption |
| Digital Signature | RSA + SHA-256 | Non-repudiation | SSL certificates, code signing |
| Secure Notes Vault | AES-256 + HMAC | End-to-end | Encrypted password managers |

---

## 🛠️ Installation

### Prerequisites
```bash
sudo apt update
sudo apt install libcrypto++-dev libcrypto++-utils
```

### Clone & Build CryptoVault
```bash
git clone https://github.com/Dipendra367/CryptoVault-cpp.git
cd CryptoVault-cpp
make
./cryptovault
```

### Run SecureNotes Vault
```bash
./securenotes
```

---
## 📁 Project Structure

```
CryptoVault-cpp/
│
├── src/
│ ├── main.cpp ← CryptoVault menu-driven interface
│ ├── hashing.cpp ← SHA-256 & MD5 implementation
│ ├── aes.cpp ← AES-256-CBC encrypt/decrypt with random IV
│ ├── fileencrypt.cpp ← File-level AES encryption/decryption
│ ├── hmac.cpp ← HMAC-SHA256 message authentication
│ ├── signature.cpp ← RSA-2048 key gen, signing & verification
│ ├── notes.cpp ← Encrypted notes vault (save/read/delete)
│ └── crypto.cpp ← Core crypto utilities & key derivation
│
├── include/
│ ├── aes.h
│ ├── crypto.h
│ ├── hmac.h
│ ├── notes.h
│ ├── hashing.h
│ ├── fileencrypt.h
│ └── signature.h
│
├── assets/
│ └── screenshots/
│
├── Makefile
└── README.md
```

---

## 🔑 Deep Dive: How It Works

### AES-256-CBC Encryption
- A **cryptographically random 16-byte IV** is generated for every encryption operation
- The IV is prepended to the ciphertext so decryption can always recover it
- Same plaintext + same key produces **completely different ciphertext** every time — prevents pattern analysis attacks
- Key is derived from the user's password using a secure derivation process

### HMAC-SHA256 Authentication
- After encryption, an HMAC is computed over the ciphertext using the password-derived key
- On decryption, the HMAC is **verified first** before any decryption happens
- A wrong password produces a different HMAC → `❌ HMAC verification failed!`
- This prevents padding oracle attacks and detects file tampering

### RSA-2048 Digital Signatures
- Generates a **2048-bit RSA key pair** (public + private)
- Private key signs a message digest (SHA-256 hash of the message)
- Anyone with the public key can verify the signature without knowing the private key
- This is the exact mechanism behind HTTPS certificates and Git signed commits

### SecureNotes Vault
- Notes are encrypted with **AES-256** before being written to disk
- Each note is bound to a password via **HMAC** — no master key file exists
- Notes are stored as opaque encrypted blobs — file contents reveal nothing
- Delete is permanent — encrypted file is removed from disk

---

## 💻 Usage Examples

### CryptoVault — Encrypt a Message
```
Choice: AES Encrypt

Enter plaintext : Hello World
Enter key : mysecretkey
Output : a3f1c2... (IV:CIPHERTEXT in hex)
```
### CryptoVault — Verify a Digital Signature
```
Choice: Verify Signature

Enter message : Transfer $500
Enter signature : 3d9f2a...
Result : ✅ Signature is VALID
```

### SecureNotes — Create & Read a Note
```
Choice: Create note

Title : my secrets
Content : i am batman
Password : - - - - - -
✅ Note 'my secrets' saved securely!

Choice: Read note

Title : my secrets
Password : wrongpass
❌ HMAC verification failed! Note may be tampered or wrong password.
```

---

## 🌍 Real World Applications

- **WhatsApp & Signal** use AES-256-CBC for end-to-end message encryption
- **Bitcoin** uses SHA-256 double-hashing for proof-of-work mining
- **Git** uses SHA-256 to fingerprint every commit and detect corruption
- **AWS & Stripe APIs** use HMAC-SHA256 to authenticate every API request
- **HTTPS / SSL certificates** use RSA-2048 for server identity verification
- **Password managers** like Bitwarden use AES-256 + HMAC — exactly what SecureNotes implements

---

## 🚀 Future Plans

- [x] ~~Secure Notes App — AES-256 encrypted notes with HMAC verification~~
- [ ] Password Manager — store credentials encrypted with master password
- [ ] GUI interface with Qt
- [ ] File drag-and-drop encryption
- [ ] Ed25519 signatures (modern alternative to RSA)
- [ ] Argon2 password hashing for key derivation

---

## 👨‍💻 Author

**Dipendra** — Bachelor of Software Engineering, Pokhara University

[@Dipendra367](https://github.com/Dipendra367)

---

## 📄 License

This project is licensed under the **MIT License** — free to use, modify, and distribute.

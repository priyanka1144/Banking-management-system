# Banking System Management 🔐

A secure console-based banking system management application built in C, featuring robust financial operations and audit capabilities.

![CLI Banking System Demo](https://via.placeholder.com/800x400.png?text=CLI+Banking+System+Demo) <!-- Add actual screenshot -->

## Features ✨

- 🔒 **Role-Based Access Control** (Username: `priyanka` | Password: `priyanka123`)
- 📋 Account lifecycle management (Create/Read/Update/Delete)
- 💰 Transaction processing with balance validation
- 🕵️♂️ Comprehensive audit logging system
- 📆 Date validation for DOB (DD-MM-YYYY format)
- 🆔 National ID validation (10-digit format)
- 📜 Transaction history tracking

## Installation 💻

### Kali Linux
```bash
sudo apt update && sudo apt install build-essential -y
git clone https://github.com/yourusername/banking-system.git
cd banking-system
gcc -o banking_system main.c
./banking_system

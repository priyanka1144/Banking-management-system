# Banking System Management 🔐

A secure console-based banking system management application built in C, featuring robust financial operations and audit capabilities.


## Features ✨

- 🔒 **Role-Based Access Control** 
- 📋 **Account lifecycle management** (Create/Read/Update/Delete)
- 💰 **Transaction processing** with balance validation
- 🕵️♂️ **Comprehensive audit logging system**
- 📆 **Date validation** for DOB (DD-MM-YYYY format)
- 🆔 **National ID validation** (10-digit format)
- 📜 **Transaction history tracking**
- 🧑‍💼 Secure login with user authentication

## Installation 💻

### Linux (Ubuntu, Kali Linux, Debian-based distributions)

1. Update your package list and install the build tools:
    ```bash
    sudo apt update && sudo apt install build-essential -y
    ```

2. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/banking-system.git
    ```

3. Navigate into the project directory:
    ```bash
    cd banking-system
    ```

4. Compile the C source code using GCC:
    ```bash
    gcc -o banking_system main.c
    ```

5. Run the banking system:
    ```bash
    ./banking_system
    ```

---

### Windows (Using MinGW or Cygwin)

1. Install MinGW or Cygwin (if not already installed).

   - For **MinGW**: Download from [MinGW](https://osdn.net/projects/mingw/releases/) and install the C compiler.
   - For **Cygwin**: Download from [Cygwin](https://www.cygwin.com/) and install the necessary packages (C Compiler and Make).

2. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/banking-system.git
    ```

3. Open Command Prompt or MinGW terminal and navigate to the project directory:
    ```bash
    cd banking-system
    ```

4. Compile the C source code:
    ```bash
    gcc -o banking_system main.c
    ```

5. Run the banking system:
    ```bash
    ./banking_system.exe
    ```

---

### macOS

1. Install Xcode command line tools (if not already installed):
    ```bash
    xcode-select --install
    ```

2. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/banking-system.git
    ```

3. Navigate into the project directory:
    ```bash
    cd banking-system
    ```

4. Compile the C source code:
    ```bash
    gcc -o banking_system main.c
    ```

5. Run the banking system:
    ```bash
    ./banking_system
    ```

---

## Usage 🛠️

1. After running the program, you’ll be prompted to log in.
2. Once logged in, you will be presented with a banking menu where you can:
   - **Create a new account**
   - **Deposit/Withdraw funds**
   - **Check balance and view transactions**
   - **Update account details (name, address, NID, DOB)**
   - **Delete accounts**
   - **View all accounts and transaction logs**
   - **Access audit logs** for system activity tracking

## Contributing 🤝

Feel free to fork, modify, and contribute to this project. If you encounter any bugs or have ideas for improvements, please open an issue or submit a pull request.

## License 📄

This project is open-source and available under the MIT License.

## Author 🖋️

[Priyanka](https://github.com/priyanka1144)

---

### Demo Screenshots 📸

**Login Menu:**
![Login Menu]([https://via.placeholder.com/400x200.png?text=Login+Menu](https://github.com/priyanka1144/Banking-management-system/blob/main/login.png))

**Banking System Menu:**
![Banking Menu](https://via.placeholder.com/400x200.png?text=Banking+System+Menu)

---

### Project Structure 🏗️


# 🏦 C-Based Banking Management System

A **console-based banking management system** written in pure **C**, designed for basic financial account operations such as account creation, deposits, withdrawals, balance inquiry, and audit tracking. This system is ideal for learning structured programming, file handling, and user input validation.

---

## 🔧 Features

- User login authentication system
- Account creation with personal details (Name, Address, NID, DOB)
- Deposit and Withdrawal transactions
- View account balance and transaction history
- Update account information
- Delete accounts
- View all accounts in the system
- Audit log for tracking system activities

---

## 📂 Project Structure


---

## 💻 Requirements

- C Compiler (`gcc` recommended)
- Standard C Libraries: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `time.h`

---

## 🚀 Installation & Usage

### 🔸 On Linux (Ubuntu/Debian/Fedora)

1. **Clone or Download the Project**
   ```bash
   git clone https://github.com/yourusername/banking_system.git
   cd banking_system
   Compile the Program

bash
Copy
Edit
gcc banking_system.c -o banking_system
Run the Executable

bash
Copy
Edit
./banking_system
🔹 On Windows (Using MinGW or Code::Blocks)
Install GCC (MinGW)

Download from: https://www.mingw-w64.org

Add mingw/bin to your system's PATH

Compile using CMD or PowerShell

sh
Copy
Edit
gcc banking_system.c -o banking_system.exe
Run

sh
Copy
Edit
banking_system.exe
✅ Alternatively, use Code::Blocks:

Open banking_system.c in Code::Blocks

Click Build & Run

🍎 On macOS
Install Xcode Command Line Tools (if not already)

bash
Copy
Edit
xcode-select --install
Compile and Run

bash
Copy
Edit
gcc banking_system.c -o banking_system
./banking_system
🔐 Default Login

Username	Password
priyanka	priyanka123
Only valid credentials allow access to the banking features.

📸 Sample Functionalities
Creating Account

mathematica
Copy
Edit
Enter name: John Doe
Enter address: 123 Main St
Enter NID: 1234567890
Enter Date of Birth (DD-MM-YYYY): 01-01-2000
Enter initial balance: 1000
Depositing Money

yaml
Copy
Edit
Enter account number: 10000
Deposit amount: 500
📋 Notes
Data is stored in memory only; persistent file storage is not implemented in this version.

Maximum number of accounts supported: 100

Maximum transactions per account: 100

Maximum audit entries: 1000

📦 Future Improvements
Add persistent storage (file or database)

Role-based access (admin/user)

GUI version using GTK or Qt

Online mode with network sockets

🤝 Contributing
Pull requests are welcome! For major changes, please open an issue first to discuss what you would like to change.

📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

🙋‍♀️ Author
Arafat Rahman – Web Learning Engineer

Feel free to connect: LinkedIn

yaml
Copy
Edit

---

Let me know if you want to integrate **file I/O**, **persistent data**, or convert this to a **GUI version** using GTK or Qt — I'd be happy to help!









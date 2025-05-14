#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sqlite3.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100
#define MAX_AUDIT_LOGS 1000

sqlite3 *db;

int initializeDatabase() {
    char *errMsg = 0;

    const char *sqlCreateAccounts = 
    "CREATE TABLE IF NOT EXISTS Accounts ("
    "accountNumber INTEGER PRIMARY KEY,"
    "name TEXT NOT NULL,"
    "fatherName TEXT NOT NULL,"
    "motherName TEXT NOT NULL,"
    "phone TEXT NOT NULL,"
    "address TEXT NOT NULL,"
    "nid TEXT UNIQUE NOT NULL,"
    "dob TEXT NOT NULL,"
    "balance REAL NOT NULL);";

    const char *sqlCreateTransactions = 
        "CREATE TABLE IF NOT EXISTS Transactions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "accountNumber INTEGER NOT NULL,"
        "type TEXT NOT NULL,"
        "amount REAL NOT NULL,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY(accountNumber) REFERENCES Accounts(accountNumber));";

    const char *sqlCreateAuditLogs = 
        "CREATE TABLE IF NOT EXISTS AuditLogs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "action TEXT NOT NULL,"
        "accountNumber INTEGER);";

    int rc = sqlite3_exec(db, sqlCreateAccounts, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }

    rc = sqlite3_exec(db, sqlCreateTransactions, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }

    rc = sqlite3_exec(db, sqlCreateAuditLogs, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }

    return 1;
}


int isInteger(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int isFloat(const char *str) {
    int decimalPoint = 0;
    while (*str) {
        if (*str == '.') {
            if (decimalPoint) return 0;
            decimalPoint = 1;
        } else if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int isAlphabeticWithSpaces(const char *str) {
    while (*str) {
        if (!isalpha(*str) && *str != ' ') return 0;
        str++;
    }
    return 1;
}

void getIntegerInput(const char *prompt, char *input, int maxLength) {
    while (1) {
        printf("%s", prompt);
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (isInteger(input)) break;
        printf("Invalid input. Enter digits only.\n");
    }
}

void getFloatInput(const char *prompt, char *input, int maxLength) {
    while (1) {
        printf("%s", prompt);
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (isFloat(input)) break;
        printf("Invalid input. Enter a valid number.\n");
    }
}

void getAlphabeticWithSpacesInput(const char *prompt, char *input, int maxLength) {
    while (1) {
        printf("%s", prompt);
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (isAlphabeticWithSpaces(input)) break;
        printf("Invalid input. Use letters and spaces only.\n");
    }
}

int validateDOB(const char *dob) {
    if (strlen(dob) != 10) return 0;
    if (dob[2] != '-' || dob[5] != '-') return 0;

    char dd[3], mm[3], yyyy[5];
    strncpy(dd, dob, 2);
    dd[2] = '\0';
    strncpy(mm, dob + 3, 2);
    mm[2] = '\0';
    strncpy(yyyy, dob + 6, 4);
    yyyy[4] = '\0';

    if (!isInteger(dd) || !isInteger(mm) || !isInteger(yyyy)) return 0;

    int day = atoi(dd);
    int month = atoi(mm);
    int year = atoi(yyyy);

    if (year < 1900 || year > 2007) return 0;
    if (month < 1 || month > 12) return 0;

    int maxDay;
    switch (month) {
        case 2: maxDay = 28; break;
        case 4: case 6: case 9: case 11: maxDay = 30; break;
        default: maxDay = 31;
    }
    return day >= 1 && day <= maxDay;
}

void getDOBInput(char *input, int maxLength) {
    while (1) {
        printf("Enter Date of Birth (DD-MM-YYYY): ");
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (validateDOB(input)) break;
        printf("Invalid date format or impossible date.\n");
    }
}

void getNIDInput(char *input, int maxLength) {
    while (1) {
        getIntegerInput("Enter NID (10 digits): ", input, maxLength);
        if (strlen(input) == 10) break;
        printf("NID must be exactly 10 digits.\n");
    }
}
void getPhoneInput(char *input, int maxLength) {
    while (1) {
        printf("Enter phone number (11 digits, starts with 01): ");
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) != 11) {
            printf("Phone number must be exactly 11 digits.\n");
            continue;
        }

        if (input[0] != '0' || input[1] != '1') {
            printf("Phone number must start with '01'.\n");
            continue;
        }

        if (!isInteger(input)) {
            printf("Phone number must contain digits only.\n");
            continue;
        }

        break;
    }
}


void addAuditLog(const char *action, unsigned long long accountNumber) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO AuditLogs (action, accountNumber) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, action, -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, accountNumber);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}


void createAccount() {
    char name[50], fatherName[50], motherName[50], phone[15];
    char address[100], nid[20], dob[20], balanceStr[20];
    float balance;


    getAlphabeticWithSpacesInput("Enter name: ", name, 50);
    getAlphabeticWithSpacesInput("Enter father's name: ", fatherName, 50);
    getAlphabeticWithSpacesInput("Enter mother's name: ", motherName, 50);
    getPhoneInput(phone, 15);
    getAlphabeticWithSpacesInput("Enter address: ", address, 100);
    getNIDInput(nid, 20);
    getDOBInput(dob, 20);
    getFloatInput("Enter initial balance: ", balanceStr, 20);


    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Accounts (name, fatherName, motherName, phone, address, nid, dob, balance) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, fatherName, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, motherName, -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 4, phone, -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 5, address, -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 6, nid, -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 7, dob, -1, SQLITE_STATIC);
       sqlite3_bind_double(stmt, 8, balance);


        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("Account created successfully. Account Number: %lld\n", sqlite3_last_insert_rowid(db));
            addAuditLog("Account created", sqlite3_last_insert_rowid(db));
        } else {
            printf("Failed to create account.\n");
        }
        sqlite3_finalize(stmt);
    } else {
        printf("Database error.\n");
    }
}

void updateAccount() {
    long long accountNumber;
    printf("Enter account number: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *checkSql = "SELECT 1 FROM Accounts WHERE accountNumber = ?;";
    int exists = 0;
    
    if (sqlite3_prepare_v2(db, checkSql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_ROW) exists = 1;
        sqlite3_finalize(stmt);
    }
    
    if (!exists) {
        printf("Account not found.\n");
        return;
    }

    char name[50], address[100], dob[20];
    printf("Update name? (y/n): ");
    if (getchar() == 'y') {
        while (getchar() != '\n');
        getAlphabeticWithSpacesInput("New name: ", name, 50);
        const char *sql = "UPDATE Accounts SET name = ? WHERE accountNumber = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
            sqlite3_bind_int64(stmt, 2, accountNumber);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    printf("Update address? (y/n): ");
    if (getchar() == 'y') {
        while (getchar() != '\n');
        getAlphabeticWithSpacesInput("New address: ", address, 100);
        const char *sql = "UPDATE Accounts SET address = ? WHERE accountNumber = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, address, -1, SQLITE_STATIC);
            sqlite3_bind_int64(stmt, 2, accountNumber);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    printf("Update DOB? (y/n): ");
    if (getchar() == 'y') {
        while (getchar() != '\n');
        getDOBInput(dob, 20);
        const char *sql = "UPDATE Accounts SET dob = ? WHERE accountNumber = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, dob, -1, SQLITE_STATIC);
            sqlite3_bind_int64(stmt, 2, accountNumber);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    addAuditLog("Account updated", accountNumber);
    printf("Account updated successfully.\n");
}

void deposit() {
    long long accountNumber;
    float amount;
    printf("Enter account number: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');

    printf("Enter deposit amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Accounts SET balance = balance + ? WHERE accountNumber = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_double(stmt, 1, amount);
        sqlite3_bind_int64(stmt, 2, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            const char *transSql = "INSERT INTO Transactions (accountNumber, type, amount) VALUES (?, 'Deposit', ?);";
            sqlite3_stmt *transStmt;
            if (sqlite3_prepare_v2(db, transSql, -1, &transStmt, 0) == SQLITE_OK) {
                sqlite3_bind_int64(transStmt, 1, accountNumber);
                sqlite3_bind_double(transStmt, 2, amount);
                sqlite3_step(transStmt);
                sqlite3_finalize(transStmt);
            }
            addAuditLog("Deposit", accountNumber);
            printf("Deposit successful.\n");
        } else {
            printf("Account not found.\n");
        }
        sqlite3_finalize(stmt);
    }
}

void withdraw() {
    long long accountNumber;
    float amount;
    printf("Enter account number: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');
    printf("Enter withdrawal amount: ");
    scanf("%f", &amount);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *checkBalance = "SELECT balance FROM Accounts WHERE accountNumber = ?;";
    float currentBalance = 0;
    
    if (sqlite3_prepare_v2(db, checkBalance, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            currentBalance = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (currentBalance < amount) {
        printf("Insufficient funds.\n");
        return;
    }

    const char *sql = "UPDATE Accounts SET balance = balance - ? WHERE accountNumber = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_double(stmt, 1, amount);
        sqlite3_bind_int64(stmt, 2, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
           
            const char *transSql = "INSERT INTO Transactions (accountNumber, type, amount) VALUES (?, 'Withdrawal', ?);";
            sqlite3_stmt *transStmt;
            if (sqlite3_prepare_v2(db, transSql, -1, &transStmt, 0) == SQLITE_OK) {
                sqlite3_bind_int64(transStmt, 1, accountNumber);
                sqlite3_bind_double(transStmt, 2, amount);
                sqlite3_step(transStmt);
                sqlite3_finalize(transStmt);
            }
            addAuditLog("Withdrawal", accountNumber);
            printf("Withdrawal successful.\n");
        } else {
            printf("Account not found.\n");
        }
        sqlite3_finalize(stmt);
    }
}

void checkBalance() {
    long long accountNumber;
    printf("Enter account number: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *sql = "SELECT name, balance FROM Accounts WHERE accountNumber = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("Account Holder: %s\n", sqlite3_column_text(stmt, 0));
            printf("Current Balance: %.2f\n", sqlite3_column_double(stmt, 1));
            addAuditLog("Balance checked", accountNumber);
        } else {
            printf("Account not found.\n");
        }
        sqlite3_finalize(stmt);
    }
}

void viewTransactions() {
    long long accountNumber;
    printf("Enter account number: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *sql = "SELECT timestamp, type, amount FROM Transactions "
                     "WHERE accountNumber = ? ORDER BY timestamp DESC LIMIT 10;";
    
    printf("Last 10 transactions:\n");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, accountNumber);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%s | %s | %.2f\n",
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_double(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
    addAuditLog("Transactions viewed", accountNumber);
}

void deleteAccount() {
    long long accountNumber;
    printf("Enter account number to delete: ");
    scanf("%lld", &accountNumber);
    while (getchar() != '\n');

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM Accounts WHERE accountNumber = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, accountNumber);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("Account deleted successfully.\n");
            addAuditLog("Account deleted", accountNumber);
        } else {
            printf("Account not found.\n");
        }
        sqlite3_finalize(stmt);
    }
}

void displayAllAccounts() {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT accountNumber, name, balance FROM Accounts;";
    
    printf("\nAll Accounts:\n");
    printf("Account Number\tName\t\tBalance\n");
    printf("----------------------------------------\n");
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%-15lld%-20s%.2f\n",
                   sqlite3_column_int64(stmt, 0),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_double(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
    addAuditLog("Viewed all accounts", 0);
}

void viewAudit() {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT timestamp, action, accountNumber FROM AuditLogs ORDER BY timestamp DESC LIMIT 20;";
    
    printf("\nLast 20 Audit Logs:\n");
    printf("Timestamp\t\tAction\t\tAccount Number\n");
    printf("--------------------------------------------------\n");
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%-20s%-20s%lld\n",
                   sqlite3_column_text(stmt, 0),
                   sqlite3_column_text(stmt, 1),
                   sqlite3_column_int64(stmt, 2));
        }
        sqlite3_finalize(stmt);
    }
}

void bankingMenu() {
    int choice;
    do {
        printf("\n--- Banking System Menu ---\n");
        printf("1. Create New Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. View Transactions\n");
        printf("6. Update Account\n");
        printf("7. Delete Account\n");
        printf("8. Display All Accounts\n");
        printf("9. View Audit Logs\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: checkBalance(); break;
            case 5: viewTransactions(); break;
            case 6: updateAccount(); break;
            case 7: deleteAccount(); break;
            case 8: displayAllAccounts(); break;
            case 9: viewAudit(); break;
            case 10: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 10);
}

int main() {
    int rc = sqlite3_open("bank.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    if (!initializeDatabase()) {
        sqlite3_close(db);
        return 1;
    }

    int loginAttempts = 0;
    char username[100], password[100];
    
    while (loginAttempts < 3) {
        printf("\n-------  Login  ------\n");
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';
        
        printf("Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        if (strcmp(username, "priyanka12$") == 0 && strcmp(password, "Priyanka12$$") == 0) {
            printf("Login successful!\n");
            bankingMenu();
            break;
        } else {
            printf("Invalid credentials ! Attempts left: %d\n", 2 - loginAttempts);
            loginAttempts++;
        }
    }

    if (loginAttempts >= 3) {
        printf("Too many failed attempts. Exiting.\n");
    }

    sqlite3_close(db);
    return 0;
}

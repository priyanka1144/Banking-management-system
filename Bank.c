#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100
#define MAX_AUDIT_LOGS 1000

typedef struct {
    char type[20]; 
    float amount;
} Transaction;

typedef struct {
    unsigned long long accountNumber;
    char name[50];
    char address[100];
    char nid[20];      
    char dob[20];      
    float balance;
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
} Account;

typedef struct {
    time_t timestamp;
    char action[100];
    unsigned long long accountNumber;
} AuditLog;

Account accounts[MAX_ACCOUNTS];
AuditLog auditLogs[MAX_AUDIT_LOGS];
int accountCount = 0;
int auditLogCount = 0;
unsigned long long nextAccountNumber = 10000; 

int isInteger(const char *str);
int isFloat(const char *str);
int isAlphabeticWithSpaces(const char *str);
void getIntegerInput(const char *prompt, char *input, int maxLength);
void getFloatInput(const char *prompt, char *input, int maxLength);
void getAlphabeticWithSpacesInput(const char *prompt, char *input, int maxLength);
void getDOBInput(char *input, int maxLength);
void getNIDInput(char *input, int maxLength);
int validateDOB(const char *dob);
void addAuditLog(const char *action, unsigned long long accountNumber);


void bankingMenu();
void createAccount();
void updateAccount();
void deposit();
void withdraw();
void checkBalance();
void viewTransactions();
void deleteAccount();
void displayAllAccounts();
void viewAudit();

int main() {
    int choice;
    while (1) {
        printf("\n--- Login Menu ---\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); 

        if (choice == 1) {
            char username[20], password[20];
            printf("Enter username: ");
            scanf("%19s", username);
            printf("Enter password: ");
            scanf("%19s", password);
            while (getchar() != '\n'); 

            if (strcmp(username, "priyanka") == 0 && strcmp(password, "priyanka123") == 0) {
                printf("Login successful.\n");
                bankingMenu();
            } else {
                printf("Invalid username or password.\n");
            }
        } else if (choice == 2) {
            exit(0);
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

void bankingMenu() {
    int choice;
    int stayInMenu = 1;
    while (stayInMenu) {
        printf("\n--- Banking System ---\n");
        printf("1. Create New Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. View Transactions\n");
        printf("6. Update Account\n");
        printf("7. Delete Account\n");
        printf("8. Display All Accounts\n");
        printf("9. Audit \n");
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
            case 10: stayInMenu = 0; break;
            default: printf("Invalid choice.\n");
        }
    }
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
        scanf("%s", input);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (isInteger(input)) break;
        printf("Invalid input. Enter digits only.\n");
    }
}

void getFloatInput(const char *prompt, char *input, int maxLength) {
    while (1) {
        printf("%s", prompt);
        scanf("%s", input);
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
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

void getDOBInput(char *input, int maxLength) {
    while (1) {
        printf("Enter Date of Birth (DD-MM-YYYY): ");
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (validateDOB(input)) break;
        printf("Invalid date. Check format (DD-MM-YYYY) and validity.\n");
    }
}

void getNIDInput(char *input, int maxLength) {
    while (1) {
        getIntegerInput("Enter NID (10 digits): ", input, maxLength);
        if (strlen(input) == 10) break;
        printf("NID must be 10 digits.\n");
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
    if (day < 1 || day > maxDay) return 0;
    return 1;
}

void addAuditLog(const char *action, unsigned long long accountNumber) {
    if (auditLogCount >= MAX_AUDIT_LOGS) return;
    AuditLog log;
    log.timestamp = time(NULL);
    strncpy(log.action, action, 99);
    log.action[99] = '\0';
    log.accountNumber = accountNumber;
    auditLogs[auditLogCount++] = log;
}


void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Account limit reached.\n");
        return;
    }

    Account newAccount;
    newAccount.transactionCount = 0;
    newAccount.accountNumber = nextAccountNumber++;

    getAlphabeticWithSpacesInput("Enter name: ", newAccount.name, 50);
    getAlphabeticWithSpacesInput("Enter address: ", newAccount.address, 100);
    getNIDInput(newAccount.nid, 20);
    getDOBInput(newAccount.dob, 20);

    char balanceStr[20];
    getFloatInput("Enter initial balance: ", balanceStr, 20);
    newAccount.balance = atof(balanceStr);

    accounts[accountCount++] = newAccount;
    printf("Account created successfully. Your account number is %llu.\n", newAccount.accountNumber);
    addAuditLog("Account created", newAccount.accountNumber);
}

void updateAccount() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n'); 

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            printf("Updating account %llu\n", accNum);
            char choice;

            printf("Update name? (y/n): ");
            scanf(" %c", &choice);
            while (getchar() != '\n');
            if (choice == 'y' || choice == 'Y')
                getAlphabeticWithSpacesInput("New name: ", accounts[i].name, 50);

            printf("Update address? (y/n): ");
            scanf(" %c", &choice);
            while (getchar() != '\n');
            if (choice == 'Y' || choice == 'y')
                getAlphabeticWithSpacesInput("New address: ", accounts[i].address, 100);

            printf("Update NID? (y/n): ");
            scanf(" %c", &choice);
            while (getchar() != '\n');
            if (choice == 'Y' || choice == 'y')
                getNIDInput(accounts[i].nid, 20);

            printf("Update DOB? (y/n): ");
            scanf(" %c", &choice);
            while (getchar() != '\n');
            if (choice == 'Y' || choice == 'y')
                getDOBInput(accounts[i].dob, 20);

            printf("Account updated.\n");
            addAuditLog("Account updated", accNum);
            return;
        }
    }
    printf("Account not found.\n");
}

void deposit() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n');

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            char amountStr[20];
            getFloatInput("Deposit amount: ", amountStr, 20);
            float amount = atof(amountStr);

            accounts[i].balance += amount;
            Transaction t = {"Deposit", amount};
            accounts[i].transactions[accounts[i].transactionCount++] = t;
            printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);
            addAuditLog("Deposit", accNum);
            return;
        }
    }
    printf("Account not found.\n");
}

void withdraw() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n');

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            char amountStr[20];
            getFloatInput("Withdrawal amount: ", amountStr, 20);
            float amount = atof(amountStr);

            if (amount > accounts[i].balance) {
                printf("Insufficient funds.\n");
            } else {
                accounts[i].balance -= amount;
                Transaction t = {"Withdrawal", amount};
                accounts[i].transactions[accounts[i].transactionCount++] = t;
                printf("Withdrawal successful. New balance: %.2f\n", accounts[i].balance);
                addAuditLog("Withdrawal", accNum);
            }
            return;
        }
    }
    printf("Account not found.\n");
}

void checkBalance() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n'); 

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            printf("Name: %s\n", accounts[i].name);
            printf("Address: %s\n", accounts[i].address);
            printf("NID: %s\n", accounts[i].nid);
            printf("DOB: %s\n", accounts[i].dob);
            printf("Balance: %.2f\n", accounts[i].balance);
            addAuditLog("Balance checked", accNum);
            return;
        }
    }
    printf("Account not found.\n");
}

void viewTransactions() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n');

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            if (accounts[i].transactionCount == 0) {
                printf("No transactions.\n");
                return;
            }
            printf("Transaction History:\n");
            for (int j = 0; j < accounts[i].transactionCount; j++) {
                printf("%s: %.2f\n", accounts[i].transactions[j].type,
                                      accounts[i].transactions[j].amount);
            }
            addAuditLog("Transactions viewed", accNum);
            return;
        }
    }
    printf("Account not found.\n");
}

void deleteAccount() {
    unsigned long long accNum;
    printf("Enter account number: ");
    scanf("%llu", &accNum);
    while (getchar() != '\n'); 
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            for (int j = i; j < accountCount-1; j++)
                accounts[j] = accounts[j+1];
            accountCount--;
            printf("Account deleted.\n");
            addAuditLog("Account deleted", accNum);
            return;
        }
    }
    printf("Account not found.\n");
}

void displayAllAccounts() {
    if (accountCount == 0) {
        printf("No accounts.\n");
        return;
    }
    printf("All Accounts:\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Account %llu: %s, %s, NID: %s, DOB: %s, Balance: %.2f\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].address,
               accounts[i].nid, accounts[i].dob, accounts[i].balance);
    }
    addAuditLog("Viewed all accounts", 0);
}

void viewAudit() {
    if (auditLogCount == 0) {
        printf("No audit entries.\n");
        return;
    }
    printf("\n--- Audit  ---\n");
    for (int i = 0; i < auditLogCount; i++) {
        struct tm *timeinfo = localtime(&auditLogs[i].timestamp);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("[%s] Account %llu: %s\n",
               timeStr, auditLogs[i].accountNumber, auditLogs[i].action);
    }
}

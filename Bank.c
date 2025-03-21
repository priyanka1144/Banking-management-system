#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100

typedef struct {
    char type[20]; // "Deposit" or "Withdrawal"
    float amount;
} Transaction;

typedef struct {
    int accountNumber;
    char name[50];
    char address[100]; // Address field
    char nid[20];      // National ID (NID) field
    char dob[20];      // New field: Date of Birth (DOB)
    float balance;
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function to check if an account number already exists
int isAccountNumberUnique(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return 0; // Account number is not unique
        }
    }
    return 1; // Account number is unique
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts. Limit reached.\n");
        return;
    }

    Account newAccount;
    newAccount.transactionCount = 0;

    // Prompt user to enter account number
    printf("Enter account number: ");
    scanf("%d", &newAccount.accountNumber);

    // Check if the account number is unique
    if (!isAccountNumberUnique(newAccount.accountNumber)) {
        printf("Account number already exists. Please try again.\n");
        return;
    }

    printf("Enter name: ");
    scanf("%s", newAccount.name);

    printf("Enter address: ");
    getchar(); // Clear the newline character from the buffer
    fgets(newAccount.address, sizeof(newAccount.address), stdin);
    newAccount.address[strcspn(newAccount.address, "\n")] = '\0'; // Remove trailing newline

    printf("Enter NID (National ID): ");
    scanf("%s", newAccount.nid);

    printf("Enter Date of Birth (DD-MM-YYYY): ");
    scanf("%s", newAccount.dob); // Input DOB in DD-MM-YYYY format

    printf("Enter initial balance: ");
    scanf("%f", &newAccount.balance);

    accounts[accountCount] = newAccount;
    accountCount++;

    printf("Account created successfully! Account Number: %d\n", newAccount.accountNumber);
}

void updateAccount() {
    int accountNumber;
    printf("Enter account number to update: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Current Name: %s\n", accounts[i].name);
            printf("Do you want to update the name? (y/n): ");
            char choice;
            scanf(" %c", &choice); // Note the space before %c to consume leftover newline
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new name: ");
                scanf("%s", accounts[i].name);
            }

            printf("Current Address: %s\n", accounts[i].address);
            printf("Do you want to update the address? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new address: ");
                getchar(); // Clear the newline character from the buffer
                fgets(accounts[i].address, sizeof(accounts[i].address), stdin);
                accounts[i].address[strcspn(accounts[i].address, "\n")] = '\0'; // Remove trailing newline
            }

            printf("Current NID: %s\n", accounts[i].nid);
            printf("Do you want to update the NID? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new NID: ");
                scanf("%s", accounts[i].nid);
            }

            printf("Current Date of Birth: %s\n", accounts[i].dob);
            printf("Do you want to update the Date of Birth? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new Date of Birth (DD-MM-YYYY): ");
                scanf("%s", accounts[i].dob);
            }

            printf("Account updated successfully!\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void deposit() {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);

            accounts[i].balance += amount;

            // Log the transaction
            strcpy(accounts[i].transactions[accounts[i].transactionCount].type, "Deposit");
            accounts[i].transactions[accounts[i].transactionCount].amount = amount;
            accounts[i].transactionCount++;

            printf("Deposit successful! New balance: %.2f\n", accounts[i].balance);
            return;
        }
    }

    printf("Account not found.\n");
}

void withdraw() {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount > accounts[i].balance) {
                printf("Insufficient balance.\n");
            } else {
                accounts[i].balance -= amount;

                // Log the transaction
                strcpy(accounts[i].transactions[accounts[i].transactionCount].type, "Withdrawal");
                accounts[i].transactions[accounts[i].transactionCount].amount = amount;
                accounts[i].transactionCount++;

                printf("Withdrawal successful! New balance: %.2f\n", accounts[i].balance);
            }
            return;
        }
    }

    printf("Account not found.\n");
}

void checkBalance() {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Account Holder: %s\n", accounts[i].name);
            printf("Address: %s\n", accounts[i].address); // Display address
            printf("NID: %s\n", accounts[i].nid);         // Display NID
            printf("Date of Birth: %s\n", accounts[i].dob); // Display DOB
            printf("Balance: %.2f\n", accounts[i].balance);
            return;
        }
    }

    printf("Account not found.\n");
}

void viewTransactions() {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            if (accounts[i].transactionCount == 0) {
                printf("No transactions available for this account.\n");
                return;
            }

            printf("Transaction history for Account Number %d:\n", accounts[i].accountNumber);
            for (int j = 0; j < accounts[i].transactionCount; j++) {
                printf("Type: %s, Amount: %.2f\n",
                       accounts[i].transactions[j].type,
                       accounts[i].transactions[j].amount);
            }
            return;
        }
    }

    printf("Account not found.\n");
}

void deleteAccount() {
    int accountNumber;

    printf("Enter account number to delete: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            for (int j = i; j < accountCount - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            accountCount--;
            printf("Account deleted successfully.\n");
            return;
        }
    }

    printf("Account not found.\n");
}

void displayAllAccounts() {
    if (accountCount == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("List of all accounts:\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Account Number: %d, Name: %s, Address: %s, NID: %s, DOB: %s, Balance: %.2f\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].address, accounts[i].nid, accounts[i].dob, accounts[i].balance);
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n--- Banking System ---\n");
        printf("1. Create New Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. View Transactions\n");
        printf("6. Update Account\n");
        printf("7. Delete Existing Account\n");
        printf("8. Display All Accounts\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                viewTransactions();
                break;
            case 6:
                updateAccount();
                break;
            case 7:
                deleteAccount();
                break;
            case 8:
                displayAllAccounts();
                break;
            case 9:
                printf("Exiting the system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

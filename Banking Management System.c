#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a bank account
struct Account {
    char accountID[20];
    char name[50];
    float balance;
    char password[20];
};

// Structure for admin
struct Admin {
    char id[20];
    char password[20];
};

// File Names
const char *accountFile = "accounts.txt";
const char *adminFile = "admin.txt";

// Function Prototypes
void registerAdmin();
int adminLogin();
void adminMenu();
void userMenu(char *accountID);
void createAccount();
void viewAccountDetails(char *accountID);
void checkBalance(char *accountID);
void depositMoney(char *accountID);
void withdrawMoney(char *accountID);
void transferMoney(char *accountID);
void manageAccount(char *accountID);
void deleteAccount(char *accountID);
void updateAccounts(struct Account *accounts, int numAccounts);

// Function to load accounts into memory
int loadAccounts(struct Account *accounts);

int main() {
    int choice;

    while (1) {
        printf("\n=== Banking Transaction Management System ===\n");
        printf("1. Register Admin\n");
        printf("2. Admin Login\n");
        printf("3. User Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerAdmin();
                break;
            case 2:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 3: {
                char accountID[20];
                printf("\nEnter Account ID: ");
                scanf("%s", accountID);
                userMenu(accountID);
                break;
            }
            case 4:
                printf("Exiting the system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Admin Registration Function
void registerAdmin() {
    struct Admin admin;
    FILE *file = fopen(adminFile, "w");

    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    printf("\nRegister Admin:\n");
    printf("Enter Admin ID: ");
    scanf("%s", admin.id);
    printf("Enter Password: ");
    scanf("%s", admin.password);

    fprintf(file, "%s,%s\n", admin.id, admin.password);
    fclose(file);

    printf("Admin registered successfully!\n");
}

// Admin Login Function
int adminLogin() {
    char id[20], password[20];
    struct Admin admin;

    FILE *file = fopen(adminFile, "r");
    if (!file) {
        printf("No registered admin found. Please register first.\n");
        return 0;
    }

    printf("\nAdmin Login:\n");
    printf("Enter Admin ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    fscanf(file, "%[^,],%s", admin.id, admin.password);
    fclose(file);

    if (strcmp(id, admin.id) == 0 && strcmp(password, admin.password) == 0) {
        printf("Admin login successful.\n");
        return 1;
    } else {
        printf("Invalid Admin credentials!\n");
        return 0;
    }
}

// Admin Menu Function
void adminMenu() {
    int choice;

    while (1) {
        printf("\n=== Admin Menu ===\n");
        printf("1. Create Account\n");
        printf("2. View Account Details\n");
        printf("3. Delete Account\n");
        printf("4. Manage Account\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2: {
                char accountID[20];
                printf("Enter Account ID to view: ");
                scanf("%s", accountID);
                viewAccountDetails(accountID);
                break;
            }
            case 3: {
                char accountID[20];
                printf("Enter Account ID to delete: ");
                scanf("%s", accountID);
                deleteAccount(accountID);
                break;
            }
            case 4: {
                char accountID[20];
                printf("Enter Account ID to manage: ");
                scanf("%s", accountID);
                manageAccount(accountID);
                break;
            }
            case 5:
                printf("Admin logged out.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// User Menu Function
void userMenu(char *accountID) {
    int choice;

    while (1) {
        printf("\n=== User Menu ===\n");
        printf("1. View Account Details\n");
        printf("2. Check Balance\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Transfer Money\n");
        printf("6. Manage Account\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAccountDetails(accountID);
                break;
            case 2:
                checkBalance(accountID);
                break;
            case 3:
                depositMoney(accountID);
                break;
            case 4:
                withdrawMoney(accountID);
                break;
            case 5:
                transferMoney(accountID);
                break;
            case 6:
                manageAccount(accountID);
                break;
            case 7:
                printf("User logged out.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Create Account Function
void createAccount() {
    FILE *file = fopen(accountFile, "a");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;

    printf("\nEnter Account ID: ");
    scanf("%s", account.accountID);
    printf("Enter Name: ");
    scanf(" %[^\n]", account.name);
    account.balance = 0;  // Initial balance is 0
    printf("Enter Password: ");
    scanf("%s", account.password);

    fprintf(file, "%s,%s,%.2f,%s\n", account.accountID, account.name, account.balance, account.password);
    fclose(file);

    printf("Account created successfully!\n");
}

// View Account Details Function
void viewAccountDetails(char *accountID) {
    FILE *file = fopen(accountFile, "r");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;
    int found = 0;

    while (fscanf(file, "%[^,],%[^,],%f,%s\n", account.accountID, account.name, &account.balance, account.password) != EOF) {
        if (strcmp(account.accountID, accountID) == 0) {
            printf("\nAccount Details:\n");
            printf("Account ID: %s\n", account.accountID);
            printf("Name: %s\n", account.name);
            printf("Balance: %.2f\n", account.balance);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Account ID not found.\n");
    }
}

// Check Balance Function
void checkBalance(char *accountID) {
    FILE *file = fopen(accountFile, "r");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;
    int found = 0;

    while (fscanf(file, "%[^,],%[^,],%f,%s\n", account.accountID, account.name, &account.balance, account.password) != EOF) {
        if (strcmp(account.accountID, accountID) == 0) {
            printf("Your Balance: %.2f\n", account.balance);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Account ID not found.\n");
    }
}

// Deposit Money Function
void depositMoney(char *accountID) {
    FILE *file = fopen(accountFile, "r+");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;
    int found = 0;
    float depositAmount;

    while (fscanf(file, "%[^,],%[^,],%f,%s\n", account.accountID, account.name, &account.balance, account.password) != EOF) {
        if (strcmp(account.accountID, accountID) == 0) {
            printf("Enter deposit amount: ");
            scanf("%f", &depositAmount);
            if (depositAmount > 0) {
                account.balance += depositAmount;
                fseek(file, -((long)sizeof(account)), SEEK_CUR);
                fprintf(file, "%s,%s,%.2f,%s\n", account.accountID, account.name, account.balance, account.password);
                printf("Deposit successful!\n");
            } else {
                printf("Invalid deposit amount!\n");
            }
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Account ID not found.\n");
    }
}

// Withdraw Money Function
void withdrawMoney(char *accountID) {
    FILE *file = fopen(accountFile, "r+");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;
    int found = 0;
    float withdrawAmount;

    while (fscanf(file, "%[^,],%[^,],%f,%s\n", account.accountID, account.name, &account.balance, account.password) != EOF) {
        if (strcmp(account.accountID, accountID) == 0) {
            printf("Enter withdrawal amount: ");
            scanf("%f", &withdrawAmount);
            if (withdrawAmount > 0 && withdrawAmount <= account.balance) {
                account.balance -= withdrawAmount;
                fseek(file, -((long)sizeof(account)), SEEK_CUR);
                fprintf(file, "%s,%s,%.2f,%s\n", account.accountID, account.name, account.balance, account.password);
                printf("Withdrawal successful!\n");
            } else {
                printf("Invalid withdrawal amount or insufficient balance!\n");
            }
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Account ID not found.\n");
    }
}

// Transfer Money Function
void transferMoney(char *accountID) {
    FILE *file = fopen(accountFile, "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to open file.\n");
        return;
    }

    struct Account account;
    int foundSender = 0, foundReceiver = 0;
    struct Account senderAccount, receiverAccount;
    char targetAccountID[20];
    float transferAmount;

    printf("Enter target account ID: ");
    scanf("%s", targetAccountID);
    printf("Enter transfer amount: ");
    scanf("%f", &transferAmount);

    while (fscanf(file, "%[^,],%[^,],%f,%s\n", account.accountID, account.name, &account.balance, account.password) != EOF) {
        if (strcmp(account.accountID, accountID) == 0) {
            senderAccount = account;
            foundSender = 1;
        }
        if (strcmp(account.accountID, targetAccountID) == 0) {
            receiverAccount = account;
            foundReceiver = 1;
        }
        fprintf(tempFile, "%s,%s,%.2f,%s\n", account.accountID, account.name, account.balance, account.password);
    }

    fclose(file);
    fclose(tempFile);

    if (!foundSender) {
        printf("Sender account not found.\n");
        return;
    }
    if (!foundReceiver) {
        printf("Receiver account not found.\n");
        return;
    }

    if (senderAccount.balance >= transferAmount) {
        senderAccount.balance -= transferAmount;
        receiverAccount.balance += transferAmount;
        updateAccounts(&senderAccount, 1);
        updateAccounts(&receiverAccount, 1);
        printf("Transfer successful!\n");
    } else {
        printf("Insufficient balance in sender account.\n");
    }
}

// Update Accounts Function
void updateAccounts(struct Account *accounts, int numAccounts) {
    FILE *file = fopen(accountFile, "w");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%.2f,%s\n", accounts[i].accountID, accounts[i].name, accounts[i].balance, accounts[i].password);
    }

    fclose(file);
}

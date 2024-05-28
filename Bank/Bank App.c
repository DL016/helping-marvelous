#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "accounts.dat"
#define MAX_NAME_LENGTH 100

typedef struct {
    int account_number;
    char name[MAX_NAME_LENGTH];
    double balance;
} Account;

// Function prototypes
void create_account();
void display_accounts();
void deposit_money();
void withdraw_money();
void save_account(Account account);
void load_accounts();

int main() {
    int choice;

    // Load existing accounts from file
    load_accounts();

    while (1) {
        printf("\Welcome to FCMB dear customer\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                display_accounts();
                break;
            case 3:
                deposit_money();
                break;
            case 4:
                withdraw_money();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void create_account() {
    Account new_account;
    FILE *file = fopen(FILENAME, "ab");

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &new_account.account_number);
    printf("Enter name: ");
    scanf(" %[^\n]", new_account.name);  // To read a string with spaces
    printf("Enter initial balance: ");
    scanf("%lf", &new_account.balance);

    fwrite(&new_account, sizeof(Account), 1, file);
    fclose(file);

    printf("Account created successfully.\n");
}

void display_accounts() {
    Account account;
    FILE *file = fopen(FILENAME, "rb");

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nAccount Details:\n");
    printf("Account Number\tName\t\tBalance\n");
    while (fread(&account, sizeof(Account), 1, file)) {
        printf("%d\t\t%s\t\t%.2f\n", account.account_number, account.name, account.balance);
    }

    fclose(file);
}

void deposit_money() {
    int account_number;
    double amount;
    Account account;
    FILE *file = fopen(FILENAME, "rb+");
    int found = 0;

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &account_number);
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    while (fread(&account, sizeof(Account), 1, file)) {
        if (account.account_number == account_number) {
            account.balance += amount;
            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&account, sizeof(Account), 1, file);
            found = 1;
            printf("Deposit successful.\n");
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void withdraw_money() {
    int account_number;
    double amount;
    Account account;
    FILE *file = fopen(FILENAME, "rb+");
    int found = 0;

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &account_number);
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    while (fread(&account, sizeof(Account), 1, file)) {
        if (account.account_number == account_number) {
            if (account.balance >= amount) {
                account.balance -= amount;
                fseek(file, -sizeof(Account), SEEK_CUR);
                fwrite(&account, sizeof(Account), 1, file);
                found = 1;
                printf("Withdrawal successful.\n");
            } else {
                printf("Insufficient balance.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void load_accounts() {
    // This function ensures the file exists
    FILE *file = fopen(FILENAME, "ab");
    if (file) {
        fclose(file);
    }
}

#include <stdio.h>
#include <string.h>

#define DATA_FILE "accounts.dat"
#define NAME_LENGTH 60

typedef struct {
    long account_number;
    char holder_name[NAME_LENGTH];
    double balance;
} Account;

void clear_input(void)
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

void read_line(char *text, size_t size)
{
    if (fgets(text, (int)size, stdin) == NULL) {
        text[0] = '\0';
        return;
    }
    text[strcspn(text, "\n")] = '\0';
}

int find_account(long account_number, Account *account, long *position)
{
    FILE *file = fopen(DATA_FILE, "rb");
    long current_position;

    if (file == NULL) {
        return 0;
    }

    while ((current_position = ftell(file)) >= 0 && fread(account, sizeof(*account), 1, file) == 1) {
        if (account->account_number == account_number) {
            if (position != NULL) {
                *position = current_position;
            }
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void create_account(void)
{
    Account account;
    FILE *file;

    printf("Account number: ");
    if (scanf("%ld", &account.account_number) != 1 || account.account_number < 1) {
        clear_input();
        printf("Invalid account number.\n");
        return;
    }
    clear_input();

    if (find_account(account.account_number, &account, NULL)) {
        printf("An account with that number already exists.\n");
        return;
    }

    printf("Account holder name: ");
    read_line(account.holder_name, sizeof(account.holder_name));
    printf("Opening deposit: ");
    if (scanf("%lf", &account.balance) != 1 || account.balance < 0.0) {
        clear_input();
        printf("Opening deposit cannot be negative.\n");
        return;
    }
    clear_input();

    file = fopen(DATA_FILE, "ab");
    if (file == NULL || fwrite(&account, sizeof(account), 1, file) != 1) {
        printf("Unable to save the account.\n");
        if (file != NULL) {
            fclose(file);
        }
        return;
    }
    fclose(file);
    printf("Account created successfully.\n");
}

int update_balance(long account_number, double amount, int deposit)
{
    Account account;
    long position;
    FILE *file;

    if (!find_account(account_number, &account, &position)) {
        printf("Account not found.\n");
        return 0;
    }

    if (deposit) {
        account.balance += amount;
    } else {
        if (amount > account.balance) {
            printf("Insufficient balance.\n");
            return 0;
        }
        account.balance -= amount;
    }

    file = fopen(DATA_FILE, "r+b");
    if (file == NULL || fseek(file, position, SEEK_SET) != 0 || fwrite(&account, sizeof(account), 1, file) != 1) {
        printf("Unable to update the account.\n");
        if (file != NULL) {
            fclose(file);
        }
        return 0;
    }
    fclose(file);
    printf("Transaction successful. New balance: %.2f\n", account.balance);
    return 1;
}

void deposit(void)
{
    long account_number;
    double amount;

    printf("Account number: ");
    if (scanf("%ld", &account_number) != 1) {
        clear_input();
        printf("Invalid account number.\n");
        return;
    }
    printf("Deposit amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0.0) {
        clear_input();
        printf("Deposit must be greater than zero.\n");
        return;
    }
    clear_input();
    update_balance(account_number, amount, 1);
}

void withdraw(void)
{
    long account_number;
    double amount;

    printf("Account number: ");
    if (scanf("%ld", &account_number) != 1) {
        clear_input();
        printf("Invalid account number.\n");
        return;
    }
    printf("Withdrawal amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0.0) {
        clear_input();
        printf("Withdrawal must be greater than zero.\n");
        return;
    }
    clear_input();
    update_balance(account_number, amount, 0);
}

void balance_enquiry(void)
{
    long account_number;
    Account account;

    printf("Account number: ");
    if (scanf("%ld", &account_number) != 1) {
        clear_input();
        printf("Invalid account number.\n");
        return;
    }
    clear_input();

    if (find_account(account_number, &account, NULL)) {
        printf("Account holder: %s\nBalance: %.2f\n", account.holder_name, account.balance);
    } else {
        printf("Account not found.\n");
    }
}

void display_accounts(void)
{
    Account account;
    FILE *file = fopen(DATA_FILE, "rb");

    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("%-16s %-30s %s\n", "Account", "Holder", "Balance");
    printf("---------------------------------------------------------------\n");
    while (fread(&account, sizeof(account), 1, file) == 1) {
        printf("%-16ld %-30s %.2f\n", account.account_number, account.holder_name, account.balance);
    }
    fclose(file);
}

int main(void)
{
    int choice;

    do {
        printf("\n=== Banking System ===\n");
        printf("1. Create account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Balance enquiry\n");
        printf("5. Display accounts\n");
        printf("0. Exit\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input();
            printf("Please enter a number.\n");
            continue;
        }
        clear_input();

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: balance_enquiry(); break;
            case 5: display_accounts(); break;
            case 0: printf("Goodbye.\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 0);

    return 0;
}

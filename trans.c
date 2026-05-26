#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[15];
    double balance;
};

unsigned int enterChoice(void);
void addAccount(FILE *fPtr);
void viewAccount(FILE *fPtr);
void updateAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void searchAccount(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void addInterest(FILE *fPtr);
void bankSummary(FILE *fPtr);
void exportTextFile(FILE *fPtr);
void logTransaction(char message[]);

int main()
{
    FILE *cfPtr;
    unsigned int choice;
    char password[20];

    printf("===== BANK MANAGEMENT SYSTEM =====\n");
    printf("Enter Admin Password: ");
    scanf("%s", password);

    if(strcmp(password, "admin123") != 0)
    {
        printf("Access Denied!\n");
        return 0;
    }

    if((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if(cfPtr == NULL)
        {
            printf("File cannot be created.\n");
            return 1;
        }

        struct clientData blank = {0, "", "", 0.0};

        for(int i = 0; i < 100; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }
    }

    while((choice = enterChoice()) != 13)
    {
        switch(choice)
        {
            case 1:
                exportTextFile(cfPtr);
                break;

            case 2:
                updateAccount(cfPtr);
                break;

            case 3:
                addAccount(cfPtr);
                break;

            case 4:
                deleteAccount(cfPtr);
                break;

            case 5:
                viewAccount(cfPtr);
                break;

            case 6:
                depositMoney(cfPtr);
                break;

            case 7:
                withdrawMoney(cfPtr);
                break;

            case 8:
                transferMoney(cfPtr);
                break;

            case 9:
                searchAccount(cfPtr);
                break;

            case 10:
                displayAllAccounts(cfPtr);
                break;

            case 11:
                addInterest(cfPtr);
                break;

            case 12:
                bankSummary(cfPtr);
                break;

            default:
                printf("Invalid Choice\n");
        }
    }

    fclose(cfPtr);
    printf("Thank You!\n");

    return 0;
}

unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== MENU ==========\n");
    printf("1. Export Accounts to Text File\n");
    printf("2. Update Account Balance\n");
    printf("3. Add New Account\n");
    printf("4. Delete Account\n");
    printf("5. View Account\n");
    printf("6. Deposit Money\n");
    printf("7. Withdraw Money\n");
    printf("8. Transfer Money\n");
    printf("9. Search Account\n");
    printf("10. Display All Accounts\n");
    printf("11. Add Interest\n");
    printf("12. Bank Summary\n");
    printf("13. Exit\n");
    printf("Enter Choice: ");

    scanf("%u", &choice);

    return choice;
}

void addAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter Account Number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter Last Name: ");
    scanf("%s", client.lastName);

    printf("Enter First Name: ");
    scanf("%s", client.firstName);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account Added Successfully.\n");
}

void viewAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    printf("\nAccount Number : %u\n", client.acctNum);
    printf("Name           : %s %s\n",
           client.firstName,
           client.lastName);
    printf("Balance        : %.2lf\n",
           client.balance);
}

void updateAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    printf("Current Balance: %.2lf\n", client.balance);

    printf("Enter Amount (+Deposit / -Withdraw): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Balance Updated.\n");
}

void depositMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;
    char msg[100];

    printf("Enter Account Number: ");
    scanf("%u", &account);

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    sprintf(msg, "Deposit %.2lf to Account %u", amount, account);
    logTransaction(msg);

    printf("Deposit Successful.\n");
}

void withdrawMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;
    char msg[100];

    printf("Enter Account Number: ");
    scanf("%u", &account);

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    if(client.balance < amount)
    {
        printf("Insufficient Balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    sprintf(msg, "Withdraw %.2lf from Account %u", amount, account);
    logTransaction(msg);

    printf("Withdrawal Successful.\n");
}

void transferMoney(FILE *fPtr)
{
    struct clientData sender, receiver;
    unsigned int fromAcc, toAcc;
    double amount;
    char msg[100];

    printf("From Account: ");
    scanf("%u", &fromAcc);

    printf("To Account: ");
    scanf("%u", &toAcc);

    printf("Amount: ");
    scanf("%lf", &amount);

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&receiver, sizeof(struct clientData), 1, fPtr);

    if(sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid Account.\n");
        return;
    }

    if(sender.balance < amount)
    {
        printf("Insufficient Balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&receiver, sizeof(struct clientData), 1, fPtr);

    sprintf(msg,
            "Transfer %.2lf from %u to %u",
            amount,
            fromAcc,
            toAcc);
    logTransaction(msg);

    printf("Transfer Successful.\n");
}

void deleteAccount(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};
    struct clientData client;
    unsigned int account;

    printf("Enter Account Number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if(client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account Deleted.\n");
}

void searchAccount(FILE *fPtr)
{
    struct clientData client;
    char name[15];
    int found = 0;

    printf("Enter Last Name: ");
    scanf("%s", name);

    rewind(fPtr);

    while(fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if(client.acctNum != 0 &&
           strcmp(client.lastName, name) == 0)
        {
            printf("%u %s %s %.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);

            found = 1;
        }
    }

    if(!found)
        printf("No Matching Record Found.\n");
}

void displayAllAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-10s%-15s%-15s%-10s\n",
           "Acc No",
           "Last Name",
           "First Name",
           "Balance");

    while(fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if(client.acctNum != 0)
        {
            printf("%-10u%-15s%-15s%.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

void addInterest(FILE *fPtr)
{
    struct clientData client;
    double rate = 5.0;

    rewind(fPtr);

    while(fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if(client.acctNum != 0)
        {
            client.balance += (client.balance * rate / 100);

            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
            fseek(fPtr, 0, SEEK_CUR);
        }
    }

    printf("5%% Interest Added.\n");
}

void bankSummary(FILE *fPtr)
{
    struct clientData client;

    int totalAccounts = 0;
    double totalBalance = 0;

    rewind(fPtr);

    while(fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if(client.acctNum != 0)
        {
            totalAccounts++;
            totalBalance += client.balance;
        }
    }

    printf("\n===== BANK SUMMARY =====\n");
    printf("Total Accounts : %d\n", totalAccounts);
    printf("Total Balance  : %.2lf\n", totalBalance);
}

void exportTextFile(FILE *fPtr)
{
    FILE *txtPtr;
    struct clientData client;

    txtPtr = fopen("accounts.txt", "w");

    if(txtPtr == NULL)
    {
        printf("Unable to create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(txtPtr,
            "%-10s%-15s%-15s%-10s\n",
            "AccNo",
            "LastName",
            "FirstName",
            "Balance");

    while(fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if(client.acctNum != 0)
        {
            fprintf(txtPtr,
                    "%-10u%-15s%-15s%.2lf\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(txtPtr);

    printf("accounts.txt Created Successfully.\n");
}

void logTransaction(char message[])
{
    FILE *logFile;

    logFile = fopen("transactions.txt", "a");

    if(logFile != NULL)
    {
        fprintf(logFile, "%s\n", message);
        fclose(logFile);
    }
}

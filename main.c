#include <stdio.h>
#include <stdlib.h>

/* Authentication & Core */
void login();
void createFile();

/* Database Initializers */
void createProductsFile();
void createTransactionsFile();

/* Utility */
void clearScreen();
void pressEnter();
void logAction(const char *userId, const char *action);

/* Role Dashboard Forward Declarations */
void adminDashboard(char userId[], char userName[]);
void storeManagerDashboard(char userId[], char userName[]);
void salesStaffDashboard(char userId[], char userName[]);

void roleMenu(char rolePrefix, char userId[], char userName[])
{
    if (rolePrefix == 'A')
    {
        adminDashboard(userId, userName);
    }
    else if (rolePrefix == 'M')
    {
        storeManagerDashboard(userId, userName);
    }
    else if (rolePrefix == 'S')
    {
        salesStaffDashboard(userId, userName);
    }
    else
    {
        printf("\nUnknown User Role!\n");
        pressEnter();
    }
}

int main()
{
    // Initialize active data files
    createFile();
    createProductsFile();
    createTransactionsFile();

    int choice;

    while(1)
    {
        clearScreen();

        printf("\n\n=================================================\n");
        printf("       STOCK INVENTORY MANAGEMENT SYSTEM\n");
        printf("=================================================\n\n");

        printf("1. Login\n");
        printf("0. Exit\n");

        printf("\n=================================================\n");
        printf("Enter Choice : ");

        if(scanf("%d",&choice)!=1)
        {
            while(getchar()!='\n');

            printf("\nInvalid Input!\n");

            pressEnter();
            continue;
        }

        switch(choice)
        {
            case 1:
                login();
                break;

            case 0:
                clearScreen();

                logAction("SYSTEM", "System shut down safely");

                printf("=========================================\n");
                printf("   Thank You For Using The System\n");
                printf("=========================================\n\n");

                return 0;

            default:
                printf("\nInvalid Choice!\n");
                pressEnter();
        }
    }

    return 0;
}
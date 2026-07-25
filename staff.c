#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen();
void pressEnter();
void delay();
void logAction(const char *userId, const char *action);

void processSale(const char *userId);
void viewProducts();
void searchProducts();
void checkLowStockAlerts();
void requestRestock(const char *userId);
void viewTransactions();
void viewAndEditProfile(const char *userId);

void salesStaffDashboard(char userId[], char userName[])
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("=================================================\n");
        printf("              SALES STAFF DASHBOARD\n");
        printf("=================================================\n");
        printf(" Logged in as: %s (%s)\n", userName, userId);
        printf("=================================================\n\n");

        printf("1. Process Sales Transaction\n");
        printf("2. View Products\n");
        printf("3. Search Products\n");
        printf("4. Low Stock Alerts\n");
        printf("5. Request Restock\n");
        printf("6. View Transaction History\n");
        printf("7. My Profile (View & Edit)\n");
        printf("8. Logout\n");
        printf("\n=================================================\n");
        printf("Enter Choice : ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("\nInvalid Input!\n");
            pressEnter();
            continue;
        }

        switch (choice)
        {
            case 1:
                processSale(userId);
                break;
            case 2:
                viewProducts();
                break;
            case 3:
                searchProducts();
                break;
            case 4:
                checkLowStockAlerts();
                break;
            case 5:
                requestRestock(userId);
                break;
            case 6:
                viewTransactions();
                break;
            case 7:
                viewAndEditProfile(userId);
                break;
            case 8:
                logAction(userId, "Logged out");
                printf("\nLogging out...\n");
                delay();
                return;
            default:
                printf("\nInvalid Choice!\n");
                pressEnter();
        }
    }
}

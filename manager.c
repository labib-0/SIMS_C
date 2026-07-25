#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen();
void pressEnter();
void delay();
void logAction(const char *userId, const char *action);

void viewProducts();
void searchProducts();
void updateStock(const char *userId);
void checkLowStockAlerts();
void requestRestock(const char *userId);
void processRestockRequest(const char *userId);

void viewTransactions();
void generateInventoryReport(const char *userId);
void generateSalesReport(const char *userId);
void generatePerformanceReport(const char *userId);
void viewAndEditProfile(const char *userId);

void storeManagerDashboard(char userId[], char userName[])
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("=================================================\n");
        printf("            STORE MANAGER DASHBOARD\n");
        printf("=================================================\n");
        printf(" Logged in as: %s (%s)\n", userName, userId);
        printf("=================================================\n\n");

        printf("1. View Products\n");
        printf("2. Search Products\n");
        printf("3. Update Stock\n");
        printf("4. Low Stock & Restock Alerts\n");
        printf("5. Request Restock\n");
        printf("6. Manage Restock Requests\n");
        printf("7. View Transaction History\n");
        printf("8. Reports System\n");
        printf("9. My Profile (View & Edit)\n");
        printf("10. Logout\n");
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
                viewProducts();
                break;
            case 2:
                searchProducts();
                break;
            case 3:
                updateStock(userId);
                break;
            case 4:
                checkLowStockAlerts();
                break;
            case 5:
                requestRestock(userId);
                break;
            case 6:
                {
                    int subChoice;
                    clearScreen();
                    printf("=========================================\n");
                    printf("           RESTOCK MANAGEMENT\n");
                    printf("=========================================\n\n");
                    printf("1. View Low Stock & Restock Alerts\n");
                    printf("2. Process / Fulfill Restock Request\n");
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) checkLowStockAlerts();
                        else if (subChoice == 2) processRestockRequest(userId);
                    }
                }
                break;
            case 7:
                viewTransactions();
                break;
            case 8:
                {
                    int subChoice;
                    clearScreen();
                    printf("=========================================\n");
                    printf("             REPORTS SYSTEM\n");
                    printf("=========================================\n\n");
                    printf("1. Inventory Report\n");
                    printf("2. Sales Summary Report\n");
                    printf("3. Product Performance Report\n");
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) generateInventoryReport(userId);
                        else if (subChoice == 2) generateSalesReport(userId);
                        else if (subChoice == 3) generatePerformanceReport(userId);
                    }
                }
                break;
            case 9:
                viewAndEditProfile(userId);
                break;
            case 10:
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

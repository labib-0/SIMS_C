#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearScreen();
void pressEnter();
void delay();
void logAction(const char *userId, const char *action);

void addProduct(const char *userId);
void updateProduct(const char *userId);
void deleteProduct(const char *userId);
void viewProducts();
void searchProducts();
void requestRestock(const char *userId);

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

        printf("1. Product Management\n");
        printf("2. Request Restock\n");
        printf("3. View Transaction History\n");
        printf("4. Reports System\n");
        printf("5. My Profile (View & Edit)\n");
        printf("0. Logout\n");
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
                {
                    int subChoice;
                    clearScreen();
                    printf("=========================================\n");
                    printf("           PRODUCT MANAGEMENT\n");
                    printf("=========================================\n\n");
                    printf("1. Add New Product\n");
                    printf("2. Update Product Details\n");
                    printf("3. Delete Product\n");
                    printf("4. View All Products\n");
                    printf("5. Search Products\n");
                    printf("0. Back\n");
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) addProduct(userId);
                        else if (subChoice == 2) updateProduct(userId);
                        else if (subChoice == 3) deleteProduct(userId);
                        else if (subChoice == 4) viewProducts();
                        else if (subChoice == 5) searchProducts();
                        else if (subChoice == 0) break;
                    }
                }
                break;
            case 2:
                requestRestock(userId);
                break;
            case 3:
                viewTransactions();
                break;
            case 4:
                {
                    int subChoice;
                    clearScreen();
                    printf("=========================================\n");
                    printf("             REPORTS SYSTEM\n");
                    printf("=========================================\n\n");
                    printf("1. Inventory Report\n");
                    printf("2. Sales Summary Report\n");
                    printf("3. Product Performance Report\n");
                    printf("0. Back\n");
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) generateInventoryReport(userId);
                        else if (subChoice == 2) generateSalesReport(userId);
                        else if (subChoice == 3) generatePerformanceReport(userId);
                        else if (subChoice == 0) break;
                    }
                }
                break;
            case 5:
                viewAndEditProfile(userId);
                break;
            case 0:
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

#include <stdio.h>
#include <stdlib.h>

/* Utility & Core Prototypes */
void clearScreen();
void pressEnter();
void delay();
void logAction(const char *userId, const char *action);
void createFile();
void login();
void viewAndEditProfile(const char *userId);

/* Database Initializers */
void createProductsFile();
void createTransactionsFile();

/* Admin / User Management Features */
void userManagementMenu(const char *userId);
void backupData(const char *userId);
void restoreData(const char *userId);
void viewAuditLog(const char *userId);

/* Inventory Features */
void addProduct(const char *userId);
void updateProduct(const char *userId);
void deleteProduct(const char *userId);
void viewProducts();
void searchProducts();
void stockManagement(const char *userId);
void requestRestock(const char *userId);

/* Sales & Transactions */
void processSale(const char *userId);
void viewTransactions();

/* Reports System */
void generateInventoryReport(const char *userId);
void generateSalesReport(const char *userId);
void generatePerformanceReport(const char *userId);

/* Role Dashboard Functions */
void adminDashboard(char userId[], char userName[])
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("=================================================\n");
        printf("                ADMIN DASHBOARD\n");
        printf("=================================================\n");
        printf(" Logged in as: %s (%s)\n", userName, userId);
        printf("=================================================\n\n");

        printf("1. User Management\n");
        printf("2. Product Management\n");
        printf("3. Stock Management\n");
        printf("4. Process Sales Transaction\n");
        printf("5. View Transaction History\n");
        printf("6. Reports System\n");
        printf("7. Data Management\n");
        printf("8. View System Audit Logs\n");
        printf("9. My Profile (View & Edit)\n");
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
                userManagementMenu(userId);
                break;
            case 2:
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
            case 3:
                stockManagement(userId);
                break;
            case 4:
                processSale(userId);
                break;
            case 5:
                viewTransactions();
                break;
            case 6:
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
            case 7:
                {
                    int subChoice;
                    clearScreen();
                    printf("=========================================\n");
                    printf("            DATA MANAGEMENT\n");
                    printf("=========================================\n\n");
                    printf("1. Backup System Data\n");
                    printf("2. Restore System Data\n");
                    printf("0. Back\n");
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) backupData(userId);
                        else if (subChoice == 2) restoreData(userId);
                        else if (subChoice == 0) break;
                    }
                }
                break;
            case 8:
                viewAuditLog(userId);
                break;
            case 9:
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
        printf("4. Request Restock\n");
        printf("5. View Transaction History\n");
        printf("6. My Profile (View & Edit)\n");
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
                processSale(userId);
                break;
            case 2:
                viewProducts();
                break;
            case 3:
                searchProducts();
                break;
            case 4:
                requestRestock(userId);
                break;
            case 5:
                viewTransactions();
                break;
            case 6:
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
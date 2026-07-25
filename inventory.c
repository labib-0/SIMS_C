#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Product
{
    char id[10];
    char name[50];
    char category[30];
    double price;
    int quantity;
    int minStock;
    int restockQty;
};

void clearScreen();
void pressEnter();
void skipHeader(FILE *);
void logAction(const char *userId, const char *action);

int binarySearchProduct(struct Product products[], int n, const char *targetId);
int loadAndSortProducts(struct Product products[], int maxProducts);
void saveProductsArray(struct Product products[], int n);

void createProductsFile()
{
    FILE *fp = fopen("products.csv", "r");
    if (fp == NULL)
    {
        fp = fopen("products.csv", "w");
        if(fp != NULL)
        {
            fprintf(fp, "ProductID,Name,Category,Price,Quantity,MinStock,RestockQty\n");
            fclose(fp);
        }
    }
    else
    {
        fclose(fp);
    }
}

int selectProductHelper(char selectedProdId[], const char *actionTitle)
{
    struct Product products[1000];

    while (1)
    {
        int count = loadAndSortProducts(products, 1000);

        clearScreen();
        printf("=========================================\n");
        printf("         %s\n", actionTitle);
        printf("=========================================\n\n");
        printf("1. Search Product by Name\n");
        printf("2. View All Products\n");
        printf("3. Enter Product ID\n");
        printf("4. Back\n");
        printf("\nEnter Choice : ");

        int choice;
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("\nInvalid Input!\n");
            pressEnter();
            continue;
        }
        while (getchar() != '\n');

        if (choice == 4) return 0;

        if (choice == 1)
        {
            while (1)
            {
                clearScreen();
                printf("=========================================\n");
                printf("         SEARCH PRODUCT BY NAME\n");
                printf("=========================================\n\n");
                printf("Enter Product Name to Search (or '0' to cancel) : ");

                char query[50];
                fgets(query, sizeof(query), stdin);
                query[strcspn(query, "\n")] = '\0';

                if (strcmp(query, "0") == 0) break;
                if (strlen(query) == 0) continue;

                int searchCount = 0;
                printf("\n%-10s %-25s %-15s %-10s %-8s %-10s\n",
                       "ID", "Product Name", "Category", "Price($)", "Qty", "ReorderQty");
                printf("--------------------------------------------------------------------------------\n");

                for (int i = 0; i < count; i++)
                {
                    if (strcasestr(products[i].name, query) != NULL)
                    {
                        searchCount++;
                        printf("%-10s %-25s %-15s %-10.2f %-8d %-10d\n",
                               products[i].id, products[i].name, products[i].category,
                               products[i].price, products[i].quantity, products[i].restockQty);
                    }
                }
                printf("--------------------------------------------------------------------------------\n");

                if (searchCount == 0)
                {
                    printf("\nNothing found matching '%s'! Search again.\n", query);
                    pressEnter();
                    continue;
                }
                else
                {
                    printf("\nEnter Product ID (or '0' to cancel) : ");
                    scanf("%9s", selectedProdId);
                    while (getchar() != '\n');

                    if (strcmp(selectedProdId, "0") == 0)
                    {
                        selectedProdId[0] = '\0';
                        break;
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else if (choice == 2)
        {
            clearScreen();
            printf("================================================================================\n");
            printf("                              ALL PRODUCTS LIST\n");
            printf("================================================================================\n\n");

            printf("%-10s %-25s %-15s %-10s %-8s %-10s\n",
                   "ID", "Product Name", "Category", "Price($)", "Qty", "ReorderQty");
            printf("--------------------------------------------------------------------------------\n");

            for (int i = 0; i < count; i++)
            {
                printf("%-10s %-25s %-15s %-10.2f %-8d %-10d\n",
                       products[i].id, products[i].name, products[i].category,
                       products[i].price, products[i].quantity, products[i].restockQty);
            }
            printf("--------------------------------------------------------------------------------\n");
            printf("Total Products : %d\n\n", count);

            printf("Enter Product ID (or '0' to cancel) : ");
            scanf("%9s", selectedProdId);
            while (getchar() != '\n');

            if (strcmp(selectedProdId, "0") == 0)
            {
                selectedProdId[0] = '\0';
                continue;
            }
            return 1;
        }
        else if (choice == 3)
        {
            printf("\nEnter Product ID : ");
            scanf("%9s", selectedProdId);
            while (getchar() != '\n');
            return 1;
        }
        else
        {
            printf("\nInvalid Choice!\n");
            pressEnter();
        }
    }
}

void generateProductID(char id[])
{
    struct Product products[1000];
    int count = loadAndSortProducts(products, 1000);

    int max = 0;
    for (int i = 0; i < count; i++)
    {
        if (products[i].id[0] == 'P')
        {
            int num = atoi(products[i].id + 1);
            if (num > max) max = num;
        }
    }
    sprintf(id, "P%03d", max + 1);
}

void addProduct(const char *userId)
{
    createProductsFile();
    struct Product p;
    p.restockQty = 0;

    clearScreen();
    printf("=========================================\n");
    printf("               ADD PRODUCT\n");
    printf("=========================================\n\n");

    while(getchar() != '\n');

    printf("Product Name              : ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Category                  : ");
    fgets(p.category, sizeof(p.category), stdin);
    p.category[strcspn(p.category, "\n")] = '\0';

    printf("Unit Price ($)            : ");
    while(scanf("%lf", &p.price) != 1 || p.price < 0)
    {
        while(getchar() != '\n');
        printf("Invalid Price! Re-enter   : ");
    }

    printf("Initial Quantity          : ");
    while(scanf("%d", &p.quantity) != 1 || p.quantity < 0)
    {
        while(getchar() != '\n');
        printf("Invalid Quantity! Re-enter: ");
    }

    printf("Minimum Stock Level       : ");
    while(scanf("%d", &p.minStock) != 1 || p.minStock < 0)
    {
        while(getchar() != '\n');
        printf("Invalid Min Level! Re-enter: ");
    }

    generateProductID(p.id);

    FILE *fp = fopen("products.csv", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s,%s,%s,%.2f,%d,%d,%d\n", p.id, p.name, p.category, p.price, p.quantity, p.minStock, p.restockQty);
        fclose(fp);

        char logMsg[100];
        sprintf(logMsg, "Added Product %s (%s)", p.id, p.name);
        logAction(userId, logMsg);

        clearScreen();
        printf("=========================================\n");
        printf("       PRODUCT ADDED SUCCESSFULLY\n");
        printf("=========================================\n\n");
        printf("Product ID   : %s\n", p.id);
        printf("Name         : %s\n", p.name);
        printf("Category     : %s\n", p.category);
        printf("Price        : $%.2f\n", p.price);
        printf("Quantity     : %d\n", p.quantity);
        printf("Min Stock    : %d\n", p.minStock);
    }

    pressEnter();
}

void updateProduct(const char *userId)
{
    createProductsFile();
    struct Product products[1000];
    char targetId[10];

    if (!selectProductHelper(targetId, "UPDATE PRODUCT")) return;

    int count = loadAndSortProducts(products, 1000);
    int idx = binarySearchProduct(products, count, targetId);

    if (idx != -1)
    {
        printf("\nCurrent Details:\n");
        printf("Name: %s | Category: %s | Price: $%.2f | Quantity: %d | MinStock: %d\n\n",
               products[idx].name, products[idx].category, products[idx].price, products[idx].quantity, products[idx].minStock);
        printf("Enter New Details:\n");

        printf("Product Name (Leave blank to keep '%s') : ", products[idx].name);
        char newName[50];
        fgets(newName, sizeof(newName), stdin);
        newName[strcspn(newName, "\n")] = '\0';
        if (strlen(newName) > 0) strcpy(products[idx].name, newName);

        printf("Category (Leave blank to keep '%s')     : ", products[idx].category);
        char newCat[30];
        fgets(newCat, sizeof(newCat), stdin);
        newCat[strcspn(newCat, "\n")] = '\0';
        if (strlen(newCat) > 0) strcpy(products[idx].category, newCat);

        printf("Price (-1 to keep $%.2f)               : ", products[idx].price);
        double newPrice;
        if (scanf("%lf", &newPrice) == 1 && newPrice >= 0) products[idx].price = newPrice;
        while(getchar() != '\n');

        printf("Min Stock (-1 to keep %d)             : ", products[idx].minStock);
        int newMin;
        if (scanf("%d", &newMin) == 1 && newMin >= 0) products[idx].minStock = newMin;
        while(getchar() != '\n');

        saveProductsArray(products, count);
        printf("\nProduct %s updated successfully.\n", targetId);

        char logMsg[100];
        sprintf(logMsg, "Updated product details for %s", targetId);
        logAction(userId, logMsg);
    }
    else
    {
        printf("\nProduct ID %s not found!\n", targetId);
    }

    pressEnter();
}

void deleteProduct(const char *userId)
{
    createProductsFile();
    struct Product products[1000];
    char targetId[10];

    if (!selectProductHelper(targetId, "DELETE PRODUCT")) return;

    int count = loadAndSortProducts(products, 1000);
    int idx = binarySearchProduct(products, count, targetId);

    if (idx != -1)
    {
        printf("\nFound Product: %s - %s ($%.2f, Qty: %d)\n",
               products[idx].id, products[idx].name, products[idx].price, products[idx].quantity);
        printf("Are you sure you want to delete this product? (1 = Yes / 0 = No) : ");
        int confirm = 0;
        scanf("%d", &confirm);
        if (confirm == 1)
        {
            for (int i = idx; i < count - 1; i++)
            {
                products[i] = products[i + 1];
            }
            count--;

            saveProductsArray(products, count);
            printf("\nProduct %s deleted successfully.\n", targetId);

            char logMsg[100];
            sprintf(logMsg, "Deleted product %s", targetId);
            logAction(userId, logMsg);
        }
        else
        {
            printf("\nDeletion cancelled.\n");
        }
    }
    else
    {
        printf("\nProduct ID %s not found!\n", targetId);
    }

    pressEnter();
}

void viewProducts()
{
    createProductsFile();
    struct Product products[1000];
    int count = loadAndSortProducts(products, 1000);

    clearScreen();
    printf("====================================================================================================\n");
    printf("                                      PRODUCT INVENTORY LIST\n");
    printf("====================================================================================================\n\n");

    printf("%-8s %-25s %-18s %-10s %-8s %-10s %-10s %-8s\n",
           "ID", "Product Name", "Category", "Price($)", "Qty", "MinStock", "ReorderQty", "Status");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        char status[15];
        if (products[i].quantity <= 0)
            strcpy(status, "[OUT OF STOCK]");
        else if (products[i].quantity <= products[i].minStock)
            strcpy(status, "[LOW STOCK]");
        else
            strcpy(status, "OK");

        printf("%-8s %-25s %-18s %-10.2f %-8d %-10d %-10d %-8s\n",
               products[i].id, products[i].name, products[i].category,
               products[i].price, products[i].quantity, products[i].minStock, products[i].restockQty, status);
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Total Products in System: %d\n", count);
    pressEnter();
}

void searchProducts()
{
    createProductsFile();
    struct Product products[1000];
    int count = loadAndSortProducts(products, 1000);

    clearScreen();
    printf("=========================================\n");
    printf("             SEARCH PRODUCTS\n");
    printf("=========================================\n\n");
    printf("Search by:\n");
    printf("1. Product ID\n");
    printf("2. Product Name\n");
    printf("3. Category\n");
    printf("Choice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("\nInvalid Input!\n");
        pressEnter();
        return;
    }
    while (getchar() != '\n');

    char query[50];
    printf("Enter search term: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    printf("\n%-8s %-25s %-18s %-10s %-8s %-10s %-10s\n",
           "ID", "Product Name", "Category", "Price($)", "Qty", "MinStock", "ReorderQty");
    printf("---------------------------------------------------------------------------------\n");

    int foundCount = 0;

    if (choice == 1)
    {
        int idx = binarySearchProduct(products, count, query);
        if (idx != -1)
        {
            foundCount = 1;
            printf("%-8s %-25s %-18s %-10.2f %-8d %-10d %-10d\n",
                   products[idx].id, products[idx].name, products[idx].category,
                   products[idx].price, products[idx].quantity, products[idx].minStock, products[idx].restockQty);
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            int match = 0;
            if (choice == 2 && strcasestr(products[i].name, query) != NULL) match = 1;
            else if (choice == 3 && strcasestr(products[i].category, query) != NULL) match = 1;

            if (match)
            {
                foundCount++;
                printf("%-8s %-25s %-18s %-10.2f %-8d %-10d %-10d\n",
                       products[i].id, products[i].name, products[i].category,
                       products[i].price, products[i].quantity, products[i].minStock, products[i].restockQty);
            }
        }
    }

    printf("---------------------------------------------------------------------------------\n");
    printf("Matching records found: %d\n", foundCount);
    pressEnter();
}

void updateStock(const char *userId)
{
    createProductsFile();
    struct Product products[1000];
    char targetId[10];

    if (!selectProductHelper(targetId, "STOCK ARRIVAL / UPDATE")) return;

    int count = loadAndSortProducts(products, 1000);
    int idx = binarySearchProduct(products, count, targetId);

    if (idx != -1)
    {
        printf("\nProduct Matched: %s (%s)\nCurrent Stock: %d\n",
               products[idx].id, products[idx].name, products[idx].quantity);

        printf("Enter Quantity to Add : ");
        int addQty = 0;
        if (scanf("%d", &addQty) == 1 && addQty > 0)
        {
            products[idx].quantity += addQty;
            saveProductsArray(products, count);
            printf("\nStock updated! New Stock: %d\n", products[idx].quantity);

            char logMsg[100];
            sprintf(logMsg, "Restocked %d units for Product %s (New Stock: %d)", addQty, targetId, products[idx].quantity);
            logAction(userId, logMsg);
        }
        else
        {
            printf("\nInvalid quantity entered.\n");
        }
    }
    else
    {
        printf("\nProduct ID %s not found!\n", targetId);
    }

    pressEnter();
}

void checkLowStockAlerts()
{
    createProductsFile();
    struct Product products[1000];
    int count = loadAndSortProducts(products, 1000);

    clearScreen();
    printf("====================================================================================================\n");
    printf("                               LOW STOCK & RESTOCK ALERTS REPORT\n");
    printf("====================================================================================================\n\n");

    int alertCount = 0;

    printf("%-8s %-25s %-18s %-8s %-10s %-12s %-22s\n",
           "ID", "Product Name", "Category", "Qty", "MinStock", "ReorderQty", "Alert Status");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        int isLowStock = (products[i].quantity <= products[i].minStock);
        int hasRestockReq = (products[i].restockQty > 0);

        if (isLowStock || hasRestockReq)
        {
            alertCount++;
            char status[30];

            if (products[i].quantity <= 0 && hasRestockReq)
                strcpy(status, "[OUT OF STOCK & REORDERED]");
            else if (products[i].quantity <= 0)
                strcpy(status, "[OUT OF STOCK]");
            else if (isLowStock && hasRestockReq)
                strcpy(status, "[LOW STOCK & REORDERED]");
            else if (isLowStock)
                strcpy(status, "[LOW STOCK]");
            else
                strcpy(status, "[RESTOCK REQUESTED]");

            printf("%-8s %-25s %-18s %-8d %-10d %-12d %-22s\n",
                   products[i].id, products[i].name, products[i].category,
                   products[i].quantity, products[i].minStock, products[i].restockQty, status);
        }
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    if (alertCount == 0)
    {
        printf("All products have sufficient stock levels and no pending restock requests.\n");
    }
    else
    {
        printf("ATTENTION: %d product(s) require attention for low stock or pending restock!\n", alertCount);
    }

    pressEnter();
}

void viewRestockRequests()
{
    checkLowStockAlerts();
}

void requestRestock(const char *userId)
{
    createProductsFile();
    struct Product products[1000];
    char prodId[10];

    if (!selectProductHelper(prodId, "REQUEST STOCK REPLENISHMENT")) return;

    int count = loadAndSortProducts(products, 1000);
    int idx = binarySearchProduct(products, count, prodId);

    if (idx != -1)
    {
        printf("\nProduct Name      : %s\n", products[idx].name);
        printf("Current Quantity  : %d\n", products[idx].quantity);
        printf("Current Reorder   : %d\n", products[idx].restockQty);
        printf("Enter Quantity to Request : ");
        int reqQty = 0;
        if (scanf("%d", &reqQty) == 1 && reqQty > 0)
        {
            products[idx].restockQty += reqQty;
            saveProductsArray(products, count);
            printf("\nRestock request of %d units added for product %s!\n", reqQty, products[idx].id);

            char logMsg[100];
            sprintf(logMsg, "Requested restock of %d units for Product %s", reqQty, products[idx].id);
            logAction(userId, logMsg);
        }
        else
        {
            printf("\nInvalid quantity!\n");
        }
    }
    else
    {
        printf("\nProduct ID %s not found!\n", prodId);
    }

    pressEnter();
}

void processRestockRequest(const char *userId)
{
    createProductsFile();
    struct Product products[1000];
    char prodId[10];

    clearScreen();
    printf("=========================================\n");
    printf("         PROCESS RESTOCK REQUEST\n");
    printf("=========================================\n\n");

    int count = loadAndSortProducts(products, 1000);
    int pendingCount = 0;

    printf("%-10s %-25s %-12s %-12s\n", "Prod ID", "Product Name", "Current Stock", "Requested Qty");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        if (products[i].restockQty > 0)
        {
            pendingCount++;
            printf("%-10s %-25s %-12d %-12d\n", products[i].id, products[i].name, products[i].quantity, products[i].restockQty);
        }
    }
    printf("------------------------------------------------------------------\n\n");

    if (pendingCount == 0)
    {
        printf("No pending restock requests found.\n");
        pressEnter();
        return;
    }

    printf("Enter Product ID to Fulfill Restock (or '0' to cancel) : ");
    scanf("%9s", prodId);
    while (getchar() != '\n');

    if (strcmp(prodId, "0") == 0) return;

    int idx = binarySearchProduct(products, count, prodId);

    if (idx != -1)
    {
        if (products[idx].restockQty <= 0)
        {
            printf("\nProduct %s does not have any pending restock requests.\n", prodId);
        }
        else
        {
            printf("\nProduct Details: ID: %s | Name: %s | Stock: %d | Requested: %d\n",
                   products[idx].id, products[idx].name, products[idx].quantity, products[idx].restockQty);
            printf("Approve or Reject request? (1 = Approve & Add Stock / 2 = Reject Request / 0 = Cancel) : ");
            int decision = 0;
            scanf("%d", &decision);

            if (decision == 1)
            {
                products[idx].quantity += products[idx].restockQty;
                printf("\nRestock Approved! Added %d units to Stock. New Stock: %d\n", products[idx].restockQty, products[idx].quantity);

                char logMsg[100];
                sprintf(logMsg, "Approved restock of %d units for Product %s (New Stock: %d)", products[idx].restockQty, products[idx].id, products[idx].quantity);
                logAction(userId, logMsg);

                products[idx].restockQty = 0;
                saveProductsArray(products, count);
            }
            else if (decision == 2)
            {
                printf("\nRestock Request Rejected.\n");
                char logMsg[100];
                sprintf(logMsg, "Rejected restock request for Product %s", products[idx].id);
                logAction(userId, logMsg);

                products[idx].restockQty = 0;
                saveProductsArray(products, count);
            }
        }
    }
    else
    {
        printf("\nProduct ID %s not found!\n", prodId);
    }

    pressEnter();
}

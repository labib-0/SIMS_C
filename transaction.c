#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product
{
    char id[10];
    char name[50];
    char category[30];
    double price;
    int quantity;
    int minStock;
    int restockQty;
    char stockAlert[30];
};

struct Transaction
{
    char transId[15];
    char date[15];
    char time[15];
    char productId[10];
    char productName[50];
    int quantity;
    double unitPrice;
    double totalPrice;
    char soldBy[10];
};

void clearScreen();
void pressEnter();
void skipHeader(FILE *);
long dateToInteger(const char *dateStr);
void getCurrentDateTime(char dateStr[], char timeStr[]);
void logAction(const char *userId, const char *action);

void createProductsFile();
int binarySearchProduct(struct Product products[], int n, const char *targetId);
int loadAndSortProducts(struct Product products[], int maxProducts);
void saveProductsArray(struct Product products[], int n);

void createTransactionsFile()
{
    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        fp = fopen("transactions.csv", "w");
        if (fp != NULL)
        {
            fprintf(fp, "TransactionID,Date,Time,ProductID,ProductName,Quantity,UnitPrice,TotalPrice,SoldBy\n");
            fclose(fp);
        }
    }
    else
    {
        fclose(fp);
    }
}

void generateTransactionID(char id[])
{
    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        strcpy(id, "T10001");
        return;
    }

    skipHeader(fp);
    struct Transaction t;
    long max = 10000;

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                  t.transId, t.date, t.time, t.productId, t.productName,
                  &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
    {
        if (t.transId[0] == 'T')
        {
            long num = atol(t.transId + 1);
            if (num > max) max = num;
        }
    }
    fclose(fp);
    sprintf(id, "T%ld", max + 1);
}

void processSale(const char *userId)
{
    createProductsFile();
    createTransactionsFile();

    struct Product products[1000];
    char selectedProdId[10] = "";

    while (1)
    {
        int count = loadAndSortProducts(products, 1000);

        clearScreen();
        printf("===================================================================\n");
        printf("                        PROCESS SALES TRANSACTION\n");
        printf("===================================================================\n\n");

        printf("%-12s %-30s %-12s %-15s\n", "Product ID", "Product Name", "Price ($)", "Available Qty");
        printf("-------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++)
        {
            printf("%-12s %-30s %-12.2f %-15d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
        }

        printf("-------------------------------------------------------------------\n");
        printf("Total Products : %d\n\n", count);

        printf("Enter Product ID to sell (or '0' to cancel) : ");
        if (scanf("%9s", selectedProdId) != 1) return;
        while (getchar() != '\n');

        if (strcmp(selectedProdId, "0") == 0) return;

        int idx = binarySearchProduct(products, count, selectedProdId);

        if (idx == -1)
        {
            printf("\nProduct ID '%s' not found!\n", selectedProdId);
            pressEnter();
            selectedProdId[0] = '\0';
            continue;
        }

        printf("\nProduct Selected: %s (%s)\n", products[idx].name, products[idx].id);
        printf("Category         : %s\n", products[idx].category);
        printf("Price per item   : $%.2f\n", products[idx].price);
        printf("Available Stock  : %d\n\n", products[idx].quantity);

        if (products[idx].quantity <= 0)
        {
            printf("ERROR: Product is OUT OF STOCK!\n");
            pressEnter();
            selectedProdId[0] = '\0';
            continue;
        }

        printf("Enter Quantity to Sell : ");
        int saleQty = 0;
        if (scanf("%d", &saleQty) != 1 || saleQty <= 0)
        {
            printf("\nInvalid sale quantity!\n");
            pressEnter();
            selectedProdId[0] = '\0';
            continue;
        }

        if (saleQty > products[idx].quantity)
        {
            printf("\nERROR: Insufficient Stock! Only %d available.\n", products[idx].quantity);
            pressEnter();
            selectedProdId[0] = '\0';
            continue;
        }

        double totalAmount = products[idx].price * saleQty;
        products[idx].quantity -= saleQty;
        int newQuantity = products[idx].quantity;

        saveProductsArray(products, count);

        struct Transaction t;
        generateTransactionID(t.transId);
        getCurrentDateTime(t.date, t.time);
        strcpy(t.productId, products[idx].id);
        strcpy(t.productName, products[idx].name);
        t.quantity = saleQty;
        t.unitPrice = products[idx].price;
        t.totalPrice = totalAmount;
        strcpy(t.soldBy, (userId && strlen(userId) > 0) ? userId : "N/A");

        FILE *tf = fopen("transactions.csv", "a");
        if (tf != NULL)
        {
            fprintf(tf, "%s,%s,%s,%s,%s,%d,%.2f,%.2f,%s\n",
                    t.transId, t.date, t.time, t.productId, t.productName,
                    t.quantity, t.unitPrice, t.totalPrice, t.soldBy);
            fclose(tf);
        }

        char logMsg[120];
        sprintf(logMsg, "Processed Sale %s: Product %s (Qty: %d, Total: $%.2f)",
                t.transId, t.productId, saleQty, totalAmount);
        logAction(userId, logMsg);

        clearScreen();
        printf("=========================================\n");
        printf("        TRANSACTION COMPLETED\n");
        printf("=========================================\n\n");
        printf("Transaction ID : %s\n", t.transId);
        printf("Date & Time    : %s %s\n", t.date, t.time);
        printf("Product        : %s (%s)\n", t.productName, t.productId);
        printf("Quantity Sold  : %d\n", t.quantity);
        printf("Unit Price     : $%.2f\n", t.unitPrice);
        printf("Total Price    : $%.2f\n", t.totalPrice);
        printf("Remaining Stock: %d\n", newQuantity);

        if (newQuantity <= products[idx].minStock)
        {
            printf("\n-----------------------------------------\n");
            printf("ALERT: Stock level is now LOW (%d <= %d)!\n", newQuantity, products[idx].minStock);
            printf("-----------------------------------------\n");
        }

        pressEnter();
        return;
    }
}

void viewTransactions()
{
    createTransactionsFile();

    clearScreen();
    printf("========================================================================================\n");
    printf("                               TRANSACTION HISTORY\n");
    printf("========================================================================================\n\n");

    printf("1. View All Transactions\n");
    printf("2. Filter Transactions by Date Range\n");
    printf("0. Back\n");
    printf("Choice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("\nInvalid Choice!\n");
        pressEnter();
        return;
    }
    while (getchar() != '\n');

    if (choice == 0) return;

    char startDateStr[15] = "", endDateStr[15] = "";
    long startInt = 0, endInt = 99999999L;

    if (choice == 2)
    {
        printf("\nEnter Start Date (DD/MM/YYYY) : ");
        scanf("%14s", startDateStr);
        printf("Enter End Date (DD/MM/YYYY)   : ");
        scanf("%14s", endDateStr);
        while (getchar() != '\n');

        startInt = dateToInteger(startDateStr);
        endInt = dateToInteger(endDateStr);
    }

    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        printf("\nUnable to open transactions database!\n");
        pressEnter();
        return;
    }

    skipHeader(fp);
    struct Transaction t;
    int count = 0;
    double grandTotal = 0.0;

    printf("\n%-10s %-12s %-10s %-8s %-20s %-6s %-10s %-10s %-8s\n",
           "Trans ID", "Date", "Time", "Prod ID", "Product Name", "Qty", "Price($)", "Total($)", "Sold By");
    printf("----------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                  t.transId, t.date, t.time, t.productId, t.productName,
                  &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
    {
        long transDateInt = dateToInteger(t.date);
        if (transDateInt >= startInt && transDateInt <= endInt)
        {
            count++;
            grandTotal += t.totalPrice;
            printf("%-10s %-12s %-10s %-8s %-20s %-6d %-10.2f %-10.2f %-8s\n",
                   t.transId, t.date, t.time, t.productId, t.productName,
                   t.quantity, t.unitPrice, t.totalPrice, t.soldBy);
        }
    }

    fclose(fp);

    printf("----------------------------------------------------------------------------------------\n");
    printf("Total Transactions : %d\n", count);
    printf("Total Sales Revenue: $%.2f\n", grandTotal);
    pressEnter();
}

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

struct CartItem
{
    char productId[10];
    char productName[50];
    double unitPrice;
    int quantity;
    double itemTotal;
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
    struct CartItem cart[100];
    int cartCount = 0;
    char inputStr[50];

    while (1)
    {
        int count = loadAndSortProducts(products, 1000);

        clearScreen();
        printf("===================================================================\n");
        printf("                   PROCESS SALES (SHOPPING CART)\n");
        printf("===================================================================\n\n");

        printf("AVAILABLE STORE PRODUCTS:\n");
        printf("%-10s %-25s %-15s %-10s %-12s\n", "Prod ID", "Product Name", "Category", "Price ($)", "Available Qty");
        printf("-------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++)
        {
            int inCartQty = 0;
            for (int k = 0; k < cartCount; k++)
            {
                if (strcasecmp(cart[k].productId, products[i].id) == 0)
                    inCartQty += cart[k].quantity;
            }
            int effQty = products[i].quantity - inCartQty;

            printf("%-10s %-25s %-15s %-10.2f %-12d\n",
                   products[i].id, products[i].name, products[i].category, products[i].price, effQty);
        }
        printf("-------------------------------------------------------------------\n\n");

        double cartTotal = 0.0;
        printf("CURRENT CART ITEMS:\n");
        if (cartCount == 0)
        {
            printf("  [Cart is empty]\n");
        }
        else
        {
            printf("%-5s %-10s %-25s %-6s %-10s %-10s\n", "No.", "Prod ID", "Product Name", "Qty", "Price($)", "Subtotal($)");
            printf("-------------------------------------------------------------------\n");
            for (int k = 0; k < cartCount; k++)
            {
                cartTotal += cart[k].itemTotal;
                printf("%-5d %-10s %-25s %-6d %-10.2f %-10.2f\n",
                       k + 1, cart[k].productId, cart[k].productName,
                       cart[k].quantity, cart[k].unitPrice, cart[k].itemTotal);
            }
        }
        printf("-------------------------------------------------------------------\n");
        printf("TOTAL AMOUNT SO FAR : $%.2f\n\n", cartTotal);

        printf("Enter Product ID to Add to Cart, '1' to Checkout, or '0' to Cancel : ");
        if (scanf("%49s", inputStr) != 1) return;
        while (getchar() != '\n');

        if (strcmp(inputStr, "0") == 0)
        {
            printf("\nSales transaction cancelled.\n");
            pressEnter();
            return;
        }

        if (strcmp(inputStr, "1") == 0)
        {
            if (cartCount == 0)
            {
                printf("\nCart is empty! Add at least 1 product before checking out.\n");
                pressEnter();
                continue;
            }

            char transDate[15], transTime[15];
            getCurrentDateTime(transDate, transTime);

            FILE *tf = fopen("transactions.csv", "a");

            clearScreen();
            printf("===================================================================\n");
            printf("                     SALES RECEIPT / INVOICE\n");
            printf("===================================================================\n\n");
            printf("Date & Time : %s %s\n", transDate, transTime);
            printf("Cashier/Staff: %s\n", (userId && strlen(userId) > 0) ? userId : "N/A");
            printf("-------------------------------------------------------------------\n");
            printf("%-5s %-10s %-25s %-6s %-10s %-10s\n", "No.", "Trans ID", "Product Name", "Qty", "Price($)", "Subtotal($)");
            printf("-------------------------------------------------------------------\n");

            double grandTotal = 0.0;

            for (int k = 0; k < cartCount; k++)
            {
                char transId[15];
                generateTransactionID(transId);

                int idx = binarySearchProduct(products, count, cart[k].productId);
                if (idx != -1)
                {
                    products[idx].quantity -= cart[k].quantity;
                    if (products[idx].quantity <= 0)
                        strcpy(products[idx].stockAlert, "System Out of Stock");
                    else if (products[idx].quantity <= products[idx].minStock)
                        strcpy(products[idx].stockAlert, "System Low Stock");
                }

                if (tf != NULL)
                {
                    fprintf(tf, "%s,%s,%s,%s,%s,%d,%.2f,%.2f,%s\n",
                            transId, transDate, transTime, cart[k].productId, cart[k].productName,
                            cart[k].quantity, cart[k].unitPrice, cart[k].itemTotal,
                            (userId && strlen(userId) > 0) ? userId : "N/A");
                }

                grandTotal += cart[k].itemTotal;

                printf("%-5d %-10s %-25s %-6d %-10.2f %-10.2f\n",
                       k + 1, transId, cart[k].productName,
                       cart[k].quantity, cart[k].unitPrice, cart[k].itemTotal);
            }

            if (tf != NULL) fclose(tf);
            saveProductsArray(products, count);

            printf("-------------------------------------------------------------------\n");
            printf("GRAND TOTAL AMOUNT : $%.2f\n", grandTotal);
            printf("===================================================================\n\n");

            char logMsg[150];
            sprintf(logMsg, "Processed Sale Invoice: %d item(s) (Grand Total: $%.2f)", cartCount, grandTotal);
            logAction(userId, logMsg);

            pressEnter();
            return;
        }

        int idx = binarySearchProduct(products, count, inputStr);

        if (idx == -1)
        {
            printf("\nProduct ID '%s' not found!\n", inputStr);
            pressEnter();
            continue;
        }

        int inCartQty = 0;
        int existingCartIdx = -1;
        for (int k = 0; k < cartCount; k++)
        {
            if (strcasecmp(cart[k].productId, products[idx].id) == 0)
            {
                inCartQty += cart[k].quantity;
                existingCartIdx = k;
            }
        }

        int availableQty = products[idx].quantity - inCartQty;

        if (availableQty <= 0)
        {
            printf("\nERROR: Product '%s' is OUT OF STOCK!\n", products[idx].name);
            pressEnter();
            continue;
        }

        printf("\nSelected Product : %s (%s) | Price: $%.2f | Available: %d\n",
               products[idx].name, products[idx].id, products[idx].price, availableQty);
        printf("Enter Quantity to Add to Cart : ");

        int addQty = 0;
        if (scanf("%d", &addQty) != 1 || addQty <= 0)
        {
            while (getchar() != '\n');
            printf("\nInvalid quantity!\n");
            pressEnter();
            continue;
        }
        while (getchar() != '\n');

        if (addQty > availableQty)
        {
            printf("\nERROR: Insufficient Stock! Only %d available.\n", availableQty);
            pressEnter();
            continue;
        }

        if (existingCartIdx != -1)
        {
            cart[existingCartIdx].quantity += addQty;
            cart[existingCartIdx].itemTotal = cart[existingCartIdx].quantity * cart[existingCartIdx].unitPrice;
        }
        else
        {
            strcpy(cart[cartCount].productId, products[idx].id);
            strcpy(cart[cartCount].productName, products[idx].name);
            cart[cartCount].unitPrice = products[idx].price;
            cart[cartCount].quantity = addQty;
            cart[cartCount].itemTotal = addQty * products[idx].price;
            cartCount++;
        }

        printf("\nAdded %d unit(s) of %s to Cart!\n", addQty, products[idx].name);
    }
}

void viewTransactions()
{
    createTransactionsFile();

    clearScreen();
    printf("========================================================================================\n");
    printf("                               TRANSACTION HISTORY\n");
    printf("========================================================================================\n\n");

    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        printf("Unable to open transactions database!\n");
        pressEnter();
        return;
    }

    skipHeader(fp);
    struct Transaction t;
    int count = 0;
    double grandTotal = 0.0;

    printf("%-10s %-12s %-10s %-8s %-20s %-6s %-10s %-10s %-8s\n",
           "Trans ID", "Date", "Time", "Prod ID", "Product Name", "Qty", "Price($)", "Total($)", "Sold By");
    printf("----------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                  t.transId, t.date, t.time, t.productId, t.productName,
                  &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
    {
        count++;
        grandTotal += t.totalPrice;
        printf("%-10s %-12s %-10s %-8s %-20s %-6d %-10.2f %-10.2f %-8s\n",
               t.transId, t.date, t.time, t.productId, t.productName,
               t.quantity, t.unitPrice, t.totalPrice, t.soldBy);
    }

    fclose(fp);

    printf("----------------------------------------------------------------------------------------\n");
    printf("Total Transactions : %d\n", count);
    printf("Total Sales Revenue: $%.2f\n", grandTotal);
    pressEnter();
}

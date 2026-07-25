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

struct PerformanceItem
{
    char id[10];
    char name[50];
    int totalSold;
    double totalRevenue;
};

void clearScreen();
void pressEnter();
void skipHeader(FILE *);
long dateToInteger(const char *dateStr);
void getCurrentDateTime(char dateStr[], char timeStr[]);
void logAction(const char *userId, const char *action);

void createProductsFile();
void createTransactionsFile();

void generateInventoryReport(const char *userId)
{
    createProductsFile();

    clearScreen();
    printf("=================================================================================\n");
    printf("                              INVENTORY REPORT\n");
    printf("=================================================================================\n\n");

    FILE *fp = fopen("products.csv", "r");
    if (fp == NULL)
    {
        printf("Unable to open products database!\n");
        pressEnter();
        return;
    }

    skipHeader(fp);
    struct Product p;
    int totalProducts = 0;
    int totalItems = 0;
    double totalValuation = 0.0;

    printf("%-8s %-25s %-15s %-10s %-8s %-12s\n",
           "ID", "Product Name", "Category", "Price($)", "Qty", "Value($)");
    printf("---------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%[^,],%[^,],%[^,],%lf,%d,%d,%d\n",
                  p.id, p.name, p.category, &p.price, &p.quantity, &p.minStock, &p.restockQty) >= 6)
    {
        totalProducts++;
        totalItems += p.quantity;
        double itemVal = p.price * p.quantity;
        totalValuation += itemVal;

        printf("%-8s %-25s %-15s %-10.2f %-8d %-12.2f\n",
               p.id, p.name, p.category, p.price, p.quantity, itemVal);
    }
    fclose(fp);

    printf("---------------------------------------------------------------------------------\n");
    printf("Total Unique Products : %d\n", totalProducts);
    printf("Total Items in Stock  : %d\n", totalItems);
    printf("Total Valuation       : $%.2f\n\n", totalValuation);

    printf("Do you want to save this report to 'inventory_report.txt'? (1 = Yes / 0 = No) : ");
    int saveChoice = 0;
    if (scanf("%d", &saveChoice) == 1 && saveChoice == 1)
    {
        FILE *rf = fopen("inventory_report.txt", "w");
        if (rf != NULL)
        {
            char dateStr[15], timeStr[15];
            getCurrentDateTime(dateStr, timeStr);
            fprintf(rf, "=================================================================================\n");
            fprintf(rf, "                       INVENTORY REPORT - Generated: %s %s\n", dateStr, timeStr);
            fprintf(rf, "=================================================================================\n\n");
            
            fp = fopen("products.csv", "r");
            skipHeader(fp);
            fprintf(rf, "%-8s %-25s %-15s %-10s %-8s %-12s\n", "ID", "Product Name", "Category", "Price($)", "Qty", "Value($)");
            fprintf(rf, "---------------------------------------------------------------------------------\n");
            while (fscanf(fp, "%[^,],%[^,],%[^,],%lf,%d,%d,%d\n", p.id, p.name, p.category, &p.price, &p.quantity, &p.minStock, &p.restockQty) >= 6)
            {
                fprintf(rf, "%-8s %-25s %-15s %-10.2f %-8d %-12.2f\n", p.id, p.name, p.category, p.price, p.quantity, p.price * p.quantity);
            }
            fclose(fp);
            fprintf(rf, "---------------------------------------------------------------------------------\n");
            fprintf(rf, "Total Unique Products : %d\n", totalProducts);
            fprintf(rf, "Total Items in Stock  : %d\n", totalItems);
            fprintf(rf, "Total Valuation       : $%.2f\n", totalValuation);
            fclose(rf);

            printf("\nReport successfully saved to 'inventory_report.txt'\n");
            logAction(userId, "Generated and saved Inventory Report to file");
        }
    }
    else
    {
        logAction(userId, "Viewed Inventory Report");
    }

    pressEnter();
}

void generateSalesReport(const char *userId)
{
    createTransactionsFile();

    clearScreen();
    printf("========================================================================================\n");
    printf("                                 SALES SUMMARY REPORT\n");
    printf("========================================================================================\n\n");

    char startDateStr[15], endDateStr[15];
    printf("Enter Start Date (DD/MM/YYYY) : ");
    scanf("%14s", startDateStr);
    printf("Enter End Date (DD/MM/YYYY)   : ");
    scanf("%14s", endDateStr);
    while (getchar() != '\n');

    long startInt = dateToInteger(startDateStr);
    long endInt = dateToInteger(endDateStr);

    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        printf("\nUnable to open transactions database!\n");
        pressEnter();
        return;
    }

    skipHeader(fp);
    struct Transaction t;
    int totalSales = 0;
    int totalQtySold = 0;
    double totalRevenue = 0.0;

    printf("\n%-10s %-12s %-10s %-8s %-20s %-6s %-10s %-10s\n",
           "Trans ID", "Date", "Time", "Prod ID", "Product Name", "Qty", "Price($)", "Total($)");
    printf("----------------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                  t.transId, t.date, t.time, t.productId, t.productName,
                  &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
    {
        long transInt = dateToInteger(t.date);
        if (transInt >= startInt && transInt <= endInt)
        {
            totalSales++;
            totalQtySold += t.quantity;
            totalRevenue += t.totalPrice;

            printf("%-10s %-12s %-10s %-8s %-20s %-6d %-10.2f %-10.2f\n",
                   t.transId, t.date, t.time, t.productId, t.productName,
                   t.quantity, t.unitPrice, t.totalPrice);
        }
    }
    fclose(fp);

    printf("----------------------------------------------------------------------------------------\n");
    printf("Report Period          : %s to %s\n", startDateStr, endDateStr);
    printf("Total Transactions     : %d\n", totalSales);
    printf("Total Products Sold    : %d units\n", totalQtySold);
    printf("Total Revenue Generated: $%.2f\n\n", totalRevenue);

    printf("Do you want to save this report to 'sales_report.txt'? (1 = Yes / 0 = No) : ");
    int saveChoice = 0;
    if (scanf("%d", &saveChoice) == 1 && saveChoice == 1)
    {
        FILE *rf = fopen("sales_report.txt", "w");
        if (rf != NULL)
        {
            char dStr[15], tmStr[15];
            getCurrentDateTime(dStr, tmStr);
            fprintf(rf, "========================================================================================\n");
            fprintf(rf, "                    SALES SUMMARY REPORT (%s to %s) - Generated: %s %s\n", startDateStr, endDateStr, dStr, tmStr);
            fprintf(rf, "========================================================================================\n\n");
            fprintf(rf, "%-10s %-12s %-10s %-8s %-20s %-6s %-10s %-10s\n", "Trans ID", "Date", "Time", "Prod ID", "Product Name", "Qty", "Price($)", "Total($)");
            fprintf(rf, "----------------------------------------------------------------------------------------\n");

            fp = fopen("transactions.csv", "r");
            skipHeader(fp);
            while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                          t.transId, t.date, t.time, t.productId, t.productName,
                          &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
            {
                long transInt = dateToInteger(t.date);
                if (transInt >= startInt && transInt <= endInt)
                {
                    fprintf(rf, "%-10s %-12s %-10s %-8s %-20s %-6d %-10.2f %-10.2f\n",
                            t.transId, t.date, t.time, t.productId, t.productName,
                            t.quantity, t.unitPrice, t.totalPrice);
                }
            }
            fclose(fp);
            fprintf(rf, "----------------------------------------------------------------------------------------\n");
            fprintf(rf, "Total Transactions     : %d\n", totalSales);
            fprintf(rf, "Total Products Sold    : %d units\n", totalQtySold);
            fprintf(rf, "Total Revenue Generated: $%.2f\n", totalRevenue);
            fclose(rf);

            printf("\nReport successfully saved to 'sales_report.txt'\n");
            logAction(userId, "Generated and saved Sales Report to file");
        }
    }
    else
    {
        logAction(userId, "Viewed Sales Report");
    }

    pressEnter();
}

void generatePerformanceReport(const char *userId)
{
    createProductsFile();
    createTransactionsFile();

    clearScreen();
    printf("=================================================================================\n");
    printf("                           PRODUCT PERFORMANCE REPORT\n");
    printf("=================================================================================\n\n");

    struct PerformanceItem items[200];
    int itemCnt = 0;

    FILE *fp = fopen("products.csv", "r");
    if (fp != NULL)
    {
        skipHeader(fp);
        struct Product p;
        while (fscanf(fp, "%[^,],%[^,],%[^,],%lf,%d,%d,%d\n", p.id, p.name, p.category, &p.price, &p.quantity, &p.minStock, &p.restockQty) >= 6)
        {
            if (itemCnt < 200)
            {
                strcpy(items[itemCnt].id, p.id);
                strcpy(items[itemCnt].name, p.name);
                items[itemCnt].totalSold = 0;
                items[itemCnt].totalRevenue = 0.0;
                itemCnt++;
            }
        }
        fclose(fp);
    }

    FILE *tf = fopen("transactions.csv", "r");
    if (tf != NULL)
    {
        skipHeader(tf);
        struct Transaction t;
        while (fscanf(tf, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^\n]\n",
                      t.transId, t.date, t.time, t.productId, t.productName,
                      &t.quantity, &t.unitPrice, &t.totalPrice, t.soldBy) == 9)
        {
            for (int i = 0; i < itemCnt; i++)
            {
                if (strcmp(items[i].id, t.productId) == 0)
                {
                    items[i].totalSold += t.quantity;
                    items[i].totalRevenue += t.totalPrice;
                    break;
                }
            }
        }
        fclose(tf);
    }

    for (int i = 0; i < itemCnt - 1; i++)
    {
        for (int j = i + 1; j < itemCnt; j++)
        {
            if (items[j].totalSold > items[i].totalSold)
            {
                struct PerformanceItem temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }

    printf("TOP-SELLING PRODUCTS SUMMARY:\n");
    printf("%-5s %-8s %-25s %-12s %-12s\n", "Rank", "ID", "Product Name", "Units Sold", "Revenue($)");
    printf("---------------------------------------------------------------------------------\n");
    for (int i = 0; i < itemCnt && i < 10; i++)
    {
        printf("%-5d %-8s %-25s %-12d %-12.2f\n",
               i + 1, items[i].id, items[i].name, items[i].totalSold, items[i].totalRevenue);
    }
    printf("---------------------------------------------------------------------------------\n\n");

    printf("Do you want to save this performance report to 'performance_report.txt'? (1 = Yes / 0 = No) : ");
    int saveChoice = 0;
    if (scanf("%d", &saveChoice) == 1 && saveChoice == 1)
    {
        FILE *rf = fopen("performance_report.txt", "w");
        if (rf != NULL)
        {
            char dStr[15], tmStr[15];
            getCurrentDateTime(dStr, tmStr);
            fprintf(rf, "=================================================================================\n");
            fprintf(rf, "                    PERFORMANCE REPORT - Generated: %s %s\n", dStr, tmStr);
            fprintf(rf, "=================================================================================\n\n");
            fprintf(rf, "%-5s %-8s %-25s %-12s %-12s\n", "Rank", "ID", "Product Name", "Units Sold", "Revenue($)");
            fprintf(rf, "---------------------------------------------------------------------------------\n");
            for (int i = 0; i < itemCnt; i++)
            {
                fprintf(rf, "%-5d %-8s %-25s %-12d %-12.2f\n",
                        i + 1, items[i].id, items[i].name, items[i].totalSold, items[i].totalRevenue);
            }
            fclose(rf);

            printf("\nReport successfully saved to 'performance_report.txt'\n");
            logAction(userId, "Generated and saved Performance Report to file");
        }
    }
    else
    {
        logAction(userId, "Viewed Performance Report");
    }

    pressEnter();
}

void viewAuditLog(const char *userId)
{
    clearScreen();
    printf("=================================================================================\n");
    printf("                              SYSTEM AUDIT LOGS\n");
    printf("=================================================================================\n\n");

    FILE *fp = fopen("audit_log.txt", "r");
    if (fp == NULL)
    {
        printf("No audit log entries found.\n");
        pressEnter();
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), fp))
    {
        count++;
        printf("%s", line);
    }
    fclose(fp);

    printf("---------------------------------------------------------------------------------\n");
    printf("Total Log Entries: %d\n", count);

    logAction(userId, "Viewed System Audit Logs");
    pressEnter();
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

struct User
{
    char id[10];
    char name[50];
    char role[20];
    char dob[15];
    char email[60];
    unsigned long password;
};

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

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter()
{
    printf("\nPress Enter To Continue...");
    while(getchar()!='\n');
    getchar();
}

void delay()
{
#ifdef _WIN32
    Sleep(500);
#else
    usleep(500000);
#endif
}

void getCurrentDateTime(char dateStr[], char timeStr[])
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if(dateStr != NULL)
        sprintf(dateStr, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    if(timeStr != NULL)
        sprintf(timeStr, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
}

void logAction(const char *userId, const char *action)
{
    char dateStr[15], timeStr[15];
    getCurrentDateTime(dateStr, timeStr);

    FILE *fp = fopen("audit_log.txt", "a");
    if(fp != NULL)
    {
        fprintf(fp, "[%s %s] [%s] %s\n", dateStr, timeStr, (userId && strlen(userId) > 0) ? userId : "SYSTEM", action);
        fclose(fp);
    }
}

void skipHeader(FILE *fp)
{
    fscanf(fp, "%*[^\n]\n");
}

long dateToInteger(const char *dateStr)
{
    int d = 0, m = 0, y = 0;
    if (sscanf(dateStr, "%d/%d/%d", &d, &m, &y) == 3)
    {
        return y * 10000L + m * 100L + d;
    }
    return 0;
}

int validateEmail(char email[])
{
    int len = strlen(email);
    if (len < 5) return 0;

    int atPos = -1;
    int dotPos = -1;

    for (int i = 0; i < len; i++)
    {
        if (email[i] == '@')
        {
            if (atPos != -1) return 0;
            atPos = i;
        }
        else if (email[i] == '.')
        {
            dotPos = i;
        }
    }

    if (atPos == -1 || dotPos == -1) return 0;
    if (atPos == 0 || atPos == len - 1) return 0;
    if (dotPos == 0 || dotPos == len - 1) return 0;
    if (abs(atPos - dotPos) <= 1) return 0;
    if (atPos > dotPos) return 0;

    return 1;
}

/* BINARY SEARCH & DATA HELPERS FOR USERS */

static int compareUsersByID(const void *a, const void *b)
{
    const struct User *u1 = (const struct User *)a;
    const struct User *u2 = (const struct User *)b;
    return strcmp(u1->id, u2->id);
}

int binarySearchUser(struct User users[], int n, const char *targetId)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(users[mid].id, targetId);
        if (cmp == 0) return mid;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int loadAndSortUsers(struct User users[], int maxUsers)
{
    FILE *fp = fopen("users.csv", "r");
    if (fp == NULL) return 0;
    skipHeader(fp);
    int count = 0;
    while (count < maxUsers && fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lu\n",
                  users[count].id, users[count].name, users[count].role,
                  users[count].dob, users[count].email, &users[count].password) == 6)
    {
        count++;
    }
    fclose(fp);
    qsort(users, count, sizeof(struct User), compareUsersByID);
    return count;
}

void saveUsersArray(struct User users[], int n)
{
    FILE *fp = fopen("users.csv", "w");
    if (fp != NULL)
    {
        fprintf(fp, "UserID,FullName,Role,DOB,Email,Password\n");
        for (int i = 0; i < n; i++)
        {
            fprintf(fp, "%s,%s,%s,%s,%s,%lu\n",
                    users[i].id, users[i].name, users[i].role, users[i].dob, users[i].email, users[i].password);
        }
        fclose(fp);
    }
}

/* BINARY SEARCH & DATA HELPERS FOR PRODUCTS */

static int compareProductsByID(const void *a, const void *b)
{
    const struct Product *p1 = (const struct Product *)a;
    const struct Product *p2 = (const struct Product *)b;
    return strcasecmp(p1->id, p2->id);
}

int binarySearchProduct(struct Product products[], int n, const char *targetId)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcasecmp(products[mid].id, targetId);
        if (cmp == 0) return mid;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int loadAndSortProducts(struct Product products[], int maxProducts)
{
    FILE *fp = fopen("products.csv", "r");
    if (fp == NULL) return 0;
    skipHeader(fp);
    int count = 0;
    while (count < maxProducts)
    {
        int read = fscanf(fp, "%[^,],%[^,],%[^,],%lf,%d,%d,%d,%[^\n]\n",
                          products[count].id, products[count].name, products[count].category,
                          &products[count].price, &products[count].quantity, &products[count].minStock,
                          &products[count].restockQty, products[count].stockAlert);
        if (read >= 7)
        {
            if (read == 7 || strlen(products[count].stockAlert) == 0) strcpy(products[count].stockAlert, "-");
            count++;
        }
        else
        {
            break;
        }
    }
    fclose(fp);
    qsort(products, count, sizeof(struct Product), compareProductsByID);
    return count;
}

void saveProductsArray(struct Product products[], int n)
{
    FILE *fp = fopen("products.csv", "w");
    if (fp != NULL)
    {
        fprintf(fp, "ProductID,Name,Category,Price,Quantity,MinStock,RestockQty,StockAlert\n");
        for (int i = 0; i < n; i++)
        {
            if (strlen(products[i].stockAlert) == 0) strcpy(products[i].stockAlert, "-");
            fprintf(fp, "%s,%s,%s,%.2f,%d,%d,%d,%s\n",
                    products[i].id, products[i].name, products[i].category,
                    products[i].price, products[i].quantity, products[i].minStock,
                    products[i].restockQty, products[i].stockAlert);
        }
        fclose(fp);
    }
}
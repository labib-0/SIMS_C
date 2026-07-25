#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    char id[10];
    char name[50];
    char role[20];
    char dob[15];
    char email[60];
    unsigned long password;
};

void clearScreen();
void pressEnter();
void delay();
void logAction(const char *userId, const char *action);
void createFile();
void generateID(char prefix, char id[]);
unsigned long getPassword();
unsigned long hashPassword(char password[]);
int validateEmail(char email[]);

int binarySearchUser(struct User users[], int n, const char *targetId);
int loadAndSortUsers(struct User users[], int maxUsers);
void saveUsersArray(struct User users[], int n);

void addProduct(const char *userId);
void updateProduct(const char *userId);
void deleteProduct(const char *userId);
void viewProducts();
void searchProducts();

void updateStock(const char *userId);
void manageLowStock(const char *userId);

void processSale(const char *userId);
void viewTransactions();

void generateInventoryReport(const char *userId);
void generateSalesReport(const char *userId);
void generatePerformanceReport(const char *userId);
void viewAuditLog(const char *userId);
void viewAndEditProfile(const char *userId);

void addUser(char prefix, char role[])
{
    FILE *fp;
    struct User user;
    strcpy(user.role, role);

    createFile(); 

    clearScreen();
    printf("=========================================\n");
    printf("              ADD %s\n", role);
    printf("=========================================\n\n");

    while(getchar()!='\n');

    printf("Full Name : ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    printf("Date of Birth (DD/MM/YYYY) : ");
    fgets(user.dob, sizeof(user.dob), stdin);
    user.dob[strcspn(user.dob, "\n")] = '\0';

    while (1)
    {
        printf("Email Address : ");
        fgets(user.email, sizeof(user.email), stdin);
        user.email[strcspn(user.email, "\n")] = '\0';

        if (validateEmail(user.email)) break;
        printf("Invalid Email format! Must contain '@' and '.' within valid positions (e.g. user@domain.com)\n\n");
    }

    user.password = getPassword();
    generateID(prefix, user.id);

    fp = fopen("users.csv", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%lu\n", user.id, user.name, user.role, user.dob, user.email, user.password);
        fclose(fp);
    }

    char logMsg[100];
    sprintf(logMsg, "Created %s account %s (%s)", role, user.id, user.name);
    logAction(user.id, logMsg);

    clearScreen();
    printf("=========================================\n");
    printf("      %s ADDED SUCCESSFULLY\n", role);
    printf("=========================================\n\n");
    printf("User ID   : %s\n", user.id);
    printf("Full Name : %s\n", user.name);
    printf("Email     : %s\n", user.email);

    pressEnter();
}

void addUserMenu()
{
    clearScreen();
    printf("=========================================\n");
    printf("                ADD USER\n");
    printf("=========================================\n\n");
    printf("1. Add Admin\n");
    printf("2. Add Store Manager\n");
    printf("3. Add Sales Staff\n");
    printf("4. Back\n");
    printf("\nEnter Choice : ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("\nInvalid Input!\n");
        pressEnter();
        return;
    }

    switch (choice)
    {
        case 1: addUser('A', "Admin"); break;
        case 2: addUser('M', "Store Manager"); break;
        case 3: addUser('S', "Sales Staff"); break;
        case 4: return;
        default:
            printf("\nInvalid Choice!\n");
            pressEnter();
    }
}

void deleteUserAny()
{
    struct User users[500];
    int count = loadAndSortUsers(users, 500);

    char id[10];
    char password[20];
    int adminCount = 0;

    clearScreen();
    printf("=========================================\n");
    printf("               DELETE USER\n");
    printf("=========================================\n\n");

    printf("Enter User ID to Delete : ");
    scanf("%9s", id);

    for (int i = 0; i < count; i++)
    {
        if (users[i].id[0] == 'A') adminCount++;
    }

    int idx = binarySearchUser(users, count, id);

    if (idx == -1)
    {
        printf("\nUser ID '%s' Not Found!\n", id);
        pressEnter();
        return;
    }

    if (users[idx].id[0] == 'A')
    {
        if (adminCount == 1)
        {
            printf("\nCannot Delete Last Admin!\n");
            pressEnter();
            return;
        }

        printf("Confirm Admin Password : ");
        scanf("%19s", password);
        if (hashPassword(password) != users[idx].password)
        {
            printf("\nIncorrect Password!\n");
            pressEnter();
            return;
        }
    }

    char role[20];
    strcpy(role, users[idx].role);

    for (int i = idx; i < count - 1; i++)
    {
        users[i] = users[i + 1];
    }
    count--;

    saveUsersArray(users, count);

    char logMsg[100];
    sprintf(logMsg, "Deleted %s account %s", role, id);
    logAction(id, logMsg);

    printf("\nUser %s (%s) Deleted Successfully.\n", id, role);
    pressEnter();
}

void viewAllUsers()
{
    struct User users[500];
    int count = loadAndSortUsers(users, 500);

    clearScreen();
    printf("====================================================================================================\n");
    printf("                                         ALL USERS LIST\n");
    printf("====================================================================================================\n\n");

    printf("%-5s %-10s %-25s %-18s %-15s %-25s\n", "No.", "User ID", "Full Name", "Role", "DOB", "Email");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-10s %-25s %-18s %-15s %-25s\n",
               i + 1, users[i].id, users[i].name, users[i].role, users[i].dob, users[i].email);
    }

    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Total Registered Users: %d\n", count);
    pressEnter();
}

void userManagementMenu(const char *userId)
{
    int choice;
    while(1)
    {
        clearScreen();
        printf("=========================================\n");
        printf("             USER MANAGEMENT\n");
        printf("=========================================\n\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. View All Users\n");
        printf("4. Back to Dashboard\n");
        printf("\nEnter Choice : ");

        if(scanf("%d", &choice) != 1)
        {
            while(getchar() != '\n');
            printf("\nInvalid Input!\n");
            pressEnter();
            continue;
        }

        switch(choice)
        {
            case 1: addUserMenu(); break;
            case 2: deleteUserAny(); break;
            case 3: viewAllUsers(); break;
            case 4: return;
            default:
                printf("\nInvalid Choice!\n");
                pressEnter();
        }
    }
}

void stockManagementMenu(const char *userId)
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("=========================================\n");
        printf("            STOCK MANAGEMENT\n");
        printf("=========================================\n\n");
        printf("1. Restock Product\n");
        printf("2. Manage Low Stock\n");
        printf("0. Back\n");
        printf("\nEnter Choice : ");

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
                updateStock(userId);
                break;
            case 2:
                manageLowStock(userId);
                break;
            case 0:
                return;
            default:
                printf("\nInvalid Choice!\n");
                pressEnter();
        }
    }
}

void backupData(const char *userId)
{
    clearScreen();
    printf("=========================================\n");
    printf("            BACKUP SYSTEM DATA\n");
    printf("=========================================\n\n");

#ifdef _WIN32
    system("if not exist backups mkdir backups");
    system("copy /y users.csv backups\\ 2>nul");
    system("copy /y products.csv backups\\ 2>nul");
    system("copy /y transactions.csv backups\\ 2>nul");
    system("copy /y audit_log.txt backups\\ 2>nul");
#else
    system("mkdir -p backups");
    system("cp users.csv backups/ 2>/dev/null");
    system("cp products.csv backups/ 2>/dev/null");
    system("cp transactions.csv backups/ 2>/dev/null");
    system("cp audit_log.txt backups/ 2>/dev/null");
#endif

    printf("Data backup completed successfully!\n");
    printf("All active database files copied to 'backups/' directory.\n");

    logAction(userId, "Created complete data backup");
    pressEnter();
}

void restoreData(const char *userId)
{
    clearScreen();
    printf("=========================================\n");
    printf("           RESTORE SYSTEM DATA\n");
    printf("=========================================\n\n");

    printf("WARNING: Restoring backup will overwrite current system database files!\n");
    printf("Are you sure you want to proceed? (1 = Yes / 0 = No) : ");

    int confirm = 0;
    if (scanf("%d", &confirm) != 1 || confirm != 1)
    {
        printf("\nRestore operation cancelled.\n");
        pressEnter();
        return;
    }

#ifdef _WIN32
    system("copy /y backups\\users.csv . 2>nul");
    system("copy /y backups\\products.csv . 2>nul");
    system("copy /y backups\\transactions.csv . 2>nul");
    system("copy /y backups\\audit_log.txt . 2>nul");
#else
    system("cp backups/users.csv . 2>/dev/null");
    system("cp backups/products.csv . 2>/dev/null");
    system("cp backups/transactions.csv . 2>/dev/null");
    system("cp backups/audit_log.txt . 2>/dev/null");
#endif

    printf("\nSystem data restored successfully from 'backups/' directory!\n");
    logAction(userId, "Restored system data from backups");
    pressEnter();
}

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
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) addProduct(userId);
                        else if (subChoice == 2) updateProduct(userId);
                        else if (subChoice == 3) deleteProduct(userId);
                        else if (subChoice == 4) viewProducts();
                        else if (subChoice == 5) searchProducts();
                    }
                }
                break;
            case 3:
                stockManagementMenu(userId);
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
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) generateInventoryReport(userId);
                        else if (subChoice == 2) generateSalesReport(userId);
                        else if (subChoice == 3) generatePerformanceReport(userId);
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
                    printf("Choice: ");
                    if (scanf("%d", &subChoice) == 1)
                    {
                        if (subChoice == 1) backupData(userId);
                        else if (subChoice == 2) restoreData(userId);
                    }
                }
                break;
            case 8:
                viewAuditLog(userId);
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

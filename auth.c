#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
void skipHeader(FILE *);
void logAction(const char *userId, const char *action);

int validateEmail(char email[]);
int binarySearchUser(struct User users[], int n, const char *targetId);
int loadAndSortUsers(struct User users[], int maxUsers);
void saveUsersArray(struct User users[], int n);
void roleMenu(char rolePrefix, char userId[], char userName[]);

unsigned long getPassword();
int validatePassword(char[]);
unsigned long hashPassword(char[]);
void generateID(char prefix, char id[]);

int validatePassword(char password[])
{
    int upper=0, lower=0, digit=0;

    if(strlen(password) < 6)
        return 0;

    for(int i=0; password[i]!='\0'; i++)
    {
        if(isupper(password[i])) upper=1;
        if(islower(password[i])) lower=1;
        if(isdigit(password[i])) digit=1;
    }

    return upper && lower && digit;
}

unsigned long hashPassword(char password[])
{
    unsigned long hash=0;
    for(int i=0; password[i]!='\0'; i++)
        hash = hash * 31 + password[i];
    return hash;
}

unsigned long getPassword()
{
    char password[20];
    while(1)
    {
        printf("Password (Min 6 Chars, 1 Upper, 1 Lower, 1 Digit) : ");
        scanf("%19s",password);

        if(validatePassword(password))
            return hashPassword(password);

        printf("\nInvalid Password!\n");
        printf("Must Contain:\n");
        printf("- At least 6 Characters\n");
        printf("- At least One Uppercase letter\n");
        printf("- At least One Lowercase letter\n");
        printf("- At least One Digit\n\n");
    }
}

void createFile()
{
    FILE *fp = fopen("users.csv", "r");

    if (fp == NULL)
    {
        fp = fopen("users.csv", "w");
        if(fp != NULL)
        {
            fprintf(fp, "UserID,FullName,Role,DOB,Email,Password\n");
            fprintf(fp, "A100,Default Admin,Admin,01/01/2000,admin@sims.com,%lu\n", hashPassword("Admin123"));
            fclose(fp);
        }
    }
    else 
    {
        skipHeader(fp);
        struct User u;
        int hasAdmin = 0;
        while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lu\n", u.id, u.name, u.role, u.dob, u.email, &u.password) == 6)
        {
            if(u.id[0] == 'A')
            {
                hasAdmin = 1;
                break;
            }
        }
        fclose(fp);

        if (!hasAdmin)
        {
            fp = fopen("users.csv", "a");
            if (fp != NULL)
            {
                fprintf(fp, "A100,Default Admin,Admin,01/01/2000,admin@sims.com,%lu\n", hashPassword("Admin123"));
                fclose(fp);
            }
        }
    }
}

void generateID(char prefix, char id[])
{
    FILE *fp;
    struct User user;
    int max=99;
    int number;

    fp=fopen("users.csv","r");
    if(fp==NULL)
    {
        sprintf(id,"%c100",prefix);
        return;
    }

    skipHeader(fp);

    while(fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lu\n",
                 user.id, user.name, user.role, user.dob, user.email, &user.password)==6)
    {
        if(user.id[0]==prefix)
        {
            number=atoi(user.id+1);
            if(number>max) max=number;
        }
    }
    fclose(fp);
    sprintf(id,"%c%d",prefix,max+1);
}

void registerAdmin()
{
    FILE *fp;
    struct User user;
    strcpy(user.role, "Admin");

    createFile(); 

    clearScreen();
    printf("=========================================\n");
    printf("            REGISTER ADMIN\n");
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
        printf("Invalid Email format! Must contain '@' and '.' within valid positions (e.g. admin@sims.com)\n\n");
    }

    user.password = getPassword();
    generateID('A', user.id);

    fp = fopen("users.csv", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%lu\n", user.id, user.name, user.role, user.dob, user.email, user.password);
        fclose(fp);
    }

    logAction(user.id, "Registered new Admin account");

    clearScreen();
    printf("=========================================\n");
    printf("      ADMIN REGISTERED SUCCESSFULLY\n");
    printf("=========================================\n\n");
    printf("User ID   : %s\n", user.id);
    printf("Full Name : %s\n", user.name);
    printf("Email     : %s\n", user.email);

    pressEnter();
}

void forgotPassword()
{
    struct User users[500];
    int count = loadAndSortUsers(users, 500);

    char id[10];
    char name[50];
    char dob[15];
    char email[60];

    clearScreen();
    printf("=========================================\n");
    printf("            FORGOT PASSWORD\n");
    printf("=========================================\n\n");

    printf("Enter User ID                  : ");
    scanf("%9s", id);
    while (getchar() != '\n');

    printf("Enter Full Name                : ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Date of Birth (DD/MM/YYYY): ");
    fgets(dob, sizeof(dob), stdin);
    dob[strcspn(dob, "\n")] = '\0';

    printf("Enter Email Address            : ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    int idx = binarySearchUser(users, count, id);

    if (idx != -1 &&
        strcasecmp(users[idx].name, name) == 0 &&
        strcmp(users[idx].dob, dob) == 0 &&
        strcasecmp(users[idx].email, email) == 0)
    {
        printf("\nIdentity Verified Successfully!\n\nCreate a New ");
        users[idx].password = getPassword();

        saveUsersArray(users, count);

        logAction(id, "Reset password via verification (Name, DOB, Email)");
        printf("\nPassword updated successfully. You can now log in.\n");
    }
    else
    {
        printf("\nVerification Failed! Details (User ID, Name, DOB, Email) do not match our records.\n");
    }
    pressEnter();
}

void changePassword(const char *userId)
{
    struct User users[500];
    int count = loadAndSortUsers(users, 500);

    char currentPass[20];

    clearScreen();
    printf("=========================================\n");
    printf("             CHANGE PASSWORD\n");
    printf("=========================================\n\n");

    printf("Enter Current Password : ");
    scanf("%19s", currentPass);

    int idx = binarySearchUser(users, count, userId);

    if (idx != -1)
    {
        if (hashPassword(currentPass) == users[idx].password)
        {
            printf("\nCurrent Password Verified!\n\nEnter New ");
            users[idx].password = getPassword();

            saveUsersArray(users, count);

            logAction(userId, "Changed password successfully");
            printf("\nPassword changed successfully!\n");
        }
        else
        {
            printf("\nIncorrect Current Password!\n");
        }
    }
    else
    {
        printf("\nUser session not found.\n");
    }
    pressEnter();
}

void viewAndEditProfile(const char *userId)
{
    struct User users[500];

    while (1)
    {
        int count = loadAndSortUsers(users, 500);
        int idx = binarySearchUser(users, count, userId);

        if (idx == -1)
        {
            printf("\nUser session not found!\n");
            pressEnter();
            return;
        }

        clearScreen();
        printf("=========================================\n");
        printf("               MY PROFILE\n");
        printf("=========================================\n\n");
        printf("User ID       : %s\n", users[idx].id);
        printf("Full Name     : %s\n", users[idx].name);
        printf("Role          : %s\n", users[idx].role);
        printf("Date of Birth : %s\n", users[idx].dob);
        printf("Email Address : %s\n", users[idx].email);
        printf("\n=========================================\n");
        printf("1. Edit Full Name\n");
        printf("2. Edit Date of Birth\n");
        printf("3. Edit Email Address\n");
        printf("4. Change Password\n");
        printf("5. Back to Dashboard\n");
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

        if (choice == 5) return;

        if (choice == 1)
        {
            printf("\nEnter New Full Name : ");
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if (strlen(newName) > 0)
            {
                strcpy(users[idx].name, newName);
                saveUsersArray(users, count);
                logAction(userId, "Updated profile full name");
                printf("\nFull Name updated successfully!\n");
            }
            pressEnter();
        }
        else if (choice == 2)
        {
            printf("\nEnter New Date of Birth (DD/MM/YYYY) : ");
            char newDob[15];
            fgets(newDob, sizeof(newDob), stdin);
            newDob[strcspn(newDob, "\n")] = '\0';
            if (strlen(newDob) > 0)
            {
                strcpy(users[idx].dob, newDob);
                saveUsersArray(users, count);
                logAction(userId, "Updated profile date of birth");
                printf("\nDate of Birth updated successfully!\n");
            }
            pressEnter();
        }
        else if (choice == 3)
        {
            char newEmail[60];
            while (1)
            {
                printf("\nEnter New Email Address : ");
                fgets(newEmail, sizeof(newEmail), stdin);
                newEmail[strcspn(newEmail, "\n")] = '\0';

                if (validateEmail(newEmail))
                {
                    strcpy(users[idx].email, newEmail);
                    saveUsersArray(users, count);
                    logAction(userId, "Updated profile email address");
                    printf("\nEmail Address updated successfully!\n");
                    break;
                }
                else
                {
                    printf("Invalid Email format! Example: user@domain.com\n");
                }
            }
            pressEnter();
        }
        else if (choice == 4)
        {
            changePassword(userId);
        }
        else
        {
            printf("\nInvalid Choice!\n");
            pressEnter();
        }
    }
}

void login()
{
    struct User users[500];
    char id[10];
    char password[20];
    int attempts = 0;

    createFile();

    while (1)
    {
        int count = loadAndSortUsers(users, 500);

        clearScreen();
        printf("=========================================\n");
        printf("                 LOGIN\n");
        printf("=========================================\n\n");
        printf("Attempt %d of 3\n\n", attempts + 1);

        printf("User ID  : ");
        scanf("%9s",id);

        printf("Password : ");
        scanf("%19s",password);

        int idx = binarySearchUser(users, count, id);

        if (idx != -1 && hashPassword(password) == users[idx].password)
        {
            logAction(users[idx].id, "Logged in successfully");

            clearScreen();
            printf("=========================================\n");
            printf("         LOGIN SUCCESSFUL\n");
            printf("=========================================\n\n");
            printf("Welcome, %s (%s)\n", users[idx].name, users[idx].role);
            printf("\nLoading Dashboard...\n");

            delay();
            
            roleMenu(users[idx].id[0], users[idx].id, users[idx].name);
            return; 
        }
        else
        {
            attempts++;
            printf("\nInvalid User ID or Password!\n");
            
            if (attempts >= 3)
            {
                printf("\nMaximum login attempts reached.\n\n");
                printf("1. Forgot Password\n");
                printf("2. Try Again\n");
                printf("Choice: ");
                
                int choice;
                if (scanf("%d", &choice) != 1) {
                    while(getchar() != '\n');
                }

                if (choice == 1) {
                    forgotPassword();
                    attempts = 0;
                } else {
                    attempts = 0;
                }
            } else {
                pressEnter();
            }
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define CONFIG_PATH "./src/.config"
#define TAB_SPACE_CNT 4
#define DEFAULT_PORT 8888

typedef enum Role
{
    USER,
    ADMIN,
    VISITOR
} Role;

typedef struct User
{
    char uuid[16];
    char username[32];
    int score;
    int port;
    Role role;
} User;

void printHelpCommands();
int userExists(const char *username);
int validatePassword(const char *password);
int createUser(const char *username, const char *password, int role, int port);

int initUser(User *user, const char *username, Role role, int port);
Role strToRole(const char *role);

int signup();
int login(User *user, int argc, const char **argv);

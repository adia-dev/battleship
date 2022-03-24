#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CONFIG_PATH "./temp"
#define TAB_SPACE_CNT 4
#define DEFAULT_PORT 8888

enum role
{
    USER,
    ADMIN,
    VISITOR
};

int userExists(const char *username);
int validatePassword(const char *password);
int createUser(const char *username, const char *password, int role, int port);
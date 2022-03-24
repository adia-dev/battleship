#include "config_helper.h"

int userExists(const char *username)
{
    if (username == NULL)
        return 0;
    FILE *f;

    f = fopen(CONFIG_PATH, "rt");

    if (f != NULL)
    {
        char line[1000];

        int readingUsers = 0;
        int readingUser = 0;

        while ((fgets(line, 1000, f)) != NULL)
        {
            size_t length = strlen(line);

            if (line[length - 1] == '\n')
                line[length - 1] = '\0';

            if (readingUser)
            {

                if (strncmp(line + TAB_SPACE_CNT * 2, "USERNAME=", 9) == 0 && strcmp(username, line + TAB_SPACE_CNT * 2 + 9) == 0)
                {
                    return 1;
                }
            }

            if (readingUser && (strncmp(line, "        ", 8) != 0))
            {
                readingUser = 0;
            }

            if (readingUsers)
            {
                if (strcmp(line + TAB_SPACE_CNT, "[USER]") == 0)
                {
                    readingUser = 1;
                }
            }

            if (strcmp(line, "[USERS]") == 0)
            {
                readingUsers = 1;
            }
        }
        fclose(f);
    }

    return 0;
}

int validatePassword(const char *password)
{
    if (password == NULL)
        return 0;

    size_t password_len = strlen(password);

    return password_len >= 8 && password_len <= 16 && strpbrk(password, "!#$%&'()*+,-./:;<=>?@[\\]^_`{|}~") != NULL && strpbrk(password, "0123456789") != NULL;
}

int createUser(const char *username, const char *password, int role, int port)
{
    if (username == NULL || password == NULL || port < 1000 || port > 9999)
    {
        printf("Unable to create a new user with these parameters.\n");
        return 0;
    }

    FILE *f;

    f = fopen("./temp", "wb");

    if (f != NULL)
    {

        char roleStr[1000];

        switch (role)
        {
        case USER:
            strcpy(roleStr, "USER");
            break;
        case ADMIN:
            strcpy(roleStr, "ADMIN");
            break;
        case VISITOR:
            strcpy(roleStr, "VISITOR");
            break;

        default:
            strcpy(roleStr, "UNDEFINED");
            break;
        }

        fprintf(f, "[USERS]\n");
        fprintf(f, "\t[USER]\n");
        fprintf(f, "\t\tUSERNAME=%s\n", username);
        fprintf(f, "\t\tPASSWORD=%s\n", password);
        fprintf(f, "\t\tROLE=%s\n", roleStr);
        fprintf(f, "\t\tPORT=%d\n", port);

        fclose(f);
    }

    return 1;
}

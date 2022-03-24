#include "config_helper.h"

int main(int argc, char const *argv[])
{

    if (argc == 2 && (strcmp(*(argv + 1), "-r") == 0 || strcmp(*(argv + 1), "--register") == 0))
    {
        printf("\t == Register ==\n");

        char username[1000] = {0};
        char password[17] = {0};

        while (1)
        {
            printf("username: ");
            fgets(username, 1000, stdin);
            size_t length = strlen(username);

            if (username[length - 1] == '\n')
                username[length - 1] = '\0';
            fflush(stdin);

            if (!userExists(username))
            {
            password:
                printf("The password has to be between 8 and 16 characters and has to contain at least a special character and a digit.\n");
                printf("password: ");
                fgets(password, 1000, stdin);
                size_t length = strlen(password);

                if (password[length - 1] == '\n')
                    password[length - 1] = '\0';
                fflush(stdin);

                if (validatePassword(password))
                {
                    printf("Creating new user: %s\n", username);
                    createUser(username, password, USER, DEFAULT_PORT);
                }
                else
                {
                    goto password;
                }

                break;
            }
            else
            {
                printf("This username is not available.\n");
            }
        }

        return EXIT_SUCCESS;
    }
    else if (argc < 5)
    {
        printf("Available Commands:\n");
        printf("\t-h, --help\tHelp\n");
        printf("\t-r, --register\tRegister an new account\n");
        printf("\t-u, --username\tUsername of the account\n");
        printf("\t-p, --port\tPassword of the account\n");

        return EXIT_FAILURE;
    }
    else
    {

        char log_username[1000];
        char log_password[1000];
        char log_port[1000];
        int logged_in = 0;

        for (size_t i = 0; i < argc - 1; i++)
        {
            if (strcmp(*(argv + i), "-u") == 0 || strcmp(*(argv + i), "--username") == 0)
            {
                strcpy(log_username, *(argv + i + 1));
            }
            else if (strcmp(*(argv + i), "-p") == 0 || strcmp(*(argv + i), "--password") == 0)
            {
                strcpy(log_password, *(argv + i + 1));
            }
            else if (strcmp(*(argv + i), "--port") == 0)
            {
                strcpy(log_port, *(argv + i + 1));
            }
        }

        FILE *f;

        f = fopen("./login", "rt");
        char username[1000] = {0};
        char password[1000] = {0};
        char port[1000] = {0};
        char role[1000] = {0};

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

                    if (strncmp(line + TAB_SPACE_CNT * 2, "USERNAME=", 9) == 0)
                    {
                        strcpy(username, line + TAB_SPACE_CNT * 2 + 9);
                        // printf("Username found: %s\n", username);
                    }
                    else if (strncmp(line + TAB_SPACE_CNT * 2, "PASSWORD=", 9) == 0)
                    {
                        strcpy(password, line + TAB_SPACE_CNT * 2 + 9);
                        // printf("Password found: %s\n", password);
                    }
                    else if (strncmp(line + TAB_SPACE_CNT * 2, "PORT=", 5) == 0)
                    {
                        strcpy(port, line + TAB_SPACE_CNT * 2 + 5);
                        // printf("Port found: %s\n", port);
                    }
                    else if (strncmp(line + TAB_SPACE_CNT * 2, "ROLE=", 5) == 0)
                    {
                        strcpy(role, line + TAB_SPACE_CNT * 2 + 5);
                        // printf("Role found: %s\n", role);
                    }
                }

                if (readingUser && (strncmp(line, "        ", 8) != 0))
                {
                    readingUser = 0;

                    if (strcmp(log_username, username) == 0 && strcmp(log_password, password) == 0)
                    {
                        printf("\t == Successfully logged in as %s (%s) ==\n", username, role);
                        logged_in = 1;
                        fclose(f);
                        break;
                    }
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

        if (!logged_in)
        {
            printf("wrong username or password.\n");
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}
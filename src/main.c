#include "config_helper.h"
#include "client.h"

int main(int argc, char const *argv[])
{
    if (argc == 2 && (strcmp(*(argv + 1), "-r") == 0 || strcmp(*(argv + 1), "--register") == 0))
    {

        if (signup() != -1)
            return EXIT_SUCCESS;
    }
    else if (argc < 5)
    {
        printHelpCommands();

        return EXIT_FAILURE;
    }
    else
    {
        User user;
        if (login(&user, argc, argv) == -1)
            return EXIT_SUCCESS;

        switch (user.role)
        {
        case ADMIN:
            startServer(PORT);
            break;

        default:
            connectToServer(ADDRESS, PORT);
            break;
        }
    }

    return EXIT_SUCCESS;
}

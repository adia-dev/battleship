#include "game_manager.h"

int play()
{

    srand(time(NULL));

    int isPlaying = 1;
    int firstPlayerTurn = 1;
    size_t width = 10;
    size_t height = 10;
    Board b = board_create(width, height);
    BoardCoords bombs[10];

    int x, y;

    printf("Coordonées des bombes:\n");
    for (size_t i = 0; i < sizeof(bombs) / sizeof(BoardCoords); i++)
    {
        bombs[i].x = rand() % (height - 1) + 1;
        bombs[i].y = rand() % (height - 1) + 1;
        printf("\t%zu - x: %d, y: %d, content: %s\n", i, bombs[i].x, bombs[i].y, "💥");
    }

    printf("=== Game Started ===\n\n");

    while (isPlaying)
    {
        board_render(&b);

        fflush(stdin);
        printf("Player turn: %s\n", (firstPlayerTurn ? "🔴" : "🟡"));
        printf("Coordonnées: \nx: ");
        scanf("%d", &x);
        if (x <= 0)
            break;
        fflush(stdin);
        printf("y: ");
        scanf("%d", &y);
        if (y <= 0)
            break;

               for (size_t i = 0; i < sizeof(bombs) / sizeof(BoardCoords); i++)
        {
            if (bombs[i].x == x && bombs[i].y == y)
            {
                strcpy(b.boardCoords[y][x].content, "💥");
                printf("\nBOOM !!!\nPlay again !\n");
                firstPlayerTurn = !firstPlayerTurn;
                break;
            }

            if (i == 2)
            {
                printf("\nMissed ...\n");
                if (firstPlayerTurn)
                    strcpy(b.boardCoords[y][x].content, "🔴");
                else
                    strcpy(b.boardCoords[y][x].content, "🟡");
            }
        }

        firstPlayerTurn = !firstPlayerTurn;
    }
    printf("\n\n=== Game Ended ===\n\n\n");
    board_clean(&b);

    return 1;
}

#include "game_manager.h"

int play()
{

    srand(time(NULL));

    int isPlaying = 1;
    int firstPlayerTurn = 1;
    Board b = board_create(10, 10);
    BoardCoords bombs[] = {
        {.x = rand() % 10 + 1,
         .y = rand() % 10 + 1},
        {.x = rand() % 10 + 1,
         .y = rand() % 10 + 1},
        {.x = rand() % 10 + 1,
         .y = rand() % 10 + 1}};

    int x, y;

    printf("Coordonées des bombes:\n");
    for (size_t i = 0; i < 3; i++)
    {
        printf("\t%zu - x: %d, y: %d, content: %s\n", i, bombs[i].x, bombs[i].y, "💥");
    }

    printf("=== Game Started ===\n\n");

    while (isPlaying)
    {
        board_render(&b);

        fflush(stdin);
        printf("Coordonnées: \nx: ");
        scanf("%d", &x);
        fflush(stdin);
        printf("y: ");
        scanf("%d", &y);

        for (size_t i = 0; i < 3; i++)
        {
            if (bombs[i].x == x && bombs[i].y == y)
            {
                strcpy(b.boardCoords[y][x].content, "💥");
                printf("\nBOOM !!!\n");
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

        if (x == 0 || y == 0)
            isPlaying = 0;
        firstPlayerTurn = !firstPlayerTurn;
    }
    printf("\n\n=== Game Ended ===\n\n\n");
    board_clean(&b);

    return 1;
}

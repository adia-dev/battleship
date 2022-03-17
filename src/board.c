#include "board.h"

// const char HIT = '✖';
// const char EMPTY = '□';

Board board_create(size_t width, size_t height)
{
    BoardCoords **coords;
    coords = (BoardCoords **)(malloc(sizeof(BoardCoords) * height));

    for (size_t i = 0; i < height; i++)
    {
        coords[i] = (BoardCoords *)(malloc(sizeof(BoardCoords) * width));
    }

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            strcpy(coords[i][j].content, "🔲");
        }
    }

    Board newBoard = {
        .boardCoords = coords,
        .height = height,
        .width = width};

    return newBoard;
}

void board_clean(Board *b)
{
    if (b == NULL)
        return;

    for (size_t i = 0; i < b->height; i++)
    {
        free(b->boardCoords[i]);
    }

    free(b->boardCoords);
}

void board_render(const Board *b)
{
    if (b == NULL)
        return;

    for (size_t i = 0; i < b->height; i++)
    {

        for (size_t j = 0; j < b->width; j++)
        {
            if (i + j == 0)
            {
                printf("   ");
            }
            else if (i == 0)
            {
                printf(" %c  ", (char)('A' + (j - 1)));
            }
            else if (j == 0)
            {

                printf(" %zu ", i);
            }
            else
                printCharacter(b->boardCoords[i][j].content);
        }
        printf("\n");
    }
}

void printCharacter(const char *content)
{
    printf(" %s ", content);
}

#include "ship.h"

typedef struct BoardCoords
{
    int x, y;
    char content[10];
} BoardCoords;

typedef struct Board
{
    size_t width, height;
    BoardCoords **boardCoords;
} Board;

Board board_create(size_t width, size_t height);
void board_clean(Board *b);

void board_render(const Board *b);
void printCharacter(const char *content);
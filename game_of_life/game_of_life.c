#include "game_of_life.h"

#include <stdio.h>
#include <stdlib.h>

int create_game_of_life(int width, int height, GameOfLife* dest) {
    if (width <= 0 || height <= 0) {
        return INVALID_BOARD_SIZE;
    }

    GameOfLife game_of_life = {.width = width, .height = height};
    game_of_life.tiles = (Tile*)malloc(width * height * sizeof(Tile));

    printf("%p", &game_of_life);
    for (int i = 0; i < width * height; i++) {
        game_of_life.tiles[i].alive = 0;
        game_of_life.tiles[i].hovered = 0;
    }

    *dest = game_of_life;

    return OK;
}

Tile* get_tile(GameOfLife* game, int x, int y) {
    return &game->tiles[x + y * game->width];
}

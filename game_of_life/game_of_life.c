#include "game_of_life.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Creates a new game with the given dimensions.
 * @param width The width of the board.
 * @param height The height of the board.
 * @param dest Where to store the created board.
 * @return Status code of the creation. OK if no errors, INVALID_BOARD_SIZE if
 * dimensions are less than 1.
 */
int create_game_of_life(unsigned int width, unsigned int height,
                        GameOfLife* dest) {
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

int valid_coordinate(GameOfLife* game, int x, int y) {
    int width = game->width;
    int height = game->height;

    return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * @brief Gets a reference to a tile at the given coordinates.
 * @param game The game to get the tile from.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A reference to the tile. NULL if dimensions are outside board range.
 */
Tile* get_tile(GameOfLife* game, unsigned int x, unsigned int y) {
    if (!valid_coordinate(game, x, y)) {
        return NULL;
    }

    return &game->tiles[x + y * game->width];
}

void flip_tile_status(Tile* tile) { tile->alive = !tile->alive; }

int get_alive_neighbour_count(GameOfLife* game, unsigned int x,
                              unsigned int y) {
    if (!valid_coordinate(game, x, y)) {
        return INVALID_BOARD_SIZE;
    }

    int width = game->width;
    int height = game->height;
    int alive_count = 0;

    /*
     * Based on this diagram:
     * [
            [(x-1, y-1), (x, y-1), (x+1, y-1)],
            [(x-1, y), (tile to check), (x+1, y)],
            [(x-1, y+1), (x, y+1), (x-+1, y+1)]
        ]
    */
    for (int x_offset = -1; x_offset <= 1; x_offset++) {
        for (int y_offset = -1; y_offset <= 1; y_offset++) {
            // Skip the cell being checked for neighbours
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }

            int tile_x = x + x_offset;
            int tile_y = y + y_offset;

            // If coordinate is not valid it means we're dealing with a border
            // tile so outside ranges should be treated as dead.
            if (valid_coordinate(game, tile_x, tile_y) &&
                get_tile(game, tile_x, tile_y)->alive) {
                alive_count++;
            }
        }
    }

    return alive_count;
}

void print_game(GameOfLife* game) {
    printf("\n");
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            printf("%d ", get_tile(game, x, y)->alive);
        }
        printf("\n");
    }
}

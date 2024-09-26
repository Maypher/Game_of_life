#pragma once

#define OK 0
#define INVALID_BOARD_SIZE -1

typedef struct {
    int alive;
    int hovered;
} Tile;

typedef struct {
    int width;
    int height;
    Tile* tiles;
} GameOfLife;

/**
 * @brief Creates a game of life board given the proper
 *
 *
 * @param width Widht of the board.
 * @param height Height of the board.
 * @param dest Where to store the board after creation.
 * @return Status code. OK if board was created successfully. INVALID_BOARD_SIZE
 * if dimension values are 0 or negative.
 */
int create_game_of_life(unsigned int width, unsigned int height,
                        GameOfLife* dest);

Tile* get_tile(GameOfLife* game, unsigned int x, unsigned int y);

/**
 * @brief changes the alive status of the given tile.
 * @param tile The tile to flip the status of.
 */
void flip_tile_status(Tile* tile);

/**
 * @brief Returns the amount of live neighbours for a given tile.
 * @param x The x coordinate of the tile to check.
 * @param The y coordinate of the tile to check.
 * @return The amount of live neighbours. INVALID_BOARD_SIZE if outside board
 * range.
 */
int get_alive_neighbour_count(GameOfLife* game, unsigned int x, unsigned int y);

/**
 * @brief Prints the given game to the console. Used for debugging.
 */
void print_game(GameOfLife* game);

/**
 * @brief Returns true if the given coordinates are valid for the given board.
 */
int valid_coordinate(GameOfLife* game, int x, int y);

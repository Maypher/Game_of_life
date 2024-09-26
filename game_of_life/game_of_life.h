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
int create_game_of_life(int width, int height, GameOfLife* dest);

Tile* get_tile(GameOfLife* game, int x, int y);

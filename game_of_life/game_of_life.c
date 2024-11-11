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
    game_of_life.cells = (Cell*)malloc(width * height * sizeof(Cell));
    game_of_life.cells_buffer = (Cell*)malloc(width * height * sizeof(Cell));

    for (int i = 0; i < width * height; i++) {
        game_of_life.cells[i].id = i;
        game_of_life.cells[i].alive = 0;
        game_of_life.cells[i].hovered = 0;

        game_of_life.cells_buffer[i].id = i;
        game_of_life.cells_buffer[i].alive = 0;
        game_of_life.cells_buffer[i].hovered = 0;
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
 * @brief Gets a reference to a cell at the given coordinates.
 * @param game The game to get the cell from.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return A reference to the cell. NULL if dimensions are outside board range.
 */
Cell* get_cell(GameOfLife* game, unsigned int x, unsigned int y) {
    if (!valid_coordinate(game, x, y)) {
        return NULL;
    }

    return &game->cells[x + y * game->width];
}

Cell* get_cell_from_buffer(GameOfLife* game, unsigned int x, unsigned int y) {
    if (!valid_coordinate(game, x, y)) {
        return NULL;
    }

    return &game->cells_buffer[x + y * game->width];
}

/**
 * @brief Returns the x coordinates of a cell given its index in the cell
 * array. Return INVALID_BOARD_SIZE if index > game->width * game->height
 */
int get_x_coordinate_by_index(GameOfLife* game, unsigned int index) {
    if (index < 0 || index > game->width * game->height) {
        return INVALID_BOARD_SIZE;
    }
    return index % game->width;
}

/**
 * @brief Returns the y coordinates of a cell given its index in the cell
 * array. Return INVALID_BOARD_SIZE if index > game->width * game->height
 */
int get_y_coordinate_by_index(GameOfLife* game, unsigned int index) {
    if (index < 0 || index > game->width * game->height) {
        return INVALID_BOARD_SIZE;
    }
    return index / game->height;
}

void flip_cell_status(Cell* cell) { cell->alive = !cell->alive; }

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
            [(x-1, y), (cell to check), (x+1, y)],
            [(x-1, y+1), (x, y+1), (x-+1, y+1)]
        ]
    */
    for (int x_offset = -1; x_offset <= 1; x_offset++) {
        for (int y_offset = -1; y_offset <= 1; y_offset++) {
            // Skip the cell being checked for neighbours
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }

            int cell_x = x + x_offset;
            int cell_y = y + y_offset;

            // If coordinate is not valid it means we're dealing with a border
            // cell so outside ranges should be treated as dead.
            if (valid_coordinate(game, cell_x, cell_y) &&
                get_cell(game, cell_x, cell_y)->alive) {
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
            printf("%d ", get_cell(game, x, y)->alive);
        }
        printf("\n");
    }
}

void tick_cell(GameOfLife* game, unsigned int x, unsigned int y) {
    Cell* cell = get_cell_from_buffer(game, x, y);
    if (!cell) {
        return;
    }

    int live_neighbours = get_alive_neighbour_count(game, x, y);

    if (cell->alive) {
        // Any live cell with fewer than two live neighbours dies, as if by
        // underpopulation.
        // Any live cell with more than three live neighbours dies, as if by
        // overpopulation.
        if (live_neighbours < 2 || live_neighbours > 3) {
            flip_cell_status(cell);
        }
    }
    // Any dead cell with exactly three live neighbours becomes a live cell, as
    // if by reproduction
    else if (!cell->alive && live_neighbours == 3) {
        flip_cell_status(cell);
    }
}

void swap_buffers(GameOfLife* game) {
    Cell* temp = game->cells;
    game->cells = game->cells_buffer;
    game->cells_buffer = game->cells;
}

void tick_game(GameOfLife* game) {
    for (int i = 0; i < game->width * game->height; i++) {
        int x = get_x_coordinate_by_index(game, i);
        int y = get_y_coordinate_by_index(game, i);

        tick_cell(game, x, y);
    }
    swap_buffers(game);
}

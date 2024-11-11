#pragma once

#define OK 0
#define INVALID_BOARD_SIZE -1

typedef struct {
    unsigned int id;
    unsigned int alive;
    unsigned int hovered;
} Cell;

typedef struct {
    int width;
    int height;
    Cell* cells;
    Cell* cells_buffer;
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
/**
 * @brief Gets a cell from the main buffer given its coordinates. Returns NULL
 * if coordinates are invalid.
 */
Cell* get_cell(GameOfLife* game, unsigned int x, unsigned int y);

/**
 * @brief Gets a cell from the swap buffer given its coordinates. Returns NULL
 * if coordinates are invalid.
 */
Cell* get_cell_from_buffer(GameOfLife* game, unsigned int x, unsigned int y);

/**
 * @brief Gets the x coordinate of a cell given its index in the array.
 */
int get_x_coordinate_by_index(GameOfLife* game, unsigned int index);

/**
 * @brief Gets the y coordinate of a cell given its index in the array.
 */
int get_y_coordinate_by_index(GameOfLife* game, unsigned int index);

/**
 * @brief changes the alive status of the given cell.
 * @param cell The cell to flip the status of.
 */
void flip_cell_status(Cell* cell);

/**
 * @brief Returns the amount of live neighbours for a given cell.
 * @param x The x coordinate of the cell to check.
 * @param The y coordinate of the cell to check.
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

/**
 * @brief Updates a cell's status based on the rules of Game of Life.
 */
void tick_cell(GameOfLife* game, unsigned int x, unsigned int y);

/**
 * @brief Ticks an entire board. Updating its cells based on the rules of Game
 * of Life.
 */
void tick_game(GameOfLife* game);

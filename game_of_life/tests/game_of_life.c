#include "game_of_life.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "unity.h"

void test_create_game_of_life() {
    int width = (rand() % 10) + 1;  // To make sure it doesn't throw out 0
    int height = (rand() % 10) + 1;

    GameOfLife game;
    create_game_of_life(width, height, &game);

    Tile *tiles = malloc(width * height * sizeof(Tile));

    for (int i = 0; i < width * height; i++) {
        tiles[i].alive = 0;
        tiles[i].hovered = 0;
    }

    GameOfLife true_game = {width, height, tiles = tiles};

    TEST_ASSERT_EQUAL_INT_MESSAGE(width, game.width,
                                  "Created game width doesn't match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(width, true_game.width,
                                  "True game width doesn't match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(height, game.height,
                                  "Created game height doesn't match");
    TEST_ASSERT_EQUAL_INT_MESSAGE(height, true_game.height,
                                  "True game height doesn't match");

    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(true_game.tiles, game.tiles,
                                        width * height, "Tiles don't match");

    free(tiles);
}

void test_create_game_invalid_dimensions() {
    int width = 0;  // To make sure it doesn't throw out 0
    int height = 0;

    GameOfLife game;
    int error = create_game_of_life(width, height, &game);

    TEST_ASSERT_EQUAL_INT8(INVALID_BOARD_SIZE, error);
}

void test_live_neighbour_count() {
    // Game dimensions
    int width = (rand() % 10) + 1;  // To stop size from being 0
    int height = (rand() % 10) + 1;

    // Coordinates of the tile to check for neighbours.
    int center_x = rand() % width;
    int center_y = rand() % height;

    GameOfLife game;
    create_game_of_life(width, height, &game);

    int tiles_flipped = 0;

    for (int x_offset = -1; x_offset <= 1; x_offset++) {
        for (int y_offset = -1; y_offset <= 1; y_offset++) {
            // Don't flip the tile being checked for neighbours
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }

            int tile_x = center_x + x_offset;
            int tile_y = center_y + y_offset;

            // Give a 50% change to flip a tile
            if (valid_coordinate(&game, tile_x, tile_y) && rand() % 2 == 1) {
                Tile *tile_to_flip = get_tile(&game, tile_x, tile_y);
                flip_tile_status(tile_to_flip);
                tiles_flipped++;
            }
        }
    }

    TEST_ASSERT_EQUAL_INT_MESSAGE(
        tiles_flipped, get_alive_neighbour_count(&game, center_x, center_y),
        "Live neighbours don't match");
}

void setUp() { srand(time(NULL)); }
void tearDown() {}

int main(int argc, char *argv[]) {
    UNITY_BEGIN();
    RUN_TEST(test_create_game_of_life);
    RUN_TEST(test_create_game_invalid_dimensions);
    RUN_TEST(test_live_neighbour_count);
    return UNITY_END();
}

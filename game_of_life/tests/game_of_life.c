#include "game_of_life.h"

#include <stdlib.h>

#include "unity.h"

void test_create_game_of_life() {
    int width = rand() + 1;  // To make sure it doesn't throw out 0
    int height = rand() + 1;

    GameOfLife game;
    create_game_of_life(width, height, &game);

    Tile *tiles = malloc(width * height * sizeof(Tile));

    for (int i = 0; i < width * height; i++) {
        tiles[i].alive = 0;
        tiles[i].hovered = 0;
    }

    GameOfLife true_game = {width, height, tiles = tiles};

    TEST_ASSERT_EQUAL_INT(width, game.width);
    TEST_ASSERT_EQUAL_INT(width, true_game.width);
    TEST_ASSERT_EQUAL_INT(height, game.height);

    TEST_ASSERT_EQUAL_INT(height, true_game.height);

    TEST_ASSERT_EQUAL_INT_ARRAY(true_game.tiles, game.tiles, width * height);

    free(tiles);
}

void test_create_game_invalid_dimensions() {
    int width = 0;  // To make sure it doesn't throw out 0
    int height = 0;

    GameOfLife game;
    int error = create_game_of_life(width, height, &game);

    TEST_ASSERT_EQUAL_INT8(INVALID_BOARD_SIZE, error);
}

void setUp() {}
void tearDown() {}

int main(int argc, char *argv[]) {
    UNITY_BEGIN();
    RUN_TEST(test_create_game_of_life);
    RUN_TEST(test_create_game_invalid_dimensions);
    return UNITY_END();
}

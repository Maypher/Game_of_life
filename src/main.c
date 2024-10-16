#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_of_life.h"
#include "glad/glad.h"
#include "glfw3/glfw3.h"
#include "opengl.h"
#include "scene.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(
            "Invalid amount of arguments passed. Should be game dimensions: x "
            "y\n");
        return -1;
    }

    int width = atoi(argv[1]);
    if (!width) {
        printf("Invalid argument type for width. Expected 'int' got '%s'",
               argv[1]);
        return -1;
    }

    int height = atoi(argv[2]);
    if (!height) {
        printf("Invalid argument type for height. Expected 'int' got '%s'",
               argv[2]);
        return -1;
    }

    GLFWwindow *window = init_opengl();

    Scene *scene = create_scene("./shaders/vertex.glsl",
                                "./shaders/fragment.glsl", width, height);

    GameOfLife game;
    create_game_of_life(width, height, &game);

    srand(time(NULL));

    for (int i = 0; i < width * height; i++) {
        double rand_val = (double)rand() / (double)RAND_MAX;

        if (rand_val < 0.5) {
            game.cells[i].alive = 1;
        }
    }

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        draw_game(scene, &game);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            printf("OpenGL error: %d\n", err);
        }
    }

    glfwTerminate();
    return 0;
}

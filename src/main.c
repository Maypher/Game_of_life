#include <glad/glad.h>
#include <stdio.h>

#include "game_of_life.h"
#include "glfw3/glfw3.h"
#include "opengl.h"
#include "scene.h"

int main(int argc, char *argv[]) {
    GLFWwindow *window = init_opengl();

    Scene *scene =
        create_scene("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    GameOfLife game;
    create_game_of_life(600, 800, &game);

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

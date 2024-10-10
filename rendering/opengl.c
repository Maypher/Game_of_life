#include <stdio.h>

#include "glad/glad.h"
#include "glfw3/glfw3.h"

void resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

GLFWwindow *init_opengl() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int WIDTH = 800;
    const int HEIGHT = 600;

    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Unable to load opengl functions");
        return 0;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, resize_callback);
    return window;
}

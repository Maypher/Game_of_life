#include <stdio.h>

#include "glad/glad.h"
#include "glfw3/glfw3.h"

GLFWwindow *init_opengl() {
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_PROFILE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Unable to load opengl functions");
        return 0;
    }
    GLFWwindow *window = glfwCreateWindow(600, 800, "Game of Life", 0, 0);

    if (!window) {
        printf("Unable to create window");
        return 0;
    }

    return window;
}

#include "glfw3/glfw3.h"
#include "opengl.h"

int main(int argc, char *argv[]) {
    GLFWwindow *window = init_opengl();

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

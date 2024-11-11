#include "scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_of_life.h"
#include "glad/glad.h"
#include "glfw3/glfw3.h"

void create_input_buffer(unsigned int *framebuffer, unsigned int *texture) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewport[2], viewport[3], 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           *texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int create_shader_program(char *vertex_shader_filename,
                          char *fragment_shader_filename) {
    // Load shaders
    const char *vertex_source = get_shader_content(vertex_shader_filename);
    const char *fragment_source = get_shader_content(fragment_shader_filename);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Shader error
    int success;
    char infoLog[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED (%s)\n%s\n",
               vertex_shader_filename, infoLog);
        return -1;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED (%s)\n%s\n",
               fragment_shader_filename, infoLog);
        return -1;
    }

    // Creates the program and binds the shaders
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    return program;
};

Scene *create_scene(char *vertex_shader_filename,
                    char *fragment_shader_filename, int width, int height) {
    // Single quad vertices
    const float QUAD_VERTICES[] = {-1.0, 1.0,  0.0, 1.0, 1.0,  0.0,
                                   -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};
    const int INDICES[] = {0, 1, 2, 1, 2, 3};

    unsigned int vbo;  // Vertex buffer object (Stores vertices data)
    glGenBuffers(1, &vbo);

    unsigned int instance_buffer;
    glGenBuffers(1, &instance_buffer);

    unsigned int
        instance_vertex_object;  // Stores cell state for every instance
    glGenBuffers(1, &instance_vertex_object);

    unsigned int ebo;  // Element buffer object (Stores indices)
    glGenBuffers(1, &ebo);

    unsigned int vao;  // Vertex attribute object (Stores all data related to a
                       // given vbo)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind vbo and pass the vertices data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Bing ebo and pass all the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES,
                 GL_STATIC_DRAW);

    // Buffer to store the instance data for a cell
    glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
    glBufferData(GL_ARRAY_BUFFER, width * height * sizeof(Cell), NULL,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Cell),
                          (void *)0);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Cell),
                          (void *)offsetof(Cell, alive));
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Cell),
                          (void *)offsetof(Cell, hovered));
    glVertexAttribDivisor(3, 1);

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create draw program
    unsigned int draw_program =
        create_shader_program(vertex_shader_filename, fragment_shader_filename);

    // Sends the game dimensions to the shader
    glUseProgram(draw_program);
    unsigned int dimensions[] = {width, height};
    int game_dimensions_location =
        glGetUniformLocation(draw_program, "game_dimensions");
    glUniform2uiv(game_dimensions_location, 1, dimensions);
    glUseProgram(0);

    unsigned int input_program =
        create_shader_program("./shaders/main.vert", "./shaders/input.frag");

    // Input texture
    unsigned int input_framebuffer;
    unsigned int input_texture;

    create_input_buffer(&input_framebuffer, &input_texture);

    // Creates the scene and stores all data related to it
    Scene *scene = (Scene *)malloc(sizeof(Scene));

    scene->VAO = vao;
    scene->instance_vertex_obj = instance_buffer;
    scene->input_framebuffer = input_framebuffer;
    scene->input_texture = input_texture;
    scene->draw_program = draw_program;
    scene->input_program = input_program;

    return scene;
}

char *get_shader_content(const char *fileName) {
    FILE *fp;
    long size = 0;
    char *shaderContent;

    /* Read File to get size */
    fopen_s(&fp, fileName, "rb");

    if (fp == NULL) {
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    /* Read File for Content */
    fopen_s(&fp, fileName, "rb");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size - 1, fp);
    fclose(fp);

    return shaderContent;
}

void draw_game(GLFWwindow *window, Scene *scene, GameOfLife *game) {
    double cursor_x, cursor_y;
    glfwGetCursorPos(window, &cursor_x, &cursor_y);

    cursor_y = cursor_y > 0 ? cursor_y : 0;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];                // x position of the lower-left corner
    int y = viewport[1];                // y position of the lower-left corner
    int viewport_width = viewport[2];   // width of the viewport
    int viewport_height = viewport[3];  // height of the viewport

    int game_size = game->width * game->height;

    // Bind all the cells data to the buffer
    glBindVertexArray(scene->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, scene->instance_vertex_obj);

    glBufferData(GL_ARRAY_BUFFER, game_size * sizeof(Cell), game->cells,
                 GL_STATIC_DRAW);
    // glBindVertexArray(0);

    // First draw the input texture
    glBindFramebuffer(GL_FRAMEBUFFER, scene->input_framebuffer);
    glUseProgram(scene->input_program);

    glEnableVertexAttribArray(1);

    glEnable(GL_SCISSOR_TEST);
    glScissor(cursor_x, viewport_height - cursor_y, 1, 1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, game_size);

    glDisableVertexAttribArray(1);
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Then draw the actual game
    glUseProgram(scene->draw_program);

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, game_size);

    glUseProgram(0);
    glBindVertexArray(0);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    glBindFramebuffer(GL_FRAMEBUFFER, scene->input_framebuffer);

    float hovered_cell_id;
    glReadPixels(xpos, ypos, 1, 1, GL_RED, GL_FLOAT, &hovered_cell_id);
    printf("x: %f y: %f cell id: %f\n", xpos, ypos, hovered_cell_id);
}

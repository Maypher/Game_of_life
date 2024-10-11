#include "scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"

Scene *create_scene(char *vertex_shader_filename,
                    char *fragment_shader_filename, int width, int height) {
    // Single quad vertices
    const float QUAD_VERTICES[] = {-1.0, 1.0,  0.0, 1.0, 1.0,  0.0,
                                   -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};
    const int INDICES[] = {0, 1, 2, 1, 2, 3};

    unsigned int vbo;  // Vertex buffer object (Stores vertices data)
    glGenBuffers(1, &vbo);

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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shaders
    const char *vertex_source = get_shader_content(vertex_shader_filename);
    const char *fragment_source = get_shader_content(fragment_shader_filename);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // Creates the program and binds the shaders
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    // Sends the game dimensions to the shader
    glUseProgram(program);
    unsigned int dimensions[] = {width, height};
    int game_dimensions_location =
        glGetUniformLocation(program, "game_dimensions");
    glUniform2uiv(game_dimensions_location, 1, dimensions);

    glUseProgram(0);

    // Creates the scene and stores all data related to it
    Scene *scene = (Scene *)malloc(sizeof(Scene));

    scene->VAO = vao;
    scene->program = program;

    return scene;
}

char *get_shader_content(const char *fileName) {
    FILE *fp;
    long size = 0;
    char *shaderContent;

    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size - 1, fp);
    fclose(fp);

    return shaderContent;
}

void draw_game(Scene *scene, GameOfLife *game) {
    glUseProgram(scene->program);
    glBindVertexArray(scene->VAO);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glBindVertexArray(0);
}

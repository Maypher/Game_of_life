#include "scene.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"

Scene *create_scene(char *vertex_shader_filename,
                    char *fragment_shader_filename) {
    const float QUAD_VERTICES[] = {-1.0, 1.0,  0.0, 1.0, 1.0,  0.0,
                                   -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};
    const int INDICES[] = {0, 1, 2, 1, 2, 3};

    unsigned int vbo;
    glGenBuffers(1, &vbo);

    unsigned int ebo;  // Element buffer object (Stores indices)
    glGenBuffers(1, &ebo);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

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
}

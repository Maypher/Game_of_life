#pragma once

#define GLFW_INCLUDE_NONE

#include "game_of_life.h"
#include "glfw3/glfw3.h"

typedef struct {
    unsigned int VAO;  // vertex array object (stores all data related to a VBO)
    unsigned int instance_vertex_obj;
    unsigned int input_framebuffer;
    unsigned int input_texture;
    unsigned int draw_program;
    unsigned int input_program;
} Scene;

Scene *create_scene(char *vertex_shader_filename,
                    char *fragment_shader_filename, int widht, int height);

char *get_shader_content(const char *fileName);

void draw_game(GLFWwindow *window, Scene *scene, GameOfLife *game);

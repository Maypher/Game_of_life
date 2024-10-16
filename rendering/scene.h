#pragma once
#include <stdio.h>

#include "game_of_life.h"

typedef struct {
    unsigned int VAO;  // vertex array object (stores all data related to a VBO)
    unsigned int instance_vertex_obj;
    unsigned int program;
} Scene;

Scene *create_scene(char *vertex_shader_filename,
                    char *fragment_shader_filename, int widht, int height);

char *get_shader_content(const char *fileName);

void draw_game(Scene *scene, GameOfLife *game);

#include "objects.hpp"

int program, bg_program, menu_program;
float ground_y = -0.8f;
float object_speed = 0.02f;
float block_height = 0.1;
float current_ground_y = ground_y;
unsigned int titleVAO, titleVBO;
unsigned int bgVAO, bgVBO;
unsigned int playVAO, playVBO;
unsigned int playerVAO, playerVBO;
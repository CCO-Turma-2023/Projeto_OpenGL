#include "objects.hpp"

int program, bg_program, menu_program;
float block_height = 0.1f, block_width = 0.1f;
float spike_height = 0.1, spike_width = 0.1f;
float player_height = 0.1f, player_width = 0.1f;
float ground_y = -0.8f;
float object_speed = 0.02f;
float current_ground_y = ground_y;
unsigned int titleVAO, titleVBO;
unsigned int bgVAO, bgVBO;
unsigned int playVAO, playVBO;
unsigned int playerVAO, playerVBO;
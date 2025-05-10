#include "objects.hpp"

// Definindo variaveis globais
int program, bg_program, menu_program;
float block_height = 0.1f, block_width = 0.1f;
float spike_height = 0.1, spike_width = 0.1f;
float player_height = 0.1f, player_width = 0.1f;
float portal_height = 0.4f, portal_width = 0.1f;
float player_x = -0.7f;
float ground_y = -0.8f;
float object_speed = 0.02f;
float current_ground_y = ground_y;
unsigned int titleVAO, titleVBO;
unsigned int bgVAO, bgVBO;
unsigned int playVAO, playVBO;
unsigned int portalVAO, portalVBO;
unsigned int playerVAO, playerVBO;
bool on_block;
float block_left;
float block_right;
float block_top;
float block_bottom;
float obs_left;
float obs_right;
float obs_top;
float player_left = player_x - player_width / 2.0f;
float player_right = player_x + player_width / 2.0f;
float obs_bottom;
float portal_y = ground_y + 0.15f, portal_x = 9.0f, portal_initial_x = 9.0f;
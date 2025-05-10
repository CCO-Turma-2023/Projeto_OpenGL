#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../lib/utils.h"
#include "../lib/stb_image.h"
#include <vector>

extern int program, bg_program, menu_program;
extern unsigned int bgVAO, bgVBO;
extern unsigned int titleVAO, titleVBO;
extern unsigned int playVAO, playVBO;
extern unsigned int portalVAO, portalVBO;
extern unsigned int playerVAO, playerVBO;

extern float player_y, player_x;
extern float portal_y, portal_x, portal_initial_x;
extern float velocity_y;
extern bool jumping;

extern float ground_y;
extern float current_ground_y;

extern float object_speed;

extern float block_height, block_width;
extern float spike_height, spike_width;
extern float player_height, player_width;
extern float portal_height, portal_width;

extern float bg_offset;
extern float bg_scroll_speed;

extern float angle;
extern float angle_increment;

extern bool on_block;
extern float block_left;
extern float block_right;
extern float block_top;
extern float block_bottom;

extern float obs_left;
extern float obs_right;
extern float obs_top;
extern float player_left;
extern float player_right;
extern float obs_bottom;

struct Obstacle {
    float x;
    float y;
    float width;
    float height;
    float initial_x;
    GLuint VAO, VBO;
};

struct Block {
    float x;
    float y;
    float width;
    float height;
    float initial_x;
    GLuint VAO, VBO;
};


extern std::vector<Obstacle> obstacles;

extern std::vector<Block> blocks;

void initPlayer ();
void initObstacle (float x, float y);
void initBlockObstacle (float x, float y);
void initBackground ();
void Portal();
void initTitle ();
void initPlay ();

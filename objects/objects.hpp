#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../lib/utils.h"
#include "../lib/stb_image.h"


extern int program, bg_program;
extern unsigned int bgVAO, bgVBO, bgTexture;
extern unsigned int playerVAO, playerVBO;

extern float player_y;
extern float velocity_y;
extern bool jumping;

extern float ground_y;
extern float current_ground_y;

extern float object_speed;

extern float block_height;

extern float bg_offset;
extern float bg_scroll_speed;

extern float angle;
extern float angle_increment;

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
void initBlockObstacle (float x, float width, float y);
void initBackground ();
/**
 * @file runner.cpp
 * Jogo estilo runner: quadrado pula obstáculos girando no próprio centro
 */

#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <thread>
#include <chrono>
#include "./objects/objects.hpp"

/* Globals */
int win_width = 800;
int win_height = 600;

int showPlayer = 1;

/* Shaders */
const char *vertex_code = R"(
 #version 330 core
 layout (location = 0) in vec3 position;
 layout (location = 1) in vec3 color;
 out vec3 vColor;
 uniform mat4 offset;
 void main()
 {
     gl_Position = offset * vec4(position, 1.0);
     vColor = color;
 }
 )";

const char *fragment_code = R"(
 #version 330 core
 in vec3 vColor;
 out vec4 FragColor;
 void main()
 {
     FragColor = vec4(vColor, 1.0f);
 }
 )";

const char *bg_vertex_code = R"(
 #version 330 core
 layout (location = 0) in vec2 position;
 layout (location = 1) in vec2 texCoord;
 out vec2 TexCoord;
 uniform float bg_offset;
 void main()
 {
     gl_Position = vec4(position, 0.0, 1.0);
     TexCoord = vec2(texCoord.x + bg_offset, texCoord.y);
 }
 )";

const char *bg_fragment_code = R"(
 #version 330 core
 in vec2 TexCoord;
 out vec4 FragColor;
 uniform sampler2D texture1;
 void main()
 {
     FragColor = texture(texture1, TexCoord);
 }
 )";

/* Functions */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void timer(int);
void initData(void);
void initShaders(void);
void loadBackgroundTexture(const char *filename);
void reset(void);

void display()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Fundo
    glUseProgram(bg_program);
    glBindVertexArray(bgVAO);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    int bgOffsetLoc = glGetUniformLocation(bg_program, "bg_offset");
    glUniform1f(bgOffsetLoc, bg_offset);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 2. Jogador (quadrado)
    glUseProgram(program);

    int offsetLoc = glGetUniformLocation(program, "offset");

    if (showPlayer)
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(-0.7f, player_y, 0.0f));

        if (jumping)
        {
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(playerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    for (const auto &obs : obstacles)
    {
        glm::mat4 obstacleModel = glm::translate(glm::mat4(1.0f), glm::vec3(obs.x, -0.8f, 0.0f));
        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(obstacleModel));
        glBindVertexArray(obs.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    for (const auto &bl : blocks)
    {
        glm::mat4 blockModel = glm::translate(glm::mat4(1.0f), glm::vec3(bl.x, -0.8f, 0.0f));
        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(blockModel));
        glBindVertexArray(bl.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glutSwapBuffers();
}

void reset()
{
    bg_scroll_speed = 0.0f;
    obstacle_speed = 0.0f;
    showPlayer = 0;
    display();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    showPlayer = 1;
    bg_scroll_speed = 0.0011f;
    obstacle_speed = 0.02f;

    for (auto &obs : obstacles)
    {
        obs.x = obs.initial_x;
    }

    for (auto &bl : blocks)
    {
        bl.x = bl.initial_x;
    }

    player_y = -0.8f;
    jumping = false;
    angle = 0.0f;
    display();
}

void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27 || key == 'q' || key == 'Q')
        exit(0);

    if (key == ' ')
    {
        if (!jumping)
        {
            velocity_y = 0.035f;
            jumping = true;
            angle = 0.0f;
        }
    }
}

void timer(int value)
{
    bool resetAllObs = true;
    bool resetAllBl = true;

    for (auto &obs : obstacles)
    {
        obs.x -= obstacle_speed;

        if (obs.x > -1.2f)
        {
            resetAllObs = false;
        }
    }

    for (auto &bl : blocks)
    {
        bl.x -= block_speed;

        if (bl.x > -1.2f)
        {
            resetAllBl = false;
        }
    }

    if (resetAllObs && resetAllBl)
    {
        for (auto &obs : obstacles)
        {
            obs.x = obs.initial_x;
        }
        for (auto &bl : blocks)
        {
            bl.x = bl.initial_x;
        }
    }

    for (auto &obs : obstacles)
    {
        if (obs.x < -0.65f && obs.x > -0.75f &&
            player_y - 0.05f <= -0.8f + 0.023f)
        {
            std::cout << "morreu otário" << std::endl;
            reset();
        }
    }

    bool on_block = false;

    for (auto &bl : blocks)
    {
        if (bl.x < -0.65f + (bl.width / 2.0f) && bl.x > -0.75f - (bl.width / 2.0f))
        {

            if (player_y - 0.05f >= bl.y + (bl.height / 2.0f) &&
                player_y - 0.05f <= bl.y + (bl.height / 2.0f) + 0.05f &&
                velocity_y <= 0.0f)
            {

                ground_y = bl.y + (bl.height / 2.0f);
                on_block = true;
            }

            else if (player_y - 0.05f < bl.y + (bl.height / 2.0f))
            {
                std::cout << "morreu otário" << std::endl;
                reset();
            }
        }
    }

    if (!on_block)
    {
        ground_y = -0.8f;
    }

    bg_offset += bg_scroll_speed;
    if (bg_offset > 1.0f)
        bg_offset -= 1.0f;

    if (jumping)
    {
        player_y += velocity_y;
        velocity_y -= 0.003f;

        angle -= 8.0f;
        if (angle < -360.0f)
            angle += 360.0f;

        if (player_y <= ground_y)
        {
            player_y = ground_y;
            velocity_y = 0.0f;
            jumping = false;
            angle = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void initData()
{
    initPlayer();
    initObstacle(1.0);
    initObstacle(1.5);
    initObstacle(2.0);
    initObstacle(3.0);
    initBlockObstacle(4.0, 0.5);
    initBackground();
}

void loadBackgroundTexture(const char *filename)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &bgTexture);
        glBindTexture(GL_TEXTURE_2D, bgTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        printf("Falha ao carregar textura!\n");
        stbi_image_free(data);
    }
}

void initShaders()
{
    program = createShaderProgram(vertex_code, fragment_code);
    bg_program = createShaderProgram(bg_vertex_code, bg_fragment_code);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Geometry Copy Dash");
    glewExperimental = GL_TRUE;
    glewInit();

    initData();
    loadBackgroundTexture("img.png");
    initShaders();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
}

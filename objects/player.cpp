#include "objects.hpp"

float player_y = -0.8f;
float velocity_y = 0.0f;
bool jumping = false;

float angle = 0.0f;
float angle_increment = 0.0f;

void initPlayer ()
{
    // Jogador (quadrado)
    float playerVertices[] = {
        -0.05f, -0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.05f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.05f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.05f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &playerVAO);
    glGenBuffers(1, &playerVBO);
    glBindVertexArray(playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
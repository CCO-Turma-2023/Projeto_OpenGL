#include "objects.hpp"

float player_y = ground_y;
float velocity_y = 0.0f;
bool jumping = false;

float angle = 0.0f;
float angle_increment = 0.0f;

void initPlayer ()
{
    // Jogador (quadrado)
    float playerVertices[] = {
        //   x                             y                             u       v (invertido)
        -player_width / 2.0f, -player_height / 2.0f,  0.0f, 1.0f, // Canto inferior esquerdo
         player_width / 2.0f, -player_height / 2.0f,  1.0f, 1.0f, // Canto inferior direito
        -player_width / 2.0f,  player_height / 2.0f,  0.0f, 0.0f, // Canto superior esquerdo
    
         player_width / 2.0f, -player_height / 2.0f,  1.0f, 1.0f, // Canto inferior direito
         player_width / 2.0f,  player_height / 2.0f,  1.0f, 0.0f, // Canto superior direito
        -player_width / 2.0f,  player_height / 2.0f,  0.0f, 0.0f, // Canto superior esquerdo
    };
    
    glGenVertexArrays(1, &playerVAO);
    glGenBuffers(1, &playerVBO);
    glBindVertexArray(playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);            // posição
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texCoord
    glEnableVertexAttribArray(1);

}
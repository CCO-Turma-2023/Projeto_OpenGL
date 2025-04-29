#include "objects.hpp"

std::vector<Obstacle> obstacles;

float obstacle_x;

void initObstacle(float x, float y)
{
    Obstacle newObstacle;
    newObstacle.x = x;
    newObstacle.y = y;
    newObstacle.width = spike_width;
    newObstacle.height = spike_height;
    newObstacle.initial_x = x;

    float obstacleVertices[] = {
        //   x                             y                             u       v (invertido)
        -spike_width / 2.0f, -spike_height / 2.0f,  0.0f, 1.0f,  // Canto inferior esquerdo
         spike_width / 2.0f, -spike_height / 2.0f,  1.0f, 1.0f,  // Canto inferior direito
         0.0f,    spike_height / 2.0f,  0.5f, 0.0f,    // Canto superior central
    };
    
    glGenVertexArrays(1, &newObstacle.VAO);
    glGenBuffers(1, &newObstacle.VBO);

    glBindVertexArray(newObstacle.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, newObstacle.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices), obstacleVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);            // posição
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texCoord
    glEnableVertexAttribArray(1);

    obstacles.push_back(newObstacle);
}
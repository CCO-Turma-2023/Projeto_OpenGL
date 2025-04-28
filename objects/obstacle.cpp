#include "objects.hpp"

std::vector<Obstacle> obstacles;

float obstacle_speed = 0.02f;
float obstacle_x;

void initObstacle(float x)
{
    Obstacle newObstacle;
    newObstacle.x = x;
    newObstacle.initial_x = x;

    float obstacleVertices[] = {
        -0.05f, -0.05f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.05f, -0.05f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,   0.05f, 0.0f,  0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &newObstacle.VAO);
    glGenBuffers(1, &newObstacle.VBO);

    glBindVertexArray(newObstacle.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, newObstacle.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices), obstacleVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    obstacles.push_back(newObstacle);
}
#include "objects.hpp"

std::vector<Block> blocks;

void initBlockObstacle(float x, float y)
{
    Block newBlock;
    newBlock.x = x;
    newBlock.initial_x = x;
    newBlock.width = block_width;
    newBlock.height = block_height; 
    newBlock.y = y;

    float blockVertices[] = {
        //    x               y             z        u     v (invertido)
        -0.5f * block_width, -0.5f * block_height, 0.0f,   0.0f, 1.0f,
         0.5f * block_width, -0.5f * block_height, 0.0f,   1.0f, 1.0f,
        -0.5f * block_width,  0.5f * block_height, 0.0f,   0.0f, 0.0f,
         0.5f * block_width, -0.5f * block_height, 0.0f,   1.0f, 1.0f,
         0.5f * block_width,  0.5f * block_height, 0.0f,   1.0f, 0.0f,
        -0.5f * block_width,  0.5f * block_height, 0.0f,   0.0f, 0.0f,
    };

    glGenVertexArrays(1, &newBlock.VAO);
    glGenBuffers(1, &newBlock.VBO);

    glBindVertexArray(newBlock.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, newBlock.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertices), blockVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);                // posição
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texCoord
    glEnableVertexAttribArray(1);

    blocks.push_back(newBlock);
}

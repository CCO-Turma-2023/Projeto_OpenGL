#include "objects.hpp"

std::vector<Block> blocks;

void initBlockObstacle(float x, float width, float y)
{
    const float height = block_height; 

    Block newBlock;
    newBlock.x = x;
    newBlock.initial_x = x;
    newBlock.width = width;
    newBlock.height = height; 
    newBlock.y = y;

    float blockVertices[] = {
        -width / 2.0f, -height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto inferior esquerdo
         width / 2.0f, -height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto inferior direito
        -width / 2.0f,  height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto superior esquerdo

         width / 2.0f, -height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto inferior direito
         width / 2.0f,  height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto superior direito
        -width / 2.0f,  height / 2.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Canto superior esquerdo
    };

    glGenVertexArrays(1, &newBlock.VAO);
    glGenBuffers(1, &newBlock.VBO);

    glBindVertexArray(newBlock.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, newBlock.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertices), blockVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    blocks.push_back(newBlock);
}

#include "objects.hpp"

std::vector<Block> blocks;

float block_speed = 0.02f;
float block_x;

void initBlockObstacle(float x, float width)
{
    const float height = 0.1f; // Altura fixa do bloco

    Block newBlock;
    newBlock.x = x;
    newBlock.width = width;
    newBlock.height = height; // Agora temos a altura do bloco
    newBlock.y = -0.8f;

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

#include "objects.hpp"

void initTitle ()
// Inicializa o título "Geometry Dash"
{
    float titleWidth = 0.8f;   // Largura em relação ao viewport (-1 a 1)
    float titleHeight = 0.2f; // Altura proporcional
    float titleY = 0.3f;       // Posição centralizada verticalmente

    float TitleVertices[] = {
        -titleWidth, -titleHeight + titleY,  0.0f, 1.0f,
         titleWidth, -titleHeight + titleY,  1.0f, 1.0f,
        -titleWidth,  titleHeight + titleY,  0.0f, 0.0f,
         titleWidth, -titleHeight + titleY,  1.0f, 1.0f,
         titleWidth,  titleHeight + titleY,  1.0f, 0.0f,
        -titleWidth,  titleHeight + titleY,  0.0f, 0.0f,
    };
    glGenVertexArrays(1, &titleVAO);
    glGenBuffers(1, &titleVBO);
    glBindVertexArray(titleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, titleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TitleVertices), TitleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void initPlay ()
// Inicializa o botão de play
{
    float playSize = 0.2f; // Definição do tamanho inicial do botão play
    float playY = -0.3f; // Definição da posição abaixo do centro

    float PlayVertices[] = {
        -playSize, -playSize + playY,  0.0f, 1.0f,
         playSize, -playSize + playY,  1.0f, 1.0f,
        -playSize,  playSize + playY,  0.0f, 0.0f,
         playSize, -playSize + playY,  1.0f, 1.0f,
         playSize,  playSize + playY,  1.0f, 0.0f,
        -playSize,  playSize + playY,  0.0f, 0.0f,
    };
    glGenVertexArrays(1, &playVAO);
    glGenBuffers(1, &playVBO);
    glBindVertexArray(playVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlayVertices), PlayVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
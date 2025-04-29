#include "objects.hpp"

float bg_offset = 0.0f;
float bg_scroll_speed = 0.0011f;

void initBackground ()
{
    // Fundo
    float bgVertices[] = {
        -1.0f, -1.0f,    0.0f, 1.0f,
         1.0f, -1.0f,    1.0f, 1.0f,
        -1.0f,  1.0f,    0.0f, 0.0f,
         1.0f, -1.0f,    1.0f, 1.0f,
         1.0f,  1.0f,    1.0f, 0.0f,
        -1.0f,  1.0f,    0.0f, 0.0f,
    };
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);
    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
#include "objects.hpp"


void Portal ()
{
    float PortalVertices[] = {
        //        x                             y                           u     v (invertido)
        -portal_width / 2.0f, -portal_height / 2.0f,   0.0f, 1.0f, // Inferior esquerdo
         portal_width / 2.0f, -portal_height / 2.0f,   1.0f, 1.0f, // Inferior direito
        -portal_width / 2.0f,  portal_height / 2.0f,   0.0f, 0.0f, // Superior esquerdo
    
         portal_width / 2.0f, -portal_height / 2.0f,   1.0f, 1.0f, // Inferior direito
         portal_width / 2.0f,  portal_height / 2.0f,   1.0f, 0.0f, // Superior direito
        -portal_width / 2.0f,  portal_height / 2.0f,   0.0f, 0.0f  // Superior esquerdo
    };
    
    glGenVertexArrays(1, &portalVAO);
    glGenBuffers(1, &portalVBO);
    glBindVertexArray(portalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, portalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PortalVertices), PortalVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
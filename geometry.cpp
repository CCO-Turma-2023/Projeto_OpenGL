/**
 * @file geometry.cpp
 * Jogo estilo geometry dash
 */

#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <thread>
#include <chrono>
#include "./objects/objects.hpp"

/* Globals */
int win_width = 800;
int win_height = 600;

int showPlayer = 1;
bool in_menu = true;
bool final = false;

float animation_time = 0.0f;

int bufferJump = 0;

GLuint titleTexture;
GLuint playButtonTexture;
GLuint blockTexture;
GLuint SpikeTexture;
GLuint PlayerTexture;
GLuint PortalTexture;
GLuint bgTexture;

/* Shaders */
const char *vertex_code = R"(
    #version 330 core
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texCoord;
    out vec2 TexCoord;
    uniform mat4 offset;
    void main()
    {
        gl_Position = offset * vec4(position, 0.0, 1.0);
        TexCoord = texCoord;
    }
    )";

const char *fragment_code = R"(
 #version 330 core
 in vec2 TexCoord;
 out vec4 FragColor;
 uniform sampler2D texture1;
 void main()
 {
     FragColor = texture(texture1, TexCoord);
 }
 )";

const char *bg_vertex_code = R"(
 #version 330 core
 layout (location = 0) in vec2 position;
 layout (location = 1) in vec2 texCoord;
 out vec2 TexCoord;
 uniform float bg_offset;
 void main()
 {
     gl_Position = vec4(position, 0.0, 1.0);
     TexCoord = vec2(texCoord.x + bg_offset, texCoord.y);
 }
 )";

const char *bg_fragment_code = R"(
 #version 330 core
 in vec2 TexCoord;
 out vec4 FragColor;
 uniform sampler2D texture1;
 void main()
 {
     FragColor = texture(texture1, TexCoord);
 }
 )";

 const char *menu_vertex_code = R"(
    #version 330 core
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texCoord;
    out vec2 TexCoord;
    uniform mat4 menu_transform;  // Uniforme para transformação

    void main()
    {
        gl_Position = menu_transform * vec4(position, 0.0, 1.0);
        TexCoord = texCoord;  // As coordenadas de textura permanecem inalteradas
    }
)";

const char *menu_fragment_code = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D texture1;

    void main()
    {
        FragColor = texture(texture1, TexCoord);
    }
)";

/* Functions */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void timer(int);
void initData(void);
void initShaders(void);
void reset(int in_menu);

void display()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Fundo
    glUseProgram(bg_program);

    // Desenhar o Fundo
    glBindVertexArray(bgVAO);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    int bgOffsetLoc = glGetUniformLocation(bg_program, "bg_offset");
    glUniform1f(bgOffsetLoc, bg_offset);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (in_menu) {
        // Renderizar o menu
        
        glUseProgram(menu_program);
        glm::mat4 title_model = glm::mat4(1.0f);  
        title_model = glm::scale(title_model, glm::vec3(1.0f, 1.0f, 1.0f));  
    
        int transformLoc = glGetUniformLocation(menu_program, "menu_transform");

        // Desenhar o título
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(title_model)); 
        glBindTexture(GL_TEXTURE_2D, titleTexture);  
        glBindVertexArray(titleVAO);  
        glDrawArrays(GL_TRIANGLES, 0, 6);  
       
        // Aplicar a escala no botão de play
        float scale = 1.0f + 0.05f * sin(animation_time);
        glm::mat4 play_model = glm::mat4(1.0f);  
        play_model = glm::scale(play_model, glm::vec3(scale, scale, 1.0f));  
    
        // Desenhar o botão de play
        transformLoc = glGetUniformLocation(menu_program, "menu_transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(play_model));  
    
        glBindTexture(GL_TEXTURE_2D, playButtonTexture);  
        glBindVertexArray(playVAO);  
        glDrawArrays(GL_TRIANGLES, 0, 6);  
    
        glutSwapBuffers();  
        return;
    }

    // 2. Jogador (quadrado)
    glUseProgram(program);
    
    int offsetLoc = glGetUniformLocation(program, "offset");

    if (showPlayer)
    {
        // Renderizar o jogador

        // Matriz transformadora
        glm::mat4 model = glm::mat4(1.0f);

        // Aplicando translação do jogador para a parte esquerda da tela
        model = glm::translate(model, glm::vec3(player_x, player_y, 0.0f));

        if (jumping)
        {
            // Função de rotação para que o objeto, ao pular, dê um giro em torno do próprio eixo durante o salto
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        // Renderizar o jogador com a matriz transformadora
        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(playerVAO);

        glBindTexture(GL_TEXTURE_2D, PlayerTexture);
       
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // A translação desses objetos (obstáculos e blocos) são aplicados para move-los
    // em direção ao jogador. O valor dessa translação é atualizado no timer
    
    for (const auto &obs : obstacles)
    {   
        // Aplicar translação em cada obstáculo e renderizar ele
        glm::mat4 obstacleModel = glm::translate(glm::mat4(1.0f), glm::vec3(obs.x, obs.y, 0.0f));
        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(obstacleModel));
        glBindVertexArray(obs.VAO);
        glBindTexture(GL_TEXTURE_2D, SpikeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    for (const auto &bl : blocks)
    {
        // Aplicar translação em cada bloco e renderizar ele
        glm::mat4 blockModel = glm::translate(glm::mat4(1.0f), glm::vec3(bl.x, bl.y, 0.0f));
        glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(blockModel));
        glBindVertexArray(bl.VAO);
        glBindTexture(GL_TEXTURE_2D, blockTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Aplicar translação no portal e renderizar ele
    glm::mat4 portalModel = glm::translate(glm::mat4(1.0f), glm::vec3(portal_x, portal_y, 0.0f));
    glUniformMatrix4fv(offsetLoc, 1, GL_FALSE, glm::value_ptr(portalModel));
    glBindVertexArray(portalVAO);
    glBindTexture(GL_TEXTURE_2D, PortalTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glutSwapBuffers();
}

void reset(int in_menu)
// Volta os objetos para o estado inicial da fase após morrer ou finalizar a fase
{
    bg_scroll_speed = 0.0f;
    object_speed = 0.0f;
    showPlayer = 0;
    bufferJump = 0;
    display();

    if (!in_menu)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    showPlayer = 1;
    bg_scroll_speed = 0.0011f;
    object_speed = 0.02f;

    portal_x = portal_initial_x;

    for (auto &obs : obstacles)
    {
        obs.x = obs.initial_x;
    }

    for (auto &bl : blocks)
    {
        bl.x = bl.initial_x;
    }

    player_y = ground_y;
    // Impede que o player inicialize a fase pulando
    jumping = false;
    angle = 0.0f;
    display();
}

// Aplica janela 
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    // Binds para entrar no Menu:
    if (key == 27 || key == 'q' || key == 'Q')
    {
        in_menu = true;
        return;
    }

    // Bind para pular:
    if (key == ' ')
    {
        if (in_menu)
        {
            in_menu = false;
            reset(1);

            return;
        }

        if (!jumping && !final)
        {
            // Função de pular
            velocity_y = 0.035f;
            jumping = true;
            angle = 0.0f;
        }
        

        bufferJump = 2;

        return;
    }
}


void SpecialInput(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        if (!jumping && !final)
        {
            // Função de pular
            velocity_y = 0.035f;
            jumping = true;
            angle = 0.0f;
        }

        bufferJump = 2;

        return;
    }
}


void mouse(int button, int state, int x, int y) {
    if (in_menu && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // converter x, y da tela para coordenadas OpenGL
        float gl_x = (float)x / win_width * 2.0f - 1.0f;
        float gl_y = 1.0f - (float)y / win_height * 2.0f;

        if (gl_x >= -0.2f && gl_x <= 0.2f && gl_y >= -0.5f && gl_y <= -0.1f) {
            in_menu = false;
            glutPostRedisplay();
        }
    }
}


void timer(int value)
{

    if (in_menu) {
        bg_offset += bg_scroll_speed;
        if (bg_offset > 1.0f)
            bg_offset -= 1.0f;
        animation_time += 0.05f;
        glutPostRedisplay();
        glutTimerFunc(16, timer, 0); 
        return;
    }

    portal_x -= object_speed;

    // Se o portal chegar em determinada posição, é considerado fim do jogo
    if(portal_x <= -0.10f) final = true;
    
    // Impede que o player pule a partir de determinada área do jogo (perto do final)
    if(portal_x <= -0.59f){
        velocity_y = 0.035f;
        player_y += velocity_y;
        if(portal_x <= -0.7f){
            showPlayer = 0;
            display();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            in_menu = true;
            // Reseta o portal para a posição inicial novamente
            reset(0);
            final = false;
        }
    }
    

    for (auto &obs : obstacles)
    {
        obs.x -= object_speed;

    }

    for (auto &bl : blocks)
    {
        bl.x -= object_speed;

    }

    for (auto &obs : obstacles)
    {
        obs_left = obs.x - obs.width / 4.0f;
        obs_right = obs.x + obs.width / 4.0f;
        obs_top = obs.y + obs.height;
        obs_bottom = obs.y - obs.height;
        if (
            player_right > obs_left && player_left < obs_right &&
            player_y < obs_top && player_y > obs_bottom
        ) {
            reset(0);
        }
    }

    if (bufferJump > 0)
    {
        if (!jumping && !final)
        {
            // Função de pular
            velocity_y = 0.035f;
            jumping = true;
            angle = 0.0f;
        }
        bufferJump--;
    }


    on_block = false;

    for (const auto& bl : blocks) {
        block_left = bl.x - bl.width / 2.0f;
        block_right = bl.x + bl.width / 2.0f;
        block_top = bl.y + bl.height;
        block_bottom = bl.y - bl.height;

        if (player_right > block_left && player_left < block_right) {
            on_block = true;
            if ((player_y >= block_top - 0.01f && player_y <= block_top + 0.01f) && velocity_y <= 0.0f) {
                current_ground_y = block_top;
                velocity_y = 0.0f;
                player_y = block_top;
                angle = 0.0f;
            }else if (player_y < block_top && player_y > block_bottom) {
                reset(0);
            }
        }
    }

    if (!on_block && !jumping && (current_ground_y != ground_y)) {
        jumping= true;
        velocity_y = 0.0f;
    }

    bg_offset += bg_scroll_speed;
    if (bg_offset > 1.0f)
        bg_offset -= 1.0f;

    if (jumping) {
        player_y += velocity_y;
        velocity_y -= 0.003f;
        
        angle -= 8.0f;
        if (angle < -360.0f)
            angle += 360.0f;

        if(!on_block){
            current_ground_y = ground_y;
        }
        
        if (player_y <= current_ground_y) {
            player_y = current_ground_y;
            velocity_y = 0.0f;
            jumping = false;
            angle = 0.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


void initData()
{
    initPlayer();
    
    for(float i = 1.0; i <= 3.0; i += 0.5){
        initObstacle(i, ground_y);
    }
    for(float i = 4.0; i <= 4.7; i += 0.1){
        initBlockObstacle(i, ground_y);
    }

    initObstacle(4.375, ground_y + block_height);

    for (float i = 5.1; i <= 5.6; i += 0.1) {
        for (float j = 0; j <= 0.2; j += 0.1) {  
            initBlockObstacle(i, ground_y + j);
        }
    }
    
   
    for (float i = 6.0; i <= 6.4; i += 0.1) {
        for (float j = 0; j <= 0.3; j += 0.1) {  
            initBlockObstacle(i, ground_y + j);
        }
    }

    initBlockObstacle(6.80, -0.5);
    initBlockObstacle(6.90, -0.5);
    initBlockObstacle(7.15, -0.4);
    initBlockObstacle(7.25, -0.4);

    for(float i = 4.80; i <= 7.8; i += 0.1){
        if((i >= 4.80 && i <= 5.0) || (i >= 5.60 && i <= 5.90) || (i >= 6.4 && i <= 7.8))
            initObstacle(i, ground_y);
    }

    Portal();

    initBackground();
    initTitle();
    initPlay();
}

// Aplicando texturas
GLuint loadTexture(const char *filename) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    GLuint textureID;

    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Permite repetição da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Filtros de minificação/magnificação
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     (nrChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return textureID;
    } else {
        printf("Erro ao carregar a textura %s\n", filename);
        return 0;
    }
}


// Carrega todas as texturas 
void loadTextures() {
    bgTexture = loadTexture("assets/bg.png");
    titleTexture = loadTexture("assets/title.png");
    playButtonTexture = loadTexture("assets/play.png");
    blockTexture = loadTexture("assets/block.png");
    SpikeTexture = loadTexture("assets/spike.png");
    PlayerTexture = loadTexture("assets/player.jpg");
    PortalTexture = loadTexture("assets/portal.png");
}

void initShaders()
{
    program = createShaderProgram(vertex_code, fragment_code);
    bg_program = createShaderProgram(bg_vertex_code, bg_fragment_code);
    menu_program = createShaderProgram(menu_vertex_code, menu_fragment_code);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Geometry Copy Dash");
    glutSpecialFunc(SpecialInput);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loadTextures();
    initData();
    initShaders();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
}

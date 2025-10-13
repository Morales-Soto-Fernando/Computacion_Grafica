/*
Practica 8
Fecha de entrega 13 de octubre del 2025
Morales Soto Fernando
315143977
*/


#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Cámara 
Camera camera(glm::vec3(0.0f, 6.0f, 25.0f));
bool keys[1024]{};
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//  Ciclo Día/Noche + órbitas 
bool day = true;
bool night = false;
float moonRotate = 90.0f;
float sunRotate = 90.0f;
float radius = 12.0f; // radio de la órbita

glm::vec3 lightPos(radius* cos(glm::radians(sunRotate)), radius* sin(glm::radians(sunRotate)), 0.0f);
glm::vec3 newlightPos(radius* cos(glm::radians(moonRotate)), radius* sin(glm::radians(moonRotate)), 0.0f);

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//  Lamp (marcador visible) 
GLuint lampVAO = 0, lampVBO = 0;

int main()
{
    //  GLFW/GLEW 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fernando Morales", nullptr, nullptr);
    if (!window) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialize GLEW\n"; return EXIT_FAILURE; }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    //  Shaders 
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag"); // iluminación de la escena
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");     // marcador visible

    // Modelos 
    Model dog((char*)"Models/RedDog.obj");

    Model asteroide3((char*)"Models/Rocky_Asteroid_3.obj");
    Model asteroide1((char*)"Models/Asteroid_1.obj");

    Model luna((char*)"Models/Moon 2K.obj");
    Model tierra((char*)"Models/Earth.obj");
    Model saturno((char*)"Models/Stylized_Planets.obj");

    // Proyecciones por modelo 
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projectionas = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 100.0f);
    glm::mat4 projectionast = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 100.0f);
    glm::mat4 projectionluna = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 100.0f);
    glm::mat4 projectiontierra = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 100.0f);
    glm::mat4 projectionsat = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 100.0f);

    //cubo 
    {
        float lampVerts[] = {
            // -Z
         -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
         0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
             // +Z
         -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
              // -X
         -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
         -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
              // +X
         0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
               // -Y
         -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
                // +Y
         -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
        };
        glGenVertexArrays(1, &lampVAO);
        glGenBuffers(1, &lampVBO);
        glBindVertexArray(lampVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lampVerts), lampVerts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    // -------------------- Loop --------------------
    while (!glfwWindowShouldClose(window))
    {
        // timing
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // IO
        glfwPollEvents();
        DoMovement();

        // clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Iluminacion
        lightingShader.Use();

        // actualizar órbitas (arcos)
        lightPos = glm::vec3(radius * cos(glm::radians(sunRotate)), radius * sin(glm::radians(sunRotate)), 0.0f);
        newlightPos = glm::vec3(radius * cos(glm::radians(moonRotate)), radius * sin(glm::radians(moonRotate)), 0.0f);

        // posiciones de luces
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.position"),
            newlightPos.x, newlightPos.y, newlightPos.z);

        // parámetros según modo
        if (day) {
            // Sol ON, Luna OFF
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.7f, 0.7f, 0.75f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.7f, 1.6f, 1.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.2f, 1.2f, 1.2f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.specular"), 0.0f, 0.0f, 0.0f);

            // materiales día
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.7f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.6f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        }
        else {
            // Noche: Sol OFF, Luna ON
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.ambient"), 0.5f, 0.5f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.diffuse"), 1.2f, 1.2f, 1.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "newlight.specular"), 1.5f, 1.5f, 1.8f);

            // materiales noche
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.2f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.4f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        }

        // matrices base
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // DIBUJO (proyección específica por modelo) 
        glm::mat4 model(1.0f);

        // Perro  
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Tierra 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectiontierra));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tierra.Draw(lightingShader);

        // Saturno 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionsat));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-8.0f, 15.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        saturno.Draw(lightingShader);

        // Luna 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionluna));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(14.0f, 15.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        luna.Draw(lightingShader);

        // Asteroide1
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionast));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 8.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        asteroide1.Draw(lightingShader);

        // Asteroide3 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionas));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.0f, 8.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        asteroide3.Draw(lightingShader);

        //MARCADORES LAMP (cubitos visibles) 
        lampshader.Use();
        // Puedes usar la proyección "general" para los marcadores
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(lampVAO);

        // Sol 
        if (day) {
            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, lightPos);
            m = glm::scale(m, glm::vec3(0.35f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
            glUniform3f(glGetUniformLocation(lampshader.Program, "color"), 1.0f, 0.9f, 0.6f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Luna 
        if (night) {
            glm::mat4 m = glm::mat4(1.0f);
            m = glm::translate(m, newlightPos);
            m = glm::scale(m, glm::vec3(0.30f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
            glUniform3f(glGetUniformLocation(lampshader.Program, "color"), 0.7f, 0.8f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // swap
        glfwSwapBuffers(window);
    }

    // limpieza VAO/VBO lamp
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);

    glfwTerminate();
    return 0;
}

//  Movimiento cámara + ciclo 
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // L/O: limitar 0–180°
    if (keys[GLFW_KEY_L]) {
        moonRotate += 1.0f; if (moonRotate > 180.0f) moonRotate = 180.0f;
        sunRotate += 1.0f; if (sunRotate > 180.0f) sunRotate = 180.0f;
    }
    if (keys[GLFW_KEY_O]) {
        moonRotate -= 1.0f; if (moonRotate < 0.0f)  moonRotate = 0.0f;
        sunRotate -= 1.0f; if (sunRotate < 0.0f)  sunRotate = 0.0f;
    }
}

// Teclado 
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // J/K: conmutar Día/Noche
    if (keys[GLFW_KEY_J]) { night = true;  day = false; }
    if (keys[GLFW_KEY_K]) { night = false; day = true; }
}

// Mouse 
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // y invertida
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}



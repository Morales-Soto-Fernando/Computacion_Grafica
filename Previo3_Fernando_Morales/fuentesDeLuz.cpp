/*
Practica 9
Fecha de entrega 21 de octubre del 2025
Morales Soto Fernando
315143977
*/
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 6.0f, 25.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f,  0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f,  0.5f, 0.5f,-0.5f, 0.0f,0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f,0.0f,-1.0f, -0.5f, 0.5f,-0.5f, 0.0f,0.0f,-1.0f, -0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f,

    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, 1.0f,  0.5f,-0.5f, 0.5f, 0.0f,0.0f, 1.0f,  0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f, -0.5f,-0.5f, 0.5f, 0.0f,0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f,-1.0f,0.0f,0.0f,  -0.5f, 0.5f,-0.5f,-1.0f,0.0f,0.0f,  -0.5f,-0.5f,-0.5f,-1.0f,0.0f,0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f,0.0f,0.0f,  -0.5f,-0.5f, 0.5f,-1.0f,0.0f,0.0f,  -0.5f, 0.5f, 0.5f,-1.0f,0.0f,0.0f,

     0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f,   0.5f, 0.5f,-0.5f, 1.0f,0.0f,0.0f,   0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f,
     0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f,   0.5f,-0.5f, 0.5f, 1.0f,0.0f,0.0f,   0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f,

    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,  0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,  0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, -0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, -0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,

    -0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f,   0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f,   0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f,
     0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f,  -0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f,  -0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fernando Morales", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    //Modelos
    Model redDog((char*)"Models/RedDog.obj");
    Model starObj((char*)"Models/obj/objStar.obj");   // estrella (dir light amarilla)
    Model estrella((char*)"Models/Estrella.obj");     // estrella (spot light blanca)
    Model asteroide3((char*)"Models/Rocky_Asteroid_3.obj");
    Model asteroide1((char*)"Models/Asteroid_1.obj");
    Model luna((char*)"Models/Moon 2K.obj");
    Model tierra((char*)"Models/Earth.obj");
    Model saturno((char*)"Models/Stylized_Planets.obj");
    

    // VAO/VBO para cubo (marcador luz puntual)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Samplers
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Posiciones de emisores
    const glm::vec3 earthPos = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 objStarPos = glm::vec3(6.0f, 11.0f, -8.0f);
    const glm::vec3 estrellaPos = glm::vec3(-14.5f, 16.5f, -3.0f);
    const glm::vec3 saturnPos = glm::vec3(-8.0f, 15.0f, 0.0f); // <- para orientar la spotlight

    glEnable(GL_DEPTH_TEST);

    // Uniform locations para material emisivo
    const GLint uModel = glGetUniformLocation(lightingShader.Program, "model");
    const GLint uNormalMatrix = glGetUniformLocation(lightingShader.Program, "normalMatrix");
    const GLint uView = glGetUniformLocation(lightingShader.Program, "view");
    const GLint uProj = glGetUniformLocation(lightingShader.Program, "projection");
    const GLint uViewPos = glGetUniformLocation(lightingShader.Program, "viewPos");

    const GLint uEmisColor = glGetUniformLocation(lightingShader.Program, "material.emissiveColor");
    const GLint uEmisStrength = glGetUniformLocation(lightingShader.Program, "material.emissiveStrength");
    const GLint uRimStrength = glGetUniformLocation(lightingShader.Program, "material.rimStrength");
    const GLint uRimPower = glGetUniformLocation(lightingShader.Program, "material.rimPower");
    const GLint uShininess = glGetUniformLocation(lightingShader.Program, "material.shininess");
    const GLint uTransparency = glGetUniformLocation(lightingShader.Program, "transparency");

    auto sendModel = [&](const glm::mat4& M) {
        glm::mat3 N = glm::mat3(glm::transpose(glm::inverse(M)));
        glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix3fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(N));
    };

    auto setEmissive = [&](const glm::vec3& c, float e, float rimS, float rimP) {
        glUniform3f(uEmisColor, c.r, c.g, c.b);
        glUniform1f(uEmisStrength, e);
        glUniform1f(uRimStrength, rimS);
        glUniform1f(uRimPower, rimP);
    };

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // delta
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // clear
        glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader activo y matrices cámara
        lightingShader.Use();
        glUniform3f(uViewPos, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(uShininess, 16.0f);
        glUniform1i(uTransparency, 0);

        // Luces de la escena 

        // DIR LIGHT (objStar) AMARILLO
        {
            glm::vec3 dir = glm::normalize(-objStarPos);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), dir.x, dir.y, dir.z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.06f, 0.05f, 0.02f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.30f, 1.20f, 0.40f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.00f, 0.90f, 0.30f);
        }

        // POINT LIGHT (Tierra)AZUL
        {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
                earthPos.x, earthPos.y, earthPos.z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.03f, 0.05f, 0.12f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.20f, 0.40f, 1.20f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.60f, 0.80f, 1.00f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.014f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0007f);
        }

        // SPOT LIGHT (Estrella.obj)  BLANCO 
        {
            //  orientamos hacia saturno
            glm::vec3 sdir = glm::normalize(saturnPos - estrellaPos);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
                estrellaPos.x, estrellaPos.y, estrellaPos.z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
                sdir.x, sdir.y, sdir.z);

            // cono amplio 
            glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
                glm::cos(glm::radians(20.0f)));
            glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
                glm::cos(glm::radians(28.0f)));

            // blanco intenso
            glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.03f, 0.03f, 0.03f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 2.20f, 2.20f, 2.20f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 2.00f, 2.00f, 2.00f);

            // atenuación suave 
            glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.007f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0002f);
        }

        // DIBUJO DE MODELOS 
        glm::mat4 model(1.0f);

        // Perro 
        setEmissive(glm::vec3(0.0f), 0.0f, 0.0f, 2.0f);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        redDog.Draw(lightingShader);

        // Tierra ( AZUL)
        setEmissive(glm::vec3(0.15f, 0.35f, 1.00f), 0.28f, 0.35f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, earthPos);
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        tierra.Draw(lightingShader);

        // Saturno (subimos shininess para ver mejor el highlight de la spotlight)
        glUniform1f(uShininess, 48.0f);
        setEmissive(glm::vec3(0.0f), 0.0f, 0.0f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, saturnPos);
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        saturno.Draw(lightingShader);
        // restaurar shininess para el resto
        glUniform1f(uShininess, 16.0f);

        // Luna 
        setEmissive(glm::vec3(0.0f), 0.0f, 0.0f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.0f, 15.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        luna.Draw(lightingShader);

        // Asteroide1 
        setEmissive(glm::vec3(0.0f), 0.0f, 0.0f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 8.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        asteroide1.Draw(lightingShader);

        // Asteroide3 
        setEmissive(glm::vec3(0.0f), 0.0f, 0.0f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.0f, 8.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        sendModel(model);
        asteroide3.Draw(lightingShader);

        // objStar ( AMARILLO)
        setEmissive(glm::vec3(1.30f, 1.20f, 0.40f), 0.65f, 0.50f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, objStarPos);
        model = glm::scale(model, glm::vec3(0.8f));
        sendModel(model);
        starObj.Draw(lightingShader);

        // Estrella.obj (BLANCO)
        setEmissive(glm::vec3(1.0f, 1.0f, 1.0f), 0.80f, 0.60f, 3.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, estrellaPos);
        model = glm::scale(model, glm::vec3(0.22f));
        sendModel(model);
        estrella.Draw(lightingShader);

        // Marcador cubo (luz puntual de la Tierra) 
        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glm::mat4 m = glm::mat4(1);
        m = glm::translate(m, earthPos);
        m = glm::scale(m, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])   camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Key callback
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        Light1 = active ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(0.0f);
    }
}

// Mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

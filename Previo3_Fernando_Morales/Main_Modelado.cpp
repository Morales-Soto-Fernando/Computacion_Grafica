/*
Practica 4
Fecha de entrega 3 de Septiembre del 2025
Morales Soto Fernando
315143977
*/
#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Fernando Morales", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
      
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO

	// 🔹 Lambdas para manejar color constante por pieza
	auto SetColor = [&](float r, float g, float b) {
		glDisableVertexAttribArray(1);   // deja de leer color del VBO
		glVertexAttrib3f(1, r, g, b);    // usa este color constante
	};
	auto UseVBOColors = [&]() {
		glEnableVertexAttribArray(1);    // volver a usar colores del VBO (si algún día lo necesitas)
	};

	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(glm::radians(45.0f),
		(GLfloat)screenWidth / (GLfloat)screenHeight,
		0.1f, 100.0f);

	//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.5f, 1.0f, 0.83f, 1.0f); //se cambio el color a agua marina
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
	    //Pinguino
		// Cabeza
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
			model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.125f, 0.223f, 0.31f); // azul marino
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Pecho
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.10f, 0.15f));
			model = glm::scale(model, glm::vec3(1.05f, 0.9f, 0.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(1.0f, 1.0f, 1.0f); // blanco
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Detalle pecho - rectángulo superior 
		{
			glm::mat4 model = glm::mat4(1.0f);
			// más bajo (zona blanca) y casi coplanar con el frente del pecho
			model = glm::translate(model, glm::vec3(0.0f, 0.20f, 0.660f)); // Z pegado (evita z-fighting)
			model = glm::scale(model, glm::vec3(0.36f, 0.10f, 0.020f));  // muy delgado en Z
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.91f, 0.78f, 0.35f); // dorado
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

    // Detalle pecho - cuadrado inferior 
{
    glm::mat4 model = glm::mat4(1.0f);
    // un poco más abajo que el rectángulo superior, centrado
    model = glm::translate(model, glm::vec3(0.0f,  0.08f, 0.660f));
    model = glm::scale(model,     glm::vec3(0.18f, 0.18f, 0.020f));  // delgado en Z
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    SetColor(0.91f, 0.78f, 0.35f); // dorado
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


		// Ala derecha
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, 0.20f, 0.0f));
			model = glm::scale(model, glm::vec3(0.35f, 0.8f, 0.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.125f, 0.223f, 0.31f); // azul marino
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Extensión ala derecha (alineada con el borde inferior y cara trasera)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// x igual al ala, y calculada para alinear el borde inferior, z centrado para que su cara delantera toque la cara trasera del ala
			model = glm::translate(model, glm::vec3(0.75f, 0.05f, -0.40f));
			// más angosta y corta
			model = glm::scale(model, glm::vec3(0.30f, 0.50f, 0.20f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.125f, 0.223f, 0.31f); // azul marino
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}




		// Ala izquierda
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.75f, 0.20f, 0.0f));
			model = glm::scale(model, glm::vec3(0.35f, 0.8f, 0.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.125f, 0.223f, 0.31f); // azul marino
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Extensión ala izquierda (alineada con el borde inferior y cara trasera)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.75f, 0.05f, -0.40f));
			model = glm::scale(model, glm::vec3(0.30f, 0.50f, 0.20f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.125f, 0.223f, 0.31f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Pico
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.66f, 0.76f));
			model = glm::scale(model, glm::vec3(0.28f, 0.14f, 0.42f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.96f, 0.48f, 0.16f); // naranja
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Gancho del pico 
		{
			glm::mat4 model = glm::mat4(1.0f);
			// ubicado al frente del pico principal
			model = glm::translate(model, glm::vec3(0.0f, 0.60f, 0.88f));
			// muy delgado en Y y un poco más estrecho en Z
			model = glm::scale(model, glm::vec3(0.20f, 0.08f, 0.18f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.96f, 0.48f, 0.16f); // naranja
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		// Patita izquierda
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.35f, -0.25f, 0.7f));
			model = glm::scale(model, glm::vec3(0.4f, 0.15f, 0.35f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.96f, 0.48f, 0.16f); // naranja
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.35f, -0.25f, 0.7f));
			model = glm::scale(model, glm::vec3(0.4f, 0.15f, 0.35f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			SetColor(0.96f, 0.48f, 0.16f); // naranja
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	
        // Ojo derecho  + brillo
		{
		    //negro 
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.65f, 0.98f, 0.22f)); // pegado al borde +X
			model = glm::scale(model, glm::vec3(0.12f, 0.24f, 0.12f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.02f, 0.02f, 0.03f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Brillo blanco
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.65f, 1.05f, 0.28f));
			model = glm::scale(model, glm::vec3(0.05f, 0.08f, 0.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Ojo izquierdo  + brillo
		{
			// Prisma negro
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.65f, 0.98f, 0.22f)); // pegado al borde -X
			model = glm::scale(model, glm::vec3(0.12f, 0.24f, 0.12f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.02f, 0.02f, 0.03f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Brillo blanco
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.65f, 1.05f, 0.28f));
			model = glm::scale(model, glm::vec3(0.05f, 0.08f, 0.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Pestañas ojo izquierdo
		{
			// superior
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.66f, 1.00f, 0.16f)); // un poco detrás en Z
			model = glm::scale(model, glm::vec3(0.14f, 0.06f, 0.06f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.91f, 0.78f, 0.35f); // amarillo
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// inferior
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.66f, 0.92f, 0.16f));
			model = glm::scale(model, glm::vec3(0.14f, 0.06f, 0.06f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.91f, 0.78f, 0.35f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Pestañas ojo derecho
		{
			// superior
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.66f, 1.00f, 0.16f)); // un poco detrás en Z
			model = glm::scale(model, glm::vec3(0.14f, 0.06f, 0.06f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.91f, 0.78f, 0.35f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// inferior
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.66f, 0.92f, 0.16f));
			model = glm::scale(model, glm::vec3(0.14f, 0.06f, 0.06f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			SetColor(0.91f, 0.78f, 0.35f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }



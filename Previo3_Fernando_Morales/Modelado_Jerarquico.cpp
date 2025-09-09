/*
Practica 5
Fecha de entrega 9 de septiembre del 2025
Morales Soto Fernando
315143977
*/
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f;
float codo = 0.0f;
float muneca = 0.0f;
float dedo1 = 0.0f;
float dedo2 = 0.0f;
float dedo3 = 0.0f;
float dedo4 = 0.0f,
dedo5 = 0.0f,
dedo6 = 0.0f,
dedo7 = 0.0f,
dedo8 = 0.0f,
dedo9 = 0.0f,
dedo10 = 0.0f,
dedo11 = 0.0f,
dedo12 = 0.0f,
dedo13 = 0.0f,//pulgar A
dedo14 = 0.0f;//pulgar B
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Fernando Morales", nullptr, nullptr);

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
	// use with Orthographic Projection


	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
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
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		// la jerarquia en opengl va segun cual agregues primero
		//Model 
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm:: rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B
		
		// Model palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = modelTemp=model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C
		//Model dedo1 A
	
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.421875f, 0.421875f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar (en x)
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//D
		//Model dedo1 B
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo1 B
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar (en x)
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 0.0f, 1.0f);//coloreo de magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//E
		//Model Dedo1 C
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo1 C
		model = glm::translate(model, glm::vec3(0.125f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar (en x)
		model = glm::scale(model, glm::vec3(0.25f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 1.0f, 0.0f);//coloreo de amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//F

		//Dedo2
		//Model Dedo2 A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.421875f, 0.140625f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo2 A
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(0.0f, 1.0f, 1.0f);//coloreo de cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//G

		//Model Dedo2 B
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo2 B
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 0.0f, 1.0f);//coloreo de magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//H

		//Model Dedo2 C
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo6), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo2 C
		model = glm::translate(model, glm::vec3(0.125f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.25f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 1.0f, 0.0f);//coloreo de amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//I	

		//Dedo3
			//Model Dedo3 A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.421875f, -0.140625f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo7), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo3 A
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(0.0f, 1.0f, 1.0f);//coloreo de cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//J

		//Model Dedo3 B
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo8), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo3 B
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 0.0f, 1.0f);//coloreo de magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//K

		//Model Dedo3 C
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo9), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo3 C
		model = glm::translate(model, glm::vec3(0.125f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.25f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 1.0f, 0.0f);//coloreo de amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//L	

		//Dedo4
			//Model Dedo4 A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.421875f, -0.421875f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo10), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo4 A
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(0.0f, 1.0f, 1.0f);//coloreo de cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//M

		//Model Dedo4 B
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo11), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo4 B
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 0.0f, 1.0f);//coloreo de magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//N

		//Model Dedo4 C
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo12), glm::vec3(0.0f, 0.0f, 1.0f)); //dedo4 C
		model = glm::translate(model, glm::vec3(0.125f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.25f, 0.15625f, 0.15625f));//dibujo caja
		color = glm::vec3(1.0f, 1.0f, 0.0f);//coloreo de amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//O


		//Dedo5
			//Model Dedo5 A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.3828125, 0.3828125));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo13), glm::vec3(0.0f, 1.0f, 0.0f)); //dedo5 A
		modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.5f, 0.234375f, 0.234375f));//dibujo caja
		color = glm::vec3(0.0f, 1.0f, 1.0f);//coloreo de cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//P

		//Model Dedo5 B
		model = glm::translate(modelTemp, glm::vec3(0.25f, 0.0f, 0.0f));//me muevo al final de la caja para dibujar la otra caja
		model = glm::rotate(model, glm::radians(dedo14), glm::vec3(0.0f, 1.0f, 0.0f)); //dedo5 B
		modelTemp = model = glm::translate(model, glm::vec3(0.125f, 0.0f, 0.0f));//para ubicarse en el centro de la figura a dibujar
		model = glm::scale(model, glm::vec3(0.25f, 0.234375f, 0.234375f));//dibujo caja
		color = glm::vec3(1.0f, 0.0f, 1.0f);//coloreo de magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//Q









		// Swap the screen buffers
		glfwSwapBuffers(window);





	
		glBindVertexArray(0);
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
	 if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.18f;
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		 hombro += 0.1f;
		 if (hombro > 90.0f) //hombro no puede ser mayor a 90° para arriba
			 hombro = 90.0f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		 hombro -= 0.1f;
		 if (hombro < -90.0f) //hombro no puede ser menor a -90° para abajo
			 hombro = -90.0f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		 codo += 0.1f;
		 if (codo > 0.0f) //codo no puede ser mayor a 0°
			 codo = 0.0f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		 codo -= 0.1f;
		 if (codo < -90.0f) //codo no puede ser menor a -90°
			 codo = -90.0f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		 muneca += 0.1f;
		 if (muneca > 90.0f) //muneca no puede ser mayor a 90°
			 muneca = 90.0f;
	 }
	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		 muneca -= 0.1f;
		 if (muneca < -90.0f) //muneca no puede ser menor a -90°
			 muneca = -90.0f;
	 }
	 //Falange proximal
	 if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		 dedo1 += 0.1f;
		 dedo4 += 0.2f;
		 dedo7 += 0.3f;
		 dedo10 += 0.4f;
		 if (dedo1 > 6.0f) dedo1 = 6.0f; //dedo1 no puede ser mayor a 6°
		 if (dedo4 > 6.0f) dedo4 = 6.0f; //dedo1 no puede ser mayor a 6°
		 if (dedo7 > 6.0f) dedo7 = 6.0f; //dedo1 no puede ser mayor a 6°
		 if (dedo10 > 6.0f) dedo10 = 6.0f; //dedo1 no puede ser mayor a 6°
	 }
	 if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		 dedo1 -= 0.1f;
		 dedo4 -= 0.2f;
		 dedo7 -= 0.3f;
		 dedo10 -= 0.4f;
		 if (dedo1 < -12.0f) dedo1 = -12.0f; //dedo1 no puede ser menor a -12°
		 if (dedo4 < -12.0f) dedo4 = -12.0f; //dedo4 no puede ser menor a -12°
		 if (dedo7 < -12.0f) dedo7 = -12.0f; //dedo7 no puede ser menor a -12°
		 if (dedo10 < -12.0f) dedo10 = -12.0f; //dedo10 no puede ser menor a -12°
	 }
	 //Falange intermedia
	 if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		 dedo2 += 0.1f;
		 dedo5 += 0.1f;
		 dedo8 += 0.1f;
		 dedo11 += 0.1f;
		 if (dedo2 > 0.0f) dedo2 = 0.0f; //dedo2 no puede ser mayor a 0°
		 if (dedo5 > 0.0f) dedo5 = 0.0f; //dedo5 no puede ser mayor a 0°
		 if (dedo8 > 0.0f) dedo8 = 0.0f; //dedo8 no puede ser mayor a 0°
		 if (dedo11 > 0.0f) dedo11 = 0.0f; //dedo11 no puede ser mayor a 0°
	 }
	 if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		 dedo2 -= 0.1f;
		 dedo5 -= 0.1f;
		 dedo8 -= 0.1f;
		 dedo11 -= 0.1f;
		 if (dedo2 < -105.0f) dedo2 = -105.0f; //dedo2 no puede ser menor a -105°
		 if (dedo5 < -105.0f) dedo5 = -105.0f; //dedo5 no puede ser menor a -105°
		 if (dedo8 < -105.0f) dedo8 = -105.0f; //dedo8 no puede ser menor a -105°
		 if (dedo11 < -105.0f) dedo11 = -105.0f; //dedo11 no puede ser menor a -105°
	 }
	 //Falange distal
	 if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		 dedo3 += 0.1f;
		 dedo6 += 0.1f;
		 dedo9 += 0.1f;
		 dedo12 += 0.1f;
		 if (dedo3 > 0.0f) dedo3 = 0.0f; //dedo2 no puede ser mayor a 0°
		 if (dedo6 > 0.0f) dedo6 = 0.0f; //dedo6 no puede ser mayor a 0°
		 if (dedo9 > 0.0f) dedo9 = 0.0f; //dedo9 no puede ser mayor a 0°
		 if (dedo12 > 0.0f) dedo12 = 0.0f; //dedo12 no puede ser mayor a 0°
	 }
	 if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		 dedo3 -= 0.1f;
		 dedo6 -= 0.1f;
		 dedo9 -= 0.1f;
		 dedo12 -= 0.1f;
		 if (dedo3 < -90.0f) dedo3 = -90.0f; //dedo2 no puede ser menor a -90°
		 if (dedo6 < -90.0f) dedo6 = -90.0f; //dedo6 no puede ser menor a -90°
		 if (dedo9 < -90.0f) dedo9 = -90.0f; //dedo9 no puede ser menor a -90°
		 if (dedo12 < -90.0f) dedo12 = -90.0f; //dedo12 no puede ser menor a -90°
	 }
	 //Falange proximal del pulgar
	 if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		 dedo13 += 0.1f;
		 if (dedo13 > 38.0f) dedo13 = 38.0f; //dedo13 no puede ser mayor a 38°
	 }
	 if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		 dedo13 -= 0.1f;
		 if (dedo13 < -6.0f) dedo13 = -6.0f; //dedo13 no puede ser menor a -6°
	 }
	 //Falange distal del pulgar
	 if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		 dedo14 += 0.1f;
		 if (dedo14 > 90.0f) dedo14 = 90.0f; //dedo14 no puede ser mayor a 90°
	 }
	 if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		 dedo14 -= 0.1f;
		 if (dedo14 < 0.0f) dedo14 = 0.0f; //dedo14 no puede ser menor a 0°
	 }
 }


 
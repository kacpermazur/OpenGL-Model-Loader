#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "ModelLoader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error: GLEW didnt initilize" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f,
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Add<float>(2);
		layout.Add<float>(2);
		
		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);
		
		IndexBuffer indexBuffer(indices, 6);

		Shader shader("res/shaders/Default.shader");
		shader.bind();

		Model test = ModelLoader::Load("res/models/Creeper/Creeper.obj");
		test.Print();
		

		//Texture texture("res/textures/chungus.png");
		//texture.Bind(0);
		
		Renderer renderer;

		//ToDO: Move This Out to Mesh/Gameobject and Camera
		glm::mat4 Projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
		glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); // Mesh
		
		glm::mat4 ModelViewProjection = Projection * View * Model;
		
		shader.bind();
		shader.SetUniformMat4f("u_MVP", ModelViewProjection);
		
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			
			renderer.Draw(vertexArray, indexBuffer, shader);

			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}

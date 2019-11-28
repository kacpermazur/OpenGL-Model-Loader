#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
//#include "ModelLoader.h"
#include "ModerLoaderRefactor.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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

		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		

		//("res/models/Creeper/Creeper.obj"); ("res/models/LowPolyBoat/low_poly_boat.obj"

		std::vector<unsigned int> indices1;
		
		auto a = MeshLoader::Load("res/models/Creeper/Creeper.obj", indices1);

		VertexArray va;
		VertexBuffer vb(&a[0], sizeof(Vertex) * a.size());
		//VertexBuffer vb(vertices, sizeof(GLfloat) * 108);
		
		VertexBufferLayout layout;
		
		layout.Add<float>(3);
		layout.Add<float>(2);
		layout.Add<float>(3);
		va.AddBuffer(vb, layout);
	
		IndexBuffer ib(&indices1[0], indices1.size());
		
		Shader shader("res/shaders/Default.shader");
		shader.bind();

		//shader.SetUniform4f("u_Color", 0.5f, 0.3f, 0.0f, 1.0f);

		Texture texture("res/models/Creeper/Texture.png");
		texture.Bind(0);
		shader.SetUniform1i("u_Texture", 0);
		
		Renderer renderer;

		//ToDO: Move This Out to Mesh/Gameobject and Camera
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
		
		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)); // Camera
		
		glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
		
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		
		while (!glfwWindowShouldClose(window))
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glFrontFace(GL_CW);
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			
			renderer.Clear();

			Model = glm::rotate(Model, glm::radians(deltaTime * 10.0f), glm::vec3(0.0, 1.0f, 0.0f));
			glm::mat4 ModelViewProjection = Projection * View * Model;
			shader.bind();
			shader.SetUniformMat4f("u_MVP", ModelViewProjection);
			
			renderer.Draw(va, ib, shader);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}

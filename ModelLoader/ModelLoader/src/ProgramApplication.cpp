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

		std::vector <glm::vec4> duck;
		duck.push_back(glm::make_vec4(positions));
		duck.push_back(glm::make_vec4(positions + 4));
		duck.push_back(glm::make_vec4(positions + 8));
		duck.push_back(glm::make_vec4(positions + 12));

		std::cout << "=====================================" << std::endl;
		for(auto& vec : duck)
		{
			std::cout << glm::to_string(vec) << std::endl;
		}
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Model test = ModelLoader::Load("res/models/Creeper/Creeper.obj");
		test.Print();

		VertexArray va;
		VertexBuffer vb(&test.vertices[0].x, sizeof(glm::vec3) * test.vertices.size());
		VertexBufferLayout layout;
		
		layout.Add<float>(3);
		//layout.Add<float>(2);
		va.AddBuffer(vb, layout);
		
		
		IndexBuffer ib(&test.vertexIndex[0], test.vertexIndex.size());

		Shader shader("res/shaders/Default.shader");
		shader.bind();

		//Texture texture("res/textures/chungus.png");
		//texture.Bind(0);
		
		Renderer renderer;

		//ToDO: Move This Out to Mesh/Gameobject and Camera
		glm::mat4 Projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
		glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)); // Mesh

		Model = glm::scale(Model, glm::vec3(100.0f, 100.0f, 100.0f));
		
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		
		while (!glfwWindowShouldClose(window))
		{

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			
			renderer.Clear();

			glm::mat4 ModelViewProjection = Projection * View * Model;
			Model = glm::rotate(Model, glm::radians(deltaTime), glm::vec3(1.0f, 0.0f, 0.0f));
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

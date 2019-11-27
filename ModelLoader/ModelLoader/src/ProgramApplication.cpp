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
		/*
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
		*/

		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, //0 b l
			 0.5f, -0.5f, -0.5f, //1 b r
			 0.5f,  0.5f, -0.5f, //2 t r
			 0.5f,  0.5f, -0.5f, //3 t r
			-0.5f,  0.5f, -0.5f, //4 t l 
			-0.5f, -0.5f, -0.5f, //5 b l

			-0.5f, -0.5f,  0.5f, //6 b l
			 0.5f, -0.5f,  0.5f, //7 b r
			 0.5f,  0.5f,  0.5f, //8 t r
			 0.5f,  0.5f,  0.5f, //9 t r 
			-0.5f,  0.5f,  0.5f, //10 t l
			-0.5f, -0.5f,  0.5f, //11 b l

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f, // b l b
			 0.5f, -0.5f, -0.5f, // b r b
			 0.5f, -0.5f,  0.5f, // b r f
			 0.5f, -0.5f,  0.5f, // b r b
			-0.5f, -0.5f,  0.5f, // b l f
			-0.5f, -0.5f, -0.5f, // b l b

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		GLuint indices[] = {  // note that we start from 0!
			0, 1, 2,  // first Triangle front
			3, 4, 5,   // second Triangle

			8, 7, 6 ,
			11, 10, 9,

			14, 13, 12,
			17, 16, 15,

			18, 19, 20,
			21, 22, 23,

			26, 25, 24,
			29, 28, 27,

			30, 31, 32,  // first Triangle back
			33, 34, 35    // second Triangle
		};
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Model test = ModelLoader::Load("res/models/Creeper/Creeper.obj");
		//test.Print();

		std::vector<unsigned int> indices1;
		
		auto a = MeshLoader::Load("res/models/LowPolyBoat/low_poly_boat.obj", indices1);
		
		//auto a = test.GetVerticesIndex();
		
		//std::cout << test.GetVerticesIndex().size() << std::endl;

		VertexArray va;
		VertexBuffer vb(&a[0], sizeof(Vertex) * a.size());
		//VertexBuffer vb(vertices, sizeof(GLfloat) * 108);
		
		VertexBufferLayout layout;
		
		layout.Add<float>(3);
		layout.Add<float>(2);
		layout.Add<float>(3);
		va.AddBuffer(vb, layout);
		
		/* 
		unsigned int abc[] =
		{
			2, 4, 1,
			6, 8, 5,
			4, 8, 1,
			5, 3, 6,
			1, 7, 2,
			2, 6, 3,
			2, 3, 4,
			6, 7, 8,
			4, 5, 8,
			5, 4, 3,
			1, 8, 7,
			2, 7, 6
		};
		*/
		
		IndexBuffer ib(&indices1[0], indices1.size());
		//IndexBuffer ib(&test.vertexIndex[0], test.vertexIndex.size());
		//IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Default.shader");
		shader.bind();

		//Texture texture("res/textures/chungus.png");
		//texture.Bind(0);
		
		Renderer renderer;

		//ToDO: Move This Out to Mesh/Gameobject and Camera
		//glm::mat4 Projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, 0.1f, 100.0f);
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);
		
		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -3.0f, 0.0f)); // Camera
		
		glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		//= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)); // Mesh

		//Model = glm::scale(Model, glm::vec3(2.0f, 2.0f, 2.0f));
		
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		
		while (!glfwWindowShouldClose(window))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			
			renderer.Clear();

			Model = glm::rotate(Model, glm::radians(deltaTime * 3.0f), glm::vec3(0.0, 1.0f, 0.0f));
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

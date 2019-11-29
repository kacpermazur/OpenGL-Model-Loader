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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool Clear = false;

void Unbind(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib, Shader& shader, Texture& texture)
{
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	texture.Unbind();
	shader.Unbind();
	
}

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
		//auto mesh = Loader::LoadMesh("res/models/LowPolyBoat/low_poly_boat.obj");
		auto mesh = Loader::LoadMesh("res/models/Creeper/Creeper.obj");
		//mesh.materials[0].View();

		VertexArray va;
		VertexBuffer vb(&mesh.vertices[0], sizeof(Vertex) * mesh.vertices.size());
		VertexBufferLayout layout;

		layout.Add<float>(3);
		layout.Add<float>(2);
		layout.Add<float>(3);
		va.AddBuffer(vb, layout);
		IndexBuffer ib(&mesh.indices[0], mesh.indices.size());

		Shader shader("res/shaders/Default.shader");
		shader.bind();

		shader.SetUniform4f("u_Color", mesh.materials[0].ambientColor);

		Texture texture("res/models/Creeper/Texture.png");
		texture.Bind(0);
		//shader.SetUniform1i("u_Texture", 0);
		
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		//ToDO: Move This Out to Mesh/Gameobject and Camera
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

		glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)); // Camera

		glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		

		glm::vec3 translateA(0.0f, 0.0f, -4.0f);
		glm::vec3 rot(0.0f, 1.0f, 0.0f);
		
		while (!glfwWindowShouldClose(window))
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glFrontFace(GL_CW);
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glfwSetKeyCallback(window, key_callback);
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();

			shader.bind();
			
			//glm::mat4 Model = glm::translate(glm::mat4(1.0f), translateA);
			View = glm::translate(glm::mat4(1.0f), translateA);


			
			Model = glm::rotate(Model, glm::radians(deltaTime * 15.0f), rot);

				glm::mat4 ModelViewProjection = Projection * View * Model;
				shader.SetUniformMat4f("u_MVP", ModelViewProjection);
			
			
			
			renderer.Draw(va, ib, shader);

			{
				ImGui::SliderFloat3("Camera", &translateA.x, -5.0f, 5.0f);
				ImGui::SliderFloat3("Rotation", &rot.x, -1.0f, 2.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			

			if (Clear == true)
			{
				Unbind(va, vb, ib, shader, texture);
				Clear = false;
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "CLEARING MODELS" << std::endl;
		Clear = true;
		
	}
}
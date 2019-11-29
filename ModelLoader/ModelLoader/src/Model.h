#pragma once
#include "ModelLoader.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"


class Model
{
private:
	Mesh m_mesh;

	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	VertexBufferLayout m_vertexBufferLayout;

	Shader m_shader;
	Texture m_texture;

	glm::vec3 m_model;
	
public:
	 Model(const Mesh& mesh);

	 void Update(float& deltaTime);
	
	 void Translate(glm::vec3& vector);
	 void Rotate(glm::vec3& vector, float speed);
	 void Scale(float factor);

	 void SetTexture(const std::string& path);

	 void Bind();

private:
	void Setup();
};

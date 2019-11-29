#include "Model.h"


Model::Model(const Mesh& mesh)
	: m_mesh(mesh), m_vertexArray(), m_vertexBuffer(&m_mesh.vertices[0], sizeof(Mesh)* mesh.vertices.size()),
	m_vertexBufferLayout(), m_indexBuffer(&m_mesh.indices[0], m_mesh.indices.size()), m_shader("res/shaders/Default.shader"),
	m_texture("")
{
	m_vertexBufferLayout.Add<float>(3);
	m_vertexBufferLayout.Add<float>(2);
	m_vertexBufferLayout.Add<float>(3);

	m_vertexArray.AddBuffer(m_vertexBuffer, m_vertexBufferLayout);
}

void Model::Update(float& deltaTime)
{
}

void Model::Translate(glm::vec3& vector)
{
}

void Model::Rotate(glm::vec3& vector, float speed)
{
}

void Model::Scale(float factor)
{
}

void Model::SetTexture(const std::string& path)
{
	m_texture.SetPath(path);
	m_texture.Bind();
	
	m_shader.bind();
	m_shader.SetUniform1i("u_Texture", 0);
	
}

void Model::Bind()
{
	
}

void Model::Setup()
{
	m_shader.bind();;

}

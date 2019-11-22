#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::TestDraw(const VertexBuffer& vb, const Shader& shader, unsigned long long& count)
{
	vb.Bind();
	shader.bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

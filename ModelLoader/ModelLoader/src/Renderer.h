#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	void TestDraw(unsigned long long& count);
};
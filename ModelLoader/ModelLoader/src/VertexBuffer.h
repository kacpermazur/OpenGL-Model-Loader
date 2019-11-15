#pragma once

class VertexBuffer
{
private:
	unsigned int m_RenderID;
	
public:

	VertexBuffer();
	~VertexBuffer();

	void Bind();
	void UnBind();
};
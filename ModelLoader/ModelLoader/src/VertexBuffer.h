#pragma once

class VertexBuffer
{
private:
	unsigned int m_BindID;
	
public:

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Set(const unsigned int* data, unsigned int size);
	
	void Bind() const;
	void Unbind() const;
};
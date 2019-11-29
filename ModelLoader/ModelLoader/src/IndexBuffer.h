#pragma once

class IndexBuffer
{
private:
	unsigned int m_BindID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count); 
	~IndexBuffer();

	void Set(const unsigned int* data, unsigned int count);
	
	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
};
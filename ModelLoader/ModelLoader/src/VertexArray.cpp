#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_BindID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_BindID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.amountOfProperties, element.dataType, element.isNormalized, layout.GetStride(), (const void*)offset);
		offset += element.amountOfProperties * VertexBufferElement::GetSizeOfType(element.dataType);
	}
}


void VertexArray::Bind() const
{
	glBindVertexArray(m_BindID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

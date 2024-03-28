#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void LayoutVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void UnBind() const;
private:
	unsigned int m_RendererID;
};

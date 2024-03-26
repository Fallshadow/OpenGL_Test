#pragma once

class VertexBuffer
{
public:
	void Bind() const;
	void UnBind() const;

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
private:
	unsigned int m_RendererID;
};


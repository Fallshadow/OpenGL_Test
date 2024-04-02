#pragma once

class VertexBuffer
{
public:
	void BindDynamic(const void* data, unsigned int size);
	void Bind() const;
	void UnBind() const;

	VertexBuffer(unsigned int size);
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
private:
	unsigned int m_RendererID;
};


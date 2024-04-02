#include "TestBatchRenderDynamicIndex.h"

#include "CheckError.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>
#include "Renderer.h"

namespace test
{
	struct Vec2
	{
		float x, y;
	};

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		float TexID;
	};

	static Vertex* CreateQuad(Vertex* target, float x, float y, float size, float texIndex)
	{
		target->Position = { x, y, 0.0f };
		target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		target->TexCoords = { 0.0f,0.0f };
		target->TexID = texIndex;
		target++;

		target->Position = { x + size, y, 0.0f };
		target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		target->TexCoords = { 1.0f,0.0f };
		target->TexID = texIndex;
		target++;

		target->Position = { x + size, y + size, 0.0f };
		target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		target->TexCoords = { 1.0f,1.0f };
		target->TexID = texIndex;
		target++;

		target->Position = { x, y + size, 0.0f };
		target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		target->TexCoords = { 0.0f,1.0f };
		target->TexID = texIndex;
		target++;

		return target;
	}


	static std::array<Vertex, 4> CreateQuad(float x, float y, float size, float texIndex)
	{
		Vertex v0, v1, v2, v3;
		v0.Position = { x, y, 0.0f };
		v0.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		v0.TexCoords = { 0.0f,0.0f };
		v0.TexID = texIndex;

		v1.Position = { x + size, y, 0.0f };
		v1.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		v1.TexCoords = { 1.0f,0.0f };
		v1.TexID = texIndex;

		v2.Position = { x + size, y + size, 0.0f };
		v2.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		v2.TexCoords = { 1.0f,1.0f };
		v2.TexID = texIndex;

		v3.Position = { x, y + size, 0.0f };
		v3.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		v3.TexCoords = { 0.0f,1.0f };
		v3.TexID = texIndex;

		return { v0,v1,v2,v3 };
	}


	TestBatchRenderDynamicIndex::TestBatchRenderDynamicIndex()
	{
		const size_t MaxQuadCount = 1000;
		const size_t MaxVertexCount = MaxQuadCount * 4;
		const size_t MaxIndexCount = MaxQuadCount * 6;

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		// 先申请动态最大空间
		m_VertexBuffer = std::make_unique<VertexBuffer>(1000 * sizeof(Vertex));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VAO->LayoutVertexBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

		m_Shader = std::make_unique<Shader>("res/shaders/BatchTexture.shader");
		m_Shader->Bind();

		m_Texture[0] = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Texture[1] = std::make_unique<Texture>("res/textures/Pao.jpg");
		for (size_t i = 0; i < 2; i++)
		{
			m_Texture[i]->Bind(i);
		}
		int samplers[2] = { 0, 1 };
		m_Shader->SetUniform1iv("u_Textures", 2, samplers);
	}

	TestBatchRenderDynamicIndex::~TestBatchRenderDynamicIndex()
	{
	}

	void TestBatchRenderDynamicIndex::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderDynamicIndex::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		uint32_t indexCount = 0;
		std::array<Vertex, 1000> vertices;
		Vertex* buffer = vertices.data();
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				buffer = CreateQuad(buffer, x * 0.1f, y * 0.1f,0.1f, (x + y) % 2);
				indexCount += 6;
			}
		}

		buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 0.2f, 0);
		indexCount += 6;

		// 这里才是实际数据赋值点
		m_VertexBuffer->BindDynamic(vertices.data(), vertices.size() * sizeof(Vertex));
		

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, indexCount);
	}

	void TestBatchRenderDynamicIndex::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.01f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
#include "TestBatchRenderDynamicVertex.h"

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


	TestBatchRenderDynamicVertex::TestBatchRenderDynamicVertex()
	{
		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

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

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 4);

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

	TestBatchRenderDynamicVertex::~TestBatchRenderDynamicVertex()
	{
	}

	void TestBatchRenderDynamicVertex::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderDynamicVertex::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));


		auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0.2f, 0);
		auto q1 = CreateQuad(0.3f, 0.0f, 0.3f, 1);

		Vertex vertices[8];
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
		// 这里才是实际数据赋值点
		m_VertexBuffer->BindDynamic(vertices, sizeof(vertices));

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRenderDynamicVertex::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
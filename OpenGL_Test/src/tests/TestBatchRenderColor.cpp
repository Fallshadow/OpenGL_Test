#include "TestBatchRenderColor.h"

#include "CheckError.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

namespace test
{
	TestBatchRenderColor::TestBatchRenderColor()
	{
		float positions[] = {
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.2f, 0.2f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.2f, 1.0f, 0.0f, 0.0f, 1.0f,

			0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.3f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, (2 + 4) * 8 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		m_VAO->LayoutVertexBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 4);

		m_Shader = std::make_unique<Shader>("res/shaders/BatchColor.shader");
		m_Shader->Bind();
	}

	TestBatchRenderColor::~TestBatchRenderColor()
	{
	}

	void TestBatchRenderColor::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRenderColor::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRenderColor::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
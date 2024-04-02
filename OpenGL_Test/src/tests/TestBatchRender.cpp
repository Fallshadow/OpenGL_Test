#include "TestBatchRender.h"

#include "CheckError.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

namespace test
{
	TestBatchRender::TestBatchRender()
		:m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
		m_Translation(glm::vec3(200, 200, 0))
	{
		float positions[] = {
			100.0f, 100.0f,
			200.0f, 100.0f,
			200.0f, 200.0f,
			100.0f, 200.0f,

			300.0f, 100.0f,
			400.0f, 100.0f,
			400.0f, 200.0f,
			300.0f, 200.0f,

		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 2 * 8 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		m_VAO->LayoutVertexBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 4);

		m_Shader = std::make_unique<Shader>("res/shaders/Batch.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
	}

	TestBatchRender::~TestBatchRender()
	{
	}

	void TestBatchRender::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRender::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestBatchRender::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
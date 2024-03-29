#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	if (!glfwInit()) return -1;

	// glfwWindowHint����һЩ����
	// ���ｫ���汾��(Major)�ʹΰ汾��(Minor)����Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // ����������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// ����800 * 600 �ı���ΪOpenGL�Ĵ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// ����������ͬ��֡�ʣ�������ʾ֡�ʣ�һ֡һ����
	glfwSwapInterval(1);
	// ������Ⱦ��Χͬ��Ϊ���ڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK) 
		std::cout << "Error" << std::endl;

	{
		/**
		* ���:
		* �������ɫ(�ж���ɫ���������ɫ)��Ŀ�껺�������е���ɫ���
		* glEnable/glDisable(����&�ر�) => glBlendFunc(ָ����ɫ����) => glBlendEquation(ָ�����ģʽ)
		* glBlendEquation(mode) mode: src��dest�Ļ�Ϸ�ʽ(Ĭ��GL_FUNC_ADD, ����)
		*
		**/
		/* ���û��(Ĭ�ϲ�������) */
		GLCall(glEnable(GL_BLEND));
		/**
		 * glBlendFunc(src, dest) ָ����ɫ����
		 * src ָ�������ɫ(RGBA)���ӵļ��㷽ʽ, Ĭ��ΪGL_ONE
		 * dest ָ��Ŀ����ɫ���ӵļ��㷽ʽ, Ĭ��ΪGL_ZERO
		 * GL_SRC_ALPHA ��Ϊsrc��alphaΪ0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
		 * RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
		 **/
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::TestClearColor test;

		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			renderer.Clear();

			test.OnUpdate(0.0f);
			test.OnRender();

			ImGui_ImplGlfwGL3_NewFrame();

			{
				test.OnImGuiRender();
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	// �����˳������ô��ڹرձ�ʶ
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
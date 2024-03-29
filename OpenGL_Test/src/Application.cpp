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

	// glfwWindowHint设置一些参数
	// 这里将主版本号(Major)和次版本号(Minor)都设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // 兼容性配置
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// 创建800 * 600 的标题为OpenGL的窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 交换缓冲区同步帧率，对齐显示帧率，一帧一交换
	glfwSwapInterval(1);
	// 设置渲染范围同步为窗口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK) 
		std::cout << "Error" << std::endl;

	{
		/**
		* 混合:
		* 将输出颜色(判断着色器输出的颜色)和目标缓冲区已有的颜色结合
		* glEnable/glDisable(启用&关闭) => glBlendFunc(指定颜色因子) => glBlendEquation(指定混合模式)
		* glBlendEquation(mode) mode: src和dest的混合方式(默认GL_FUNC_ADD, 叠加)
		*
		**/
		/* 启用混合(默认不会启用) */
		GLCall(glEnable(GL_BLEND));
		/**
		 * glBlendFunc(src, dest) 指定颜色因子
		 * src 指定输出颜色(RGBA)因子的计算方式, 默认为GL_ONE
		 * dest 指定目标颜色因子的计算方式, 默认为GL_ZERO
		 * GL_SRC_ALPHA 因为src的alpha为0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
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
	// 按下退出就设置窗口关闭标识
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
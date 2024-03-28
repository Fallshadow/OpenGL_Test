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

	if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;

	{

		float positions[] =
		{
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			100.0f, 200.0f, 0.0f, 1.0f,
			200.0f, 200.0f, 1.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			1, 2, 3
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.LayoutVertexBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

		shader.UnBind();
		va.UnBind();
		vb.UnBind();
		ib.UnBind();

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		glm::vec3 translation(200, 200, 0);

		// 渲染循环
		while (!glfwWindowShouldClose(window))
		{
			// 处理输入
			processInput(window);

			// 在每个新的渲染迭代开始的时候我们总是希望清屏，否则我们仍能看见上一次迭代的渲染结果（这可能是你想要的效果，但通常这不是）。
			// 我们还可以指定清除底色glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

			

			
			va.Bind();
			ib.Bind();
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", proj * view * model);

			renderer.Draw(va, ib, shader);

			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 800.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}


			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			// 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
			glfwSwapBuffers(window);

			// 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
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
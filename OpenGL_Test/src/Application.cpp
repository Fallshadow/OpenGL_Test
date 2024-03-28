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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
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

	if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;

	{

		float positions[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
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

		shader.UnBind();
		va.UnBind();
		vb.UnBind();
		ib.UnBind();

		Renderer renderer;

		// ��Ⱦѭ��
		while (!glfwWindowShouldClose(window))
		{
			// ��������
			processInput(window);

			// ��ÿ���µ���Ⱦ������ʼ��ʱ����������ϣ�������������������ܿ�����һ�ε�������Ⱦ����������������Ҫ��Ч������ͨ���ⲻ�ǣ���
			// ���ǻ�����ָ�������ɫglClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			renderer.Clear();

			shader.Bind();
			va.Bind();
			ib.Bind();

			renderer.Draw(va, ib, shader);

			// ������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
			glfwSwapBuffers(window);

			// �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
			glfwPollEvents();
		}
	}
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
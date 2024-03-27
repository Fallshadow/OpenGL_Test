#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int GetUniformLocation(const std::string& name);
};
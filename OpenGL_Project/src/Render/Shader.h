#pragma once
#include <string>
#include <unordered_map>
#include "../vendor/glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	int GetUniformLocation(const std::string& name);
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(std::string filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	ShaderProgramSource Parseshader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& FragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	void SetUniform1i(std::string name, int value);
	void SetUniform4f(std::string name, float v0, float v1, float v2, float fv3);
	void SetUniformMat4f(std::string name, glm::mat4& matrix);
};
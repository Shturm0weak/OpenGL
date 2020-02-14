#include "../pch.h"
#include "Shader.h"


Shader::Shader(std::string filepath) : m_FilePath(filepath), m_RendererID(0) {
	ShaderProgramSource source = Parseshader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	//std::cout << source.FragmentSource;
}

Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
	glUseProgram(m_RendererID);
}

void Shader::UnBind() const {
	glUseProgram(0);
}

void Shader::SetUniform1i(std::string name, int value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(std::string name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name),v0,v1,v2,v3);
}


void Shader::SetUniform4fv(std::string name, glm::vec4& vec4) {
	float color[4] = { vec4[0],vec4[1],vec4[2],vec4[3] };
	glUniform4fv(GetUniformLocation(name), 1,color);
}

void Shader::SetUniformMat4f(std::string name, glm::mat4& matrix) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::UploadUnifromMat4(const std::string& name,const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::Parseshader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class shadertype {

		NONE = -1, VERTEX = 0, FRAGMENT = 1

	};

	shadertype shadertp = shadertype::NONE;
	std::string line;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				shadertp = shadertype::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				shadertp = shadertype::FRAGMENT;
			}
		}
		else {
			ss[(int)shadertp] << line << '\n';
		}
	}

	return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile shader!\n");
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& FragmentShader) {
	unsigned int programm = glCreateProgram();
	unsigned vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	glAttachShader(programm, vs);
	glAttachShader(programm, fs);
	glLinkProgram(programm);
	glValidateProgram(programm);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return programm;
}
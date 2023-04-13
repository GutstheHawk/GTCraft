#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	: m_VertexFilePath(vertexFilepath), m_FragementFilePath(fragmentFilepath), m_RendererID(0)
{
	std::string VertexShader = ParseShader(vertexFilepath);
	std::string FragmentShader = ParseShader(fragmentFilepath);
	m_RendererID = CreateShader(VertexShader, FragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

std::string Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		printf("Warning: uniform location doesn't exist!\n");
	}
	m_UniformLocationCache[name] = location;

	return location;
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, i0);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	int location = GetUniformLocation(name);
	glUniform2f(location, v0, v1);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, int count, bool transpose, const float* value)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, count, transpose, value);
}

#pragma once
#include <string>
#include <unordered_map>

class Shader
{
private:
	std::string m_VertexFilePath;
	std::string m_FragementFilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int i0);

	void SetUniform2f(const std::string& name, float v0, float v1);

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4fv(const std::string& name, int count, bool transpose, const float* value);

private:
	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string&, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};
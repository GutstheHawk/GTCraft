#pragma once

#include <vector>
#include <GL/glew.h>
#include <stdexcept>

struct VertexBufferElement
{
	unsigned int type;
	int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(int count, bool normalized)
	{
		std::runtime_error(false);
	}

	template<>
	void Push<float>(int count, bool normalized)
	{
		m_Elements.push_back( {GL_FLOAT, count, normalized});
		m_Stride += count * sizeof(GLfloat);
	}

	template<>
	void Push<unsigned int>(int count, bool normalized)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, normalized });
		m_Stride += count * sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(int count, bool normalized)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, normalized });
		m_Stride += count * sizeof(GLbyte);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
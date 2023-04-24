#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(int renderID);
	Texture(const std::string& path);
	Texture(const std::string& path, int offset_x, int offset_y, int width, int height);
	~Texture();

	void Bind(unsigned int, unsigned int slot = 0) const;
	void Unbind(unsigned int) const;

	void loadCubemap(std::vector<std::string> faces);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};
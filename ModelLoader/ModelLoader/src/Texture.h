#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_BindID;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_width, m_hight, m_bitsPerPixel;
public:
	Texture(const std::string& path);
	~Texture();

	void SetPath(const std::string& path);
	void Bind(unsigned int slot = 0);
	void Unbind();

	int GetWidth() const { return m_width; }
	int GetHight() const { return m_hight; }

private:
	void SetTexture(const std::string& path);
};
#include <texture/texture.h>
#pragma once


class CTextureCompressor
{	
public:
	CTextureCompressor(const std::shared_ptr<CNSTexture>& texture);

public:
	bool compressToFile(const char* path, size_t& size);

public:
	static bool compress(const std::shared_ptr<CNSTexture>& texture, const char* path, size_t& size);

private:
	std::shared_ptr<CNSTexture> m_texture;
};
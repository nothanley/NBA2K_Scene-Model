#include <texture/texture_compress.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <fstream>

using namespace DirectX;

CTextureCompressor::CTextureCompressor(const std::shared_ptr<CNSTexture>& texture)
	: m_texture(texture)
{
}

bool CTextureCompressor::compress(const std::shared_ptr<CNSTexture>& texture, const char* path, size_t& size)
{
	CTextureCompressor compressor(texture);
	compressor.compressToFile(path, size);
	return true;
}

inline static void formatBitmap(std::vector<uint8_t>& bitmap, const std::shared_ptr<CNSTexture>& tex)
{
	auto pixmap = tex->pixmap();
	bitmap.resize(pixmap.size());

	size_t rowSize = tex->width() * 4;

	for (size_t y = 0; y < tex->height(); ++y)
	{
		// Get the index for the current row in the original pixmap
		size_t srcRowIndex = (tex->height() - 1 - y) * rowSize;
		size_t dstRowIndex = y * rowSize;

		// Copy pixel data for this row, flipping it vertically
		for (size_t x = 0; x < tex->width(); ++x)
		{
			bitmap[dstRowIndex + x * 4 + 0] = static_cast<uint8_t>(std::clamp(pixmap[srcRowIndex + x * 4 + 2] * 255.0f, 0.0f, 255.0f)); // R
			bitmap[dstRowIndex + x * 4 + 1] = static_cast<uint8_t>(std::clamp(pixmap[srcRowIndex + x * 4 + 1] * 255.0f, 0.0f, 255.0f)); // G
			bitmap[dstRowIndex + x * 4 + 2] = static_cast<uint8_t>(std::clamp(pixmap[srcRowIndex + x * 4 + 0] * 255.0f, 0.0f, 255.0f)); // B
			bitmap[dstRowIndex + x * 4 + 3] = static_cast<uint8_t>(std::clamp(pixmap[srcRowIndex + x * 4 + 3] * 255.0f, 0.0f, 255.0f)); // A
		}
	}
}

inline static bool initImageTex(ScratchImage& image, const std::shared_ptr<CNSTexture>& tex)
{
	HRESULT hr = image.Initialize2D(
		DXGI_FORMAT_B8G8R8A8_UNORM, tex->width(), tex->height(), /* Array Size */ 1, /* Total Mips */ tex->mips());
	
	if (FAILED(hr))
		return false;

	// Ensure the pixel data is in the correct format (unsigned byte)
	std::vector<uint8_t> pixelBits;
	::formatBitmap(pixelBits, tex);

	// Copy the pixel data into the ScratchImage
	memcpy(image.GetPixels(), pixelBits.data(), pixelBits.size());

	//printf("Texture initialized with width: %d, height: %d, mips: %d\n", tex->width(), tex->height(), tex->mips());
	return true;
}

inline static bool compressTexture(ScratchImage& image, ScratchImage& compressedImage, const DXGI_FORMAT& fmt)
{
	HRESULT hr = Compress(*image.GetImage(0, 0, 0), fmt, TEX_COMPRESS_DEFAULT, TEX_THRESHOLD_DEFAULT, compressedImage);

	//printf("Texture compressed to format: %d\n", fmt);
	return !FAILED(hr);
}

inline std::wstring wide_char_str(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

bool CTextureCompressor::compressToFile(const char* path, size_t& size)
{
	// check if texture is empty
	if (std::string(path).empty() || m_texture->empty())
		return false;

	// Create a DirectX texture from the pixel data
	ScratchImage image, compressedImage;
	if (!initImageTex(image, m_texture) || 
		!compressTexture(image, compressedImage, DXGI_FORMAT_BC3_UNORM))
		return false;

	// Save the compressed texture to disk
	auto fmtPath = ::wide_char_str(path);
	auto status = SaveToDDSFile(*compressedImage.GetImage(0, 0, 0), DDS_FLAGS_NONE, fmtPath.c_str());
	if (FAILED(status))
		return false;
	
	return true;
}



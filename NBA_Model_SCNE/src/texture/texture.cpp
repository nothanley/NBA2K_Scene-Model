#include <texture/texture.h>
#include <common.h>

CNSTexture::CNSTexture()
	: m_name("")
	, m_width(0)
	, m_height(0)
	, m_mips(1)
	, m_channels(4)
{
}

CNSTexture::CNSTexture(const char* name, const int width, const int height)
	: m_name(name)
	, m_width(width)
	, m_height(height)
	, m_mips(1)
	, m_channels(4)
{
}

void CNSTexture::setSize(const int width, const int height, const int channels)
{
	m_width = width;
	m_height = height;
	m_channels = channels;
}

void CNSTexture::setPath(const char* path)
{
	m_path = path;
}

void CNSTexture::setName(const char* name)
{
	m_name = name;
}

void CNSTexture::setPixmap(const std::vector<float>& pixmap)
{
	m_pixMap = pixmap;
}

void CNSTexture::setPixmap(const float* pixmap, const size_t& size)
{
	m_pixMap.resize(size);
	memcpy(m_pixMap.data(), pixmap, size * sizeof(float));
}

void CNSTexture::setMipsTarget(const int mips)
{
	m_mips = mips;
}

void CNSTexture::setType(const char* type)
{
	m_type = type;
}

const std::vector<float>& CNSTexture::pixmap() const
{
	return m_pixMap;
}

const std::string CNSTexture::name() const
{
	return m_name;
}

const std::string CNSTexture::path() const
{
	return m_path;
}

const int CNSTexture::width() const
{
	return m_width;
}

const int CNSTexture::height() const
{
	return m_height;
}

const int CNSTexture::channels() const
{
	return m_channels;
}

const std::string CNSTexture::type() const
{
	return m_type;
}

const int CNSTexture::mips() const
{
	return m_mips;
}

bool CNSTexture::empty() const
{
	return m_pixMap.empty();
}

std::string CNSTexture::generateFileId() const
{
	std::string texture_id    = name() + type();
	std::string texture_hash  = common::get_u64_hash_str(texture_id);
	std::string base_name     = name().empty() ? "unlinked" : name();
	
	return base_name + "." + texture_hash + ".dds";
}




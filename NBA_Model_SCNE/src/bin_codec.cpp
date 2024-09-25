#include <bin_codec.h>
#include <common.h>

BinaryCodec::BinaryCodec(const char* encode_fmt, const char* data_type)
    :
    m_encodeFmt(encode_fmt),
    m_type(common::to_lower(data_type))
{
    this->registerTypes();
}

BinaryCodec::BinaryCodec(const std::string& encode_fmt, const std::string& data_type)
    :
    m_encodeFmt(encode_fmt),
    m_type(common::to_lower(data_type))
{
    this->registerTypes();
}

void 
BinaryCodec::registerTypes()
{
    m_map["R10G10B10A2"]  = std::make_unique<R10G10B10A2>();
    m_map["R32G32B32A32"] = std::make_unique<R32G32B32A32>();
    m_map["R32G32B32"]    = std::make_unique<R32G32B32>();
    m_map["R32G32"]       = std::make_unique<R32G32>();
    m_map["R32"]          = std::make_unique<R32>();
    m_map["R16G16B16A16"] = std::make_unique<R16G16B16A16>();
    m_map["R16G16B16"]    = std::make_unique<R16G16B16>();
    m_map["R16G16"]       = std::make_unique<R16G16>();
    m_map["R16"]          = std::make_unique<R16>();
    m_map["R8G8B8A8"]     = std::make_unique<R8G8B8A8>();
    m_map["R8G8B8"]       = std::make_unique<R8G8B8>();
    m_map["R8G8"]         = std::make_unique<R8G8>();
    m_map["R8"]           = std::make_unique<R8>();
}

void
BinaryCodec::decode(
    char*& src,
    int size,
    std::vector<float>& target,
    const uint64_t offset,
    const uint8_t stride)
{
    auto it = m_map.find(m_encodeFmt);
    if (it == m_map.end())
        throw std::invalid_argument("Unknown format key: " + m_encodeFmt);

    return it->second->decode(src, size, target, m_type.c_str(), offset, stride);
}

void
BinaryCodec::update(
    char*& src,
    int size,
    const std::vector<float>& target,
    const uint64_t offset,
    const uint8_t stride)
{
    auto it = m_map.find(m_encodeFmt);
    if (it == m_map.end())
        throw std::invalid_argument("Unknown format key: " + m_encodeFmt);

    return it->second->updateData(src, size, target, m_type.c_str(), offset, stride);
}

int
BinaryCodec::size(const int items)
{
    auto it = m_map.find(m_encodeFmt);
    if (it == m_map.end()) {
        throw std::invalid_argument("Unknown format key: " + m_encodeFmt);
    }

    return it->second->get_size(items);
}

int
BinaryCodec::num_channels()
{
    auto it = m_map.find(m_encodeFmt);
    if (it == m_map.end()) {
        throw std::invalid_argument("Unknown format key: " + m_encodeFmt);
    }

    return it->second->get_channels();
}

char*
BinaryCodec::encode(
	const std::vector<float>& target,
    size_t& dataSize
)
{
	auto it = m_map.find(m_encodeFmt);
	if (it == m_map.end()) {
		throw std::invalid_argument("Unknown format key: " + m_encodeFmt);
	}

	return it->second->encode(target, m_type.c_str(), dataSize);
}



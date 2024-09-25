#include <bin_format.h>
#pragma once

class BinaryCodec
{
public:
    BinaryCodec(const char* encode_fmt, const char* data_type);
    BinaryCodec(const std::string& encode_fmt, const std::string& data_type);

public:
    int size(const int items);

    int num_channels();

    void decode(
        char*& src,
        int size,
        std::vector<float>& target,
        const uint64_t offset,
        const uint8_t stride);

    void update(
        char*& src,
        int size,
        const std::vector<float>& target,
        const uint64_t offset,
        const uint8_t stride);

    char* encode(
        const std::vector<float>& target,
        size_t& dataSize
    );

private:
    void registerTypes();

private:
    std::string m_encodeFmt;
    std::string m_type;
    std::unordered_map<std::string, std::unique_ptr<Format>> m_map;
};


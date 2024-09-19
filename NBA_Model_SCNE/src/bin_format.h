#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <stdexcept>
#pragma once

#define FMT_DT_PARAMS char*& src, int size, std::vector<float>& target, const std::string type, const uint64_t offset, const uint8_t stride
#define INJ_DT_PARAMS char*& src, int size, const std::vector<float>& target, const std::string type, const uint64_t offset, const uint8_t stride

// Base class
class Format {
public:
    virtual ~Format() = default;
    virtual void updateData(INJ_DT_PARAMS) = 0;
    virtual void decode(FMT_DT_PARAMS) = 0;
    virtual int get_size(const int items) = 0;
    virtual int get_channels() = 0;

protected:
    int m_bits;
};

// Variable Bit Formats
template <int Channels>
class Format_32Bit : public Format{
public:
    Format_32Bit() { m_bits = 32; }
    void updateData(INJ_DT_PARAMS) override;
    void decode(FMT_DT_PARAMS) override;
    int get_size(const int items) override;
    int get_channels() override;
};

template <int Channels1>
class Format_16Bit : public Format{
public:
    Format_16Bit() { m_bits = 16; }
    void updateData(INJ_DT_PARAMS) override;
    void decode(FMT_DT_PARAMS) override;
    int get_size(const int items) override;
    int get_channels() override;
};

template <int Channels>
class Format_8Bit : public Format{
public:
    Format_8Bit() { m_bits = 8; }
    void updateData(INJ_DT_PARAMS) override;
    void decode(FMT_DT_PARAMS) override;
    int get_size(const int items) override;
    int get_channels() override;
};

// Additional Formats:
class R10G10B10A2 : public Format {
public:
    R10G10B10A2() { m_bits = 32; }
    void updateData(INJ_DT_PARAMS) override;
    void decode(FMT_DT_PARAMS) override;
    int get_size(const int items) override;
    int get_channels() override;
};

// Explicit instantiations for known types
template class Format_32Bit<4>;
template class Format_32Bit<3>;
template class Format_32Bit<2>;
template class Format_32Bit<1>;
template class Format_16Bit<4>;
template class Format_16Bit<3>;
template class Format_16Bit<2>;
template class Format_16Bit<1>;
template class Format_8Bit<4>;
template class Format_8Bit<3>;
template class Format_8Bit<2>;
template class Format_8Bit<1>;

// Define Known Type Alias
using R32G32B32A32 = Format_32Bit<4>;
using R32G32B32    = Format_32Bit<3>;
using R32G32       = Format_32Bit<2>;
using R32          = Format_32Bit<1>;
using R16G16B16A16 = Format_16Bit<4>;
using R16G16B16    = Format_16Bit<3>;
using R16G16       = Format_16Bit<2>;
using R16          = Format_16Bit<1>;
using R8G8B8A8     = Format_8Bit<4>;
using R8G8B8       = Format_8Bit<3>;
using R8G8         = Format_8Bit<2>;
using R8           = Format_8Bit<1>;

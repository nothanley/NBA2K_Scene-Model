#include <memory>
#include <json.hpp>
#pragma once

using JSON = nlohmann::ordered_json;

class CSerializer 
{
public:
    virtual std::shared_ptr<JSON> serialize() = 0;
    virtual ~CSerializer() = default;

public:
    virtual std::shared_ptr<JSON> json() { return m_json; };

protected:
    std::shared_ptr<JSON> m_json = std::make_shared<JSON>();
};



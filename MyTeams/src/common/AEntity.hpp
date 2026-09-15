#pragma once

#include "ISerializable.hpp"
#include <string>

class AEntity : public ISerializable {
public:
    const std::string &uuid() const { return _uuid; }
    const std::string &name() const { return _name; }

    virtual std::string type() const = 0;

protected:
    AEntity() = default;
    AEntity(std::string uuid, std::string name)
        : _uuid(std::move(uuid)), _name(std::move(name)) {}

    std::string _uuid;
    std::string _name;
};

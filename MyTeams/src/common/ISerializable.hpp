#pragma once

#include <iostream>

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void serialize(std::ostream &out) const = 0;
    virtual void deserialize(std::istream &in) = 0;
};

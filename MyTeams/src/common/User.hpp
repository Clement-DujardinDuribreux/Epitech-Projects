#pragma once

#include "AEntity.hpp"
#include "Uuid.hpp"

class User : public AEntity {
public:
    User() : _connected(false) {}
    explicit User(const std::string &name)
        : AEntity(Uuid::generate(), name), _connected(false) {}

    User(std::string uuid, std::string name, bool connected)
        : AEntity(std::move(uuid), std::move(name)), _connected(connected) {}

    bool connected() const { return _connected; }
    void setConnected(bool v) { _connected = v; }

    std::string type() const override { return "user"; }

    void serialize(std::ostream &out) const override
    {
        out << _uuid << '\n' << _name << '\n' << _connected << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _uuid);
        std::getline(in, _name);
        in >> _connected;
        in.ignore();
    }

private:
    bool _connected;
};

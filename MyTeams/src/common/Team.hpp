#pragma once

#include "AEntity.hpp"
#include "Uuid.hpp"

class Team : public AEntity {
public:
    Team() = default;
    Team(const std::string &name, const std::string &description)
        : AEntity(Uuid::generate(), name), _description(description) {}

    Team(std::string uuid, std::string name, std::string description)
        : AEntity(std::move(uuid), std::move(name)), _description(std::move(description)) {}

    const std::string &description() const { return _description; }

    std::string type() const override { return "team"; }

    void serialize(std::ostream &out) const override
    {
        out << _uuid << '\n' << _name << '\n' << _description << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _uuid);
        std::getline(in, _name);
        std::getline(in, _description);
    }

private:
    std::string _description;
};

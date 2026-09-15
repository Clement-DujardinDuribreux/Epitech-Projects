#pragma once

#include "AEntity.hpp"
#include "Uuid.hpp"

class Channel : public AEntity {
public:
    Channel() = default;
    Channel(const std::string &name, const std::string &description, const std::string &teamUuid)
        : AEntity(Uuid::generate(), name), _description(description), _teamUuid(teamUuid) {}

    Channel(std::string uuid, std::string name, std::string description, std::string teamUuid)
        : AEntity(std::move(uuid), std::move(name)),
          _description(std::move(description)),
          _teamUuid(std::move(teamUuid)) {}

    const std::string &description() const { return _description; }
    const std::string &teamUuid()    const { return _teamUuid; }

    std::string type() const override { return "channel"; }

    void serialize(std::ostream &out) const override
    {
        out << _uuid << '\n' << _name << '\n' << _description << '\n' << _teamUuid << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _uuid);
        std::getline(in, _name);
        std::getline(in, _description);
        std::getline(in, _teamUuid);
    }

private:
    std::string _description;
    std::string _teamUuid;
};

#pragma once

#include "AEntity.hpp"
#include "Uuid.hpp"
#include <ctime>

class Thread : public AEntity {
public:
    Thread() : _timestamp(0) {}
    Thread(const std::string &title, const std::string &body,
           const std::string &creatorUuid, const std::string &channelUuid)
        : AEntity(Uuid::generate(), title),
          _body(body),
          _creatorUuid(creatorUuid),
          _channelUuid(channelUuid),
          _timestamp(std::time(nullptr)) {}

    Thread(std::string uuid, std::string title, std::string body,
           std::string creatorUuid, std::string channelUuid, time_t timestamp)
        : AEntity(std::move(uuid), std::move(title)),
          _body(std::move(body)),
          _creatorUuid(std::move(creatorUuid)),
          _channelUuid(std::move(channelUuid)),
          _timestamp(timestamp) {}

    const std::string &body()        const { return _body; }
    const std::string &creatorUuid() const { return _creatorUuid; }
    const std::string &channelUuid() const { return _channelUuid; }
    time_t             timestamp()   const { return _timestamp; }

    std::string type() const override { return "thread"; }

    void serialize(std::ostream &out) const override
    {
        out << _uuid << '\n' << _name << '\n' << _body << '\n'
            << _creatorUuid << '\n' << _channelUuid << '\n' << _timestamp << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _uuid);
        std::getline(in, _name);
        std::getline(in, _body);
        std::getline(in, _creatorUuid);
        std::getline(in, _channelUuid);
        in >> _timestamp;
        in.ignore();
    }

private:
    std::string _body;
    std::string _creatorUuid;
    std::string _channelUuid;
    time_t      _timestamp;
};

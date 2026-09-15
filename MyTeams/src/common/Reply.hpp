#pragma once

#include "ISerializable.hpp"
#include "Uuid.hpp"
#include <ctime>
#include <string>

class Reply : public ISerializable {
public:
    Reply() : _timestamp(0) {}
    Reply(const std::string &body, const std::string &creatorUuid, const std::string &threadUuid)
        : _uuid(Uuid::generate()),
          _body(body),
          _creatorUuid(creatorUuid),
          _threadUuid(threadUuid),
          _timestamp(std::time(nullptr)) {}

    Reply(std::string uuid, std::string body, std::string creatorUuid,
          std::string threadUuid, time_t timestamp)
        : _uuid(std::move(uuid)),
          _body(std::move(body)),
          _creatorUuid(std::move(creatorUuid)),
          _threadUuid(std::move(threadUuid)),
          _timestamp(timestamp) {}

    const std::string &uuid()        const { return _uuid; }
    const std::string &body()        const { return _body; }
    const std::string &creatorUuid() const { return _creatorUuid; }
    const std::string &threadUuid()  const { return _threadUuid; }
    time_t             timestamp()   const { return _timestamp; }

    void serialize(std::ostream &out) const override
    {
        out << _uuid << '\n' << _body << '\n'
            << _creatorUuid << '\n' << _threadUuid << '\n' << _timestamp << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _uuid);
        std::getline(in, _body);
        std::getline(in, _creatorUuid);
        std::getline(in, _threadUuid);
        in >> _timestamp;
        in.ignore();
    }

private:
    std::string _uuid;
    std::string _body;
    std::string _creatorUuid;
    std::string _threadUuid;
    time_t      _timestamp;
};

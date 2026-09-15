#pragma once

#include "ISerializable.hpp"
#include <ctime>
#include <string>

class PrivateMessage : public ISerializable {
public:
    PrivateMessage() : _timestamp(0) {}
    PrivateMessage(const std::string &senderUuid, const std::string &receiverUuid,
                   const std::string &body)
        : _senderUuid(senderUuid),
          _receiverUuid(receiverUuid),
          _body(body),
          _timestamp(std::time(nullptr)) {}

    PrivateMessage(std::string senderUuid, std::string receiverUuid,
                   std::string body, time_t timestamp)
        : _senderUuid(std::move(senderUuid)),
          _receiverUuid(std::move(receiverUuid)),
          _body(std::move(body)),
          _timestamp(timestamp) {}

    const std::string &senderUuid()   const { return _senderUuid; }
    const std::string &receiverUuid() const { return _receiverUuid; }
    const std::string &body()         const { return _body; }
    time_t             timestamp()    const { return _timestamp; }

    void serialize(std::ostream &out) const override
    {
        out << _senderUuid << '\n' << _receiverUuid << '\n'
            << _body << '\n' << _timestamp << '\n';
    }

    void deserialize(std::istream &in) override
    {
        std::getline(in, _senderUuid);
        std::getline(in, _receiverUuid);
        std::getline(in, _body);
        in >> _timestamp;
        in.ignore();
    }

private:
    std::string _senderUuid;
    std::string _receiverUuid;
    std::string _body;
    time_t      _timestamp;
};

#include "DataStore.hpp"
#include "MyTeamsException.hpp"

User &DataStore::addUser(const std::string &name)
{
    if (findUserByName(name) != nullptr)
        throw AlreadyExistsException("user '" + name + "' already exists");
    User user(name);
    auto [it, _] = _users.emplace(user.uuid(), std::move(user));
    return it->second;
}

Team &DataStore::addTeam(const std::string &name, const std::string &description,
                         const std::string &)
{
    for (auto &[_, t] : _teams)
        if (t.name() == name)
            throw AlreadyExistsException("team '" + name + "' already exists");
    Team team(name, description);
    auto [it, _2] = _teams.emplace(team.uuid(), std::move(team));
    return it->second;
}

Channel &DataStore::addChannel(const std::string &name, const std::string &description,
                               const std::string &teamUuid)
{
    findTeam(teamUuid);
    for (auto &[_, c] : _channels)
        if (c.teamUuid() == teamUuid && c.name() == name)
            throw AlreadyExistsException("channel '" + name + "' already exists in this team");
    Channel channel(name, description, teamUuid);
    auto [it, _2] = _channels.emplace(channel.uuid(), std::move(channel));
    return it->second;
}

Thread &DataStore::addThread(const std::string &title, const std::string &body,
                             const std::string &creatorUuid, const std::string &channelUuid)
{
    findChannel(channelUuid);
    for (auto &[_, t] : _threads)
        if (t.channelUuid() == channelUuid && t.name() == title)
            throw AlreadyExistsException("thread '" + title + "' already exists in this channel");
    Thread thread(title, body, creatorUuid, channelUuid);
    auto [it, _2] = _threads.emplace(thread.uuid(), std::move(thread));
    return it->second;
}

Reply &DataStore::addReply(const std::string &body, const std::string &creatorUuid,
                           const std::string &threadUuid)
{
    findThread(threadUuid);
    _replies.emplace_back(body, creatorUuid, threadUuid);
    return _replies.back();
}

PrivateMessage &DataStore::addPrivateMessage(const std::string &senderUuid,
                                             const std::string &receiverUuid,
                                             const std::string &body)
{
    _messages.emplace_back(senderUuid, receiverUuid, body);
    return _messages.back();
}

void DataStore::subscribe(const std::string &userUuid, const std::string &teamUuid)
{
    findUser(userUuid);
    findTeam(teamUuid);
    _subscriptions.emplace(userUuid, teamUuid);
}

void DataStore::unsubscribe(const std::string &userUuid, const std::string &teamUuid)
{
    _subscriptions.erase({userUuid, teamUuid});
}

bool DataStore::isSubscribed(const std::string &userUuid, const std::string &teamUuid) const
{
    return _subscriptions.count({userUuid, teamUuid}) > 0;
}

User &DataStore::findUser(const std::string &uuid)
{
    auto it = _users.find(uuid);
    if (it == _users.end())
        throw NotFoundException("user '" + uuid + "' not found");
    return it->second;
}

Team &DataStore::findTeam(const std::string &uuid)
{
    auto it = _teams.find(uuid);
    if (it == _teams.end())
        throw NotFoundException("team '" + uuid + "' not found");
    return it->second;
}

Channel &DataStore::findChannel(const std::string &uuid)
{
    auto it = _channels.find(uuid);
    if (it == _channels.end())
        throw NotFoundException("channel '" + uuid + "' not found");
    return it->second;
}

Thread &DataStore::findThread(const std::string &uuid)
{
    auto it = _threads.find(uuid);
    if (it == _threads.end())
        throw NotFoundException("thread '" + uuid + "' not found");
    return it->second;
}

User *DataStore::findUserByName(const std::string &name)
{
    for (auto &[_, u] : _users)
        if (u.name() == name)
            return &u;
    return nullptr;
}

Team &DataStore::findTeamByName(const std::string &name)
{
    for (auto &[_, t] : _teams)
        if (t.name() == name)
            return t;
    throw NotFoundException("team '" + name + "' not found");
}

Channel &DataStore::findChannelByName(const std::string &name)
{
    for (auto &[_, c] : _channels)
        if (c.name() == name)
            return c;
    throw NotFoundException("channel '" + name + "' not found");
}

Thread &DataStore::findThreadByName(const std::string &name)
{
    for (auto &[_, t] : _threads)
        if (t.name() == name)
            return t;
    throw NotFoundException("thread '" + name + "' not found");
}

std::vector<Reply> DataStore::repliesForThread(const std::string &threadUuid) const
{
    std::vector<Reply> result;
    for (const auto &r : _replies)
        if (r.threadUuid() == threadUuid)
            result.push_back(r);
    return result;
}

std::vector<Thread> DataStore::threadsOfChannel(const std::string &channelUuid) const
{
    std::vector<Thread> result;
    for (const auto &t : _threads)
        if (t.second.channelUuid() == channelUuid)
            result.push_back(t.second);
    return result;
}

std::vector<Channel> DataStore::channelsOfTeam(const std::string &teamUuid) const
{
    std::vector<Channel> result;
    for (const auto &c : _channels)
        if (c.second.teamUuid() == teamUuid)
            result.push_back(c.second);
    return result;
}

std::vector<PrivateMessage> DataStore::messagesBetween(const std::string &a,
                                                        const std::string &b) const
{
    std::vector<PrivateMessage> result;
    for (const auto &m : _messages)
        if ((m.senderUuid() == a && m.receiverUuid() == b) ||
            (m.senderUuid() == b && m.receiverUuid() == a))
            result.push_back(m);
    return result;
}

std::vector<std::string> DataStore::subscribersOf(const std::string &teamUuid) const
{
    std::vector<std::string> result;
    for (const auto &[user, team] : _subscriptions)
        if (team == teamUuid)
            result.push_back(user);
    return result;
}

std::vector<std::string> DataStore::subscribedTeamsOf(const std::string &userUuid) const
{
    std::vector<std::string> result;
    for (const auto &[user, team] : _subscriptions)
        if (user == userUuid)
            result.push_back(team);
    return result;
}

std::ofstream DataStore::openForWrite(const std::string &path)
{
    std::ofstream f(path);
    if (!f)
        throw SerializationException("cannot open '" + path + "' for writing");
    return f;
}

std::ifstream DataStore::openForRead(const std::string &path)
{
    return std::ifstream(path);
}

bool DataStore::isGoodPath(const std::string &teamUuid, const std::string &channelUuid, const std::string &threadUuid) const
{
    bool good = false;
    if (!threadUuid.empty()) {
        for (auto thread : threadsOfChannel(channelUuid))
            if (thread.uuid() == threadUuid) good = true;
        if (!good) return false;
    }
    good = false;
    if (!channelUuid.empty()) {
        for (auto channel : channelsOfTeam(teamUuid))
            if (channel.uuid() == channelUuid) good = true;
        if (!good) return false;
    }
    return true;
}

void DataStore::save(const std::string &dir) const
{
    {
        auto f = openForWrite(dir + "/users.dat");
        f << _users.size() << '\n';
        for (const auto &[_, v] : _users) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/teams.dat");
        f << _teams.size() << '\n';
        for (const auto &[_, v] : _teams) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/channels.dat");
        f << _channels.size() << '\n';
        for (const auto &[_, v] : _channels) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/threads.dat");
        f << _threads.size() << '\n';
        for (const auto &[_, v] : _threads) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/replies.dat");
        f << _replies.size() << '\n';
        for (const auto &v : _replies) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/messages.dat");
        f << _messages.size() << '\n';
        for (const auto &v : _messages) v.serialize(f);
    }
    {
        auto f = openForWrite(dir + "/subscriptions.dat");
        f << _subscriptions.size() << '\n';
        for (const auto &[user, team] : _subscriptions)
            f << user << '\n' << team << '\n';
    }
}

void DataStore::load(const std::string &dir,
                     const std::function<void(const User &)> &onUserLoaded)
{
    {
        auto f = openForRead(dir + "/users.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                User u; u.deserialize(f);
                _users.emplace(u.uuid(), std::move(u));
            }
        }
    }
    {
        auto f = openForRead(dir + "/teams.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                Team t; t.deserialize(f);
                _teams.emplace(t.uuid(), std::move(t));
            }
        }
    }
    {
        auto f = openForRead(dir + "/channels.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                Channel c; c.deserialize(f);
                _channels.emplace(c.uuid(), std::move(c));
            }
        }
    }
    {
        auto f = openForRead(dir + "/threads.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                Thread t; t.deserialize(f);
                _threads.emplace(t.uuid(), std::move(t));
            }
        }
    }
    {
        auto f = openForRead(dir + "/replies.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                Reply r; r.deserialize(f);
                _replies.push_back(std::move(r));
            }
        }
    }
    {
        auto f = openForRead(dir + "/messages.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                PrivateMessage m; m.deserialize(f);
                _messages.push_back(std::move(m));
            }
        }
    }
    {
        auto f = openForRead(dir + "/subscriptions.dat");
        if (f) {
            size_t count; f >> count; f.ignore();
            for (size_t i = 0; i < count; ++i) {
                std::string user, team;
                std::getline(f, user);
                std::getline(f, team);
                _subscriptions.emplace(user, team);
            }
        }
    }

    for (const auto &[_, u] : _users)
        if (!u.name().empty())
            onUserLoaded(u);
}

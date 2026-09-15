#pragma once

#include "User.hpp"
#include "Team.hpp"
#include "Channel.hpp"
#include "Thread.hpp"
#include "Reply.hpp"
#include "PrivateMessage.hpp"
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

class DataStore {
public:
    User    &addUser(const std::string &name);
    Team    &addTeam(const std::string &name, const std::string &description, const std::string &creatorUuid);
    Channel &addChannel(const std::string &name, const std::string &description, const std::string &teamUuid);
    Thread  &addThread(const std::string &title, const std::string &body, const std::string &creatorUuid, const std::string &channelUuid);
    Reply   &addReply(const std::string &body, const std::string &creatorUuid, const std::string &threadUuid);
    PrivateMessage &addPrivateMessage(const std::string &senderUuid, const std::string &receiverUuid, const std::string &body);

    void subscribe(const std::string &userUuid, const std::string &teamUuid);
    void unsubscribe(const std::string &userUuid, const std::string &teamUuid);
    bool isSubscribed(const std::string &userUuid, const std::string &teamUuid) const;

    User    &findUser(const std::string &uuid);
    Team    &findTeam(const std::string &uuid);
    Channel &findChannel(const std::string &uuid);
    Thread  &findThread(const std::string &uuid);

    Team    &findTeamByName(const std::string &name);
    Channel &findChannelByName(const std::string &name);
    Thread  &findThreadByName(const std::string &name);

    User *findUserByName(const std::string &name);

    const std::map<std::string, User>           &users()    const { return _users; }
    const std::map<std::string, Team>           &teams()    const { return _teams; }
    const std::map<std::string, Channel>        &channels() const { return _channels; }
    const std::map<std::string, Thread>         &threads()  const { return _threads; }
    const std::vector<Reply>                    &replies()  const { return _replies; }
    const std::vector<PrivateMessage>           &messages() const { return _messages; }
    const std::set<std::pair<std::string,std::string>> &subscriptions() const { return _subscriptions; }

    std::vector<Reply>          repliesForThread(const std::string &threadUuid) const;
    std::vector<Thread>         threadsOfChannel(const std::string &channelUuid) const;
    std::vector<Channel>        channelsOfTeam(const std::string &teamUuid) const;

    std::vector<PrivateMessage> messagesBetween(const std::string &a, const std::string &b) const;
    std::vector<std::string>    subscribersOf(const std::string &teamUuid) const;
    std::vector<std::string>    subscribedTeamsOf(const std::string &userUuid) const;

    bool isGoodPath(const std::string &teamUuid, const std::string &channelUuid, const std::string &threadUuid) const;

    void save(const std::string &dir) const;
    void load(const std::string &dir, const std::function<void(const User &)> &onUserLoaded);

private:
    std::map<std::string, User>    _users;
    std::map<std::string, Team>    _teams;
    std::map<std::string, Channel> _channels;
    std::map<std::string, Thread>  _threads;
    std::vector<Reply>             _replies;
    std::vector<PrivateMessage>    _messages;
    std::set<std::pair<std::string,std::string>> _subscriptions;

    template<typename T>
    void saveCollection(const std::string &path, const std::map<std::string, T> &col) const;
    template<typename T>
    void saveCollection(const std::string &path, const std::vector<T> &col) const;

    template<typename T>
    void loadIntoMap(const std::string &path, std::map<std::string, T> &col);
    template<typename T>
    void loadIntoVec(const std::string &path, std::vector<T> &col);
    void loadSubscriptions(const std::string &path);
    void saveSubscriptions(const std::string &path) const;

    static std::ofstream openForWrite(const std::string &path);
    static std::ifstream openForRead(const std::string &path);
};

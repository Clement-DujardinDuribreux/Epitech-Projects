#include <criterion/criterion.h>
#include <arpa/inet.h>
#include <chrono>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "ArgvParser.hpp"
#include "Cell.hpp"
#include "Map.hpp"
#include "client/Client.hpp"

int createListeningSocket(int &port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    socklen_t length = sizeof(addr);
    int opt = 1;

    cr_assert_geq(fd, 0);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;
    cr_assert_eq(bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)), 0);
    cr_assert_eq(getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &length), 0);
    cr_assert_eq(listen(fd, 1), 0);
    port = ntohs(addr.sin_port);
    return fd;
}

bool sendAll(int fd, const std::string &payload)
{
    const char *data = payload.c_str();
    std::size_t rest = payload.size();

    while (rest > 0) {
        ssize_t written = send(fd, data, rest, 0);
        if (written <= 0)
            return false;
        data += written;
        rest -= static_cast<std::size_t>(written);
    }
    return true;
}

void runProtocolServer(int listenFd)
{
    int clientFd = accept(listenFd, nullptr, nullptr);

    if (clientFd < 0) {
        close(listenFd);
        return;
    }
    sendAll(clientFd, "WELCOME\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sendAll(clientFd,
        "msz 5 4\n"
        "bct 2 3 1 2 3 4 5 6 7\n"
        "pnw #9 2 3 1 4 red\n"
        "pin #9 2 3 8 7 6 5 4 3 2\n"
        "pic 2 3 4 #9\n"
        "plv #9 5\n"
        "enw #4 #9 1 2\n"
        "seg red\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    close(clientFd);
    close(listenFd);
}

bool waitClientState(Client &client)
{
    for (int attempt = 0; attempt < 30; attempt++) {
        Map map = client.getMap();
        if (map.getWidth() == 5 && map.getHeight() == 4
            && map.getPlayers().size() == 1 && map.getEggs().size() == 1
            && client.getEnd())
            return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

Test(gui_args, parses_flags)
{
    char arg0[] = "zappy_gui";
    char arg1[] = "-p";
    char arg2[] = "4242";
    char arg3[] = "-h";
    char arg4[] = "localhost";
    char *argv[] = {arg0, arg1, arg2, arg3, arg4, nullptr};
    ArgvParser parser(argv);

    cr_assert_eq(parser.getArgFromFlag<int>("-p", [](const std::string &value) {
        return std::stoi(value);
    }), 4242);
    cr_assert_eq(parser.getArgFromFlag<std::string>("-h", [](const std::string &value) {
        return value;
    }), "localhost");
    cr_assert_eq(parser.getArgFromIndex(0), "zappy_gui");
}

Test(gui_args, reports_invalid_access)
{
    char arg0[] = "zappy_gui";
    char arg1[] = "-p";
    char *argv[] = {arg0, arg1, nullptr};
    ArgvParser parser(argv);

    cr_assert_throw(parser.getArgFromFlag<int>("-p", [](const std::string &value) {
        return std::stoi(value);
    }), ArgvParser::ArgvParserException);
    cr_assert_throw(parser.getArgFromIndex(4), ArgvParser::ArgvParserException);
}

Test(gui_cell, updates_resources)
{
    Cell cell;

    cr_assert_eq(cell.getResources(), std::vector<int>(7, 0));
    cell.setResources({1, 2, 3, 4, 5, 6, 7});
    cell.addResource(0, 3);
    cell.removeResource(6, 2);
    cr_assert_eq(cell.getResources()[0], 4);
    cr_assert_eq(cell.getResources()[6], 5);
}

Test(gui_map, aggregates_state)
{
    Map map(4, 4);

    map.getCell(1, 1).setResources({1, 2, 3, 4, 5, 6, 7});
    map.getCell(2, 2).setResources({7, 6, 5, 4, 3, 2, 1});
    map.getPlayers().emplace_back(1, 1, 1, 1, 2, "red");
    map.getPlayers().emplace_back(2, 1, 1, 2, 3, "red");
    map.getPlayers().emplace_back(3, 2, 2, 3, 8, "blue");
    map.getPlayers()[0].setInventory({1, 1, 1, 1, 1, 1, 1});
    map.getPlayers()[1].setInventory({2, 2, 2, 2, 2, 2, 2});
    map.getPlayers()[0].setLevel(4);
    map.startIncantation(1, 1, 2);

    cr_assert_eq(map.getResources(), (std::vector<int>{8, 8, 8, 8, 8, 8, 8}));
    cr_assert_eq(map.getTeamNames().size(), 2);
    cr_assert_eq(map.getTeamsResources("red"),
        (std::vector<int>{3, 3, 3, 3, 3, 3, 3}));

    std::vector<int> levels = map.getPlayersLevelPerTeam("red");

    cr_assert_eq(levels[2], 1);
    cr_assert_eq(levels[3], 1);
    cr_assert(map.getPlayers()[0].isLevelUpAnimationVisible());
    cr_assert(map.hasIncantationAt(1, 1));
    cr_assert_eq(map.getPlayersOn(1, 1).size(), 2);
}

Test(gui_map, converts_coordinates)
{
    Map map(6, 6);
    sf::Vector2f screen = Map::getTileScreenPosition(2.5f, 3.5f, map);
    std::pair<int, int> cell = map.getCellPos(
        static_cast<int>(screen.x), static_cast<int>(screen.y));

    cr_assert_eq(cell, (std::make_pair(2, 3)));
    cr_assert_eq(map.getCellPos(-100, -100), (std::make_pair(-1, -1)));
}

Test(gui_client, applies_protocol_flow, .timeout = 5)
{
    int port = 0;
    int listenFd = createListeningSocket(port);
    std::thread server(runProtocolServer, listenFd);
    Client client;

    cr_assert(client.connectToServer("127.0.0.1", port));
    cr_assert(waitClientState(client));
    client.stopUpdateThread();
    server.join();

    Map map = client.getMap();

    cr_assert_eq(map.getWidth(), 5);
    cr_assert_eq(map.getHeight(), 4);
    cr_assert_eq(map.getCell(2, 3).getResources(),
        (std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    cr_assert_eq(map.getPlayers().size(), 1);
    cr_assert_eq(map.getPlayers()[0].getId(), 9);
    cr_assert_eq(map.getPlayers()[0].getLevel(), 5);
    cr_assert(map.hasIncantationAt(2, 3));
    cr_assert_eq(map.getPlayers()[0].getInventory(),
        (std::vector<int>{8, 7, 6, 5, 4, 3, 2}));
    cr_assert_eq(map.getEggs().size(), 1);
    cr_assert_eq(map.getEggs()[0].getId(), 4);
    cr_assert(client.getEnd());
    cr_assert_eq(client.getWinnerTeam(), "red");
}

#include <criterion/criterion.h>
#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

int freePort()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    socklen_t length = sizeof(addr);

    cr_assert_geq(fd, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;
    cr_assert_eq(bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)), 0);
    cr_assert_eq(getsockname(fd, reinterpret_cast<sockaddr *>(&addr), &length), 0);

    int port = ntohs(addr.sin_port);

    close(fd);
    return port;
}

int connectToServer(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};

    cr_assert_geq(fd, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(port);
    cr_assert_eq(connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)), 0);
    return fd;
}

std::string readLine(int fd)
{
    std::string line;
    char character = 0;

    while (read(fd, &character, 1) == 1) {
        line += character;
        if (character == '\n')
            break;
    }
    return line;
}

bool writeAll(int fd, const std::string &payload)
{
    const char *data = payload.c_str();
    std::size_t rest = payload.size();

    while (rest > 0) {
        ssize_t written = write(fd, data, rest);
        if (written <= 0)
            return false;
        data += written;
        rest -= static_cast<std::size_t>(written);
    }
    return true;
}

pid_t startServer(int port)
{
    pid_t pid = fork();

    cr_assert_geq(pid, 0);
    if (pid == 0) {
        std::string portArg = std::to_string(port);
        char path[] = "../zappy_server";
        char p[] = "-p";
        char x[] = "-x";
        char width[] = "10";
        char y[] = "-y";
        char height[] = "10";
        char c[] = "-c";
        char clients[] = "2";
        char f[] = "-f";
        char frequency[] = "100";
        char n[] = "-n";
        char red[] = "red";
        char blue[] = "blue";
        char *argv[] = {
            path, p, portArg.data(), x, width, y, height, c, clients,
            f, frequency, n, red, blue, nullptr
        };

        std::freopen("/dev/null", "w", stdout);
        std::freopen("/dev/null", "w", stderr);
        execv(path, argv);
        _exit(84);
    }
    return pid;
}

void stopServer(pid_t pid)
{
    if (pid <= 0)
        return;
    kill(pid, SIGTERM);
    waitpid(pid, nullptr, 0);
}

bool waitServerReady(int port)
{
    for (int attempt = 0; attempt < 60; attempt++) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr = {};

        if (fd < 0)
            return false;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = htons(port);
        if (connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == 0) {
            close(fd);
            return true;
        }
        close(fd);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return false;
}

Test(network_protocol, ai_handshake, .timeout = 5)
{
    int port = freePort();
    pid_t server = startServer(port);
    int fd = -1;

    cr_assert(waitServerReady(port));
    fd = connectToServer(port);
    cr_expect_eq(readLine(fd), "WELCOME\n");
    cr_expect(writeAll(fd, "red\n"));
    cr_expect_eq(readLine(fd), "1\n");
    cr_expect_eq(readLine(fd), "10 10\n");
    cr_expect(writeAll(fd, "Connect_nbr\n"));
    cr_expect_eq(readLine(fd), "1\n");
    close(fd);
    stopServer(server);
}

Test(network_protocol, graphic_handshake, .timeout = 5)
{
    int port = freePort();
    pid_t server = startServer(port);
    int fd = -1;
    bool foundMapSize = false;

    cr_assert(waitServerReady(port));
    fd = connectToServer(port);
    cr_expect_eq(readLine(fd), "WELCOME\n");
    cr_expect(writeAll(fd, "GRAPHIC\n"));

    std::vector<std::string> lines;
    for (int index = 0; index < 108; index++)
        lines.push_back(readLine(fd));
    cr_expect_eq(lines[0], "msz 10 10\n");
    cr_expect_eq(lines[1], "sgt 100\n");
    cr_expect(writeAll(fd, "msz\n"));
    for (int index = 0; index < 8; index++) {
        if (readLine(fd) == "msz 10 10\n") {
            foundMapSize = true;
            break;
        }
    }
    close(fd);
    stopServer(server);
    cr_assert(foundMapSize);
}

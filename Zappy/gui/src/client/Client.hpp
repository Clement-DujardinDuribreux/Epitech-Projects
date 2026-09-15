/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Client def
*/

#ifndef CLIENT
    #define CLIENT

    #include <functional>
    #include <memory>
    #include <chrono>
    #include "client/threads/Thread.hpp"
    #include "client/threads/MyMutex.hpp"
    #include "client/threads/ScopedLock.hpp"
    #include <SFML/Network.hpp>
    #include <string>
    #include "Player.hpp"
    #include "utils/Socket.hpp"
    #include "utils/Poll.hpp"
    #include "Egg.hpp"
    #include "Map.hpp"

    #define CMD_WELCOME      "WELCOME"
    #define CMD_MAP_SIZE     "msz"
    #define CMD_TILE_CONTENT "bct"
    #define CMD_TEAM_NAME    "tna"
    #define CMD_PLAYER_NEW   "pnw"
    #define CMD_PLAYER_POS   "ppo"
    #define CMD_PLAYER_LVL   "plv"
    #define CMD_PLAYER_DEAD  "pdi"
    #define CMD_PLAYER_INV   "pin"
    #define CMD_PLAYER_EXP   "pex"
    #define CMD_PLAYER_BROAD "pbc"
    #define CMD_INCANT_START "pic"
    #define CMD_INCANT_END   "pie"
    #define CMD_PLAYER_DROP  "pdr"
    #define CMD_PLAYER_TAKE  "pgt"
    #define CMD_PLAYER_FORK  "pfk"
    #define CMD_EGG_NEW      "enw"
    #define CMD_EGG_BOUND    "ebo"
    #define CMD_EGG_DEAD     "edi"
    #define CMD_TIME_GET     "sgt"
    #define CMD_TIME_SET     "sst"
    #define CMD_GAME_END     "seg"
    #define CMD_SERVER_MSG   "smg"
    #define CMD_UNKNOWN      "suc"
    #define CMD_PARAMETER    "sbp"

/**
 * @brief Network client used by the GUI to consume the graphical protocol.
 */
class Client {
    private:
        std::unique_ptr<Socket> _socket;
        Poll _poll;
        std::string _buffer;
        std::string _writeBuffer;
        Map _map;
        int _timeUnit = 100;
        std::vector<std::string> _teamName;
        std::vector<std::string> _serverMessage;
        bool _isEnding = false;
        std::string _winnerTeam;
        std::unique_ptr<Thread<void>> _thread;
        bool _running = false;
        MyMutex _mutex;
        std::chrono::steady_clock::time_point _lastSendTime = std::chrono::steady_clock::now();

        using CommandHandler = std::function<void(const std::vector<std::string> &)>;
        std::map<std::string, CommandHandler> _commandMap = {
            { CMD_WELCOME,      [this](const std::vector<std::string> &a){ _handleWelcome(a);        } },
            { CMD_MAP_SIZE,     [this](const std::vector<std::string> &a){ _handleMapSize(a);        } },
            { CMD_TILE_CONTENT, [this](const std::vector<std::string> &a){ _handleTileContent(a);    } },
            { CMD_TEAM_NAME,    [this](const std::vector<std::string> &a){ _handleTeamName(a);       } },
            { CMD_PLAYER_NEW,   [this](const std::vector<std::string> &a){ _handlePlayerNew(a);      } },
            { CMD_PLAYER_POS,   [this](const std::vector<std::string> &a){ _handlePlayerPos(a);      } },
            { CMD_PLAYER_LVL,   [this](const std::vector<std::string> &a){ _handlePlayerLevel(a);    } },
            { CMD_PLAYER_INV,   [this](const std::vector<std::string> &a){ _handlePlayerInv(a);      } },
            { CMD_PLAYER_EXP,   [this](const std::vector<std::string> &a){ _handlePlayerExp(a);      } },
            { CMD_PLAYER_BROAD, [this](const std::vector<std::string> &a){ _handlePlayerBroadcast(a);} },
            { CMD_PLAYER_DROP,  [this](const std::vector<std::string> &a){ _handlePlayerDrop(a);     } },
            { CMD_PLAYER_TAKE,  [this](const std::vector<std::string> &a){ _handlePlayerTake(a);     } },
            { CMD_PLAYER_FORK,  [this](const std::vector<std::string> &a){ _handlePlayerFork(a);     } },
            { CMD_PLAYER_DEAD,  [this](const std::vector<std::string> &a){ _handlePlayerDead(a);     } },
            { CMD_INCANT_START, [this](const std::vector<std::string> &a){ _handleIncantStart(a);    } },
            { CMD_INCANT_END,   [this](const std::vector<std::string> &a){ _handleIncantEnd(a);      } },
            { CMD_EGG_NEW,      [this](const std::vector<std::string> &a){ _handleEggNew(a);         } },
            { CMD_EGG_BOUND,    [this](const std::vector<std::string> &a){ _handleEggBound(a);       } },
            { CMD_EGG_DEAD,     [this](const std::vector<std::string> &a){ _handleEggDead(a);        } },
            { CMD_TIME_GET,     [this](const std::vector<std::string> &a){ _handleTimeGet(a);        } },
            { CMD_TIME_SET,     [this](const std::vector<std::string> &a){ _handleTimeSet(a);        } },
            { CMD_GAME_END,     [this](const std::vector<std::string> &a){ _handleGameEnd(a);        } },
            { CMD_SERVER_MSG,   [this](const std::vector<std::string> &a){ _handleMessageServer(a);  } },
            { CMD_UNKNOWN,      [this](const std::vector<std::string> &a){ _handleUnknownCommand(a); } },
            { CMD_PARAMETER,    [this](const std::vector<std::string> &a){ _handleBadParameter(a);   } },
        };

        /**
         * @brief Extracts complete newline-terminated commands from the receive buffer.
         */
        void _extractCommands();

        /**
         * @brief Parses one protocol line and dispatches it to the matching handler.
         * @param command Raw command line without trailing newline.
         */
        void _parseCommand(const std::string &command);

        /** @brief Handles the server welcome command. @param args Tokenized command arguments. */
        void _handleWelcome([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles the map size command. @param args Tokenized command arguments. */
        void _handleMapSize([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles one tile content command. @param args Tokenized command arguments. */
        void _handleTileContent([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles one team name command. @param args Tokenized command arguments. */
        void _handleTeamName([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles creation of a player. @param args Tokenized command arguments. */
        void _handlePlayerNew([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player position updates. @param args Tokenized command arguments. */
        void _handlePlayerPos([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player level updates. @param args Tokenized command arguments. */
        void _handlePlayerLevel([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player inventory updates. @param args Tokenized command arguments. */
        void _handlePlayerInv([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player expulsion events. @param args Tokenized command arguments. */
        void _handlePlayerExp([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player broadcast messages. @param args Tokenized command arguments. */
        void _handlePlayerBroadcast([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player resource drop events. @param args Tokenized command arguments. */
        void _handlePlayerDrop([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player resource take events. @param args Tokenized command arguments. */
        void _handlePlayerTake([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player fork events. @param args Tokenized command arguments. */
        void _handlePlayerFork([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles player death events. @param args Tokenized command arguments. */
        void _handlePlayerDead([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles incantation start events. @param args Tokenized command arguments. */
        void _handleIncantStart([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles incantation end events. @param args Tokenized command arguments. */
        void _handleIncantEnd([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles egg creation events. @param args Tokenized command arguments. */
        void _handleEggNew([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles egg connection events. @param args Tokenized command arguments. */
        void _handleEggBound([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles egg death events. @param args Tokenized command arguments. */
        void _handleEggDead([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles current time unit responses. @param args Tokenized command arguments. */
        void _handleTimeGet([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles updated time unit responses. @param args Tokenized command arguments. */
        void _handleTimeSet([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles game end events. @param args Tokenized command arguments. */
        void _handleGameEnd([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles server messages. @param args Tokenized command arguments. */
        void _handleMessageServer([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles unknown command errors reported by the server. @param args Tokenized command arguments. */
        void _handleUnknownCommand([[maybe_unused]]const std::vector<std::string> &args);
        /** @brief Handles bad parameter errors reported by the server. @param args Tokenized command arguments. */
        void _handleBadParameter([[maybe_unused]]const std::vector<std::string> &args);

    public:
        /**
         * @brief Creates a disconnected GUI client.
         */
        Client();

        /**
         * @brief Stops the update thread and releases the client.
         */
        ~Client();

        /**
         * @brief Connects to the Zappy server and starts asynchronous updates.
         * @param ip Hostname or IP address.
         * @param port TCP port.
         * @return True when the connection succeeds.
         */
        bool connectToServer(std::string ip, int port);

        /**
         * @brief Polls the socket once and applies received protocol commands.
         */
        void update();

        /**
         * @brief Starts the background update thread.
         */
        void startUpdateThread();

        /**
         * @brief Stops the background update thread.
         */
        void stopUpdateThread();

        /**
         * @brief Sends a raw protocol message to the server.
         * @param msg Message to send, usually ending with a newline.
         */
        void sendMsg(std::string msg);

        /**
         * @brief Returns whether the server announced the end of the game.
         * @return True after a `seg` command.
         */
        bool getEnd() const;

        /**
         * @brief Returns the winning team announced by the server.
         * @return Winner team name, or an empty string before game end.
         */
        std::string getWinnerTeam() const;

        /**
         * @brief Sets the local end-of-game flag.
         * @param end New end state.
         */
        void setEnd(bool end);

        /**
         * @brief Returns the current time unit.
         * @return Server time unit.
         */
        int getTimeUnit() const;

        /**
         * @brief Sets the current time unit.
         * @param timeUnit New time unit.
         */
        void setTimeUnit(int timeUnit);

        /**
         * @brief Returns a copy of the current map state.
         * @return Map snapshot protected by the client mutex.
         */
        Map getMap() const;

        /**
         * @brief Requests the map size from the server.
         */
        void requestMapSize();

        /**
         * @brief Requests the content of one tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         */
        void requestContentTile(int x, int y);

        /**
         * @brief Requests the content of every tile.
         */
        void requestContentMap();

        /**
         * @brief Requests all team names.
         */
        void requestNameTeams();

        /**
         * @brief Requests one player position.
         * @param id Player identifier.
         */
        void requestPlayerPosition(int id);

        /**
         * @brief Requests one player level.
         * @param id Player identifier.
         */
        void requestPlayerLevel(int id);

        /**
         * @brief Requests one player inventory.
         * @param id Player identifier.
         */
        void requestPlayerInventory(int id);

        /**
         * @brief Requests the current time unit.
         */
        void requestTimeUnit();

        /**
         * @brief Requests a server time unit update.
         * @param time New time unit.
         */
        void requestEditTimeUnit(int time);
};

#endif

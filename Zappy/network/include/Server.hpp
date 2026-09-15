/*
** EPITECH PROJECT, 2026
** Server.hpp
** File description:
** Server class definition
*/

#ifndef SERVER
    #define SERVER

    #include <poll.h>
    #include <vector>
    #include <map>
    #include <memory>
    #include <queue>
    #include <ctime>
    #include <utility>
    #include "Client.hpp"
    #include "game/Timeline.hpp"
    #include "game/Game.hpp"
    #include "utils/Poll.hpp"
    #include "utils/Socket.hpp"
    #include "commands/CommandFactory.hpp"
    #include "utils/ServerArgs.hpp"

/**
 * @class Server
 * @brief Runs the Zappy TCP server, game loop, and text protocol.
 */
class Server {
    private:
        /**
         * @enum State
         * @brief High-level server state.
         */
        enum State {
            /** Game still accepts and executes AI actions. */
            RUNNING,
            /** Victory was reached; only allowed GUI/event commands continue. */
            GAME_OVER
        };

        ServerArgs _args;
        bool _isOn;
        State _state;
        Socket _socket;
        Poll _poll;
        
        std::map<int, Client> _clients;
        std::vector<int> _graphicsFds;
        std::map<std::string, int> _teamsRest;
        int _nextPlayerId;
        int _timeUnit;
        std::string _winnerTeam;

        Timeline _timeline;
        Game _game;

        /**
         * @brief Creates initial eggs for every configured team slot.
         */
        void initializeEggs();
        /**
         * @brief Prints the startup summary to stdout.
         */
        void printStartupBanner() const;
        /**
         * @brief Switches the server to game-over state.
         * @param teamName Winning team name.
         */
        void enterGameOver(const std::string &teamName);
        /**
         * @brief Accepts one pending TCP client and sends WELCOME.
         */
        void acceptClient();
        /**
         * @brief Removes a client and updates scheduled poll indexes.
         * @param fd Client file descriptor.
         */
        void removeClient(int fd);

        /**
         * @brief Handles the team or GRAPHIC registration line.
         * @param poll_index Poll index of the client.
         * @param teamName Registration token sent by the client.
         */
        void sendTeamNumber(int poll_index, std::string& teamName);

        /**
         * @brief Parses complete protocol lines from a client read buffer.
         * @param poll_index Poll index of the client.
         */
        void processClientCommands(int poll_index);

        /**
         * @brief Handles readable and writable events for a client socket.
         * @param poll_index Poll index of the client.
         */
        void processClientSocket(int poll_index);

        /**
         * @brief Disconnects a client from a poll index or file descriptor.
         * @param poll_index Poll index or file descriptor, depending on caller.
         */
        void clientDeconnection(int poll_index);

        /**
         * @brief Executes the next due timeline action.
         */
        void executeTimeline();
        /**
         * @brief Consumes one food unit for a player or schedules death.
         * @param playerId Player identifier.
         */
        void consumeFood(int playerId);
        /**
         * @brief Respawns missing map resources and notifies graphical clients.
         */
        void respawnResources();
        /**
         * @brief Completes a scheduled incantation attempt.
         * @param args Serialized incantation data.
         */
        void finishIncantation(const std::string &args);
        /**
         * @brief Checks whether a team reached the victory condition.
         * @param teamName Team to inspect.
         */
        void checkVictory(const std::string &teamName);

    public:
        /**
         * @brief Builds a server from parsed command-line arguments.
         * @param args Parsed server configuration.
         */
        Server(ServerArgs args);
        ~Server() = default;

        /**
         * @brief Starts the main poll loop.
         */
        void startUp();

        /**
         * @brief Queues data to a client write buffer.
         * @param poll_index Poll index of the target client.
         * @param str Protocol payload.
         */
        void sendToClient(std::size_t poll_index, std::string str);

        /**
         * @brief Queues data to every graphical client.
         * @param str Graphical protocol payload.
         */
        void sendToGraphics(std::string str);
        /**
         * @brief Schedules a graphical event for immediate timeline execution.
         * @param event Graphical protocol payload.
         */
        void queueGraphicsEvent(const std::string &event);
        /**
         * @brief Executes one graphical protocol request.
         * @param command GUI command name.
         * @param args Raw request arguments.
         * @param pollIndex Poll index of the graphical client.
         */
        void executeGuiCommand(const std::string &command,
            const std::string &args, int pollIndex);
        /**
         * @brief Executes one internal server event.
         * @param args Event name and serialized arguments.
         */
        void executeGameEvent(const std::string &args);

        /**
         * @brief Parses a resource protocol name.
         * @param name Textual resource name.
         * @param resource Output resource enum value.
         * @return True when the name is valid.
         */
        static bool parseResource(const std::string &name, Ressources &resource);

        /**
         * @brief Finds the poll index attached to a player id.
         * @param playerId Player identifier.
         * @return Poll index, or -1 when not found.
         */
        int findPollIndexByPlayerId(int playerId);
        /**
         * @brief Schedules an internal server event after a number of cycles.
         * @param cycles Delay in game cycles.
         * @param event Event payload.
         */
        void scheduleGameEvent(int cycles, const std::string &event);

        /**
         * @brief Notifies graphical clients about a new player.
         * @param pollIndex Poll index of the player client.
         */
        void notifyPlayerConnection(int pollIndex);
        /**
         * @brief Notifies graphical clients about a player position.
         * @param pollIndex Poll index of the player client.
         */
        void notifyPlayerPosition(int pollIndex);
        /**
         * @brief Notifies graphical clients about a player level.
         * @param pollIndex Poll index of the player client.
         */
        void notifyPlayerLevel(int pollIndex);
        /**
         * @brief Notifies graphical clients about a player inventory.
         * @param pollIndex Poll index of the player client.
         */
        void notifyPlayerInventory(int pollIndex);
        /**
         * @brief Notifies graphical clients about a player expulsion.
         * @param playerId Expelling player identifier.
         */
        void notifyPlayerExpulsion(int playerId);
        /**
         * @brief Notifies graphical clients about a broadcast.
         * @param pollIndex Poll index of the sending player.
         * @param message Broadcast message.
         */
        void notifyPlayerBroadcast(int pollIndex, const std::string &message);
        /**
         * @brief Notifies graphical clients that an incantation started.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @param level Incantation level.
         * @param playerIds Participating player identifiers.
         */
        void notifyIncantationStart(std::size_t x, std::size_t y, int level,
            const std::vector<int> &playerIds);
        /**
         * @brief Notifies graphical clients that an incantation ended.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @param success True when the elevation succeeded.
         */
        void notifyIncantationEnd(std::size_t x, std::size_t y, bool success);
        /**
         * @brief Notifies graphical clients that a player started forking.
         * @param pollIndex Poll index of the player client.
         */
        void notifyPlayerFork(int pollIndex);
        /**
         * @brief Notifies graphical clients that a resource was dropped.
         * @param pollIndex Poll index of the player client.
         * @param resource Dropped resource.
         */
        void notifyResourceDrop(int pollIndex, Ressources resource);
        /**
         * @brief Notifies graphical clients that a resource was collected.
         * @param pollIndex Poll index of the player client.
         * @param resource Collected resource.
         */
        void notifyResourceCollect(int pollIndex, Ressources resource);
        /**
         * @brief Notifies graphical clients that a player died.
         * @param playerId Dead player identifier.
         */
        void notifyPlayerDeath(int playerId);
        /**
         * @brief Notifies graphical clients that an egg was laid.
         * @param eggId Egg identifier.
         * @param playerId Creator player identifier.
         * @param x Egg x coordinate.
         * @param y Egg y coordinate.
         */
        void notifyEggPutGround(int eggId, int playerId, std::size_t x,
            std::size_t y);
        /**
         * @brief Notifies graphical clients that an egg was consumed by a connection.
         * @param eggId Egg identifier.
         */
        void notifyEggConnection(int eggId);
        /**
         * @brief Notifies graphical clients that an egg was destroyed.
         * @param eggId Egg identifier.
         */
        void notifyEggDeath(int eggId);
        /**
         * @brief Notifies graphical clients that the game ended.
         * @param teamName Winning team.
         */
        void notifyGameEnd(const std::string &teamName);
        /**
         * @brief Sends a server message to graphical clients.
         * @param message Message text.
         */
        void notifyServerMessage(const std::string &message);

        /**
         * @brief Converts an internal direction to a GUI protocol orientation.
         * @param direction Internal direction.
         * @return Protocol orientation from 1 to 4.
         */
        static int protocolOrientation(Directions direction);
        /**
         * @brief Removes a leading player id marker.
         * @param value Protocol value, optionally prefixed by '#'.
         * @return Value without the leading marker.
         */
        static std::string stripPlayerPrefix(std::string value);
        /**
         * @brief Replaces line breaks in protocol message payloads.
         * @param message Raw message.
         * @return Message safe to embed in one protocol line.
         */
        static std::string cleanProtocalMessage(std::string message);

        /**
         * @brief Formats a tile content event.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return `bct` protocol line.
         */
        std::string formatTile(std::size_t x, std::size_t y);
        /**
         * @brief Formats a player position event.
         * @param playerId Player identifier.
         * @param player Player state to format.
         * @return `ppo` protocol line.
         */
        std::string formatPlayerPosition(int playerId, Player &player);
        /**
         * @brief Formats a player level event.
         * @param playerId Player identifier.
         * @param player Player state to format.
         * @return `plv` protocol line.
         */
        std::string formatPlayerLevel(int playerId, Player &player);
        /**
         * @brief Formats a player inventory event.
         * @param playerId Player identifier.
         * @param player Player state to format.
         * @return `pin` protocol line.
         */
        std::string formatPlayerInventory(int playerId, Player &player);

        /**
         * @brief Marks a client for disconnection after pending writes.
         * @param poll_index Poll index of the client.
         */
        void disconnectClient(std::size_t poll_index);

        /**
         * @brief Gets connected clients by file descriptor.
         * @return Mutable client map.
         */
        std::map<int, Client>& getClients();
        /**
         * @brief Gets remaining team slots.
         * @return Mutable team slot map.
         */
        std::map<std::string, int>& getTeams();
        /**
         * @brief Gets the poll wrapper.
         * @return Mutable poll object.
         */
        Poll& getPoll();
        /**
         * @brief Gets the player id attached to a poll index.
         * @param pollIndex Poll index of the client.
         * @return Player id, or -1 when unavailable.
         */
        int getPlayerId(int pollIndex);

        /**
         * @class ServerException
         * @brief Reports unrecoverable server protocol or socket errors.
         */
        class ServerException : public std::exception {
            public:
                /**
                 * @brief Builds an exception with a diagnostic message.
                 * @param msg Message returned by what().
                 */
                ServerException(const std::string &msg);
                ~ServerException() = default;
                /**
                 * @brief Gets the diagnostic message.
                 * @return Null-terminated message string.
                 */
                const char *what() const noexcept override;

            private:
                std::string _msg;
        };
};

#endif

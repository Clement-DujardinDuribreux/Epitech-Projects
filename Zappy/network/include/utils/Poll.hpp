/*
** EPITECH PROJECT, 2026
** Poll.hpp
** File description:
** Poll class definition
*/

#ifndef POLL
    #define POLL

    #include <poll.h>
    #include <vector>
    #include <unistd.h>

/**
 * @class Poll
 * @brief Owns the pollfd list used by the server event loop.
 */
class Poll {
    private:
        std::vector<pollfd> _fds;
        
    public:
        /**
         * @brief Builds an empty poll set.
         */
        Poll();
        /**
         * @brief Closes every registered file descriptor.
         */
        ~Poll();

        /**
         * @brief Gets the mutable pollfd list.
         * @return Registered descriptors and their events.
         */
        std::vector<pollfd>& getFds();

        /**
         * @brief Adds a descriptor watched for input events.
         * @param fd File descriptor to register.
         */
        void addFd(int fd);
        /**
         * @brief Removes a descriptor from the poll set.
         * @param fd File descriptor to remove.
         */
        void removeFd(int fd);

        /**
         * @brief Waits for activity on registered descriptors.
         * @param timeout Poll timeout in milliseconds, or -1 for infinite wait.
         * @return poll(2) return value.
         */
        int wait(int timeout);
};

#endif

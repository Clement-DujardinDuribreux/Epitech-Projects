/*
** EPITECH PROJECT, 2026
** Timeline.hpp
** File description:
** Timeline class definition
*/

#ifndef TIMELINE
    #define TIMELINE

    #include <memory>
    #include <chrono>
    #include <iostream>
    #include "commands/ICommand.hpp"

/**
 * @struct Action
 * @brief Scheduled command entry stored in the timeline.
 */
struct Action {
    /** Absolute time at which the command can execute. */
    std::chrono::system_clock::time_point timeToExec;
    /** Command object to execute. */
    std::unique_ptr<ICommand> command;
    /** Previous action in the sorted list. */
    std::weak_ptr<Action> prev;
    /** Next action in the sorted list. */
    std::shared_ptr<Action> next;
    /** Poll index of the client, or a negative value for server events. */
    int poll_index;
    /** Command arguments or server event payload. */
    std::string args;
};


/**
 * @class Timeline
 * @brief Keeps scheduled commands ordered by execution time.
 */
class Timeline {
    private:
        std::shared_ptr<Action> _first;
        std::shared_ptr<Action> _last;
        std::size_t _size;

    public:
        /**
         * @brief Builds an empty timeline.
         */
        Timeline();
        ~Timeline() = default;
        
        /**
         * @brief Gets the earliest action.
         * @return First action, or nullptr when empty.
         */
        std::shared_ptr<Action> getFirst();
        /**
         * @brief Gets the latest action.
         * @return Last action, or nullptr when empty.
         */
        std::shared_ptr<Action> getLast();
        /**
         * @brief Gets the number of scheduled actions.
         * @return Timeline size.
         */
        std::size_t size();

        /**
         * @brief Inserts an action while preserving chronological order.
         * @param timeToExec Absolute execution time.
         * @param command Command to move into the timeline.
         * @param args Command arguments.
         * @param poll_index Target client poll index.
         */
        void addAction(std::chrono::system_clock::time_point timeToExec, std::unique_ptr<ICommand> &command, std::string args, int poll_index);
        /**
         * @brief Updates actions after a client poll index has been removed.
         * @param pollIndex Removed poll index.
         */
        void removePollIndex(int pollIndex);
        /**
         * @brief Removes and returns the earliest action.
         * @return Earliest scheduled action.
         */
        std::shared_ptr<Action> pop();

        /**
         * @brief Computes the poll timeout until the next action.
         * @return Milliseconds to wait, 0 if due, or -1 if empty.
         */
        int getNextTimeout();
};

#endif

/*
** EPITECH PROJECT, 2026
** Timeline.cpp
** File description:
** Timeline class definition
*/

#include "game/Timeline.hpp"

Timeline::Timeline() : _size(0) {}

std::shared_ptr<Action> Timeline::getFirst() {
    return _first;
}

std::shared_ptr<Action> Timeline::getLast() {
    return _last;
}

std::size_t Timeline::size() {
    return _size;
}

void Timeline::addAction(std::chrono::system_clock::time_point timeToExec, std::unique_ptr<ICommand> &command, std::string args, int poll_index) {
    std::shared_ptr<Action> newAction = std::make_shared<Action>();
    newAction->timeToExec = timeToExec;
    newAction->command = std::move(command);
    newAction->poll_index = poll_index;
    newAction->args = args;

    if (_size == 0) {
        _last = newAction;
        _first = newAction;
        _size++;
        return;
    }
    std::shared_ptr<Action> tmp;
    std::shared_ptr<Action> current = _last; 
    while (current != nullptr) {
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(current->timeToExec - newAction->timeToExec);
        if (diff.count() <= 0) {
            tmp = current->next;
            current->next = newAction;  
            current->next->prev = current;
            if (tmp)
                tmp->prev = current->next;
            else
                _last = current->next;
            current->next->next = tmp;
            _size++;
            return;
        }
        current = current->prev.lock();
    }
    tmp = _first;
    _first = newAction;
    tmp->prev = _first;
    _first->next = tmp;
    _size++;
}

void Timeline::removePollIndex(int pollIndex) {
    std::shared_ptr<Action> action = _first;

    while (action != nullptr) {
        if (action->poll_index == pollIndex)
            action->poll_index = -2;
        else if (action->poll_index > pollIndex)
            action->poll_index--;
        action = action->next;
    }
}

std::shared_ptr<Action> Timeline::pop() {
    std::shared_ptr<Action> first = _first;
    
    _first = _first->next;
    if (_first)
        _first->prev.reset();
    else
        _last = nullptr;
    _size--;

    return first;
}

int Timeline::getNextTimeout() {
    if (_size == 0)
        return -1;
    auto now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(_first->timeToExec - now);
    int rest = diff.count();
    if (rest <= 0)
        return 0;
    return rest;
}

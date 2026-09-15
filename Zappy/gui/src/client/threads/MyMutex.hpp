#ifndef MYMUTEX_HPP
    #define MYMUTEX_HPP

    #include "IMutex.hpp"

/**
 * @brief Pthread-based mutex implementation.
 */
class MyMutex : public IMutex {
    private:
        pthread_mutex_t _mutex;

    public:
        /**
         * @brief Initializes the native pthread mutex.
         */
        MyMutex() {
            pthread_mutex_init(&_mutex, NULL);
        }

        /**
         * @brief Destroys the native pthread mutex.
         */
        ~MyMutex() override {
            pthread_mutex_destroy(&_mutex);
        }

        /**
         * @brief Locks the mutex and blocks until it is available.
         */
        void lock() override {
            pthread_mutex_lock(&_mutex);
        }

        /**
         * @brief Unlocks the mutex.
         */
        void unlock() override {
            pthread_mutex_unlock(&_mutex);
        }

        /**
         * @brief Attempts to lock the mutex without blocking.
         */
        void trylock() override {
            pthread_mutex_trylock(&_mutex);
        }

        /**
         * @brief Returns the wrapped pthread mutex.
         * @return Pointer to the native mutex handle.
         */
        pthread_mutex_t *getNativeHandle() override {
            return &_mutex;
        }
};

#endif

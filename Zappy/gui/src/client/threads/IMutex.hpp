#ifndef IMUTEX_HPP
    #define IMUTEX_HPP

    #include <pthread.h>

/**
 * @brief Interface implemented by mutex wrappers used by the GUI client.
 */
class IMutex {
    public:
        /**
         * @brief Releases the mutex interface.
         */
        virtual ~IMutex() = default;

        /**
         * @brief Locks the mutex and blocks until it is available.
         */
        virtual void lock() = 0;

        /**
         * @brief Unlocks the mutex.
         */
        virtual void unlock() = 0;

        /**
         * @brief Attempts to lock the mutex without blocking.
         */
        virtual void trylock() = 0;

        /**
         * @brief Returns the wrapped pthread mutex.
         * @return Pointer to the native mutex handle.
         */
        virtual pthread_mutex_t *getNativeHandle() = 0;
};

#endif

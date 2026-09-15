#ifndef SCOPELOCK_HPP
    #define SCOPELOCK_HPP

    #include "MyMutex.hpp"

/**
 * @brief RAII lock guard for IMutex implementations.
 */
class ScopedLock {
    private:
        IMutex &_mutex;
    public:
        /**
         * @brief Locks the mutex for the lifetime of the object.
         * @param mutex Mutex to lock.
         */
        ScopedLock(IMutex &mutex) : _mutex(mutex) {
            _mutex.lock();
        }

        /**
         * @brief Unlocks the mutex.
         */
        ~ScopedLock() {
            _mutex.unlock();
        }

        /**
         * @brief Copy construction is disabled to keep one unlock owner.
         */
        ScopedLock(const ScopedLock &) = delete;

        /**
         * @brief Copy assignment is disabled to keep one unlock owner.
         * @return This object is never returned because assignment is deleted.
         */
        ScopedLock &operator=(const ScopedLock &) = delete;
};

#endif

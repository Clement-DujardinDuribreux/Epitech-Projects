#ifndef THREAD_HPP
    #define THREAD_HPP

    #include <pthread.h>
    #include <functional>

/**
 * @brief Small pthread wrapper that starts a function with one argument.
 * @tparam T Type of the argument passed to the thread function.
 */
template <typename T>
class Thread {
    private:
        pthread_t _thread;
        std::function<void(T)> _function;
        T _args;

        /**
         * @brief Pthread entry point for functions with one argument.
         * @param arg Pointer to the Thread instance.
         * @return Always nullptr.
         */
        static void *threadFunction(void *arg) {
            Thread *thread = static_cast<Thread *>(arg);
            thread->_function(thread->_args);
            return nullptr;
        }

        /**
         * @brief Pthread entry point for functions without arguments.
         * @param arg Pointer to the Thread instance.
         * @return Always nullptr.
         */
        static void *threadFunctionNoArgs(void *arg) {
            Thread *thread = static_cast<Thread *>(arg);
            thread->_function();
            return nullptr;
        }

    public:
        /**
         * @brief Starts a thread with one argument.
         * @param function Function executed in the new thread.
         * @param args Argument passed to the function.
         */
        Thread(std::function<void(T)> function, T args) : _function(function), _args(args) {
            pthread_create(&_thread, NULL, threadFunction, this);
        }

        /**
         * @brief Starts a thread without arguments.
         * @param function Function executed in the new thread.
         */
        Thread(std::function<void()> function) : _function(function) {
            pthread_create(&_thread, NULL, threadFunctionNoArgs, this);
        }

        /**
         * @brief Joins the thread before destruction.
         */
        ~Thread() {
            pthread_join(_thread, NULL);
        }
};

/**
 * @brief Void specialization for functions without arguments.
 */
template <>
class Thread<void> {
    private:
        pthread_t _thread;
        std::function<void()> _function;

        /**
         * @brief Pthread entry point for the void specialization.
         * @param arg Pointer to the Thread instance.
         * @return Always nullptr.
         */
        static void *threadFunction(void *arg) {
            Thread *thread = static_cast<Thread *>(arg);
            thread->_function();
            return nullptr;
        }

    public:
        /**
         * @brief Starts a thread without arguments.
         * @param function Function executed in the new thread.
         */
        Thread(std::function<void()> function) : _function(function) {
            pthread_create(&_thread, NULL, threadFunction, this);
        }

        /**
         * @brief Joins the thread before destruction.
         */
        ~Thread() {
            pthread_join(_thread, NULL);
        }
};

#endif

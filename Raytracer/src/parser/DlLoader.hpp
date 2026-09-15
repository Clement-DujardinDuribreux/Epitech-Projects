/*
** EPITECH PROJECT, 2026
** DlLoader
** File description:
** DlLoader def
*/

#ifndef DLLOADER
    #define DLLOADER

    #include <iostream>
    #include <string>
    #include <memory>
    #include <dlfcn.h>

template <typename T>
/** @class DlLoader
 * @brief A loader for dynamically loading shared libraries
 * This class is responsible for loading shared libraries at runtime and providing a simple interface for creating objects from the loaded library.
 * It uses the dlopen, dlsym, and dlclose functions from the dlfcn.h library to manage the dynamic loading of shared libraries. The class provides methods for setting entry and exit points for object creation and destruction, as well as a method for creating unique pointers to objects created from the loaded library.
 * The DlLoader class also defines a nested exception class, DlLoaderException, which is used to handle errors that may occur during the loading process, such as failure to open the library or find the specified functions.
 * @tparam T The type of object that will be created from the loaded library
 */
class DlLoader {
    private:
        std::string _path;
        void *_dl;
        T *(*_create)();
        void (*_exit)(T *);

        /** @class DlLoaderException
         * @brief An exception class for handling errors in the DlLoader
         * This class inherits from std::exception and is used to represent errors that occur during the loading of shared libraries in the DlLoader class. It contains a message that describes the error, which can be retrieved using the what() method.
         */
        class DlLoaderException : public std::exception {
            private:
                std::string _message;
            public:
                /** @brief Construct a DlLoaderException with a specific error message
                 * @param message The error message describing the exception
                 */
                DlLoaderException(const std::string &message) : _message(message) {}
                /** @brief Get the error message associated with the exception
                 * @return The error message as a C-style string
                 */
                const char *what() const noexcept override { return _message.c_str(); }
        };

    public:
        /** @brief Construct a DlLoader object
         * This constructor initializes the DlLoader with default values and does not load any library.
         */
        DlLoader() : _dl(nullptr), _create(nullptr), _exit(nullptr) {}

        /** @brief Construct a DlLoader object and load a library
         * @param path The path to the shared library to be loaded
         * This constructor initializes the DlLoader and attempts to load the specified shared library. If the library cannot be loaded, it throws a DlLoaderException with an appropriate error message.
         */
        DlLoader(std::string path) : _path(path), _dl(nullptr), _create(nullptr), _exit(nullptr) {
            open(path);
        }

        /** @brief Construct a DlLoader object and load a library with specified entry and exit points
         * @param path The path to the shared library to be loaded
         * @param entryPoint The name of the function to be used as the entry point for object creation
         * @param exitPoint The name of the function to be used as the exit point for object destruction
         * This constructor initializes the DlLoader and attempts to load the specified shared library, as well as set the entry and exit points for object creation and destruction. If any of these steps fail, it throws a DlLoaderException with an appropriate error message.
         */
        DlLoader(std::string path, std::string entryPoint, std::string exitPoint) : _path(path), _dl(nullptr), _create(nullptr), _exit(nullptr) {
            open(path, entryPoint, exitPoint);
        }

        /** @brief Destroy the DlLoader object and close the loaded library
         * This destructor checks if a library is currently loaded and, if so, it closes the library using dlclose. It ensures that any resources associated with the loaded library are properly released when the DlLoader object is destroyed.
         */
        ~DlLoader() {
            if (_dl)
                dlclose(_dl);
        }

        /** @brief Open a shared library
         * @param path The path to the shared library to be loaded
         * This method attempts to open the specified shared library using dlopen. If the library cannot be opened, it throws a DlLoaderException with an appropriate error message.
         */
        void open(std::string path) {
            _path = path;
            _dl = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
            if (!_dl)
                throw DlLoaderException("Cannot open library: " + std::string(dlerror()));
        }

        /** @brief Set the entry point for object creation
         * @param entryPoint The name of the function to be used as the entry point for object creation
         * This method attempts to find the specified function in the loaded library using dlsym and sets it as the entry point for object creation. If the function cannot be found, it throws a DlLoaderException with an appropriate error message.
         */
        void setEntryPoint(std::string entryPoint) {
            dlerror();
            _create = reinterpret_cast<T *(*)()>(dlsym(_dl, entryPoint.c_str()));
            char *err = dlerror();
            if (err)
                throw DlLoaderException("Cannot find " + entryPoint + " in " + _path + ": " + std::string(err));
        }

        /** @brief Set the exit point for object destruction
         * @param exitPoint The name of the function to be used as the exit point for object destruction
         * This method attempts to find the specified function in the loaded library using dlsym and sets it as the exit point for object destruction. If the function cannot be found, it throws a DlLoaderException with an appropriate error message.
         */
        void setExitPoint(std::string exitPoint) {
            dlerror();
            _exit = reinterpret_cast<void (*)(T *)>(dlsym(_dl, exitPoint.c_str()));
            char *err = dlerror();
            if (err)
                throw DlLoaderException("Cannot find " + exitPoint + " in " + _path + ": " + std::string(err));
        }

        /** @brief Open a shared library and set entry and exit points
         * @param path The path to the shared library to be loaded
         * @param entryPoint The name of the function to be used as the entry point for object creation
         * @param exitPoint The name of the function to be used as the exit point for object destruction
         * This method combines the functionality of opening a shared library and setting the entry and exit points for object creation and destruction. It first opens the specified library and then sets the entry and exit points using the provided function names. If any of these steps fail, it throws a DlLoaderException with an appropriate error message.
         */
        void open(std::string path, std::string entryPoint, std::string exitPoint) {
            open(path);
            setEntryPoint(entryPoint);
            setExitPoint(exitPoint);
        }

        /** @brief Get the raw pointer to the loaded library
         * @return A void pointer representing the loaded library
         * This method returns the raw pointer to the loaded library, which can be used for advanced operations or debugging purposes. It allows access to the underlying library handle managed by the DlLoader.
         */
        void *getDl() const { return _dl; }

        /** @brief Check if a specific function exists in the loaded library
         * @param name The name of the function to check for
         * @return True if the function exists in the loaded library, false otherwise
         * This method checks if a function with the specified name exists in the loaded library by using dlsym to attempt to retrieve its address. It returns true if the function is found and false if it is not found or if an error occurs during the lookup.
         */
        bool hasFunction(std::string name) {
            dlerror();
            return dlsym(_dl, name.c_str()) != nullptr;
        }

        /** @brief Create a unique pointer to an object created from the loaded library
         * @return A unique pointer to an object of type T created using the entry point function
         * This method uses the entry point function defined in the loaded library to create an object of type T and returns it as a unique pointer. The unique pointer is configured with a custom deleter that uses the exit point function to properly destroy the object when it goes out of scope.
         */
        std::unique_ptr<T, void (*)(T *)> createUnique() {
            return std::unique_ptr<T, void (*)(T *)>(_create(), _exit);
        }

        /** @brief Get a specific function from the loaded library
         * @tparam U The type of the function to retrieve
         * @param name The name of the function to retrieve
         * @return A pointer to the specified function
         * This method attempts to retrieve a function with the specified name from the loaded library using dlsym and returns it as a pointer of the specified type. If the function cannot be found, it throws a DlLoaderException with an appropriate error message.
         */
        template <typename U>
        U getFunction(std::string name) {
            dlerror();
            U func = reinterpret_cast<U>(dlsym(_dl, name.c_str()));
            char *err = dlerror();
            if (err)
                throw DlLoaderException("Cannot find function " + name + " in " + _path + ": " + std::string(err));
            return func;
        }
};

#endif
#ifndef ARGVPARSER_HPP
    #define ARGVPARSER_HPP

    #include <vector>
    #include <functional>
    #include <string>

/**
 * @brief Extracts typed values from the GUI command line.
 */
class ArgvParser {
    private:
        std::vector<std::string> _argv;

    public:
        /**
         * @brief Copies a null-terminated argv array into an owned vector.
         * @param argv Null-terminated argument array received by main.
         */
        ArgvParser(char **argv) { for (int i = 0; argv[i] != nullptr; i++) { _argv.push_back(argv[i]); } }

        /**
         * @brief Releases the parser.
         */
        ~ArgvParser() = default;

        /**
         * @brief Returns the copied argument list.
         * @return Vector containing every command-line argument.
         */
        std::vector<std::string> getArgv() const { return _argv; }

        /**
         * @brief Returns one argument by position.
         * @param index Position inside the copied argument list.
         * @return Argument value at the requested index.
         * @throw ArgvParserException If the index is outside the argument list.
         */
        std::string getArgFromIndex(size_t index) {
            if (index < _argv.size())
                return _argv[index];
            throw ArgvParserException("Index out of range");
        }

        /**
         * @brief Reads and converts the value placed after a flag.
         * @tparam T Type returned by the converter.
         * @param flag Flag to search, for example "-p".
         * @param converter Function used to convert the raw string value.
         * @return Converted value associated with the flag.
         * @throw ArgvParserException If the flag is missing, has no value or conversion fails.
         */
        template<typename T>
        T getArgFromFlag(const std::string &flag, std::function<T(const std::string&)> converter) {
            try {
                for (size_t i = 0; i < _argv.size(); i++)
                    if (_argv[i] == flag && i + 1 < _argv.size())
                        return converter(_argv[i + 1]);
            } catch (const std::exception &e) {
                throw ArgvParserException("Conversion failed: " + std::string(e.what()));
            }
            throw ArgvParserException("Flag not found or no value provided");
        }

        /**
         * @brief Error raised when command-line parsing fails.
         */
        class ArgvParserException : public std::exception {
            private:
                std::string _message;
            public:
                /**
                 * @brief Builds a parser error.
                 * @param message Human-readable error message.
                 */
                ArgvParserException(const std::string &message) : _message(message) {}

                /**
                 * @brief Returns the stored error message.
                 * @return Null-terminated error message.
                 */
                const char *what() const noexcept override { return _message.c_str(); }
        };

};

#endif

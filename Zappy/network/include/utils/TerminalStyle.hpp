/*
** EPITECH PROJECT, 2026
** TerminalStyle.hpp
** File description:
** TerminalStyle class definition
*/

#ifndef TERMINAL_STYLE_HPP
    #define TERMINAL_STYLE_HPP

    #include <ostream>

/**
 * @class TerminalStyle
 * @brief Represents an ANSI terminal style sequence.
 */
class TerminalStyle {
    public:
        /**
         * @brief Creates the reset style.
         * @return Style writing the ANSI reset sequence.
         */
        static TerminalStyle reset();
        /**
         * @brief Creates the bold cyan style.
         * @return Style writing the ANSI bold cyan sequence.
         */
        static TerminalStyle boldCyan();
        /**
         * @brief Creates the bold green style.
         * @return Style writing the ANSI bold green sequence.
         */
        static TerminalStyle boldGreen();
        /**
         * @brief Creates the bold yellow style.
         * @return Style writing the ANSI bold yellow sequence.
         */
        static TerminalStyle boldYellow();
        /**
         * @brief Creates the dim style.
         * @return Style writing the ANSI dim sequence.
         */
        static TerminalStyle dim();

        ~TerminalStyle() = default;

        /**
         * @brief Gets the ANSI escape sequence.
         * @return Null-terminated ANSI sequence.
         */
        const char *sequence() const;

    private:
        enum Code {
            RESET,
            BOLD_CYAN,
            BOLD_GREEN,
            BOLD_YELLOW,
            DIM
        };

        /**
         * @brief Builds a style from an internal style code.
         * @param code Style code to render.
         */
        TerminalStyle(Code code);
        Code _code;
};

/**
 * @brief Writes an ANSI style sequence to an output stream.
 * @param stream Target output stream.
 * @param style Style to write.
 * @return The target stream.
 */
std::ostream &operator<<(std::ostream &stream, const TerminalStyle &style);

#endif

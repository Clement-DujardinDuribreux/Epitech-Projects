/*
** EPITECH PROJECT, 2026
** Raytracer
** File description:
** CommonParser
*/

#ifndef COMMONPARSER
    #define COMMONPARSER

#include "Point3D.hpp"
#include "Vector3D.hpp"

#include <iostream>
#include <libconfig.h++>

namespace RayTracer {
    /** @class CommonParserException
     * @brief An exception class for handling errors in the CommonParser
     * This class inherits from std::exception and is used to represent errors that occur during the parsing of configuration settings in the CommonParser class. 
     * It contains a message that describes the error, which can be retrieved using the what() method.
     */
    class CommonParserException : public std::exception {
        private:
            std::string _message;
        public:
            /** @brief Construct a CommonParserException with a specific error message
             * @param message The error message describing the exception
             */
            CommonParserException(const std::string &message) : _message(message) {}
            /** @brief Get the error message associated with the exception
             * @return The error message as a C-style string
             */
            const char *what() const noexcept override { return _message.c_str(); }
    };

    /** @class CommonParser
     * @brief A utility class for parsing configuration settings for ray tracing primitives
     * This class provides static methods for verifying the presence of required fields in configuration settings and for parsing common data types such as points, vectors, colors, and numbers. 
     * It is used by various primitive classes to ensure that their configuration settings are valid and to extract the necessary parameters for constructing the primitives.
     */
    class CommonParser {
        public:
            /** @brief Verify that a configuration setting contains a valid point with x, y, and z fields
             * @param setting The configuration setting to verify
             * @param path The path to the point field within the setting
             * @return true if the point is valid, false otherwise
             * This method checks if the specified path exists in the configuration setting and if it contains the required fields for a point (x, y, z). It logs error messages for any missing fields.
             */
            static bool verifPoint(const libconfig::Setting &setting, const std::string &path) {
                if (!setting.exists(path))
                    std::cerr << "CommonParser: missing field '" << path << "' in '" << setting.getPath() << "'" << std::endl;
                else if (!setting.lookup(path).exists("x"))
                    std::cerr << "CommonParser: missing field 'x' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("y"))
                    std::cerr << "CommonParser: missing field 'y' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("z"))
                    std::cerr << "CommonParser: missing field 'z' in '" << setting.getPath() << "." << path << "'" << std::endl;
                if (!setting.exists(path) || !setting.lookup(path).exists("x") || !setting.lookup(path).exists("y") || !setting.lookup(path).exists("z"))
                    return false;
                return true;
            }

            /** @brief Verify that a configuration setting contains a valid vector with x, y, and z fields
             * @param setting The configuration setting to verify
             * @param path The path to the vector field within the setting
             * @return true if the vector is valid, false otherwise
             * This method checks if the specified path exists in the configuration setting and if it contains the required fields for a vector (x, y, z). It logs error messages for any missing fields.
             */
            static bool verifVector(const libconfig::Setting &setting, const std::string &path) {
                if (!setting.exists(path))
                    std::cerr << "CommonParser: missing field '" << path << "' in '" << setting.getPath() << "'" << std::endl;
                else if (!setting.lookup(path).exists("x"))
                    std::cerr << "CommonParser: missing field 'x' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("y"))
                    std::cerr << "CommonParser: missing field 'y' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("z"))
                    std::cerr << "CommonParser: missing field 'z' in '" << setting.getPath() << "." << path << "'" << std::endl;
                if (!setting.exists(path) || !setting.lookup(path).exists("x") || !setting.lookup(path).exists("y") || !setting.lookup(path).exists("z"))
                    return false;
                return true;
            }

            /** @brief Verify that a configuration setting contains a valid color with r, g, and b fields
             * @param setting The configuration setting to verify
             * @param path The path to the color field within the setting
             * @return true if the color is valid, false otherwise
             * This method checks if the specified path exists in the configuration setting and if it contains the required fields for a color (r, g, b). It logs error messages for any missing fields.
             */
            static bool verifColor(const libconfig::Setting &setting, const std::string &path) {
                if (!setting.exists(path))
                    std::cerr << "CommonParser: missing field '" << path << "' in '" << setting.getPath() << "'" << std::endl;
                else if (!setting.lookup(path).exists("r"))
                    std::cerr << "CommonParser: missing field 'r' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("g"))
                    std::cerr << "CommonParser: missing field 'g' in '" << setting.getPath() << "." << path << "'" << std::endl;
                else if (!setting.lookup(path).exists("b"))
                    std::cerr << "CommonParser: missing field 'b' in '" << setting.getPath() << "." << path << "'" << std::endl;
                if (!setting.exists(path) || !setting.lookup(path).exists("r") || !setting.lookup(path).exists("g") || !setting.lookup(path).exists("b"))
                    return false;
                return true;
            }

            /** @brief Verify that a configuration setting contains a valid number
             * @param setting The configuration setting to verify
             * @param path The path to the number field within the setting
             * @return true if the number is valid, false otherwise
             * This method checks if the specified path exists in the configuration setting and if it contains a valid number. It logs error messages for any missing or invalid fields.
             */
            static bool verifNumber(const libconfig::Setting &setting, const std::string &path) {
                if (!setting.exists(path)) {
                    std::cerr << "CommonParser: missing field '" << path << "' in '" << setting.getPath() << "'" << std::endl;
                    return false;
                }
                return true;
            }

            /** @brief Parse a point from a configuration setting
             * @param setting The configuration setting containing the point data
             * @return A Point3D object representing the parsed point
             * This method attempts to parse a point from the specified configuration setting by extracting the x, y, and z fields. If any of the required fields are missing, it throws a CommonParserException with an appropriate error message.
             */
            static Math::Point3D parsePoint(const libconfig::Setting &setting) {
                try {
                    return Math::Point3D(
                        getNumber(setting.lookup("x")),
                        getNumber(setting.lookup("y")),
                        getNumber(setting.lookup("z"))
                    );
                } catch (const libconfig::SettingNotFoundException &e) {
                    throw CommonParserException("CommonParser: missing field in '" + std::string(setting.getPath()) + "'");
                }
            }

            /** @brief Parse a vector from a configuration setting
            * @param setting The configuration setting containing the vector data
            * @return A Vector3D object representing the parsed vector
            * This method attempts to parse a vector from the specified configuration setting by extracting the x, y, and z fields. If any of the required fields are missing, it throws a CommonParserException with an appropriate error message.
            */
            static Math::Vector3D parseVector(const libconfig::Setting &setting) {
                try {
                    return Math::Vector3D(
                        getNumber(setting.lookup("x")),
                        getNumber(setting.lookup("y")),
                        getNumber(setting.lookup("z"))
                    );
                } catch (const libconfig::SettingNotFoundException &e) {
                    throw CommonParserException("CommonParser: missing field in '" + std::string(setting.getPath()) + "'");
                }
            }
            
            /** @brief Parse a color from a configuration setting
             * @param setting The configuration setting containing the color data
             * @return A Vector3D object representing the parsed color (r, g, b)
             * This method attempts to parse a color from the specified configuration setting by extracting the r, g, and b fields. If any of the required fields are missing, it throws a CommonParserException with an appropriate error message.
             */
            static Math::Vector3D parseColor(const libconfig::Setting &setting) {
                try {
                    return Math::Vector3D(
                        getNumber(setting.lookup("r")),
                        getNumber(setting.lookup("g")),
                        getNumber(setting.lookup("b"))
                    );
                } catch (const libconfig::SettingNotFoundException &e) {
                    throw CommonParserException("CommonParser: missing field in '" + std::string(setting.getPath()) + "'");
                }
            }

            /** @brief Get a number from a configuration setting
             * @param setting The configuration setting containing the number
             * @return The parsed number
             */
            static double getNumber(const libconfig::Setting &setting) {
                if (setting.getType() == libconfig::Setting::TypeInt)
                    return static_cast<int>(setting);
                if (setting.getType() == libconfig::Setting::TypeInt64)
                    return static_cast<long long>(setting);
                if (setting.getType() == libconfig::Setting::TypeFloat)
                    return static_cast<double>(setting);
                throw CommonParserException("CommonParser: expected numeric value at '" + std::string(setting.getPath()) + "'");
            }
    };
}

#endif
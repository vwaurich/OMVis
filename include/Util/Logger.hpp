/*
* Copyright (C) 2016, Volker Waurich
*
* This file is part of OMVis.
*
* OMVis is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* OMVis is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with OMVis.  If not, see <http://www.gnu.org/licenses/>.
*/

/** @addtogroup Util
 *  @{
 *  \copyright TU Dresden ZIH. All rights reserved.
 *  \authors Martin Flehmig, Marc Hartung, Marcus Walther
 *  \date Oct 2015
 */

#ifndef INCLUDE_UTIL_LOGGER_HPP_
#define INCLUDE_UTIL_LOGGER_HPP_

#ifdef USE_LOGGER
#define LOGGER_WRITE(message,category,level) Util::Logger::write(message,category,level)
#define LOGGER_WRITE_TUPLE(message,categoryLevel) Util::Logger::write(message,categoryLevel)
#else
#define LOGGER_WRITE(x,y,z)
#define LOGGER_WRITE_TUPLE(x,y)
#endif //USE_LOGGER

#include <string>
#include <tuple>
#include <vector>
#include <iostream>

namespace Util
{
    enum LogCategory
    {
        LC_LOADER = 0,
        LC_CTR = 1,
        LC_VIEWER = 2,
        LC_SOLVER = 3,
        LC_GUI = 4,
        LC_OTHER = 5
    };
    enum LogLevel
    {
        LL_ERROR = 0,
        LL_WARNING = 1,
        LL_INFO = 2,
        LL_DEBUG = 3
    };

    /**
     * A container to store the configuration settings of the logger.
     */
    struct LogSettings
    {
        /**
         * An array that stores the level of the logger for all known categories.
         */
        std::vector<LogLevel> modes;

        /**
         * Create a new object for logger settings. All categories are set to the warning level by default.
         * \todo The number of logger categories is explicitly used in this function. Make it more generic.
         */
        LogSettings()
        {
            modes = std::vector<LogLevel>(6, LL_DEBUG);
        }
        /**
         * Set all categories at once to the given level.
         * @param level The new level for all categories.
         */
        void setAll(LogLevel level)
        {
            for (unsigned i = 0; i < modes.size(); ++i)
                modes[i] = level;
        }

        void print()
        {
            for (unsigned i = 0; i < modes.size(); ++i)
                std::cout << "Category: " << i << " Level: " << modes[i] << std::endl;
        }
    };

    /**
     * This is the base class for all logger instances. It is implemented as singleton and will write all messages
     * to the error stream.
     */
    class Logger
    {
     public:
        /**
         * Destroy the logger object.
         */
        virtual ~Logger();

        /**
         * Get the singleton instance.
         * @return The internal instance of the logger that should be used.
         */
        static Logger& getInstance()
        {
            if (instance == nullptr)
                initialize(LogSettings());

            return *instance;
        }

        /**
         * Initialize a new logger instance configured by the given settings.
         * @param settings
         */
        static void initialize(LogSettings settings)
        {
            if (instance != nullptr)
                delete instance;

            instance = new Logger(settings, true);
        }

        /**
         * Initialize an instance of the default logger and store it as internal instance.
         */
        static void initialize()
        {
            initialize(LogSettings());
        }

        /**
         * This function will write the given message with the category and level information to the output of the
         * logger-instance.
         * @param message The message that should be written.
         * @param category The category of the message.
         * @param level The significance of the given message.
         */
        static inline void write(std::string message, LogCategory category, LogLevel level)
        {
            Logger& instance = getInstance();
            if (instance.isEnabled())
                instance.writeInternal(message, category, level);
        }

        /**
         * This function will write the given message with the category and level information to the output of the
         * logger-instance.
         * @param message The message that should be written.
         * @param mode The category and significance indicator encapsulated as one pair.
         */
        static inline void write(std::string message, std::pair<LogCategory, LogLevel> mode)
        {
            write(message, mode.first, mode.second);
        }

        /**
         * Enable or disable the logger.
         * @param enabled True if the logger should be enabled.
         */
        static void setEnabled(bool enabled)
        {
            getInstance().setEnabledInternal(enabled);
        }

        /**
         * Check if the logger is enabled.
         * @return True if the logger is enabled.
         */
        static bool isEnabled()
        {
            return getInstance().isEnabledInternal();
        }

        /**
         * Check if a message with the given level, belonging the give category will be written through the logger.
         * @param category The category that should be checked.
         * @param level The level that should be checked.
         * @return True if the message would not be filtered and written to the output.
         */
        bool isOutput(LogCategory category, LogLevel level) const;
        /**
         * Check if a message with the given level, belonging the give category will be written through the logger.
         * @param mode The category and level that should be checked.
         * @return True if the message would not be filtered and written to the output.
         */
        bool isOutput(std::pair<LogCategory, LogLevel> mode) const;

     protected:
        /**
         * Create a new logger instance with the given settings.
         * @attention There should be just one logger-instance, so take care about the singleton.
         * @param settings The settings of the logger.
         * @param enabled True if the logger should be enabled.
         */
        Logger(LogSettings settings, bool enabled);
        /**
         * Create a new logger instance with default settings.
         * @attention There should be just one logger-instance, so take care about the singleton.
         * @param enabled True if the logger should be enabled.
         */
        Logger(bool enabled);
        /**
         * Write the message with the given category and level information to the output.
         * @note This function should be overwritten by concrete logger implementation.
         * @param message The message that should be written.
         * @param category The category of the message.
         * @param level The significance of the given message.
         */
        virtual void writeInternal(std::string message, LogCategory category, LogLevel level);
        /**
         * Enable or disable the logger.
         * @param enabled True if the logger should be enabled.
         * @note This function can be overwritten by concrete logger implementations.
         */
        virtual void setEnabledInternal(bool enabled);
        /**
         * Check if the logger is enabled.
         * @return True if the logger is enabled.
         * @note This function can be overwritten by concrete logger implementations.
         */
        virtual bool isEnabledInternal();
        /**
         * Get a prefix text decoration for a message of the given category and level.
         * @param category The category of the message.
         * @param level The level of the message.
         * @return The prefix that contains color information.
         */
        std::string getPrefix(LogCategory category, LogLevel level) const;
        /**
         * Get a suffix text decoration for a message of the given category and level.
         * @param category The category of the message.
         * @param level The level of the message.
         * @return The suffix that contains color and category information.
         */
        std::string getSuffix(LogCategory category, LogLevel level) const;
        /**
         * Convert the given log category to string.
         * @param category The category that should be converted.
         * @return The result string.
         */
        std::string logCategoryToString(LogCategory category) const;
        /**
         * Convert the given log level to string.
         * @param level The level that should be converted.
         * @return The result string.
         */
        std::string logLevelToString(LogLevel level) const;
        /**
         * The static logger singleton instance. This should be set by the initialize method of the concrete logger
         * implementation.
         */
        static Logger* instance;

     private:
        LogSettings _settings;
        std::vector<std::tuple<std::string, std::string> > _textDecorator;
        bool _isEnabled;
    };

} /* namespace Util */

#endif /* INCLUDE_UTIL_LOGGER_HPP_ */
/**
 * @}
 */

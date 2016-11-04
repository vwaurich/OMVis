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

#include "Util/Logger.hpp"

#include <iostream>
#include <utility>

namespace OMVIS
{
    namespace Util
    {

        Logger* Logger::instance = nullptr;

        Logger::Logger(LogSettings settings, bool enabled)
                : _settings(std::move(settings)),
                  _textDecorator(4, std::tuple<std::string, std::string>("", "")),
                  _isEnabled(enabled)
        {
            _textDecorator[LL_ERROR] = std::tuple<std::string, std::string>("\e[31m\e[1m", "\e[0m");
            _textDecorator[LL_WARNING] = std::tuple<std::string, std::string>("\e[33m", "\e[0m");
            _textDecorator[LL_INFO] = std::tuple<std::string, std::string>("\e[39m", "\e[0m");
            _textDecorator[LL_DEBUG] = std::tuple<std::string, std::string>("\e[90m", "\e[0m");
        }

        Logger::Logger(bool enabled)
                : _settings(LogSettings()),
                  _isEnabled(enabled)
        {
        }

        void Logger::writeInternal(const std::string& message, LogCategory category, LogLevel level)
        {
            if (isOutput(category, level))
            {
                std::cerr << getPrefix(category, level) << "  " << message << getSuffix(level) << std::endl;
            }
        }

        void Logger::setEnabledInternal(bool enabled)
        {
            _isEnabled = enabled;
        }

        bool Logger::isEnabledInternal()
        {
            return _isEnabled;
        }

        bool Logger::isOutput(LogCategory category, LogLevel level) const
        {
            return _settings.modes[category] >= level && _isEnabled;
        }

        bool Logger::isOutput(std::pair<LogCategory, LogLevel> mode) const
        {
            return isOutput(mode.first, mode.second);
        }

        std::string Logger::getPrefix(LogCategory category, LogLevel level) const
        {
            std::string prefix = std::get<0>(_textDecorator[level]);
            prefix += std::string("[") + logCategoryToString(category) + std::string("]");
            return prefix;
        }

        std::string Logger::getSuffix(LogLevel level) const
        {
            std::string suffix = std::get<1>(_textDecorator[level]);
            return suffix;
        }

        std::string Logger::logCategoryToString(LogCategory category) const
        {
            switch (category)
            {
                case LC_LOADER:
                    return "LOADER";
                case LC_CTR:
                    return "CTR";
                case LC_VIEWER:
                    return "VIEWER";
                case LC_SOLVER:
                    return "SOLVER";
                case LC_GUI:
                    return "GUI";
                case LC_OTHER:
                    return "OTHER";
                default:
                    return "";
            }
        }

        std::string Logger::logLevelToString(LogLevel level) const
        {
            switch (level)
            {
                case (LL_DEBUG):
                    return "DEBUG";
                case (LL_ERROR):
                    return "ERROR";
                case (LL_INFO):
                    return "INFO";
                case (LL_WARNING):
                    return "WARNING";
                default:
                    return "";
            }
        }

    }  // namespace Util
}  // namespace OMVIS

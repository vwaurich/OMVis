/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OMVIS.hpp"

#include <stdexcept>
#include <iostream>
#include <clocale>

using namespace OMVIS;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");
    setlocale(LC_NUMERIC, "en_US.UTF-8");

    try
    {
        // Parse command line arguments for logger specific settings.
        Initialization::CommandLineArgs clArgs = Initialization::getCommandLineArguments(argc, argv);

        // Initialize logger with logger settings from command line.
        clArgs.print();
        Util::Logger::initialize(clArgs.logSet);
        Util::Logger logger = Util::Logger::getInstance();

        LOGGER_WRITE("Okay, let's create the main widget...", Util::LC_OTHER, Util::LL_INFO);
        QApplication app(argc, argv);

        View::OMVISViewer omvisViewer(Q_NULLPTR, clArgs);
        omvisViewer.show();

        app.exec();
    }
    catch (std::exception &ex)
    {
        LOGGER_WRITE("Execution failed. Error: " + std::string(ex.what()), Util::LC_OTHER, Util::LL_ERROR);
        return -1;
    }

    return 0;
}

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

/**
 *
 *
 * Purpose: Calculates the total of 6 checks
 *
 * @author Volker Waurich
 * @version 0.1
 *
 *
 * General todos:
 * \todo Pass arguments to functions by reference
 * \todo Const correctness
 * \todo Attributes private if possible
 * \todo Use forward declaration where possible
 * \todo Remove unused includes.
 */

/**
 *
 *
 * \todo What can be reused of the Parallel FMU program? Or, what classes and functions can
 * Parallel FMU Program and OMVIS share in the sense of unique file location?
 *
 */

#include <stdexcept>
#include <iostream>
#include "OMVIS.hpp"

int main(int argc, char* argv[])
{
    try
    {
        // Parse command line arguments for logger specific settings.
        Util::CommandLineArgs clArgs = Util::getCommandLineArguments(argc, argv);

        // Initialize logger with logger settings from command line.
        Util::Logger::initialize(clArgs._logSettings);
        Util::Logger logger = Util::Logger::getInstance();

        LOGGER_WRITE(std::string("Okay, let's create the main widget..."), Util::LC_OTHER, Util::LL_INFO);
        QApplication app(argc, argv);

        OMVisViewer omvisViewer;
        omvisViewer.show();

        app.exec();
    }
    catch (std::exception &ex)
    {
        LOGGER_WRITE(std::string("Execution failed. Error: ") + std::string(ex.what()), Util::LC_OTHER, Util::LL_ERROR);
        return -1;
    }

    return 0;
}

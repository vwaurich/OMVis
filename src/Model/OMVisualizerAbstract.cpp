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

#include "Model/OMVisualizerAbstract.hpp"
#include "Util/Logger.hpp"

#include <boost/filesystem.hpp>

#include <string>
#include <stdlib.h>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OMVisualizerAbstract::OMVisualizerAbstract()
                : _baseData(nullptr),
                  _viewerStuff(nullptr),
                  _nodeUpdater(nullptr),
                  _timeManager(nullptr)
        {
        }

        OMVisualizerAbstract::OMVisualizerAbstract(const std::string& modelFile, const std::string& path)
                : //Siehe unten _baseData(new OMVisualBase(modelFile, path)),
                  _viewerStuff(new OMVisScene),
                  _nodeUpdater(new Model::UpdateVisitor),
                  _timeManager(new Control::TimeManager(0.0, 0.0, -1.0, 0.0, 0.1, 0.0, 100.0))
        {
            // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
            //char fullPathTmp[PATH_MAX];
            //realpath(path.c_str(), fullPathTmp);
            boost::filesystem::path bPath = boost::filesystem::path(path.c_str());
            bPath = boost::filesystem::absolute(bPath);
            std::string fullPath = bPath.string();

            // Now we can use the full path.
            _baseData = std::shared_ptr<OMVisualBase>(new OMVisualBase(modelFile, fullPath));
            _viewerStuff->getScene().setPath(fullPath);
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        int OMVisualizerAbstract::initData()
        {
            int isOk(0);
            // In case of reloading, we need to make sure, that we have empty members.
            _baseData->clearXMLDoc();

            // Initialize XML file and get visAttributes.
            isOk = _baseData->initXMLDoc();

            isOk = _baseData->initVisObjects();
            return isOk;
        }

        int OMVisualizerAbstract::setUpScene()
        {
            //build scene graph
            std::cout << "SETUPSCENE FOR " << _baseData->_shapes.size() << std::endl;

            int isOk = _viewerStuff->getScene().setUpScene(_baseData->_shapes);
            return isOk;
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        std::string OMVisualizerAbstract::getType() const
        {
            return "abstract";
        }

        std::shared_ptr<OMVisualBase> OMVisualizerAbstract::getBaseData() const
        {
            return _baseData;
        }

        std::shared_ptr<Control::TimeManager> OMVisualizerAbstract::getTimeManager() const
        {
            return _timeManager;
        }

        std::shared_ptr<OMVisScene> OMVisualizerAbstract::getOMVisScene() const
        {
            return _viewerStuff;
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void OMVisualizerAbstract::startVisualization()
        {
            if (_timeManager->getVisTime() < _timeManager->getEndTime() - 1.e-6)
            {
                _timeManager->setPause(false);
                LOGGER_WRITE(std::string("Start visualization ..."), Util::LC_CTR, Util::LL_INFO);
            }
            else
                LOGGER_WRITE(std::string("There is nothing left to visualize. Initialize the model first."), Util::LC_CTR, Util::LL_INFO);
        }

        void OMVisualizerAbstract::pauseVisualization()
        {
            LOGGER_WRITE(std::string("Pause visualization at ") + std::to_string(_timeManager->getVisTime()) + std::string("."), Util::LC_CTR, Util::LL_INFO);
            _timeManager->setPause(true);
        }

        void OMVisualizerAbstract::initVisualization()
        {
            LOGGER_WRITE(std::string("Initialize visualization."), Util::LC_CTR, Util::LL_INFO);
            initializeVisAttributes(_timeManager->getStartTime());
            _timeManager->setVisTime(_timeManager->getStartTime());
            _timeManager->setPause(true);
        }

        void OMVisualizerAbstract::donationVisualization()
        {
            LOGGER_WRITE(std::string("Want to support Martin and Volker? Buy us a coffee."), Util::LC_CTR, Util::LL_INFO);
        }

        void OMVisualizerAbstract::sceneUpdate()
        {
            _timeManager->updateTick();

            if (!_timeManager->isPaused())
            {
                updateScene(_timeManager->getVisTime());
                _timeManager->setVisTime(_timeManager->getVisTime() + _timeManager->getHVisual());

                LOGGER_WRITE(std::string("Update scene at ") + std::to_string(_timeManager->getVisTime()) + std::string(" simTime ") + std::to_string(_timeManager->getSimTime()) + std::string(" _visStepSize ") + std::to_string(_timeManager->getHVisual()), Util::LC_CTR, Util::LL_INFO);
                if (_timeManager->getVisTime() >= _timeManager->getEndTime() - 1.e-6)
                {
                    LOGGER_WRITE(std::string("The End."), Util::LC_CTR, Util::LL_INFO);
                    _timeManager->setPause(true);
                }
            }
        }

    }  // End namespace Model
}  // End namespace OMVIS

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

#include "Model/VisualizerAbstract.hpp"
#include "Util/Logger.hpp"

#include <boost/filesystem.hpp>

#include <stdlib.h>
#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        VisualizerAbstract::VisualizerAbstract()
                : _visType(VisType::NONE),
                  _baseData(nullptr),
                  _viewerStuff(nullptr),
                  _nodeUpdater(nullptr),
                  _timeManager(nullptr)
        {
        }

        VisualizerAbstract::VisualizerAbstract(const std::string& modelFile, const std::string& path,
                                               const VisType visType)
                : _visType(visType),
                  _baseData(nullptr),
                  _viewerStuff(std::make_shared<OMVisScene>()),
                  _nodeUpdater(std::make_shared<Model::UpdateVisitor>()),
                  _timeManager(std::make_shared<Control::TimeManager>(0.0, 0.0, 0.0, 0.0, 0.1, 0.0, 100.0))
        {
            // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
            //char fullPathTmp[PATH_MAX];
            //realpath(path.c_str(), fullPathTmp);
            auto bPath = boost::filesystem::path(path.c_str());
            bPath = boost::filesystem::absolute(bPath);
            std::string fullPath = bPath.string();

            // Now we can use the full path.
            _baseData = std::make_shared<OMVisualBase>(modelFile, fullPath);
            _viewerStuff->getScene()->setPath(fullPath);
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/
        void VisualizerAbstract::initialize()
        {
            initData();
            setUpScene();
            updateVisAttributes(0.0);
        }

        void VisualizerAbstract::initData()
        {
            // In case of reloading, we need to make sure, that we have empty members.
            _baseData->clearXMLDoc();

            // Initialize XML file and get visAttributes.
            _baseData->initXMLDoc();

            _baseData->initVisObjects();
        }

        void VisualizerAbstract::setUpScene()
        {
            // Build scene graph.
            LOGGER_WRITE(std::string("Setup scene for ") + std::to_string(_baseData->_shapes.size()) + " shapes.",
                         Util::LC_LOADER, Util::LL_DEBUG);
            _viewerStuff->getScene()->setUpScene(_baseData->_shapes);
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        VisType VisualizerAbstract::getVisType() const
        {
            return _visType;
        }

        std::shared_ptr<OMVisualBase> VisualizerAbstract::getBaseData() const
        {
            return _baseData;
        }

        std::shared_ptr<Control::TimeManager> VisualizerAbstract::getTimeManager() const
        {
            return _timeManager;
        }

        std::shared_ptr<OMVisScene> VisualizerAbstract::getOMVisScene() const
        {
            return _viewerStuff;
        }

        std::string VisualizerAbstract::getModelFile() const
        {
            return _baseData->getModelFile();
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void VisualizerAbstract::startVisualization()
        {
            if (_timeManager->getVisTime() < _timeManager->getEndTime() - 1.e-6)
            {
                _timeManager->setPause(false);
                LOGGER_WRITE(std::string("Start visualization ..."), Util::LC_CTR, Util::LL_INFO);
            }
            else
            {
                LOGGER_WRITE(std::string("There is nothing left to visualize. Initialize the model first."),
                             Util::LC_CTR, Util::LL_INFO);
            }
        }

        void VisualizerAbstract::pauseVisualization()
        {
            LOGGER_WRITE(
                    std::string("Pause visualization at ") + std::to_string(_timeManager->getVisTime())
                            + std::string("."),
                    Util::LC_CTR, Util::LL_INFO);
            _timeManager->setPause(true);
        }

        void VisualizerAbstract::initVisualization()
        {
            LOGGER_WRITE(std::string("Initialize visualization."), Util::LC_CTR, Util::LL_INFO);
            initializeVisAttributes(_timeManager->getStartTime());
            _timeManager->setVisTime(_timeManager->getStartTime());
            _timeManager->setRealTimeFactor(0.0);
            _timeManager->setPause(true);
        }

        void VisualizerAbstract::sceneUpdate()
        {
            _timeManager->updateTick();

            if (!_timeManager->isPaused())
            {
                updateScene(_timeManager->getVisTime());
                _timeManager->setVisTime(_timeManager->getVisTime() + _timeManager->getHVisual());

                LOGGER_WRITE(
                        std::string("Update scene at ") + std::to_string(_timeManager->getVisTime())
                                + std::string(" simTime ") + std::to_string(_timeManager->getSimTime())
                                + std::string(" _visStepSize ") + std::to_string(_timeManager->getHVisual()),
                        Util::LC_CTR, Util::LL_INFO);
                if (_timeManager->getVisTime() >= _timeManager->getEndTime() - 1.e-6)
                {
                    LOGGER_WRITE(std::string("The End."), Util::LC_CTR, Util::LL_INFO);
                    _timeManager->setPause(true);
                }
            }
        }

    }  // namespace Model
}  // namespace OMVIS

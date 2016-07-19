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

#include <Model/VisualizerFMUClient.hpp>
#include "Util/Logger.hpp"

namespace OMVIS
{
    namespace Model
    {

        VisualizerFMUClient::VisualizerFMUClient(const Initialization::RemoteVisualizationConstructionPlan* cP)
                : VisualizerAbstract(cP->modelFile, cP->workingDirectory),
                  _noFC(cP->ipAddress, cP->portNumber),
                  _outputVars(),
                  _simSettings(new SimSettings()),
                  _inputData(new InputData()),
                  _joysticks(),
                  _remotePathToModelFile(cP->path)
        {
            LOGGER_WRITE(std::string("Initialize joysticks"), Util::LC_LOADER, Util::LL_INFO);
            initJoySticks();
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/
        void VisualizerFMUClient::initializeConnectionToServer()
        {
            // test if server can be reached
            if (!_noFC.initializeConnection())
                throw std::runtime_error("Couldn't reach server.");
        }

        /// \todo: Set the error variable isOk.
        int VisualizerFMUClient::loadFMU()
        {
            int isOk(0);
            auto a = _baseData->getModelName();
            auto p = _baseData->getPath();

            // Send server data about the simulation you want to calculate and get the ID of the simulation.
            //_simID = _noFC.addSimulation(_baseData->getModelName());
            auto c = _remotePathToModelFile + _baseData->getModelName();
            _simID = _noFC.addSimulation(a);

            // Set visualization relevant output variables.
            NetOff::VariableList outputVars = getOutputVariables();

            // Set input variables. Todo: Open GUI window.
            NetOff::VariableList inputVars = getInputVariables();
            _inputData->initializeInputs(inputVars);
            _inputData->printValues();

            _outputVars = getOutputVariables();

            // Communicate input and output variables with server.
            _noFC.initializeSimulation(_simID, inputVars, outputVars, nullptr, nullptr, nullptr);

            return isOk;
        }

        int VisualizerFMUClient::initData()
        {
            int isOk(0);
            isOk = VisualizerAbstract::initData();
            isOk += loadFMU();
            _simSettings->setTend(_timeManager->getEndTime());
            _simSettings->setHdef(0.001);
            setVarReferencesInVisAttributes();

            //OMVisualizerFMU::initializeVisAttributes(_omvManager->getStartTime());
            return isOk;
        }

        void VisualizerFMUClient::resetInputs()
        {
            _inputData->resetInputValues();
        }

        void VisualizerFMUClient::initJoySticks()
        {
            //Initialize SDL
            if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
                LOGGER_WRITE(std::string("SDL could not be initialized."), Util::LC_LOADER, Util::LL_ERROR);

            //Check for joysticks
            _numJoysticks = SDL_NumJoysticks();
            if (SDL_NumJoysticks() < 1)
                LOGGER_WRITE(std::string("No joysticks connected!"), Util::LC_LOADER, Util::LL_WARNING);
            else
            {
                LOGGER_WRITE(std::string("Found ") + std::to_string(SDL_NumJoysticks()) + std::string(" joystick(s)"), Util::LC_LOADER, Util::LL_INFO);
                //Load joystick
                std::cout << "START LOADING JOYSTICKS!!!!!!!!!" << _numJoysticks << std::endl;

                Control::JoystickDevice* newJoyStick;
                for (size_t i = 0; i < _numJoysticks; ++i)
                {
                    std::cout << "LOAD JOYSTICKS!!!!!!!!!" << i << std::endl;

                    newJoyStick = new Control::JoystickDevice(i);
                    _joysticks.push_back(newJoyStick);

                    if (newJoyStick == nullptr)
                        LOGGER_WRITE(std::string("Unable to open joystick! SDL Error: ") + SDL_GetError(), Util::LC_LOADER, Util::LL_INFO);
                }
            }
        }

        int VisualizerFMUClient::initialize()
          {
              // If visual XML file is not present, we need to copy it from server to localhost
              // (before calling OMVisualizerAbstract::initialize().

              int isOk(0);
              try
              {
                  initializeConnectionToServer();
              }
              catch (std::exception& ex)
              {
                  LOGGER_WRITE(std::string("VisualizerFMUClient exception: ") + ex.what(), Util::LC_LOADER, Util::LL_ERROR);
                  isOk += 1;
              }

              VisualizerAbstract::initialize();

              //initializeSimulation();

              return isOk;
          }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        NetOff::VariableList VisualizerFMUClient::getOutputVariables()
        {
            NetOff::VariableList varList;
            varList.addReals(_baseData->getVisualizationVariables());
            return varList;
        }

        // Todo! Implement me. Use a GUI to set input values.
        NetOff::VariableList VisualizerFMUClient::getInputVariables()
        {
            NetOff::VariableList varList;
            varList.addReals(_noFC.getPossibleInputVariableNames(_simID).getReals());
            return varList;
        }

        std::string VisualizerFMUClient::getType() const
        {
            return "fmuclient";
        }

        std::shared_ptr<InputData> VisualizerFMUClient::getInputData()
        {
            return _inputData;
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void VisualizerFMUClient::simulate(Control::TimeManager& omvm)
        {
            while (omvm.getSimTime() < omvm.getRealTime() + omvm.getHVisual() && omvm.getSimTime() < omvm.getEndTime())
            {
                double newSimTime = simulateStep(omvm.getSimTime());
                omvm.setSimTime(newSimTime);
            }
        }

        double VisualizerFMUClient::simulateStep(const double time)
        {
            double newTime = time + _simSettings->getHdef();

            NetOff::ValueContainer & inputCont = _noFC.getInputValueContainer(_simID);
            // Set inputs in inputData
            for (auto& joystick : _joysticks)
            {
                joystick->detectContinuousInputEvents(_inputData);

                //_inputData->setInputsInFMU(_fmu->getFMU()); //todo aus der schleife raus???
                //std::cout << "JOY" << i << " XDir " <<_joysticks[i]->getXDir() <<" YDir "<< _joysticks[i]->getYDir() << std::endl;
            }

            // Set inputs for network communication.
            inputCont.setRealValues(_inputData->getRealValues());
            inputCont.setIntValues(_inputData->getIntValues());
            inputCont.setBoolValues(_inputData->getBoolValues());

            // Send input values to server
            _noFC.sendInputValues(_simID, newTime, inputCont);
            // Receive output values from server for visualisation
            _noFC.recvOutputValues(_simID, newTime);  // implicit check if its really [time]
            return newTime;
        }

        void VisualizerFMUClient::initializeVisAttributes(const double time)
        {
            _timeManager->setVisTime(_timeManager->getStartTime());
            _timeManager->setSimTime(_timeManager->getStartTime());
            setVarReferencesInVisAttributes();
            updateVisAttributes(_timeManager->getVisTime());
        }

        fmi1_value_reference_t VisualizerFMUClient::getVarReferencesForObjectAttribute(ShapeObjectAttribute* attr)
        {
            fmi1_value_reference_t vr = 0;
            if (!attr->isConst)
                vr = _outputVars.findRealVariableNameIndex(attr->cref);
            return vr;
        }

        int VisualizerFMUClient::setVarReferencesInVisAttributes()
        {
            int isOk(0);

            try
            {
                for (auto& shape : _baseData->_shapes)
                {
                    shape._length.fmuValueRef = getVarReferencesForObjectAttribute(&shape._length);
                    shape._width.fmuValueRef = getVarReferencesForObjectAttribute(&shape._width);
                    shape._height.fmuValueRef = getVarReferencesForObjectAttribute(&shape._height);

                    shape._lDir[0].fmuValueRef = getVarReferencesForObjectAttribute(&shape._lDir[0]);
                    shape._lDir[1].fmuValueRef = getVarReferencesForObjectAttribute(&shape._lDir[1]);
                    shape._lDir[2].fmuValueRef = getVarReferencesForObjectAttribute(&shape._lDir[2]);

                    shape._wDir[0].fmuValueRef = getVarReferencesForObjectAttribute(&shape._wDir[0]);
                    shape._wDir[1].fmuValueRef = getVarReferencesForObjectAttribute(&shape._wDir[1]);
                    shape._wDir[2].fmuValueRef = getVarReferencesForObjectAttribute(&shape._wDir[2]);

                    shape._r[0].fmuValueRef = getVarReferencesForObjectAttribute(&shape._r[0]);
                    shape._r[1].fmuValueRef = getVarReferencesForObjectAttribute(&shape._r[1]);
                    shape._r[2].fmuValueRef = getVarReferencesForObjectAttribute(&shape._r[2]);

                    shape._rShape[0].fmuValueRef = getVarReferencesForObjectAttribute(&shape._rShape[0]);
                    shape._rShape[1].fmuValueRef = getVarReferencesForObjectAttribute(&shape._rShape[1]);
                    shape._rShape[2].fmuValueRef = getVarReferencesForObjectAttribute(&shape._rShape[2]);

                    shape._T[0].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[0]);
                    shape._T[1].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[1]);
                    shape._T[2].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[2]);
                    shape._T[3].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[3]);
                    shape._T[4].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[4]);
                    shape._T[5].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[5]);
                    shape._T[6].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[6]);
                    shape._T[7].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[7]);
                    shape._T[8].fmuValueRef = getVarReferencesForObjectAttribute(&shape._T[8]);
                }  //end for
            }  // end try

            catch (std::exception& e)
            {
                LOGGER_WRITE(std::string("Something went wrong in Visualizer::setVarReferencesInVisAttributes"), Util::LC_SOLVER, Util::LL_WARNING);
                isOk = 1;
            }
            return isOk;
        }

        int VisualizerFMUClient::updateVisAttributes(const double time)
        {
            int isOk(0);

            // Update all shapes
            OMVIS::Util::rAndT rT;
            NetOff::ValueContainer & outputCont = _noFC.getOutputValueContainer(_simID);
            osg::ref_ptr<osg::Node> child = nullptr;
            try
            {
                size_t i =0;
                for (auto& shape : _baseData->_shapes)
                {
                    // get the values for the scene graph objects
                    Util::updateObjectAttributeFMUClient(shape._length, outputCont);
                    Util::updateObjectAttributeFMUClient(shape._width, outputCont);
                    Util::updateObjectAttributeFMUClient(shape._height, outputCont);

                    Util::updateObjectAttributeFMUClient(shape._lDir[0], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._lDir[1], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._lDir[2], outputCont);

                    Util::updateObjectAttributeFMUClient(shape._wDir[0], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._wDir[1], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._wDir[2], outputCont);

                    Util::updateObjectAttributeFMUClient(shape._r[0], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._r[1], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._r[2], outputCont);

                    Util::updateObjectAttributeFMUClient(shape._rShape[0], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._rShape[1], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._rShape[2], outputCont);

                    Util::updateObjectAttributeFMUClient(shape._T[0], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[1], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[2], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[3], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[4], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[5], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[6], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[7], outputCont);
                    Util::updateObjectAttributeFMUClient(shape._T[8], outputCont);
                    rT = Util::rotation(osg::Vec3f(shape._r[0].exp, shape._r[1].exp, shape._r[2].exp), osg::Vec3f(shape._rShape[0].exp, shape._rShape[1].exp, shape._rShape[2].exp), osg::Matrix3(shape._T[0].exp, shape._T[1].exp, shape._T[2].exp, shape._T[3].exp, shape._T[4].exp, shape._T[5].exp, shape._T[6].exp, shape._T[7].exp, shape._T[8].exp),
                                        osg::Vec3f(shape._lDir[0].exp, shape._lDir[1].exp, shape._lDir[2].exp), osg::Vec3f(shape._wDir[0].exp, shape._wDir[1].exp, shape._wDir[2].exp), shape._length.exp, shape._width.exp, shape._height.exp, shape._type);

                    Util::assemblePokeMatrix(shape._mat, rT._T, rT._r);

                    //update the shapes
                    _nodeUpdater->_shape = shape;

                    //get the scene graph nodes and stuff
                    //_viewerStuff->dumpOSGTreeDebug();
                    child = _viewerStuff->getScene().getRootNode()->getChild(i);  // the transformation
                    child->accept(*_nodeUpdater);
                    ++i;
                }  //end for
            }  // end try

            catch (std::exception& e)
            {
                LOGGER_WRITE(std::string("Something went wrong in Visualizer::updateVisAttributes at time point ") + std::to_string(time) + std::string(" ."), Util::LC_SOLVER, Util::LL_WARNING);
                isOk = 1;
            }
            return isOk;
        }

        void VisualizerFMUClient::startVisualization()
        {
            if (!_noFC.isStarted())
                _noFC.start();
            else
                _noFC.unpause();
            VisualizerAbstract::startVisualization();
        }

        void VisualizerFMUClient::pauseVisualization()
        {
            _noFC.pause();
            VisualizerAbstract::pauseVisualization();
        }

        void VisualizerFMUClient::updateScene(const double time)
        {
            _timeManager->updateTick();            //for real-time measurement

            _timeManager->setSimTime(_timeManager->getVisTime());
            double nextStep = _timeManager->getVisTime() + _timeManager->getHVisual();

            double vis1 = _timeManager->getRealTime();
            while (_timeManager->getSimTime() < nextStep)
            {
                //std::cout<<"simulate "<<omvManager->_simTime<<" to "<<nextStep<<std::endl;
                //_inputData.printValues();
                _timeManager->setSimTime(simulateStep(_timeManager->getSimTime()));
            }
            _timeManager->updateTick();                     //for real-time measurement
            _timeManager->setRealTimeFactor(_timeManager->getHVisual() / (_timeManager->getRealTime() - vis1));
            updateVisAttributes(_timeManager->getVisTime());
        }

    }  // End namespace Model
}  // End namespace OMVIS

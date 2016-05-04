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
#include "Util/Util.hpp"
#include "Util/ObjectAttribute.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "View/OMVisScene.hpp"
#include "Control/OMVisManager.hpp"
#include "Control/JoystickDevice.hpp"

#include <SDL.h>

#include <iostream>

namespace OMVIS
{
    namespace Model
    {

        OMVisualizerFMU::OMVisualizerFMU(const std::string& fileName, const std::string& dirPath)
                : OMVisualizerAbstract(fileName, dirPath),
                  _fmu(new FMU()),
                  _simSettings(new SimSettings()),
                  _inputData(),
                  _joysticks()
        {
            LOGGER_WRITE(std::string("Initialize joysticks"), Util::LC_LOADER, Util::LL_INFO);
            initJoySticks();
        }

        void OMVisualizerFMU::initJoySticks()
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

                for (size_t i = 0; i < _numJoysticks; ++i)
                {
                    std::cout << "LOAD JOYSTICKS!!!!!!!!!" << i << std::endl;

                    Control::JoystickDevice* newJoyStick = new Control::JoystickDevice(i);
                    _joysticks.push_back(newJoyStick);

                    if (newJoyStick == nullptr)
                        LOGGER_WRITE(std::string("Unable to open joystick! SDL Error: ") + SDL_GetError(), Util::LC_LOADER, Util::LL_INFO);
                }
            }
        }

        int OMVisualizerFMU::initData()
        {
            int isOk(0);
            isOk = OMVisualizerAbstract::initData();
            isOk += loadFMU(_baseData->getModelName(), _baseData->getDirName());
            _simSettings->setTend(_omvManager->getEndTime());
            _simSettings->setHdef(0.001);

            return isOk;
        }

        /// \todo: Set the error variable isOk.
        int OMVisualizerFMU::loadFMU(const std::string& modelFile, const std::string& dirPath)
        {
            int isOk(0);
            //setup fmu-simulation stuff
            //_simSettings = new SimSettings;
            //std::string fmuFileName = dirPath + modelFile + ".fmu";

            //load and initialize fmu
            _fmu->load(modelFile, dirPath);
            LOGGER_WRITE(std::string("OMVisualizerFMU::loadFMU: FMU was successfully loaded."), Util::LC_LOADER, Util::LL_DEBUG);

            _fmu->initialize(_simSettings);
            LOGGER_WRITE(std::string("OMVisualizerFMU::loadFMU: FMU was successfully initialized."), Util::LC_LOADER, Util::LL_DEBUG);

            _inputData.initializeInputs(_fmu->getFMU());
            _inputData.printValues();
            //assign interactive inputs
            //for (unsigned int i = 0; i < inputs.n_inputs; i++){
            //string key = "";
            //std::cout<<"assign input "<<i<<" :"<<std::endl;
            //getline(cin,key);
            //int keyInt = getchar();
            //std::cout<<"the key is "<<keyInt<<" !"<<std::endl;
            //}
            return isOk;
        }

        void OMVisualizerFMU::simulate(Control::OMVisManager& omvm)
        {
            while (omvm.getSimTime() < omvm.getRealTime() + omvm.getHVisual() && omvm.getSimTime() < omvm.getEndTime())
            {
                //<<"simulate "<<omvManager.simTime<<endl;
                //omv.fmuData.inputs.printValues();
                omvm.setSimTime(simulateStep(omvm.getSimTime()));
            }
        }

        double OMVisualizerFMU::simulateStep(const double time)
        {
            //tcur = settings.tstart;
            //hcur = settings.hdef;
            int zero_crossning_event = 0;
//X9            _fmu->_fmuData._fmiStatus = fmi1_import_set_time(_fmu->_fmu, time);
//X9            _fmu->_fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu->_fmu, _fmu->_fmuData->_eventIndicators, _fmu->_fmuData._nEventIndicators);
            _fmu->prepareSimulationStep(time);

            /* Check if an event indicator has triggered */
//X9            for (size_t k = 0; k < _fmu->_fmuData._nEventIndicators; ++k)
//X9            {
//X9                if (_fmu->_fmuData._eventIndicators[k] * _fmu->_fmuData._eventIndicatorsPrev[k] < 0)
//                {
//                    zero_crossning_event = 1;
//                    LOGGER_WRITE(std::string("OMVisualizerFMU::loadFMU: Event occurred at ") + std::to_string(_fmu->_fmuData._tcur), Util::LC_CTR, Util::LL_DEBUG);
//                    //std::cout << "AN EVENT at " << _fmuData._tcur << std::endl;
//                    break;
//X9                }
//X9            }
            bool zeroCrossingEvent = _fmu->checkForTriggeredEvent();

            /* Handle any events */
//X9            if (_simSettings->getCallEventUpdate() || zero_crossning_event || (_fmu->_fmuData._eventInfo.upcomingTimeEvent && _fmu->_fmuData._tcur == _fmu->_fmuData._eventInfo.nextEventTime))
//X9            {
//                std::cout << "HANDLE EVENT at " << _fmu->_fmuData._tcur << std::endl;
//                _fmu->_fmuData._fmiStatus = fmi1_import_eventUpdate(_fmu->getFmu(), _simSettings->getIntermediateResults(), &_fmu->_fmuData._eventInfo);
//                _fmu->_fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu->_fmu, _fmu->_fmuData._states, _fmu->_fmuData._nStates);
//                _fmu->_fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu->_fmu, _fmu->_fmuData._eventIndicators, _fmu->_fmuData._nEventIndicators);
//                _fmu->_fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu->_fmu, _fmu->_fmuData._eventIndicatorsPrev, _fmu->_fmuData._nEventIndicators);
//X9            }
            if (_simSettings->getCallEventUpdate() || zeroCrossingEvent || _fmu->itsEventTime())
            {
                _fmu->handleEvents(_simSettings->getIntermediateResults());
            }

            /* Updated next time step */
//X9            if (_fmu->_fmuData._eventInfo.upcomingTimeEvent)
//X9            {
//                if (_fmu->_fmuData._tcur + _simSettings->getHdef() < _fmu->_fmuData._eventInfo.nextEventTime)
//                    _fmu->_fmuData._hcur = _simSettings->getHdef();
//                else
//                    _fmu->_fmuData._hcur = _fmu->_fmuData._eventInfo.nextEventTime - _fmu->_fmuData._tcur;
//            }
//            else
//X9                _fmu->_fmuData._hcur = _simSettings->getHdef();
//X9            /* increase with step size*/
//X9            _fmu->_fmuData._tcur += _fmu->_fmuData._hcur;
            _fmu->updateNextTimeStep(_simSettings->getHdef());

            /* last step */
//X9            if (_fmu->_fmuData._tcur > _simSettings->getTend() - _fmu->_fmuData._hcur / 1e16)
//X9            {
//                _fmu->_fmuData._tcur -= _fmu->_fmuData._hcur;
//                _fmu->_fmuData._hcur = _simSettings->getTend() - _fmu->_fmuData._tcur;
//                _fmu->_fmuData._tcur = _simSettings->getTend();
//X9            }
            _fmu->updateTimes(_simSettings->getTend());

            //set inputs
            for (size_t i = 0; i < _numJoysticks; ++i)
            {
                _joysticks[i]->detectContinuousInputEvents(_inputData);
                _inputData.setInputsInFMU(_fmu->getFMU());
                //std::cout << "JOY" << i << " XDir " <<_joysticks[i]->getXDir() <<" YDir "<< _joysticks[i]->getYDir() << std::endl;
            }

            //_inputData.printValues();
            //X2 MF: On my system, this line is needed in order to get the keyboard input working
            _inputData.setInputsInFMU(_fmu->getFMU());

            /* Solve system */
//X9            _fmu->_fmuData._fmiStatus = fmi1_import_get_derivatives(_fmu->getFmu(), _fmu->_fmuData._statesDer, _fmu->_fmuData._nStates);
            _fmu->solveSystem();

            //print out some values for debugging:
            //std::cout<<"DO EULER at "<< _fmu->_fmuData._tcur<<std::endl;
            //fmi1_import_variable_t* var = fmi1_import_get_variable_by_name(_fmul._fmu, "prismatic.s");
            //const fmi1_value_reference_t vr  = fmi1_import_get_variable_vr(var);
            //double value = -1.0;
            //fmi1_import_get_real(_fmul._fmu, &vr, 1, &value);
            //std::cout<<"value "<<value<<std::endl;

            // integrate a step with euler
//X9            for (size_t k = 0; k < _fmu->_fmuData._nStates; ++k)
//X9                _fmu->_fmuData._states[k] = _fmu->_fmuData._states[k] + _fmu->_fmuData._hcur * _fmu->_fmuData._statesDer[k];
            _fmu->doEulerStep();

            /* Set states */
//X9            _fmu->_fmuData._fmiStatus = fmi1_import_set_continuous_states(_fmu->_fmu, _fmu->_fmuData._states, _fmu->_fmuData._nStates);
            _fmu->setContinuousStates();

            /* Step is complete */
//X9            _fmu->_fmuData._fmiStatus = fmi1_import_completed_integrator_step(_fmu->_fmu, &_simSettings->_callEventUpdate);
            _fmu->completedIntegratorStep(&_simSettings->_callEventUpdate);

            //vw: since we are detecting changing inputs, we have to keep the values during the steps. do not reset it
            //X2 MF: On my system, this line is needed in order to get the keyboard inpot working
            _inputData.resetDiscreteInputValues();
//X9            return _fmu->_fmuData._tcur;
            return _fmu->getFMUData()->_tcur;
        }

        void OMVisualizerFMU::initializeVisAttributes(const double time)
        {
            _fmu->initialize(_simSettings);
            _omvManager->setVisTime(_omvManager->getStartTime());
            _omvManager->setSimTime(_omvManager->getStartTime());
            updateVisAttributes(_omvManager->getVisTime());
        }

        void OMVisualizerFMU::resetInputs()
        {
            //reset real input values to 0
            for (size_t r = 0; r < _inputData._data.getNumReal(); ++r)
                _inputData._data._valuesReal[r] = 0.0;
            //reset integer input values to 0
            for (size_t i = 0; i < _inputData._data.getNumInteger(); ++i)
                _inputData._data._valuesInteger[i] = 0;
            //reset boolean input values to 0
            for (size_t b = 0; b < _inputData._data.getNumBoolean(); ++b)
                _inputData._data._valuesBoolean[b] = false;
            //reset string input values to 0
            for (size_t s = 0; s < _inputData._data.getNumString(); ++s)
                _inputData._data._valuesString[s] = "";
        }

        int OMVisualizerFMU::updateVisAttributes(const double time)
        {
            int isOk(0);

            // Update all shapes
            OMVIS::Util::rAndT rT;
            osg::ref_ptr<osg::Node> child = nullptr;
            try
            {
                fmi1_import_t* fmu = _fmu->getFMU();
                ShapeObject shape;
                for (std::vector<Model::ShapeObject>::size_type i = 0; i != _baseData->_shapes.size(); i++)
                {
                    shape = _baseData->_shapes[i];

                    // get the values for the scene graph objects
                    Util::updateObjectAttributeFMU(&shape._length, time, fmu);
                    Util::updateObjectAttributeFMU(&shape._width, time, fmu);
                    Util::updateObjectAttributeFMU(&shape._height, time, fmu);

                    Util::updateObjectAttributeFMU(&shape._lDir[0], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._lDir[1], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._lDir[2], time, fmu);

                    Util::updateObjectAttributeFMU(&shape._wDir[0], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._wDir[1], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._wDir[2], time, fmu);

                    Util::updateObjectAttributeFMU(&shape._r[0], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._r[1], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._r[2], time, fmu);

                    Util::updateObjectAttributeFMU(&shape._rShape[0], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._rShape[1], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._rShape[2], time, fmu);

                    Util::updateObjectAttributeFMU(&shape._T[0], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[1], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[2], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[3], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[4], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[5], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[6], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[7], time, fmu);
                    Util::updateObjectAttributeFMU(&shape._T[8], time, fmu);

                    rT = Util::rotation(osg::Vec3f(shape._r[0].exp, shape._r[1].exp, shape._r[2].exp), osg::Vec3f(shape._rShape[0].exp, shape._rShape[1].exp, shape._rShape[2].exp), osg::Matrix3(shape._T[0].exp, shape._T[1].exp, shape._T[2].exp, shape._T[3].exp, shape._T[4].exp, shape._T[5].exp, shape._T[6].exp, shape._T[7].exp, shape._T[8].exp),
                                        osg::Vec3f(shape._lDir[0].exp, shape._lDir[1].exp, shape._lDir[2].exp), osg::Vec3f(shape._wDir[0].exp, shape._wDir[1].exp, shape._wDir[2].exp), shape._length.exp, shape._width.exp, shape._height.exp, shape._type);

                    shape._mat = Util::assemblePokeMatrix(shape._mat, rT._T, rT._r);

                    //update the shapes
                    _nodeUpdater->_shape = shape;
                    //_baseData->_visAttr.dumpVisAttributes();

                    //get the scene graph nodes and stuff
                    child = _viewerStuff->getScene().getRootNode()->getChild(i);  // the transformation
                    child->accept(*_nodeUpdater);

                }  //end for
            }  // end try

            catch (std::exception& e)
            {
                LOGGER_WRITE(std::string("Something went wrong in OMVisualizer::updateVisAttributes at time point ") + std::to_string(time) + std::string(" ."), Util::LC_SOLVER, Util::LL_WARNING);
                isOk = 1;
            }
            return isOk;
        }

        void OMVisualizerFMU::updateScene(const double time)
        {
            _omvManager->updateTick();  //for real-time measurement
            double simTime = _omvManager->getRealTime();

            _omvManager->setSimTime(_omvManager->getVisTime());
            double nextStep = _omvManager->getVisTime() + _omvManager->getHVisual();

            while (_omvManager->getSimTime() < nextStep)
            {
                //std::cout<<"simulate "<< _omvManager->getSimTime()<<" to "<<nextStep<<std::endl;
                //_inputData.printValues();
                _omvManager->setSimTime(simulateStep(_omvManager->getSimTime()));
            }
            _omvManager->updateTick();            //for real-time measurement
            simTime = _omvManager->getRealTime() - simTime;

            _omvManager->updateTick();            //for real-time measurement
            double visTime = _omvManager->getRealTime();

            updateVisAttributes(_omvManager->getVisTime());
            _omvManager->updateTick();            //for real-time measurement
            visTime = _omvManager->getRealTime() - visTime;
            //std::cout << "SIMTIME " << _omvManager->getHVisual() / (simTime) << "     VISTIME" << _omvManager->getHVisual() / (visTime) << std::endl;
            _omvManager->setRealTimeFactor(_omvManager->getHVisual() / (visTime + simTime));

        }

        std::string OMVisualizerFMU::getType() const
        {
            return "fmu";
        }

//        void OMVisualizerFMU::unload()
//        {
//            _fmu->clear();
//        }

        const FMU* OMVisualizerFMU::getFMU() const
        {
            return _fmu.get();
        }

        const InputData* OMVisualizerFMU::getInputData() const
        {
            return &_inputData;
        }

        InputData* OMVisualizerFMU::getInputData()
        {
            return &_inputData;
        }

    }  // End namespace Model
}  // End namespace OMVIS

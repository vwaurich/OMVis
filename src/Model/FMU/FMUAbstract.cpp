/*
 * FMUAbstract.cpp
 *
 *  Created on: 13.12.2016
 *      Author: mf
 */

#include "Model/FMU/FMUAbstract.hpp"
#include "Util/Logger.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        FMUAbstract::FMUAbstract()
                : _states(nullptr),
                  _statesDer(nullptr),
                  _eventIndicators(nullptr),
                  _eventIndicatorsPrev(nullptr),
                  _nStates(0),
                  _nEventIndicators(0),
                  _tcur(0.0),
                  _hcur(0.0)
        {
        }

        FMUAbstract::~FMUAbstract()
        {
            delete _states;
            delete _statesDer;
            delete _eventIndicators;
            delete _eventIndicatorsPrev;
        }

        bool FMUAbstract::checkForTriggeredEvent() const
        {
            for (size_t k = 0; k < _nEventIndicators; ++k)
            {
                if (_eventIndicators[k] * _eventIndicatorsPrev[k] < 0)
                {
                    LOGGER_WRITE("Event occurred at " + std::to_string(_tcur), Util::LC_CTR, Util::LL_DEBUG);
                    return true;
                }
            }
            return false;
        }

        void FMUAbstract::setLastStepSize(const double simTimeEnd)
        {
            if (_tcur > simTimeEnd - _hcur / 1e16)
            {
                _tcur -= _hcur;
                _hcur = simTimeEnd - _tcur;
                _tcur = simTimeEnd;
            }
        }

        void FMUAbstract::doEulerStep()
        {
            for (size_t k = 0; k < _nStates; ++k)
            {
                _states[k] = _states[k] + _hcur * _statesDer[k];
            }
        }

        double FMUAbstract::getTcur() const
        {
            return _tcur;
        }

    }  // namespace Model
}  // namespace OMVIS

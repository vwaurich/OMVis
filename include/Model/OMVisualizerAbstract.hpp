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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISUALIZERABSTRACT_HPP_
#define INCLUDE_OMVISUALIZERABSTRACT_HPP_

#include "View/OMVisScene.hpp"
#include "Model/OMVisualBase.hpp"
#include "Model/UpdateVisitor.hpp"
#include "Util/ObjectAttribute.hpp"
#include "Visualize.hpp"

#include <memory>

// Forward declaration
namespace OMVIS
{
    namespace Control
    {
        class OMVisManager;
    }
}

namespace OMVIS
{
    namespace Model
    {

        /*! \brief This class serves as abstract class for visualization.
         *
         * It provides basic methods for visualization.
         * Concrete implementations are \ref OMVisualizerMat and \ref OMVisualizerFMU.
         */
        class OMVisualizerAbstract
        {
         public:
            /// The default constructor.
            OMVisualizerAbstract();

            /*! \brief Constructs OMVisualizer object from arguments.
             *
             * @param model Name of the model.
             * @param dir Path to the FMU or result file and corresponding XML-file.
             */
            OMVisualizerAbstract(const std::string& modelName, const std::string& dir);

            /// Destructs OMVisualizer object.
            virtual ~OMVisualizerAbstract() = default;

            /// The copy constructor is forbidden.
            OMVisualizerAbstract(const OMVisualizerAbstract& omv) = delete;

            /// The assignment operator is forbidden.
            OMVisualizerAbstract& operator=(const OMVisualizerAbstract& omv) = delete;

            /*! \brief This methods initializes a Visualizer object.
             *
             * Encapsulates the three stages/methods of initialization process into one single method.
             */
            virtual int initialize()
            {
                int isOk(0);
                isOk += initData();
                isOk += setUpScene();
                isOk += updateVisAttributes(0.0);
                return isOk;
            }

            /*! \brief Initializes OMVisualizer object.
             *
             * XML-file is parsed and the values of the attributes are set.
             * The viewer is initialized and the scene is set up.
             * FMU is loaded if used, or the Matfile is read.
             * @return 1
             *
             * \todo Quick and dirty hack by passing OMVmanager, \see OMVisualizerFMU.
             *
             */
            virtual int initData();

            /*! \brief Set up the scene.
             *
             * @return Error value.
             */
            int setUpScene();

            /*! \brief In case of FMU visualization, this methods performs a simulation step.
             *
             * \remark All classes that derive from OMVisualizerAbstract
             * @param omvm
             */
            virtual void simulate(Control::OMVisManager& omvm) = 0;

            /*! \brief Virtual Method to update the scene. Is implemented either by using FMU or mat-file.
             *
             * \remark All classes that derive from OMVisualizerAbstract
             * @param omvm
             * \return Error value.
             */
            virtual int updateVisAttributes(const double time) = 0;

            /*! \brief Virtual Method to initialize the scene. Is implemented either by using FMU or mat-file.
             *
             * \remark All classes that derive from OMVisualizerAbstract
             */
            virtual void initializeVisAttributes(const double time) = 0;

            /*! \brief Virtual Prepares everything to make the correct visualization attributes available for that time step (i.e. simulate the fmu)
             *
             * \remark All classes that derive from OMVisualizerAbstract
             */
            virtual void updateScene(const double time) = 0;

            /*! \brief Returns "abstract"
             */
            virtual std::string getType() const;

            /*! \brief Starts the visualization.
             */
            void startVisualization();

            /*! \brief Pauses the visualization.
             */
            void pauseVisualization();
            /*! \brief Sets the scene to start position.
             */
            void initVisualization();

            /*! \brief Prints a message which tells you to buy us a coffee.
             */
            void donationVisualization();

            /*! \brief Calls for a scene update.
             */
            void sceneUpdate();

            std::shared_ptr<OMVisualBase> getBaseData() const
            {
                return _baseData;
            }

            std::shared_ptr<Control::OMVisManager> getOMVisManager() const
            {
                return _omvManager;
            }

            std::shared_ptr<View::OMVisScene> getOMVisScene() const
            {
                return _viewerStuff;
            }

         protected:
            std::shared_ptr<OMVisualBase> _baseData;
            std::shared_ptr<View::OMVisScene> _viewerStuff;
            std::shared_ptr<UpdateVisitor> _nodeUpdater;
            std::shared_ptr<Control::OMVisManager> _omvManager;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISUALIZERABSTRACT_HPP_ */
/**
 * @}
 */

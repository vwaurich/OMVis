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

/** \addtogroup View
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISVIEWER_HPP_
#define INCLUDE_OMVISVIEWER_HPP_

#include "Initialization/CommandLineArgs.hpp"
#include "Model/VisualizerAbstract.hpp"
#include "Control/TimeManager.hpp"
#include "Control/GUIController.hpp"
#include "View/ViewSettings.hpp"

#include <QTimer>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/View>
#include <osgQt/GraphicsWindowQt>

#include <string>
#include <memory>

QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QSlider)

namespace OMVIS
{
    namespace View
    {

        /*! \brief This class holds the OMVIS GUI.
         *
         * OMVIS is a open source tool for model and simulation visualization. What shall be visualized is described
         * in a XML file and the result files (in MAT format) provide the corresponding data. Besides the animation
         * of result files, there is a possibility to animate multi-body-systems with a FMU interactively.
         *
         * Since we use a toolbar, we have to derive from QMainWindow. In order to visualize the models, we have to
         * inherit from osgViewer::CompositeViewer.
         */
        class OMVISViewer : public QMainWindow, public osgViewer::CompositeViewer
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructs the OMVISViewer from command line arguments. */
            OMVISViewer(QWidget* parent = Q_NULLPTR, const Initialization::CommandLineArgs& clArgs =
                                Initialization::CommandLineArgs());

            OMVISViewer(const OMVISViewer& rhs) = delete;

            OMVISViewer& operator=(const OMVISViewer& rhs) = delete;

            /*! \brief Destructs the OMVISViewer object and frees memory.
             *
             * Since we use smart pointers where possible, we do not need to call delete explicitly.
             * The Qt stuff uses the parent-child model and thus memory is freed automatically.
             */
            ~OMVISViewer() = default;

         private:
            /*! \brief Constructs the OMVISViewer object from arguments.
             *
             * A empty OMVIS GUI is created and the model has to be loaded via the file open dialog.
             *
             * \param parent            Parent for initialization of QWidget Baseclass.
             * \param flags             Window flags for initialization of QWidget Baseclass.
             * \param threadingModel    The threading model.
             */
            OMVISViewer(QWidget* parent = Q_NULLPTR, osgViewer::ViewerBase::ThreadingModel threadingModel =
                                osgViewer::CompositeViewer::SingleThreaded,
                        const Initialization::CommandLineArgs& clArgs = Initialization::CommandLineArgs());

            /*-----------------------------------------
             * INITIALIZATION FUNCTIONS
             *---------------------------------------*/

            /*! \brief Adds an empty view-widget to the main-widget
             *
             * \param gw        The graphical window.
             * \param rootNode  The root node of the scene.
             * \return The widget for the osg-viewer.
             */
            QWidget* setupViewWidget(osg::ref_ptr<osgQt::GraphicsWindowQt> gw, const osg::ref_ptr<osg::Node> rootNode);

            /*! \brief Creates a default osg-viewer-widget. */
            QWidget* setupOSGViewerWidget(const osg::ref_ptr<osg::Node> rootNode);

            /*! \brief Creates a osgQt::createGraphicsWindow.
             *
             * \param x
             * \param y
             * \param w     The width.
             * \param h     The height.
             * \param name  The name of the window.
             * \param windowDecoration
             * \return The osgQt::GraphicsWindowQt - window.
             */
            osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow(int x, int y, int w, int h,
                                                                       const std::string& name = "",
                                                                       bool windowDecoration = false);

            /*! \brief Renders the widget in case of a paint event. */
            virtual void paintEvent(QPaintEvent* event);

            /*! \brief This function sets up the widgets _timeSliderWidget, _osgViewerWidget and _controlElementwidget.
             *
             * This function encapsulates all the functions to set up the widgets of the GUI.
             */
            void setupWidgets();

            /*! \brief Creates the layout which is used for OMVIS.
             *
             * We use a row layout consisting of the three widgets _timeSliderWidget, _osgViewerWidget and _controlElementwidget.
             */
            void createLayout();

            /*! \brief Creates the top menu bar.
             *
             * The top menu bar is created by adding the menu points connecting them to corresponding slots.
             * Currently, there are the top menus File, Settings and Input.
             */
            void createMenuBar();

            /*! \brief Creates the menu actions and connects them to the corresponding slot functions. */
            void createActions();

            /*! \brief Creates the control widget (play, pause,...). */
            QWidget* setupControlElementWidget();

            /*! \brief Creates and sets up the time slider widget.
             *
             * We use a horizontal slider widget with a range from 0 to 100.
             * The slider position is set to default value of 0, i.e., to the very left position.
             * A connection to the method \ref setVisTimeSlotFunction(int) is created and triggered if the user
             * moves the slider.
             */
            void setupTimeSliderWidget();

            /*! \brief Creates a GUI element to set the mapping for a input value. */
            QHBoxLayout* createInputMapperRow(const int inputIdx, const std::string& varName,
                                              const std::string& type) const;

            /*! \brief Updates the time slider to the position which corresponds to the current visualization time.
             *
             * The new position of the slider is provided by the function \ref Control::GUIController::getTimeProgress().
             * If the position is out of the sliders range, a warning is triggered by the Logger.
             */
            void updateTimeSliderPosition();

            /*! \brief Updates the time display to the current visualization time.
             *
             * The current visualization time is provided by the function \ref Control::GUIController::getVisTime().
             */
            void updateTimeDisplays();

            /*! \brief Resets the time slider and time display. */
            void resetTimingElements();

            /*! \brief Enables the time slider.
             *
             * In case of MAT result file visualization, the user can move the time slider and the appropriate
             * simulation status will be loaded.
             * \todo This is a todo!
             */
            void enableTimeSlider();

            /*! \brief Disables the time slider.
             *
             * If no model or a FMU is loaded, the user is not allowed to move the time slider.
             */
            void disableTimeSlider();

            /*! \brief Function that updates the timing elements, i.e., the slider and the visualization time information.
             *
             * Calls \ref updateTimeSliderPosition() and \ref updateTimeDisplays().
             */
            void updateTimingElements();

            /*-----------------------------------------
             * SLOT FUNCTIONS
             *---------------------------------------*/

         private slots:

            /*! \brief Function that is triggered by the play-button. */
            void playSlotFunction();

            /*! \brief Function that is triggered by the pause-button. */
            void pauseSlotFunction();

            /*! \brief Function that is triggered by the initialize-button. */
            void initSlotFunction();

            /*! \brief Function that is triggered the scene-update timer.
             */
            void updateScene();

            /*! \brief Function that is triggered the scene-update timer. */
            void setVisTimeSlotFunction(int val);

            /*! \brief Function that opens a new model from file for visualization.
             *
             * The model which should be loaded is chosen via a file open dialog. The user can specify whether to load
             * a FMU or a MAT file for visualization.
             *
             * \param clArgs    The parsed command line arguments.
             */
            void open(const Initialization::CommandLineArgs& clArgs = Initialization::CommandLineArgs());

            /*! \brief Open remote connection. */
            void openRemoteConnection(const Initialization::CommandLineArgs& clArgs =
                    Initialization::CommandLineArgs());

            void unloadModel();

            /*! \todo Implement me. */
            void exportVideo();

            /*! \brief Function that opens the input mapper dialog. */
            void openDialogInputMapper();

            /*! \brief Creates dialog window to specify the perspective to the scene view. */
            void perspectiveDialog();

            /*! \brief This functions resets the camera to the initial view.
             *
             * This functionality will be useful if the user got lost in rotating the model and wants to reset the view.
             */
            void resetCamera();

            /*! \brief This functions resets the camera normal to X-Y-plane. */
            void cameraPositionXY();

            /*! \brief This functions resets the camera normal to X-Z-plane. */
            void cameraPositionXZ();

            /*! \brief This functions resets the camera normal to Y-Z-plane. */
            void cameraPositionYZ();

            /*! \brief Creates dialog window to specify the background color of the scene view.
             *
             * \todo Add some more suitable colors. The currently available colors are a bit too loud.
             */
            void backgroundColorDialog();

            /*! \brief Creates dialog window to specify the simulationSettings
             */
            void simSettingsDialog();

            /*! \brief Updates the key-input-map
             */
            void updateKeyMapValue(QString key);

            /*! \brief Shows information about OMVIS. */
            void aboutOMVIS() const;

            void help() const;

         private:

            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            // --- Menus ---
            QMenu* _fileMenu;
            QMenu* _settingsMenu;
            QMenu* _inputMenu;
            QMenu* _helpMenu;

            // --- Actions ---
            QAction* _openAct;
            QAction* _openRCAct;
            QAction* _exportAct;
            QAction* _exitAct;
            QAction* _mapInputAct;
            QAction* _dontCareAct;
            QAction* _aboutOMVISAct;
            QAction* _helpAct;
            QAction* _perspectiveAct;
            QAction* _bgcAct;
            QAction* _simSettingsAct;
            QAction* _unloadAct;

            /*! \brief The view which holds the osg scene. */
            osg::ref_ptr<osgViewer::View> _sceneView;

            // --- Widgets---
            /// Widget which handles the OSG scene.
            QWidget* _osgViewerWidget;
            /// Widget which handles the control elements (e.g., play, stop button).
            QWidget* _controlElementWidget;

            /// Slider that corresponds to the current simulation time with respect to start and end time.
            QSlider* _timeSlider;
            /// This label displays the current visualization time.
            QLabel* _timeDisplay;
            /// This label displays the current real time factor.
            QLabel* _RTFactorDisplay;
            /// Triggers a new frame.
            QTimer _renderTimer;

            /*! This is the central widget for QMainWindow */
            QWidget* _centralWidget;
            /*! This is the layout added to the central widget. */
            QVBoxLayout* _mainLayout;

            /*! \brief This timer controls the frequency of the scene updates.
             *
             * Its timeout() signal is connected to the \ref updateScene method in order to trigger a scene update at
             * constant time. The default value is 100 ms. It can be changed by the user via the simulation settings
             * dialog.
             */
            QTimer _visTimer;

            /*! \brief The GUIController object will take the users input from GUI and handle it.
             *
             * The GUIController holds the VisualizerObject and controls it in order to the users required actions (e.g.,
             * load a model, initialize a model, play simulation/visualization, pause simulation/visualization, etc.).
             */
            std::unique_ptr<Control::GUIController> _guiController;

            /*! \brief The name of the logo file of OMVIS. */
            const std::string _logo = "OMVISLogo.osg";
        };

    }  // namespace View
}  // namespace OMVIS

#endif /* INCLUDE_OMVISVIEWER_HPP_ */
/**
 * \}
 */

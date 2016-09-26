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

#include "Control/GUIController.hpp"
#include "Util/Logger.hpp"
#include "Util/Algebra.hpp"
#include "View/OMVisViewer.hpp"
#include "View/Dialogs.hpp"
#include "Initialization/VisualizationConstructionPlans.hpp"
#include "Model/FMUWrapper.hpp"
#include "Model/VisualizerFMU.hpp"
#include "Model/SimSettings.hpp"

#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Vec3>
#include <osg/io_utils>
#include <osg/MatrixTransform>

#include <QScreen>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGuiApplication>
#include <QPushButton>
#include <QObject>
#include <QAction>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QComboBox>
#include <QMenuBar>

#include <assert.h>

#include <stdexcept>

namespace OMVIS
{
    namespace View
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/


        OMVisViewer::OMVisViewer(const Initialization::CommandLineArgs& clArgs)
            : OMVisViewer(osgViewer::CompositeViewer::SingleThreaded, clArgs)
        {

        }

        //OMVisViewer::OMVisViewer(/*QWidget* parent, Qt::WindowFlags f,*/osgViewer::ViewerBase::ThreadingModel threadingModel)
        OMVisViewer::OMVisViewer(/*QWidget* parent, Qt::WindowFlags f,*/osgViewer::ViewerBase::ThreadingModel threadingModel,
                                 const Initialization::CommandLineArgs& clArgs)
                : QMainWindow(/*parent, f*/),
                  osgViewer::CompositeViewer(),
                  _fileMenu(nullptr),
                  _settingsMenu(nullptr),
                  _inputMenu(nullptr),
                  _helpMenu(nullptr),
                  _openAct(nullptr),
                  _openRCAct(nullptr),
                  _exportAct(nullptr),
                  _exitAct(nullptr),
                  _mapInputAct(nullptr),
                  _dontCareAct(nullptr),
                  _aboutOMVisAct(nullptr),
                  _helpAct(nullptr),
                  _perspectiveAct(nullptr),
                  _bgcAct(nullptr),
                  _simSettingsAct(nullptr),
                  _sceneView(new osgViewer::View()),
                  _osgViewerWidget(nullptr),
                  _controlElementWidget(nullptr),
                  _timeSliderWidget(nullptr),
                  _timeSlider(nullptr),
                  _timeDisplay(new QLabel()),
                  _RTFactorDisplay(new QLabel()),
                  _renderTimer(),
                  _visTimer(),
                  _guiController(new Control::GUIController())
        {
            // Yeah, setting QLocale did not help to convert atof("0.05") to double(0.05) when the (bash) environment is german.
            setlocale(LC_ALL, "en_US.UTF-8");

            //the names
            setObjectName("MainWindow");
            setWindowTitle("OMVIS - The Open-Source FMU-visualization");

            //the osg threading model
            setThreadingModel(threadingModel);

            // disable the default setting of viewer.done() by pressing Escape.
            setKeyEventSetsDone(0);

            // Create the widgets for time slider, osg viewer and control elements.
            setupWidgets();

            // Set up the top menu-bar.
            createActions();
            createMenuBar();

            // Assemble the widgets to a layout and create the father of all widgets with this layout.
            createLayout();

            // To trigger the paint event which renders the view.
            //MF: What is this good for?
            QObject::connect(&_renderTimer, SIGNAL(timeout()), this, SLOT(update()));
            _renderTimer.start(10);

            // To trigger the scene updates with the visualization step size.
            QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateScene()));
            QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateTimingElements()));

            // GUI will be resized to half of screen.
            resize(QGuiApplication::primaryScreen()->availableSize() * 0.5);

            // Default interval to trigger timeout signal for this timer.
            _visTimer.setInterval(100);

            // Load model from command line
            if (clArgs.localVisualization())
            {
                std::cout << "Local Visualization!" << std::endl;
                open(clArgs);
            }
            else if (clArgs.remoteVisualization())
            {
                std::cout << "Remote Visualization!" << std::endl;
                openRemoteConnection(clArgs);
            }
        }

        /*-----------------------------------------
         * INITIALIZATION FUNCTIONS
         *---------------------------------------*/

        void OMVisViewer::setupWidgets()
        {
            //X8 We have a initial scene:
            osg::ref_ptr<osg::Node> rootNode = osgDB::readNodeFile("dumptruck.osg");

            // Set up the osg viewer widget
            _osgViewerWidget = setupOSGViewerWidget(rootNode);

            // Set up the control elements widget
            _controlElementWidget = setupControlElementWidget();

            // Set up the time slider widget and set the slider to start position.
            _timeSliderWidget = setupTimeSliderWidget();
        }

        // Assemble the widgets to a layout and create the father of all widgets with this layout.
        void OMVisViewer::createLayout()
        {
            QVBoxLayout* mainRowLayout = new QVBoxLayout();
            assert(_osgViewerWidget != nullptr);
            assert(_timeSliderWidget != nullptr);
            assert(_controlElementWidget != nullptr);
            mainRowLayout->addWidget(_osgViewerWidget);
            mainRowLayout->addWidget(_timeSliderWidget);
            mainRowLayout->addWidget(_controlElementWidget);

            QWidget* topWidget = new QWidget();
            topWidget->setLayout(mainRowLayout);
            setCentralWidget(topWidget);
        }

        void OMVisViewer::createActions()
        {
            // Menu caption "File".
            _openAct = new QAction(tr("Open..."), this);
            _openAct->setShortcut(tr("Ctrl+O"));
            QObject::connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));

            _openRCAct = new QAction(tr("Open Remote Connection..."), this);
            _openRCAct->setShortcut(tr("Ctrl+R"));
            QObject::connect(_openRCAct, SIGNAL(triggered()), this, SLOT(openRemoteConnection()));

            _unloadAct = new QAction(tr("Remove Current Model"), this);
            _unloadAct->setShortcut(tr("Ctrl+D"));
            QObject::connect(_unloadAct, SIGNAL(triggered()), this, SLOT(unloadModel()));

            _exportAct = new QAction(tr("Export Video"), this);
            QObject::connect(_exportAct, SIGNAL(triggered()), this, SLOT(exportVideo()));
            _exitAct = new QAction(tr("Quit"), this);
            _exitAct->setShortcut(tr("Ctrl+Q"));
            QObject::connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

            // Menu caption "Settings".
            _perspectiveAct = new QAction(tr("Perspective..."), this);
            QObject::connect(_perspectiveAct, SIGNAL(triggered()), this, SLOT(perspectiveDialog()));
            _bgcAct = new QAction(tr("Background Color..."), this);
            QObject::connect(_bgcAct, SIGNAL(triggered()), this, SLOT(backgroundColorDialog()));
            _simSettingsAct = new QAction(tr("Simulation Settings..."), this);
            _simSettingsAct->setEnabled(false);
            QObject::connect(_simSettingsAct, SIGNAL(triggered()), this, SLOT(simSettingsDialog()));

            // Menu caption "Inputs".
            _mapInputAct = new QAction(tr("Map Inputs..."), this);
            QObject::connect(_mapInputAct, SIGNAL(triggered()), this, SLOT(openDialogInputMapper()));
            _dontCareAct = new QAction(tr("I Don't Care About Inputs"), this);
            //QObject::connect(_dontCareAct, SIGNAL(triggered()), this, SLOT());

            // Menu caption "Help".
            _aboutOMVisAct = new QAction(tr("About OMVis..."), this);
            QObject::connect(_aboutOMVisAct, SIGNAL(triggered()), this, SLOT(aboutOMVis()));
            _helpAct = new QAction(tr("Help..."), this);
            QObject::connect(_helpAct, SIGNAL(triggered()), this, SLOT(help()));
        }

        void OMVisViewer::createMenuBar()
        {
            // Menu caption "File".
            _fileMenu = new QMenu(tr("&File"), this);
            _fileMenu->addAction(_openAct);
            _fileMenu->addAction(_openRCAct);
            _fileMenu->addAction(_unloadAct);

            _fileMenu->addAction(_exportAct);
            _fileMenu->addSeparator();
            _fileMenu->addAction(_exitAct);

            // Menu caption "Settings".
            _settingsMenu = new QMenu(tr("Settings"), this);
            _settingsMenu->addAction(_perspectiveAct);
            _settingsMenu->addAction(_bgcAct);
            _settingsMenu->addAction(_simSettingsAct);

            // Menu caption "Inputs".
            _inputMenu = new QMenu(tr("Inputs"), this);
            _inputMenu->addAction(_mapInputAct);
            _inputMenu->addAction(_dontCareAct);

            // Menu caption "Help".
            _helpMenu = new QMenu(tr("&Help"), this);
            _helpMenu->addAction(_aboutOMVisAct);
            _helpMenu->addAction(_helpAct);

            // Add the menus to the menubar.
            menuBar()->addMenu(_fileMenu);
            menuBar()->addMenu(_settingsMenu);
            menuBar()->addMenu(_inputMenu);
            menuBar()->addMenu(_helpMenu);
        }

        QWidget* OMVisViewer::setupOSGViewerWidget(osg::ref_ptr<osg::Node> rootNode)
        {
            //the osg-viewer widget
            osg::ref_ptr<osgQt::GraphicsWindowQt> window = createGraphicsWindow(0, 0, 100, 100);

            if (rootNode == nullptr)
                std::cout << "Something went wrong loading the osg file." << std::endl;
            return setupViewWidget(window, rootNode);
        }

        QWidget* OMVisViewer::setupViewWidget(osg::ref_ptr<osgQt::GraphicsWindowQt> gw, osg::ref_ptr<osg::Node> rootNode)
        {
            if (_sceneView == nullptr)
                _sceneView = new osgViewer::View();
            addView(_sceneView);

            osg::ref_ptr<osg::Camera> camera = _sceneView->getCamera();
            camera->setGraphicsContext(gw);

            const osg::GraphicsContext::Traits* traits = gw->getTraits();

            camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
            camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
            camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height),
                                                     1.0f, 10000.0f);

            _sceneView->setSceneData(rootNode);
            _sceneView->addEventHandler(new osgViewer::StatsHandler());
            _sceneView->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator());
            gw->setTouchEventsEnabled(true);

            return gw->getGLWidget();
        }

        osg::ref_ptr<osgQt::GraphicsWindowQt> OMVisViewer::createGraphicsWindow(int x, int y, int w, int h,
                                                                                const std::string& name, bool windowDecoration)
        {
            osg::ref_ptr<osg::DisplaySettings> ds = osg::DisplaySettings::instance().get();
            osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
            traits->windowName = name;
            traits->windowDecoration = windowDecoration;
            traits->x = x;
            traits->y = y;
            traits->width = w;
            traits->height = h;
            traits->doubleBuffer = true;
            traits->alpha = ds->getMinimumNumAlphaBits();
            traits->stencil = ds->getMinimumNumStencilBits();
            traits->sampleBuffers = ds->getMultiSamples();
            traits->samples = ds->getNumMultiSamples();

            return new osgQt::GraphicsWindowQt(traits.get());
        }

        QWidget* OMVisViewer::setupTimeSliderWidget()
        {
            _timeSlider = new QSlider(Qt::Horizontal, this);
            _timeSlider->setFixedHeight(30);
            _timeSlider->setMinimum(0);
            _timeSlider->setMaximum(100);
            _timeSlider->setSliderPosition(0);

            LOGGER_WRITE(std::string("min ") + std::to_string(_timeSlider->minimum()) + std::string(" and max ")
                         + std::to_string(_timeSlider->maximum()), Util::LC_GUI, Util::LL_INFO);

            QObject::connect(_timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVisTimeSlotFunction(int)));
            return _timeSlider;
        }

        QWidget* OMVisViewer::setupControlElementWidget()
        {
            QPushButton* playButton = new QPushButton("Play", this);
            QPushButton* pauseButton = new QPushButton("Pause", this);
            QPushButton* initButton = new QPushButton("Initialize", this);

            _timeDisplay->setText(QString("Time [s]: ").append(QString::fromStdString("")));
            _RTFactorDisplay->setText(QString("RT-Factor: ").append(QString::fromStdString("")));

            //the button row
            QHBoxLayout* buttonRowLayOut = new QHBoxLayout();
            QGroupBox* buttonRowBox = new QGroupBox();
            buttonRowLayOut->addWidget(initButton);
            buttonRowLayOut->addWidget(playButton);
            buttonRowLayOut->addWidget(pauseButton);
            buttonRowLayOut->addWidget(_RTFactorDisplay);
            buttonRowLayOut->addWidget(_timeDisplay);
            buttonRowBox->setLayout(buttonRowLayOut);
            buttonRowBox->setFixedHeight(60);

            // Connect the buttons to the corresponding slot functions.
            QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(playSlotFunction()));
            QObject::connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseSlotFunction()));
            QObject::connect(initButton, SIGNAL(clicked()), this, SLOT(initSlotFunction()));

            return buttonRowBox;
        }

        /*-----------------------------------------
         * SLOT FUNCTIONS
         *---------------------------------------*/

        void OMVisViewer::playSlotFunction()
        {
            _guiController->startVisualization();
        }

        void OMVisViewer::pauseSlotFunction()
        {
            _guiController->pauseVisualization();
        }

        void OMVisViewer::initSlotFunction()
        {
            _guiController->initVisualization();
        }

        void OMVisViewer::updateScene()
        {
            _guiController->sceneUpdate();
        }

        void OMVisViewer::setVisTimeSlotFunction(int val)
        {
            _guiController->setVisTime(val);
            _guiController->sceneUpdate();
        }

        /// \todo FIXME We the user clicks "Cancel" the error message is shown.
        void OMVisViewer::open(const Initialization::CommandLineArgs& clArgs)
        {
            try
            {
                Initialization::VisualizationConstructionPlan constructionPlan;

                if (clArgs.empty())
                {
                    // Get model file name and path from dialog.
                    OpenFileDialog dialog(this);
                    dialog.exec();
                    constructionPlan = dialog.getConstructionPlan();
                }
                else
                {
                    constructionPlan = clArgs.getVisualizationConstructionPlan();
                }

                // Let the GUIController load the model.
                _guiController->loadModel(constructionPlan, _timeSlider->minimum(), _timeSlider->maximum());

                LOGGER_WRITE(std::string("The model has been successfully loaded and initialized."), Util::LC_GUI, Util::LL_INFO);

                // Set up the osg viewer widget
                osg::ref_ptr<osg::Node> rootNode = _guiController->getSceneRootNode();
                if (rootNode == nullptr)
                    LOGGER_WRITE(std::string("Scene root node is null pointer."), Util::LC_GUI, Util::LL_ERROR);
                _sceneView->setSceneData(rootNode);

                //start the timer to trigger model and scene update
                _visTimer.start(_guiController->getVisStepsize());  // we need milliseconds in here

                //set the inputData to handle Keyboard-events as inputs
                if (_guiController->visTypeIsFMU())
                {
                    Control::KeyboardEventHandler* kbEventHandler = new Control::KeyboardEventHandler(_guiController->getInputData());
                    _sceneView->addEventHandler(kbEventHandler);
                }

                // Update the slider and the time displays.
                updateTimingElements();

                LOGGER_WRITE(std::string("OSGViewUpdated"), Util::LC_LOADER, Util::LL_WARNING);
            }
            catch (std::exception& ex)
            {
                QMessageBox::critical(0, QString("Error"), QString(ex.what()));
                std::cout << ex.what();
            }

            // If a model is loaded, we can enable some buttons
            _simSettingsAct->setEnabled(true);
        }

        //MF: Compute on a server, visualize on localhost
        void OMVisViewer::openRemoteConnection(const Initialization::CommandLineArgs& clArgs)
        {
            try
            {
                Initialization::RemoteVisualizationConstructionPlan constructionPlan;

                if (clArgs.empty())
                {
                    // Get IP address and port as well as model file and path from dialog.
                    OpenRemoteConnectionDialog dialog(this);
                    dialog.exec();
                    constructionPlan = dialog.getConstructionPlan();
                }
                else
                {
                    constructionPlan = clArgs.getRemoteVisualizationConstructionPlan();
                }

                assert(constructionPlan.visType != Model::VisType::FMU);
                assert(constructionPlan.visType != Model::VisType::MAT);

                // Now, let the factory create the VisualizerFMUClient object, establish the connection
                // and initialize the simulation.
                _guiController->loadModel(constructionPlan, _timeSlider->minimum(), _timeSlider->maximum());

                LOGGER_WRITE(std::string("The model has been successfully loaded and initialized for remote visualization."),
                             Util::LC_GUI, Util::LL_INFO);

                // Set up the osg viewer widget
                osg::ref_ptr<osg::Node> rootNode = _guiController->getSceneRootNode();
                if (rootNode == nullptr)
                    LOGGER_WRITE(std::string("Scene root node is null pointer."), Util::LC_GUI, Util::LL_ERROR);
                _sceneView->setSceneData(rootNode);

                //start the timer to trigger model and scene update
                _visTimer.start(_guiController->getVisStepsize());  // we need milliseconds in here

                //set the inputData to handle Keyboard-events as inputs
                if (_guiController->visTypeIsFMURemote())
                {
                    Control::KeyboardEventHandler* kbEventHandler = new Control::KeyboardEventHandler(_guiController->getInputData());
                    _sceneView->addEventHandler(kbEventHandler);
                }

                // Update the slider and the time displays.
                updateTimingElements();

                LOGGER_WRITE(std::string("OSGViewUpdated"), Util::LC_LOADER, Util::LL_INFO);
            }
            catch (std::exception& ex)
            {
                QMessageBox::critical(0, QString("Error OMVis"), QString(ex.what()));
                std::cout << ex.what();
            }

            // If a model is loaded, we can enable some buttons
            _simSettingsAct->setEnabled(true);
        }

        void OMVisViewer::unloadModel()
        {
            LOGGER_WRITE(std::string("Unload model..."), Util::LC_LOADER, Util::LL_INFO);
            _sceneView->setSceneData(new osg::Node());

            _guiController->unloadModel();

            _visTimer.stop();
            resetTimingElements();
            LOGGER_WRITE(std::string("Model unloaded."), Util::LC_LOADER, Util::LL_INFO);

            // If a model is loaded, we can enable some buttons
            _simSettingsAct->setEnabled(false);
        }

        void OMVisViewer::exportVideo()
        {
            QMessageBox::warning(0, QString("Information"), QString("This functionality might come soon."));
        }

        void OMVisViewer::openDialogInputMapper()
        {
            // Proceed, if a model is already loaded. Otherwise give the user a hint to load a model first.
            if (!_guiController->modelIsLoaded())
            {
                QString information("Please load a model into OMVis first.");
                QMessageBox msgBox(QMessageBox::Information, tr("No Model Loaded"), information, QMessageBox::NoButton);
                msgBox.setStandardButtons(QMessageBox::Close);
                msgBox.exec();
            }
            // If a result file is visualized, we cannot map keys to input variables.
            else if (_guiController->visTypeIsMAT())
            {
                QString information("Input Mapping is not available for mat file visualization.");
                QMessageBox msgBox(QMessageBox::Information, tr("Not Available"), information, QMessageBox::NoButton);
                msgBox.setStandardButtons(QMessageBox::Close);
                msgBox.exec();
            }
            // FMU
            else
            {
                QDialog* inputDialog = new QDialog(this);

                // The first way of adding the buttons Apply and Close
                QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                connect(buttonBox, SIGNAL(accepted()), inputDialog, SLOT(accept()));
                connect(buttonBox, SIGNAL(rejected()), inputDialog, SLOT(reject()));

                //X4 The second way of adding the buttons Apply and Close
                //X4        QDialogButtonBox* buttonBox = new QDialogButtonBox();
                //X4        QPushButton* close = new QPushButton(tr("&Close"));
                //X4        QPushButton* apply = new QPushButton(tr("&Apply"));
                //X4        apply->setDefault(true);

                // todo: Currently we do not differentiate between the two buttons.
                //       If "Apply" is clicked, the chosen selection of key mapping should be applied. Up to
                //       this point it is only "cached".
                //       If "Close" is clicked, the cached selection is discarded.
                //X4        connect(buttonBox, SIGNAL(accepted()), inputDialog, SLOT(close()));
                //X4        connect(buttonBox, SIGNAL(rejected()), inputDialog, SLOT(close()));
                //X4        buttonBox->addButton(close, QDialogButtonBox::RejectRole);
                //X4        buttonBox->addButton(apply, QDialogButtonBox::AcceptRole);

                QVBoxLayout* mainLayout = new QVBoxLayout();
                QGroupBox* qgroupBox;

                std::shared_ptr<Model::InputData> inputData = _guiController->getInputData();
                const Model::InputValues* inputValues = inputData->getInputValues();

                // Boolean inputs
                if (inputValues->getNumBoolean() > 0)
                {
                    qgroupBox = new QGroupBox(tr("Boolean Inputs"), this);
                    QFormLayout* layout = new QFormLayout();
                    // layout->addRow(new QLabel(tr("Line 1:")), new QLineEdit);
                    // layout->addRow(new QLabel(tr("Line 2, long text:")), new QComboBox);
                    // layout->addRow(new QLabel(tr("Line 3:")), new QSpinBox);

                    for (size_t inputIdx = 0; inputIdx < inputValues->getNumBoolean(); ++inputIdx)
                    {
                        QHBoxLayout* inputRow = createInputMapperRow(inputIdx, inputValues->_namesBool[inputIdx], "bool");
                        layout->addRow(inputRow);
                    }
                    qgroupBox->setLayout(layout);
                    mainLayout->addWidget(qgroupBox);
                }

                // Real inputs
                if (inputValues->getNumReal() > 0)
                {
                    qgroupBox = new QGroupBox(tr("Real Inputs"), this);
                    QFormLayout* layout = new QFormLayout();
                    for (size_t inputIdx = 0; inputIdx < inputValues->getNumReal(); ++inputIdx)
                    {
                        QHBoxLayout* inputRow = createInputMapperRow(inputIdx, inputValues->_namesReal[inputIdx], "real");
                        layout->addRow(inputRow);
                    }
                    qgroupBox->setLayout(layout);
                    mainLayout->addWidget(qgroupBox);
                }

                // Integer inputs
                if (inputValues->getNumInteger() > 0)
                {
                    qgroupBox = new QGroupBox(tr("Integer Inputs"), this);
                    QFormLayout* layout = new QFormLayout();
                    for (size_t inputIdx = 0; inputIdx < inputValues->getNumInteger(); ++inputIdx)
                    {
                        QHBoxLayout* inputRow = createInputMapperRow(inputIdx, inputValues->_namesInteger[inputIdx], "integer");
                        layout->addRow(inputRow);
                    }
                    qgroupBox->setLayout(layout);
                    mainLayout->addWidget(qgroupBox);
                }

                // String inputs
                //    for (size_t inputIdx = 0; inputIdx < inputData->_data.getNumString(); ++inputIdx)
                //        {
                //            QHBoxLayout* inputRow = createInputMapperRow(inputIdx, inputData->_data._namesString.at(inputIdx), "string");
                //            valueLayout->addLayout(inputRow);
                //        }
                //

                mainLayout->addWidget(buttonBox);
                inputDialog->setLayout(mainLayout);
                inputDialog->exec();
            }
        }

        QHBoxLayout* OMVisViewer::createInputMapperRow(const int inputIdx, const std::string& varName, const std::string& type) const
        {
            QHBoxLayout* inputRow = new QHBoxLayout();
            QLabel* inputLabel = new QLabel(QString("Input ").append(QString::number(inputIdx)));
            QLabel* varLabel = new QLabel(QString::fromStdString(varName));
            QLabel* typeLabel = new QLabel(QString::fromStdString(type));
            QComboBox* inputText = new QComboBox();
            if (type == "bool")
            {
                QObject::connect(inputText, SIGNAL(currentIndexChanged(QString)), SLOT(updateKeyMapValue(QString)));

                inputText->addItem(QString("bool").append(QString::number(inputIdx)).append(" -> KEY_W"));
                inputText->addItem(QString("bool").append(QString::number(inputIdx)).append(" -> KEY_A"));
                inputText->addItem(QString("bool").append(QString::number(inputIdx)).append(" -> KEY_S"));
                inputText->addItem(QString("bool").append(QString::number(inputIdx)).append(" -> KEY_D"));
            }
            else if (type == "real")
            {
                QObject::connect(inputText, SIGNAL(currentIndexChanged(QString)), SLOT(updateKeyMapValue(QString)));

                inputText->addItem((QString("real").append(QString::number(inputIdx)).append(" -> JOY_1_X")));
                inputText->addItem((QString("real").append(QString::number(inputIdx)).append(" -> JOY_1_Y")));
                inputText->addItem((QString("real").append(QString::number(inputIdx)).append(" -> JOY_2_X")));
                inputText->addItem((QString("real").append(QString::number(inputIdx)).append(" -> JOY_2_Y")));
            }
            else
                inputText->addItem("IMPLEMENT ME", "IMPLEMENT ME");

            inputText->setMaximumHeight(20);
            inputLabel->setMaximumHeight(20);
            inputRow->addWidget(inputLabel);
            inputRow->addWidget(varLabel);
            inputRow->addWidget(typeLabel);
            inputRow->addWidget(inputText);
            return inputRow;
        }

        void OMVisViewer::updateKeyMapValue(QString varToKey)
        {
//    std::string varToKeyString = varToKey.toStdString();
//    int posKey = varToKeyString.find(" -> ");
//    std::string typeString = varToKeyString.substr(0, lengthOfTypeStr);
//    std::string idxString = varToKeyString.substr(lengthOfTypeStr, posKey - lengthOfTypeStr);
//    std::string keyString = varToKeyString.substr(posKey + std::string(" -> ").length(), varToKeyString.length());
//    std::cout << "type: " << typeString << std::endl;
//    std::cout << "idx: " << idxString << std::endl;
//    std::cout << "key: " << keyString << std::endl;
//    fmi1_base_type_enu_t type = Model::getFMI1baseTypeFor4CharString(typeString);
//
//    KeyMapValue mapValue = { type, std::stoi(idxString) };
//    fmuVisualizer->_inputData._keyToInputMap[Model::getInputDataKeyForString(keyString)] = mapValue;
//    //remove current mapping!!
//
//    fmuVisualizer->_inputData.printKeyToInputMap();
        }

        void OMVisViewer::perspectiveDialog()
        {
            PerspectiveSettingDialog dialog(this);
            if (dialog.exec())
            {
                Perspective newPerspective = dialog.getResult();
                switch (newPerspective)
                {
                    case home:
                        resetCamera();
                        break;
                    case xy:
                        cameraPositionXY();
                        break;
                    case xz:
                        cameraPositionXZ();
                        break;
                    case yz:
                        cameraPositionYZ();
                        break;
                }
            }
        }

        void OMVisViewer::simSettingsDialog()
        {
            if (_guiController->modelIsLoaded())
            {
                if (_guiController->visTypeIsFMU() || _guiController->visTypeIsFMURemote())
                {
                    SimSettingDialogFMU dialog(this);
                    if (dialog.exec())
                    {
                        Model::UserSimSettingsFMU simSetFMU = dialog.getSimSettings();
                        _guiController->handleSimulationSettings(simSetFMU);

                        // Set new interval value to call sceneUpdate()
                        _visTimer.setInterval(simSetFMU.visStepSize);
                    }
                }
                else if (_guiController->visTypeIsMAT() || _guiController->visTypeIsMATRemote())
                {
                    SimSettingDialogMAT dialog(this);
                    if (dialog.exec())
                    {
                        Model::UserSimSettingsMAT simSetMAT = dialog.getSimSettings();
                        // Currently, the user can only specify a speed up and slow down, respectively.
                        _guiController->handleSimulationSettings(simSetMAT);
                    }
                }
            }
        }

        void OMVisViewer::backgroundColorDialog()
        {
            BackgroundColorSettingDialog dialog(this);
            if (dialog.exec())
            {
                BackgroundColor newColor = dialog.getResult();
                osg::Vec4 colVec;
                switch (newColor)
                {
                    case blue:
                        colVec = osg::Vec4(0.0f, 0.0f, 100.0f, 1.0f);
                        break;
                    case lila:
                        colVec = osg::Vec4(150.0f, 0.0f, 200.0f, 10.0f);
                        break;
                    case white:
                        colVec = osg::Vec4(255.0f, 255.0f, 255.0f, 0.0f);
                        break;
                    case green:
                        colVec = osg::Vec4(0.0f, 200.0f, 0.0f, 0.0f);
                        break;
                    case black:
                        colVec = osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f);
                        break;
                }
                _sceneView->getCamera()->setClearColor(colVec);
            }
        }

        void OMVisViewer::cameraPositionXY()
        {
            resetCamera();
            //the new orientation
            osg::Matrix3 newOrient = osg::Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);

            osg::ref_ptr<osgGA::CameraManipulator> manipulator = _sceneView->getCameraManipulator();
            osg::Matrixd mat = manipulator->getMatrix();

            //assemble
            mat = osg::Matrixd(newOrient(0, 0), newOrient(0, 1), newOrient(0, 2), 0,
                               newOrient(1, 0), newOrient(1, 1), newOrient(1, 2), 0,
                               newOrient(2, 0), newOrient(2, 1), newOrient(2, 2), 0,
                               abs(mat(3, 0)), abs(mat(3, 2)), abs(mat(3, 1)), 1);
            manipulator->setByMatrix(mat);
        }

        void OMVisViewer::cameraPositionYZ()
        {
            //to get the correct distance of the bodies, reset to home position and use the values of this camera position
            resetCamera();
            //the new orientation
            osg::Matrix3 newOrient = osg::Matrix3(0, 1, 0, 0, 0, 1, 1, 0, 0);

            osg::ref_ptr<osgGA::CameraManipulator> manipulator = _sceneView->getCameraManipulator();
            osg::Matrixd mat = manipulator->getMatrix();

            //assemble
            mat = osg::Matrixd(newOrient(0, 0), newOrient(0, 1), newOrient(0, 2), 0,
                               newOrient(1, 0), newOrient(1, 1), newOrient(1, 2), 0,
                               newOrient(2, 0), newOrient(2, 1), newOrient(2, 2), 0,
                               abs(mat(3, 1)), abs(mat(3, 2)), abs(mat(3, 0)), 1);
            manipulator->setByMatrix(mat);
        }

        void OMVisViewer::cameraPositionXZ()
        {
            //to get the correct distance of the bodies, reset to home position and use the values of this camera position
            resetCamera();
            //the new orientation
            osg::Matrix3 newOrient = osg::Matrix3(1, 0, 0, 0, 0, 1, 0, -1, 0);

            osg::ref_ptr<osgGA::CameraManipulator> manipulator = _sceneView->getCameraManipulator();
            osg::Matrixd mat = manipulator->getMatrix();

            //assemble
            mat = osg::Matrixd(newOrient(0, 0), newOrient(0, 1), newOrient(0, 2), 0,
                               newOrient(1, 0), newOrient(1, 1), newOrient(1, 2), 0,
                               newOrient(2, 0), newOrient(2, 1), newOrient(2, 2), 0,
                               abs(mat(3, 0)), -abs(mat(3, 1)), abs(mat(3, 2)), 1);
            manipulator->setByMatrix(mat);
        }

        void OMVisViewer::resetCamera()
        {
            _sceneView->home();
        }

        void OMVisViewer::aboutOMVis()
        {
            QString information("OMVis - An open source tool for model and simulation visualization.<br><br>"
                                "Copyright (C) 2016 Volker Waurich and Martin Flehmig,<br>"
                                "TU Dresden, Federal Republic of Germany"
                                "<p>Version: 0.01 <br>"
                                "License: GPLv3, see LICENSE file<br>"
                                "Website: https://github.com/vwaurich/OMVis<br></p>"
                                "<p><b>OMVis</b> is a tool to visualize and animate simulation models. "
                                "It is meant to visualize models from different simulation tools. "
                                "What shall be visualized is described in a XML-file and the result files "
                                "provide the corresponding data."
                                "Besides the animation of result files, there is a possibility to animate "
                                "multibody-systems with a FMU interactively.</p>");

            QMessageBox msgBox(QMessageBox::Information, tr("About OMVis"), information, QMessageBox::NoButton);
            msgBox.setStandardButtons(QMessageBox::Close);
            int ret = msgBox.exec();
        }

        void OMVisViewer::help()
        {
            QString information("<p><b>Useful Keyboard Bindings:</b>"
                                "<ul>"
                                "<li> Space bar: Reset the view to home position. </li>"
                                "</ul>"
                                "<p><b>Useful Mouse Bindings:</b>"
                                "<ul>"
                                "<li> Drag and stir: Rotate the model </li>"
                                "<li> Trackball +: Zoom in </li>"
                                "<li> Trackball +: Zoom in </li>"
                                "</ul>"
                                "<p><b>Steering of FMUs with inputs:</b>"
                                "<ol>"
                                "<li> Press Start button </li>"
                                "<li> Left click the scene view (model) </li>"
                                "<li> Press buttons and move joystick device, respectively to steer the model </li>"
                                "</ol>"
                                );
            QMessageBox msgBox(QMessageBox::Information, tr("Help"), information);
            msgBox.setStandardButtons(QMessageBox::Close);
            msgBox.exec();
        }

        /*-----------------------------------------
         * OTHER FUNCTIONS
         *---------------------------------------*/

        void OMVisViewer::resetTimingElements()
        {
            _timeSlider->setSliderPosition(0);
            _timeDisplay->setText(QString("Time [s]: ").append(QString::fromStdString("")));
            _RTFactorDisplay->setText(QString("RT-Factor: ").append(QString::fromStdString("")));
        }

        void OMVisViewer::updateTimingElements()
        {
            updateTimeDisplays();
            updateTimeSliderPosition();
        }

        void OMVisViewer::updateTimeDisplays()
        {
            double visTime = _guiController->getVisTime();
            double rtf = _guiController->getRealTimeFactor();
            _timeDisplay->setText(QString("Time [s]: ").append(QString::number(visTime)));
            _RTFactorDisplay->setText(QString("RT-Factor: ").append(QString::number(rtf)));
        }

        void OMVisViewer::updateTimeSliderPosition()
        {
            int newPos = _guiController->getTimeProgress();
            // Check if newPos in the sliders range. If not give out a warning and reset position.
            if (_timeSlider->minimum() > newPos)
            {
                LOGGER_WRITE(std::string("New position of time slider is out of range! New value ") + std::to_string(newPos)
                             + std::string(" is not in the sliders range [") + std::to_string(_timeSlider->minimum())
                             + std::string(", ") + std::to_string(_timeSlider->maximum()) + std::string("]. Set slider to minimum."),
                             Util::LC_GUI, Util::LL_WARNING);
                newPos = _timeSlider->minimum();
            }
            else if (_timeSlider->maximum() < newPos)
            {
                LOGGER_WRITE(std::string("New position of time slider is out of range! New value ") + std::to_string(newPos)
                             + std::string(" is not in the sliders range [") + std::to_string(_timeSlider->minimum())
                             + std::string(", ") + std::to_string(_timeSlider->maximum()) + std::string("]. Set slider to maximum."),
                             Util::LC_GUI, Util::LL_WARNING);
                newPos = _timeSlider->maximum();
            }

            // Set new position.
            _timeSlider->setSliderPosition(newPos);
        }

    }  // End namespace View
}  // End namespace OMVIS

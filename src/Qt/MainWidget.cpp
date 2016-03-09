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
 OMVis
 MainWidget.cpp
 Purpose: Provides a nice gui

 @author vw
 @version 0.1
 */

#include <Control/GUIController.hpp>
#include "Qt/MainWidget.hpp"
#include "Model/FMU.hpp"
#include "Util/Logger.hpp"
#include "Model/OMVisualizerFMU.hpp"

MainWidget::MainWidget(QWidget* parent, Qt::WindowFlags f, osgViewer::ViewerBase::ThreadingModel threadingModel/*, Model::OMVisualizerAbstract* omv*/)
        : QMainWindow(parent, f),
          _timeDisplay(new QLabel()),
          //X1 _omVisualizer(nullptr),
          _timeSlider(new QSlider(Qt::Horizontal)),
          _osgViewerWidget(),
          _guiController(new Control::GUIController()),
          _modelLoaded(false)
{
    //the names
    setObjectName("MainWindow");
    setWindowTitle("OMVIS - The Open-Source FMU-visualization");

    //the osg threading model
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);

    // Set up the top menu-bar
    setupMenuBar();

    //Set up the osg viewer widget
    //_osgViewerWidget = setupOSGViewerWidget();
    _osgViewerWidget = setupOSGViewerWidgetDefault();

    //Set up the control elements widget
    _controlElementWidget = setupControlElementWidget();

    //Set up the time slider widget
    //X2 _timeSliderWidget = setupTimeSliderWidgetDefault(_timeSlider);
    _timeSliderWidget = setupTimeSliderWidget(0);

    //assemble the layouts
    QVBoxLayout* mainRowLayout = new QVBoxLayout;
    mainRowLayout->addWidget(_osgViewerWidget);
    mainRowLayout->addWidget(_timeSliderWidget);
    mainRowLayout->addWidget(_controlElementWidget);

    //the father of all widget
    QWidget* topWidget = new QWidget;
    topWidget->setLayout(mainRowLayout);
    setCentralWidget(topWidget);

    //to trigger the the paint events which render the view
    QObject::connect(&_renderTimer, SIGNAL(timeout()), this, SLOT(update()));
    _renderTimer.start(10);

    //to trigger the scene updates with the visualization step size
    QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateScene()));
    QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateGUIelements()));
    //MF \todo Move to approriate place _visTimer.start(omv->omvManager->_hVisual * 1000.0);  // we need milliseconds in here
}

void MainWidget::setupMenuBar()
{
    // Menu caption "File"
    _fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* exportAction = _fileMenu->addAction(tr("Export Video"));
    QAction* changeModelAction = _fileMenu->addAction(tr("Load Model"));
    _fileMenu->addSeparator();
    _fileMenu->addAction(tr("&Quit"), this, SLOT(close()));

    QObject::connect(exportAction, SIGNAL(triggered()), this, SLOT(exportVideo()));
    QObject::connect(changeModelAction, SIGNAL(triggered()), this, SLOT(loadModel()));

    // Menu caption "Settings"
    _settingsMenu = menuBar()->addMenu(tr("Settings"));
    QAction* generalSettingsAction = _settingsMenu->addAction(tr("General Settings"));

    QObject::connect(generalSettingsAction, SIGNAL(triggered()), this, SLOT(openDialogSettings()));

    // Menu caption "Inputs"
    _inputMenu = menuBar()->addMenu(tr("Inputs"));
    QAction* mapInputAction = _inputMenu->addAction(tr("Map Input To Devices"));
    QAction* dontCareAction = _inputMenu->addAction(tr("I Don't Care About Input"));

    QObject::connect(mapInputAction, SIGNAL(triggered()), this, SLOT(openDialogInputMapper()));
}

QWidget* MainWidget::setupOSGViewerWidget()
{
    //the osg-viewer widget
    osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);
    //X1 return addViewWidget(window, _omVisualizer->_viewerStuff->_scene._rootNode);

    osg::ref_ptr<osg::Node> rN = _guiController->getSceneRootNode();
    return addViewWidget(window, rN);
}

QWidget* MainWidget::setupOSGViewerWidgetDefault()
{
    //the osg-viewer widget
    osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);
    return addViewWidgetDefault(window);
}

QWidget* MainWidget::setupTimeSliderWidget(const int position)
{
    // \todo This should be done once. Not every time we call this function.
    _timeSlider->setFixedHeight(30);

    //int tP = _guiController->getTimeProgress();
    _timeSlider->setSliderPosition(position);

    LOGGER_WRITE(std::string("min ") + std::to_string(_timeSlider->minimum()) + std::string(" and max ")
                 + std::to_string(_timeSlider->maximum()), Util::LC_GUI, Util::LL_INFO);
    int value = _timeSlider->sliderPosition();

    QObject::connect(_timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVisTimeSlotFunction(int)));
    return _timeSlider;
}

QWidget* MainWidget::setupControlElementWidget()
{
    QPushButton* playButton = new QPushButton("Play", this);
    QPushButton* pauseButton = new QPushButton("Pause", this);
    QPushButton* initButton = new QPushButton("Initialize", this);
    QPushButton* coffeeButton = new QPushButton("Support: Buy us a coffee.", this);

    // Time value of -1.0 indicates, that no model is loaded into OMVis.
    _timeDisplay->setText(QString("Time ").append(QString::fromStdString("-1.0")));
    _timeDisplay->setFixedWidth(60);
    _timeDisplay->setFixedHeight(20);

    //the button row
    QHBoxLayout* buttonRowLayOut = new QHBoxLayout(this);
    QGroupBox* buttonRowBox = new QGroupBox();
    buttonRowLayOut->addWidget(initButton);
    buttonRowLayOut->addWidget(playButton);
    buttonRowLayOut->addWidget(pauseButton);
    buttonRowLayOut->addWidget(coffeeButton);
    buttonRowLayOut->addWidget(_timeDisplay);
    buttonRowBox->setLayout(buttonRowLayOut);
    buttonRowBox->setFixedHeight(60);

    // Connect the buttons to the corresponding slot functions.
    QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(playSlotFunction()));
    QObject::connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseSlotFunction()));
    QObject::connect(initButton, SIGNAL(clicked()), this, SLOT(initSlotFunction()));
    QObject::connect(coffeeButton, SIGNAL(clicked()), this, SLOT(coffeeSlotFunction()));

    return buttonRowBox;
}

QWidget* MainWidget::addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene)
{
    //osgViewer::View* view = new osgViewer::View;
    //X1 osgViewer::View* view = &_omVisualizer->_viewerStuff->_osgViewer;
    osgViewer::View* view = _guiController->getViewer();
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    //MFgw->setTouchEventsEnabled(true);

    return gw->getGLWidget();
}

QWidget* MainWidget::addViewWidgetDefault(osgQt::GraphicsWindowQt* gw)
{
    osgViewer::View* view = new osgViewer::View;
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);

    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    //MFgw->setTouchEventsEnabled(true);

    return gw->getGLWidget();
}

osgQt::GraphicsWindowQt* MainWidget::createGraphicsWindow(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
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


/*-----------------------------------------
 * Slot Functions
 *---------------------------------------*/
void MainWidget::playSlotFunction()
{
    //X1 _omVisualizer->startVisualization();
    _guiController->startVisualization();
}

void MainWidget::pauseSlotFunction()
{
    //X1 _omVisualizer->pauseVisualization();
    _guiController->pauseVisualization();
}

void MainWidget::initSlotFunction()
{
    //X1 _omVisualizer->initVisualization();
    _guiController->initVisualization();
}

void MainWidget::coffeeSlotFunction()
{
    //X1 _omVisualizer->donationVisualization();
    _guiController->donationVisualization();
}

void MainWidget::updateScene()
{
    //X1 _omVisualizer->sceneUpdate();
    _guiController->sceneUpdate();
}

void MainWidget::updateGUIelements()
{
    //X1 _timeDisplay->setText(QString("time ").append(QString::number(_omVisualizer->_omvManager->_visTime)).append(QString(" sec")));
    //X1 _timeSlider->setSliderPosition(_omVisualizer->_omvManager->getTimeProgress());

    double visTime = _guiController->getVisTime();
    int tP = _guiController->getTimeProgress();
    _timeDisplay->setText(QString("time ").append(QString::number(visTime)).append(QString(" sec")));
    _timeSlider->setSliderPosition(tP);
}

void MainWidget::setVisTimeSlotFunction(int val)
{
    //X1 _omVisualizer->_omvManager->_visTime = (_omVisualizer->_omvManager->_endTime - _omVisualizer->_omvManager->_startTime) * (float) (val / 100.0);
    //X1 _omVisualizer->sceneUpdate();

    _guiController->setVisTime(val);
    _guiController->sceneUpdate();
}

void MainWidget::loadModel(/*bool& visFMU*/)
{
    // User has to select model from file. _modelName, _pathName and _visFMU are set.
    QString modelName = modelSelectionDialog();

    // Let create a OMVisualizer object by the GUIController.
    //X1_omVisualizer = _guiController->loadModel(modelName.toStdString());
    _guiController->loadModel(modelName.toStdString());

    _modelLoaded = true;
    LOGGER_WRITE(std::string("The model has been successfully been loaded and initialized."), Util::LC_GUI, Util::LL_INFO);

    _osgViewerWidget = setupOSGViewerWidget();

    // Set value of time display to 0.0
    _timeDisplay->setText(QString("Time ").append(QString::fromStdString("0.0")));

    // Okay, at this point it was trial end error. What do we really need to do in order to show the loaded model??

    // Set the time slider widget to the simulation start time of the loaded model.
    //X2 _timeSliderWidget = setupTimeSliderWidget(_timeSlider);
    int tP = _guiController->getTimeProgress();
    _timeSliderWidget = setupTimeSliderWidget(tP);

    //assemble the layouts
    QVBoxLayout* mainRowLayout = new QVBoxLayout;
    mainRowLayout->addWidget(_osgViewerWidget);
    mainRowLayout->addWidget(_timeSliderWidget);
    mainRowLayout->addWidget(_controlElementWidget);

    //the father of all widget
    QWidget* topWidget = new QWidget;
    topWidget->setLayout(mainRowLayout);
    setCentralWidget(topWidget);
}

QString MainWidget::modelSelectionDialog()
{
    QFileDialog* dialog = new QFileDialog;
    // The user can filter for *.fmu or *.mat files.
    QString fileName = dialog->getOpenFileName(this, tr("Choose a Scene Description File"), QString(), tr("Visualization FMU(*.fmu);; Visualization MAT(*.mat)"));
    return fileName;
}

void MainWidget::exportVideo()
{
    QMessageBox::warning(0, QString("Information"), QString("This functionality might come soon."));
}

void MainWidget::openDialogInputMapper()
{
    // Proceed, if a model is already loaded. Otherwise give the user some hint to load a model first.
    if (!_modelLoaded)
    {

    }
    else
    {
        QMainWindow* inputDialog = new QMainWindow(this);
        inputDialog->setWindowTitle("Input Mapper");

        QWidget* dialogWidget = new QWidget;
        QVBoxLayout* valueLayout = new QVBoxLayout;

        // If a result file is visualized, we can not map keys to input variables.
        //X1 if (_omVisualizer->getDataTypeID() == 0)
        if (_guiController->modelIsMATFile())
        {
            std::cout << "MAT TYPE " << std::endl;
        }
        // FMU
        //X1 else if (_omVisualizer->getDataTypeID() == 1)
        else if (_guiController->modelIsFMU())
        {
//X1            Model::OMVisualizerFMU* fmuData = (Model::OMVisualizerFMU*) _omVisualizer;
//X1            std::cout << "FMU TYPE " << std::endl;
//X1            //real inputs
//            if (fmuData->_inputData._data._numReal > 0)
//            {
//                QLabel* realInputLabel = new QLabel("all real inputs");
//                valueLayout->addWidget(realInputLabel);
//            }
//            for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numReal; ++inputIdx)
//            {
//                std::string var = fmuData->_inputData._data._namesReal.at(inputIdx);
//
//                QHBoxLayout* inputRow = createInputMapperRow(inputIdx, var, "real");
//                valueLayout->addLayout(inputRow);
//            }
//            //bool inputs
//            if (fmuData->_inputData._data._numBoolean > 0)
//            {
//                QLabel* realInputLabel = new QLabel("all bool inputs");
//                valueLayout->addWidget(realInputLabel);
//            }
//            for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numBoolean; ++inputIdx)
//            {
//                QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data._namesBool.at(inputIdx), "bool");
//                valueLayout->addLayout(inputRow);
//            }
//            //integer inputs
//            if (fmuData->_inputData._data._numInteger > 0)
//            {
//                QLabel* realInputLabel = new QLabel("all integer inputs");
//                valueLayout->addWidget(realInputLabel);
//            }
//            for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numInteger; ++inputIdx)
//            {
//                QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data._namesInteger.at(inputIdx), "integer");
//                valueLayout->addLayout(inputRow);
//            }
//            //string inputs
//            for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numString; ++inputIdx)
//            {
//                QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data._namesString.at(inputIdx), "string");
//                valueLayout->addLayout(inputRow);
//            }
        }
        else
        {
            std::cout << "UNKNOWN TYPE " << std::endl;
        }

        dialogWidget->setLayout(valueLayout);
        inputDialog->setCentralWidget(dialogWidget);

        inputDialog->show();
    }
}

QHBoxLayout* MainWidget::createInputMapperRow(int inputIdx, std::string varName, std::string type)
{
    QHBoxLayout* inputRow = new QHBoxLayout;
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
    {
        inputText->addItem("IMPLEMENT ME", "IMPLEMENT ME");
    }

    inputText->setMaximumHeight(20);
    inputLabel->setMaximumHeight(20);
    inputRow->addWidget(inputLabel);
    inputRow->addWidget(varLabel);
    inputRow->addWidget(typeLabel);
    inputRow->addWidget(inputText);
    return inputRow;
}

void MainWidget::openDialogSettings()
{
    QComboBox* bgcDropDownList = new QComboBox();

    //use a QStringList here
    bgcDropDownList->addItem("midnight blue", "KEY_A");
    bgcDropDownList->addItem("lovely lila", "KEY_B");
    bgcDropDownList->addItem("ivory white", "KEY_C");
    bgcDropDownList->addItem("froggy green", "KEY_D");
    bgcDropDownList->setMaximumHeight(20);

    QVBoxLayout* settingRowsLayout = new QVBoxLayout();
    QHBoxLayout* backgroundColourRow = new QHBoxLayout();
    QLabel* bgcLabel = new QLabel(QString("Set the background color to "));
    backgroundColourRow->addWidget(bgcLabel);
    backgroundColourRow->addWidget(bgcDropDownList);
    settingRowsLayout->addLayout(backgroundColourRow);

    QObject::connect(bgcDropDownList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBGColourInOSGViewer(int)));

    QWidget* dialogWidget = new QWidget();
    dialogWidget->setLayout(settingRowsLayout);
    QMainWindow* inputDialog = new QMainWindow(this);
    inputDialog->setWindowTitle("General Settings");
    inputDialog->setCentralWidget(dialogWidget);

    inputDialog->show();
}

void MainWidget::changeBGColourInOSGViewer(const int colorIdx)
{
    //osg::Vec4 colVec = osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    osg::Vec4 colVec(0.0f, 0.0f, 0.0f, 0.0f);

    LOGGER_WRITE(std::string("Color Idx ") + std::to_string(colorIdx), Util::LC_GUI, Util::LL_INFO);
    switch (colorIdx)
    {
        case 0:
            colVec = osg::Vec4(0.0f, 0.0f, 100.0f, 1.0f);
            break;  //blue
        case 1:
            colVec = osg::Vec4(150.0f, 0.0f, 200.0f, 10.0f);
            break;  //lila
        case 2:
            colVec = osg::Vec4(255.0f, 255.0f, 255.0f, 0.0f);
            break;  //white
        case 3:
            colVec = osg::Vec4(0.0f, 200.0f, 0.0f, 0.0f);
            break;  //green
        default:
            colVec = osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f);
            break;  //black
    }
    //X1 _omVisualizer->_viewerStuff->_osgViewer.getCamera()->setClearColor(colVec);
    _guiController->setBackgroundColor(colVec);
}

void MainWidget::updateKeyMapValue(QString varToKey)
{
//X1    const int lengthOfTypeStr = 4;
//X1    Model::OMVisualizerFMU* fmuVisualizer = (Model::OMVisualizerFMU*) _omVisualizer;
//X1
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

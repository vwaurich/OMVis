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
#include <osgDB/ReadFile>
#include <QScreen>
#include <assert.h>

OMVisViewer::OMVisViewer(/*QWidget* parent, Qt::WindowFlags f,*/osgViewer::ViewerBase::ThreadingModel threadingModel)
        : QMainWindow(/*parent, f*/),
          osgViewer::CompositeViewer(),
          _timeDisplay(new QLabel()),
          _guiController(new Control::GUIController()),
          _modelLoaded(false),
          _view(new osgViewer::View())
{
    //the names
    setObjectName("MainWindow");
    setWindowTitle("OMVIS - The Open-Source FMU-visualization");

    //the osg threading model
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);

    // Create the widgets for time slider, osg viewer and control elements.
    setupWidgets();

    // Set up the top menu-bar
    createActions();
    createMenuBar();

    // Assemble the widgets to a layout and create the father of all widgets with this layout.
    createLayout();

    //to trigger the the paint events which render the view
    QObject::connect(&_renderTimer, SIGNAL(timeout()), this, SLOT(update()));
    _renderTimer.start(10);

    //to trigger the scene updates with the visualization step size
    QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateScene()));
    QObject::connect(&_visTimer, SIGNAL(timeout()), this, SLOT(updateGUIelements()));
    //MF \todo Move to appropriate place _visTimer.start(omv->omvManager->_hVisual * 1000.0);  // we need milliseconds in here

    resize(QGuiApplication::primaryScreen()->availableSize() * 0.5);
}

void OMVisViewer::setupWidgets()
{
    //X8 We have a initial scene:
    osg::Node* scene = osgDB::readNodeFile("teapot.osg");

    // Set up the osg viewer widget
    _osgViewerWidget = setupOSGViewerWidgetDefault(scene);

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
    _openAct = new QAction(tr("&Open..."), this);
    _openAct->setShortcut(tr("Ctrl+O"));
    QObject::connect(_openAct, SIGNAL(triggered()), this, SLOT(loadModel()));

    //X9
    _loadCowAct = new QAction(tr("Load Cow"), this);
    QObject::connect(_loadCowAct, SIGNAL(triggered()), this, SLOT(loadModelCow()));
    _loadCessnaAct = new QAction(tr("Load Cessna"), this);
    QObject::connect(_loadCessnaAct, SIGNAL(triggered()), this, SLOT(loadModelCessna()));
    _unloadAct = new QAction(tr("Remove Current Model"), this);
    QObject::connect(_unloadAct, SIGNAL(triggered()), this, SLOT(unloadModel()));

    _exportAct = new QAction(tr("Export Video"), this);
    QObject::connect(_exportAct, SIGNAL(triggered()), this, SLOT(exportVideo()));

    _exitAct = new QAction(tr("&Quit"), this);
    _exitAct->setShortcut(tr("Ctrl+Q"));
    QObject::connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    _genSetAct = new QAction(tr("General Settings"), this);
    QObject::connect(_genSetAct, SIGNAL(triggered()), this, SLOT(openDialogSettings()));

    _mapInputAct = new QAction(tr("Map Inputs"), this);
    QObject::connect(_mapInputAct, SIGNAL(triggered()), this, SLOT(openDialogInputMapper()));

    _dontCareAct = new QAction(tr("I Don't Care About Inputs"), this);
    //QObject::connect(_dontCareAct, SIGNAL(triggered()), this, SLOT());

    _aboutOMVisAct = new QAction(tr("About OMVis"), this);
    QObject::connect(_aboutOMVisAct, SIGNAL(triggered()), this, SLOT(aboutOMVis()));
}

void OMVisViewer::createMenuBar()
{
    // Menu caption "File"
    _fileMenu = new QMenu(tr("&File"), this);
    _fileMenu->addAction(_openAct);

    //X9
    _fileMenu->addAction(_loadCowAct);
    _fileMenu->addAction(_loadCessnaAct);
    _fileMenu->addAction(_unloadAct);

    _fileMenu->addAction(_exportAct);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAct);

    _settingsMenu = new QMenu(tr("Settings"), this);
    _settingsMenu->addAction(_genSetAct);

    _inputMenu = new QMenu(tr("Inputs"), this);
    _inputMenu->addAction(_mapInputAct);
    _inputMenu->addAction(_dontCareAct);

    _helpMenu = new QMenu(tr("&Help"), this);
    _helpMenu->addAction(_aboutOMVisAct);

    menuBar()->addMenu(_fileMenu);
    menuBar()->addMenu(_settingsMenu);
    menuBar()->addMenu(_inputMenu);
    menuBar()->addMenu(_helpMenu);
}

QWidget* OMVisViewer::setupOSGViewerWidgetDefault(osg::Node* scene)
{
    //the osg-viewer widget
    osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);

    if (scene == nullptr)
        std::cout << "Something went wrong loading the osg file." << std::endl;
    return addViewWidgetDefault(window, scene);

    //QGroupBox* buttonRowBox = new QGroupBox();
    //return buttonRowBox;
}

QWidget* OMVisViewer::addViewWidgetDefault(osgQt::GraphicsWindowQt* gw, osg::Node* scene)
{
//X9    osgViewer::View* view = new osgViewer::View();
//X9    addView(view);
    if (_view == nullptr)
        _view = new osgViewer::View();
    addView(_view);

    osg::Camera* camera = _view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);

    _view->setSceneData(scene);
    _view->addEventHandler(new osgViewer::StatsHandler);
    _view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    gw->setTouchEventsEnabled(true);

    return gw->getGLWidget();
}

osgQt::GraphicsWindowQt* OMVisViewer::createGraphicsWindow(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
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

void OMVisViewer::setTimeSliderPosition(int newPos)
{
    // Check if newPos in the sliders range. If not give out a warning and reset position.
    if (_timeSlider->minimum() > newPos)
    {
        LOGGER_WRITE(std::string("New position of time slider is out of range! New value ") + std::to_string(newPos) + std::string(" is not in the sliders range [") + std::to_string(_timeSlider->minimum()) + std::string(", ") + std::to_string(_timeSlider->maximum()) + std::string("]. Set slider to minimum."), Util::LC_GUI, Util::LL_WARNING);
        newPos = _timeSlider->minimum();
    }
    else if (_timeSlider->maximum() < newPos)
    {
        LOGGER_WRITE(std::string("New position of time slider is out of range! New value ") + std::to_string(newPos) + std::string(" is not in the sliders range [") + std::to_string(_timeSlider->minimum()) + std::string(", ") + std::to_string(_timeSlider->maximum()) + std::string("]. Set slider to maximum."), Util::LC_GUI, Util::LL_WARNING);
        newPos = _timeSlider->maximum();
    }

    // Set new position.
    _timeSlider->setSliderPosition(newPos);
}

QWidget* OMVisViewer::setupTimeSliderWidget()
{
    _timeSlider = new QSlider(Qt::Horizontal, this);
    _timeSlider->setFixedHeight(30);
    _timeSlider->setMinimum(0);
    _timeSlider->setMaximum(99);
    _timeSlider->setSliderPosition(0);

    LOGGER_WRITE(std::string("min ") + std::to_string(_timeSlider->minimum()) + std::string(" and max ") + std::to_string(_timeSlider->maximum()), Util::LC_GUI, Util::LL_INFO);

    QObject::connect(_timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVisTimeSlotFunction(int)));
    return _timeSlider;
}

QWidget* OMVisViewer::setupControlElementWidget()
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
    QHBoxLayout* buttonRowLayOut = new QHBoxLayout();
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

/*-----------------------------------------
 * Slot Functions
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

void OMVisViewer::coffeeSlotFunction()
{
    _guiController->donationVisualization();
}

void OMVisViewer::updateScene()
{
    _guiController->sceneUpdate();
}

void OMVisViewer::updateGUIelements()
{
    double visTime = _guiController->getVisTime();
    int tP = _guiController->getTimeProgress();
    _timeDisplay->setText(QString("time ").append(QString::number(visTime)).append(QString(" sec")));
    _timeSlider->setSliderPosition(tP);
}

void OMVisViewer::setVisTimeSlotFunction(int val)
{
    _guiController->setVisTime(val);
    _guiController->sceneUpdate();
}

void OMVisViewer::loadModel()
{
    // User has to select model from file. _modelName, _pathName and _visFMU are set.
    QString modelName = modelSelectionDialog();

    // Let create a OMVisualizer object by the GUIController.
    _guiController->loadModel(modelName.toStdString());

    _modelLoaded = true;
    LOGGER_WRITE(std::string("The model has been successfully been loaded and initialized."), Util::LC_GUI, Util::LL_INFO);

    //X9 osg::Node* scene = osgDB::readNodeFile("cessna.osg");

    // Set up the osg viewer widget
    osg::Node* scene = _guiController->getSceneRootNode();
    _view->setSceneData(scene);

    LOGGER_WRITE(std::string("OSGViewUpdated"), Util::LC_LOADER, Util::LL_WARNING);

    // Set value of time display to simulation start time of the model.
    double startT = _guiController->getSimulationStartTime();
    _timeDisplay->setText(QString("Time ").append(QString::number(startT)));

    // Set the time slider to a position according to the simulation start time of the loaded model.
    int tP = _guiController->getTimeProgress();
    setTimeSliderPosition(0);

    LOGGER_WRITE(std::string("OSGViewUpdated2"), Util::LC_LOADER, Util::LL_WARNING);
}

void OMVisViewer::loadModelCessna()
{
    LOGGER_WRITE(std::string("Hier, lade die Cessna!"), Util::LC_LOADER, Util::LL_INFO);
    osg::Node* scene = osgDB::readNodeFile("cessna.osg");
    if (scene == nullptr)
            LOGGER_WRITE(std::string("Uiuiui, das Model cessna.osg konnte nicht geladen werden. Die Szene ist leer. Liegt die Datei cessna.osg im aktuellen Verzeichnis? "), Util::LC_LOADER, Util::LL_ERROR);
    _view->setSceneData(scene);
    _modelLoaded = true;
    LOGGER_WRITE(std::string("Dort, Cessna geladen!"), Util::LC_LOADER, Util::LL_INFO);
}

void OMVisViewer::loadModelCow()
{
    LOGGER_WRITE(std::string("Hier, lade die Kuh!"), Util::LC_LOADER, Util::LL_INFO);
    osg::Node* scene = osgDB::readNodeFile("cow.osg");
    if (scene == nullptr)
        LOGGER_WRITE(std::string("Uiuiui, das Model cow.osg konnte nicht geladen werden. Die Szene ist leer. Liegt die Datei cow.osg im aktuellen Verzeichnis?"), Util::LC_LOADER, Util::LL_ERROR);
    _view->setSceneData(scene);
    _modelLoaded = true;
    LOGGER_WRITE(std::string("Dort, Kuh geladen!"), Util::LC_LOADER, Util::LL_INFO);
}

void OMVisViewer::unloadModel()
{
    LOGGER_WRITE(std::string("Hier, entlade das aktuelle Model!"), Util::LC_LOADER, Util::LL_INFO);
    _view->setSceneData(new osg::Node() );
    _modelLoaded = false;
    LOGGER_WRITE(std::string("Dort, kein Model geladen!"), Util::LC_LOADER, Util::LL_INFO);
}

QString OMVisViewer::modelSelectionDialog()
{
    QFileDialog* dialog = new QFileDialog;
    // The user can filter for *.fmu or *.mat files.
    QString fileName = dialog->getOpenFileName(this, tr("Choose a Scene Description File"), QString(), tr("Visualization FMU(*.fmu);; Visualization MAT(*.mat)"));
    return fileName;
}

void OMVisViewer::exportVideo()
{
    QMessageBox::warning(0, QString("Information"), QString("This functionality might come soon."));
}

void OMVisViewer::openDialogInputMapper()
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

QHBoxLayout* OMVisViewer::createInputMapperRow(int inputIdx, std::string varName, std::string type)
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

void OMVisViewer::openDialogSettings()
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

void OMVisViewer::changeBGColourInOSGViewer(const int colorIdx)
{
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
    _guiController->setBackgroundColor(colVec);
}

void OMVisViewer::updateKeyMapValue(QString varToKey)
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

void OMVisViewer::aboutOMVis()
{
    QMessageBox::about(this, tr("About OMVis"), tr("<p><b>OMVis</b> is a tool to visualize and animate simulation models. "
                                                   "It is meant to visualize models from different simulation tools. "
                                                   "What shall be visualized is described in a XML-file and the result files "
                                                   "provide the corresponding data."
                                                   "Besides the animation of result files, there is a possibility to animate "
                                                   "multibody-systems with a FMU interactively.</p>"
                                                   "<p>Version: 0.01 "
                                                   "Website: https://github.com/vwaurich/OMVis"
                                                   "(c) Copyright: See license text."
                                                   "Authors: Volker Waurich, Martin Flehmig</p>"));
}





///** Updates the OSGViewerWidget if a new model was loaded into OMVis.
// *
// * We assume, that the _osgViewerWidget != nullptr, i.e., it points to a already created Widget.
// * Thus, we only need to update this Window content with the new model information.
// *
// * @return Pointer to the updated Widget object.
// */
////QWidget*
//void OMVisViewer::updateOSGViewerWidget()
//{
//    //X5 das muss glaube ich nicht nochmal gemacht werden. _osgViewerWidget zeigt gerade auf das in
//    //   setupOSGViewerWidgeDefault mit New erzeugte osgQt::GraphicsWindowQt Objekt.
//    //osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);
//
//    // Get new root node from model data.
//    osg::ref_ptr<osg::Node> rN = _guiController->getSceneRootNode();
//
//    // Muessen wir den view ersetzen oder nur die szene updaten?? --> removeView??
//    // Auf der anderen Seite: Der view ist ja schon da, also pruefen ob der view schon hinzugefuegt wurde
//    // und dann benutzen, sonst anlegen
//    osgViewer::View* view = _guiController->getViewer();
//    if (view == nullptr)
//        LOGGER_WRITE(std::string("Hey, your osg::View is a nullptr!"), Util::LC_LOADER, Util::LL_WARNING);
//    else
//    {
//        removeView(view);
////        osg::Node* scene = osgDB::readNodeFile("cessna.osg");
////        if (scene == nullptr)
////            std::cout << "Something went wrong loading the osg file." << std::endl;
////        view->setSceneData(scene);
//    }
//    _osgViewerWidget->update();
//    this->update();
//    LOGGER_WRITE(std::string("OSGViewUpdated"), Util::LC_LOADER, Util::LL_WARNING);
//    //    osgViewer::View* view = _guiController->getViewer();
//    //addView(view);
//    //osg::Camera* camera = view->getCamera();
//    //camera->setGraphicsContext(gw);
//
//    //const osg::GraphicsContext::Traits* traits = gw->getTraits();
////
////    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
////    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
////    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
////
////    view->setSceneData(scene);
////    view->addEventHandler(new osgViewer::StatsHandler);
////    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
//    //MFgw->setTouchEventsEnabled(true);
//
////    return _osgViewerWidget->getGLWidget();
//}

//QWidget* OMVisViewer::setupOSGViewerWidget()
//{
//    //the osg-viewer widget
//    //X5 das muss glaube ich nicht nochmal gemacht werden. _osgViewerWidget zeigt gerade auf das in
//    //   setupOSGViewerWidgeDefault mit New erzeugte osgQt::GraphicsWindowQt Objekt.
//    osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);
//
//    osg::ref_ptr<osg::Node> rN = _guiController->getSceneRootNode();
//    return addViewWidget(window, rN);
//}
//
//QWidget* OMVisViewer::addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene)
//{
//    osgViewer::View* view = _guiController->getViewer();
//    addView(view);
//
//    osg::Camera* camera = view->getCamera();
//    camera->setGraphicsContext(gw);
//
//    const osg::GraphicsContext::Traits* traits = gw->getTraits();
//
//    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
//    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
//    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
//
//    view->setSceneData(scene);
//    view->addEventHandler(new osgViewer::StatsHandler);
//    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
//    //MFgw->setTouchEventsEnabled(true);
//
//    return gw->getGLWidget();
//}

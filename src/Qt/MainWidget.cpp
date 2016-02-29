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

#include "Qt/MainWidget.hpp"

MainWidget::MainWidget(QWidget* parent, Qt::WindowFlags f, osgViewer::ViewerBase::ThreadingModel threadingModel, Model::OMVisualizerAbstract* omv)
        : QMainWindow(parent, f),
          _timeDisplay(new QLabel),
          _omVisualizer(omv),
          _timeSlider(new QSlider(Qt::Horizontal))

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
    osgViewerWidget = setupOSGViewerWidget();

    //Set up the control elements widget
    controlElementWidget = setupControlElementWidget();

    //Set up the time slider widget
    timeSliderWidget = setupTimeSliderWidget(_timeSlider);

    //assemble the layouts
    QVBoxLayout* mainRowLayout = new QVBoxLayout;
    mainRowLayout->addWidget(osgViewerWidget);
    mainRowLayout->addWidget(timeSliderWidget);
    mainRowLayout->addWidget(controlElementWidget);

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
    _visTimer.start(omv->omvManager->_hVisual * 1000.0);  // we need milliseconds in here
}

void MainWidget::setupMenuBar()
{
    //menu caption "File"
    fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* exportAction = fileMenu->addAction(tr("Export Video"));
    QAction* changeModelAction = fileMenu->addAction(tr("Change Model"));
    QAction* somethingAction = fileMenu->addAction(tr("Do Something"));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Quit"), this, SLOT(close()));

    QObject::connect(changeModelAction, SIGNAL(triggered()), this, SLOT(openFileDialogGetFileName()));

    //menu caption "Settings"
    settingsMenu = menuBar()->addMenu(tr("Settings"));
    QAction* generalSettingsAction = settingsMenu->addAction(tr("General Settings"));

    QObject::connect(generalSettingsAction, SIGNAL(triggered()), this, SLOT(openDialogSettings()));

    //menu caption "Inputs"
    inputMenu = menuBar()->addMenu(tr("Inputs"));
    QAction* mapInputAction = inputMenu->addAction(tr("Map Input To Devices"));
    QAction* dontCareAction = inputMenu->addAction(tr("I Don't Care About Input"));

    QObject::connect(mapInputAction, SIGNAL(triggered()), this, SLOT(openDialogInputMapper()));
}

QWidget* MainWidget::setupOSGViewerWidget()
{
    //the osg-viewer widget
    osgQt::GraphicsWindowQt* window = createGraphicsWindow(0, 0, 100, 100);
    return addViewWidget(window, _omVisualizer->_viewerStuff->_scene._rootNode);
}

QWidget* MainWidget::setupTimeSliderWidget(QSlider* timeSlider)
{
    //the slider row
    int value = -1;
    timeSlider->setFixedHeight(30);
    timeSlider->setSliderPosition(_omVisualizer->omvManager->getTimeProgress());
    std::cout << "min " << timeSlider->minimum() << " max " << timeSlider->maximum() << std::endl;
    value = timeSlider->sliderPosition();

    QObject::connect(timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVisTimeSlotFunction(int)));
    return timeSlider;
}

QWidget* MainWidget::setupControlElementWidget()
{
    //the button row
    QHBoxLayout* buttonRowLayOut = new QHBoxLayout;
    QGroupBox* buttonRowBox = new QGroupBox;

    QPushButton* playButton = new QPushButton;
    playButton->setText("PLAY");

    QPushButton* pauseButton = new QPushButton;
    pauseButton->setText("PAUSE");

    QPushButton* initButton = new QPushButton;
    initButton->setText("INITIAL");

    QPushButton* coffeeButton = new QPushButton;
    coffeeButton->setText("Buy us a coffee");

    _timeDisplay->setText(QString("time ").append(QString::number(_omVisualizer->omvManager->_visTime)));
    _timeDisplay->setFixedWidth(70);
    _timeDisplay->setFixedHeight(20);

    buttonRowLayOut->addWidget(initButton);
    buttonRowLayOut->addWidget(playButton);
    buttonRowLayOut->addWidget(pauseButton);
    buttonRowLayOut->addWidget(coffeeButton);
    buttonRowLayOut->addWidget(_timeDisplay);
    buttonRowBox->setLayout(buttonRowLayOut);

    buttonRowBox->setFixedHeight(50);

    //the button connections
    QObject::connect(playButton, SIGNAL(clicked()), this, SLOT(playSlotFunction()));
    QObject::connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseSlotFunction()));
    QObject::connect(initButton, SIGNAL(clicked()), this, SLOT(initSlotFunction()));
    QObject::connect(coffeeButton, SIGNAL(clicked()), this, SLOT(coffeeSlotFunction()));

    return buttonRowBox;
}

QWidget* MainWidget::addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene)
{
    //osgViewer::View* view = new osgViewer::View;
    osgViewer::View* view = &_omVisualizer->_viewerStuff->_viewer;
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

void MainWidget::playSlotFunction()
{
    _omVisualizer->startVisualization();
}

void MainWidget::pauseSlotFunction()
{
    _omVisualizer->pauseVisualization();
}

void MainWidget::initSlotFunction()
{
    _omVisualizer->initVisualization();
}

void MainWidget::coffeeSlotFunction()
{
    _omVisualizer->donationVisualization();
}

void MainWidget::updateScene()
{
    _omVisualizer->sceneUpdate();
}

void MainWidget::updateGUIelements()
{
    _timeDisplay->setText(QString("time ").append(QString::number(_omVisualizer->omvManager->_visTime)).append(QString(" sec")));
    _timeSlider->setSliderPosition(_omVisualizer->omvManager->getTimeProgress());

}

void MainWidget::setVisTimeSlotFunction(int val)
{
    _omVisualizer->omvManager->_visTime = (_omVisualizer->omvManager->_endTime - _omVisualizer->omvManager->_startTime) * (float) (val / 100.0);
    _omVisualizer->sceneUpdate();

}

void MainWidget::openFileDialogGetFileName()
{
    QFileDialog* dialog = new QFileDialog;

    QString fileName = dialog->getOpenFileName(this, tr("Choose A Scene Description File"), QString(), tr("Visualization XML(*_visual.xml)"));
    std::string fileNameStd = fileName.toStdString();
    std::size_t pos = fileNameStd.find_last_of("/");
    std::string pathName = fileNameStd.substr(0, pos + 1);
    std::string modelName = fileNameStd.substr(pos + 1, fileNameStd.length());
    pos = modelName.find("_visual.xml");
    modelName = modelName.substr(0, pos);

    std::cout << "FILENAME " << pathName << " MODELNAME " << modelName << std::endl;
    std::cout << "IMPLEMENT SOMETHING THAT LOADS THE NEW MODEL AND INTIALIZES THE SCENE!!!" << std::endl;
}

void MainWidget::openDialogInputMapper()
{
    QMainWindow* inputDialog = new QMainWindow(this);
    inputDialog->setWindowTitle("Input Mapper");

    QWidget* dialogWidget = new QWidget;
	QVBoxLayout* valueLayout = new QVBoxLayout;

	if (_omVisualizer->getDataTypeID() == 0)
	{
		std::cout << "MAT TYPE " << std::endl;
	}
	else if (_omVisualizer->getDataTypeID() == 1)
	{
		Model::OMVisualizerFMU* fmuData = (Model::OMVisualizerFMU*)_omVisualizer;
		std::cout << "FMU TYPE " << std::endl;
		//real inputs 
		if (fmuData->_inputData._data._numReal > 0)
		{
			QLabel* realInputLabel = new QLabel("all real inputs");
			valueLayout->addWidget(realInputLabel);
		}
		for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numReal; inputIdx++)
		{
			std::string var = fmuData->_inputData._data.namesReal.at(inputIdx);

			QHBoxLayout* inputRow = createInputMapperRow(inputIdx,var,"real");
			valueLayout->addLayout(inputRow);
		}
		//bool inputs
		if (fmuData->_inputData._data._numBoolean > 0)
		{
			QLabel* realInputLabel = new QLabel("all bool inputs");
			valueLayout->addWidget(realInputLabel);
		}
		for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numBoolean; inputIdx++)
		{
			QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data.namesBool.at(inputIdx), "bool");
			valueLayout->addLayout(inputRow);
		}
		//integer inputs
		if (fmuData->_inputData._data._numInteger > 0)
		{
			QLabel* realInputLabel = new QLabel("all integer inputs");
			valueLayout->addWidget(realInputLabel);
		}
		for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numInteger; inputIdx++)
		{
			QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data.namesInteger.at(inputIdx), "integer");
			valueLayout->addLayout(inputRow);
		}
		//string inputs 
		for (uint inputIdx = 0; inputIdx < fmuData->_inputData._data._numString; inputIdx++)
		{
			QHBoxLayout* inputRow = createInputMapperRow(inputIdx, fmuData->_inputData._data.namesString.at(inputIdx), "string");
			valueLayout->addLayout(inputRow);
		}
	}
	else
	{
		std::cout << "UNKNOWN TYPE " << std::endl;
	}

    dialogWidget->setLayout(valueLayout);
    inputDialog->setCentralWidget(dialogWidget);

    inputDialog->show();
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
		inputText->addItem("KEY_A", "KEY_A");
		inputText->addItem("KEY_B", "KEY_B");
		inputText->addItem("KEY_C", "KEY_C");
		inputText->addItem("KEY_D", "KEY_D");
	}
	else if (type == "real")
	{
		inputText->addItem("JOY1_X", "JOY1_X");
		inputText->addItem("JOY1_Y", "JOY1_Y");
		inputText->addItem("JOY2_X", "JOY2_X");
		inputText->addItem("JOY2_Y", "JOY2_Y");
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
    QMainWindow* inputDialog = new QMainWindow(this);
    inputDialog->setWindowTitle("Settings");

    QWidget* dialogWidget = new QWidget;

    const int numOfInputs = 3;

    QVBoxLayout* settingRowsLayout = new QVBoxLayout;

    QHBoxLayout* backgroundColourRow = new QHBoxLayout;
    QLabel* bgcLabel = new QLabel(QString("background colour "));
    QComboBox* bgcDropDownList = new QComboBox();

    //use a QStringList here
    bgcDropDownList->addItem("midnight blue", "KEY_A");
    bgcDropDownList->addItem("lovely lila", "KEY_B");
    bgcDropDownList->addItem("ivory white", "KEY_C");
    bgcDropDownList->addItem("froggy green", "KEY_D");

    bgcDropDownList->setMaximumHeight(20);
    bgcDropDownList->setMaximumHeight(20);
    backgroundColourRow->addWidget(bgcLabel);
    backgroundColourRow->addWidget(bgcDropDownList);
    settingRowsLayout->addLayout(backgroundColourRow);

    QObject::connect(bgcDropDownList, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBGColourInOSGViewer(int)));

    dialogWidget->setLayout(settingRowsLayout);
    inputDialog->setCentralWidget(dialogWidget);

    inputDialog->show();
}

void MainWidget::changeBGColourInOSGViewer(int colorIdx)
{
    osg::Vec4 colVec = osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f);

    std::cout << "colorIdx " << colorIdx << std::endl;
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
    _omVisualizer->_viewerStuff->_viewer.getCamera()->setClearColor(colVec);
}

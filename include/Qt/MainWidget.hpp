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
 MainWidget.hpp
 Purpose: Provides a nice gui

 @author vw
 @version 0.1
 */

#ifndef INCLUDE_MAINWIDGET_HPP_
#define INCLUDE_MAINWIDGET_HPP_

#include <QTimer>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>
#include <QList>
#include <QPushButton>
#include <QGroupBox>
#include <QObject>
#include <QLabel>
#include <QAction>
#include <QSlider>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QComboBox>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/EventQueue>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

#include "Model/OMVisualizerAbstract.hpp"
#include "Model/OMVisualizerFMU.hpp"

class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

class MainWidget : public QMainWindow, public osgViewer::CompositeViewer
//If we want to use a toolbar, we have to derive from QMainWindow.
{
Q_OBJECT
 public:
    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *inputMenu;
    QWidget* osgViewerWidget;
    QWidget* controlElementWidget;
    QWidget* timeSliderWidget;

    /*! \brief Constructs MainWidget object from arguments.
     *
     * @param parent Parent for initialization of QWidget Baseclass.
     * @param flags Window flags for initialization of QWidget Baseclass.
     * @param threadingModel The threading model.
     * @param omv The OMVisualizer which stores the scene.
     */
    MainWidget(QWidget* parent = 0, Qt::WindowFlags f = 0, osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::CompositeViewer::SingleThreaded, Model::OMVisualizerAbstract* omv = NULL);

    /*! \brief Adds a view-widget to the main-widget
     *
     * @param gw The graphical window.
     * @param scene The osg scene.
     * @return The widget for the osg-viewer.
     */
    QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw, osg::ref_ptr<osg::Node> scene);

    /*! \brief Creates a osgQt::createGraphicsWindow
     *
     * @param x
     * @param y
     * @param w The width.
     * @param h The height.
     * @param name The name of the window.
     * @param windowDecoration
     * @return The osgQt::GraphicsWindowQt - window.
     */
    osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h, const std::string& name = "", bool windowDecoration = false);

    /*! \brief Renders the widget in case of a paint event
     */
    virtual void paintEvent(QPaintEvent* event)
    {
        frame();
    }

    /*! \brief Creates the top menu.
     */
    void setupMenuBar();

    /*! \brief Creates the osg-viewer-widget
     */
    QWidget* setupOSGViewerWidget();

    /*! \brief Creates the control widget (play, pause,...)
     */
    QWidget* setupControlElementWidget();

    /*! \brief Creates the time slider widget
     */
	QWidget* setupTimeSliderWidget(QSlider*);

	/*! \brief Creates a gui elemnt to set the mapping for a input value.
	*/
	QHBoxLayout* createInputMapperRow(int inputIdx, std::string varName, std::string type);

 public slots:
    /*! \brief Function that is triggered by the play-button.
     */
    void playSlotFunction();

    /*! \brief Function that is triggered by the pause-button.
     */
    void pauseSlotFunction();

    /*! \brief Function that is triggered by the init-button.
     */
    void initSlotFunction();

    /*! \brief Function that is triggered by the coffee-button.
     */
    void coffeeSlotFunction();

    /*! \brief Function that is triggered the scene-update timer.
     */
    void updateScene();

    /*! \brief Function that is triggered the scene-update timer.
     */
    void updateGUIelements();

    /*! \brief Function that is triggered the scene-update timer.
     */
    void setVisTimeSlotFunction(int val);

    /*! \brief Function that opens a file dialog
     */
    void openFileDialogGetFileName();

	/*! \brief Function that opens the input mapper dialog
	*/
	void openDialogInputMapper();

	/*! \brief Function that opens the settings dialog
	*/
	void openDialogSettings();

	/*! \brief Changes the Background colour in the osg::viewer
	*/		
	void changeBGColourInOSGViewer(int colorIdx);

	/*! \brief Updates the key-input-map
	*/
	void updateKeyMapValue(QString key);

 protected:
    QSlider* _timeSlider;
    QLabel* _timeDisplay;
    QTimer _renderTimer;  //!< Brief Triggers a new frame.
    QTimer _visTimer;  //!< Brief Triggers a new scene-update.
    Model::OMVisualizerAbstract* _omVisualizer;

};

#endif /* INCLUDE_MAINWIDGET_HPP_ */

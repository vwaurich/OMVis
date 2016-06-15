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

#ifndef INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_

#include <QDialog>
#include <QLineEdit>

QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace OMVIS
{
    namespace View
    {
        /*! \brief Class that encapsulates the dialog to specify the simulation settings.
         *
         */
        class SimSettingDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*! \brief Constructor which creates the Qt dialog window to set simulation settings.
             *
             */
			 SimSettingDialog(QWidget* parent = Q_NULLPTR);

         private:
            /*! Qt combo box which provides the simsettings to set. */
            QComboBox* _solverMethodBox;
			QLineEdit* _simStepSize;
			QLineEdit* _visStepSize;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_ */

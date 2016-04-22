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

#ifndef INCLUDE_VIEW_SETTINGSDIALOGS_HPP_
#define INCLUDE_VIEW_SETTINGSDIALOGS_HPP_

#include "View/ViewSettings.hpp"

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace OMVIS
{

    namespace View
    {

        /*! \brief Class that encapsulates the dialog to specify the perspective settings.
         *
         * There are four predefined perspective settings which can be chosen from this dialog window, see \ref Perspective.
         */
        class PerspectiveSettingDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*! \brief Constructor which creates the Qt dialog window to chose the perspective.
             *
             */
            PerspectiveSettingDialog(QWidget* parent = Q_NULLPTR);

            /*! \brief
             *
             * @return Perspective which was selected.
             */
            Perspective getResult() const;

         private slots:
            /*! If the user made a selection and clicked the apply button, this method reads and stores the selection into member \ref _perspective.
             *
             */
            void accept() Q_DECL_OVERRIDE;

         private:
            /*! Qt combo box which provides the predefined perspectives. */
            QComboBox* _perspectiveCombo;
            Perspective _perspective;
        };

        /*! \brief Class that encapsulates the dialog to specify the background color for the scene view.
         *
         * There are four predefined background colors  which can be chosen from this dialog window, see \ref BackgroundColor.
         */
        class BackgroundColorSettingDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*! \brief Constructor which creates the Qt dialog window to chose the background color.
             *
             */
            BackgroundColorSettingDialog(QWidget* parent = Q_NULLPTR);

            /*! \brief
             *
             * @return BackgroundColor which was selected.
             */
            BackgroundColor getResult() const;

         private slots:
            /*! If the user made a selection and clicked the apply button, this method reads and stores the selection into member \ref _bgc.
             *
             */
            void accept() Q_DECL_OVERRIDE;

         private:
            /*! Qt combo box which provides the predefined perspectives. */
            QComboBox* _bgcCombo;
            BackgroundColor _bgc;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_SETTINGSDIALOGS_HPP_ */

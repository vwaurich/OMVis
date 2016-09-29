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

#ifndef INCLUDE_VIEW_DIALOGS_PERSPECTIVESETTINGDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_PERSPECTIVESETTINGDIALOG_HPP_

#include "View/ViewSettings.hpp"

#include <QDialog>
#include <QComboBox>

#include <memory>

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
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructor which creates the Qt dialog window to chose the perspective.
             *
             */
            PerspectiveSettingDialog(QWidget* parent = Q_NULLPTR);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*! \brief
             *
             * @return Perspective which was selected.
             */
            Perspective getResult() const;

            /*-----------------------------------------
             * SLOTS
             *---------------------------------------*/

         private slots:
            /*! If the user made a selection and clicked the apply button, this method reads and stores the selection into member \ref _perspective.
             *
             */
            void accept() Q_DECL_OVERRIDE;

         private:

            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! Qt combo box which provides the predefined perspectives. */
            std::unique_ptr<QComboBox> _perspectiveCombo;
            Perspective _perspective;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_PERSPECTIVESETTINGDIALOG_HPP_ */

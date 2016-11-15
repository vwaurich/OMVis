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

#ifndef INCLUDE_VIEW_BACKGROUNDCOLORSETTINGDIALOG_HPP_
#define INCLUDE_VIEW_BACKGROUNDCOLORSETTINGDIALOG_HPP_

#include "View/ViewSettings.hpp"

#include <QDialog>
#include <QComboBox>

#include <memory>

namespace OMVIS
{
    namespace View
    {

        /*! \brief Class that encapsulates the dialog to specify the background color for the scene view.
         *
         * There are four predefined background colors  which can be chosen from this dialog window, see \ref BackgroundColor.
         */
        class BackgroundColorSettingDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructor which creates the Qt dialog window to chose the background color.
             *
             */
            BackgroundColorSettingDialog(QWidget* parent = Q_NULLPTR);

            ~BackgroundColorSettingDialog() = default;

            BackgroundColorSettingDialog(const BackgroundColorSettingDialog& rhs) = delete;

            BackgroundColorSettingDialog& operator=(const BackgroundColorSettingDialog& rhs) = delete;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*! \brief
             *
             * @return BackgroundColor which was selected.
             */
            BackgroundColor getResult() const;

            /*-----------------------------------------
             * SLOTS
             *---------------------------------------*/

         private slots:
            /*! \brief If the user made a selection this method reads and stores the selection into member \ref _bgc. */
            void accept() Q_DECL_OVERRIDE;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! Qt combo box which provides the predefined perspectives. */
            std::unique_ptr<QComboBox> _bgcCombo;
            BackgroundColor _bgc;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_BACKGROUNDCOLORSETTINGDIALOG_HPP_ */

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

#include "View/Dialogs/PerspectiveSettingDialog.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>

namespace OMVIS
{
    namespace View
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        PerspectiveSettingDialog::PerspectiveSettingDialog(QWidget* parent)
                : QDialog(parent),
                  _perspectiveCombo(new QComboBox()),
                  _perspective(Perspective::home)
        {
            // Perspective
            _perspectiveCombo->addItem(QString("to home position"));
            _perspectiveCombo->addItem(QString("normal to x-y plane"));
            _perspectiveCombo->addItem(QString("normal to x-z plane"));
            _perspectiveCombo->addItem(QString("normal to y-z plane"));

            QLabel* perspectiveLabel = new QLabel(tr("Set view "));
            QHBoxLayout* perspectiveLayout = new QHBoxLayout();
            perspectiveLayout->addWidget(perspectiveLabel);
            perspectiveLayout->addWidget(_perspectiveCombo.get());

            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            QVBoxLayout* buttonsLayout = new QVBoxLayout();
            buttonsLayout->addStretch(1);
            buttonsLayout->addWidget(buttonBox);

            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addLayout(perspectiveLayout);
            mainLayout->addLayout(buttonsLayout);
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Perspective"));
        }

        /*-----------------------------------------
         * SLOTS
         *---------------------------------------*/
        void PerspectiveSettingDialog::accept()
        {
            // qDebug() << "perspective = " << _perspectiveCombo->currentText() << " " << _perspectiveCombo->currentIndex();
            _perspective = static_cast<Perspective>(_perspectiveCombo->currentIndex());
            QDialog::accept();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        Perspective PerspectiveSettingDialog::getResult() const
        {
            return _perspective;
        }

    }  // End namespace View
 }  // End namespace OMVIS

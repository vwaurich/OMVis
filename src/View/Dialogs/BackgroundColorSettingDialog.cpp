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

#include "View/Dialogs/BackgroundColorSettingDialog.hpp"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>

namespace OMVIS
{
    namespace View
    {
        BackgroundColorSettingDialog::BackgroundColorSettingDialog(QWidget* parent)
                : QDialog(parent),
                  _bgcCombo(new QComboBox())
        {
            // Perspective
            _bgcCombo->addItem(QString("midnight blue"));
            _bgcCombo->addItem(QString("lovely lila"));
            _bgcCombo->addItem(QString("ivory white"));
            _bgcCombo->addItem(QString("froggy green"));

            //X11 QGroupBox* perspectiveGroup = new QGroupBox(tr("View Settings"));
            QLabel* bgcLabel = new QLabel(tr("Set background color of scene view to "));
            QHBoxLayout* bgcLayout = new QHBoxLayout();
            bgcLayout->addWidget(bgcLabel);
            bgcLayout->addWidget(_bgcCombo);
            //X11 perspectiveGroup->setLayout(bgcLayout);

            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            QVBoxLayout* buttonsLayout = new QVBoxLayout();
            buttonsLayout->addStretch(1);
            buttonsLayout->addWidget(buttonBox);

            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            //X11 mainLayout->addWidget(perspectiveGroup);
            mainLayout->addLayout(bgcLayout);
            mainLayout->addLayout(buttonsLayout);
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Perspective"));
        }

        void BackgroundColorSettingDialog::accept()
        {
            // qDebug() << "perspective = " << _perspectiveCombo->currentText() << " " << _perspectiveCombo->currentIndex();
            _bgc = static_cast<BackgroundColor>(_bgcCombo->currentIndex());
            QDialog::accept();
        }

        BackgroundColor BackgroundColorSettingDialog::getResult() const
        {
            return _bgc;
        }

    }  // End namespace View
}  // End namespace OMVIS

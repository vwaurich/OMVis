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

#include "View/Dialogs/OpenFileDialog.hpp"
#include "View/Dialogs/OpenRemoteConnectionDialog.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>

#include <string>

namespace OMVIS
{
    namespace View
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OpenFileDialog::OpenFileDialog(QWidget* parent)
                : QFileDialog(parent, tr("Open Simulation File"), QString(), tr("Visualization FMU(*.fmu);; Visualization MAT(*.mat)")),
                  _modelFile(),
                  _path()
        {
        }

        /*-----------------------------------------
         * SLOTS
         *---------------------------------------*/

        void OpenFileDialog::accept()
        {
            QStringList fileNames;
            QString modelFile;

			//QString fileName = getOpenFileName(this, tr("Choose a Scene Description File"), QString(), tr("Visualization FMU(*.fmu);; Visualization MAT(*.mat)"));
			//std::cout << "fileName " << fileName.toStdString() << std::endl;
		
//            if (exec())
//            {
                fileNames = selectedFiles();


				if (fileNames.size() > 0) {
					modelFile = fileNames.at(0);
				}

//            }

			if (modelFile.isEmpty())
			{
				QMessageBox::warning(0, QString("Information"), QString(" Filename is empty."));
			}

            _path = Util::getPath(modelFile.toStdString());           // /home/user/models/
            _modelFile = Util::getFileName(modelFile.toStdString());  // modelX.fmu
            QDialog::accept();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        Initialization::VisualizationConstructionPlan OpenFileDialog::getConstructionPlan() const
        {
            return Initialization::VisualizationConstructionPlan(_modelFile, _path);
        }

		void OpenFileDialog::getFileNameAndPath()
		{
			_modelFile = getOpenFileName(this, tr("Choose a Scene Description File"), QString(), tr("Visualization FMU(*.fmu);; Visualization MAT(*.mat)")).toStdString();
//			int pos = _modelFile.find_last_of("/\\");
//			_path = _modelFile.substr(0, pos + 1);
//			_modelFile = _modelFile.substr(pos + 1);

			_path = Util::getPath(_modelFile);           // /home/user/models/
			_modelFile = Util::getFileName(_modelFile);  // modelX.fmu
		}

    }  // End namespace View
}  // End namespace OMVIS

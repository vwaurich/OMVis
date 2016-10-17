/*
 * OkCancelHelpButtonBox.hpp
 *
 *  Created on: 17.10.2016
 *      Author: mf
 */

#ifndef INCLUDE_VIEW_DIALOGS_OKCANCELHELPBUTTONBOX_HPP_
#define INCLUDE_VIEW_DIALOGS_OKCANCELHELPBUTTONBOX_HPP_

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>

namespace OMVIS
{
    namespace View
    {

        class OkCancelHelpButtonBox : public QDialog
        {
        Q_OBJECT

         public:
            OkCancelHelpButtonBox(QWidget *parent = Q_NULLPTR)
                : QDialog(),
                  _okCancelHelpButtonLayout(new QVBoxLayout())
            {
                QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help);
                QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
                QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
                QObject::connect(buttonBox, SIGNAL(helpRequested()), this, SLOT(help()));
                _okCancelHelpButtonLayout->addStretch(1);
                _okCancelHelpButtonLayout->addWidget(buttonBox);
            }

            OkCancelHelpButtonBox(OkCancelHelpButtonBox& rhs) = delete;

            OkCancelHelpButtonBox& operator= (const OkCancelHelpButtonBox& rhs) = delete;

            ~OkCancelHelpButtonBox() = default;

         private slots:
            virtual void accept() = 0;
            virtual void help() const = 0;

         protected:
            /*! This layout holds the Ok, Cancel and Help button.
             *
             * If a QWidget inherits this class, these three buttons can be added to the layout by adding this member to the appropriate layout.
             */
            QVBoxLayout* _okCancelHelpButtonLayout;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_OKCANCELHELPBUTTONBOX_HPP_ */

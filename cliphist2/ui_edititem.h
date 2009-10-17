/********************************************************************************
** Form generated from reading ui file 'edititem.ui'
**
** Created: Wed Oct 14 23:43:38 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDITITEM_H
#define UI_EDITITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_EditItem
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EditItem)
    {
        if (EditItem->objectName().isEmpty())
            EditItem->setObjectName(QString::fromUtf8("EditItem"));
        EditItem->resize(400, 300);
        gridLayout = new QGridLayout(EditItem);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(EditItem);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textEdit = new QTextEdit(frame);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setLineWrapMode(QTextEdit::NoWrap);

        gridLayout_2->addWidget(textEdit, 0, 0, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(EditItem);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(EditItem);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditItem, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditItem, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditItem);
    } // setupUi

    void retranslateUi(QDialog *EditItem)
    {
        EditItem->setWindowTitle(QApplication::translate("EditItem", "Edit item", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EditItem);
    } // retranslateUi

};

namespace Ui {
    class EditItem: public Ui_EditItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITITEM_H

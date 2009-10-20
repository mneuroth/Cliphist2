#ifndef EDITITEM_H
#define EDITITEM_H

#include "ui_edititem.h"

#include <QString>
#include <QFont>

class EditItem : public QDialog
{
    Q_OBJECT

public:
    EditItem(QWidget *parent = 0, const QFont & aFont = QFont(), const QString & sText = "");
 
    QString text() const;

private:
    Ui::EditItem ui;
};

#endif // EDITITEM_H

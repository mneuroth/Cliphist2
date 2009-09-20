#include "edititem.h"

#include <QtGui/QTextEdit>

EditItem::EditItem(QWidget *parent, const QString & sText)
: QDialog(parent)
{
    ui.setupUi(this);
    ui.textEdit->setText(sText);
}

QString EditItem::text() const
{
    return ui.textEdit->toPlainText();
}

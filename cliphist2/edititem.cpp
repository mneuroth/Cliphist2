#include "edititem.h"

#include <QtGui/QTextEdit>

EditItem::EditItem(QWidget *parent, const QFont & aFont, const QString & sText)
: QDialog(parent)
{
    ui.setupUi(this);
    ui.textEdit->setText(sText);
    ui.textEdit->setFont(aFont);
}

QString EditItem::text() const
{
    return ui.textEdit->toPlainText();
}

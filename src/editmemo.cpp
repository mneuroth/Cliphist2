#include "editmemo.h"
#include "ui_editmemo.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

EditMemo::EditMemo(QWidget *parent, const QFont & aFont, const QString & sText)
    : QDialog(parent)
    , ui(new Ui::EditMemo)
{
    ui->setupUi(this);
    ui->textEdit->setText(sText);
    ui->textEdit->setFont(aFont);
}

EditMemo::~EditMemo()
{
    delete ui;
}

QString EditMemo::getMemo() const
{
    return ui->textEdit->toPlainText();
}

void EditMemo::OnLoad()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open text file"), QDir::homePath(), tr("Textfiles (*.txt);;Alle Files (*)"));

    if (sFileName.isEmpty())
    {
        return;
    }

    QFile aFile(sFileName);
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream aStream(&aFile);
    QString content = aStream.readAll();
    aFile.close();

    ui->textEdit->setText(content);
}

void EditMemo::OnSave()
{
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save text file"), QDir::homePath(), tr("Textfiles (*.txt);;Alle Files (*)"));

    if (sFileName.isEmpty())
    {
        return;
    }

    QFile aFile(sFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream aStream(&aFile);
    aStream << getMemo();
    aFile.close();
}
